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
 * @file common/control_interface/footbot/ci_footbot_magnetic_gripper_actuator.cpp
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
 * @author Touraj Soleymani - <tsoleyma@ulb.ac.be>
 */

#include "ci_footbot_magnetic_gripper_actuator.h"

namespace argos {

/*************************************************************************************************************/

    const CRange<CRadians> CCI_FootBotMagneticGripperActuator::FOOTBOT_MAGNETIC_GRIPPER_ELEVATION_ANGLE_RANGE(CRadians(0.0), CRadians(ARGOS_PI));
    const CRange<CRadians> CCI_FootBotMagneticGripperActuator::FOOTBOT_MAGNETIC_GRIPPER_TILT_ANGLE_RANGE(CRadians(0.0), CRadians(ARGOS_PI));
	
    const CRadians CCI_FootBotMagneticGripperActuator::FOOTBOT_MAGNETIC_GRIPPER_PROXIMITY_SENSORS_FACING_FORWARD;
    const CRadians CCI_FootBotMagneticGripperActuator::FOOTBOT_MAGNETIC_GRIPPER_PROXIMITY_SENSORS_FACING_DOWNWARD;
	  
    const CRadians CCI_FootBotMagneticGripperActuator::FOOTBOT_MAGNETIC_GRIPPER_MAGNETIC_SWITCH_FACING_FORWARD;
    const CRadians CCI_FootBotMagneticGripperActuator::FOOTBOT_MAGNETIC_GRIPPER_MAGNETIC_SWITCH_FACING_DOWNWARD;
  
/*************************************************************************************************************/

}
