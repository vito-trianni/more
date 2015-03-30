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
 * @file <argos2/simulator/physics_engines/dynamics2d_tile_entity.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 * @author Dhananjay Ipparthi - <ipparthi@gmail.com>
 */

#include "dynamics2d_tile_entity.h"
#include "dynamics2d_engine.h"

namespace argos {

   /****************************************/
   /****************************************/

   CDynamics2DTileEntity::CDynamics2DTileEntity(CDynamics2DEngine& c_engine,
                                                CTileEntity& c_entity) :
      CDynamics2DEntity(c_engine, c_entity),
      m_cTileEntity(c_entity),
      m_fMass(c_entity.GetMass()),
      m_ptShape(NULL),
      m_ptBody(NULL) {
      /* Get the size of the entity */
      Real fHalfSideLength = c_entity.GetSideLength() * 0.5f;
      /* Create a polygonal object in the physics space */
      /* Start defining the vertices
         NOTE: points must be defined in a clockwise winding
      */
      cpVect tVertices[] = {
         cpv(-fHalfSideLength, -fHalfSideLength),
         cpv(-fHalfSideLength,  fHalfSideLength),
         cpv( fHalfSideLength,  fHalfSideLength),
         cpv( fHalfSideLength, -fHalfSideLength)
      };
      const CVector3& cPosition = GetEmbodiedEntity().GetPosition();
      CRadians cXAngle, cYAngle, cZAngle;
      GetEmbodiedEntity().GetOrientation().ToEulerAngles(cZAngle, cYAngle, cXAngle);
      /* Create the body */
      m_ptBody =
         cpSpaceAddBody(m_cEngine.GetPhysicsSpace(),
                        cpBodyNew(m_fMass,
                                  cpMomentForPoly(m_fMass,
                                                  4,
                                                  tVertices,
                                                  cpvzero)));
      m_ptBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      cpBodySetAngle(m_ptBody, cZAngle.GetValue());
      /* Create the geometry */
      m_ptShape =
         cpSpaceAddShape(m_cEngine.GetPhysicsSpace(),
                         cpPolyShapeNew(m_ptBody,
                                        4,
                                        tVertices,
                                        cpvzero));
      cpShapeSetCollisionType(m_ptShape, CDynamics2DEngine::SHAPE_TILE_BODY);
      /*
       * Add interaction shapes
       */
      Real fHalfRange = c_entity.GetInteractionRange() * 0.5f;
      cpVect tUpDownRangeVertices[] = {
         cpv(-fHalfSideLength * 0.9f, -fHalfRange),
         cpv(-fHalfSideLength * 0.9f,  fHalfRange),
         cpv( fHalfSideLength * 0.9f,  fHalfRange),
         cpv( fHalfSideLength * 0.9f, -fHalfRange)
      };
      cpVect tLeftRightRangeVertices[] = {
         cpv(-fHalfRange, -fHalfSideLength * 0.9f),
         cpv(-fHalfRange,  fHalfSideLength * 0.9f),
         cpv( fHalfRange,  fHalfSideLength * 0.9f),
         cpv( fHalfRange, -fHalfSideLength * 0.9f),
      };
      /* Right */
      m_ptInteractionRangeShapes[CTileEntity::LOCK_RIGHT] =
         cpSpaceAddShape(m_cEngine.GetPhysicsSpace(),
                         cpPolyShapeNew(m_ptBody,
                                        4,
                                        tLeftRightRangeVertices,
                                        cpv(fHalfSideLength + fHalfRange,
                                            0.0f)));
      /* Up */
      m_ptInteractionRangeShapes[CTileEntity::LOCK_UP] =
         cpSpaceAddShape(m_cEngine.GetPhysicsSpace(),
                         cpPolyShapeNew(m_ptBody,
                                        4,
                                        tUpDownRangeVertices,
                                        cpv(0.0f,
                                            fHalfSideLength + fHalfRange)));
      /* Left */
      m_ptInteractionRangeShapes[CTileEntity::LOCK_LEFT] =
         cpSpaceAddShape(m_cEngine.GetPhysicsSpace(),
                         cpPolyShapeNew(m_ptBody,
                                        4,
                                        tLeftRightRangeVertices,
                                        cpv(-fHalfSideLength - fHalfRange,
                                            0.0f)));
      /* Down */
      m_ptInteractionRangeShapes[CTileEntity::LOCK_DOWN] =
         cpSpaceAddShape(m_cEngine.GetPhysicsSpace(),
                         cpPolyShapeNew(m_ptBody,
                                        4,
                                        tUpDownRangeVertices,
                                        cpv(0.0f,
                                            -fHalfSideLength - fHalfRange)));
      /* Set attributes */
      for(size_t i = 0; i < 4; ++i) {
         cpShapeSetSensor(m_ptInteractionRangeShapes[i], true);
         cpShapeSetCollisionType(m_ptInteractionRangeShapes[i],
                                 CDynamics2DEngine::SHAPE_TILE_INTERACTION_AREA);
         cpShapeSetUserData(m_ptInteractionRangeShapes[i],
                            new SDynamics2DEngineTileInteractionAreaData(c_entity.GetId(), i, c_entity.GetLock(i)));
      }
      /* Add the interaction shapes as user data to the body */
      cpBodySetUserData(m_ptBody, m_ptInteractionRangeShapes);
      /* This object is grippable */
      //m_ptShape->collision_type = CDynamics2DEngine::SHAPE_GRIPPABLE;
      //m_ptShape->data = reinterpret_cast<void*>(&c_entity);
      /* Lots of elasticity to help solving overlaps (1.0 makes the collision too bouncey) */
      m_ptShape->e = 0.9;
      /* Lots contact friction to help pushing */
      m_ptShape->u = 0.7;
      /* Random number generator*/
      m_pcRNG = CARGoSRandom::CreateRNG("argos");
   }
   
