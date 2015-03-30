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
 * @file <argos2/simulator/sensors/foot-bot/footbot_light_sensor.cpp>
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "footbot_light_sensor.h"
#include <argos2/simulator/simulator.h>
#include <argos2/simulator/space/space.h>
#include <argos2/simulator/space/entities/light_entity.h>

namespace argos {

   /****************************************/
   /****************************************/

   CFootBotLightSensor::CFootBotLightSensor() :
      m_cSpace(CSimulator::GetInstance().GetSpace()),
      m_bShowRays(false),
      m_cSensorSpacing(CRadians::TWO_PI / NUM_READINGS),
      m_cSensorHalfSpacing(m_cSensorSpacing / 2.0),
      m_bCheckOcclusions(true),
      m_pcRNG(NULL),
      m_fNoiseLevel(0.0f) {}

   /****************************************/
   /****************************************/

   CFootBotLightSensor::~CFootBotLightSensor() {
   }

   /****************************************/
   /****************************************/

   void CFootBotLightSensor::Init(TConfigurationNode& t_tree) {
      try {
         CCI_FootBotLightSensor::Init(t_tree);
         /* Show rays? */
         GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
         /* Parse noise level */
         GetNodeAttributeOrDefault(t_tree, "noise_level", m_fNoiseLevel, m_fNoiseLevel);
         /* Parse occlusion check */
         GetNodeAttributeOrDefault(t_tree, "check_occlusions", m_bCheckOcclusions, m_bCheckOcclusions);
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error in foot-bot light sensor.", ex);
      }
      /* Random number generator*/
      m_pcRNG = CARGoSRandom::CreateRNG("argos");
   }

   /****************************************/
   /****************************************/

   static SInt32 Modulo(SInt32 n_value, SInt32 un_modulo) {
      while(n_value < 0) n_value += un_modulo;
      while(n_value >= un_modulo) n_value -= un_modulo;
      return n_value;
   }

   static Real ComputeReading(Real f_distance) {
      if(f_distance > 2.5f) {
         return 0.0f;
      }
      else {
         return 4096.0f * ::exp(-f_distance * 2.0f);
      }
   }

   static Real ScaleReading(const CRadians& c_angular_distance) {
      if(c_angular_distance > CRadians::PI_OVER_TWO) {
         return 0.0f;
      }
      else {
         return (1.0f - 2.0f * c_angular_distance / CRadians::PI);
      }
   }

   /****************************************/
   /****************************************/

