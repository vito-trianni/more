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
 * @file <argos2/simulator/space/entities/led_entity.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef LED_ENTITY_H
#define LED_ENTITY_H

namespace argos {
   class CLedEntity;
}

#include <argos2/simulator/space/entities/entity.h>
#include <argos2/common/utility/datatypes/color.h>
#include <argos2/common/utility/math/vector3.h>
#include <argos2/simulator/space/space_hash.h>

namespace argos {

   class CLedEntity : public CEntity {

   public:

      CLedEntity(CEntity* pc_parent,
                 const CVector3& c_position,
                 const CColor& c_color) :
         CEntity(pc_parent),
         m_cPosition(c_position),
         m_cColor(c_color) {}
      virtual ~CLedEntity() {}

      virtual void Reset();

      const CVector3& GetPosition() const {
         return m_cPosition;
      }

      inline void SetPosition(const CVector3& c_position) {
         m_cPosition = c_position;
      }

      inline const CColor& GetColor() const {
         return m_cColor;
      }

      inline void SetColor(const CColor& c_color) {
         m_cColor = c_color;
      }

      inline virtual void Accept(CEntityVisitor& visitor) {
         visitor.Visit(*this);
      }

      inline virtual std::string GetTypeDescription() const {
         return "led_entity";
      }

   protected:

      CVector3 m_cPosition;
      CColor m_cColor;

   };

   typedef std::vector<CLedEntity*> TLedEntityList;
   typedef std::tr1::unordered_set<CLedEntity*> TLedEntitySet;

   /****************************************/
   /****************************************/

   class CLEDEntitySpaceHashUpdater : public CSpaceHashUpdater<CLedEntity> {

   public:

      virtual void operator()(CAbstractSpaceHash<CLedEntity>& c_space_hash,
                              CLedEntity& c_element);

   private:

      SInt32 m_nI, m_nJ, m_nK;

   };

   /****************************************/
   /****************************************/

}

#endif
