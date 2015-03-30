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
 * @file argos2/common/utility/math/rotationmatrix2.h
 *
 * @brief Contains the definition of a 2x2 rotation matrix
 *
 * @author Michael Allwright <michael.allwright@upb.de>
 */
 
#ifndef ROTATION_MATRIX2_H
#define ROTATION_MATRIX2_H

namespace argos {
   class CRadians;
}

#include "squarematrix.h"
#include "vector2.h"

namespace argos {

   class CRotationMatrix2 : public CSquareMatrix<2> {
      
   public:
      CRotationMatrix2() : CSquareMatrix<2>() {
         SetIdentityMatrix();
      }
      
      CRotationMatrix2(const CRadians& c_angle) : CSquareMatrix<2>() {
         SetFromAngle(c_angle);
      }
      
      void SetFromAngle(const CRadians& c_angle);
      
      CVector2 operator*(const CVector2& c_vector) const {
         return CVector2(m_fValues[0]*c_vector.m_fX + m_fValues[1]*c_vector.m_fY,
		                   m_fValues[2]*c_vector.m_fX + m_fValues[3]*c_vector.m_fY);
      }
   };
}

#endif
