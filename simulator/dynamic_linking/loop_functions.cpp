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
 * @file <simulator/dynamic_linking/loop_functions.cpp>
 *
 * @brief This file provides the definition of
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "loop_functions.h"
#include <argos2/simulator/factories/entities_factory.h>

#include <argos2/simulator/space/entities/booth_entity.h>
#include <argos2/simulator/space/entities/box_entity.h>
#include <argos2/simulator/space/entities/cylinder_entity.h>
#include <argos2/simulator/space/entities/light_entity.h>
#include <argos2/simulator/space/entities/footbot_entity.h>
#include <argos2/simulator/space/entities/eyebot_entity.h>
#include <argos2/simulator/space/entities/kumar_micro_quadrotor_entity.h>
#include <argos2/simulator/space/entities/epuck_entity.h>

namespace argos {

   /****************************************/
   /****************************************/
   
   template <class T> T& AddEntityToSpace(TConfigurationNode& t_tree) {
      /* Create an entity of the wanted type */
      T* pcEntity = dynamic_cast<T*>(CEntitiesFactory::NewEntity(t_tree.Value()));
      if(pcEntity == NULL) {
         THROW_ARGOSEXCEPTION("Loop functions: error to creating entity type \"" << t_tree.Value() << "\"");
      }
      /* Initialize the entity */
      pcEntity->Init(t_tree);
      /* Add the entity to the space */
      CSimulator::GetInstance().GetSpace().AddEntity(*pcEntity);
      /* Return the entity */
      return *pcEntity;
   }

   /****************************************/
   /****************************************/

   void AddEntityToPhysics(CEntity& c_entity,
                           const std::vector<std::string>& str_engines) {
      /* Add the entity to the physics engine(s) */
      for(size_t i = 0; i < str_engines.size(); ++i) {
         CSimulator::GetInstance().GetPhysicsEngine(str_engines[i]).AddEntity(c_entity);
      }
   }

   /****************************************/
   /****************************************/

   template <class T> void AssignController(T& c_entity) {
      /* Get a reference to the controllable entity */
      CControllableEntity& cControllableEntity = c_entity.GetControllableEntity();
      /* Look for the controller with the right id in the XML */
      TConfigurationNode tControllersTree;
      tControllersTree = GetNode(CSimulator::GetInstance().GetConfigurationRoot(), "controllers");
      bool found = false;
      TConfigurationNodeIterator itControllers;
      std::string strControllerId;
      itControllers = itControllers.begin(&tControllersTree);
      while(!found && itControllers != itControllers.end()) {
         GetNodeAttribute(*itControllers, "id", strControllerId);
         if(strControllerId == cControllableEntity.GetControllerId()) {
            found = true;
         }
         else {
            ++itControllers;
         }
      }
      /* Did we find the controller? */
      ARGOS_ASSERT(found,
                   "[FATAL] The entity \"" <<
                   c_entity.GetId() << "\" has been associated with a controller with id \"" <<
                   cControllableEntity.GetControllerId() <<
                   "\", but a controller with this id wasn't found in the <controllers> section of the XML file.");
      /* Now itControllers points to the right controller subtree */
      /* Get the parameters subtree */
      TConfigurationNode tControllerParameters;
      tControllerParameters = GetNode(*itControllers, "parameters");
      /* Create the controller */
      CCI_Controller* pcController =
         CSimulator::GetInstance().
         GetDynamicLinkingManager().
         NewController(c_entity,
                       cControllableEntity.GetControllerId(),
                       tControllerParameters);      
      /* Set the controller to the entity */
      cControllableEntity.SetController(*pcController);
   }

   /****************************************/
   /****************************************/

   CBoothEntity& CLoopFunctions::AddBooth(const std::string& str_id,
		   const CVector3& c_position,
		   const CQuaternion& c_orientation,
		   const std::string& str_controller_id,
		   const std::string& str_physics_id) {

	   /* Get the angles in degrees from the quaternion */
	   CRadians cX, cY, cZ;
	   c_orientation.ToEulerAngles(cZ, cY, cX);
	   CVector3 cOrientationAngles(ToDegrees(cZ).GetValue(),
			   ToDegrees(cY).GetValue(),
			   ToDegrees(cX).GetValue());

	   /* Build the XML tree */
	   TConfigurationNode tRootNode("booth");
	   SetNodeAttribute(tRootNode, "id", str_id);
	   SetNodeAttribute(tRootNode, "position", c_position);
	   SetNodeAttribute(tRootNode, "orientation", cOrientationAngles);
	   SetNodeAttribute(tRootNode, "controller", str_controller_id);
	   /* Add the entity to the space */
	   CBoothEntity& cEntity = AddEntityToSpace<CBoothEntity>(tRootNode);
	   /* Add the entity to physics */
	   AddEntityToPhysics(cEntity, std::vector<std::string>(1, str_physics_id));
	   /* Assign the controller */
	   AssignController(cEntity);
	   /* Return the entity */
	   return cEntity;
   }

   /****************************************/
   /****************************************/

