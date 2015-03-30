/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file <common/control_interface/ctrnn_perceptron_controller.cpp>
 *
 * @brief This file provides the definition of the CTRNN perceptron
 * neural network controller
 * 
 * @author Vito Trianni - <vito.trianni@istc.cnr.it>
 */

#include "ctrnn_perceptron_controller.h"

#include <math.h>
#include <fstream>
#include <argos2/common/utility/string_utilities.h>

using namespace argos;

const string CONFIGURATION_CTRNN_TIMESTEP  = "integration_step";
const string CONFIGURATION_CTRNN_WEIGHTS   = "weight_range";
const string CONFIGURATION_CTRNN_BIASES    = "bias_range";
const string CONFIGURATION_CTRNN_TAUS      = "tau_range";


CCtrnnPerceptronController::CCtrnnPerceptronController() :
   m_unNumParameters( 0 ),
   m_fTimeStep( 0.1 ),
   m_pfInputToOutputWeights(NULL),
   m_pfOutputToOutputWeights(NULL),
   m_pfOutputBiases(NULL),
   m_pfOutputTaus(NULL),
   m_pfOutputDeltaStates(NULL),
   m_pfOutputStates(NULL),
   m_cWeightsBounds(-5,5),
   m_cBiasesBounds(-5,5),
   m_cTausBounds(-1,3)
{}


/****************************************/
/****************************************/

CCtrnnPerceptronController::~CCtrnnPerceptronController() {
   if( m_pfInputToOutputWeights )  delete[] m_pfInputToOutputWeights;
   if( m_pfOutputToOutputWeights ) delete[] m_pfOutputToOutputWeights;

   if( m_pfOutputBiases )          delete[] m_pfOutputBiases;
   if( m_pfOutputTaus )            delete[] m_pfOutputTaus;
   if( m_pfOutputDeltaStates )     delete[] m_pfOutputDeltaStates;
   if( m_pfOutputStates )          delete[] m_pfOutputStates;
}


/****************************************/
/****************************************/

void CCtrnnPerceptronController::Init( TConfigurationNode& t_node ) {
   ////////////////////////////////////////////////////////////////////////////////
   // First perform common initialisation from base class
   ////////////////////////////////////////////////////////////////////////////////
   CNeuralNetworkController::Init( t_node );
  
   ////////////////////////////////////////////////////////////////////////////////
   // Compute number of parameters
   ////////////////////////////////////////////////////////////////////////////////
   m_unNumParameters = 
      m_unNumberOfOutputs * m_unNumberOfInputs        + // weights from inputs to output
      m_unNumberOfOutputs * m_unNumberOfOutputs   + // weigths from output to output
      m_unNumberOfOutputs * 2;                          // taus and output bias


   ////////////////////////////////////////////////////////////////////////////////
   // integration step
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttributeOrDefault(t_node, CONFIGURATION_CTRNN_TIMESTEP, m_fTimeStep, m_fTimeStep);


   ////////////////////////////////////////////////////////////////////////////////
   // Load upper and lower bounds for weigths, biases and taus
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttributeOrDefault(t_node, CONFIGURATION_CTRNN_WEIGHTS, m_cWeightsBounds, m_cWeightsBounds);
   GetNodeAttributeOrDefault(t_node, CONFIGURATION_CTRNN_BIASES,  m_cBiasesBounds, m_cBiasesBounds);
   GetNodeAttributeOrDefault(t_node, CONFIGURATION_CTRNN_TAUS,    m_cTausBounds, m_cTausBounds);


   ////////////////////////////////////////////////////////////////////////////////
   // allocate memory for the parameters and internal variables
   ////////////////////////////////////////////////////////////////////////////////
   if( m_pfInputToOutputWeights == NULL ) m_pfInputToOutputWeights = new Real[m_unNumberOfInputs * m_unNumberOfOutputs];
   if( m_pfOutputToOutputWeights == NULL ) m_pfOutputToOutputWeights = new Real[m_unNumberOfOutputs * m_unNumberOfOutputs];
   if( m_pfOutputBiases == NULL ) m_pfOutputBiases = new Real[m_unNumberOfOutputs];
   if( m_pfOutputTaus == NULL ) m_pfOutputTaus = new Real[m_unNumberOfOutputs];
   if( m_pfOutputDeltaStates == NULL) m_pfOutputDeltaStates  = new Real[m_unNumberOfOutputs];
   if( m_pfOutputStates == NULL ) m_pfOutputStates = new Real[m_unNumberOfOutputs];


   ////////////////////////////////////////////////////////////////////////////////
   // check and load parameters from file
   ////////////////////////////////////////////////////////////////////////////////
   if( m_sParamterFile != "" ) {
      try {
	 LoadNetworkParameters( m_sParamterFile );
      }
      catch(CARGoSException& ex) {
	 THROW_ARGOSEXCEPTION_NESTED("cannot load parameters from file.", ex);
      }
   }


   ////////////////////////////////////////////////////////////////////////////////
   // initialise internal variables
   ////////////////////////////////////////////////////////////////////////////////
   for( UInt32 i = 0; i < m_unNumberOfOutputs; i++ ) {
      m_pfOutputDeltaStates[i] = 0.0;
      m_pfOutputStates[i] = 0.0;
      m_pfOutputs[i] = 1.0/(exp(-(m_pfOutputStates[i] + m_pfOutputBiases[i])) + 1.0);
   }
}

/****************************************/
/****************************************/

