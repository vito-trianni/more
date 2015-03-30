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

#ifndef CQUATERNION_H
#define CQUATERNION_H

#include <argos2/common/utility/math/vector3.h>

namespace argos {

   class CQuaternion {

   public:
      CQuaternion() {
         m_fValues[0] = 1.0;
         m_fValues[1] = 0.0;
         m_fValues[2] = 0.0;
         m_fValues[3] = 0.0;
      }

      CQuaternion(const CQuaternion& c_quaternion) {
         *this = c_quaternion;
      }

      CQuaternion(Real f_real,
                  Real f_img1,
                  Real f_img2,
                  Real f_img3) {
         m_fValues[0] = f_real;
         m_fValues[1] = f_img1;
         m_fValues[2] = f_img2;
         m_fValues[3] = f_img3;
      }

      CQuaternion(const CRadians& c_radians,
                  const CVector3& c_vector3) {
         FromAngleAxis(c_radians, c_vector3);
      }

      inline CQuaternion(const CVector3& c_vector1,
                         const CVector3& c_vector2) {
         BetweenTwoVectors(c_vector1, c_vector2);
      }

      inline Real GetW() const {
         return m_fValues[0];
      }

      inline Real GetX() const {
         return m_fValues[1];
      }

      inline Real GetY() const {
         return m_fValues[2];
      }

      inline Real GetZ() const {
         return m_fValues[3];
      }

      inline CQuaternion Conjugate() const {
         return CQuaternion(m_fValues[0],
                            -m_fValues[1],
                            -m_fValues[2],
                            -m_fValues[3]);
      }

      inline CQuaternion Inverse() const {
         return CQuaternion(m_fValues[0],
                            -m_fValues[1],
                            -m_fValues[2],
                            -m_fValues[3]);
      }

      inline Real Length() const {
         return ::sqrt(SquareLength());
      }

      inline Real SquareLength() const {
         return
            Square(m_fValues[0]) +
            Square(m_fValues[1]) +
            Square(m_fValues[2]) +
            Square(m_fValues[3]);
      }

      inline CQuaternion& Normalize() {
         Real fInvLength = 1.0 / Length();
         m_fValues[0] *= fInvLength;
         m_fValues[1] *= fInvLength;
         m_fValues[2] *= fInvLength;
         m_fValues[3] *= fInvLength;
         return *this;
      }

      inline CQuaternion& FromAngleAxis(const CRadians& c_angle,
                                        const CVector3& c_vector) {
         CRadians fHalfAngle = c_angle * 0.5;
         Real fSin = Sin(fHalfAngle);
         m_fValues[0] = Cos(fHalfAngle);
         m_fValues[1] = c_vector.GetX() * fSin;
         m_fValues[2] = c_vector.GetY() * fSin;
         m_fValues[3] = c_vector.GetZ() * fSin;
         return *this;
      }

      inline void ToAngleAxis(CRadians& c_angle,
                              CVector3& c_vector) const {
         Real fSquareLength =
            Square(m_fValues[1]) +
            Square(m_fValues[2]) +
            Square(m_fValues[3]);
         
         if(fSquareLength > 0.0f) {
            c_angle = 2.0f * ACos(m_fValues[0]);
            Real fInvLength = 1.0f / ::sqrt(fSquareLength);
            c_vector.Set(m_fValues[1] * fInvLength,
                         m_fValues[2] * fInvLength,
                         m_fValues[3] * fInvLength);
         }
         else {
            /* By default, to ease the support of robot orientation, no rotation refers to the Z axis */
            c_angle = CRadians::ZERO;
            c_vector = CVector3::Z;
         }
      }

      inline CQuaternion& FromEulerAngles(const CRadians& c_z_angle,
                                          const CRadians& c_y_angle,
                                          const CRadians& c_x_angle) {
         (*this) = CQuaternion(c_x_angle, CVector3::X) *
            CQuaternion(c_y_angle, CVector3::Y) *
            CQuaternion(c_z_angle, CVector3::Z);
         return (*this);
      }

      inline void ToEulerAngles(CRadians& c_z_angle,
                                CRadians& c_y_angle,
                                CRadians& c_x_angle) const {
         /* With the ZYX convention, gimbal lock happens when
            cos(y_angle) = 0, that is when y_angle = +- pi/2
            In this condition, the Z and X axis overlap and we
            lose one degree of freedom. It's a problem of the
            Euler representation of rotations that is not
            present when we deal with quaternions.
            For reasons of speed, we consider gimbal lock
            happened when fTest > 0.499 and when fTest < -0.499.
         */
         /* Computed to understand if we have gimbal lock or not */
         Real fTest =
            m_fValues[1] * m_fValues[3] +
            m_fValues[0] * m_fValues[2];

         if(fTest > 0.499f) {
            /* Gimbal lock */
            c_x_angle = CRadians::ZERO;
            c_y_angle = CRadians::PI_OVER_TWO;
            c_z_angle = ATan2(2.0f * (m_fValues[1] * m_fValues[2] + m_fValues[0] * m_fValues[3]),
                              1.0f - 2.0f * (m_fValues[1] * m_fValues[1] + m_fValues[3] * m_fValues[3]));
         }
         else if(fTest < -0.499f) {
            /* Gimbal lock */
            c_x_angle = CRadians::ZERO;
            c_y_angle = -CRadians::PI_OVER_TWO;
            c_z_angle = ATan2(2.0f * (m_fValues[1] * m_fValues[2] + m_fValues[0] * m_fValues[3]),
                              1.0f - 2.0f * (m_fValues[1] * m_fValues[1] + m_fValues[3] * m_fValues[3]));
         }
         else {
            /* Normal case */
            Real fSqValues[4] = {
               Square(m_fValues[0]),
               Square(m_fValues[1]),
               Square(m_fValues[2]),
               Square(m_fValues[3])
            };
            
            c_x_angle = ATan2(2.0 * (m_fValues[0] * m_fValues[1] - m_fValues[2] * m_fValues[3]),
                              fSqValues[0] - fSqValues[1] - fSqValues[2] + fSqValues[3]);
            c_y_angle = ASin(2.0 * (m_fValues[1] * m_fValues[3] + m_fValues[0] * m_fValues[2]));
            c_z_angle = ATan2(2.0 * (m_fValues[0] * m_fValues[3] - m_fValues[1] * m_fValues[2]),
                              fSqValues[0] + fSqValues[1] - fSqValues[2] - fSqValues[3]);
         }
      }

