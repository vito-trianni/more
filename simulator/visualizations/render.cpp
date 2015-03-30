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
 * @file <simulator/visualisations/render.cpp>
 *
 * @brief This file contains the definition of the CRender interface.
 *
 * This file contains the definition of the CRender interface. Such interface
 * is the base for all the visualisations (OpenGL, OGRE, gnuplot, blender,
 * etc.) in ARGOS.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include <visualizations/render.h>
#include <argos2/simulator/space/space.h>

namespace argos {

   /****************************************/
   /****************************************/

   void CRender::Init(TConfigurationNode& t_tree) {
      /* Get render id from the XML */
      GetNodeAttribute(t_tree, "id", m_strId);
   }

   /****************************************/
   /****************************************/

   void CRender::Execute() {
      /* Main cycle */
      while(!m_cSimulator.IsExperimentFinished()) {
         m_cSimulator.UpdateSpace();
      }
   }

   /****************************************/
   /****************************************/

}
