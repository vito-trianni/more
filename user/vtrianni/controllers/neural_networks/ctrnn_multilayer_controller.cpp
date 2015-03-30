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
 * @file <common/control_interface/ctrnn_multilayer_controller.cpp>
 *
 * @brief This file provides the definition of the CTRNN multilayer
 * neural network controller
 * 
 * @author Vito Trianni - <vito.trianni@istc.cnr.it>
 */

#include "ctrnn_multilayer_controller.h"

#include <math.h>
#include <fstream>
#include <argos2/common/utility/string_utilities.h>

using namespace argos;

const string CONFIGURATION_CTRNN_NUMHIDDEN = "num_hidden";
const string CONFIGURATION_CTRNN_TIMESTEP  = "integration_step";
const string CONFIGURATION_CTRNN_WEIGHTS   = "weight_range";
const string CONFIGURATION_CTRNN_BIASES    = "bias_range";
const string CONFIGURATION_CTRNN_TAUS      = "tau_range";


CCtrnnMultilayerController::CCtrnnMultilayerController() :
   m_unNumberOfHiddenNodes( 0 ),
   m_unNumParameters( 0 ),
   m_fTimeStep( 0.1 ),
   m_pfInputToHiddenWeights(NULL),
   m_pfHiddenToHiddenWeights(NULL),
   m_pfHiddenToOutputWeights(NULL),
   m_pfHiddenBiases(NULL),
   m_pfHiddenTaus(NULL),
   m_pfHiddenDeltaStates(NULL),
   m_pfHiddenStates(NULL),
   m_pfHiddenSigmaStates(NULL),
   m_pfOutputBiases(NULL),
   m_cWeightsBounds(-4.0,4.0),
   m_cBiasesBounds(-4.0,4.0),
   m_cTausBounds(-1.0,3.0)
{}


/****************************************/
/****************************************/

CCtrnnMultilayerController::~CCtrnnMultilayerController() {
   if( m_pfInputToHiddenWeights )  delete[] m_pfInputToHiddenWeights;
   if( m_pfHiddenToHiddenWeights ) delete[] m_pfHiddenToHiddenWeights;
   if( m_pfHiddenToOutputWeights ) delete[] m_pfHiddenToOutputWeights;

   if( m_pfHiddenBiases )          delete[] m_pfHiddenBiases;
   if( m_pfHiddenTaus )            delete[] m_pfHiddenTaus;
   if( m_pfHiddenDeltaStates )     delete[] m_pfHiddenDeltaStates;
   if( m_pfHiddenStates )          delete[] m_pfHiddenStates;
   if( m_pfHiddenSigmaStates )     delete[] m_pfHiddenSigmaStates;

   if( m_pfOutputBiases )          delete[] m_pfOutputBiases;
}


/****************************************/
/****************************************/

