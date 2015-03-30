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
 * @file <argos2/simulator/physics_engines/dynamics2d/dynamics2d_engine.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "dynamics2d_engine.h"
#include <argos2/simulator/simulator.h>
#include <argos2/simulator/space/entities/embodied_entity.h>
#include <argos2/simulator/space/entities/controllable_entity.h>
#include <argos2/simulator/space/entities/gripper_equipped_entity.h>

#include <cmath>

namespace argos {

   /****************************************/
   /****************************************/

   /* How many possible types of locks for each side of the tiles */
   const UInt32 TILE_LOCK_NUMBER = 8;

   /****************************************/
   /****************************************/

   SDynamics2DEngineGripperData::SDynamics2DEngineGripperData(cpSpace* pt_space,
                                                              CGripperEquippedEntity& c_entity,
                                                              cpVect t_anchor) :
      Space(pt_space),
      GripperEntity(c_entity),
      GripperAnchor(t_anchor),
      GripConstraint(NULL) {}

   SDynamics2DEngineGripperData::~SDynamics2DEngineGripperData() {
      ClearConstraints();
   }

   void SDynamics2DEngineGripperData::ClearConstraints() {
      if(GripConstraint != NULL) {
         cpSpaceRemoveConstraint(Space, GripConstraint);
         cpConstraintFree(GripConstraint);
         GripConstraint = NULL;
         GripperEntity.ClearGrippedEntity();
      }
   }

   /****************************************/
   /****************************************/

   void MagneticGripperGrippableCollisionPostStep(cpSpace* pt_space, void* p_obj, void* p_data) {
      /* Get the shapes involved */
      cpShape* ptGripperShape = reinterpret_cast<cpShape*>(p_obj);
      cpShape* ptGrippableShape = reinterpret_cast<cpShape*>(p_data);
      /* Get a reference to the gripper data */
      SDynamics2DEngineGripperData& sGripperData = *reinterpret_cast<SDynamics2DEngineGripperData*>(ptGripperShape->data);
      /* The gripper is locked or unlocked? */
      if(sGripperData.GripperEntity.IsUnlocked()) {
         /* The gripper is locked. If it was gripping an object,
          * release it. Then, process the collision normally */
      	 if(sGripperData.GripperEntity.IsGripping()) {
            sGripperData.ClearConstraints();
      	 }
      }
      else if(! sGripperData.GripperEntity.IsGripping()) {
         /* The gripper is unlocked and free, create the joints */
         /* Create a constraint */
         sGripperData.GripConstraint = cpSpaceAddConstraint(pt_space,
                                                            cpPivotJointNew(
                                                               ptGripperShape->body,
                                                               ptGrippableShape->body,
                                                               sGripperData.GripperAnchor));
         sGripperData.GripConstraint->maxBias = 0.95f; // Correct overlap
         sGripperData.GripConstraint->maxForce = 10000.0f; // Max correction speed
         sGripperData.GripperEntity.SetGrippedEntity(*reinterpret_cast<CEmbodiedEntity*>(ptGrippableShape->data));
      }
   }

   int MagneticGripperGrippableCollisionBegin(cpArbiter* pt_arb, cpSpace* pt_space, void* p_data) {
      /* Get the shapes involved */
      CP_ARBITER_GET_SHAPES(pt_arb, ptGripperShape, ptGrippableShape);
      /* Get a reference to the gripper data */
      SDynamics2DEngineGripperData& sGripperData = *reinterpret_cast<SDynamics2DEngineGripperData*>(ptGripperShape->data);
      /* Get a reference to the gripped entity */
      CEmbodiedEntity& cGrippedEntity = *reinterpret_cast<CEmbodiedEntity*>(ptGrippableShape->data);
      /* If the entities match, ignore the collision forever */
      return (sGripperData.GripperEntity.GetId() != cGrippedEntity.GetId());
   }

