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
 * @file common/control_interface/footbot/ci_footbot_magnetic_gripper_actuator.h
 *
 * @brief This file provides the definition of the magnetic gripper actuator.
 * 
 * This file provides the definition of the footbot magnetic gripper actuators.
 * We can change (a) the magnetic gripper state, (b) the magnetic switch state,
 * (c) the elevation angle, (d1) the tilt angle of magnetic switch, and (d2) the
 * tilt angle of proximity sensors.
 * (a) The states of the magnetic gripper are: (0) Deactivated and (1) Activated.
 * (b) The states of the magnetic switch are: (0) Deactivated and (1) Activated.
 * (c) The elevation angle is only positive and its convention is AGAINT RIGHT_HAND
 * RULE. It is measured with respect to x-y plane.
 * (d1,2) The tilt angle is always positive and its convention is AGAINT RIGHT_HAND
 * RULE. Generally, the magnetic switch and proximity sensors can have their
 * own tilt angles. However, in the current configuration the proximity sensors
 * and magnetic gripper are placed in one face so their title angles are the 
 * same here. Imagine that the elevation angle is zero, the tilt angle is zero
 * if the magnetic switch and proximity sensors are facing forward.
 * 
 * @author Roman Miletitch - <*********@ulb.ac.be>
 * @author Touraj Soleymani - <tsoleyma@ulb.ac.be>
 */

#ifndef CCI_FOOTBOT_MAGNETIC_GRIPPER_ACTUATOR_H
#define CCI_FOOTBOT_MAGNETIC_GRIPPER_ACTUATOR_H

/* To avoid dependency problems when including */
namespace argos {
   class CCI_FootBotMagneticGripperActuator;
}

#include <argos2/common/control_interface/ci_actuator.h>
#include <argos2/common/utility/math/angles.h>

namespace argos {

   class CCI_FootBotMagneticGripperActuator : virtual public CCI_Actuator {
   
   public:
      
      /**
       * @brief Constructor
       */
      CCI_FootBotMagneticGripperActuator() {}

      
      /**
       * @brief Destructor
       */
      virtual ~CCI_FootBotMagneticGripperActuator() {}

      
      /**
       * @brief Activate the magnetic gripper
       */
      virtual void ActivateMagneticGripper() = 0;

      
      /**
       * @brief Deactivate the magnetic gripper
       * 
       * This function first sets the elevation angle to zero then deactivates
       * the magnetic gripper. The magnetic switch still remains in its
       * current state although the magnetic gripper is deactivated.
       */
      virtual void DeactivateMagneticGripper() = 0;
      
      
      /**
       * @brief Activate the magnetic switch
       *
       */
      virtual void ActivateMagneticSwitch() = 0;
      
      
      /**
       * @brief Deactivate the magnetic switch
       *
       */
      virtual void DeactivateMagneticSwitch() = 0;

   
      /**
       * @brief Set the elevation angle
       * 
       * @param c_angle elevation angle in radians
       */
      virtual void SetElevationAngle(const CRadians& c_angle) = 0;
  
      
      /**
       * @brief Set tilt angle for the magnetic switch
       * 
       * @param c_angle tilt angle for the magnetic switch in radians
       */
      virtual void SetMagneticSwitchTiltAngle(const CRadians& c_angle) = 0;
      
      
      /**
       * @brief Set tilt angle for the proximity sensors
       * 
       * @param c_angle tilt angle for the proximity sensors in radians
       */
      virtual void SetProximitySensorsTiltAngle(const CRadians& c_angle) = 0;

      
      /**
       * @brief Set the elevation angle while adjusting the tilt angle
       * This functoin sets the elevation angle while maintains the current tilt
       * angle with respect to x-y plane fixed. To this end, the tilt angle must 
       * be adjusted. For example, if the magnetic switch is in level position, it 
       * remains level for different elevation angles.
       * 
       * @param c_angle elevation angle in radians
       */
      virtual void SetElevationAndTiltAngles(const CRadians& c_angle) = 0;
      
   protected:
  	  
      static const CRange<CRadians> FOOTBOT_MAGNETIC_GRIPPER_ELEVATION_ANGLE_RANGE;
      static const CRange<CRadians> FOOTBOT_MAGNETIC_GRIPPER_TILT_ANGLE_RANGE;
      
      static const CRadians FOOTBOT_MAGNETIC_GRIPPER_PROXIMITY_SENSORS_FACING_FORWARD;
      static const CRadians FOOTBOT_MAGNETIC_GRIPPER_PROXIMITY_SENSORS_FACING_DOWNWARD;
	
      static const CRadians FOOTBOT_MAGNETIC_GRIPPER_MAGNETIC_SWITCH_FACING_FORWARD;
      static const CRadians FOOTBOT_MAGNETIC_GRIPPER_MAGNETIC_SWITCH_FACING_DOWNWARD;
      
   };
   
}

#endif