   CBoxEntity& CLoopFunctions::AddBox(const std::string& str_id,
                                      const CVector3& c_position,
                                      const CQuaternion& c_orientation,
                                      const CVector3& c_size,
                                      bool b_movable,
                                      Real f_mass,
                                      const std::vector<std::string>& str_physics_ids,
                                      const std::vector<SAdditionalLED>& vec_additional_leds,
                                      bool b_visible) {

      /* Get the angles in degrees from the quaternion */
      CRadians cX, cY, cZ;
      c_orientation.ToEulerAngles(cZ, cY, cX);
      CVector3 cOrientationAngles(ToDegrees(cZ).GetValue(),
                                  ToDegrees(cY).GetValue(),
                                  ToDegrees(cX).GetValue());

      /* Build the XML tree */
      TConfigurationNode tRootNode("box");
      SetNodeAttribute(tRootNode, "id", str_id);
      SetNodeAttribute(tRootNode, "position", c_position);
      SetNodeAttribute(tRootNode, "orientation", cOrientationAngles);
      SetNodeAttribute(tRootNode, "size", c_size);
      SetNodeAttribute(tRootNode, "movable", b_movable);
      SetNodeAttribute(tRootNode, "mass", f_mass);
      SetNodeAttribute(tRootNode, "visible", b_visible);
      /* Add LEDs, if any */
      if(!vec_additional_leds.empty()) {
         TConfigurationNode tLEDNode("leds");
         for(UInt32 i = 0; i < vec_additional_leds.size(); ++i) {
            TConfigurationNode tLED("led");
            SetNodeAttribute(tLED, "position", vec_additional_leds[i].Position);
            SetNodeAttribute(tLED, "color", vec_additional_leds[i].Color);
            AddChildNode(tLEDNode, tLED);
         }
         AddChildNode(tRootNode, tLEDNode);
      }
      /* Add the entity to the space */
      CBoxEntity& cEntity = AddEntityToSpace<CBoxEntity>(tRootNode);
      /* Add the entity to physics */
      AddEntityToPhysics(cEntity, str_physics_ids);
      /* Return the entity */
      return cEntity;
   }

   /****************************************/
   /****************************************/

   CCylinderEntity& CLoopFunctions::AddCylinder(const std::string& str_id,
                                                const CVector3& c_position,
                                                const CQuaternion& c_orientation,
                                                Real f_radius,
                                                Real f_height,
                                                bool b_movable,
                                                Real f_mass,
                                                const std::vector<std::string>& str_physics_ids,
                                                const std::vector<SAdditionalLED>& vec_additional_leds,
                                                bool b_visible) {

      /* Get the angles in degrees from the quaternion */
      CRadians cX, cY, cZ;
      c_orientation.ToEulerAngles(cZ, cY, cX);
      CVector3 cOrientationAngles(ToDegrees(cZ).GetValue(),
                                  ToDegrees(cY).GetValue(),
                                  ToDegrees(cX).GetValue());

      /* Build the XML tree */
      TConfigurationNode tRootNode("cylinder");
      SetNodeAttribute(tRootNode, "id", str_id);
      SetNodeAttribute(tRootNode, "position", c_position);
      SetNodeAttribute(tRootNode, "orientation", cOrientationAngles);
      SetNodeAttribute(tRootNode, "radius", f_radius);
      SetNodeAttribute(tRootNode, "height", f_height);
      SetNodeAttribute(tRootNode, "movable", b_movable);
      SetNodeAttribute(tRootNode, "mass", f_mass);
      SetNodeAttribute(tRootNode, "visible", b_visible);
      /* Add LEDs, if any */
      if(!vec_additional_leds.empty()) {
         TConfigurationNode tLEDNode("leds");
         for(UInt32 i = 0; i < vec_additional_leds.size(); ++i) {
            TConfigurationNode tLED("led");
            SetNodeAttribute(tLED, "position", vec_additional_leds[i].Position);
            SetNodeAttribute(tLED, "color", vec_additional_leds[i].Color);
            AddChildNode(tLEDNode, tLED);
         }
         AddChildNode(tRootNode, tLEDNode);
      }
      /* Add the entity to the space */
      CCylinderEntity& cEntity = AddEntityToSpace<CCylinderEntity>(tRootNode);
      /* Add the entity to physics */
      AddEntityToPhysics(cEntity, str_physics_ids);
      /* Return the entity */
      return cEntity;
   }

   /****************************************/
   /****************************************/

   CLightEntity& CLoopFunctions::AddLight(const std::string& str_id,
                                          const CVector3& c_position,
                                          Real f_intensity,
                                          const CColor& c_color) {

      /* Build the XML tree */
      TConfigurationNode tRootNode("light");
      SetNodeAttribute(tRootNode, "id", str_id);
      SetNodeAttribute(tRootNode, "position", c_position);
      SetNodeAttribute(tRootNode, "orientation", CVector3());
      SetNodeAttribute(tRootNode, "intensity", f_intensity);
      SetNodeAttribute(tRootNode, "color", c_color);
      /* Add the entity to the space */
      CLightEntity& cEntity = AddEntityToSpace<CLightEntity>(tRootNode);
      /* Return the entity */
      return cEntity;
   }

   /****************************************/
   /****************************************/