   int MagneticGripperGrippableCollisionPreSolve(cpArbiter* pt_arb, cpSpace* pt_space, void* p_data) {
      /* Get the shapes involved */
      CP_ARBITER_GET_SHAPES(pt_arb, ptGripperShape, ptGrippableShape);
      /* Get a reference to the gripper data */
      SDynamics2DEngineGripperData& sGripperData = *reinterpret_cast<SDynamics2DEngineGripperData*>(ptGripperShape->data);
      /*
       * When to process gripping:
       * 1. when the robot was gripping and it just unlocked the gripper
       * 2. when the robot was not gripping and it just locked the gripper
       *    in this case, also precalculate the anchor point
       * Otherwise ignore it
       */
      bool bGrippingJustUnlocked = (sGripperData.GripperEntity.IsGripping() && sGripperData.GripperEntity.IsUnlocked());
      bool bNotGrippingJustLocked = (!sGripperData.GripperEntity.IsGripping() && sGripperData.GripperEntity.IsLocked());
      if(bNotGrippingJustLocked) {
         /* Calculate the anchor point on the grippable body
            as the centroid of the contact points */
         sGripperData.GripperAnchor = cpvzero;
         for(SInt32 i = 0; i < cpArbiterGetCount(pt_arb); ++i) {
            sGripperData.GripperAnchor = cpvadd(sGripperData.GripperAnchor, cpArbiterGetPoint(pt_arb, i));
         }
         sGripperData.GripperAnchor = cpvmult(sGripperData.GripperAnchor, 1.0f / cpArbiterGetCount(pt_arb));
      }
      if(bGrippingJustUnlocked || bNotGrippingJustLocked) {
         /* Instruct the engine to process gripping in a post-step callback */
         cpSpaceAddPostStepCallback(
            pt_space,
            MagneticGripperGrippableCollisionPostStep,
            ptGripperShape,
            ptGrippableShape
            );
      }
      /* Always return false, anyway the gripper is a sensor shape */
      return false;
   }

   int TileBodyCollisionPreSolve(cpArbiter* pt_arb, cpSpace* pt_space, void* p_data) {
      /* Get the bodies involved */
      CP_ARBITER_GET_BODIES(pt_arb, ptTile1, ptTile2);
      /*
       * Calculate distance vector
       * The distance vector goes from body 1 to body 2
       */
      cpVect tPos1 = cpBodyGetPos(ptTile1);
      cpVect tPos2 = cpBodyGetPos(ptTile2);
      cpVect tDistance = cpvsub(tPos2, tPos1);
      if(cpvlength(tDistance) < 0.101f) {
         CRadians cDistAngle = CRadians(cpvtoangle(tDistance));
         cDistAngle.UnsignedNormalize();
         /* Engage lock shape of tile 1 */
         CRadians cTile1Angle = CRadians(cpBodyGetAngle(ptTile1));
         cTile1Angle.UnsignedNormalize();
         size_t unLock1Idx = 2.0f * ( cDistAngle - cTile1Angle + CRadians::PI_OVER_FOUR ).UnsignedNormalize() / CRadians::PI;
         cpShape* ptLock1Shape = reinterpret_cast<cpShape**>(cpBodyGetUserData(ptTile1))[unLock1Idx];
         SDynamics2DEngineTileInteractionAreaData& sLock1Data = *reinterpret_cast<SDynamics2DEngineTileInteractionAreaData*>(cpShapeGetUserData(ptLock1Shape));
         sLock1Data.Engaged = true;
         /* Engage lock shape of tile 2 */
         CRadians cTile2Angle = CRadians(cpBodyGetAngle(ptTile2));
         cTile2Angle.UnsignedNormalize();
         size_t unLock2Idx = 2.0f * ( cDistAngle + CRadians::PI - cTile2Angle + CRadians::PI_OVER_FOUR ).UnsignedNormalize() / CRadians::PI;
         cpShape* ptLock2Shape = reinterpret_cast<cpShape**>(cpBodyGetUserData(ptTile2))[unLock2Idx];
         SDynamics2DEngineTileInteractionAreaData& sLock2Data = *reinterpret_cast<SDynamics2DEngineTileInteractionAreaData*>(cpShapeGetUserData(ptLock2Shape));
         sLock2Data.Engaged = true;
         LOG << "OBJECTS ENGAGED" << std::endl;
      }
      return true;
   }

