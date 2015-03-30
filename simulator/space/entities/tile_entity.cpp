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
 * @file <simulator/space/entities/tile_entity.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 * @author Dhananjay Ipparthi - <ipparthi@gmail.com>
 */

#include "tile_entity.h"
#include <argos2/common/utility/math/general.h>
#include <argos2/common/utility/string_utilities.h>

namespace argos {

   /****************************************/
   /****************************************/

   CTileEntity::CTileEntity() :
      CEmbodiedEntity(NULL),
      m_fMass(1.0f) {
   }

   /****************************************/
   /****************************************/

   CTileEntity::~CTileEntity() {
   }

   /****************************************/
   /****************************************/

   void CTileEntity::Init(TConfigurationNode& t_tree) {
      try {
         /* Init parent */
         CEmbodiedEntity::Init(t_tree);
         /* Parse XML to get the side length */
         Real fSideLength;
         GetNodeAttribute(t_tree, "side_length", fSideLength);
         m_cSize.Set(fSideLength, fSideLength, 0.01f);
         /* Parse XML to get the mass */
         GetNodeAttribute(t_tree, "mass", m_fMass);
         /* Parse XML to get the interaction range */
         GetNodeAttribute(t_tree, "interaction_range", m_fInteractionRange);
         /* Parse XML to get the lock configuration */
         std::string strLocks;
         GetNodeAttribute(t_tree, "locks", strLocks);
         ParseValues<SInt32>(strLocks, 4, m_nLocks, ',');
         CalculateBoundingBox();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CTileEntity::CalculateBoundingBox() {
      m_cOrientationMatrix.SetFromQuaternion(GetOrientation());
      CalculateBoundingBoxFromHalfSize(GetBoundingBox(),
                                       m_cSize * 0.5f,
                                       GetPosition(),
                                       m_cOrientationMatrix);
   }

   /****************************************/
   /****************************************/

   REGISTER_ENTITY(CTileEntity,
                   "tile",
                   "A tile that connect to other tiles.",
                   "Carlo Pinciroli [cpinciro@ulb.ac.be]",
                   "TODO",
                   "Under development"
      );

}