   CFootBotEntity& CLoopFunctions::AddFootBot(const std::string& str_id,
                                              const CVector3& c_position,
                                              const CQuaternion& c_orientation,
                                              const std::string& str_controller_id,
                                              const std::string& str_physics_id) {

      /* Get the angles in degrees from the quaternion */
      CRadians cX, cY, cZ;
      c_orientation.ToEulerAngles(cZ, cY, cX);
      CVector3 cOrientationAngles(ToDegrees(cZ).GetValue(),
                                  ToDegrees(cY).GetValue(),
                                  ToDegrees(cX).GetValue());

      /* Build the XML tree */
      TConfigurationNode tRootNode("foot-bot");
      SetNodeAttribute(tRootNode, "id", str_id);
      SetNodeAttribute(tRootNode, "position", c_position);
      SetNodeAttribute(tRootNode, "orientation", cOrientationAngles);
      SetNodeAttribute(tRootNode, "controller", str_controller_id);
      /* Add the entity to the space */
      CFootBotEntity& cEntity = AddEntityToSpace<CFootBotEntity>(tRootNode);
      /* Add the entity to physics */
      AddEntityToPhysics(cEntity, std::vector<std::string>(1, str_physics_id));
      /* Assign the controller */
      AssignController(cEntity);
      /* Return the entity */
      return cEntity;
   }
      
   /****************************************/
   /****************************************/

   CKumarMicroQuadrotorEntity& CLoopFunctions::AddKumarMicroQuadRotor(const std::string& str_id,
                                                                      const CVector3& c_position,
                                                                      const CQuaternion& c_orientation,
                                                                      const std::string& str_controller_id,
                                                                      const std::string& str_physics_id) {

      /* Get the angles in degrees from the quaternion */
      CRadians cX, cY, cZ;
      c_orientation.ToEulerAngles(cZ, cY, cX);
      CVector3 cOrientationAngles(ToDegrees(cZ).GetValue(),
                                  ToDegrees(cY).GetValue(),
                                  ToDegrees(cX).GetValue());

      /* Build the XML tree */
      TConfigurationNode tRootNode("kumar_micro_quadrotor");
      SetNodeAttribute(tRootNode, "id", str_id);
      SetNodeAttribute(tRootNode, "position", c_position);
      SetNodeAttribute(tRootNode, "orientation", cOrientationAngles);
      SetNodeAttribute(tRootNode, "controller", str_controller_id);
      /* Add the entity to the space */
      CKumarMicroQuadrotorEntity& cEntity = AddEntityToSpace<CKumarMicroQuadrotorEntity>(tRootNode);
      /* Add the entity to physics */
      AddEntityToPhysics(cEntity, std::vector<std::string>(1, str_physics_id));
      /* Assign the controller */
      AssignController(cEntity);
      /* Return the entity */
      return cEntity;
   }
      
   /****************************************/
   /****************************************/

   CEPuckEntity& CLoopFunctions::AddEPuck(const std::string& str_id,
                                          const CVector3& c_position,
                                          const CQuaternion& c_orientation,
                                          const std::string& str_controller_id,
                                          const std::string& str_physics_id) {

      /* Get the angles in degrees from the quaternion */
      CRadians cX, cY, cZ;
      c_orientation.ToEulerAngles(cZ, cY, cX);
      CVector3 cOrientationAngles(ToDegrees(cZ).GetValue(),
                                  ToDegrees(cY).GetValue(),
                                  ToDegrees(cX).GetValue());

      /* Build the XML tree */
      TConfigurationNode tRootNode("e-puck");
      SetNodeAttribute(tRootNode, "id", str_id);
      SetNodeAttribute(tRootNode, "position", c_position);
      SetNodeAttribute(tRootNode, "orientation", cOrientationAngles);
      SetNodeAttribute(tRootNode, "controller", str_controller_id);
      /* Add the entity to the space */
      CEPuckEntity& cEntity = AddEntityToSpace<CEPuckEntity>(tRootNode);
      /* Add the entity to physics */
      AddEntityToPhysics(cEntity, std::vector<std::string>(1, str_physics_id));
      /* Assign the controller */
      AssignController(cEntity);
      /* Return the entity */
      return cEntity;
   }
      
   /****************************************/
   /****************************************/

   void CLoopFunctions::RemoveEntity(CEntity& c_entity) {
      /* Remove the entity from space */
      m_cSpace.RemoveEntity(c_entity);
      /* free memory */
      delete &c_entity;
   }

   /****************************************/
   /****************************************/

   void CLoopFunctions::SetOnlineControlParameters(UInt32 un_num_params,
                                                   const Real* pf_params) {
      
      CSpace::TAnyEntityMap cEntities = m_cSpace.GetEntitiesByType("controllable_entity");
      
      for(CSpace::TAnyEntityMap::iterator it = cEntities.begin(); it != cEntities.end(); ++it) {
            CControllableEntity* pcEntity = any_cast<CControllableEntity*>(it->second);
            pcEntity->GetController().SetOnlineParameters(un_num_params,pf_params);
      }
      
   }

   /****************************************/
   /****************************************/

}
