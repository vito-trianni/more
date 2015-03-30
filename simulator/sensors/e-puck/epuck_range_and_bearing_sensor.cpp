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
 * @file <argos2/simulator/sensors/e-puck/epuck_range_and_bearing_sensor.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 * @author Gianpiero Francesca - <gianpiero.francesca@ulb.ac.be>
 */

#include "epuck_range_and_bearing_sensor.h"
#include <argos2/simulator/simulator.h>
#include <argos2/simulator/space/entities/composable_entity.h>
#include <argos2/common/utility/math/angles.h>

namespace argos {
    static const Real EPUCK_RADIUS = 3.5f;

    /****************************************/

    /****************************************/

    CEPuckRangeAndBearingSensor::CEPuckRangeAndBearingSensor() :
    m_cSpace(CSimulator::GetInstance().GetSpace()),
    m_cEmbodiedSpaceHash(m_cSpace.GetEmbodiedEntitiesSpaceHash()),
    m_cRABEquippedSpaceHash(m_cSpace.GetRABEquippedEntitiesSpaceHash()),
    m_pcEntity(NULL),
    m_pcRABEquippedEntity(NULL),
    m_pcEmbodiedEntity(NULL),
    m_pcRNG(NULL),
    m_bShowRays(false),
    m_bCheckOcclusions(true),
    m_fLossProbability(0.3),
    m_fExpA(9.06422181283387f),
    m_fExpB(-0.00565074879677167f) {
    }

    /****************************************/

    /****************************************/
    void CEPuckRangeAndBearingSensor::Init(TConfigurationNode& t_tree) {
        try {
            CCI_EPuckRangeAndBearingSensor::Init(t_tree);
            /* Show rays? */
            GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
            GetNodeAttributeOrDefault(t_tree, "check_occlusions", m_bCheckOcclusions, m_bCheckOcclusions);
            /* Power->Distance conversion parameters */
            GetNodeAttributeOrDefault(t_tree, "conversion_a", m_fExpA, m_fExpA);
            GetNodeAttributeOrDefault(t_tree, "conversion_b", m_fExpB, m_fExpB);
            
            GetNodeAttributeOrDefault(t_tree, "loss_probability", m_fLossProbability, m_fLossProbability);
            m_pcRNG = CARGoSRandom::CreateRNG("argos");
            /* REAL readings
              DISTANCE  mu      sigma
            0       80 6.828551 0.1418425
            1       60 6.828551 0.1418425
            2       45 6.823188 0.1531546
            3       30 6.917447 0.1216533
            4       15 6.984497 0.1092330
            5       10 7.084636 0.1529397
            6        5 7.249550 0.1926492
            7        2 7.596525 0.3192310
            8        0 7.646890 0.3570609
             */
            m_cMuValues.push_back(std::pair<Real, Real > (0, 7.646890));
            m_cMuValues.push_back(std::pair<Real, Real > (2, 7.596525));
            m_cMuValues.push_back(std::pair<Real, Real > (5, 7.249550));
            m_cMuValues.push_back(std::pair<Real, Real > (10, 7.084636));
            m_cMuValues.push_back(std::pair<Real, Real > (15, 6.984497));
            m_cMuValues.push_back(std::pair<Real, Real > (30, 6.917447));
            m_cMuValues.push_back(std::pair<Real, Real > (45, 6.823188));
            m_cMuValues.push_back(std::pair<Real, Real > (60, 6.828551));
            m_cMuValues.push_back(std::pair<Real, Real > (80, 6.828551));
            m_cSigmaValues.push_back(std::pair<Real, Real > (0, 0.3570609));
            m_cSigmaValues.push_back(std::pair<Real, Real > (2, 0.3192310));
            m_cSigmaValues.push_back(std::pair<Real, Real > (5, 0.1926492));
            m_cSigmaValues.push_back(std::pair<Real, Real > (10, 0.1529397));
            m_cSigmaValues.push_back(std::pair<Real, Real > (15, 0.1092330));
            m_cSigmaValues.push_back(std::pair<Real, Real > (30, 0.1216533));
            m_cSigmaValues.push_back(std::pair<Real, Real > (45, 0.1531546));
            m_cSigmaValues.push_back(std::pair<Real, Real > (60, 0.1418425));
            m_cSigmaValues.push_back(std::pair<Real, Real > (80, 0.1418425));
        } catch (CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Initialization error in e-puck range and bearing sensor.", ex);
        }
    }