void CCtrnnMultilayerController::Init( TConfigurationNode& t_node ) {
   ////////////////////////////////////////////////////////////////////////////////
   // First perform common initialisation from base class
   ////////////////////////////////////////////////////////////////////////////////
   CNeuralNetworkController::Init( t_node );
  
   ////////////////////////////////////////////////////////////////////////////////
   // Load number of hidden nodes
   ////////////////////////////////////////////////////////////////////////////////
   try{
      GetNodeAttribute(t_node, CONFIGURATION_CTRNN_NUMHIDDEN, m_unNumberOfHiddenNodes);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Missing required tag <" << CONFIGURATION_CTRNN_NUMHIDDEN << ">" 
				  << " for CTRNN multi-layer initialisation", ex);
   }
  

   ////////////////////////////////////////////////////////////////////////////////
   // Compute number of parameters
   ////////////////////////////////////////////////////////////////////////////////
   m_unNumParameters = 
      m_unNumberOfHiddenNodes * (m_unNumberOfInputs + 1)  + // weights from inputs to hidden + hidden bias
      m_unNumberOfHiddenNodes * m_unNumberOfHiddenNodes   + // weigths from hidden to hidden
      m_unNumberOfOutputs * (m_unNumberOfHiddenNodes + 1) + // weights from hidden to output + hidden bias
      m_unNumberOfHiddenNodes;                              // taus



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
   if( m_pfInputToHiddenWeights == NULL ) m_pfInputToHiddenWeights = new Real[m_unNumberOfInputs * m_unNumberOfHiddenNodes];
   if( m_pfHiddenToHiddenWeights == NULL ) m_pfHiddenToHiddenWeights = new Real[m_unNumberOfHiddenNodes * m_unNumberOfHiddenNodes];
   if( m_pfHiddenBiases == NULL ) m_pfHiddenBiases = new Real[m_unNumberOfHiddenNodes];
   if( m_pfHiddenTaus == NULL ) m_pfHiddenTaus = new Real[m_unNumberOfHiddenNodes];
   if( m_pfHiddenToOutputWeights == NULL ) m_pfHiddenToOutputWeights = new Real[m_unNumberOfHiddenNodes * m_unNumberOfOutputs];
   if( m_pfOutputBiases == NULL ) m_pfOutputBiases = new Real[m_unNumberOfOutputs];
   if( m_pfHiddenDeltaStates == NULL) m_pfHiddenDeltaStates  = new Real[m_unNumberOfHiddenNodes];
   if( m_pfHiddenStates == NULL ) m_pfHiddenStates = new Real[m_unNumberOfHiddenNodes];
   if( m_pfHiddenSigmaStates == NULL ) m_pfHiddenSigmaStates = new Real[m_unNumberOfHiddenNodes];

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
   for( UInt32 i = 0; i < m_unNumberOfHiddenNodes; i++ ) {
      m_pfHiddenDeltaStates[i] = 0.0;
      m_pfHiddenStates[i]      = 0.0;
      m_pfHiddenSigmaStates[i] = 1.0/(exp(-(m_pfHiddenStates[i] + m_pfHiddenBiases[i])) + 1.0);
   }
}

/****************************************/
/****************************************/

void CCtrnnMultilayerController::Reset() {
   for( UInt32 i = 0; i < m_unNumberOfHiddenNodes; i++ ) {
      m_pfHiddenDeltaStates[i] = 0.0;
      m_pfHiddenStates[i]      = 0.0;
      m_pfHiddenSigmaStates[i] = 1.0/(exp(-(m_pfHiddenStates[i] + m_pfHiddenBiases[i])) + 1.0);
   }
}


/****************************************/
/****************************************/

void CCtrnnMultilayerController::Destroy() {
   CNeuralNetworkController::Destroy();

   m_unNumberOfHiddenNodes = 0;

   if( m_pfInputToHiddenWeights )  delete[] m_pfInputToHiddenWeights;
   m_pfInputToHiddenWeights = NULL;
   
   if( m_pfHiddenToHiddenWeights ) delete[] m_pfHiddenToHiddenWeights;
   m_pfHiddenToHiddenWeights = NULL;
   
   if( m_pfHiddenToOutputWeights ) delete[] m_pfHiddenToOutputWeights;
   m_pfHiddenToOutputWeights = NULL;

   if( m_pfHiddenBiases )          delete[] m_pfHiddenBiases;
   m_pfHiddenBiases = NULL;
   
   if( m_pfHiddenTaus )            delete[] m_pfHiddenTaus;
   m_pfHiddenTaus = NULL;
   
   if( m_pfHiddenDeltaStates )     delete[] m_pfHiddenDeltaStates;
   m_pfHiddenDeltaStates = NULL;

   if( m_pfHiddenStates )          delete[] m_pfHiddenStates;
   m_pfHiddenStates = NULL;

   if( m_pfHiddenSigmaStates )     delete[] m_pfHiddenSigmaStates;
   m_pfHiddenSigmaStates = NULL;

   if( m_pfOutputBiases )          delete[] m_pfOutputBiases;
   m_pfOutputBiases = NULL;
}


/****************************************/
/****************************************/

