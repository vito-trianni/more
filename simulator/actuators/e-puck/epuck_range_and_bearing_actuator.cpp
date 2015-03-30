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
 * @file <argos2/simulator/actuators/e-puck/epuck_range_and_bearing_actuator.cpp>
 *
 * @brief This file provides the definition of
 *
 * @author Gianpiero Francesca - <gianpiero.francesca@ulb.ac.be>
 * @author Laurent Compere - <lcompere@ulb.ac.be>
 */

#include "epuck_range_and_bearing_actuator.h"
#include <argos2/simulator/space/entities/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CEPuckRangeAndBearingActuator::CEPuckRangeAndBearingActuator() :
      m_pcRABEquippedEntity(NULL),
      m_fRange(0.8f) {
   }

   /****************************************/
   /****************************************/

   void CEPuckRangeAndBearingActuator::Init(TConfigurationNode& t_node) {
      try {
         GetNodeAttribute(t_node, "range", m_fRange);
         m_pcRABEquippedEntity->SetRange(m_fRange);

      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Error initializing range and bearing actuator", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CEPuckRangeAndBearingActuator::SetEntity(CEntity& c_entity) {
      /* Let's check if it is a composable entity with a RAB component */
      CComposableEntity* pcComposableEntity = dynamic_cast<CComposableEntity*>(&c_entity);
      if(pcComposableEntity != NULL &&
         pcComposableEntity->HasComponent("rab_equipped_entity")) {
         /* All OK */
         m_pcRABEquippedEntity = &(pcComposableEntity->GetComponent<CRABEquippedEntity>("rab_equipped_entity"));
         /* Also, set the transmission range */
         m_pcRABEquippedEntity->SetRange(m_fRange);
      }
      else {
         /* Wrong entity associated */
         THROW_ARGOSEXCEPTION("Cannot associate a range and bearing actuator to a robot of type \"" << c_entity.GetTypeDescription() << "\"");
      }
   }
   
   /****************************************/
   /****************************************/

   void CEPuckRangeAndBearingActuator::Update() {
      m_pcRABEquippedEntity->SetData(m_tData);
   }

   /****************************************/
   /****************************************/

   void CEPuckRangeAndBearingActuator::Reset() {
      m_pcRABEquippedEntity->ClearData();
   }

   /****************************************/
   /****************************************/

   REGISTER_ACTUATOR(CEPuckRangeAndBearingActuator,
                     "epuck_range_and_bearing", "default",
                     "The e-puck range and bearing actuator",
                     "Gianpiero Francesca [gianpiero.francesca@ulb.ac.be]",
                     "This actuator accesses the e-puck range and bearing actuator. For a complete\n"
                     "description of its usage, refer to the common interface.\n"
                     "In this implementation, the actuator is sending its messages at a range of 0.8m\n"
                     "which is the basic range for the e-pucks.\n The range can be modified by adding the attribute range in the XML.\n"
                     "REQUIRED XML CONFIGURATION\n\n"
                     "  <controllers>\n"
                     "    ...\n"
                     "    <my_controller ...>\n"
                     "      ...\n"
                     "      <actuators>\n"
                     "        ...\n"
                     "        <range_and_bearing implementation=\"default\" range=\"0.8\" />\n"
                     "        ...\n"
                     "      </actuators>\n"
                     "      ...\n"
                     "    </my_controller>\n"
                     "    ...\n"
                     "  </controllers>\n\n",
                     "Under development"
                     );
}
