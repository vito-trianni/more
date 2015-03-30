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
 * @file <argos2/simulator/sensors/e-puck/epuck_range_and_bearing_sensor.h>
 *
 * @author Gianpiero Francesca - <gianpiero.francesca@ulb.ac.be>
 */

#ifndef EPUCK_RANGE_AND_BEARING_SENSOR_H
#define EPUCK_RANGE_AND_BEARING_SENSOR_H

namespace argos {
   class CEPuckRangeAndBearingSensor;
   class CControllableEntity;
}

#include <argos2/simulator/sensors/e-puck/epuck_sensor.h>
#include <argos2/common/control_interface/e-puck/ci_epuck_range_and_bearing_sensor.h>
#include <argos2/simulator/space/entities/rab_equipped_entity.h>
#include <argos2/simulator/space/space.h>
#include <argos2/common/utility/argos_random.h>

namespace argos {

   class CEPuckRangeAndBearingSensor : virtual public CSimulatedSensor,
                                       virtual public CCI_EPuckRangeAndBearingSensor {

   public:

      CEPuckRangeAndBearingSensor();
      virtual ~CEPuckRangeAndBearingSensor() {}

      virtual void Init(TConfigurationNode& t_tree);

      inline virtual CEntity& GetEntity() {
         return *m_pcEntity;
      }
      inline virtual void SetEntity(CEntity& c_entity);
      
      virtual void Update();
      virtual void Reset();

      inline virtual UInt16 PacketToInt(const SEPuckRangeAndBearingReceivedPacket& t_packet) {
         return *reinterpret_cast<const UInt16*>(t_packet.Data);
      }

   private:

      CSpace& m_cSpace;
      CSpaceHash<CEmbodiedEntity, CEmbodiedEntitySpaceHashUpdater>& m_cEmbodiedSpaceHash;
      CSpaceHash<CRABEquippedEntity, CRABEquippedEntitySpaceHashUpdater>& m_cRABEquippedSpaceHash;
      CEntity* m_pcEntity;
      CRABEquippedEntity* m_pcRABEquippedEntity;
      CEmbodiedEntity* m_pcEmbodiedEntity;
      CControllableEntity* m_pcControllableEntity;
      CARGoSRandom::CRNG* m_pcRNG;
    //  bool m_bNoise;
      std::vector<std::pair<Real,Real> > m_cMuValues,m_cSigmaValues;
      bool m_bShowRays;
      bool m_bCheckOcclusions;
      Real m_fLossProbability;//The probability of losing a package
      Real m_fExpA,m_fExpB; //The conversion between POWER (from the hw) and DISTANCE is done using the function DISTANCE(POWER)=exp(a+ b* POWER)
   };

}

#endif
