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
 * @file common/control_interface/e-puck/ci_range_and_bearing_actuator.h
 *
 * @brief This file provides the definition of the actuator part of the
 *        range and bearing system.
 *
 * The range and bearing sensor/actuator pair allows robots to
 * sense other robots in their locality and exchange data with them.
 * When the presence of a robot cannot be sensed anymore (i.e., the sensing
 * robot does not receive an IR beam from the robot), the data sent
 * over the local radio is ignored. Thus, this sensor/actuator pair
 * provides localization and local communication to the robot.
 *
 * At each timestep, each robot emits out an IR beam, allowing other robots
 * to sense its position. At the same time, the robot sends a packet
 * over the local radio. This packet is always sent, even when no data
 * has been set by the controller. In this case, the data transmitted
 * is only zeros. The data has a size of 16 bits.
 *
 * The controller can set the data to be sent. The data can be
 * set using the method CCI_RangeAndBearingActuator::SetData(). 
 *
 * @author Gianpiero Francesca <gianpiero.francesca@ulb.ac.be>
 */

#ifndef CCI_EPUCK_IRCOM_ACTUATOR_H
#define CCI_EPUCK_IRCOM_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_EPuckIrcomActuator;
}

#include <string>
#include <vector>
#include <iostream>

#include <argos2/common/control_interface/ci_actuator.h>
#include <argos2/common/control_interface/e-puck/ci_epuck_ircom_sensor.h>

namespace argos {

   class CCI_EPuckIrcomActuator : virtual public CCI_Actuator {

   public:

      virtual ~CCI_EPuckIrcomActuator() {}

      /* Checks if the last packet has been sent already. */
      //virtual bool IsLastPacketSent() const = 0;

      /* Set the data to send. This will be ignored if the last packet has not been sent yet. */
      virtual void SetData(const UInt8& t_data) = 0;
      
      virtual void SetEnabled(bool enabled) = 0;

      /* Set the data to send. This will be ignored if the last packet has not been sent yet.
         Parameter is a integer to make a easier transmission. */
      //virtual void SetData(UInt16 un_data) = 0;

   };

}

#endif
