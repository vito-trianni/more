#include "stick_pulling_qt.h"
#include "stick_pulling_nn.h"
#include <argos2/common/utility/logging/argos_log.h>
#include <argos2/simulator/space/entities/footbot_entity.h>
#include <argos2/simulator/space/entities/controllable_entity.h>
#include <argos2/simulator/space/entities/embodied_entity.h>

using namespace argos;

/****************************************/
/****************************************/

CStickPullingQt::CStickPullingQt() {
}

/****************************************/
/****************************************/

CStickPullingQt::~CStickPullingQt() {
}

/****************************************/
/****************************************/

void CStickPullingQt::Draw(CFootBotEntity& c_entity) {
   // get the info from the controller
   CStickPullingController& c_controller = dynamic_cast<CStickPullingController&>(c_entity.GetControllableEntity().GetController());
   Real height = 0.35;
   Real length = 0.25;
   CVector3 Z(0,0,height);

   CVector2 vector_red = c_controller.GetVectorRed()*length;
   CVector3 c_signalred_vector( vector_red.GetX(), vector_red.GetY(), height );
   DrawSegment( c_signalred_vector, Z, CColor::RED, true, false, CColor::RED );
   CVector2 vector_blue = c_controller.GetVectorBlue()*length;
   CVector3 c_signalblue_vector( vector_blue.GetX(), vector_blue.GetY(), height );
   DrawSegment( c_signalblue_vector, Z, CColor::BLUE, true, false, CColor::BLUE );
   
   UInt32 un_num_values = c_controller.GetNumInputsFromVector();
   Real   angle_offset  = c_controller.GetInputsFromVectorOffset();
   CRadians base_angle = CRadians::PI*2/un_num_values;
   for( UInt32 i = 0; i < un_num_values; i++ ) {
      CRadians c_direction = base_angle*(angle_offset + (Real)i);
      CVector2 versor(1.0, c_direction);
      Real d_redcomponent = vector_red.DotProduct( versor );
      Real d_bluecomponent = vector_blue.DotProduct( versor );
      if( d_redcomponent < 0 ) d_redcomponent = 0;
      if( d_bluecomponent < 0 ) d_bluecomponent = 0;
      CVector2 red_component( d_redcomponent, c_direction );
      CVector2 blue_component( d_bluecomponent, c_direction );
      CVector3 R( red_component.GetX(), red_component.GetY(), height );
      CVector3 B( blue_component.GetX(), blue_component.GetY(), height );
      CVector3 O( versor.GetX()*length, versor.GetY()*length, height );
      
      if( d_redcomponent < d_bluecomponent ) {
	 DrawSegment( R, Z, CColor::ORANGE, true, false, CColor::ORANGE );
	 DrawSegment( B, R, CColor::MAGENTA, true, false, CColor::MAGENTA );
	 DrawSegment( O, B, CColor::GRAY50, true, false, CColor::GRAY50 );
      }
      else {
	 DrawSegment( B, Z, CColor::MAGENTA, true, false, CColor::MAGENTA );
	 DrawSegment( R, B, CColor::ORANGE, true, false, CColor::ORANGE );
	 DrawSegment( O, R, CColor::GRAY50, true, false, CColor::GRAY50 );
      }
   }
}

/****************************************/
/****************************************/

REGISTER_QTOPENGL_USER_FUNCTIONS(CStickPullingQt, "stick_pulling_qt")
