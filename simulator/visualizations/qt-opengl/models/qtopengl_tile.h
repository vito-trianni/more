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

#ifndef QTOPENGL_TILE_H
#define QTOPENGL_TILE_H

namespace argos {
   class CQTOpenGLTile;
   class CTileEntity;
}

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

   class CQTOpenGLTile {

   public:

      CQTOpenGLTile();

      virtual ~CQTOpenGLTile();

      virtual void Draw(const CTileEntity& c_entity);

   private:

      void MakeBody();

      void MakeLock();

   private:

      GLuint m_unBaseList;
      GLuint m_unBodyList;
      GLuint m_unLockList;
      GLuint m_unVertices;

   };

}

#endif
