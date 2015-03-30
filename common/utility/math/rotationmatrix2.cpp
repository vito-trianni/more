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
 * @file argos2/common/utility/math/rotationmatrix2.cpp
 *
 * @brief Contains the implementation of a 2x2 rotation matrix
 *
 * @author Michael Allwright <michael.allwright@upb.de>
 */
 
#include "rotationmatrix2.h"
#include "angles.h"

namespace argos {

   /****************************************/
   /****************************************/

   void CRotationMatrix2::SetFromAngle(const CRadians& c_angle) {
      Real cos_angle = Cos(c_angle);
      Real sin_angle = Sin(c_angle);

      /* Set values */
      m_fValues[0] =  cos_angle;
      m_fValues[1] = -sin_angle;
      m_fValues[2] =  sin_angle;
      m_fValues[3] =  cos_angle;
   }
   
   /****************************************/
   /****************************************/

}