   /****************************************/
   /****************************************/

   CDynamics2DTileEntity::~CDynamics2DTileEntity() {
      cpSpaceRemoveBody(m_cEngine.GetPhysicsSpace(), m_ptBody);
      cpBodyFree(m_ptBody);
      for(size_t i = 0; i < 4; ++i) {
         delete reinterpret_cast<SInt32*>(cpShapeGetUserData(m_ptInteractionRangeShapes[i]));
         cpSpaceRemoveShape(m_cEngine.GetPhysicsSpace(), m_ptInteractionRangeShapes[i]);
         cpShapeFree(m_ptInteractionRangeShapes[i]);
      }
      cpSpaceRemoveShape(m_cEngine.GetPhysicsSpace(), m_ptShape);
      cpShapeFree(m_ptShape);
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DTileEntity::MoveTo(const CVector3& c_position,
                                         const CQuaternion& c_orientation,
                                         bool b_check_only) {
      SInt32 nCollision;
      /* Save body position and orientation */
      cpVect tOldPos = m_ptBody->p;
      cpFloat fOldA = m_ptBody->a;
      /* Move the body to the desired position */
      m_ptBody->p = cpv(c_position.GetX(), c_position.GetY());
      CRadians cXAngle, cYAngle, cZAngle;
      c_orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(m_ptBody, cZAngle.GetValue());
      /* Create a shape sensor to test the movement */
      /* First construct the vertices */
      CVector3 cHalfSize = m_cTileEntity.GetSize() * 0.5f;
      cpVect tVertices[] = {
         cpv(-cHalfSize.GetX(), -cHalfSize.GetY()),
         cpv(-cHalfSize.GetX(),  cHalfSize.GetY()),
         cpv( cHalfSize.GetX(),  cHalfSize.GetY()),
         cpv( cHalfSize.GetX(), -cHalfSize.GetY())
      };
      /* Then create the shape itself */
      cpShape* ptTestShape = cpPolyShapeNew(m_ptBody,
                                            4,
                                            tVertices,
                                            cpvzero);
      /* Check if there is a collision */
      nCollision = cpSpaceShapeQuery(m_cEngine.GetPhysicsSpace(), ptTestShape, NULL, NULL);
      /* Dispose of the sensor shape */
      cpShapeFree(ptTestShape);
      if(b_check_only || nCollision) {
         /* Restore old body state if there was a collision or
            it was only a check for movement */
         m_ptBody->p = tOldPos;
         cpBodySetAngle(m_ptBody, fOldA);
      }
      else {
         /* Update the active space hash if the movement is actual */
         cpSpaceReindexShape(m_cEngine.GetPhysicsSpace(), m_ptShape);
      }
      /* The movement is allowed if there is no collision */
      return !nCollision;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DTileEntity::Reset() {
      /* Reset body position */
      const CVector3& cPosition = GetEmbodiedEntity().GetInitPosition();
      m_ptBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      /* Reset body orientation */
      CRadians cXAngle, cYAngle, cZAngle;
      GetEmbodiedEntity().GetInitOrientation().ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(m_ptBody, cZAngle.GetValue());
      /* Zero speed and applied forces */
      m_ptBody->v = cpvzero;
      m_ptBody->w = 0.0f;
      cpBodyResetForces(m_ptBody);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DTileEntity::UpdateFromEntityStatus() {
      cpBodyResetForces(m_ptBody);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DTileEntity::UpdateEntityStatus() {
      m_cEngine.PositionPhysicsToSpace(m_cSpacePosition, GetEmbodiedEntity().GetPosition(), m_ptBody);
      GetEmbodiedEntity().SetPosition(m_cSpacePosition);
      m_cEngine.OrientationPhysicsToSpace(m_cSpaceOrientation, m_ptBody);
      GetEmbodiedEntity().SetOrientation(m_cSpaceOrientation);
      m_cTileEntity.CalculateBoundingBox();
   }

   /****************************************/
   /****************************************/

}
