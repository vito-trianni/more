#include <argos2/common/utility/logging/argos_log.h>
#include <argos2/common/utility/string_utilities.h>

#include "stick_pulling_nn.h"


#define TRACK(LINE) LOGERR << "[DEBUG] " << #LINE << std::endl; LINE
#define DISPLAY(VARIABLE) LOGERR << "[DEBUG] " #VARIABLE " = " << VARIABLE << std::endl;

const string CONFIGURATION_STICKPULLING_INPUTS4VEC          = "inputs_for_vector";
const string CONFIGURATION_STICKPULLING_INPUTS4VEC_OFFSET   = "inputs_for_vector_offset";
const string CONFIGURATION_STICKPULLING_CAMERA_RANGE        = "camera_range";
const string CONFIGURATION_STICKPULLING_SPEED_RANGE         = "speed_range";
const string CONFIGURATION_STICKPULLING_SATURATION_IR       = "saturation_ir";
const string CONFIGURATION_STICKPULLING_SATURATION_RED      = "saturation_red";
const string CONFIGURATION_STICKPULLING_SATURATION_BLUE     = "saturation_blue";


void NormalizeVectorSigmoid( CVector2& c_vec, const Real& f_beta ) {
   Real f_len = c_vec.Length();
   if( f_len == 0 ) return;
   c_vec.Normalize();
   c_vec *= 2/(1+exp(-f_beta*f_len)) - 1;
}

/****************************************/
/****************************************/

CStickPullingController::CStickPullingController() : 
   CCI_Controller(),
   CPerceptronController(),
   m_unNumInputsFromVector(6),
   m_fInputsFromVectorOffset(0),
   m_cWheelSpeedRange(-16.0,16.0),
   m_cCameraDistanceRange(0,50),
   m_fSaturationIR(1),
   m_fSaturationRed(1),
   m_fSaturationBlue(1),
   m_cVectorRed(0,0),
   m_cVectorBlue(0,0)
{  
   m_pcProximitySensor      = NULL;
   m_pcOmniCameraSensor     = NULL;
   m_pcWheelsActuator       = NULL;
   m_pcBeaconActuator    = NULL;
}


/****************************************/
/****************************************/

CStickPullingController::~CStickPullingController() {
}


/****************************************/
/****************************************/

void CStickPullingController::Init( TConfigurationNode& t_tree ) {
   CPerceptronController::Init( t_tree );

   // Get the number of inputs of the NN dedicated to a vector encoding
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_INPUTS4VEC, m_unNumInputsFromVector, m_unNumInputsFromVector);
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_INPUTS4VEC_OFFSET, m_fInputsFromVectorOffset, m_fInputsFromVectorOffset);
   
   // Load upper and lower bounds for actuator and sensor ranges
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_SPEED_RANGE, m_cWheelSpeedRange, m_cWheelSpeedRange);
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_CAMERA_RANGE, m_cCameraDistanceRange, m_cCameraDistanceRange);
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_SATURATION_IR, m_fSaturationIR, m_fSaturationIR);
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_SATURATION_RED, m_fSaturationRed, m_fSaturationRed);
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_SATURATION_BLUE, m_fSaturationBlue, m_fSaturationBlue);


   // get pointers to useful sensors and acutators
   m_pcProximitySensor      = dynamic_cast<CCI_FootBotProximitySensor*            >(GetRobot().GetSensor  ("footbot_proximity"             ));
   m_pcOmniCameraSensor     = dynamic_cast<CCI_FootBotOmnidirectionalCameraSensor*>(GetRobot().GetSensor  ("footbot_omnidirectional_camera"));

   m_pcWheelsActuator       = dynamic_cast<CCI_FootBotWheelsActuator*             >(GetRobot().GetActuator("footbot_wheels"                ));
   m_pcBeaconActuator       = dynamic_cast<CCI_FootBotBeaconActuator*             >(GetRobot().GetActuator("footbot_beacon"));


   m_pcOmniCameraSensor->Enable();
   // m_pcBeaconActuator->SetColor( CColor::BLUE );
}



/****************************************/
/****************************************/

