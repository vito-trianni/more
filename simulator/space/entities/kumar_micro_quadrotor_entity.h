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
 * @file <simulator/space/entities/kumar_micro_quadrotor_entity_entity.h>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef KUMARMICROQUADROTOR_ENTITY_H
#define KUMARMICROQUADROTOR_ENTITY_H

namespace argos {
   class CKumarMicroQuadrotorEntity;
   class CEmbodiedEntity;
   class CControllableEntity;
   class CQuaternion;
}

#include <argos2/simulator/space/entities/composable_entity.h>
#include <argos2/simulator/space/entities/rab_equipped_entity.h>

namespace argos {

   class CKumarMicroQuadrotorEntity : public CComposableEntity {

   public:

      CKumarMicroQuadrotorEntity();
      virtual ~CKumarMicroQuadrotorEntity();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      virtual CEntity& GetComponent(const std::string& str_component);
      virtual bool HasComponent(const std::string& str_component);

      virtual void UpdateComponents();

      inline CEmbodiedEntity& GetEmbodiedEntity() {
         return *m_pcEmbodiedEntity;
      }

      inline CControllableEntity& GetControllableEntity() {
         return *m_pcControllableEntity;
      }

      inline virtual std::string GetTypeDescription() const {
         return "kumar_micro_quadrotor_entity";
      }

      inline virtual void Accept(CEntityVisitor& visitor) {
          visitor.Visit(*this);
      }

   private:

      void SetLedPosition();

   private:

      CEmbodiedEntity*     m_pcEmbodiedEntity;
      CControllableEntity* m_pcControllableEntity;

   };

}

#endif
