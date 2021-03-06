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
 * @file <argos2/simulator/generic_sensors/foot-bot/eyebot_sensor.h>
 *
 * @brief This file provides the definition of
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef EYEBOT_SENSOR_H
#define EYEBOT_SENSOR_H

#include <string>
#include <map>

namespace argos {
   class CEyeBotSensor;
}

#include <argos2/simulator/sensors/simulated_sensor.h>
#include <argos2/simulator/space/entities/eyebot_entity.h>

namespace argos {

   class CEyeBotSensor : public CSimulatedSensor {

   public:

      CEyeBotSensor() :
         m_pcEyeBotEntity(NULL) {}
      virtual ~CEyeBotSensor() {}

      inline virtual CEyeBotEntity& GetEntity() {
         return *m_pcEyeBotEntity;
      }
      inline virtual void SetEntity(CEntity& c_entity) {
         m_pcEyeBotEntity = dynamic_cast<CEyeBotEntity*>(&c_entity);
         if(m_pcEyeBotEntity == NULL) {
            THROW_ARGOSEXCEPTION("Cannot associate an eye-bot sensor to a robot of type \"" << c_entity.GetTypeDescription() << "\"");
         }
      }

   private:

      CEyeBotEntity* m_pcEyeBotEntity;

   };

}

#endif
