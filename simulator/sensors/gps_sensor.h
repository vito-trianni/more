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
 * @file <argos2/simulator/sensors/gps_sensor.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef FOOTBOT_GPS_SENSOR_H
#define FOOTBOT_GPS_SENSOR_H

namespace argos {
   class CGPSSensor;
}

#include <argos2/common/control_interface/ci_gps_sensor.h>
#include <argos2/simulator/sensors/simulated_sensor.h>
#include <argos2/simulator/space/entities/embodied_entity.h>

namespace argos {

   class CGPSSensor : public CSimulatedSensor,
                      public CCI_GPSSensor {

   public:

      CGPSSensor();
      virtual ~CGPSSensor() {}

      virtual void Init(TConfigurationNode& t_tree);

      inline virtual CEntity& GetEntity() {
         return *m_pcEntity;
      }
      inline virtual void SetEntity(CEntity& c_entity);
      
      virtual void Update();
      virtual void Reset();

   private:

      CEntity* m_pcEntity;
      CEmbodiedEntity* m_pcEmbodiedEntity;
   };

}

#endif
