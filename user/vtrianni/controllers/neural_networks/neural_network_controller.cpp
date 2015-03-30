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
 * @file <common/control_interface/neural_network_controller.h>
 *
 * @brief This file provides the definition of the neural network controller
 * 
 * @author Vito Trianni - <vito.trianni@istc.cnr.it>
 */

#include "neural_network_controller.h"
#include <argos2/common/utility/string_utilities.h>


const string NEURAL_NETWORK_CONTROLLER_PARAMTER_FILE   = "parameter_file";


CNeuralNetworkController::CNeuralNetworkController() {
  m_unNumberOfInputs  = 0;
  m_unNumberOfOutputs = 0;
  m_pfInputs          = NULL;
  m_pfOutputs         = NULL;

  m_sParamterFile     = "";
}


/****************************************/
/****************************************/

CNeuralNetworkController::~CNeuralNetworkController() {
  if( m_pfInputs )  delete [] m_pfInputs;
  if( m_pfOutputs ) delete [] m_pfOutputs;
}


/****************************************/
/****************************************/

void CNeuralNetworkController::Init( TConfigurationNode& t_node ) {

  // Get the number of inputs, and initialise the input vector
  try {
    GetNodeAttribute(t_node, "num_inputs", m_unNumberOfInputs);
  }
  catch(CARGoSException& ex) {
    THROW_ARGOSEXCEPTION_NESTED("missing number of inputs for the neural network controller.", ex);
  }

  m_pfInputs = new Real[m_unNumberOfInputs];
  for( UInt32 i = 0; i < m_unNumberOfInputs; ++i ) {
    m_pfInputs[i] = 0.0;
  }


  // Get the number of outputs, and initialise the output vector
  try {
    GetNodeAttribute(t_node, "num_outputs", m_unNumberOfOutputs);
  }
  catch(CARGoSException& ex) {
    THROW_ARGOSEXCEPTION_NESTED("missing number of outputs for the neural network controller.", ex);
  }

  m_pfOutputs = new Real[m_unNumberOfOutputs];
  for( UInt32 i = 0; i < m_unNumberOfOutputs; ++i ) {
    m_pfOutputs[i] = 0.0;
  }


  // name of the parameter file
  GetNodeAttributeOrDefault( t_node, NEURAL_NETWORK_CONTROLLER_PARAMTER_FILE, m_sParamterFile, m_sParamterFile );
}


/****************************************/
/****************************************/

void CNeuralNetworkController::Reset( void ) {
  // reset the input vector
  for( UInt32 i = 0; i < m_unNumberOfInputs; ++i ) {
    m_pfInputs[i] = 0.0;
  }


  // reset the output vector
  for( UInt32 i = 0; i < m_unNumberOfOutputs; ++i ) {
    m_pfOutputs[i] = 0.0;
  }
}


/****************************************/
/****************************************/

void CNeuralNetworkController::Destroy( void ) {
  if( m_pfInputs )  delete [] m_pfInputs;
  m_pfInputs = NULL;
  m_unNumberOfInputs = 0;

  if( m_pfOutputs ) delete [] m_pfOutputs;
  m_pfOutputs = NULL;
  m_unNumberOfOutputs = 0;
}


/****************************************/
/****************************************/

void CNeuralNetworkController::SetOnlineParameters( const UInt32 un_num_params, const Real* pf_params ) {
  LoadNetworkParameters( un_num_params, pf_params );
}
