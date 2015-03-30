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

#include "qtopengl_kumar_micro_quadrotor.h"
#include <argos2/common/utility/math/vector2.h>
#include <argos2/common/utility/math/vector3.h>
#include <argos2/common/utility/logging/argos_log.h>
#include <argos2/simulator/space/entities/kumar_micro_quadrotor_entity.h>
#include <argos2/simulator/space/entities/led_equipped_entity.h>
#include <argos2/simulator/space/entities/distance_scanner_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   /* All measures are in meters */

   static const Real BOARD_HALF_DIAGONAL    = 0.029f;
   static const Real BOARD_HEIGHT           = 0.005f;

   static const Real ARM_HALF_LENGTH        = 0.063f;
   static const Real ARM_HEIGHT             = 0.003f;
   static const Real ARM_HALF_WIDTH         = 0.0015f;
   static const Real ARM_BOTTOM             = 0.001f;
   static const Real ARM_TOP                = ARM_BOTTOM + ARM_HEIGHT;

   static const Real PROPELLER_ROD_RADIUS   = 0.013f;
   static const Real PROPELLER_ROD_HEIGHT   = 0.011f;
   static const Real PROPELLER_ROD_BOTTOM   = ARM_TOP;
   static const Real PROPELLER_ROD_TOP      = PROPELLER_ROD_BOTTOM + PROPELLER_ROD_HEIGHT;

   static const Real PROPELLER_BLADE_RADIUS    = 0.041f;
   static const Real PROPELLER_BLADE_ELEVATION = 0.0105f;

   static const Real ONE_OVER_SR2           = 1.0f / Sqrt(2.0f);

   /****************************************/
   /****************************************/

   CQTOpenGLKumarMicroQuadrotor::CQTOpenGLKumarMicroQuadrotor() :
      m_unVertices(40) {
      /* Reserve the needed display lists */
      m_unLists = glGenLists(1);
      /* Create the body display list */
      glNewList(m_unLists, GL_COMPILE);
      MakeBody();
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLKumarMicroQuadrotor::~CQTOpenGLKumarMicroQuadrotor() {
      glDeleteLists(m_unLists, 1);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKumarMicroQuadrotor::Draw(CKumarMicroQuadrotorEntity& c_entity) {
      /* Place the body */
      glCallList(m_unLists);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKumarMicroQuadrotor::SetBodyMaterial() {
      const GLfloat pfColor[]     = {   0.00f, 0.00f, 0.00f, 1.00f };
      const GLfloat pfSpecular[]  = {   0.90f, 0.90f, 0.90f, 1.00f };
      const GLfloat pfShininess[] = { 100.00f                   };
      const GLfloat pfEmission[]  = {   0.00f, 0.00f, 0.00f, 1.00f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKumarMicroQuadrotor::SetBoardMaterial() {
      const GLfloat pfColor[]     = {   0.00f, 1.00f, 0.00f, 1.00f };
      const GLfloat pfSpecular[]  = {   0.90f, 0.90f, 0.90f, 1.00f };
      const GLfloat pfShininess[] = { 100.00f                      };
      const GLfloat pfEmission[]  = {   0.00f, 0.00f, 0.00f, 1.00f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKumarMicroQuadrotor::SetBladeMaterial() {
      const GLfloat pfColor[]     = {   0.75f, 0.75f, 0.75f, 1.00f };
      const GLfloat pfSpecular[]  = {   0.90f, 0.90f, 0.90f, 1.00f };
      const GLfloat pfShininess[] = { 100.00f                   };
      const GLfloat pfEmission[]  = {   0.00f, 0.00f, 0.00f, 1.00f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKumarMicroQuadrotor::MakeBody() {
      /* Board */
      MakeBoard();
      /* N->S arm */
      MakeArm();
      /* W->E arm */
      glPushMatrix();
      glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
      MakeArm();
      glPopMatrix();
      /* Propellers */
      /* N propeller */
      glPushMatrix();
      glTranslatef(0.0f, ARM_HALF_LENGTH - PROPELLER_ROD_RADIUS, 0.0f);
      MakePropeller();
      glPopMatrix();
      /* E propeller */
      glPushMatrix();
      glTranslatef(ARM_HALF_LENGTH - PROPELLER_ROD_RADIUS, 0.0f, 0.0f);
      MakePropeller();
      glPopMatrix();
      /* S propeller */
      glPushMatrix();
      glTranslatef(0.0f, -ARM_HALF_LENGTH + PROPELLER_ROD_RADIUS, 0.0f);
      MakePropeller();
      glPopMatrix();
      /* W propeller */
      glPushMatrix();
      glTranslatef(-ARM_HALF_LENGTH + PROPELLER_ROD_RADIUS, 0.0f, 0.0f);
      MakePropeller();
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKumarMicroQuadrotor::MakeBoard() {
      SetBoardMaterial();
      glBegin(GL_QUADS);
      /* Bottom face, parallel to XY */
      glNormal3f(0.0f, 0.0f, -1.0f);
      glVertex3f(                0.0f,  BOARD_HALF_DIAGONAL, 0.0f        );
      glVertex3f( BOARD_HALF_DIAGONAL,                 0.0f, 0.0f        );
      glVertex3f(                0.0f, -BOARD_HALF_DIAGONAL, 0.0f        );
      glVertex3f(-BOARD_HALF_DIAGONAL,                 0.0f, 0.0f        );
      /* Top face, parallel to XY */
      glNormal3f(0.0f, 0.0f, 1.0f);
      glVertex3f(                0.0f,  BOARD_HALF_DIAGONAL, BOARD_HEIGHT);
      glVertex3f(-BOARD_HALF_DIAGONAL,                 0.0f, BOARD_HEIGHT);
      glVertex3f(                0.0f, -BOARD_HALF_DIAGONAL, BOARD_HEIGHT);
      glVertex3f( BOARD_HALF_DIAGONAL,                 0.0f, BOARD_HEIGHT);
      /* This part covers the faces (NE, SE, SW, NW) */
      /* NE face */
      glNormal3f(        ONE_OVER_SR2,         ONE_OVER_SR2, 0.0f        );
      glVertex3f(                0.0f,  BOARD_HALF_DIAGONAL, 0.0f        );
      glVertex3f(                0.0f,  BOARD_HALF_DIAGONAL, BOARD_HEIGHT);
      glVertex3f( BOARD_HALF_DIAGONAL,                 0.0f, BOARD_HEIGHT);
      glVertex3f( BOARD_HALF_DIAGONAL,                 0.0f, 0.0f        );
      /* SE face */
      glNormal3f(        ONE_OVER_SR2,        -ONE_OVER_SR2, 0.0f        );
      glVertex3f( BOARD_HALF_DIAGONAL,                 0.0f, 0.0f        );
      glVertex3f( BOARD_HALF_DIAGONAL,                 0.0f, BOARD_HEIGHT);
      glVertex3f(                0.0f, -BOARD_HALF_DIAGONAL, BOARD_HEIGHT);
      glVertex3f(                0.0f, -BOARD_HALF_DIAGONAL, 0.0f        );
      /* SW face */
      glNormal3f(       -ONE_OVER_SR2,        -ONE_OVER_SR2, 0.0f        );
      glVertex3f(                0.0f, -BOARD_HALF_DIAGONAL, 0.0f        );
      glVertex3f(                0.0f, -BOARD_HALF_DIAGONAL, BOARD_HEIGHT);
      glVertex3f(-BOARD_HALF_DIAGONAL,                 0.0f, BOARD_HEIGHT);
      glVertex3f(-BOARD_HALF_DIAGONAL,                 0.0f, 0.0f        );
      /* NW face */
      glNormal3f(       -ONE_OVER_SR2,         ONE_OVER_SR2, 0.0f        );
      glVertex3f(-BOARD_HALF_DIAGONAL,                 0.0f, 0.0f        );
      glVertex3f(-BOARD_HALF_DIAGONAL,                 0.0f, BOARD_HEIGHT);
      glVertex3f(                0.0f,  BOARD_HALF_DIAGONAL, BOARD_HEIGHT);
      glVertex3f(                0.0f,  BOARD_HALF_DIAGONAL, 0.0f        );
      glEnd();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKumarMicroQuadrotor::MakeArm() {
      SetBodyMaterial();
      glBegin(GL_QUADS);
      /* Bottom face, parallel to XY */
      glNormal3f( 0.0f,            0.0f,            -1.0f      );
      glVertex3f( ARM_HALF_WIDTH,  ARM_HALF_LENGTH,  ARM_BOTTOM);
      glVertex3f( ARM_HALF_WIDTH, -ARM_HALF_LENGTH,  ARM_BOTTOM);
      glVertex3f(-ARM_HALF_WIDTH, -ARM_HALF_LENGTH,  ARM_BOTTOM);
      glVertex3f(-ARM_HALF_WIDTH,  ARM_HALF_LENGTH,  ARM_BOTTOM);
      /* Top face, parallel to XY */
      glNormal3f( 0.0f,            0.0f,             1.0f      );
      glVertex3f( ARM_HALF_WIDTH,  ARM_HALF_LENGTH,  ARM_TOP   );
      glVertex3f(-ARM_HALF_WIDTH,  ARM_HALF_LENGTH,  ARM_TOP   );
      glVertex3f(-ARM_HALF_WIDTH, -ARM_HALF_LENGTH,  ARM_TOP   );
      glVertex3f( ARM_HALF_WIDTH, -ARM_HALF_LENGTH,  ARM_TOP   );
      /* This part covers the faces (N, E, S, W) */
      /* North face */
      glNormal3f( 1.0f,            0.0f,             0.0f      );
      glVertex3f( ARM_HALF_WIDTH,  ARM_HALF_LENGTH,  ARM_TOP   );
      glVertex3f( ARM_HALF_WIDTH,  ARM_HALF_LENGTH,  ARM_BOTTOM);
      glVertex3f(-ARM_HALF_WIDTH,  ARM_HALF_LENGTH,  ARM_BOTTOM);
      glVertex3f(-ARM_HALF_WIDTH,  ARM_HALF_LENGTH,  ARM_TOP   );
      /* East face */
      glNormal3f( 0.0f,            1.0f,             0.0f      );
      glVertex3f( ARM_HALF_WIDTH,  ARM_HALF_LENGTH,  ARM_TOP   );
      glVertex3f( ARM_HALF_WIDTH, -ARM_HALF_LENGTH,  ARM_TOP   );
      glVertex3f( ARM_HALF_WIDTH, -ARM_HALF_LENGTH,  ARM_BOTTOM);
      glVertex3f( ARM_HALF_WIDTH,  ARM_HALF_LENGTH,  ARM_BOTTOM);
      /* South face */
      glNormal3f(-1.0f,            0.0f,             0.0f      );
      glVertex3f( ARM_HALF_WIDTH, -ARM_HALF_LENGTH,  ARM_TOP   );
      glVertex3f(-ARM_HALF_WIDTH, -ARM_HALF_LENGTH,  ARM_TOP   );
      glVertex3f(-ARM_HALF_WIDTH, -ARM_HALF_LENGTH,  ARM_BOTTOM);
      glVertex3f( ARM_HALF_WIDTH, -ARM_HALF_LENGTH,  ARM_BOTTOM);
      /* West face */
      glNormal3f( 0.0f,           -1.0f,             0.0f      );
      glVertex3f(-ARM_HALF_WIDTH,  ARM_HALF_LENGTH,  ARM_TOP   );
      glVertex3f(-ARM_HALF_WIDTH,  ARM_HALF_LENGTH,  ARM_BOTTOM);
      glVertex3f(-ARM_HALF_WIDTH, -ARM_HALF_LENGTH,  ARM_BOTTOM);
      glVertex3f(-ARM_HALF_WIDTH, -ARM_HALF_LENGTH,  ARM_TOP   );
      glEnd();      
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLKumarMicroQuadrotor::MakePropeller() {
      SetBodyMaterial();
      /* Rod side surface */
      CVector2 cNormal(1.0f, 0.0f);
      CVector2 cVertex;
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), 0.0f);
         cVertex = cNormal;
         cVertex *= PROPELLER_ROD_RADIUS;
         glVertex3f(cVertex.GetX(), cVertex.GetY(), PROPELLER_ROD_TOP);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), PROPELLER_ROD_BOTTOM);
         cNormal.Rotate(cAngle);
      }
      glEnd();
      /* Rod top disk */
      cVertex.Set(PROPELLER_ROD_RADIUS, 0.0f);
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), PROPELLER_ROD_TOP);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Rod bottom disk */
      cVertex.Set(PROPELLER_ROD_RADIUS, 0.0f);
      cAngle = -cAngle;
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, -1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), PROPELLER_ROD_BOTTOM);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      /* Rotating blade disks */
      SetBladeMaterial();
      cVertex.Set(PROPELLER_BLADE_RADIUS, 0.0f);
      cAngle = -cAngle;
      glDisable(GL_CULL_FACE);
      glBegin(GL_POLYGON);
      glNormal3f(0.0f, 0.0f, 1.0f);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glVertex3f(cVertex.GetX(), cVertex.GetY(), PROPELLER_BLADE_ELEVATION);
         cVertex.Rotate(cAngle);
      }
      glEnd();
      glEnable(GL_CULL_FACE);
   }

   /****************************************/
   /****************************************/

}
