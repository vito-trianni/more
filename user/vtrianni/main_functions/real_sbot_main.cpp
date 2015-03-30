
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file <real_robot_main.cpp>
 *
 * @brief This file provides the standard main() function to run the AHSS
 * simulator.
 *
 * In this file the standard main() function is defined. It provides the
 * basic functionalities to run real robot simulation: parses the command line,
 * loads the experiment, runs it and disposes all the data.
 *
 * @author Eliseo Ferrante - <eferrant@ulb.ac.be>
 */

//#include "command_line_argument_parser.h"
#include <unistd.h>
#include <signal.h>
#include <getopt.h>
#include "real_sbot.h"
#include "real_sbot_wheels_actuator.h"
#include "real_sbot_gripper_actuator.h"
#include "swarmanoid_datatypes/colour.h"
#include "swarmanoid_logging.h"
#include "ci_controller.h"
#include "sbot/fix_fft.h"
#include "sbotEye.h"

using namespace std;

/**
 * @brief The standard main() function to run the sbot.
 *
 * This main() function provides tha basic functionalities to run the
 * real robot: parses the command line, loads the controller, runs
 * it and disposes all the data.
 *
 * @param argc the number of command line arguments given at the shell.
 * @param argv the actual command line arguments.
 *
 * @return 0 if everything OK; -1 in case of errors.
 *
 */

// Experiment information
const string CONFIGURATION_CONTROLLER_PARAMETERS = "parameters";
const string CONFIGURATION_ROBOT_CONTROLLER = "controller";
const string CONFIGURATION_CONTROLLER_ID = "id";
const string CONFIGURATION_CONTROLLER_ACTUATORS = "actuators";
const string CONFIGURATION_CONTROLLER_ACTUATOR_IMPLEMENTATION = "implementation";
const string CONFIGURATION_CONTROLLER_SENSORS = "sensors";
const string CONFIGURATION_CONTROLLER_SENSOR_IMPLEMENTATION = "implementation";
string experimentConfigFileName;
string s_controllerID;
/** This string defines the short command line arguments. */
const char* const m_pchShortOptions = "hl:c:i:";

/** This structure contains full information about the available command line arguments. */
/* Accepted options */
const struct option m_structLongOptions[] = {
   { "help",            0, NULL, 'h' }, /* help */
   { "log-file",        1, NULL, 'l' }, /* turn logging to file on */
   { "config-file",     1, NULL, 'c' }, /* configuration file. REQUIRED */
   { "id-controller",   1, NULL, 'i' }, /* controller ID. REQUIRED */
   { NULL,              0, NULL, 0 }    /* required to end the array */
};

// Debug only
bool debuggingArguments = false;

// Robot Declaration
CRealSBot* g_pcRealSBot;

// Sensors Declarations


// Actuators Declarations

CRealSBotWheelsActuator* pc_wheelAct;
CRealSBotGripperActuator* pc_gripAct;

void RealSbotLaunchSigHandler ( int s )
{
  CSwarmanoidLogger::LogErr(  ) << "Controller terminating due to signal handling " << endl ;
  sbot_c::setSpeed ( 0, 0 );
  close ( camfid );
  sbot_c::setTurretRotation ( 0 );
  sbot_c::setGripperAperture ( g_pcRealSBot->settings->gripperApertureDefault );
  sbot_c::setGripperElevation ( g_pcRealSBot->settings->gripperElevationDefault );
  usleep ( 2000000 );
  sbot_c::setGripperAperture ( sbot_c::getGripperAperture() );
  usleep ( 2000000 );
  sbot_c::shutdownSbot();
  sbotEyeStop();
  CSwarmanoidLogger::LogErr(  ) << "Controller terminating due to signal handling. Final death" << endl ;
  exit ( 1 );
}

int RealSbotLaunchGetTime ( void )
{

  timeval _timeval;
  gettimeofday ( &_timeval, NULL );

  long long int llnSeconds       = _timeval.tv_sec;
  long long int llnMicroSeconds  = _timeval.tv_usec;

  long long int llnResult = llnSeconds * ( long long int ) 1000 + llnMicroSeconds / ( long long int ) 1000;

  return ( int ) llnResult;  // in milliseconds
}

void RealSbotLaunchWait ( int msec )
{
  int nsec = msec/1000;
  if ( nsec>0 ) sleep ( nsec );     // seconds sleep
  usleep ( ( msec % 1000 ) * 1000 );   // microsecond sleep
  return;
}

void RealSbotLaunchOutputError ( const char *error )
{
  fprintf ( stderr, "%s\n", error );
}

