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
 * @file <common/control_interface/neural_networks/perceptron_controller.h>
 *
 * @brief This file provides the definition of the perceptron controller
 * 
 * @author Vito Trianni - <vito.trianni@istc.cnr.it>
 */


#ifndef _PERCEPTRONCONTROLLER_H_
#define _PERCEPTRONCONTROLLER_H_

#include "neural_network_controller.h"

using namespace argos;
using namespace std;

class CSingleLayerFeedForwardController : public CNeuralNetworkController {
private:

  UInt32   m_unNumberOfHiddenNodes;
  UInt32   m_unNumberOfWeights;
  Real*    m_pfWeights;
  Real*    m_pfHiddens;
  
public:
  CSingleLayerFeedForwardController();
  virtual ~CSingleLayerFeedForwardController();
  
  virtual void   Init( TConfigurationNode& t_tree );
  virtual void   Destroy();

  virtual void   LoadNetworkParameters( const string filename );
  virtual void   LoadNetworkParameters( const UInt32 un_num_params, const Real* pf_params );
  virtual void   ComputeOutputs( void );  

  virtual inline UInt32 GetNumberOfParameters( void ) { return m_unNumberOfWeights;};
};


#endif
