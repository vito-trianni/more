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
 * @file <common/control_interface/ctrnn_perceptron_controller.h>
 *
 * @brief This file provides the definition of the CTRNN perceptron
 * neural network controller
 * 
 * @author Vito Trianni - <vito.trianni@istc.cnr.it>
 */


#ifndef CTRNNPERCEPTRONCONTROLLER_H_
#define CTRNNPERCEPTRONCONTROLLER_H_

#include "neural_network_controller.h"
#include <argos2/common/utility/math/range.h>

using namespace argos;
using namespace std;

class CCtrnnPerceptronController : public CNeuralNetworkController {
protected:
  
   UInt32        m_unNumParameters;
   Real          m_fTimeStep;

   Real*         m_pfInputToOutputWeights;
   Real*         m_pfOutputToOutputWeights;
    
   Real*         m_pfOutputBiases;
   Real*         m_pfOutputTaus;
   Real*         m_pfOutputDeltaStates;
   Real*         m_pfOutputStates;

   CRange<Real>  m_cWeightsBounds;
   CRange<Real>  m_cBiasesBounds;
   CRange<Real>  m_cTausBounds;

public:
   CCtrnnPerceptronController();
   virtual ~CCtrnnPerceptronController();

   virtual void Init( TConfigurationNode& t_tree );
   virtual void Reset( void );
   virtual void Destroy( void );

   virtual void LoadNetworkParameters( const string filename );
   virtual void LoadNetworkParameters( const UInt32 un_num_params, const Real* pf_params );
   virtual void ComputeOutputs( void );

   virtual inline UInt32 GetNumberOfParameters( void ) { return m_unNumParameters; };

   inline  const Real*   GetOutputStates( void )        { return m_pfOutputStates;        };
   inline  const Real*   GetOutputTaus( void )          { return m_pfOutputTaus;          };
   inline  const Real*   GetOutputBias( void )          { return m_pfOutputBiases;        };
};


#endif
