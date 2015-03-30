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
 * @file argos2/common/utility/math/matrix.h
 *
 * @brief Contains the definition of a MxN templated matrix
 *
 * @author Michael Allwright <michael.allwright@upb.de>
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <argos2/common/utility/math/general.h>
#include <argos2/common/utility/math/angles.h>
#include <cmath>
#include <iomanip>

namespace argos {

   template <UInt32 ROWS, UInt32 COLS>
   class CMatrix {

   /* Matrices of different dimensions can access each others data */
   template <UInt32 SMROWS, UInt32 SMCOLS> friend class CMatrix;
      
   public:
      CMatrix() {
         for(UInt32 i = 0; i < ROWS * COLS; i++)
            m_fValues[i] = 0;
      }
      
      CMatrix(const Real* f_values) {
         Set(f_values);
      }
      
      CMatrix(const CMatrix<ROWS,COLS>& c_matrix) {
         Set(c_matrix.m_fValues);
      }
      
      CMatrix<ROWS,COLS>& operator=(const CMatrix<ROWS,COLS>& c_matrix) {
         if(this != &c_matrix) {
            Set(c_matrix.m_fValues);
         }
         return *this;
      }
          
      inline Real& operator()(UInt32 un_row,
                              UInt32 un_col) {
         ARGOS_ASSERT(un_row < ROWS && un_col < COLS,
                      "Matrix index out of bounds: un_row = " <<
                      un_row <<
                      ", un_col = " <<
                      un_col);
         return m_fValues[un_row * COLS + un_col];
      }
      
      inline Real operator()(UInt32 un_row,
                             UInt32 un_col) const {
         ARGOS_ASSERT(un_row < ROWS && un_col < COLS,
                      "Matrix index out of bounds: un_row = " <<
                      un_row <<
                      ", un_col = " <<
                      un_col);
         return m_fValues[un_row * COLS + un_col];
      }
      
      inline Real operator()(UInt32 un_idx) const {
         ARGOS_ASSERT(un_idx < ROWS * COLS,
                      "Matrix index out of bounds: un_idx = " <<
                      un_idx);
         return m_fValues[un_idx];
      }

      inline Real& operator()(UInt32 un_idx) {
         ARGOS_ASSERT(un_idx < ROWS * COLS,
                      "Matrix index out of bounds: un_idx = " <<
                      un_idx);
         return m_fValues[un_idx];
      }
      
      void Set(const Real* f_values) {
         for(UInt32 i = 0; i < ROWS * COLS; i++)
            m_fValues[i] = f_values[i];
      }
      
      CMatrix<COLS, ROWS> GetTransposed() {
         Real fNewValues[COLS * ROWS];
         for(UInt32 i = 0; i < ROWS; i++)
            for(UInt32 j = 0; j < COLS; j++)
               fNewValues[j * ROWS + i] = m_fValues[i * COLS + j];

         return CMatrix<COLS, ROWS>(fNewValues);
      }
      
      template <UInt32 SMROWS, UInt32 SMCOLS>
      CMatrix<SMROWS, SMCOLS>& GetSubmatrix(CMatrix<SMROWS, SMCOLS>& c_matrix,
                                            UInt32 un_offset_row, 
                                            UInt32 un_offset_col) {
         ARGOS_ASSERT((SMROWS - 1 + un_offset_row) < ROWS &&
                      (SMCOLS - 1 + un_offset_col) < COLS,
                      "Submatrix range is out of bounds: cannot extract a " <<
                      SMROWS << "x" << SMCOLS << " submatrix from a " <<
                      ROWS << "x" << COLS << " matrix with offsets " <<
                      " un_offset_row = " <<
                      un_offset_row <<
                      ", un_offset_col = " <<
                      un_offset_col);
                      
         for(UInt32 i = 0; i < SMROWS; i++)
            for(UInt32 j = 0; j < SMCOLS; j++)
               c_matrix.m_fValues[i * SMCOLS + j] = m_fValues[(i + un_offset_row) * COLS + (j + un_offset_col)];
         
         return c_matrix;
      }
      