      inline CQuaternion& BetweenTwoVectors(const CVector3& c_vector1,
                                            const CVector3& c_vector2) {
         m_fValues[0] = ::sqrt(c_vector1.SquareLength() * c_vector2.SquareLength()) + c_vector1.DotProduct(c_vector2);
         CVector3 cCrossProd(c_vector1);
         cCrossProd.CrossProduct(c_vector2);
         m_fValues[1] = cCrossProd.GetX();
         m_fValues[2] = cCrossProd.GetY();
         m_fValues[3] = cCrossProd.GetZ();
         Normalize();
         return *this;
      }

      inline CQuaternion& operator=(const CQuaternion& c_quaternion) {
         if (&c_quaternion != this) {
            m_fValues[0] = c_quaternion.m_fValues[0];
            m_fValues[1] = c_quaternion.m_fValues[1];
            m_fValues[2] = c_quaternion.m_fValues[2];
            m_fValues[3] = c_quaternion.m_fValues[3];
         }
         return *this;
      }

      inline CQuaternion& operator+=(const CQuaternion& c_quaternion) {
         m_fValues[0] += c_quaternion.m_fValues[0];
         m_fValues[1] += c_quaternion.m_fValues[1];
         m_fValues[2] += c_quaternion.m_fValues[2];
         m_fValues[3] += c_quaternion.m_fValues[3];
         return *this;
      }

      inline CQuaternion& operator-=(const CQuaternion& c_quaternion) {
         m_fValues[0] -= c_quaternion.m_fValues[0];
         m_fValues[1] -= c_quaternion.m_fValues[1];
         m_fValues[2] -= c_quaternion.m_fValues[2];
         m_fValues[3] -= c_quaternion.m_fValues[3];
         return *this;
      }

      inline CQuaternion& operator*=(const CQuaternion& c_quaternion) {
         Real newv[4];
         newv[0] = m_fValues[0] * c_quaternion.m_fValues[0] -
            m_fValues[1] * c_quaternion.m_fValues[1] -
            m_fValues[2] * c_quaternion.m_fValues[2] -
            m_fValues[3] * c_quaternion.m_fValues[3];
         newv[1] = m_fValues[0] * c_quaternion.m_fValues[1] +
            m_fValues[1] * c_quaternion.m_fValues[0] +
            m_fValues[2] * c_quaternion.m_fValues[3] -
            m_fValues[3] * c_quaternion.m_fValues[2];
         newv[2] = m_fValues[0] * c_quaternion.m_fValues[2] -
            m_fValues[1] * c_quaternion.m_fValues[3] +
            m_fValues[2] * c_quaternion.m_fValues[0] +
            m_fValues[3] * c_quaternion.m_fValues[1];
         newv[3] = m_fValues[0] * c_quaternion.m_fValues[3] +
            m_fValues[1] * c_quaternion.m_fValues[2] -
            m_fValues[2] * c_quaternion.m_fValues[1] +
            m_fValues[3] * c_quaternion.m_fValues[0];
         m_fValues[0] = newv[0];
         m_fValues[1] = newv[1];
         m_fValues[2] = newv[2];
         m_fValues[3] = newv[3];
         return *this;
      }

      inline CQuaternion operator+(const CQuaternion& c_quaternion) const {
         CQuaternion result(*this);
         result += c_quaternion;
         return result;
      }

      inline CQuaternion operator-(const CQuaternion& c_quaternion) const {
         CQuaternion result(*this);
         result -= c_quaternion;
         return result;
      }

      inline CQuaternion operator*(const CQuaternion& c_quaternion) const {
         CQuaternion result(*this);
         result *= c_quaternion;
         return result;
      }

      inline friend std::ostream& operator<<(std::ostream& c_os, const CQuaternion& c_quaternion) {
         c_os << "CQuaternion("
              << c_quaternion.m_fValues[0] << ","
              << c_quaternion.m_fValues[1] << ","
              << c_quaternion.m_fValues[2] << ","
              << c_quaternion.m_fValues[3] << ")";
         return c_os;
      }

   private:

      Real m_fValues[4];
   };

}

#endif
