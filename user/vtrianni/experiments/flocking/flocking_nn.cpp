#include <argos2/common/utility/logging/argos_log.h>
#include <argos2/common/utility/string_utilities.h>

#include "flocking_nn.h"


#define TRACK(LINE) LOGERR << "[DEBUG] " << #LINE << std::endl; LINE
#define DISPLAY(VARIABLE) LOGERR << "[DEBUG] " #VARIABLE " = " << VARIABLE << std::endl;

static const Real FOOTBOT_RADIUS = 0.085036758f;

const string CONFIGURATION_FLOCKING_INPUTS4VEC        = "inputs_for_vector";
const string CONFIGURATION_FLOCKING_SPEED_RANGE       = "speed_range";
const string CONFIGURATION_FLOCKING_CAMERA_RANGE      = "camera_range";
const string CONFIGURATION_FLOCKING_LED_CONFIGURATION = "led_configuration";


void NormalizeVectorSigmoid( CVector2& c_vec, const Real& f_beta ) {
   Real f_len = c_vec.Length();
   if( f_len == 0 ) return;
   c_vec.Normalize();
   c_vec *= 2/(1+exp(-f_beta*f_len)) - 1;
}



/****************************************/
/****************************************/

CFlockingController::CFlockingController() : 
   CCI_Controller(),
   CPerceptronController(),
   m_unNumInputsFromVector(6),
   m_cWheelSpeedRange(-16.0,16.0),
   m_cCameraDistanceRange(0,50),
   m_sLedConfiguration("000000000000"),
   m_cVectorRed(0,0),
   m_cVectorBlue(0,0) {

   m_pcProximitySensor = NULL;
   m_pcOmniCameraSensor = NULL;

   m_pcWheelsActuator = NULL;
   m_pcLedsActuator = NULL;
}


/****************************************/
/****************************************/

CFlockingController::~CFlockingController() {
}


/****************************************/
/****************************************/

void CFlockingController::Init( TConfigurationNode& t_tree ) {
   CPerceptronController::Init( t_tree );

   // Get the number of inputs of the NN dedicated to a vector encoding
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_FLOCKING_INPUTS4VEC, m_unNumInputsFromVector, m_unNumInputsFromVector);

   // Load upper and lower bounds for actuator and sensor ranges
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_FLOCKING_SPEED_RANGE, m_cWheelSpeedRange, m_cWheelSpeedRange);
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_FLOCKING_CAMERA_RANGE, m_cCameraDistanceRange, m_cCameraDistanceRange);
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_FLOCKING_LED_CONFIGURATION, m_sLedConfiguration, m_sLedConfiguration);

   // get pointers to useful sensors and acutators
   m_pcProximitySensor  = dynamic_cast<CCI_FootBotProximitySensor*>(GetRobot().GetSensor("footbot_proximity"));
   m_pcOmniCameraSensor = dynamic_cast<CCI_FootBotOmnidirectionalCameraSensor*>(GetRobot().GetSensor("footbot_omnidirectional_camera"));

   m_pcWheelsActuator   = dynamic_cast<CCI_FootBotWheelsActuator*>(GetRobot().GetActuator("footbot_wheels"));
   m_pcLedsActuator   = dynamic_cast<CCI_FootBotLedsActuator*>(GetRobot().GetActuator("footbot_leds"));

   m_pcOmniCameraSensor->Enable();

   if( m_sLedConfiguration.size() != CCI_FootBotLedsActuator::NUM_LEDS ) {
      LOGERR << "[WARNING] specified led configiration does not match the correct size (" 
	     << CCI_FootBotLedsActuator::NUM_LEDS << " LEDs)" << endl;
      LOGERR << "          all leds will are switched off" << endl;
      m_sLedConfiguration = string("000000000000");
   }

   for( UInt32 i = 0; i < m_sLedConfiguration.size(); i++ ) {
      switch( m_sLedConfiguration[i] ) {
      case 'R':
      case 'r':
	 m_pcLedsActuator->SetSingleColor( i, CColor::RED );
	 break;
      case 'B':
      case 'b':
	 m_pcLedsActuator->SetSingleColor( i, CColor::BLUE );
	 break;
      case '0':
      default:
	 m_pcLedsActuator->SetSingleColor( i, CColor::BLACK );	 
      }
   }
}



/****************************************/
/****************************************/

void CFlockingController::ControlStep( void ) {
   // Inputs from the proximity sensors
   SetInputsFromProximity( CRange<UInt32>(0,m_unNumInputsFromVector-1) );

   // Inputs from the camera - 2*m_unNumInputsFromVector inputs are computed
   SetInputsFromCamera( CRange<UInt32>(m_unNumInputsFromVector,3*m_unNumInputsFromVector-1) );

   // compute outputs
   ComputeOutputs();


   // output to the wheels
   m_pcWheelsActuator->SetLinearVelocity( m_pfOutputs[0]*m_cWheelSpeedRange.GetSpan() + m_cWheelSpeedRange.GetMin(),
    					  m_pfOutputs[1]*m_cWheelSpeedRange.GetSpan() + m_cWheelSpeedRange.GetMin() );
}



