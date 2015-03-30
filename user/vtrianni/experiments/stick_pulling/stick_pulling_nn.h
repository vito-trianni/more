#ifndef _CSTICKPULLINGCONTROLLER_H_
#define _CSTICKPULLINGCONTROLLER_H_


#include <argos2/common/utility/math/vector2.h>
#include <argos2/common/control_interface/ci_controller.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_proximity_sensor.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_omnidirectional_camera_sensor.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_wheels_actuator.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_beacon_actuator.h>
#include <argos2/user/vtrianni/controllers/neural_networks/perceptron_controller.h>

using namespace argos;

class CStickPullingController : public CCI_Controller, public CPerceptronController {
 private:
   CCI_FootBotProximitySensor*             m_pcProximitySensor;
   CCI_FootBotOmnidirectionalCameraSensor* m_pcOmniCameraSensor;
   CCI_FootBotWheelsActuator*              m_pcWheelsActuator;
   CCI_FootBotBeaconActuator*              m_pcBeaconActuator;
  
   UInt32                                  m_unNumInputsFromVector;
   Real                                    m_fInputsFromVectorOffset;
   CRange<Real>                            m_cWheelSpeedRange;
   CRange<Real>                            m_cCameraDistanceRange;
   Real                                    m_fSaturationIR;
   Real                                    m_fSaturationRed;
   Real                                    m_fSaturationBlue;

   CVector2                                m_cVectorRed;
   CVector2                                m_cVectorBlue;


 public:
   CStickPullingController();
   virtual ~CStickPullingController();
  
   virtual void Init( TConfigurationNode& t_node );
   virtual void ControlStep( void );
   virtual void Reset();
   virtual void Destroy( void );

   virtual void SetInputsFromProximity( const CRange<UInt32>& c_input_range );
   virtual void SetInputsFromCamera( const CRange<UInt32>& c_input_range );
   virtual void SetInputsFromVector( const CVector2& c_vector, const CRange<UInt32>& c_input_range, const Real shift = 0 );
   virtual void SetOnlineParameters( const UInt32 un_num_params, const Real* pf_params );

   virtual inline const CVector2& GetVectorRed( void ) { return m_cVectorRed;};
   virtual inline const CVector2& GetVectorBlue( void ) { return m_cVectorBlue;};

   virtual inline const UInt32 GetNumInputsFromVector( void ) { return m_unNumInputsFromVector; };
   virtual inline const Real GetInputsFromVectorOffset( void ) { return m_fInputsFromVectorOffset; };

};



#endif
