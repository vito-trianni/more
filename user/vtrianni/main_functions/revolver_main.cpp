#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <argos2/simulator/argos_command_line_arg_parser.h>
#include <argos2/simulator/simulator.h>

#include <argos2/user/vtrianni/revolver/evolution.h>
#include <argos2/user/vtrianni/revolver/evolutions/standard_evolution.h>
#include <argos2/user/vtrianni/revolver/evolutions/post_evaluation.h>
#include <argos2/user/vtrianni/revolver/evolutions/parallel_evolution.h>
#include <argos2/user/vtrianni/revolver/evolutions/test_evolution.h>

// #include <google/profiler.h>

using namespace argos;
using namespace std;

const unsigned int SIMULATORLOGGER_NOVERBOSE = 0;
const unsigned int SIMULATORLOGGER_VERBOSE = 1;

/**
 * Function that returns the usage rules of ATOMIC
 */
void print_usage( void ) {
   LOG << "--------------------------------------------------------------------------------" << endl;
   LOG << "[REVOLVER] Command line options:                                                " << endl;
   LOG << "    -h                         shows this message                               " << endl;
   LOG << "    -c <file>                  specifies the configuration file to be loaded    " << endl;
   LOG << "    -s <seed>                  set the random seed of the evolution             " << endl;
   LOG << "    -d <dir>                   set the working dir of the evolution             " << endl;
   LOG << "    -v                         set verbose output                               " << endl;
   LOG << "--------------------------------------------------------------------------------" << endl;
   LOG.Flush();
   LOGERR.Flush();
};


/**
 * Signal handler
 */
void signal_handler( int sig ) {
   signal(sig, SIG_IGN);

   CEvolution* evolution = CEvolution::GetInstance();
   if( evolution != NULL ) {
      evolution->WriteAll();
      evolution->WriteFitnessLogs();
      evolution->StopEvaluationProcess();
   }

   LOG << "[INFO] Revolver: Programm stopped. Data saved in log directory" << endl;
   LOG.Flush();
   LOGERR.Flush();
   exit( 0 );
}


/**
 * Main function
 */

int main( int argc, char** argv ) {
   // Command-line args  
   string 	conf_filename = "";
   string 	working_dir = "";
   long int	seed = 0;
   bool          bPrintUsage = false;
   bool          bVerbose = false;

   // disable colored output
   LOG.DisableColoredOutput();
   LOGERR.DisableColoredOutput();


   // define the class for parsing the command line
   CCommandLineArgParser cCommandLineArgs;

   // argument -h - print usage
   cCommandLineArgs.AddFlag('h',
			    "--help",
			    "Prints the command line arguments and exits",
			    bPrintUsage);
  
   // argument -v - verbose output
   cCommandLineArgs.AddFlag('v',
			    "--verbose",
			    "Verbose output of the simulator",
			    bVerbose);
  
   // argument -c - configuration file
   cCommandLineArgs.AddArgument<std::string>('c',
					     "--config",
					     "Specify the xml configuration file",
					     conf_filename);
  
   // argument -d - working directory
   cCommandLineArgs.AddArgument<std::string>('d',
					     "--dir",
					     "Specify the working directory",
					     working_dir);
  
   // argument -s - seed
   cCommandLineArgs.AddArgument<long int>('s',
					  "--seed",
					  "Specify the seed of the random number generator",
					  seed);

   // parse the command line
   try {
      cCommandLineArgs.Parse(argc, argv);
   }
   catch(CARGoSException& ex) {
      LOGERR << "Error: " << ex.what() << std::endl;
      cCommandLineArgs.PrintUsage( LOGERR );
   }


   // check print usage flag
   if( bPrintUsage ) {
      cCommandLineArgs.PrintUsage(LOGERR );
      return 0;
   }


   // open the configuration file
   if( conf_filename == "" ) {
      LOGERR  << "[FATAL] Missing required configuration file" << endl;
      LOGERR  << "[FATAL] Use -h for help" << endl;
      return -1;
   }
   else {
      LOG << "[INFO] configuration file is '" << conf_filename << "'" << endl;
   }


   // signal handler definition
   signal( SIGTERM, signal_handler );
   signal( SIGINT, signal_handler );
   signal( SIGHUP, signal_handler );
   signal( SIGQUIT, signal_handler );


   // evolution seed
   if( seed == 0 ) {
      seed = time( NULL );
   }


   ticpp::Document tConfiguration;
   tConfiguration.LoadFile(conf_filename);
   TConfigurationNode tConfigurationRoot = *tConfiguration.FirstChildElement();
   TConfigurationNode t_evolution_configuration = GetNode(tConfigurationRoot, "evolution");
  
   ////////////////////////////////////////////////////////////////////////////////
   // evolution type
   ////////////////////////////////////////////////////////////////////////////////
   string s_evolution_type;
   GetNodeAttribute( t_evolution_configuration, CONFIGURATION_EVOLUTION_TYPE, s_evolution_type );
   if( s_evolution_type == "" ) {
      LOGERR << "[FATAL] Cannot find evolution type specification in <" 
	     << CONFIGURATION_EVOLUTION_TYPE << "> tag passed for initialisation" << endl;
      LOG.Flush();
      LOGERR.Flush();
      return -1;
   }
  


   CEvolution* evolution;
   if( s_evolution_type == CONFIGURATION_STANDARDEVOLUTION_TYPE ) {
      evolution = new CStandardEvolution();
   }
   else if( s_evolution_type == CONFIGURATION_TESTEVOLUTION_TYPE ) {
      evolution = new CTestEvolution();
   }
   else if( s_evolution_type == CONFIGURATION_PARALLELEVOLUTION_TYPE ) {
      evolution = new CParallelEvolution();
   }
   else if( s_evolution_type == CONFIGURATION_POSTEVALUATION_TYPE ) {
      evolution = new CPostEvaluation();
   }
   else {
      LOGERR << "[FATAL] Evolution type not recognised in <" 
	     << CONFIGURATION_EVOLUTION_TYPE << "> tag passed for initialisation" << endl;
      LOG.Flush();
      LOGERR.Flush();
      return -1;
   }

   evolution->SetVerbose( bVerbose );

   ////////////////////////////////////////////////////////////////////////////////
   // initialise evolution seed - xml overwrites commandline
   ////////////////////////////////////////////////////////////////////////////////  
   evolution->InitRandom( seed );

   // set working dir for saving logs
   if( working_dir != "" ) {
      evolution->SetWorkingDir( working_dir );
   }

   // initialise the evolution
   evolution->Init( t_evolution_configuration );

   // start evolutiuon

   // ProfilerStart("revolve.prof");
   evolution->Evolve();
   // ProfilerStop();

   return 0;
}

