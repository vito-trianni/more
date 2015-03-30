#ifndef _COBSTACLEAVOIDANCECONTROLLER_H_
#define _COBSTACLEAVOIDANCECONTROLLER_H_


#include <argos2/common/control_interface/ci_controller.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_omnidirectional_camera_sensor.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_base_ground_sensor.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_proximity_sensor.h>

#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_wheels_actuator.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_leds_actuator.h>

#include <argos2/user/vtrianni/controllers/neural_networks/perceptron_controller.h>

using namespace argos;

class CObstacleAvoidanceController : public CCI_Controller, public CPerceptronController {
 private:
   enum TInputType {
      INPUT_RAW,
      INPUT_HANDCODED,
      INPUT_EVOLVED
   };

  CCI_FootBotWheelsActuator*              m_pcWheelsActuator;
  CCI_FootBotProximitySensor*             m_pcProximitySensor;

  TInputType                              m_tInputType;
  CRange<Real>                            m_cWheelSpeedRange;

 public:
  CObstacleAvoidanceController();
  virtual ~CObstacleAvoidanceController();
  
  virtual void Init( TConfigurationNode& t_node );
  virtual void ControlStep( void );
  virtual void Reset();
  virtual void Destroy( void );

  virtual void SetInputsFromVector( const CVector2& c_vector, const CVector2& c_direction );
  virtual void SetOnlineParameters( const UInt32 un_num_params, const Real* pf_params );

};



#endif
