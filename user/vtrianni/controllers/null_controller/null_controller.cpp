#include "null_controller.h"

#include <argos2/common/utility/logging/argos_log.h>

/****************************************/
/****************************************/

CNullController::CNullController() : 
  CCI_Controller() {
}


/****************************************/
/****************************************/

CNullController::~CNullController() {
}


/****************************************/
/****************************************/

void CNullController::Init( TConfigurationNode& t_tree ) {
}



/****************************************/
/****************************************/


/*
 * To allow dynamic loading of this controller
 */
REGISTER_CONTROLLER( CNullController, "null_controller" )