void CCtrnnPerceptronController::Reset() {
   for( UInt32 i = 0; i < m_unNumberOfOutputs; i++ ) {
      m_pfOutputDeltaStates[i] = 0.0;
      m_pfOutputStates[i] = 0.0;
      m_pfOutputs[i] = 1.0/(exp(-(m_pfOutputStates[i] + m_pfOutputBiases[i])) + 1.0);
   }
}


/****************************************/
/****************************************/

void CCtrnnPerceptronController::Destroy() {
   CNeuralNetworkController::Destroy();
   
   m_unNumberOfOutputs = 0;

   if( m_pfInputToOutputWeights )  delete[] m_pfInputToOutputWeights;
   m_pfInputToOutputWeights = NULL;
   
   if( m_pfOutputToOutputWeights ) delete[] m_pfOutputToOutputWeights;
   m_pfOutputToOutputWeights = NULL;
   
   if( m_pfOutputBiases )          delete[] m_pfOutputBiases;
   m_pfOutputBiases = NULL;
   
   if( m_pfOutputTaus )            delete[] m_pfOutputTaus;
   m_pfOutputTaus = NULL;
   
   if( m_pfOutputDeltaStates )     delete[] m_pfOutputDeltaStates;
   m_pfOutputDeltaStates = NULL;

   if( m_pfOutputStates )          delete[] m_pfOutputStates;
   m_pfOutputStates = NULL;

}


/****************************************/
/****************************************/

void CCtrnnPerceptronController::LoadNetworkParameters( const string filename ) {
  ifstream in( filename.c_str(), ios::in );
   if( !in ) {
      THROW_ARGOSEXCEPTION("Cannot open parameter file '" << filename << "' for reading");
   }

   // first parameter is the number of real-valued weights
   UInt32 un_length = 0;
   if( !(in >> un_length) ) {
      THROW_ARGOSEXCEPTION("Cannot read number of data from file '" << filename << "'");
   }

  
   // create weights vector and load it from file
   Real* pfGenes = new Real[un_length];
   for( UInt32 i = 0; i < un_length; i++ ) {
      if( !(in >> pfGenes[i]) ) {
	 delete pfGenes;
	 THROW_ARGOSEXCEPTION("Cannot read data from file '" << filename << "'");
      }
   }

   // load parameters in the appropriate structures
   LoadNetworkParameters( un_length, pfGenes );
   delete [] pfGenes;
}


/****************************************/
/****************************************/

void CCtrnnPerceptronController::LoadNetworkParameters( const UInt32 un_num_params, const Real* params ) {
   // check consistency between paramter file and xml declaration  
   if( un_num_params != m_unNumParameters ) {
      THROW_ARGOSEXCEPTION("Number of paramter mismatch: '" 
			   << "passed " << un_num_params << " parameters, while " 
			   << m_unNumParameters << " were expected from the xml configuration file");
   }

   UInt32 unChromosomePosition = 0;
  
   for( UInt32 i = 0; i < m_unNumberOfInputs * m_unNumberOfOutputs; i++ ) {
      m_pfInputToOutputWeights[i] = params[unChromosomePosition++]*m_cWeightsBounds.GetSpan() + m_cWeightsBounds.GetMin();
   }

   for( UInt32 i = 0; i < m_unNumberOfOutputs * m_unNumberOfOutputs; i++ ) {
      m_pfOutputToOutputWeights[i] = params[unChromosomePosition++]*m_cWeightsBounds.GetSpan() + m_cWeightsBounds.GetMin();
   }

   for( UInt32 i = 0; i < m_unNumberOfOutputs; i++ ) {
      m_pfOutputBiases[i] = params[unChromosomePosition++]*m_cBiasesBounds.GetSpan() + m_cBiasesBounds.GetMin();
      m_pfOutputTaus[i] = pow(10, params[unChromosomePosition++]*m_cTausBounds.GetSpan() + m_cTausBounds.GetMin());
   }
}



/****************************************/
/****************************************/

void CCtrnnPerceptronController::ComputeOutputs( void ) {
   // Equation of the CTRNN dynamics:
   // dy_i/dt \tau_i = -y_i +                                        # leaky integration
   //                  \sum_j WI_{j,i}*I_j +                         # weighted contribution from inputs
   //                  \sum_j WH_{j,i}\sigma(y_j + \beta_j)          # weighted contribution from output
   
   // Update delta state of output layer from inputs:
   for( UInt32 i = 0; i < m_unNumberOfOutputs; i++ ) {
      // leaky integration
      m_pfOutputDeltaStates[i] = -m_pfOutputStates[i]; 
    
      // weighted contribution from inputs
      for( UInt32 j = 0; j < m_unNumberOfInputs; j++ ) {
	 m_pfOutputDeltaStates[i] += m_pfInputToOutputWeights[i * m_unNumberOfInputs + j] * m_pfInputs[j] ;	  
      }	  

      // weighted contribution from output
      for( UInt32 j = 0; j < m_unNumberOfOutputs; j++ ) {
	 m_pfOutputDeltaStates[i] += m_pfOutputToOutputWeights[i * m_unNumberOfOutputs + j] * m_pfOutputs[j];
      }
   }
    

   // once all delta state are computed, get the new activation for the output unit
   for( UInt32  i = 0; i < m_unNumberOfOutputs; i++ ) {
      m_pfOutputStates[i] += m_pfOutputDeltaStates[i] * m_fTimeStep/m_pfOutputTaus[i];
      m_pfOutputs[i]       = 1.0/(exp(-(m_pfOutputStates[i] + m_pfOutputBiases[i])) + 1.0 );
   }
}