    /****************************************/

    /****************************************/

    void CEPuckRangeAndBearingSensor::SetEntity(CEntity& c_entity) {
        /* Treat the entity as composable */
        CComposableEntity* pcComposableEntity = dynamic_cast<CComposableEntity*> (&c_entity);
        if (pcComposableEntity != NULL) {
            /* The entity is composable, does it have the required components? */
            if (pcComposableEntity->HasComponent("rab_equipped_entity") &&
                    pcComposableEntity->HasComponent("embodied_entity") &&
                    pcComposableEntity->HasComponent("controllable_entity")) {
                /* Yes, it does */
                m_pcRABEquippedEntity = &(pcComposableEntity->GetComponent<CRABEquippedEntity > ("rab_equipped_entity"));
                m_pcEmbodiedEntity = &(pcComposableEntity->GetComponent<CEmbodiedEntity > ("embodied_entity"));
                m_pcControllableEntity = &(pcComposableEntity->GetComponent<CControllableEntity > ("controllable_entity"));
                m_pcEntity = &c_entity;
            } else {
                /* No, error */
                THROW_ARGOSEXCEPTION("Cannot associate a range and bearing sensor to an entity of type \"" << c_entity.GetTypeDescription() << "\"");
            }
        }
    }

    /****************************************/

    /****************************************/

