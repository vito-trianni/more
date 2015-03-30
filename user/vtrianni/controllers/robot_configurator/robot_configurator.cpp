#include "robot_configurator.h"

#include <argos2/common/utility/logging/argos_log.h>
#include <unistd.h>

using namespace argos;
using namespace std;

/****************************************/
/****************************************/

CRobotConfigurator::CRobotConfigurator() : 
  CCI_Controller() {
  m_unCurSamples = 0;
  m_unNumSamples = 0;
  
  // proximity sensors
  m_pcProximitySensor = NULL;
  m_pfProximityMinValues = NULL;
  m_bMinProximityAcquired = false;

  // ground sensors
  m_pcBaseGroundSensor = NULL;
  m_pfBaseGroundMinValues = NULL;
  m_pfBaseGroundMaxValues = NULL;
  m_bMinBaseGroundAcquired = false;
  m_bMaxBaseGroundAcquired = false;

  m_bConfigurationSaved = false;
}


/****************************************/
/****************************************/

CRobotConfigurator::~CRobotConfigurator() {}


/****************************************/
/****************************************/

void CRobotConfigurator::Init( TConfigurationNode& t_tree ) {
  // number of samples for each device to configure
  GetNodeAttribute(t_tree, "num_samples", m_unNumSamples);

  // GROUND CONFIGURATION
  m_pcProximitySensor     = dynamic_cast<CCI_FootBotProximitySensor*>(GetRobot().GetSensor("footbot_proximity"));
  m_pfProximityMinValues  = new Real[CCI_FootBotProximitySensor::NUM_READINGS];
  for( UInt32 j = 0; j < CCI_FootBotProximitySensor::NUM_READINGS; j++ ) {
    m_pfProximityMinValues[j] = 0;
  }

  // GROUND CONFIGURATION
  m_pcBaseGroundSensor     = dynamic_cast<CCI_FootBotBaseGroundSensor*>(GetRobot().GetSensor("footbot_base_ground"));
  m_pfBaseGroundMinValues  = new Real[CCI_FootBotBaseGroundSensor::NUM_READINGS];
  m_pfBaseGroundMaxValues  = new Real[CCI_FootBotBaseGroundSensor::NUM_READINGS];
  for( UInt32 j = 0; j < CCI_FootBotBaseGroundSensor::NUM_READINGS; j++ ) {
    m_pfBaseGroundMinValues[j] = 0;
    m_pfBaseGroundMaxValues[j] = 0;
  }

  GetNodeAttribute(t_tree, "configuration_file", m_sConfigurationFile);
}


/****************************************/
/****************************************/

