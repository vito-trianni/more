#include "flocking_qt.h"
#include "flocking_nn.h"
#include <argos2/common/utility/logging/argos_log.h>
#include <argos2/simulator/space/entities/footbot_entity.h>
#include <argos2/simulator/space/entities/controllable_entity.h>
#include <argos2/simulator/space/entities/embodied_entity.h>

using namespace argos;

/****************************************/
/****************************************/

CFlockingQt::CFlockingQt() :
		c_com_position(0, 0, 0) {
}

/****************************************/
/****************************************/

CFlockingQt::~CFlockingQt() {
}

/****************************************/
/****************************************/

void CFlockingQt::Draw(CFootBotEntity& c_entity) {

	c_com_position += c_entity.GetEmbodiedEntity().GetPosition();

}

// called only once
void CFlockingQt::DrawInWorld() {
	// compute position of the center of mass
	c_com_position /= 10;

	// move camera
	CQTOpenGLWidget& widget = GetOpenGLWidget();

	CQTOpenGLCamera& camera = widget.GetCamera();

	CQTOpenGLCamera::SSettings& csettings = camera.GetActiveSettings();


	// tracking
	CVector2 camerabasepoint(csettings.Position.GetX(),
			csettings.Position.GetY());
	CVector2 botsbasepoint(c_com_position.GetX(), c_com_position.GetY());

	CRadians minAllowedSightAngle;
	// minimum angle is now 30 degrees
	minAllowedSightAngle.FromValueInDegrees(60);
	Real allowedmaxdistance = csettings.Position.GetZ()
			/ Tan(minAllowedSightAngle);

	CVector2 differenceVector = botsbasepoint - camerabasepoint;
	Real distance = differenceVector.Length();

	if (distance > allowedmaxdistance) {
		// adjust camera position (only x-y plane)
		Real adjustment = distance - allowedmaxdistance;
		differenceVector = differenceVector.Normalize() * adjustment;

		csettings.Position.SetX(
				camerabasepoint.GetX() + differenceVector.GetX());
		csettings.Position.SetY(
				camerabasepoint.GetY() + differenceVector.GetY());
	}

	// execute
	csettings.LookAt(c_com_position);

	// reset position
	c_com_position.SetX(0);
	c_com_position.SetY(0);
	c_com_position.SetZ(0);

}

/****************************************/
/****************************************/

REGISTER_QTOPENGL_USER_FUNCTIONS(CFlockingQt, "flocking_qt")
