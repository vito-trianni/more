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
 * @file argos2/common/utility/math/rotationmatrix3.h
 *
 * @brief Contains the definition of a 3x3 rotation matrix
 *
 * @author Michael Allwright <michael.allwright@upb.de>
 */

#ifndef ROTATION_MATRIX3_H
#define ROTATION_MATRIX3_H

namespace argos {
   class CQuaternion;
}

#include "squarematrix.h"
#include "vector3.h"

namespace argos {

   class CRotationMatrix3 : public CSquareMatrix<3> {
   
   public:
      CRotationMatrix3() : CSquareMatrix<3>() {
         SetIdentityMatrix();
      }
      
      CRotationMatrix3(const CQuaternion& c_quaternion) : CSquareMatrix<3>() {
         SetFromQuaternion(c_quaternion);
      }
      
      void SetFromQuaternion(const CQuaternion& c_quaternion);
      
      CVector3 operator*(const CVector3& c_vector) const {
         return CVector3(m_fValues[0]*c_vector.m_fX + m_fValues[1]*c_vector.m_fY + m_fValues[2]*c_vector.m_fZ,
		                   m_fValues[3]*c_vector.m_fX + m_fValues[4]*c_vector.m_fY + m_fValues[5]*c_vector.m_fZ,
		                   m_fValues[6]*c_vector.m_fX + m_fValues[7]*c_vector.m_fY + m_fValues[8]*c_vector.m_fZ);
      }
   };
}

#endif
