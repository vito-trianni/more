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
 * @file <argos2/simulator/sensors/gps_sensor.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "gps_sensor.h"
#include <argos2/simulator/space/entities/composable_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CGPSSensor::CGPSSensor() :
      m_pcEmbodiedEntity(NULL) {}

   /****************************************/
   /****************************************/

   void CGPSSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_GPSSensor::Init(t_tree);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in GPS sensor.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CGPSSensor::SetEntity(CEntity& c_entity) {
      /* Treat the entity as composable */
      CComposableEntity* pcComposableEntity = dynamic_cast<CComposableEntity*>(&c_entity);
      if(pcComposableEntity != NULL) {
         /* The entity is composable, does it have the required components? */
         if(pcComposableEntity->HasComponent("embodied_entity")) {
            /* Yes, it does */
            m_pcEmbodiedEntity = &(pcComposableEntity->GetComponent<CEmbodiedEntity>("embodied_entity"));
            m_pcEntity = &c_entity;
         }
         else {
            /* No, error */
            THROW_ARGOSEXCEPTION("Cannot associate a GPS sensor to an entity of type \"" << c_entity.GetTypeDescription() << "\"");
         }
      }
   }

   /****************************************/
   /****************************************/

   void CGPSSensor::Update() {
      m_cReading = m_pcEmbodiedEntity->GetPosition();
   }

   /****************************************/
   /****************************************/

   void CGPSSensor::Reset() {
      m_cReading = CVector3();
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CGPSSensor,
                   "gps", "default",
                   "The GPS sensor",
                   "Carlo Pinciroli [cpinciro@ulb.ac.be]",
                   "This sensor accesses the GPS sensor. For a complete description of its usage,\n"
                   "refer to the common interface.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <gps implementation=\"default\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None for the time being.",
                   "Usable"
      );

}