    void CEPuckRangeAndBearingSensor::Update() {
        Real fDistance;
        Real fMu;
        Real fSigma;
        Real fPower;
        /* Get robot position */
        const CVector3& cRobotPosition = m_pcEmbodiedEntity->GetPosition();
        /* Get robot orientation */
        CRadians cTmp1, cTmp2, cOrientationZ;
        m_pcEmbodiedEntity->GetOrientation().ToEulerAngles(cOrientationZ, cTmp1, cTmp2);
        /* Get the coordinates of the cell where the robot center is located */
        SInt32 nI, nJ, nK;
        m_cRABEquippedSpaceHash.SpaceToHashTable(nI, nJ, nK, cRobotPosition);
        /* Buffer for calculating the message--robot distance */
        CVector3 cVectorToMessage;
        CVector3 cVectorRobotToMessage;
        Real fMessageDistance;
        /* Buffer for the received packet */
        SEPuckRangeAndBearingReceivedPacket sPacket;
        /* Temp buffer of received packets*/
        std::vector<SEPuckRangeAndBearingReceivedPacket> tTempReceivedPackets;
        /* Initialize the occlusion check ray start to the position of the robot */
        CRay cOcclusionCheckRay;
        cOcclusionCheckRay.SetStart(cRobotPosition);
        /* Buffer to store the intersection data */
        CSpace::SEntityIntersectionItem<CEmbodiedEntity> sIntersectionData;
        /* Ignore the sensing robot when checking for occlusions */
        TEmbodiedEntitySet tIgnoreEntities;
        tIgnoreEntities.insert(m_pcEmbodiedEntity);
        /* Buffer to store the list of RAB entitities to check */
        TRABEquippedEntitySet tRABEntities;
        /*
         * 1. Go through the RAB entities in the bucket of the space hash corresponding
         *    to the center of the robot
         * 2. For each of them:
         *    a) check that the RAB entity is compatible with that of this robot
         *    b) Check if there is an occlusion
         *    c) If there isn't, get the info and set reading for that robot
         *    d) Add noise
         * 3. Select and add ONE message
         */
        if (m_cRABEquippedSpaceHash.CheckCell(nI, nJ, nK, tRABEntities)) {
            /* There are RAB entities to check, go through them */
            for (TRABEquippedEntitySet::iterator it = tRABEntities.begin();
                    it != tRABEntities.end();
                    ++it) {
                /* Create a reference to the RAB entity to check */
                CRABEquippedEntity& cRABEntity = **it;
                /* Check the RAB equipped entity is not this robot (avoid self-messaging) */
                if ((&cRABEntity != m_pcRABEquippedEntity) &&
                        (cRABEntity.GetMsgSize() == m_pcRABEquippedEntity->GetMsgSize())) {
                    /*
                     * The message is considered received if:
                     * 1. the source of the message is not beyond the range;
                     * 2. there are no occlusions between the message source and this RAB entity.
                     */
                    cVectorToMessage = cRABEntity.GetPosition();
                    cVectorRobotToMessage = cVectorToMessage;
                    cVectorRobotToMessage -= cRobotPosition;

                    /* Check that the distance is lower than the range */
                    fMessageDistance = cVectorRobotToMessage.Length();
                    if (fMessageDistance < cRABEntity.GetRange()) {
                        /* Set the ray end */
                        cOcclusionCheckRay.SetEnd(cVectorToMessage);
                        /* Check occlusion between robot and message location */
                        if ((!m_bCheckOcclusions) ||
                                (!m_cSpace.GetClosestEmbodiedEntityIntersectedByRay(sIntersectionData,
                                cOcclusionCheckRay,
                                tIgnoreEntities)) ||
                                (cRABEntity.GetId() == sIntersectionData.IntersectedEntity->GetId())
                                ) {
                            /* The message is not occluded */
                            if (m_bShowRays) m_pcControllableEntity->AddCheckedRay(false, cOcclusionCheckRay);
                            /* Set the reading */
                            sPacket.Range = cVectorRobotToMessage.Length();
                            sPacket.Bearing = cVectorRobotToMessage.GetZAngle();
                            /* Convert the reading to cm */
                            sPacket.Range *= 100.0f;
                            /** Add noise
                             * The noise is computed from the noise distribution of the real rb.
                             * The noise has a lognorm distribution that is function of the distance.
                             * The real robot measures the power of the received signal. This distance is
                             * converted in Distance using the function Max(exp(m_fExpA + m_fExpB * fPower), 0.0);
                             * To obtain the measured distance:
                             * 1- We calculate the parameters mu and sigma by interpolating
                             * the actual distance with the real data stored in m_cMuValues and m_cSigmaValues.
                             * 2- We get the simulated value of the power by drawing a number from the lognorm distribution
                             * 3- We convert the power in distance
                             */
                            fDistance = sPacket.Range - 2 * EPUCK_RADIUS;    /* Real Distance between the two edges of the robots*/
                            fMu = Interpolate(fDistance, m_cMuValues);       /* Value of the mu parameter of the lognorm distribution*/
                            fSigma = Interpolate(fDistance, m_cSigmaValues); /* Value of the fsigma parameter of the lognorm distribution*/
                            fPower = m_pcRNG->Lognormal(fSigma, fMu);        /* Value of the power obtained from the lognorm distribution*/
                            sPacket.Range = Max(exp(m_fExpA + m_fExpB * fPower), 0.0); /* Conversion from power to distance using the conversion fuction*/
                            sPacket.Bearing -= cOrientationZ;
                            sPacket.Bearing.SignedNormalize();
                            cRABEntity.GetData(sPacket.Data);

                            tTempReceivedPackets.push_back(sPacket);

                        } else {
                            /* The message is occluded */
                            if (m_bShowRays) {
                                m_pcControllableEntity->AddCheckedRay(true, cOcclusionCheckRay);
                                m_pcControllableEntity->AddIntersectionPoint(cOcclusionCheckRay, sIntersectionData.TOnRay);
                            }
                        }
                    }
                }
            }

            if (tTempReceivedPackets.size() > 0) {
                if (!m_pcRNG->Bernoulli(m_fLossProbability)) {
                    /* I select one message */
                    UInt32 selected = m_pcRNG->Uniform(CRange<UInt32 > (0, tTempReceivedPackets.size()));
                    m_tLastReceivedPackets.push_back(tTempReceivedPackets[selected]);
                }
            }
        }
    }