void CStickPullingController::Reset( void ) {
   CPerceptronController::Reset();

   m_pcOmniCameraSensor->Enable();
   // m_pcBeaconActuator->SetColor( CColor::BLUE );

   m_cVectorRed = CVector2(0,0);
   m_cVectorBlue = CVector2(0,0);
}


/****************************************/
/****************************************/

void CStickPullingController::ControlStep( void ) {
   // Inputs from the proximity sensors - vector encoding
   SetInputsFromProximity( CRange<UInt32>(0,m_unNumInputsFromVector-1) );

   // Inputs from the camera - sigmoid of the number of robots in range
   SetInputsFromCamera( CRange<UInt32>(m_unNumInputsFromVector,3*m_unNumInputsFromVector-1) );
   
   // compute outputs
   ComputeOutputs();

   // output to the wheels
   m_pcWheelsActuator->SetLinearVelocity( m_pfOutputs[0]*m_cWheelSpeedRange.GetSpan() + m_cWheelSpeedRange.GetMin(),
    					  m_pfOutputs[1]*m_cWheelSpeedRange.GetSpan() + m_cWheelSpeedRange.GetMin() );
   
   // Switch on/off color LEDs on the front (blue color)
   if(m_pfOutputs[2] > 0.5) {
      m_pcBeaconActuator->SetColor( CColor::BLUE );
   }
   else {
      m_pcBeaconActuator->SetColor( CColor::BLACK );
   }
}


/****************************************/
/****************************************/

void CStickPullingController::Destroy( void ) {
}


/****************************************/
/****************************************/

void CStickPullingController::SetInputsFromProximity( const CRange<UInt32>& c_input_range ) {
   CCI_FootBotProximitySensor::TReadings vec_proximity = m_pcProximitySensor->GetReadings();
   CVector2 c_vector_proximity(0,0);
   for( UInt32 i = 0; i < CCI_FootBotProximitySensor::NUM_READINGS; ++i ) {
      c_vector_proximity += CVector2( vec_proximity[i].Value, vec_proximity[i].Angle);
   }
   NormalizeVectorSigmoid( c_vector_proximity, m_fSaturationIR );
   SetInputsFromVector(c_vector_proximity, c_input_range, 0.5);
}


/****************************************/
/****************************************/

void CStickPullingController::SetInputsFromCamera( const CRange<UInt32>& c_input_range ) {
   // Chec the correct number of inputs
   if( c_input_range.GetSpan()+1 != 2*m_unNumInputsFromVector ) 
      THROW_ARGOSEXCEPTION("wrong range for inputs from camera");

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
   NormalizeVectorSigmoid( m_cVectorRed, m_fSaturationRed );
   NormalizeVectorSigmoid( m_cVectorBlue, m_fSaturationBlue );

   SetInputsFromVector(m_cVectorRed,  CRange<UInt32>(c_input_range.GetMin(),c_input_range.GetMin()+m_unNumInputsFromVector-1));
   SetInputsFromVector(m_cVectorBlue, CRange<UInt32>(c_input_range.GetMin()+m_unNumInputsFromVector,c_input_range.GetMax()));
}


/****************************************/
/****************************************/

void CStickPullingController::SetInputsFromVector( const CVector2& c_vector, const CRange<UInt32>& c_input_range, const Real shift ) {
   UInt32 un_num_values = c_input_range.GetSpan() + 1;
   CRadians base_angle = CRadians::TWO_PI/un_num_values;
   for( UInt32 i = 0; i < un_num_values; i++ ) {
      CRadians c_direction = base_angle*(i + shift);
      Real d_component = c_vector.DotProduct( CVector2(1.0, c_direction) );
      SetInput( c_input_range.GetMin() + i, d_component > 0 ?  d_component : 0);
   }
}

/****************************************/
/****************************************/

void CStickPullingController::SetOnlineParameters( const UInt32 un_num_params, const Real* pf_params ) {
   LoadNetworkParameters( un_num_params, pf_params );
}

/****************************************/
/****************************************/



/*
 * To allow dynamic loading of this controller
 */
REGISTER_CONTROLLER( CStickPullingController, "stick_pulling_controller" )
