/* -*- Mode: C++ -*- 
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


#ifndef _NEURALNETWORKCONTROLLER_H_
#define _NEURALNETWORKCONTROLLER_H_

#include <argos2/common/control_interface/ci_controller.h>

using namespace argos;
using namespace std;

class CNeuralNetworkController {
protected:
  
  UInt32   m_unNumberOfInputs;
  UInt32   m_unNumberOfOutputs;
  
  Real*    m_pfInputs;
  Real*    m_pfOutputs;
  
  string   m_sParamterFile;
  
public:
  CNeuralNetworkController();
  virtual ~CNeuralNetworkController();
  
  virtual void          Init( TConfigurationNode& t_node );
  virtual void          Reset( void );
  virtual void          Destroy( void );

  virtual void          LoadNetworkParameters( const string filename ) = 0;
  virtual void          LoadNetworkParameters( const UInt32 un_num_params, const Real* params ) = 0;
  virtual void          ComputeOutputs( void ) = 0;
  virtual UInt32        GetNumberOfParameters( void ) = 0;

  inline  UInt32  GetNumberOfInputs( void ) { return m_unNumberOfInputs; };
  inline  void          SetNumberOfInputs( UInt32 un_inputs ) { m_unNumberOfInputs = un_inputs; };
  inline  const Real*   GetInputs( void ) { return m_pfInputs; };
  void                  SetInput( UInt32 un_input_num, Real f_input_value ) {m_pfInputs[un_input_num] = f_input_value;};
  void                  SetInputRange( UInt32 un_input_start, UInt32 un_num_values, Real* pf_input_values ) {memcpy(m_pfInputs+un_input_start,pf_input_values,un_num_values);};
  
  inline  UInt32  GetNumberOfOutputs( void ) { return m_unNumberOfOutputs; };
  inline  void          SetNumberOfOutputs( UInt32 un_outputs ) { m_unNumberOfOutputs = un_outputs; };
  inline  const Real*   GetOutputs( void ) { return m_pfOutputs; };
  inline  Real          GetOutput( UInt32 un_num_output ) { return m_pfOutputs[un_num_output]; };
  
  virtual void          SetOnlineParameters( const UInt32 un_num_params, const Real* pf_params );
};

#endif