    /****************************************/

    /****************************************/

    void CEPuckRangeAndBearingSensor::Reset() {
        ClearPackets();
    }

    /****************************************/
    /****************************************/

    REGISTER_SENSOR(CEPuckRangeAndBearingSensor,
            "epuck_range_and_bearing", "default",
            "The e-puck range and bearing sensor",
            "Gianpiero Francesca [gianpiero.francesca@ulb.ac.be]",
            "This sensor accesses the e-puck range and bearing sensor. For a complete\n"
            "description of its usage, refer to the common interface.\n"
            "In this implementation, the readings are calculated under the assumption that\n"
            "the e-puck is always parallel to the XY plane, i.e., it rotates only around\n"
            "the Z axis. This implementation is faster than a 3D one and should be used\n"
            "REQUIRED XML CONFIGURATION\n\n"
            "  <controllers>\n"
            "    ...\n"
            "    <my_controller ...>\n"
            "      ...\n"
            "      <sensors>\n"
            "        ...\n"
            "        <epuck_range_and_bearing implementation=\"default\" />\n"
            "        ...\n"
            "      </sensors>\n"
            "      ...\n"
            "    </my_controller>\n"
            "    ...\n"
            "  </controllers>\n\n"
            "OPTIONAL XML CONFIGURATION\n\n"
            "It is possible to draw the rays shot by the distance scanner in the OpenGL\n"
            "visualization. This can be useful for sensor debugging but also to understand\n"
            "what's wrong in your controller. In OpenGL, the rays are drawn in cyan when\n"
            "they are not obstructed and in purple when they are. In case a ray is\n"
            "obstructed, a black dot is drawn where the intersection occurred.\n"
            "Additionally, the occlusion check for the range and bearing can be disabled\n"
            "completely. This is useful, for example, in collective transport, where you\n"
            "want the robot to communicate as it happens in reality (robots are very close\n"
            "and hence the IR beam is always received by some of the sensors) but the\n"
            "simulation would discard messages because occlusions are computed.\n"
            "The show_rays functionality is off by default, whereas the check occlusion\n"
            "is on. To switch on or off these functionalities, add the attribute\n"
            "'show_rays=\"true\"' and/or 'check_occlusions=\"false\"' in the XML\n"
            "as in this example:\n\n"
            "  <controllers>\n"
            "    ...\n"
            "    <my_controller ...>\n"
            "      ...\n"
            "      <sensors>\n"
            "        ...\n"
            "        <range_and_bearing implementation=\"default\"\n"
            "                           show_rays=\"true\"\n"
            "                           check_occlusions=\"false\" />\n"
            "        ...\n"
            "      </sensors>\n"
            "      ...\n"
            "    </my_controller>\n"
            "    ...\n"
            "  </controllers>\n\n"
            "Finally, there is noise on the sensor. This noise replicates the behavior\n"
            "of the real epuck rb sensor in three aspects:\n"
            "1- The range readings are affected by a noise that is function of the distance.\n"
            "2- The real rb sensor can receive ONE message each time step \n"
            "3- The real rb sensor sometimes receives messages from its own rb actuator."
            "This is replicated as loss of messages.\n"
            "The \"loss_probability\" is 0.3 by default."
            "You can modify the \"loss_probability\" as shown:\n\n"
            "  <controllers>\n"
            "    ...\n"
            "    <my_controller ...>\n"
            "      ...\n"
            "      <sensors>\n"
            "        ...\n"
            "        <range_and_bearing implementation=\"default\"\n"
            "                           loss_probability=\"0.3\" />\n"
            "        ...\n"
            "      </sensors>\n"
            "      ...\n"
            "    </my_controller>\n"
            "    ...\n"
            "  </controllers>\n\n",
            "Usable"
            );

}
