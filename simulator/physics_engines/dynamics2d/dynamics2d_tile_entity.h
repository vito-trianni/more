/* -*- Mode: C++ -*-
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
 * @file <argos2/simulator/physics_engines/dynamics2d_tile_entity.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 * @author Dhananjay Ipparthi - <ipparthi@gmail.com>
 */

#ifndef DYNAMICS2D_TILE_ENTITY_H
#define DYNAMICS2D_TILE_ENTITY_H

#include <argos2/simulator/physics_engines/dynamics2d/dynamics2d_entity.h>
#include <argos2/simulator/space/entities/tile_entity.h>
#include <argos2/common/utility/argos_random.h>

namespace argos {

   class CDynamics2DTileEntity : public CDynamics2DEntity {

   public:

      CDynamics2DTileEntity(CDynamics2DEngine& c_engine,
                           CTileEntity& c_entity);
      virtual ~CDynamics2DTileEntity();

      inline virtual bool CheckIntersectionWithRay(Real& f_t_on_ray,
                                                   const CRay& c_ray) const {
         return false;
      }

      virtual bool MoveTo(const CVector3& c_position,
                          const CQuaternion& c_orientation,
                          bool b_check_only = false);

      virtual void Reset();

      virtual void UpdateEntityStatus();
      virtual void UpdateFromEntityStatus();

   private:

      CTileEntity& m_cTileEntity;
      cpFloat       m_fMass;
      cpShape*      m_ptShape;
      cpShape*      m_ptInteractionRangeShapes[4];
      cpBody*       m_ptBody;
      CARGoSRandom::CRNG* m_pcRNG;

   };

}

#endif
