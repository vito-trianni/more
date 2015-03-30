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
 * @file <simulator/visualizations/models/qtopengl_kumar_micro_quadrotor.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef QTOPENGL_KUMAR_MICRO_QUADROTOR_H
#define QTOPENGL_KUMAR_MICRO_QUADROTOR_H

namespace argos {
   class CQTOpenGLKumarMicroQuadrotor;
   class CKumarMicroQuadrotorEntity;
}


#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

   class CQTOpenGLKumarMicroQuadrotor {

   public:

      CQTOpenGLKumarMicroQuadrotor();

      virtual ~CQTOpenGLKumarMicroQuadrotor();

      virtual void Draw(CKumarMicroQuadrotorEntity& c_entity);

   protected:

      /** Sets the body material */
      void SetBodyMaterial();

      /** Sets the board material */
      void SetBoardMaterial();

      /** Sets the blade material */
      void SetBladeMaterial();

      /** The robot body */
      void MakeBody();

      /** The robot board */
      void MakeBoard();

      /** On of the two trasversal arms to which propellers are attached */
      void MakeArm();

      /** One of the four propellers */
      void MakePropeller();

   private:

      /** Number of vertices to display the round parts */
      GLuint m_unVertices;

      /** Start of the display list index */
      GLuint m_unLists;

   };

}

#endif