void PrintUsage ( CSwarmanoidLogger& pc_log, int n_exit_code){
  pc_log << "Usage: <controller_name> [OPTIONS]" << endl;
  pc_log << "   -h  --help                   display this usage information" << endl;
  pc_log << "   -l  --log-file=FILE          logging to FILE instead of stdout/stderr" << endl;
  pc_log << "   -c  --config-file=FILE       the experiment XML configuration file [REQUIRED]" << endl;
  pc_log << "   -i  --id-controller=ID       the ID of the controller you want to use [REQUIRED]" << endl;
  pc_log << endl;

  exit( n_exit_code );
}

void ParseCommandLine( int n_argument_count, char** ppch_argument_values ) {
   /* Display the arguments */
   char pch_args[1000];
   pch_args[0] = '\0';
   for (int i = 0; i < n_argument_count; i++)
   {
      strcat( pch_args, "\"" );
      strcat( pch_args, ppch_argument_values[i] );
      strcat( pch_args, "\"" );
      if ( i < n_argument_count - 1 )
	 strcat( pch_args, ", " );
   }
   DEBUG_CONFIGURATION("CCommandLineArgumentParser::ParseCommandLine( %d, [ %s ] )\n", n_argument_count, pch_args);

   int n_next_option;
   bool b_is_config_file = false;
   bool b_is_id_controller = false;

   do {
      n_next_option = getopt_long( n_argument_count, ppch_argument_values, m_pchShortOptions, m_structLongOptions, NULL );

      switch ( n_next_option ) {

	 case 'h': /* -h or --help */
	    PrintUsage( CSwarmanoidLogger::Log(  ), 0 );
	    break;

	 case 'l': /* -l or --log-file */
	    CSwarmanoidLogger::SetVerbose( true );
	    CSwarmanoidLogger::SetOutputFile( string( optarg ) );
	    CSwarmanoidLogger::Log(  ) << "[INFO] Logging to file " << optarg << " is on." << endl;
	    break;

	 case 'c': /* -c or --config-file */
	    b_is_config_file = true;
	    CSwarmanoidLogger::Log(  ) << "[INFO] Configuration file to parse is " << optarg << endl;
	    experimentConfigFileName = optarg;
	    break;

	 case '?': /* the user specified an invalid option */
	    PrintUsage( CSwarmanoidLogger::LogErr(  ), 1 );
	    break;

         case 'i' : /* -i or -id-controller */
            b_is_id_controller = true;
            CSwarmanoidLogger::Log(  ) << "[INFO] The id of the controller to use is " << optarg << endl;
            s_controllerID = optarg;
            break;

	 case -1: /* done with options */
	    break;

	 default: /* something unexpected */
	    abort(  );
      }

   } while( n_next_option != -1 );

   /* checking required parameters */
   if ( ! b_is_config_file ) {
      CSwarmanoidLogger::LogErr(  ) << "[FATAL] No -c or --config-file specified. Aborting." << endl << endl;
      PrintUsage( CSwarmanoidLogger::LogErr(  ), 1 );
   }

   if ( ! b_is_id_controller ) {
      CSwarmanoidLogger::LogErr(  ) << "[FATAL] No -i or --id-controller specified. Aborting." << endl << endl;
      PrintUsage( CSwarmanoidLogger::LogErr(  ), 1 );
   }

}


