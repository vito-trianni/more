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
 * @file common/control_interface/footbot/ci_footbot_magnetic_gripper_sensor.h
 *
 * @brief This file provides the definition of the footbot magnetic gripper sensor.
 *
 * This file provides the definition of the footbot magnetic gripper sensors.
 * We can measure (a) the magnetic gripper state, (b) the magnetic switch state,
 * (c) the proximity sensors readings, (d) the elevation angle, (e1) the tilt angle 
 * of magnetic switch, and (e2) the tilt angle of proximity sensors.
 * (a) The states of the magnetic gripper are: (0) Deactivated and (1) Activated.
 * (b) The states of the magnetic switch are: (0) Deactivated and (1) Activated.
 * (c) There are six proximity sensors, three in the left and three in the right.
 * The proximity sensors readings are normalized between 0 and 1, and are in the 
 * order illustrated in the below sketch (seeing the robot from TOP, battery socket
 * is the BACK).
 * (d) The elevation angle is only positive and its convention is AGAINT RIGHT_HAND
 * RULE. It is measured with respect to x-y plane.
 * (e1,2) The tilt angle is always positive and its convention is AGAINT RIGHT_HAND
 * RULE. Generally, the magnetic switch and proximity sensors can have their
 * own tilt angles. However, in the current configuration the proximity sensors
 * and magnetic gripper are placed in one face so their title angles are the 
 * same here. Imagine that the elevation angle is zero, the tilt angle is zero
 * if the magnetic switch and proximity sensors are facing forward.
 *
 * 
 *		                    front
 * 
 *		   |  0 - 1 - 2                3 - 4 - 5  |              
 *		   |                                      |
 *		   |--------------------------------------|
 *		   |                                      |   
 *		   |                                      |
 *		                    back
 *
 * 
 * @author Roman Miletitch - <*********@ulb.ac.be>
 * @author Touraj Soleymani - <tsoleyma@ulb.ac.be>
 * 
 */

#ifndef CCI_FOOTBOT_MAGNETIC_GRIPPER_SENSOR_H
#define CCI_FOOTBOT_MAGNETIC_GRIPPER_SENSOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_FootBotMagneticGripperSensor;
}

#include <argos2/common/control_interface/ci_sensor.h>
#include <argos2/common/utility/math/angles.h>

namespace argos {

   class CCI_FootBotMagneticGripperSensor : public CCI_Sensor {

   public:

      enum State {DEACTIVATED, ACTIVATED};

      struct SReading {
	  Real Value;

	  SReading() :
	    Value(0.0f) {}

	  SReading(Real f_value) :
	    Value(f_value) {}
      };

      typedef std::vector<SReading> TReadings;
      
     
      /**
       * @brief Constructor
       */
      CCI_FootBotMagneticGripperSensor();
      
      
      /**
       * @brief Destructor
       */   
      virtual ~CCI_FootBotMagneticGripperSensor() {}

      
      /**
       * @brief Get the state of the magnetic gripper
       */
      inline virtual const State& GetMagneticGripperState() const {
         return m_eMagneticGripperState;
      }
      
      
      /**
       * @brief Get the state of the magnetic switch
       */
      inline virtual const State& GetMagneticSwtichState() const {
         return m_eMagneticSwitchState;
      }
      
      
      /**
       * @brief Get the readings of the proximity sensors
       */
      inline virtual const TReadings& GetProximitySensorReadings() const {
         return m_tProximitySensorReadings;
      }
      
      
      /**
       * @brief Get the elevation angle
       */
      inline virtual const CRadians& GetElevationAngle() const {
         return m_cElevationAngle;
      }

      
      /**
       * @brief Get the tilt angle of the magnetic switch
       */
      inline virtual const CRadians& GetMagneticSwitchTiltAngle() const {
         return m_cMagneticSwitchTiltAngle; 
      }
      
      
      /**
       * @brief Get the tilt angle of the proximity sensors
       */
      inline virtual const CRadians& GetProximitySensorsTiltAngle() const {
         return m_cProximitySensorsTiltAngle;
      }

   protected:
     
      static const UInt16 NUM_MAGNETIC_GRIPPER_PROXIMITY_SENSOR_READINGS;
      static const CRange<Real> FOOTBOT_MAGNETIC_GRIPPER_PROXIMITY_SENSORS_READING_RANGE;
      
      CRadians m_cElevationAngle;
      CRadians m_cMagneticSwitchTiltAngle;
      CRadians m_cProximitySensorsTiltAngle;
      
      State m_eMagneticGripperState;
      State m_eMagneticSwitchState;

      TReadings m_tProximitySensorReadings;
      
   };
   
   
   std::ostream& operator<<(std::ostream& cout, const CCI_FootBotMagneticGripperSensor::TReadings& t_readings);

}

#endif
