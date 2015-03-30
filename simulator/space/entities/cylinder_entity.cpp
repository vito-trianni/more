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
 * @file <simulator/space/entities/cylinder_entity.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "cylinder_entity.h"
#include <argos2/common/utility/math/rotationmatrix3.h>

namespace argos {

   /****************************************/
   /****************************************/

   class CCylinderEmbodiedEntity : public CEmbodiedEntity {

   public:

    	CCylinderEmbodiedEntity(CCylinderEntity* pc_parent,
                              Real f_cylinder_radius,
                              Real f_cylinder_height) :
         CEmbodiedEntity(pc_parent) {
         m_cHalfSize.SetX(f_cylinder_radius);
         m_cHalfSize.SetY(f_cylinder_radius);
         m_cHalfSize.SetZ(f_cylinder_height * 0.5f);
      }

   protected:

      virtual void CalculateBoundingBox() {
         m_cOrientationMatrix.SetFromQuaternion(GetOrientation());
         CalculateBoundingBoxFromHalfSize(GetBoundingBox(),
                                          m_cHalfSize,
                                          GetPosition(),
                                          m_cOrientationMatrix);
      }

   private:

      CVector3 m_cHalfSize;
      CRotationMatrix3 m_cOrientationMatrix;

   };

   /****************************************/
   /****************************************/

   CCylinderEntity::CCylinderEntity() :
      CComposableEntity(NULL),
      m_pcLEDEquippedEntity(new CLedEquippedEntity(this)),
      m_fMass(1.0f),
      m_bVisible(true) {
   }

   /****************************************/
   /****************************************/

   CCylinderEntity::~CCylinderEntity() {
    	delete m_pcEmbodiedEntity;
    	delete m_pcLEDEquippedEntity;
   }

   /****************************************/
   /****************************************/

   void CCylinderEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CEntity::Init(t_tree);
         /* Parse XML to get the radius */
         GetNodeAttribute(t_tree, "radius", m_fRadius);
         /* Parse XML to get the height */
         GetNodeAttribute(t_tree, "height", m_fHeight);
         /* Parse XML to get the movable attribute */
         bool bMovable;
         GetNodeAttribute(t_tree, "movable", bMovable);
         if(bMovable) {
            /* Parse XML to get the mass */
            GetNodeAttribute(t_tree, "mass", m_fMass);
         }
         else {
            m_fMass = 0.0f;
         }
         /* Parse XML to get the visible attribute */
         GetNodeAttributeOrDefault(t_tree, "visible", m_bVisible, m_bVisible);
         /* Init LED equipped entity component */
         m_pcLEDEquippedEntity->Init(t_tree);
         if(NodeExists(t_tree, "leds")) {
            TConfigurationNode& tLEDs = GetNode(t_tree, "leds");
            /* Go through the led entries */
            CVector3 cPosition;
            CColor cColor;
            TConfigurationNodeIterator itLED("led");
            for(itLED = itLED.begin(&tLEDs);
                itLED != itLED.end();
                ++itLED) {
               GetNodeAttribute(*itLED, "position", cPosition);
               GetNodeAttribute(*itLED, "color", cColor);
               m_vecBaseLEDPositions.push_back(cPosition);
               m_pcLEDEquippedEntity->AddLed(cPosition, cColor);
            }
         }
         /* Create embodied entity using parsed data */
         m_pcEmbodiedEntity = new CCylinderEmbodiedEntity(this, m_fRadius, m_fHeight);
         m_pcEmbodiedEntity->Init(t_tree);
         m_pcEmbodiedEntity->SetMovable(bMovable);

         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize the cylinder entity.", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CCylinderEntity::Reset() {
      /* Reset all components */
      m_pcEmbodiedEntity->Reset();
      m_pcLEDEquippedEntity->Reset();
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CCylinderEntity::Destroy() {
      m_pcEmbodiedEntity->Destroy();
      m_pcLEDEquippedEntity->Destroy();
   }

   /****************************************/
   /****************************************/

   CEntity& CCylinderEntity::GetComponent(const std::string& str_component) {
      if(str_component == "embodied_entity") {
         return *m_pcEmbodiedEntity;
      }
      else if(str_component == "led_equipped_entity") {
         return *m_pcLEDEquippedEntity;
      }
      else {
         THROW_ARGOSEXCEPTION("A cylinder does not have a component of type \"" << str_component << "\"");
      }
   }

   /****************************************/
   /****************************************/

