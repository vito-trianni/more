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
 * @file <simulator/space/entities/kumar_micro_quadrotor_entity.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "kumar_micro_quadrotor_entity.h"
#include "embodied_entity.h"
#include "controllable_entity.h"
#include <argos2/common/utility/math/rotationmatrix3.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real KUMAR_MICRO_QUADROTOR_HEIGHT                   = 0.566f;
   static const Real KUMAR_MICRO_QUADROTOR_RADIUS                   = 0.25f;
   static const Real KUMAR_MICRO_QUADROTOR_LED_UPPER_RING_RADIUS    = KUMAR_MICRO_QUADROTOR_RADIUS + 0.005;
   static const Real KUMAR_MICRO_QUADROTOR_LED_UPPER_RING_ELEVATION = 0.1635f;
   static const Real KUMAR_MICRO_QUADROTOR_LED_LOWER_RING_RADIUS    = KUMAR_MICRO_QUADROTOR_RADIUS + 0.005;
   static const Real KUMAR_MICRO_QUADROTOR_LED_LOWER_RING_ELEVATION = 0.1535f;

   /* We can't use CRadians::PI and the likes here because of the 'static initialization order fiasco' */
   static const CRadians KUMAR_MICRO_QUADROTOR_LED_ANGLE_SLICE      = CRadians(3.14159265358979323846264338327950288 / 8.0);
   static const CRadians KUMAR_MICRO_QUADROTOR_HALF_LED_ANGLE_SLICE = KUMAR_MICRO_QUADROTOR_LED_ANGLE_SLICE * 0.5f;

   /****************************************/
   /****************************************/

   class CKumarMicroQuadrotorEmbodiedEntity : public CEmbodiedEntity {

   public:

      CKumarMicroQuadrotorEmbodiedEntity(CKumarMicroQuadrotorEntity* pc_parent) :
         CEmbodiedEntity(pc_parent) {
         m_cHalfSize.SetX(KUMAR_MICRO_QUADROTOR_RADIUS);
         m_cHalfSize.SetY(KUMAR_MICRO_QUADROTOR_RADIUS);
         m_cHalfSize.SetZ(KUMAR_MICRO_QUADROTOR_HEIGHT * 0.5f);
      }

   protected:

      virtual void CalculateBoundingBox() {
         m_cCenterPos = GetPosition();
         m_cCenterPos.SetZ(m_cCenterPos.GetZ() + m_cHalfSize.GetZ());
         m_cOrientationMatrix.SetFromQuaternion(GetOrientation());
         CalculateBoundingBoxFromHalfSize(GetBoundingBox(),
                                          m_cHalfSize,
                                          m_cCenterPos,
                                          m_cOrientationMatrix);
      }

   private:

      CVector3 m_cHalfSize;
      CVector3 m_cCenterPos;
      CRotationMatrix3 m_cOrientationMatrix;

   };

   /****************************************/
   /****************************************/

   CKumarMicroQuadrotorEntity::CKumarMicroQuadrotorEntity() :
      CComposableEntity(NULL),
      m_pcEmbodiedEntity(new CKumarMicroQuadrotorEmbodiedEntity(this)),
      m_pcControllableEntity(new CControllableEntity(this)) {
   }

   /****************************************/
   /****************************************/

   CKumarMicroQuadrotorEntity::~CKumarMicroQuadrotorEntity() {
      delete m_pcEmbodiedEntity;
      delete m_pcControllableEntity;
   }

   /****************************************/
   /****************************************/

   void CKumarMicroQuadrotorEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CEntity::Init(t_tree);
         /* Init components */
         m_pcEmbodiedEntity->Init(t_tree);
         m_pcControllableEntity->Init(t_tree);
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CKumarMicroQuadrotorEntity::Reset() {
      /* Reset all components */
      m_pcEmbodiedEntity->Reset();
      m_pcControllableEntity->Reset();
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CKumarMicroQuadrotorEntity::Destroy() {
      m_pcEmbodiedEntity->Destroy();
      m_pcControllableEntity->Destroy();
   }

   /****************************************/
   /****************************************/

   CEntity& CKumarMicroQuadrotorEntity::GetComponent(const std::string& str_component) {
      if(str_component == "embodied_entity") {
         return *m_pcEmbodiedEntity;
      }
      else if(str_component == "controllable_entity") {
         return *m_pcControllableEntity;
      }
      else {
         THROW_ARGOSEXCEPTION("A Kumar micro-quadrotor does not have a component of type \"" << str_component << "\"");
      }
   }

   /****************************************/
   /****************************************/

   bool CKumarMicroQuadrotorEntity::HasComponent(const std::string& str_component) {
      return (str_component == "embodied_entity"       ||
              str_component == "controllable_entity"   ||
              str_component == "led_equipped_entity");
   }

   /****************************************/
   /****************************************/

   void CKumarMicroQuadrotorEntity::UpdateComponents() {
      m_pcEmbodiedEntity->UpdateBoundingBox();
   }

   /****************************************/
   /****************************************/
   
   REGISTER_ENTITY(CKumarMicroQuadrotorEntity,
                   "kumar_micro_quadrotor",
                   "The micro quadrotor robot, developed by prof. Kumar's group in UPenn.",
                   "Carlo Pinciroli [cpinciro@ulb.ac.be]",
                   "The micro quadrotor is a flying robot developed by prof. Kumar's group in\n"
                   "UPenn.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <kumar_micro_quadrotor id=\"kmq0\"\n"
                   "                           position=\"0.4,2.3,0.25\"\n"
                   "                           orientation=\"45,90,0\"\n"
                   "                           controller=\"mycntrl\" />\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'position' attribute specifies the position of the center of mass of the\n"
                   "micro quadrotor in the arena. The attribute values are in the X,Y,Z order.\n"
                   "The 'orientation' attribute specifies the orientation of the micro quadrotor.\n"
                   "All rotations are performed with respect to the center of mass. The order of the\n"
                   "angles is Z,Y,X, which means that the first number corresponds to the rotation\n"
                   "around the Z axis, the second around Y and the last around X. This reflects\n"
                   "the internal convention used in ARGoS, in which rotations are performed in\n"
                   "that order. Angles are expressed in degrees. When the robot is unrotated, it\n"
                   "is oriented along the X axis.\n"
                   "The 'controller' attribute is used to assign a controller to the micro\n"
                   "quadrotor. The value of the attribute must be set to the id of a previously\n"
                   "defined controller. Controllers are defined in the <controllers> XML subtree.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "None for the time being.\n",
                   "Under development"
      );

}