void CRobotConfigurator::ControlStep( void ) {
  // Acquire data for BLACK ground
  if( !m_bMinProximityAcquired ) {
    if( m_unCurSamples == 0 ) {
      LOG << "Place the robot on away from obstacles and press 'enter'";
      // LOG.Flush();
      getchar();
      m_unCurSamples++;
      return;
    }
    
    CCI_FootBotProximitySensor::TReadings readings = m_pcProximitySensor->GetReadings();
    for( UInt32 j = 0; j < CCI_FootBotProximitySensor::NUM_READINGS; j++ ) {
      m_pfProximityMinValues[j] += readings[j].Value;
      LOG << readings[j].Value << " ";
    }
    LOG << endl;
    // LOG.Flush();
      
    if( ++m_unCurSamples == m_unNumSamples+1 ) {
      LOG << "[INFO] Data for proximity sensors acquired" << endl;
      // LOG.Flush();
      m_bMinProximityAcquired = true;
      m_unCurSamples = 0;
    }
    return;
  }


  // Acquire data for BLACK ground
  if( !m_bMinBaseGroundAcquired ) {
    if( m_unCurSamples == 0 ) {
      LOG << "Place the robot on a BLACK ground and press 'enter'";
      // LOG.Flush();
      getchar();
      m_unCurSamples++;
      return;
    }
    
    CCI_FootBotBaseGroundSensor::TReadings readings = m_pcBaseGroundSensor->GetReadings();
    for( UInt32 j = 0; j < CCI_FootBotBaseGroundSensor::NUM_READINGS; j++ ) {
      m_pfBaseGroundMinValues[j] += readings[j].Value;
      LOG << readings[j].Value << " ";
    }
    LOG << endl;
    // LOG.Flush();
      
    if( ++m_unCurSamples == m_unNumSamples+1 ) {
      LOG << "[INFO] Data for BLACK ground acquired" << endl;
      // LOG.Flush();
      m_bMinBaseGroundAcquired = true;
      m_unCurSamples = 0;
    }
    return;
  }


  // Acquire data for WHITE ground
  if( !m_bMaxBaseGroundAcquired ) {
    if( m_unCurSamples == 0 ) {
      LOG << "Place the robot on a WHITE ground and press 'enter'";
      // LOG.Flush();
      getchar();
      m_unCurSamples++;
      return;
    }
    
    CCI_FootBotBaseGroundSensor::TReadings readings = m_pcBaseGroundSensor->GetReadings();
    for( UInt32 j = 0; j < CCI_FootBotBaseGroundSensor::NUM_READINGS; j++ ) {
      m_pfBaseGroundMaxValues[j] += readings[j].Value;
      LOG << readings[j].Value << " ";
    }
    LOG << endl;
    // LOG.Flush();

    if( ++m_unCurSamples == m_unNumSamples + 1 ) {
      LOG << "[INFO] Data for WHITE ground acquired" << endl;
      // LOG.Flush();
      m_bMaxBaseGroundAcquired = true;
      m_unCurSamples = 0;
    }
    return;
  }


  // if data have been acquire, save to configuration file
  if( m_bMinBaseGroundAcquired && m_bMaxBaseGroundAcquired && !m_bConfigurationSaved) {
    // average and normalize proximity data
    for( UInt32 j = 0; j < CCI_FootBotProximitySensor::NUM_READINGS; j++ ) {
      m_pfProximityMinValues[j] /= (Real)m_unNumSamples;
    }

    // average and normalize base ground data
    for( UInt32 j = 0; j < CCI_FootBotBaseGroundSensor::NUM_READINGS; j++ ) {
      m_pfBaseGroundMinValues[j] /= (Real)m_unNumSamples;
      m_pfBaseGroundMaxValues[j] /= (Real)m_unNumSamples;
    }

    // open the configuration file
    ofstream out;
    out.open(m_sConfigurationFile.c_str(), ios::out);
    
    out << "<?xml version=\"1.0\" ?>" << endl;
    out << "<robot_configuration>"  << endl;
    out << "\t<proximity_sensors>" << endl;
    out << "\t\t<base_values>";
    for( UInt32 j = 0; j < CCI_FootBotProximitySensor::NUM_READINGS; j++ ) {
      out << m_pfProximityMinValues[j];
      if( j != CCI_FootBotProximitySensor::NUM_READINGS ) out << ",";
    }
    out << "</base_values>" << endl;
    out << "\t</proximity_sensors>" << endl;

    out << "\t<base_ground_sensors>" << endl;
    out << "\t\t<min_values>";
    // average and normalize data
    for( UInt32 j = 0; j < CCI_FootBotBaseGroundSensor::NUM_READINGS; j++ ) {
      out << m_pfBaseGroundMinValues[j];
      if( j != CCI_FootBotBaseGroundSensor::NUM_READINGS ) out << ",";
    }
    out << "</min_values>" << endl;

    out << "\t\t<max_values>";
    // average and normalize data
    for( UInt32 j = 0; j < CCI_FootBotBaseGroundSensor::NUM_READINGS; j++ ) {
      out << m_pfBaseGroundMaxValues[j];
      if( j != CCI_FootBotBaseGroundSensor::NUM_READINGS ) out << ",";
    }
    out << "</max_values>" << endl;

    out << "\t</base_ground_sensors>" << endl;
    out << "</robot_configuration>" << endl;    
    
    out.close();
    m_bConfigurationSaved = true;
  }
}


/****************************************/
/****************************************/

void CRobotConfigurator::Reset() {
}


/****************************************/
/****************************************/

void CRobotConfigurator::Destroy( void ) {
  delete [] m_pfBaseGroundMinValues;
  delete [] m_pfBaseGroundMaxValues;
}




/*
 * To allow dynamic loading of this controller
 */
REGISTER_CONTROLLER( CRobotConfigurator, "robot_configurator" )

