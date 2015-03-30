#include "evolution.h"

using namespace more;

const string CONFIGURATION_EVOLUTION_TYPE            = "type";
const string CONFIGURATION_EVOLUTION_EVOLUTION       = "evolution";
const string CONFIGURATION_EVOLUTION_MAXGENERATION   = "max_generation";
const string CONFIGURATION_EVOLUTION_SAVEGENERATION  = "save_generation_step";
const string CONFIGURATION_EVOLUTION_NUMGENERATION   = "num_generation";
const string CONFIGURATION_EVOLUTION_MAXIMISTAION    = "maximisation";
const string CONFIGURATION_EVOLUTION_RANDOMSEED      = "random_seed";
const string CONFIGURATION_EVOLUTION_EVOBASEFILE     = "evolution_basefile";
const string CONFIGURATION_EVOLUTION_FITNESSBASEFILE = "fitness_basefile";
const string CONFIGURATION_EVOLUTION_BESTINDBASEFILE = "bestind_basefile";
const string CONFIGURATION_EVOLUTION_NUMSAMPLES      = "num_samples";
const string CONFIGURATION_EVOLUTION_NUMPROCESSES    = "num_processes";
const string CONFIGURATION_EVOLUTION_EXPERIMENT      = "experiment";


/****************************************/
/****************************************/

CEvolution::CEvolution() :
   m_bVerbose( true ),
   m_unRandomSeed( 1 ),
   m_bMaximization( true ),
   m_unNumGeneration( 0 ),
   m_unMaxGeneration( 0 ),
   m_unSaveGenerationStep( 1 ),
   m_sWorkingDir( "" ),
   m_sEvolutionBaseFilename(""),
   m_sBestIndividualBaseFilename(""),
   m_sFitnessBaseFilename(""),
   m_unNumSamples( 0 ),
   m_unCurrentSampleNumber( 0 ),
   m_unNumProcesses( 1 ),
   m_sExperimentConfigurationFile(""),
   m_unMsgTag( 0 )
{}


/****************************************/
/****************************************/

CEvolution::~CEvolution() {
}


/****************************************/
/****************************************/

void CEvolution::Init( TConfigurationNode& t_configuration_tree ) {
   ////////////////////////////////////////////////////////////////////////////////
   // evolutionary paramters
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_EVOLUTION_MAXIMISTAION, m_bMaximization, m_bMaximization );
   GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_EVOLUTION_NUMGENERATION, m_unNumGeneration, m_unNumGeneration );
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_EVOLUTION_MAXGENERATION, m_unMaxGeneration );
   GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_EVOLUTION_SAVEGENERATION, m_unSaveGenerationStep, m_unSaveGenerationStep );

   ////////////////////////////////////////////////////////////////////////////////
   // evolutionary base filenames
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_EVOLUTION_EVOBASEFILE, m_sEvolutionBaseFilename );
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_EVOLUTION_BESTINDBASEFILE, m_sBestIndividualBaseFilename );
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_EVOLUTION_FITNESSBASEFILE, m_sFitnessBaseFilename );

   ////////////////////////////////////////////////////////////////////////////////
   // number of samples
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_EVOLUTION_NUMSAMPLES, m_unNumSamples );
   
   ////////////////////////////////////////////////////////////////////////////////
   // number of parallel processes
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_EVOLUTION_NUMPROCESSES, m_unNumProcesses, m_unNumProcesses );
   
   ////////////////////////////////////////////////////////////////////////////////
   // experiment configuration file
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_EVOLUTION_EXPERIMENT, m_sExperimentConfigurationFile );
}


/****************************************/
/****************************************/

void CEvolution::StartEvaluationProcess() {
   ////////////////////////////////////////////////////////////////////////////////
   // read environment variables
   ////////////////////////////////////////////////////////////////////////////////
   char* pch_ahss_install_dir = getenv( "ARGOSINSTALLDIR" );
   if ( pch_ahss_install_dir == NULL ) {
      THROW_ARGOSEXCEPTION("[FATAL] Unable to find the required environment variable AHSSINSTALLDIR" );
   }

   ////////////////////////////////////////////////////////////////////////////////
   // Start MPI environment
   ////////////////////////////////////////////////////////////////////////////////
   try {
      MPI::Init();
   }
   catch(CARGoSException& ex) {
      SInt32 errorcode = 0;
      MPI::COMM_WORLD.Abort(errorcode);
      THROW_ARGOSEXCEPTION_NESTED("Initialization error of MPI parallel environment", ex);
   }
   
   ////////////////////////////////////////////////////////////////////////////////
   // spawn child processes that will evaluate the indivudals of the population
   ////////////////////////////////////////////////////////////////////////////////
   string command( string(pch_ahss_install_dir) + "/build/user/vtrianni/simulator/invalid" );
   string config( string(pch_ahss_install_dir) + "/" + m_sExperimentConfigurationFile );
   ostringstream posteval;
   posteval.fill( '0' );
   posteval << m_sWorkingDir << m_sFitnessBaseFilename << "_gen" << setw(4) << m_unNumGeneration;

   // build arguments vector
   char** c_args = new char*[8];
   c_args[0] = strdup(command.c_str());
   c_args[1] = strdup("-c");
   c_args[2] = strdup(config.c_str());
   c_args[3] = strdup( "-f" );;
   c_args[4] = strdup(posteval.str().c_str());;
   if( GetEvolutionFlag() ) {
      c_args[5] = NULL;
   }
   else {
      c_args[5] = strdup("-p");
      c_args[6] = NULL;
   }

   // try to spawn processes
   try {
      m_cEvaluatorComm = MPI::COMM_WORLD.Spawn(command.c_str(), (const char**)(c_args+1), m_unNumProcesses, MPI::Info(), 0);
   }
   catch(CARGoSException& ex) {
      THROW_ARGOSEXCEPTION_NESTED("Could not spawn process all requested processes", ex);
   }

   // initialise the map that associate to each spawned process the individual that it is evaluating
   for( UInt32 i = 0; i < m_unNumProcesses; i++ ) {
      m_mapProcessToIndividual[i] = -1;
   }

   // free allocated memory
   delete [] c_args[0];
   delete [] c_args[1];
   delete [] c_args[2];
   delete [] c_args[3];
   delete [] c_args[4];
   delete [] c_args;
}


/****************************************/
/****************************************/

void CEvolution::StopEvaluationProcess() {
   ////////////////////////////////////////////////////////////////////////////////
   // send a stop signal to all the spawned processes
   ////////////////////////////////////////////////////////////////////////////////
   SInt32 stop = -1;
   for( TMapProcessToIndividualIterator it = m_mapProcessToIndividual.begin(); it != m_mapProcessToIndividual.end(); it++ ) {
      m_cEvaluatorComm.Send(&stop, 1, MPI_INT, it->first, 1);
   }

   ////////////////////////////////////////////////////////////////////////////////
   // Terminate parallel environment
   ////////////////////////////////////////////////////////////////////////////////
   MPI::Finalize();
}


/****************************************/
/****************************************/
