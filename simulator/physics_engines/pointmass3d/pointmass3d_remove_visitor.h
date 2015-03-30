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
 * @file <argos2/simulator/physics_engines/pointmass3d/pointmass3d_remove_visitor.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef POINTMASS3D_REMOVE_VISITOR_H
#define POINTMASS3D_REMOVE_VISITOR_H

namespace argos {
   class CEntity;
   class CPointMass3DEngine;
}

#include <argos2/simulator/space/entities/entity_visitor.h>
namespace argos {

   class CPointMass3DRemoveVisitor : public CEntityVisitor {

   public:

      CPointMass3DRemoveVisitor(CPointMass3DEngine& c_engine) :
         m_cEngine(c_engine) {}
      virtual ~CPointMass3DRemoveVisitor() {}

      virtual void Visit(CBoothEntity& c_entity) {}
      virtual void Visit(CBoxEntity& c_entity) {}
      virtual void Visit(CCylinderEntity& c_entity) {}
      virtual void Visit(CTileEntity& c_entity) {}
      virtual void Visit(CBluebotEntity& c_entity) {}
      virtual void Visit(CFloorEntity& c_entity) {}
      virtual void Visit(CFootBotEntity& c_entity) {}
      virtual void Visit(CEyeBotEntity& c_entity);
      virtual void Visit(CKumarMicroQuadrotorEntity& c_entity) {}
      virtual void Visit(CLightEntity& c_entity) {}
      virtual void Visit(CEPuckEntity& c_entity) {}
      virtual void Visit(CWiFiMediumEntity& c_entity) {}

   private:

      CPointMass3DEngine& m_cEngine;
       
   };
}

#endif