   int TileInteractionBegin(cpArbiter* pt_arb, cpSpace* pt_space, void* p_data) {
      /* Get the bodies involved */
      CP_ARBITER_GET_BODIES(pt_arb, ptBody1, ptBody2);
      /* If two areas of the same tile interact, ignore it */
      return ptBody1 != ptBody2;
   }

   int TileInteractionPreSolve(cpArbiter* pt_arb, cpSpace* pt_space, void* p_data) {
      /* Get the shapes involved */
      CP_ARBITER_GET_SHAPES(pt_arb, ptArea1, ptArea2);
      CP_ARBITER_GET_BODIES(pt_arb, ptBody1, ptBody2);
      /* Get the lock data */
      SDynamics2DEngineTileInteractionAreaData& sLock1Data = *reinterpret_cast<SDynamics2DEngineTileInteractionAreaData*>(cpShapeGetUserData(ptArea1));
      SDynamics2DEngineTileInteractionAreaData& sLock2Data = *reinterpret_cast<SDynamics2DEngineTileInteractionAreaData*>(cpShapeGetUserData(ptArea2));
      /* Are both locks non-engaged and non-neutral? */
      if( (! sLock1Data.Engaged && ! sLock2Data.Engaged) &&
          (sLock1Data.Lock >= 0 && sLock2Data.Lock >= 0) ) {
         /* Yes, interaction ongoing */
         /*
          * Calculate direction
          * +1 => attraction
          * -1 => repulsion
          */
         //LOG << sLock1Data.Id << " LOCK " << sLock1Data.Idx << " -> "
         //    << sLock2Data.Id << " LOCK " << sLock2Data.Idx << " MAGNITUDE=";
         Real fDirection = -1.0f;
         if( sLock1Data.Lock + sLock2Data.Lock == (TILE_LOCK_NUMBER-1) ) {
            fDirection = 1.0f;
         }
         cpVect tPos1 = cpBodyGetPos(ptBody1);
         cpVect tPos2 = cpBodyGetPos(ptBody2);
         /* The distance vector goes from body 1 to body 2 */
         cpVect tDistance = cpvsub(tPos2, tPos1);
         /*
          * The force magnitude is given by
          * K / (4*PI*d^2)
          * We found a value for K graphically: 1/25.
          */
         Real fForceMag = cpfclamp(fDirection / (100.0f * ARGOS_PI * cpvlengthsq(tDistance)), -1.0f, 1.0f);
         cpVect tForce = cpvmult(
            cpvnormalize(tDistance), fForceMag);
         //LOG << fForceMag << " DISTANCE=" << cpvlength(tDistance) << std::endl;
         cpBodyApplyForce(ptBody1, tForce, cpvzero);
         cpBodyApplyForce(ptBody2, cpvmult(tForce, -1.0f), cpvzero);
      }
      return true;
   }

   void GrippableNormalCollisionPostSolve(cpArbiter* pt_arb, cpSpace* pt_space, void* p_data) {
  	 cpVect totalImpulse = cpArbiterTotalImpulse(pt_arb);
  	 cpVect totalImpulseWithFriction = cpArbiterTotalImpulseWithFriction(pt_arb);
   }

   void GrippableGrippableCollisionPostSolve(cpArbiter* pt_arb, cpSpace* pt_space, void* p_data) {
  	 cpVect totalImpulse = cpArbiterTotalImpulse(pt_arb);
  	 cpVect totalImpulseWithFriction = cpArbiterTotalImpulseWithFriction(pt_arb);
   }

   /****************************************/
   /****************************************/

