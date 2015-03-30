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
 * @file argos2/common/utility/math/squarematrix.h
 *
 * @brief Contains the definition of a NxN templated square matrix
 *
 * @author Michael Allwright <michael.allwright@upb.de>
 */

#ifndef SQUARE_MATRIX_H
#define SQUARE_MATRIX_H

#include "matrix.h"

namespace argos {

   template <UInt32 DIM>
   class CSquareMatrix : public CMatrix<DIM, DIM> {
      
   public:
      CSquareMatrix() : CMatrix<DIM, DIM>() {}
      
      void SetIdentityMatrix() {
         for(UInt32 i = 0; i < DIM; i++) {
            for(UInt32 j = 0; j < DIM; j++) {
               if(i == j) {
                  CMatrix<DIM, DIM>::m_fValues[i * DIM + j] = 1;
               }
               else {
                  CMatrix<DIM, DIM>::m_fValues[i * DIM + j] = 0;
               }
            }
         }
      }
      
      bool IsIdentityMatrix() {
         bool bIsIdentMat = true;
         for(UInt32 i = 0; i < DIM; i++) {
            for(UInt32 j = 0; j < DIM; j++) {
               if(i == j) {
                  if(CMatrix<DIM, DIM>::m_fValues[i * DIM + j] != 1) {
                     bIsIdentMat = false;
                     break;
                  }
               }
               else {
                  if(CMatrix<DIM, DIM>::m_fValues[i * DIM + j] != 0) {
                     bIsIdentMat = false;
                     break;
                  }
               }
            }
         }
         return bIsIdentMat;
      }
   };
}

#endif
