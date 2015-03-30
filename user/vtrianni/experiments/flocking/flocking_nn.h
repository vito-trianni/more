#ifndef _CFLOCKINGCONTROLLER_H_
#define _CFLOCKINGCONTROLLER_H_

#include <argos2/common/utility/math/vector2.h>

#include <argos2/common/control_interface/ci_controller.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_proximity_sensor.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_omnidirectional_camera_sensor.h>

#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_wheels_actuator.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_leds_actuator.h>

#include <argos2/user/vtrianni/controllers/neural_networks/perceptron_controller.h>

using namespace argos;

class CFlockingController : public CCI_Controller, public CPerceptronController {
 private:
   CCI_FootBotProximitySensor*             m_pcProximitySensor;
   CCI_FootBotOmnidirectionalCameraSensor* m_pcOmniCameraSensor;

   CCI_FootBotWheelsActuator*              m_pcWheelsActuator;
   CCI_FootBotLedsActuator*                m_pcLedsActuator;

   UInt32                                  m_unNumInputsFromVector;
   CRange<Real>                            m_cWheelSpeedRange;
   CRange<Real>                            m_cCameraDistanceRange;
   string                                  m_sLedConfiguration;

   CVector2                                m_cVectorRed;
   CVector2                                m_cVectorBlue;

 public:
   CFlockingController();
   virtual ~CFlockingController();
  
   virtual void Init( TConfigurationNode& t_node );
   virtual void ControlStep( void );
   virtual void Reset();
   virtual void Destroy( void );

   virtual void SetInputsFromProximity( const CRange<UInt32>& c_input_range );
   virtual void SetInputsFromCamera( const CRange<UInt32>& c_input_range );
   virtual void SetInputsFromVector( const CVector2& c_vector, const CRange<UInt32>& c_input_range );
   virtual void SetOnlineParameters( const UInt32 un_num_params, const Real* pf_params );

   virtual inline const CVector2 GetVectorRed() { return m_cVectorRed; };
   virtual inline const CVector2 GetVectorBlue() { return m_cVectorBlue; };

};



#endif