      bool operator==(const CMatrix<ROWS, COLS>& c_matrix) const {
         for(UInt32 i = 0; i < ROWS * COLS; i++) {
            if(m_fValues[i] != c_matrix.m_fValues[i])
               return false;
         }
         return true;
      }

      CMatrix<ROWS, COLS>& operator+=(const CMatrix<ROWS, COLS>& c_matrix) {
         for(UInt32 i = 0; i < ROWS * COLS; i++) {
            m_fValues[i] += c_matrix.m_fValues[i];
         }
         return *this;
      }
      
      CMatrix<ROWS, COLS>& operator-=(const CMatrix<ROWS, COLS>& c_matrix) {
         for(UInt32 i = 0; i < ROWS * COLS; i++) {
            m_fValues[i] -= c_matrix.m_fValues[i];
         }
         return *this;
      }
      
      CMatrix<ROWS, COLS>& operator*=(Real f_scale) {
         for(UInt32 i = 0; i < ROWS * COLS; i++) {
            m_fValues[i] *= f_scale;
         }
         return *this;
      }
      
      CMatrix<ROWS, COLS> operator+(const CMatrix<ROWS, COLS>& c_matrix) const {
         CMatrix<ROWS, COLS> cResult = (*this);
         cResult += c_matrix;
         return cResult;
      }

      CMatrix<ROWS, COLS> operator-(const CMatrix<ROWS, COLS>& c_matrix) const {
         CMatrix<ROWS, COLS> cResult = (*this);
         cResult -= c_matrix;
         return cResult;
      }
      
      CMatrix<ROWS, COLS>& operator*=(const CMatrix<COLS, COLS>& c_matrix) {
         Real fNewValues[ROWS * COLS];
         for(UInt32 i = 0; i < ROWS; i++) {
            for(UInt32 j = 0; j < COLS; j++) {
               fNewValues[i * COLS + j] = 0.0f;
               for(UInt32 k = 0; k < COLS; k++) {
                  fNewValues[i * COLS + j] += m_fValues[i * COLS + k] * c_matrix.m_fValues[k * COLS + j];
               }
            }
         }
         Set(fNewValues);
         return *this;
      }
      
      template <UInt32 OTRCOLS>
      CMatrix<ROWS, OTRCOLS> operator*(const CMatrix<COLS, OTRCOLS>& c_matrix) const {
         Real fNewValues[ROWS * OTRCOLS];
         for(UInt32 i = 0; i < ROWS; i++) {
            for(UInt32 j = 0; j < OTRCOLS; j++) {
               fNewValues[i * OTRCOLS + j] = 0.0f;
               for(UInt32 k = 0; k < COLS; k++) {
                  fNewValues[i * OTRCOLS + j] += m_fValues[i * COLS + k] * c_matrix.m_fValues[k * OTRCOLS + j];
               }
            }
         }
         return CMatrix<ROWS, OTRCOLS>(fNewValues);
      }

      friend std::ostream& operator<<(std::ostream& c_os,
                                      const CMatrix c_matrix) {
         
         std::ios_base::fmtflags unInitalFlags = c_os.flags();
         std::streamsize nInitalPrec = c_os.precision();
         
         c_os.setf(std::ios::fixed);
         c_os.precision(1);
         
         for(UInt32 i = 0; i < ROWS; i++) {
            c_os << "| ";
            for(UInt32 j = 0; j < COLS; j++) {
               c_os << std::setw(6) << c_matrix(i, j) << " ";
            }
            c_os << "|" << std::endl;
         }
         
         c_os.flags(unInitalFlags);
         c_os.precision(nInitalPrec);
         return c_os;
      }
      
   protected:

      Real m_fValues[ROWS * COLS];

   };
}

#endif