void CCtrnnMultilayerController::LoadNetworkParameters( const string filename ) {
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

void CCtrnnMultilayerController::LoadNetworkParameters( const UInt32 un_num_params, const Real* params ) {
   // check consistency between paramter file and xml declaration  
   if( un_num_params != m_unNumParameters ) {
      THROW_ARGOSEXCEPTION("Number of paramter mismatch: '" 
			   << "passed " << un_num_params << " parameters, while " 
			   << m_unNumParameters << " were expected from the xml configuration file");
   }

   UInt32 unChromosomePosition = 0;
  
   for( UInt32 i = 0; i < m_unNumberOfInputs * m_unNumberOfHiddenNodes; i++ ) {
      m_pfInputToHiddenWeights[i] = params[unChromosomePosition++]*m_cWeightsBounds.GetSpan() + m_cWeightsBounds.GetMin();
   }

   for( UInt32 i = 0; i < m_unNumberOfHiddenNodes * m_unNumberOfHiddenNodes; i++ ) {
      m_pfHiddenToHiddenWeights[i] = params[unChromosomePosition++]*m_cWeightsBounds.GetSpan() + m_cWeightsBounds.GetMin();
   }

   for( UInt32 i = 0; i < m_unNumberOfHiddenNodes; i++ ) {
      m_pfHiddenBiases[i] = params[unChromosomePosition++]*m_cBiasesBounds.GetSpan() + m_cBiasesBounds.GetMin();
   }

   for( UInt32 i = 0; i < m_unNumberOfHiddenNodes; i++ ) {
      m_pfHiddenTaus[i] = pow(10, params[unChromosomePosition++]*m_cTausBounds.GetSpan() + m_cTausBounds.GetMin());
   }

   for( UInt32 i = 0; i < m_unNumberOfHiddenNodes * m_unNumberOfOutputs; i++ ) {
      m_pfHiddenToOutputWeights[i] = params[unChromosomePosition++]*m_cWeightsBounds.GetSpan() + m_cWeightsBounds.GetMin();
   }

   for( UInt32 i = 0; i < m_unNumberOfOutputs; i++ ) {
      m_pfOutputBiases[i] = params[unChromosomePosition++]*m_cBiasesBounds.GetSpan() + m_cBiasesBounds.GetMin();
   }
}



/****************************************/
/****************************************/

void CCtrnnMultilayerController::ComputeOutputs( void ) {
   // Equation of the CTRNN dynamics:
   // dy_i/dt \tau_i = -y_i +                                        # leaky integration
   //                  \sum_j WI_{j,i}*I_j +                         # weighted contribution from inputs
   //                  \sum_j WH_{j,i}\sigma(y_j + \beta_j)          # weighted contribution from hidden
   
   // Update delta state of hidden layer from inputs:
   for( UInt32 i = 0; i < m_unNumberOfHiddenNodes; i++ ) {
      // leaky integration
      m_pfHiddenDeltaStates[i] = -m_pfHiddenStates[i]; 
    
      // weighted contribution from inputs
      for( UInt32 j = 0; j < m_unNumberOfInputs; j++ ) {
	 m_pfHiddenDeltaStates[i] += m_pfInputToHiddenWeights[i * m_unNumberOfInputs + j] * m_pfInputs[j] ;	  
      }	  

      // weighted contribution from hidden
      for( UInt32 j = 0; j < m_unNumberOfHiddenNodes; j++ ) {
	 m_pfHiddenDeltaStates[i] += m_pfHiddenToHiddenWeights[i * m_unNumberOfHiddenNodes + j] * m_pfHiddenSigmaStates[j];
      }
   }
    

   // once all delta state are computed, get the new activation for the hidden unit
   for( UInt32  i = 0; i < m_unNumberOfHiddenNodes; i++ ) {
      m_pfHiddenStates[i]     += m_pfHiddenDeltaStates[i] * m_fTimeStep/m_pfHiddenTaus[i];
      m_pfHiddenSigmaStates[i] = 1.0/(exp(-(m_pfHiddenStates[i] + m_pfHiddenBiases[i])) + 1.0 );
   }


   // Update the outputs layer
   for( UInt32 i = 0; i < m_unNumberOfOutputs; i++ ) {
      // Initialise to 0
      m_pfOutputs[i] = 0.0;
    
      // Sum over all the hidden nodes
      for( UInt32 j = 0; j < m_unNumberOfHiddenNodes; j++ ) {
	 m_pfOutputs[i] += m_pfHiddenToOutputWeights[i * m_unNumberOfHiddenNodes + j] * m_pfHiddenSigmaStates[j];
      }

      // Compute the activation function immediately, since this is
      // what we return and since the output layer is not recurrent:
      m_pfOutputs[i] = 1.0/(exp(-( m_pfOutputs[i] + m_pfOutputBiases[i])) + 1.0);
   }
}

