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
 * @file <common/control_interface/perceptron_controller.h>
 *
 * @brief This file provides the definition of the neural network controller
 * 
 * @author Vito Trianni - <vito.trianni@istc.cnr.it>
 */


#include <fstream>
#include <math.h>

#include "perceptron_controller.h"

using namespace argos;

CPerceptronController::CPerceptronController() {
  m_unNumberOfWeights = 0;
  m_pfWeights = NULL;
}


/****************************************/
/****************************************/

CPerceptronController::~CPerceptronController() {
  if( m_pfWeights ) delete [] m_pfWeights;
}


/****************************************/
/****************************************/

void CPerceptronController::Init( TConfigurationNode& t_tree ) {
  /* First perform common initialisation from base class */
  CNeuralNetworkController::Init( t_tree );
  
  /* set the number of weights */
  m_unNumberOfWeights = (m_unNumberOfInputs + 1)*(m_unNumberOfOutputs);

  /* load parameters from file */
  if( m_sParamterFile != "" ) {
    try{
      LoadNetworkParameters( m_sParamterFile );
    }
    catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("cannot load parameters from file.", ex);
    }
  }
}

/****************************************/
/****************************************/

void CPerceptronController::Destroy() {
  if( m_pfWeights ) delete [] m_pfWeights;
  m_pfWeights = NULL;
  m_unNumberOfWeights = 0;  
}


/****************************************/
/****************************************/

void CPerceptronController::LoadNetworkParameters( const string filename ) {
  
  // open the input file
  ifstream in( filename.c_str(), ios::in );
  if( !in ) {
    THROW_ARGOSEXCEPTION("Cannot open parameter file '" << filename << "' for reading");
  }

  // first parameter is the number of real-valued weights
  unsigned int un_length = 0;
  if( !(in >> un_length) ) {
    THROW_ARGOSEXCEPTION("Cannot read data from file '" << filename << "'");
  }

  // check consistency between paramter file and xml declaration
  if( un_length != m_unNumberOfWeights ) {
    THROW_ARGOSEXCEPTION("Number of paramter mismatch: '" 
			 << filename << "' contains " << un_length << " parameters, while " 
			 << m_unNumberOfWeights << " were expected from the xml configuration file");
  }


  // create weights vector and load it from file
  if( m_pfWeights == NULL ) m_pfWeights = new Real[m_unNumberOfWeights];
  for( unsigned int i = 0; i < m_unNumberOfWeights; ++i ) {
    if( !(in >> m_pfWeights[i] ) ) {
      THROW_ARGOSEXCEPTION("Cannot read data from file '" << filename << "'");
    }
  }
}


/****************************************/
/****************************************/

void CPerceptronController::LoadNetworkParameters( const unsigned int un_num_params, const Real* params ) {
  // check consistency between parameters and xml declaration
  if( un_num_params != m_unNumberOfWeights ) {
    THROW_ARGOSEXCEPTION("Number of paramter mismatch: '" 
			 << "passed " << un_num_params << " parameters, while " 
			 << m_unNumberOfWeights << " were expected from the xml configuration file");
  }

  // create weights vector and load it from file
  if( m_pfWeights == NULL ) m_pfWeights = new Real[m_unNumberOfWeights];
  for( unsigned int i = 0; i < m_unNumberOfWeights; ++i ) {
    m_pfWeights[i] = params[i];
  }
}



/****************************************/
/****************************************/

void CPerceptronController::ComputeOutputs( void ) {
  for( unsigned int i = 0; i < m_unNumberOfOutputs; ++i ) {
    // Add the bias (weighted by the first weight to the i'th output node)
    m_pfOutputs[i] = m_pfWeights[i * (m_unNumberOfInputs + 1)];
    
    for( unsigned int j = 0; j < m_unNumberOfInputs; ++j ) {
      // Compute the weight number
      unsigned int ji = i * (m_unNumberOfInputs + 1) + (j + 1);
      // Add the weighted input
      m_pfOutputs[i] += m_pfWeights[ji] * m_pfInputs[j];
      
    }
    
    // Apply the transfer function (sigmoid with output in [0,1])
    m_pfOutputs[i] = 1.0/( 1 + exp( -m_pfOutputs[i]) );
  }
}