CCI_Controller* NewController ( const string s_controller_id, const TConfigurationTree& t_controller_parameters_subtree, TConfigurationTree t_controllers_subtree )
{
  DEBUG_CONTROLLER ( "file real_sbot.main.cpp function NewController( \"%s\", \"%s\" )\n", s_controller_id.c_str( ), s_robot_id.c_str( ) );

  /* Looks for a controller whose id is s_controller_id */
  string s_controller_name = "";
  string s_found_id = "";
  TConfigurationTree t_controller_subtree;

  TConfigurationTree::pre_order_iterator it_begin = t_controllers_subtree.begin( );
  it_begin++;
  TConfigurationTree::sibling_iterator it_controllers = it_begin;

  while ( it_controllers != t_controllers_subtree.end ( it_controllers ) && s_found_id != s_controller_id ) {

    /* Check that CONFIGURATION_CONTROLLER_ID attribute has been passed */
    s_found_id = it_controllers->GetNodeAttributeValue ( CONFIGURATION_CONTROLLER_ID );
    if ( s_found_id == "" ) {
      CSwarmanoidLogger::LogErr( ) << "[FATAL] Missing " << CONFIGURATION_CONTROLLER_ID << " specified in <" << CONFIGURATION_CONTROLLERS << "><" << it_controllers->GetNodeName( ) << "> section of configuration file." << endl;
      return NULL;
    }

    if ( s_found_id != s_controller_id ) {
      /* Go to the next controller */
      it_controllers++;
    }
    else {
      /* ID found, store the type name and configuration subtree */
      s_controller_name = it_controllers->GetNodeName( );
      t_controller_subtree = TConfigurationTree ( it_controllers );
    }

  } /* end while */

  /* The id was found? */
  if ( s_controller_name == "" ) {
    CSwarmanoidLogger::LogErr( ) << "[FATAL] Controller ID \"" << s_controller_id << "\" has not been found." << endl;
    return NULL;
  }



  /* Initialise actuators */
   TConfigurationTree t_actuators_subtree = CExperimentConfiguration::GetConfigurationSubTree ( t_controller_subtree, CONFIGURATION_CONTROLLER_ACTUATORS );
   if ( t_actuators_subtree.size( ) > 1 )    /* At least one actuator has been specified */
   {
     TConfigurationTree::pre_order_iterator it_act_begin = t_actuators_subtree.begin( );
     it_act_begin++;
     TConfigurationTree::sibling_iterator it_actuators = it_act_begin;
     while ( it_actuators != t_actuators_subtree.end ( it_actuators ) )
     {
       TConfigurationTree actuator_tree = TConfigurationTree ( it_actuators );
       string s_actuator_type = it_actuators->GetNodeName( );
       CCI_Actuator* ci_act        =  g_pcRealSBot->GetActuator(s_actuator_type);
       if ( ci_act != NULL )
       {
      	 if (  ci_act->Init ( actuator_tree ) != 0 )
         {
           CSwarmanoidLogger::LogErr( ) << "[FATAL] Error initialising actuator type = \"" << s_actuator_type << "\"." << endl;
           return NULL;
         }
       }
       else
       {
         CSwarmanoidLogger::LogErr( ) << "[FATAL] Unknown actuator <\"" << s_actuator_type << "\", \"" << "\">." << endl;
         return NULL;
       }
       it_actuators++;
     }
   }

   /* Initialise sensors */
   TConfigurationTree t_sensors_subtree = CExperimentConfiguration::GetConfigurationSubTree ( t_controller_subtree, CONFIGURATION_CONTROLLER_SENSORS );
   if ( t_sensors_subtree.size( ) > 1 )    /* At least one actuator has been specified */
   {
     TConfigurationTree::pre_order_iterator it_sens_begin = t_sensors_subtree.begin( );
     it_sens_begin++;
     TConfigurationTree::sibling_iterator it_sensors = it_sens_begin;
     while ( it_sensors != t_sensors_subtree.end ( it_sensors ) )
     {
       TConfigurationTree sensor_tree = TConfigurationTree ( it_sensors );
       string s_sensor_type = it_sensors->GetNodeName( );

       CCI_Sensor* ci_sens =  g_pcRealSBot->GetSensor(s_sensor_type);
       if ( ci_sens != NULL )
       {
         if ( ci_sens->Init ( sensor_tree ) != 0 )
         {
           CSwarmanoidLogger::LogErr( ) << "[FATAL] Error initialising sensor type = \"" << s_sensor_type << "\"." << endl;
           return NULL;
         }
       }
       else
       {
         CSwarmanoidLogger::LogErr( ) << "[FATAL] Unknown sensor <\"" << s_sensor_type << "\">." << endl;
         return NULL;
       }
       it_sensors++;
     }
   }

  /* Creates a controller of that type */
  CCI_Controller* pc_controller;
  pc_controller = controllerMaker();

  if(pc_controller == NULL){
    CSwarmanoidLogger::LogErr( ) << "[FATAL] The controller couldn't be created by the controllerMaker()." << endl;
  }

  /* Initialise the controller */
  pc_controller->SetRobot ( g_pcRealSBot );
  pc_controller->SetControllerId ( s_controller_id );
  pc_controller->GetRobot ( )->SetRobotId ( "sbot" );

  TConfigurationTree t_controller_general_parameters_subtree = CExperimentConfiguration::GetConfigurationSubTree ( t_controller_subtree, CONFIGURATION_CONTROLLER_PARAMETERS );
  if ( t_controller_parameters_subtree.size() > 0 )
    t_controller_general_parameters_subtree = CExperimentConfiguration::MergeConfigurationTrees ( t_controller_parameters_subtree, t_controller_general_parameters_subtree );

  if ( pc_controller->Init ( t_controller_general_parameters_subtree ) != CCI_Controller::RETURN_OK ) {
    CSwarmanoidLogger::LogErr( ) << "[FATAL] Problem with controller initialisation - exiting " << endl;
    return NULL;
  }



  /* Returns the controller */
  return pc_controller;
}


