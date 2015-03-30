#ifndef _CNULLCONTROLLER_H_
#define _CNULLCONTROLLER_H_


#include <argos2/common/control_interface/ci_controller.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_omnidirectional_camera_sensor.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_base_ground_sensor.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_proximity_sensor.h>

#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_wheels_actuator.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_leds_actuator.h>

using namespace argos;

class CNullController : public CCI_Controller {
 private:
  CCI_FootBotProximitySensor*             m_pcProximitySensor;

 public:
  CNullController();
  virtual ~CNullController();
  
  virtual void Init( TConfigurationNode& t_node );
  virtual void ControlStep( void ) {};
  virtual void Reset() {};
  virtual void Destroy( void ) {};
};



#endif