   bool CCylinderEntity::HasComponent(const std::string& str_component) {
      return (str_component == "embodied_entity"     ||
              str_component == "led_equipped_entity" );
   }

   /****************************************/
   /****************************************/

   void CCylinderEntity::UpdateComponents() {
      /* Set LED position and update led equipped entity */
      CVector3 cLEDPosition;
      for(UInt32 i = 0; i < m_pcLEDEquippedEntity->GetAllLeds().size(); ++i) {
         cLEDPosition = m_vecBaseLEDPositions[i];
         cLEDPosition.Rotate(m_pcEmbodiedEntity->GetOrientation());
         cLEDPosition += GetEmbodiedEntity().GetPosition();
         m_pcLEDEquippedEntity->SetLedPosition(i, cLEDPosition);
      }
      m_pcEmbodiedEntity->UpdateBoundingBox();
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CCylinderEntity,
                   "cylinder",
                   "A stretchable cylinder.",
                   "Carlo Pinciroli [cpinciro@ulb.ac.be]",
                   "The cylinder entity can be used to model obstacles or cylinder-shaped\n"
                   "grippable objects. The cylinder has a red LED on the center of one\n"
                   "of the circular surfaces, that allows perception using the cameras.\n"
                   "The cylinder can be movable or not. A movable object can be pushed\n"
                   "and gripped. An unmovable object is pretty much like a column.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "To declare an unmovable object (i.e., a column) you need the following:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <cylinder id=\"box1\"\n"
                   "              position=\"0.4,2.3,0.25\"\n"
                   "              orientation=\"45,90,0\"\n"
                   "              radius=\"0.8\"\n"
                   "              height=\"0.5\"\n"
                   "              movable=\"false\" />\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "To declare a movable object you need the following:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <cylinder id=\"box1\"\n"
                   "              position=\"0.4,2.3,0.25\"\n"
                   "              orientation=\"45,90,0\"\n"
                   "              radius=\"0.8\"\n"
                   "              height=\"0.5\"\n"
                   "              movable=\"true\"\n"
                   "              mass=\"2.5\" />\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'position' attribute specifies the position of the center of mass of the\n"
                   "cylinder in the arena. The three values are in the X,Y,Z order.\n"
                   "The 'orientation' attribute specifies the orientation of the cylinder. All\n"
                   "rotations are performed with respect to the center of mass. The order of the\n"
                   "angles is Z,Y,X, which means that the first number corresponds to the rotation\n"
                   "around the Z axis, the second around Y and the last around X. This reflects\n"
                   "the internal convention used in ARGoS, in which rotations are performed in\n"
                   "that order. Angles are expressed in degrees.\n"
                   "The 'radius' and 'height' attributes specify the size of the cylinder. When\n"
                   "you add a cylinder, imagine it initially unrotated and centered in the origin.\n"
                   "The base of the cylinder, then, is parallel to the XY plane and its height\n"
                   "goes with the Z axis.\n"
                   "The 'movable' attribute specifies whether or not the object is movable. When\n"
                   "set to 'false', the object is unmovable: if another object pushes against it,\n"
                   "the cylinder won't move. When the attribute is set to 'true', the cylinder is\n"
                   "movable upon pushing or gripping. When an object is movable, the 'mass'\n"
                   "attribute is required.\n"
                   "The 'mass' attribute quantifies the mass of the cylinder in kg.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "It is possible to add any number of colored LEDs to the cylinder. In this way,\n"
                   "the cylinder is visible with a robot camera. The position and color of the\n"
                   "LEDs is specified with the following syntax:\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <cylinder id=\"box1\"\n"
                   "              position=\"0.4,2.3,0.25\"\n"
                   "              orientation=\"45,90,0\"\n"
                   "              radius=\"0.8\"\n"
                   "              height=\"0.5\"\n"
                   "              movable=\"true\"\n"
                   "              mass=\"2.5\">\n"
                   "      <leds>\n"
                   "        <led position=\"0.81,0,0\" color=\"white\" />\n"
                   "        <led position=\"-0.81,0,0\" color=\"red\" />\n"
                   "        <led position=\"0,0.81,0\" color=\"blue\" />\n"
                   "        <led position=\"0,-0.81,0\" color=\"green\" />\n"
                   "      </leds>\n"
                   "    </cylinder>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "In the example, four LEDs are added around the cylinder. The LEDs have\n"
                   "different colors and are located around the cylinder.\n",
                   "Usable"
      );

}