   void CFootBotLightSensor::Update() {
      /* Here we assume that the foot-bot is rotated only wrt to the Z axis */

      /* Erase readings */
      for(size_t i = 0; i < m_tReadings.size(); ++i) {
         m_tReadings[i].Value = 0.0f;
      }
      /* Get foot-bot position */
      const CVector3& cFootBotPosition = GetEntity().GetEmbodiedEntity().GetPosition();
      /* Get foot-bot orientation */
      CRadians cTmp1, cTmp2, cOrientationZ;
      GetEntity().GetEmbodiedEntity().GetOrientation().ToEulerAngles(cOrientationZ, cTmp1, cTmp2);
      /* Buffer for calculating the light--foot-bot distance */
      CVector3 cLightDistance;
      /* Buffer for the angle of the light wrt to the foot-bot */
      CRadians cAngleLightWrtFootbot;
      /* Initialize the occlusion check ray start to the baseline of the foot-bot */
      CRay cOcclusionCheckRay;
      cOcclusionCheckRay.SetStart(cFootBotPosition);
      /* Buffer to store the intersection data */
      CSpace::SEntityIntersectionItem<CEmbodiedEntity> sIntersectionData;
      /* Ignore the sensing robot when checking for occlusions */
      TEmbodiedEntitySet tIgnoreEntities;
      tIgnoreEntities.insert(&GetEntity().GetEmbodiedEntity());
      /*
       * 1. go through the list of light entities in the scene
       * 2. check if a light is occluded
       * 3. if it isn't, distribute the reading across the sensors
       *    NOTE: the readings are additive
       * 4. go through the sensors and clamp their values
       */
      try{
         CSpace::TAnyEntityMap& tEntityMap = m_cSpace.GetEntitiesByType("light_entity");
         for(CSpace::TAnyEntityMap::iterator it = tEntityMap.begin();
             it != tEntityMap.end();
             ++it) {
            /* Get a reference to the light */
            CLightEntity& cLight = *(any_cast<CLightEntity*>(it->second));
            /* Consider the light only if it has non zero intensity */
            if(cLight.GetIntensity() > 0.0f) {
               /* Get the light position */
               const CVector3& cLightPosition = cLight.GetPosition();
               /* Set the ray end */
               cOcclusionCheckRay.SetEnd(cLightPosition);
               /* Check occlusion between the foot-bot and the light */
               if( ! m_bCheckOcclusions ||
                   ! m_cSpace.GetClosestEmbodiedEntityIntersectedByRay(sIntersectionData, cOcclusionCheckRay, tIgnoreEntities))
               {
                  /* The light is not occluded */
                  if(m_bShowRays) GetEntity().GetControllableEntity().AddCheckedRay(false, cOcclusionCheckRay);
                  /* Get the distance between the light and the foot-bot */
                  cOcclusionCheckRay.ToVector(cLightDistance);
                  /* Linearly scale the distance with the light intensity
                     The greater the intensity, the smaller the distance */
                  cLightDistance /= cLight.GetIntensity();
                  /* Get the angle wrt to foot-bot rotation */
                  cAngleLightWrtFootbot = cLightDistance.GetZAngle();
                  cAngleLightWrtFootbot -= cOrientationZ;

                  /* Find closest sensor index to point at which ray hits footbot body */
                  // Rotate whole body by half a sensor spacing (corresponding to placement of first sensor)
                  // Division says how many sensor spacings there are between first sensor and point at which ray hits footbot body
                  // Increase magnitude of result of division to ensure correct rounding
                  Real fIdx = (cAngleLightWrtFootbot - m_cSensorHalfSpacing) / m_cSensorSpacing;
                  SInt32 nReadingIdx = (fIdx > 0) ? fIdx + 0.5f : fIdx - 0.5f;

                  /* Set the actual readings */
                  Real fReading = cLightDistance.Length();

                  // Take 6 readings before closest sensor and 6 readings after - thus we
                  // process sensors that are with 180 degrees of intersection of light
                  // ray with robot body
                  for( SInt32 nIndexOffset = -6; nIndexOffset < 7; nIndexOffset++ )
                  {
                     UInt32 unIdx = Modulo(nReadingIdx + nIndexOffset, NUM_READINGS);
                     CRadians cAngularDistanceFromOptimalLightReceptionPoint = Abs((cAngleLightWrtFootbot - m_tReadings[unIdx].Angle).SignedNormalize());
                     // ComputeReading gives value as if sensor was perfectly in line with
                     // light ray. We then linearly decrease actual reading from 1 (dist
                     // 0) to 0 (dist PI/2)
                     m_tReadings[unIdx].Value += ComputeReading(fReading) * ScaleReading(cAngularDistanceFromOptimalLightReceptionPoint);
                  }
               }
               else {
                  /* The ray is occluded */
                  if(m_bShowRays) {
                     GetEntity().GetControllableEntity().AddCheckedRay(true, cOcclusionCheckRay);
                     GetEntity().GetControllableEntity().AddIntersectionPoint(cOcclusionCheckRay, sIntersectionData.TOnRay);
                  }
               }
            }
         }
      }
      catch(argos::CARGoSException& e) {}

      /* Now go through the readings and calculate their final values */
      for(size_t i = 0; i < m_tReadings.size(); ++i) {
         /* Apply noise */
         if(m_fNoiseLevel > 0.0f) {
            m_tReadings[i].Value += m_tReadings[i].Value * m_pcRNG->Uniform(CRange<Real>(-m_fNoiseLevel, m_fNoiseLevel));
         }
         /* Clamp the value if outside range */
         CCI_FootBotLightSensor::FOOTBOT_LIGHT_SENSOR_RANGE.TruncValue(m_tReadings[i].Value);
         /* Normalize the value */
         m_tReadings[i].Value = CCI_FootBotLightSensor::FOOTBOT_LIGHT_SENSOR_RANGE.NormalizeValue(m_tReadings[i].Value);
      }
   }

   /****************************************/
   /****************************************/

   void CFootBotLightSensor::Reset() {
      for(size_t i = 0; i < m_tReadings.size(); ++i) {
         m_tReadings[i].Value = 0.0f;
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_SENSOR(CFootBotLightSensor,
                   "footbot_light", "rot_z_only",
                   "The foot-bot light sensor (optimized for 2D)",
                   "Carlo Pinciroli [cpinciro@ulb.ac.be]",
                   "This sensor accesses the foot-bot light sensor. For a complete description of\n"
                   "its usage, refer to the common interface.\n"
                   "In this implementation, the readings are calculated under the assumption that\n"
                   "the foot-bot is always parallel to the XY plane, i.e., it rotates only around\n"
                   "the Z axis. This implementation is faster than a 3D one and should be used\n"
                   "only when the assumption about the foot-bot rotation holds.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <footbot_light implementation=\"rot_z_only\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n"
                   "It is possible to draw the rays shot by the light sensor in the OpenGL\n"
                   "visualization. This can be useful for sensor debugging but also to understand\n"
                   "what's wrong in your controller. In OpenGL, the rays are drawn in cyan when\n"
                   "they are not obstructed and in purple when they are. In case a ray is\n"
                   "obstructed, a black dot is drawn where the intersection occurred.\n"
                   "To turn this functionality on, add the attribute 'show_rays=\"true\"' in the\n"
                   "XML as in this example:\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <footbot_light implementation=\"rot_z_only\"\n"
                   "                       show_rays=\"true\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n"
                   "By default, if the line of sight between a sensor in the ring and the light is\n"
                   "occluded, that sensor's reading is zero. In reality, sometimes this is not\n"
                   "true and you don't want occlusions to be checked. Try the following parameter:\n\n"
                   "  <controllers>\n"
                   "    ...\n"
                   "    <my_controller ...>\n"
                   "      ...\n"
                   "      <sensors>\n"
                   "        ...\n"
                   "        <footbot_light implementation=\"rot_z_only\"\n"
                   "                       check_occlusions=\"false\" />\n"
                   "        ...\n"
                   "      </sensors>\n"
                   "      ...\n"
                   "    </my_controller>\n"
                   "    ...\n"
                   "  </controllers>\n\n",
                   "Under development"
      )

}
