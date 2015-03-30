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
 * @file <simulator/visitors/entity_visitor.h>
 *
 * @brief This file contains the definition of the generic entity visitor
 *
 * @author Manuele Brambilla - <mbrambilla@iridia.ulb.ac.be>
 */

#ifndef ENTITYVISITOR_H
#define ENTITYVISITOR_H

namespace argos {
   class CEntity;
   class CBoothEntity;
   class CBoxEntity;
   class CCylinderEntity;
   class CTileEntity;
   class CBluebotEntity;
   class CFloorEntity;
   class CFootBotEntity;
   class CEyeBotEntity;
   class CLightEntity;
   class CEPuckEntity;
   class CWiFiMediumEntity;
   class CKumarMicroQuadrotorEntity;
}

namespace argos {

    class CEntityVisitor {

        public:

       virtual ~CEntityVisitor() {} 
       virtual void Visit(CEntity& e) {}
       virtual void Visit(CBoothEntity& e) = 0;
       virtual void Visit(CBoxEntity& e) = 0;
       virtual void Visit(CCylinderEntity& e) = 0;
       virtual void Visit(CTileEntity& e) = 0;
       virtual void Visit(CBluebotEntity& e) = 0;
       virtual void Visit(CFloorEntity& e) = 0;
       virtual void Visit(CFootBotEntity& e) = 0;
       virtual void Visit(CEyeBotEntity& e) = 0;
       virtual void Visit(CLightEntity& e) = 0;
       virtual void Visit(CEPuckEntity& e) = 0;
       virtual void Visit(CWiFiMediumEntity& e) = 0;
       virtual void Visit(CKumarMicroQuadrotorEntity& e) = 0;
       
    };
}

#endif

