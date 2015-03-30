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
 * @file <simulator/visualizations/models/qtopengl_tile.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "qtopengl_tile.h"
#include <argos2/common/utility/math/vector2.h>
#include <argos2/simulator/space/entities/tile_entity.h>
#include <argos2/simulator/space/entities/led_equipped_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

    CQTOpenGLTile::CQTOpenGLTile() :
       m_unVertices(20){

       /* Reserve the needed display lists */
       m_unBaseList = glGenLists(2);
       m_unBodyList = m_unBaseList;
       m_unLockList = m_unBaseList + 1;

       /* Make body list */
       glNewList(m_unBodyList, GL_COMPILE);
       MakeBody();
       glEndList();

       /* Make lock list */
       glNewList(m_unLockList, GL_COMPILE);
       MakeLock();
       glEndList();

    }

   /****************************************/
   /****************************************/

   CQTOpenGLTile::~CQTOpenGLTile() {
      glDeleteLists(m_unBaseList, 2);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLTile::Draw(const CTileEntity& c_entity) {
      /* Set the material */
      GLfloat pfColor[]     = { 0.0f, 1.0f, 0.0f, 1.0f };
      const GLfloat pfSpecular[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfShininess[] = { 0.0f                   };
      const GLfloat pfEmission[]  = { 0.0f, 0.0f, 0.0f, 1.0f };

      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, pfEmission);

      /* Draw the body */
      glPushMatrix();
      glScalef(c_entity.GetSize().GetX(), c_entity.GetSize().GetY(), c_entity.GetSize().GetZ());
      glCallList(m_unBodyList);
      glPopMatrix();

      /*
       * Draw the locks
       */
      /* Right lock */
      glPushMatrix();
      glScalef(c_entity.GetSize().GetX(), c_entity.GetSize().GetY(), c_entity.GetSize().GetZ());
      for(size_t i = 0; i < 4; ++i) {
         if(c_entity.GetLock(i) == 0) {
            pfColor[0] = 0.0f;
            pfColor[1] = 0.0f;
            pfColor[2] = 0.0f;
         }
         else {
            pfColor[0] = 1.0f;
            pfColor[1] = 1.0f;
            pfColor[2] = 1.0f;
         }
         glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
         glCallList(m_unLockList);
         glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
      }
      glPopMatrix();
      
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLTile::MakeBody() {
	     /* Since this shape can be stretched,
	         make sure the normal vectors are unit-long */
	      glEnable(GL_NORMALIZE);

	      /* Let's start the actual shape */

	      /* This part covers the bottom face (parallel to XY) */
	      glBegin(GL_QUADS);
	      /* Bottom face */
	      glNormal3f(0.0f, 0.0f, -1.0f);
	      glVertex3f( 0.5f,  0.5f, -0.5f);
	      glVertex3f( 0.5f, -0.5f, -0.5f);
	      glVertex3f(-0.5f, -0.5f, -0.5f);
	      glVertex3f(-0.5f,  0.5f, -0.5f);
	      glEnd();
	      /* This part covers the faces (South, East, North, West) */
	      glBegin(GL_QUADS);
	      /* South face */
	          glNormal3f(0.0f, -1.0f, 0.0f);
	          glVertex3f(-0.5f, -0.5f,  0.5f);
	          glVertex3f(-0.5f, -0.5f, -0.5f);
	          glVertex3f( 0.5f, -0.5f, -0.5f);
	          glVertex3f( 0.5f, -0.5f,  0.5f);
	          /* East face */
	          glNormal3f(1.0f, 0.0f, 0.0f);
	          glVertex3f( 0.5f, -0.5f,  0.5f);
	          glVertex3f( 0.5f, -0.5f, -0.5f);
	          glVertex3f( 0.5f,  0.5f, -0.5f);
	          glVertex3f( 0.5f,  0.5f,  0.5f);
	          /* North face */
	          glNormal3f(0.0f, 1.0f, 0.0f);
	          glVertex3f( 0.5f,  0.5f,  0.5f);
	          glVertex3f( 0.5f,  0.5f, -0.5f);
	          glVertex3f(-0.5f,  0.5f, -0.5f);
	          glVertex3f(-0.5f,  0.5f,  0.5f);
	          /* West face */
	          glNormal3f(-1.0f, 0.0f, 0.0f);
	          glVertex3f(-0.5f,  0.5f,  0.5f);
	          glVertex3f(-0.5f,  0.5f, -0.5f);
	          glVertex3f(-0.5f, -0.5f, -0.5f);
	          glVertex3f(-0.5f, -0.5f,  0.5f);
	      glEnd();
	      /* The shape definitions is finished */

	      /* We don't need it anymore */
	      glDisable(GL_NORMALIZE);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLTile::MakeLock() {
	     /* Since this shape can be stretched,
	         make sure the normal vectors are unit-long */
	      glEnable(GL_NORMALIZE);

	      /* Let's start the actual shape */

	      /* This part covers the right lock (parallel to XY) */
	      glBegin(GL_TRIANGLES);
	      /* Top face */
	      glNormal3f(0.0f, 0.0f, 1.0f);
	      glVertex3f( 0.0f,  0.0f, 0.5f);
	      glVertex3f( 0.5f, -0.5f, 0.5f);
	      glVertex3f( 0.5f,  0.5f, 0.5f);
	      glEnd();
	      /* The shape definitions is finished */

	      /* We don't need it anymore */
	      glDisable(GL_NORMALIZE);
   }

   /****************************************/
   /****************************************/

}