int main ( int argc, char** argv )
{

  ParseCommandLine( argc, argv );

  // Robot Initialization
  g_pcRealSBot = new CRealSBot();

  // Sensors Initializations

  // Actuators Initializations
  pc_gripAct = (CRealSBotGripperActuator*) g_pcRealSBot->GetActuator("sbot_gripper_actuator");
  pc_wheelAct = (CRealSBotWheelsActuator*) g_pcRealSBot->GetActuator("sbot_wheels");

  int n_errors = 0;

  int nControllerReturn;

  int nCurrentCtrlStepNum = 0;
  int nTimeBeforeControlStep;
  int nTimeAfterControlStep;
  int nControlIntervalMilliSec;

  // Set up settings struct pointer in RealSbot object
  g_pcRealSBot->settings = ( SbotSettings * ) & ( sbot_c::settings );

  signal ( SIGTERM, RealSbotLaunchSigHandler );
  signal ( SIGINT, RealSbotLaunchSigHandler );
  signal ( SIGKILL, RealSbotLaunchSigHandler );

  // Init bit of real sbot that cannot be done in controller init function
  // because need signal handler
  g_pcRealSBot->InitSbotMinimal ( RealSbotLaunchOutputError, RealSbotLaunchOutputError );


  /* Build configuration tree */
  if ( CExperimentConfiguration::ParseConfiguration ( experimentConfigFileName ) != 0 ) {
    /* Error parsing the configuration file */
    CSwarmanoidLogger::LogErr( ) << "[FATAL] Error parsing the configuration file - exiting " << endl;
    return -1;
  }
  TConfigurationTree t_experiment_configuration = CExperimentConfiguration::GetConfigurationTree( );
  // CExperimentConfiguration::PrintConfigurationTree ( t_experiment_configuration );

  TConfigurationTree t_controller_parameters_subtree = CExperimentConfiguration::GetConfigurationSubTree ( t_experiment_configuration, CONFIGURATION_CONTROLLER_PARAMETERS );

  TConfigurationTree t_controllers_subtree = CExperimentConfiguration::GetConfigurationSubTree ( t_experiment_configuration, CONFIGURATION_CONTROLLERS );

  CCI_Controller* pcController = NewController ( s_controllerID, t_controller_parameters_subtree, t_controllers_subtree );

  if ( pcController == NULL )
  {
    CSwarmanoidLogger::LogErr( ) << "[FATAL] Problem with controller creation - exiting " << endl;
    return -1;
  }

  // Get and initialise initial behaviour
  pc_gripAct->SetGripperApertureTorque ( 30 );

  // Get control interval
  nControlIntervalMilliSec = 100;
  CSwarmanoidLogger::Log( ) << "Main() - Setting Control Step Interval (MilliSeconds): " << nControlIntervalMilliSec  <<  endl;

  RealSbotLaunchWait ( 2000 );

  int nControllerStartTime = RealSbotLaunchGetTime();
  nTimeBeforeControlStep = nControllerStartTime;

  nControllerReturn = CCI_Controller::RETURN_CONTINUE;

  if ( debuggingArguments )
  {
    CSwarmanoidLogger::LogErr( ) << "Debugging arguments only. Now exiting" << endl;
    return 0;
  }

  while ( nControllerReturn == CCI_Controller::RETURN_CONTINUE ) {

    pcController->ControlStep();
    nTimeAfterControlStep = RealSbotLaunchGetTime();

    if ( nTimeAfterControlStep - nTimeBeforeControlStep <= nControlIntervalMilliSec ) {
      int nWaitTime = nControlIntervalMilliSec - nTimeAfterControlStep + nTimeBeforeControlStep;
      RealSbotLaunchWait ( nWaitTime );
      nTimeBeforeControlStep = nTimeAfterControlStep + nWaitTime;
    }
    else {
      if ( nControlIntervalMilliSec > 0 ) {
        CSwarmanoidLogger::LogErr( ) << "WARNING: Too much time taken in the control step function " << ( nTimeAfterControlStep - nTimeBeforeControlStep ) << " taken and the control step interval is " << nControlIntervalMilliSec << endl;
        nTimeBeforeControlStep = nTimeAfterControlStep;
      }
    }

    nCurrentCtrlStepNum++;

    if( pcController->IsControllerFinished()){
      nControllerReturn = CCI_Controller::RETURN_FINISHED;
      CSwarmanoidLogger::Log( ) << "INFO: Control loop terminating correctly after " << nCurrentCtrlStepNum << " control steps " << endl;
    }

  }

  usleep ( 8000000 );

  CSwarmanoidLogger::Log( ) << "INFO: Controller stopping - total time spent: " << ( RealSbotLaunchGetTime() - nControllerStartTime ) << ", # control steps: " << nCurrentCtrlStepNum << endl;
  pc_wheelAct->SetSBotWheelsAngularVelocity ( 0,0 );
  pcController->Destroy();
  // Why the second one?
  pc_wheelAct->SetSBotWheelsAngularVelocity ( 0,0 );
  pc_gripAct->SetGripperAperture ( g_pcRealSBot->settings->gripperApertureDefault );
  pc_gripAct->SetGripperElevation ( g_pcRealSBot->settings->gripperElevationDefault );

  g_pcRealSBot->ShutdownSbot();

  delete pcController;

  return n_errors;
}
