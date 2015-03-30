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
 * @file common/control_interface/footbot/ci_footbot_turret_actuator.h
 *
 * @brief This file provides the common interface definition of the turret actuator.

 * This file provides the definition of the turret actuator. The actuator
 * allows the user to control the turret's angular position or the turret
 * rotation speed.
 *
 * The user can dynamically decide the turret control mode.
 * Possible modes are: passive, speed control, position control, and off.
 *
 * When the turret is passive, it is loose and does not resist toi external
 * forces that make the turret turn, but the angular position of the turret can be tracked.
 *
 * When the turret is off, it is completely loose, and additionally no measures can be taken
 * concerning the turret rotation. Off mode should be used only to turn the actuator off, or
 * in cases where a loose turret is needed, but no angular measures are needed.
 *
 * When the turret is controlled in speed, the user can decide the rotation speed. Rotation
 * speed is expressed in PID pulses, positive values make the turret rotate counter clockwise,
 * when looking from above. When the turret is controlled in speed, it resists to external
 * forces that try to move the turret. Even if the target speed is set to zero, the turret will
 * try to resist to external forces that try to move it.
 *
 * When the turret is controlled in position, the user can decide the rotation angle.
 * Rotation is expressed in radians, positive values make the turret rotate counter clockwise,
 * when looking from above. When the turret is controlled in position, it resists to external
 * forces that try to move the turret.
 *
 * ACTUATED VALUES LIMITS:
 * TODO
 * The limits are enforced by the common interface.
 *
 * IMPORTANT: the robot uses the turret to determine when an object has been successfully gripped.
 * Therefore, right after a new gripper command has been issued, can be the case that the robot is using
 * the turret to assess whether gripping was successful. In the meantime EVERY command issued to
 * the turret is IGNORED. The check operation lasts less than a second.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 * @author Giovanni Pini   - <gpini@iridia.ulb.ac.be>
 */

#ifndef CCI_FOOTBOT_TURRET_ACTUATOR_H
#define CCI_FOOTBOT_TURRET_ACTUATOR_H

namespace argos {
   class CCI_FootBotTurretActuator;
}

#include <argos2/common/control_interface/ci_actuator.h>
#include <argos2/common/utility/math/angles.h>

namespace argos {

    class CCI_FootBotTurretActuator : virtual public CCI_Actuator {

    public:

        /** Turret modes */
        enum ETurretModes {
            MODE_OFF,
            MODE_PASSIVE,
            MODE_SPEED_CONTROL,
            MODE_POSITION_CONTROL,
        };

        /**
         *
         * Destructor.
         *
         */
        virtual ~CCI_FootBotTurretActuator() {
        }

        /**
         *
         * @brief Sets the turret rotation
         * Turret rotation is expressed in radians, positive values rotate the turret clockwise
         * when looking from above.
         * In the current real robot implementation, values are limited using range functions.
         * If the turret is not in position control mode, the only effect is that the target rotation is changed
         * but the turret will not turn: it will turn when position control mode is activated
         * (unless another target rotation is specified meantime).
         * The method as no effect if called while the robot is checking if it successfully gripped an object.
         *
         * @param c_angle desired turret rotation - radians
         */
        virtual void SetRotation(const CRadians& c_angle) = 0;

        /**
         *
         * @brief Sets the turret rotation speed
         * Turret rotation speed is expressed in motor pulses, positive values rotate the turret clockwise
         * when looking from above.
         * In the current real robot implementation, values are limited using range functions.
         * If the turret is not in speed control mode, the only effect is that the target rotation speed is changed
         * but the turret will not turn: it will turn when speed control mode is activated
         * (unless another target rotation speed is specified meantime).
         * The method as no effect if called while the robot is checking if it successfully gripped an object.
         *
         * @param n_speed_pulses desired turret rotation speed - pid pulses
         * @see CCI_FootBotTurretActuator
         */
        virtual void SetRotationSpeed(SInt32 n_speed_pulses) = 0;

        /**
         *
         * @brief Sets the turret control mode
         * Modes are: angular position control, rotation speed control, passive mode, off
         *
         * @param un_mode turret control mode, possible values are defined in CCI_FootBotTurretActuator
         * @see CCI_FootBotTurretActuator
         *
         */
        virtual void SetMode(ETurretModes e_mode) = 0;

        /**
         *
         * @brief Sets the turret control mode to active, and sets the target rotation to the given one
         *
         * @param c_angle desired turret rotation
         * @see SetMode
         * @see SetRotation
         *
         */
        inline virtual void SetActiveWithRotation(const CRadians& c_angle) {
            SetPositionControlMode();
            SetRotation(c_angle);
        }

        /**
         *
         * @brief Sets the turret control mode to speed control
         *
         * @see SetMode
         *
         */
        inline virtual void SetSpeedControlMode() {
            SetMode(MODE_SPEED_CONTROL);
        }

        /**
         *
         * @brief Sets the turret control mode to position control
         *
         * @see SetMode
         *
         */
        inline virtual void SetPositionControlMode() {
            SetMode(MODE_POSITION_CONTROL);
        }

        /**
         *
         * @brief Sets the turret control mode to passive
         *
         * @see SetMode
         *
         */
        inline virtual void SetPassiveMode() {
            SetMode(MODE_PASSIVE);
        }

        /**
         *
         * @brief returns true if the turret can be used (no check for object gripped in progress)
         *
         * The turret is used to check if an object has been gripped in a routine of the ASEBA backend.
         * While the routine is in  progress, commands to the turret (such as setting modes, position or speed)
         * are ignored. This method is used to check if the turret can be controlled or the routine is in progress.
         *
         * @return true if the turret can be used
         *
         * */
        inline bool IsTurretAvailableForCommands() {
            return !m_bIsCheckingForGrippedObject;
        }

    protected:

        /** Define ranges for the speed (angular ranges are defined in the common interface turret sensor)*/
        static const CRange<SInt32> FOOTBOT_TURRET_SPEED_RANGE;
        static const CRange<Real>   FOOTBOT_TURRET_NORMALIZED_SPEED_RANGE;

        /** When true the turret is being used for checking for a gripped object, and cannot be controlled by the user */
        bool m_bIsCheckingForGrippedObject;

    };

}

#endif