   CDynamics2DEngine::CDynamics2DEngine() :
      m_cAddVisitor(*this),
      m_cRemoveVisitor(*this),
      m_nIterations(10),
      m_fStaticHashCellSize(0.1f),
      m_fActiveHashCellSize(2.0f * 0.085036758f),
      m_nStaticHashCells(1000),
      m_nActiveHashCells(1000),
      m_ptSpace(NULL),
      m_ptGroundBody(NULL),
      m_fElevation(0.0f),
      m_bEntityTransferActive(false) {
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::Init(TConfigurationNode& t_tree) {
      /* Init parent */
      CPhysicsEngine::Init(t_tree);
      /* Parse XML */
      GetNodeAttributeOrDefault(t_tree, "iterations",       m_nIterations,         m_nIterations);
      GetNodeAttributeOrDefault(t_tree, "static_cell_size", m_fStaticHashCellSize, m_fStaticHashCellSize);
      GetNodeAttributeOrDefault(t_tree, "active_cell_size", m_fActiveHashCellSize, m_fActiveHashCellSize);
      GetNodeAttributeOrDefault(t_tree, "static_cells",     m_nStaticHashCells,    m_nStaticHashCells);
      GetNodeAttributeOrDefault(t_tree, "active_cells",     m_nActiveHashCells,    m_nActiveHashCells);
      GetNodeAttributeOrDefault(t_tree, "elevation",        m_fElevation,          m_fElevation);
      if(NodeExists(t_tree, "boundaries")) {
         /* Parse the boundary definition */
         TConfigurationNode& tBoundaries = GetNode(t_tree, "boundaries");
         SBoundarySegment sBoundSegment;
         CVector2 cLastPoint, cCurPoint;
         std::string strConnectWith;
         TConfigurationNodeIterator tVertexIt("vertex");
         /* Get the first vertex */
         tVertexIt = tVertexIt.begin(&tBoundaries);
         if(tVertexIt == tVertexIt.end()) {
            THROW_ARGOSEXCEPTION("Physics engine of type \"dynamics2d\", id \"" << GetId() << "\": you didn't specify any <vertex>!");
         }
         GetNodeAttribute(*tVertexIt, "point", cLastPoint);
         m_vecVertices.push_back(cLastPoint);
         /* Go through the other vertices */
         ++tVertexIt;
         while(tVertexIt != tVertexIt.end()) {
            /* Read vertex data and fill in segment struct */
            GetNodeAttribute(*tVertexIt, "point", cCurPoint);
            m_vecVertices.push_back(cCurPoint);
            sBoundSegment.Segment.SetStart(cLastPoint);
            sBoundSegment.Segment.SetEnd(cCurPoint);
            GetNodeAttribute(*tVertexIt, "connect_with", strConnectWith);
            if(strConnectWith == "gate") {
               /* Connect to previous vertex with a gate */
               sBoundSegment.Type = SBoundarySegment::SEGMENT_TYPE_GATE;
               GetNodeAttribute(*tVertexIt, "to_engine", sBoundSegment.EngineId);
            }
            else if(strConnectWith == "wall") {
               /* Connect to previous vertex with a wall */
               sBoundSegment.Type = SBoundarySegment::SEGMENT_TYPE_WALL;
               sBoundSegment.EngineId = "";
            }
            else {
               /* Parse error */
               THROW_ARGOSEXCEPTION("Physics engine of type \"dynamics2d\", id \"" << GetId() << "\": unknown vertex connection method \"" << strConnectWith << "\". Allowed methods are \"wall\" and \"gate\".");
            }
            m_vecSegments.push_back(sBoundSegment);
            /* Next vertex */
            cLastPoint = cCurPoint;
            ++tVertexIt;
         }
         /* Check that the boundary is a closed path */
         if(m_vecVertices.front() != m_vecVertices.back()) {
            THROW_ARGOSEXCEPTION("Physics engine of type \"dynamics2d\", id \"" << GetId() << "\": the specified path is not closed. The first and last points of the boundaries MUST be the same.");
         }
      }
      /* Initialize physics */
      cpInitChipmunk();
      cpResetShapeIdCounter();
      /* Used to attach static geometries so that they won't move and to simulate friction */
      m_ptGroundBody = cpBodyNew(INFINITY, INFINITY);
      /* Create the space to contain the movable objects */
      m_ptSpace = cpSpaceNew();
      /* Subiterations to solve constraints.
         The more, the better for precision but the worse for speed
      */
      m_ptSpace->iterations = m_nIterations;
      /* Resize the space hash.
         This has dramatic effects on performance.
         TODO: - find optimal parameters automatically (average entity size)
      cpSpaceReindexStaticHash(m_ptSpace, m_fStaticHashCellSize, m_nStaticHashCells);
      cpSpaceResizeActiveHash(m_ptSpace, m_fActiveHashCellSize, m_nActiveHashCells);
      */
      /* Gripper-Gripped callback functions */
      /*
      cpSpaceAddCollisionHandler(
         m_ptSpace,
         SHAPE_TILE_BODY,
         SHAPE_TILE_BODY,
         NULL,
         TileBodyCollisionPreSolve,
         NULL,
         NULL,
         NULL);
      */
      cpSpaceAddCollisionHandler(
         m_ptSpace,
         SHAPE_TILE_INTERACTION_AREA,
         SHAPE_TILE_INTERACTION_AREA,
         TileInteractionBegin,
         TileInteractionPreSolve,
         NULL,
         NULL,
         NULL);
      cpSpaceAddCollisionHandler(
         m_ptSpace,
         SHAPE_MAGNETIC_GRIPPER,
         SHAPE_GRIPPABLE,
         MagneticGripperGrippableCollisionBegin,
         MagneticGripperGrippableCollisionPreSolve,
         NULL,
         NULL,
         NULL);
      cpSpaceAddCollisionHandler(
      		m_ptSpace,
      		SHAPE_GRIPPABLE,
      		SHAPE_NORMAL,
      		NULL,
      		NULL,
      		GrippableNormalCollisionPostSolve,
      		NULL,
      		NULL);
      cpSpaceAddCollisionHandler(
          m_ptSpace,
          SHAPE_GRIPPABLE,
          SHAPE_GRIPPABLE,
          NULL,
          NULL,
          GrippableGrippableCollisionPostSolve,
          NULL,
          NULL);
      /* Add boundaries, if specified */
      if(! m_vecSegments.empty()) {
         cpShape* ptSegment;
         for(size_t i = 0; i < m_vecSegments.size(); ++i) {
            if(m_vecSegments[i].Type == SBoundarySegment::SEGMENT_TYPE_WALL) {
               ptSegment =
                  cpSpaceAddShape(
                     m_ptSpace,
                     cpSegmentShapeNew(
                        m_ptGroundBody,
                        cpv(m_vecSegments[i].Segment.GetStart().GetX(),
                            m_vecSegments[i].Segment.GetStart().GetY()),
                        cpv(m_vecSegments[i].Segment.GetEnd().GetX(),
                            m_vecSegments[i].Segment.GetEnd().GetY()),
                        0.0f));
               ptSegment->e = 0.0f; // no elasticity
               ptSegment->u = 1.0f; // max friction
            }
            else {
               /* There is at least a gate, transfer is activated */
               m_bEntityTransferActive = true;
            }
         }
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::Reset() {
      for(TDynamics2DEntityMap::iterator it = m_tPhysicsEntities.begin();
          it != m_tPhysicsEntities.end(); ++it) {
         it->second->Reset();
      }
      cpSpaceReindexStatic(m_ptSpace);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::Update() {
      /* Update the physics state from the entities */
      for(TDynamics2DEntityMap::iterator it = m_tPhysicsEntities.begin();
          it != m_tPhysicsEntities.end(); ++it) {
         it->second->UpdateFromEntityStatus();
      }
      /* Perform the step */
      cpSpaceStep(m_ptSpace, m_fSimulationClockTick);
      /* Update the simulated space */
      for(TDynamics2DEntityMap::iterator it = m_tPhysicsEntities.begin();
          it != m_tPhysicsEntities.end(); ++it) {
         it->second->UpdateEntityStatus();
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::Destroy() {
      /* Empty the physics entity map */
      for(TDynamics2DEntityMap::iterator it = m_tPhysicsEntities.begin();
          it != m_tPhysicsEntities.end(); ++it) {
         delete it->second;
      }
      m_tPhysicsEntities.clear();
      /* Get rid of the physics space */
      cpSpaceFree(m_ptSpace);
      cpBodyFree(m_ptGroundBody);
   }

   /****************************************/
   /****************************************/

   UInt32 CDynamics2DEngine::GetNumPhysicsEngineEntities() {
      return m_tPhysicsEntities.size();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::AddEntity(CEntity& c_entity) {
      c_entity.Accept(m_cAddVisitor);
      cpResetShapeIdCounter();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::RemoveEntity(CEntity& c_entity) {
      c_entity.Accept(m_cRemoveVisitor);
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::TransferEntities() {
      for(size_t i = 0; i < m_vecTransferData.size(); ++i) {
         CPhysicsEngine& cToEngine = CSimulator::GetInstance().GetPhysicsEngine(m_vecTransferData[i].EngineId);
         cToEngine.AddEntity(*m_vecTransferData[i].Entity);
         RemoveEntity(*m_vecTransferData[i].Entity);
      }
      m_vecTransferData.clear();
   }

   /****************************************/
   /****************************************/

   bool CDynamics2DEngine::CalculateTransfer(Real f_x, Real f_y,
                                             std::string& str_engine_id) {
      /*
       * TODO: this method makes the assumption that only one gate is trespassed at any time.
       * This assumption may be false in some ill-shaped polygons or when the gate isn't just a
       * segment, but is a sequence of segments.
       */
      for(size_t i = 0; i < m_vecSegments.size(); ++i) {
         if(m_vecSegments[i].Type == SBoundarySegment::SEGMENT_TYPE_GATE) {
            const CVector2& cP0 = m_vecSegments[i].Segment.GetStart();
            const CVector2& cP1 = m_vecSegments[i].Segment.GetEnd();
            Real fCriterion =
               (f_y - cP0.GetY()) * (cP1.GetX() - cP0.GetX()) -
               (f_x - cP0.GetX()) * (cP1.GetY() - cP0.GetY());
            if(fCriterion < 0.0f) {
               str_engine_id = m_vecSegments[i].EngineId;
               return true;
            }
         }
      }
      return false;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::AddPhysicsEntity(const std::string& str_id,
                                            CDynamics2DEntity& c_entity) {
      m_tPhysicsEntities[str_id] = &c_entity;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::AddControllableEntity(CControllableEntity& c_entity) {
      m_tControllableEntities[c_entity.GetId()] = &c_entity;
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::RemovePhysicsEntity(const std::string& str_id) {
      TDynamics2DEntityMap::iterator it = m_tPhysicsEntities.find(str_id);
      if(it != m_tPhysicsEntities.end()) {
         delete it->second;
         m_tPhysicsEntities.erase(it);
      }
      else {
         THROW_ARGOSEXCEPTION("Dynamics2D entity id \"" << str_id << "\" not found in dynamics 2D engine \"" << GetId() << "\"");
      }
   }

   /****************************************/
   /****************************************/

   void CDynamics2DEngine::RemoveControllableEntity(const std::string& str_id) {
      TControllableEntityMap::iterator it = m_tControllableEntities.find(str_id);
      if(it != m_tControllableEntities.end()) {
         m_tControllableEntities.erase(it);
      }
      else {
         THROW_ARGOSEXCEPTION("Controllable entity id \"" << str_id << "\" not found in dynamics 2D engine \"" << GetId() << "\"");
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_PHYSICS_ENGINE(CDynamics2DEngine,
                           "dynamics2d",
                           "A 2D dynamics physics engine",
                           "Carlo Pinciroli [cpinciro@ulb.ac.be]",
                           "This physics engine is a 2D dynamics engine based on the Chipmunk library\n"
                           "(http://code.google.com/p/chipmunk-physics).\n\n"
                           "REQUIRED XML CONFIGURATION\n\n"
                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <dynamics2d id=\"dyn2d\" />\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"
                           "The 'id' attribute is necessary and must be unique among the physics engines.\n"
                           "It is used in the subsequent section <arena_physics> to assign entities to\n"
                           "physics engines. If two engines share the same id, initialization aborts.\n\n"
                           "OPTIONAL XML CONFIGURATION\n\n"
                           "The plane of the physics engine can be translated on the Z axis, to simulate\n"
                           "for example hovering objects, such as flying robots. To translate the plane\n"
                           "2m up the Z axis, use the 'elevation' attribute as follows:\n\n"
                           "  <physics_engines>\n"
                           "    ...\n"
                           "    <dynamics2d id=\"dyn2d\"\n"
                           "                elevation=\"2.0\" />\n"
                           "    ...\n"
                           "  </physics_engines>\n\n"
                           "When not specified, the elevation is zero, which means that the plane\n"
                           "corresponds to the XY plane.\n",
                           "Under development"
      );

}
