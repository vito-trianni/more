#include <argos2/common/utility/logging/argos_log.h>
#include <argos2/common/utility/string_utilities.h>

#include "obstacle_avoidance_nn.h"


#define TRACK(LINE) LOGERR << "[DEBUG] " << #LINE << std::endl; LINE
#define DISPLAY(VARIABLE) LOGERR << "[DEBUG] " #VARIABLE " = " << VARIABLE << std::endl;

const string CONFIGURATION_OBSTACLEAVOIDANCE_INPUT_TYPE      = "input_type";
const string CONFIGURATION_OBSTACLEAVOIDANCE_SPEED_RANGE     = "speed_range";


static const Real SBOT2FOOTBOT = 0.029112741f/0.023f;
static const Real FOOTBOT_WHEEL = 0.029112741f;


/****************************************/
/****************************************/

CObstacleAvoidanceController::CObstacleAvoidanceController() : 
  CCI_Controller(),
  CPerceptronController(),
  m_tInputType( INPUT_RAW ),
  m_cWheelSpeedRange(-16,16) {
  m_pcProximitySensor = NULL;
  m_pcWheelsActuator = NULL;
}


/****************************************/
/****************************************/

CObstacleAvoidanceController::~CObstacleAvoidanceController() {
}


/****************************************/
/****************************************/

void CObstacleAvoidanceController::Init( TConfigurationNode& t_tree ) {
  CPerceptronController::Init( t_tree );

  // Get the type of pre-processing for the proximity sensors
  string s_input_type("");
  GetNodeAttributeOrDefault(t_tree, CONFIGURATION_OBSTACLEAVOIDANCE_INPUT_TYPE, s_input_type, s_input_type);
  if( s_input_type == "evolved" ) {
     m_tInputType = INPUT_EVOLVED;
  }
  else if( s_input_type == "handcoded" ) {
     m_tInputType = INPUT_HANDCODED;
  }
  else {
     m_tInputType = INPUT_RAW;
  }

  // Load upper and lower bounds for weigths, biases and taus
  GetNodeAttributeOrDefault(t_tree, CONFIGURATION_OBSTACLEAVOIDANCE_SPEED_RANGE, m_cWheelSpeedRange, m_cWheelSpeedRange);

  // get pointers to useful sensors and acutators
  m_pcProximitySensor = dynamic_cast<CCI_FootBotProximitySensor*>(GetRobot().GetSensor  ("footbot_proximity"));
  m_pcWheelsActuator  = dynamic_cast<CCI_FootBotWheelsActuator*>(GetRobot().GetActuator("footbot_wheels"));
}



/****************************************/
/****************************************/

void CObstacleAvoidanceController::SetInputsFromVector( const CVector2& c_vector, const CVector2& c_direction ) {
   Real x_component = c_vector.DotProduct( c_direction );
   Real y_component = c_vector.DotProduct( CVector2(c_direction).Perpendicularize() );
   SetInput( 0, x_component > 0 ?  x_component : 0);
   SetInput( 1, y_component > 0 ?  y_component : 0);
   SetInput( 2, x_component < 0 ? -x_component : 0);
   SetInput( 3, y_component < 0 ? -y_component : 0);
}

/****************************************/
/****************************************/

void CObstacleAvoidanceController::ControlStep( void ) {
   // Input from the proximity sensors
   CCI_FootBotProximitySensor::TReadings vec_proximity = m_pcProximitySensor->GetReadings();
   
   switch( m_tInputType ) {
   case INPUT_EVOLVED:
      break;
   case INPUT_HANDCODED: {
      CVector2 c_proximity_vector(0,0);
      for( UInt32 i = 0; i < CCI_FootBotProximitySensor::NUM_READINGS; ++i ) {
	 c_proximity_vector += CVector2( vec_proximity[i].Value, vec_proximity[i].Angle);
      }
      
      if( m_unNumberOfInputs == 2 ) {
	 SetInput( 0, c_proximity_vector.GetX());
	 SetInput( 1, c_proximity_vector.GetY());
      }
      else {
	 for( UInt32 i = 0; i < m_unNumberOfInputs; i++ ) {
	    CRadians c_direction = CRadians::PI*(2*i + 1)/m_unNumberOfInputs;
	    Real d_component = c_proximity_vector.DotProduct( CVector2(1.0, c_direction) );
	    SetInput( i, d_component > 0 ?  d_component : 0);
	 }
      }
      break;
   }
   case INPUT_RAW:
   default:
      CCI_FootBotProximitySensor::TReadings vec_proximity = m_pcProximitySensor->GetReadings();
      for( UInt32 i = 0; i < m_unNumberOfInputs; i++ ) {
	 SetInput( i, vec_proximity[i].Value );
      }
      break;
   }

   // compute outputs
   ComputeOutputs();

   // output to the wheels
   m_pcWheelsActuator->SetLinearVelocity( m_pfOutputs[0]*m_cWheelSpeedRange.GetSpan() + m_cWheelSpeedRange.GetMin(),
					  m_pfOutputs[1]*m_cWheelSpeedRange.GetSpan() + m_cWheelSpeedRange.GetMin() );
}


/****************************************/
/****************************************/

void CObstacleAvoidanceController::Destroy( void ) {
}


/****************************************/
/****************************************/

void CObstacleAvoidanceController::Reset( void ) {
  CPerceptronController::Reset();
}


/****************************************/
/****************************************/

void CObstacleAvoidanceController::SetOnlineParameters( const UInt32 un_num_params, const Real* pf_params ) {
  LoadNetworkParameters( un_num_params, pf_params );
}


/****************************************/
/****************************************/


/*
 * To allow dynamic loading of this controller
 */
REGISTER_CONTROLLER( CObstacleAvoidanceController, "obstacle_avoidance_controller" )

