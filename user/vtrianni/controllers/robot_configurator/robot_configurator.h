#ifndef _CROBOTCONFIGURATOR_H_
#define _CROBOTCONFIGURATOR_H_


#include <argos2/common/control_interface/ci_controller.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_omnidirectional_camera_sensor.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_base_ground_sensor.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_proximity_sensor.h>

#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_wheels_actuator.h>
#include <argos2/common/control_interface/swarmanoid/footbot/ci_footbot_leds_actuator.h>

namespace argos {

  class CRobotConfigurator : public CCI_Controller {
  private:
    UInt32 m_unCurSamples;
    UInt32 m_unNumSamples;

    // proximity sensors
    CCI_FootBotProximitySensor* m_pcProximitySensor;
    Real* m_pfProximityMinValues;
    bool  m_bMinProximityAcquired;


    // ground sensors
    CCI_FootBotBaseGroundSensor* m_pcBaseGroundSensor;
    Real* m_pfBaseGroundMinValues;
    Real* m_pfBaseGroundMaxValues;
    bool  m_bMinBaseGroundAcquired;
    bool  m_bMaxBaseGroundAcquired;
    bool  m_bConfigurationSaved;

    std::string m_sConfigurationFile;

      public:
    CRobotConfigurator();
    virtual ~CRobotConfigurator();
  
    virtual void Init( TConfigurationNode& t_node );
    virtual void ControlStep( void );
    virtual void Reset();
    virtual void Destroy( void );

    inline virtual bool IsControllerFinished() const {
      return (m_bMinBaseGroundAcquired && m_bMaxBaseGroundAcquired && m_bConfigurationSaved);
    };

  };

};

#endif