/****************************************/
/****************************************/

void CFlockingController::SetInputsFromProximity( const CRange<UInt32>& c_input_range ) {
   CCI_FootBotProximitySensor::TReadings vec_proximity = m_pcProximitySensor->GetReadings();
   CVector2 c_vector_proximity(0,0);
   for( UInt32 i = 0; i < CCI_FootBotProximitySensor::NUM_READINGS; ++i ) {
      c_vector_proximity += CVector2( vec_proximity[i].Value, vec_proximity[i].Angle);
   }
   NormalizeVectorSigmoid( c_vector_proximity, 2.0 );
   SetInputsFromVector(c_vector_proximity, c_input_range);
}


/****************************************/
/****************************************/

void CFlockingController::SetInputsFromCamera( const CRange<UInt32>& c_input_range ) {
   m_cVectorRed  = CVector2(0,0);
   m_cVectorBlue = CVector2(0,0);
   const CCI_CameraSensor::TBlobList list_leds = m_pcOmniCameraSensor->GetCameraReadings().BlobList;
   for( CCI_CameraSensor::TBlobList::const_iterator it = list_leds.begin(); it != list_leds.end(); ++it ) {
      if( m_cCameraDistanceRange.WithinMinBoundIncludedMaxBoundIncluded((*it)->Distance) ) {
	 CVector2 c_color_vector(1 - m_cCameraDistanceRange.NormalizeValue((*it)->Distance), (*it)->Angle.SignedNormalize());
	 if( (*it)->Color == CColor::RED ) {
	    m_cVectorRed += c_color_vector;
	 }
	 else if( (*it)->Color == CColor::BLUE ) {
	    m_cVectorBlue += c_color_vector;
	 }
      }
   }
   NormalizeVectorSigmoid( m_cVectorRed, 1.0 );
   NormalizeVectorSigmoid( m_cVectorBlue, 1.0 );
   
   SetInputsFromVector(m_cVectorRed,  CRange<UInt32>(c_input_range.GetMin(),c_input_range.GetMin()+m_unNumInputsFromVector-1));
   SetInputsFromVector(m_cVectorBlue, CRange<UInt32>(c_input_range.GetMin()+m_unNumInputsFromVector,c_input_range.GetMax()));
}


/****************************************/
/****************************************/

void CFlockingController::SetInputsFromVector( const CVector2& c_vector, const CRange<UInt32>& c_input_range ) {
   UInt32 un_num_values = c_input_range.GetSpan() + 1;
   for( UInt32 i = 0; i < un_num_values; i++ ) {
      CRadians c_direction = CRadians::PI*(2*i + 1)/un_num_values;
      Real d_component = c_vector.DotProduct( CVector2(1.0, c_direction) );
      SetInput( c_input_range.GetMin() + i, d_component > 0 ?  d_component : 0);
   }
}


/****************************************/
/****************************************/

void CFlockingController::Destroy( void ) {
}


/****************************************/
/****************************************/

void CFlockingController::Reset( void ) {
   CPerceptronController::Reset();


   m_pcOmniCameraSensor->Enable();

   if( m_sLedConfiguration.size() != CCI_FootBotLedsActuator::NUM_LEDS ) {
      LOGERR << "[WARNING] specified led configiration does not match the correct size (" 
	     << CCI_FootBotLedsActuator::NUM_LEDS << " LEDs)" << endl;
      LOGERR << "          all leds will are switched off" << endl;
      m_sLedConfiguration = string("000000000000");
   }

   for( UInt32 i = 0; i < m_sLedConfiguration.size(); i++ ) {
      switch( m_sLedConfiguration[i] ) {
      case 'R':
      case 'r':
	 m_pcLedsActuator->SetSingleColor( i, CColor::RED );
	 break;
      case 'B':
      case 'b':
	 m_pcLedsActuator->SetSingleColor( i, CColor::BLUE );
	 break;
      case '0':
      default:
	 m_pcLedsActuator->SetSingleColor( i, CColor::BLACK );	 
      }
   }
}


/****************************************/
/****************************************/

void CFlockingController::SetOnlineParameters( const UInt32 un_num_params, const Real* pf_params ) {
   LoadNetworkParameters( un_num_params, pf_params );
}


/****************************************/
/****************************************/


/*
 * To allow dynamic loading of this controller
 */
REGISTER_CONTROLLER( CFlockingController, "flocking_controller" )

