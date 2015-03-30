#include "parallel_evolution.h"

#include <argos2/user/vtrianni/revolver/populations/mulambda_population.h>
#include <sstream>
#include <limits.h>


using namespace argos;

const string CONFIGURATION_PARALLELEVOLUTION_NUMSAMPLES   = "num_samples";
const string CONFIGURATION_PARALLELEVOLUTION_NUMPROCESSES = "num_processes";
const string CONFIGURATION_PARALLELEVOLUTION_ENVIRONMENT  = "parallel_environment";
const string CONFIGURATION_PARALLELEVOLUTION_EXPERIMENT   = "experiment";

/****************************************/
/****************************************/

CParallelEvolution::CParallelEvolution() :
   CEvolution() {
   m_pcPopulation   = NULL;
   m_pcFitnessLog   = NULL;
   m_pcBestGenotype = NULL;
  
   m_unNumSamples   = 0;
   m_punSampleSeeds = NULL;
  
   m_unNumProcesses = 0;
   m_tParallelEnvironmentType = PE_PVM;
   m_unMsgTag = 0;
  
   m_sExperimentConfigurationFile = "";
  
   m_pcEvolutionInstance = this;
}



/****************************************/
/****************************************/

CParallelEvolution::~CParallelEvolution() {
   if( m_pcPopulation ) delete m_pcPopulation;
   if( m_pcFitnessLog ) delete m_pcFitnessLog;
   if( m_pcBestGenotype ) delete m_pcBestGenotype;
   if( m_punSampleSeeds ) delete [] m_punSampleSeeds;
}



/****************************************/
/****************************************/

void CParallelEvolution::Init( TConfigurationNode& t_configuration_tree ) {
   ////////////////////////////////////////////////////////////////////////////////
   // evolution initialisation
   ////////////////////////////////////////////////////////////////////////////////
   CEvolution::Init( t_configuration_tree );

   ////////////////////////////////////////////////////////////////////////////////
   // number of samples
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_PARALLELEVOLUTION_NUMSAMPLES, m_unNumSamples );
   if( m_unNumSamples > 0 ) {
      m_punSampleSeeds = new UInt32[m_unNumSamples];
   }

   ////////////////////////////////////////////////////////////////////////////////
   // number of parallel processes
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_PARALLELEVOLUTION_NUMPROCESSES, m_unNumProcesses );
  
   ////////////////////////////////////////////////////////////////////////////////
   // get the parallel environment type
   ////////////////////////////////////////////////////////////////////////////////
   string s_pe;
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_PARALLELEVOLUTION_ENVIRONMENT, s_pe );
   if( s_pe == "pvm" ) {
      m_tParallelEnvironmentType = PE_PVM;
      // enroll the process into PVM
      UInt32 info = pvm_mytid();
      if( info < 0 ) {
	 THROW_ARGOSEXCEPTION("[FATAL] cannot connect to PVM" );
      }
      
      pvm_catchout( stdout );
      pvm_catchout( stderr );
   }
   else if( s_pe == "mpi" ) {
      m_tParallelEnvironmentType = PE_MPI;
      
      // init MPI environment
      try{
	 MPI::Init();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Initialization error of MPI parallel environment", ex);
	 SInt32 errorcode;
	 MPI::COMM_WORLD.Abort(errorcode);
      }
   }
   else {
      THROW_ARGOSEXCEPTION("[FATAL] parallel environment not recognised" );
   }

   ////////////////////////////////////////////////////////////////////////////////
   // simulator configuration file
   ////////////////////////////////////////////////////////////////////////////////
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_PARALLELEVOLUTION_EXPERIMENT, m_sExperimentConfigurationFile );

   ////////////////////////////////////////////////////////////////////////////////
   // Read the population subtree
   ////////////////////////////////////////////////////////////////////////////////
   InitPopulation( GetNode( t_configuration_tree, CONFIGURATION_EVOLUTION_POPULATION ) );

   ////////////////////////////////////////////////////////////////////////////////
   // create the fitness log
   ////////////////////////////////////////////////////////////////////////////////
   m_pcFitnessLog = new CFitnessLog( m_pcPopulation->GetSize(), m_bMaximization, m_unNumGeneration );
}


/****************************************/
/****************************************/

void CParallelEvolution::InitPopulation( TConfigurationNode& t_configuration_tree ) {
   string s_population_type;
   GetNodeAttribute(t_configuration_tree, CONFIGURATION_POPULATION_TYPE, s_population_type );

   // Create the new population
   if( s_population_type == CONFIGURATION_MULAMBDA_TYPE ) {
      m_pcPopulation = new CMuLambdaPopulation();
   }
   //   else if( s_population_type == CONFIGURATION_STEADYSTATE_TYPE ) {
   //     m_pcPopulation = new CSteadyStatePopulation();
   //   }
   else {
      THROW_ARGOSEXCEPTION("Cannot recognise population type from '"
			   << s_population_type << "' in <" << CONFIGURATION_POPULATION_TYPE 
			   << "> tag passed for population initialisation");
   }

   m_pcPopulation->Init( t_configuration_tree );
}


/****************************************/
/****************************************/

void CParallelEvolution::Evolve( void ) {
   StartEvaluationProcess();

   while( m_unNumGeneration < m_unMaxGeneration ) {
      // initialise random number generator for this generation
      SetRandomSeed( m_unRandomSeed );
    
      // compute new sampled seeds for this generation
      for( UInt32 i = 0; i < m_unNumSamples; i++ ) {
	 m_punSampleSeeds[i] = m_pcRNG->Uniform( CRange<UInt32>(0, UINT_MAX) );
      }
   
      EvaluatePopulation();

      LOG << "[INFO] Generation " << m_unNumGeneration
	  << " max fitness: " << m_pcFitnessLog->GetCurrentBestFitness()
	  << " avg fitness: " << m_pcFitnessLog->GetCurrentAverageFitness() << endl;
      LOG.Flush();
    
      if( (m_unNumGeneration % m_unSaveGenerationStep == 0) ||
	  (m_unNumGeneration == m_unMaxGeneration -1) ) {
	 WriteAll();
      }

      // reproduction and selection
      m_pcPopulation->Reproduction( m_bMaximization );
      m_pcPopulation->Selection( m_bMaximization );
    
      // get a new random seed for the next generation
      m_unRandomSeed = m_pcRNG->Uniform( CRange<UInt32>(0, UINT_MAX) );
    
      m_unNumGeneration += 1;
   }

   StopEvaluationProcess();
}



/****************************************/
/****************************************/

void CParallelEvolution::StartEvaluationProcess( void ) {
   // read environment variables
   char* pch_ahss_install_dir = getenv( "ARGOSINSTALLDIR" );
   if ( pch_ahss_install_dir == NULL ) {
      TerminateParallelEnvironment();
      THROW_ARGOSEXCEPTION("[FATAL] Unable to find the required environment variable AHSSINSTALLDIR" );
   }

   // spawn child processes that will evaluate the indivudals of the population
   string command( string(pch_ahss_install_dir) + "/build/user/vtrianni/simulator/invalid" );
   string config( string(pch_ahss_install_dir) + "/" + m_sExperimentConfigurationFile );
   vector<string> args;
   args.push_back( string("-c") );
   args.push_back( config );
   args.push_back( string("-p") );
   args.push_back( ToString(m_tParallelEnvironmentType) );

   SpawnParallelProcesses( command, args );
}



/****************************************/
/****************************************/

void CParallelEvolution::StopEvaluationProcess( void ) {
   // send a stop signal to all the spawned processes
   for( TMapProcToIndIterator it = m_mapProcessToIndividual.begin(); it != m_mapProcessToIndividual.end(); it++ ) {
      StopParallelProcess( it->first );
   }

   TerminateParallelEnvironment();
}


/****************************************/
/****************************************/

void CParallelEvolution::Write( ostream& os ) {
   os << "<?xml version=\"1.0\" ?>" << endl;
   os << "<revolver-configuration>" << endl;
   os << "<" << CONFIGURATION_EVOLUTION_EVOLUTION << endl;
   os << "\t<" << CONFIGURATION_EVOLUTION_TYPE << " = \"" << CONFIGURATION_PARALLELEVOLUTION_TYPE << "\"" << endl;

   CEvolution::Write( os );

   os << "\t<" << CONFIGURATION_PARALLELEVOLUTION_NUMSAMPLES   << " = \"" << m_unNumSamples                 << "\"" << endl;
   os << "\t<" << CONFIGURATION_PARALLELEVOLUTION_NUMPROCESSES << " = \"" << m_unNumProcesses               << "\"" << endl;
   os << "\t<" << CONFIGURATION_PARALLELEVOLUTION_EXPERIMENT   << " = \"" << m_sExperimentConfigurationFile << "\"" << endl;
   os << ">" << endl;

   if( m_pcPopulation != NULL ) {
      m_pcPopulation->Write( os );
   }
   os << "</" << CONFIGURATION_EVOLUTION_EVOLUTION << ">" << endl;
   os << "</revolver-configuration>" << endl;
}



/****************************************/
/****************************************/

void CParallelEvolution::WriteAll( void ) {
   ofstream out;
   ostringstream filename;
   filename.fill( '0' );
  
   ofstream bestind_out;
   ostringstream bestind_filename;
   bestind_filename.fill( '0' );
  
   // Write the population
   filename.str("");
   filename << m_sWorkingDir << m_sEvolutionBaseFilename << setw(4) << m_unNumGeneration << ".log";
   out.open( filename.str().c_str(), ios::out );
   Write( out );
   out.close();

   // write the best individual
   filename.str("");
   bestind_filename.str("");

   filename << m_sWorkingDir << m_sBestIndividualBaseFilename << setw(4) << m_unNumGeneration << ".log";
   bestind_filename << m_sWorkingDir << "bestind.log";

   out.open( filename.str().c_str(), ios::out );
   bestind_out.open( bestind_filename.str().c_str(), ios::out );

   const UInt32 num_values = m_pcBestGenotype->GetNumValues();
   const Real* values = m_pcBestGenotype->GetGeneValues();
   out << num_values;
   bestind_out << num_values;
   for( UInt32 i = 0; i < num_values; i++ ) {
      out << " " << values[i];
      bestind_out << " " << values[i];
   }
   out << endl;
   bestind_out << endl;
   out.close();
   bestind_out.close();


   // write the fitness value
   filename.str("");
   filename << m_sWorkingDir << m_sFitnessFilename;
   out.open( filename.str().c_str(), ios::out );
   m_pcFitnessLog->Write( out );
   out.close();
}


/****************************************/
/****************************************/

void CParallelEvolution::WriteFitnessLogs( void ) {

}


/****************************************/
/****************************************/

void CParallelEvolution::WriteBestIndividuals( UInt32 un_num_best ) {

}


/****************************************/
/****************************************/

void CParallelEvolution::EvaluatePopulation() {
   // initialize some variables and pointers
   UInt32 numIndividuals   = m_pcPopulation->GetSize();

   // fist, send numProcess individuals in the queue. The queueing script
   // will write in a file the results, a file different for each
   // individual sample. 
   TGenotypeVec& individuals = m_pcPopulation->GetIndividuals();
   UInt32 queued    = 0;
   UInt32 ind       = 0;
   UInt32 remaining = numIndividuals;
   while( remaining > 0 ) {    
      // every time there are less queued evaluations than the maximum
      // number, stated in numProcesses, start a new one
      while( (queued < m_unNumProcesses) && (ind < numIndividuals) ) {
	 // skip already evaluated individuals
	 if( individuals[ind]->IsEvaluated() ) {
	    remaining -= 1;
	    ind       += 1;
	    continue;
	 }

	 // send the individual genotype to the evaluation program
	 SendIndividualParameters( ind, individuals[ind] );
	 queued  += 1;
	 ind     += 1;
      }
    
      // wait for any results: receive message and add fitness values
      ReceiveIndividualFitness();
      remaining -= 1;
      queued    -= 1;
   }
}

/****************************************/
/****************************************/

void CParallelEvolution::SendIndividualParameters( UInt32 individualNumber, CGenotype* individual ) {
   // search for the first available process
   SInt32 tid = -1;
   for( TMapProcToIndIterator it = m_mapProcessToIndividual.begin(); it != m_mapProcessToIndividual.end(); ++it ) {
      if( it->second == -1 ) {
	 tid = it->first;
	 break;
      }
   }

   if( tid == -1 ) {
      THROW_ARGOSEXCEPTION( "error: no process available to start evaluation" );
   }

   // get the individual number
   SInt32 ind = individualNumber;

   // get the individual genotype
   UInt32 numGeneValues = individual->GetNumValues();
   const Real*  geneValues    = individual->GetGeneValues();
   Real* genes = new Real[numGeneValues];
   memcpy( genes, geneValues, numGeneValues*sizeof(Real));
   
   switch( m_tParallelEnvironmentType ) {
   case PE_PVM: {
      pvm_initsend( PvmDataRaw );
      pvm_pksint32( &ind, 1, 1 );
      
      pvm_pkuint32( &numGeneValues, 1, 1 );
      pvm_pkreal( genes, numGeneValues, 1 );
      
      pvm_pkuint32( &m_unNumSamples, 1, 1 );
      pvm_pkuint32( m_punSampleSeeds, m_unNumSamples, 1 );
      
      pvm_send( tid, m_unMsgTag++ );
      break;
   }

   case PE_MPI: {
      m_cEvaluatorComm.Send(&ind, 1, MPI_INT, tid, 1);
      m_cEvaluatorComm.Send(&numGeneValues, 1, MPI_INT, tid, 1);
      m_cEvaluatorComm.Send(genes, numGeneValues, MPI_ARGOSREAL, tid, 1);
      m_cEvaluatorComm.Send(&m_unNumSamples, 1, MPI_INT, tid, 1);
      m_cEvaluatorComm.Send(m_punSampleSeeds, m_unNumSamples, MPI_INT, tid, 1);
      break;
   }

   default:
      break;
   }

   // record the individual into the map, for later retrieval
   m_mapProcessToIndividual[tid] = individualNumber;
}


/****************************************/
/****************************************/

void CParallelEvolution::ReceiveIndividualFitness( void ) {
   SInt32 tid;
   Real   fitness;
   switch( m_tParallelEnvironmentType ) {
   case PE_PVM: {
      SInt32 bytes, tag;
      SInt32 bufid = pvm_recv( -1, -1 );  
      pvm_bufinfo( bufid, &bytes, &tag, &tid );
      
      pvm_upkreal( &fitness, 1, 1 );
      // TODO: receive also fitness components
      break;
   }
      
   case PE_MPI: {
      MPI::Status status;
      m_cEvaluatorComm.Recv(&fitness, 1, MPI_ARGOSREAL, MPI_ANY_SOURCE, 1, status);
      // TODO: receive also fitness components
      tid = status.Get_source();
      break;
   }

   default:
      break;
   }


   CGenotype* individual = m_pcPopulation->GetIndividual( m_mapProcessToIndividual[tid] );
   if( individual == NULL ) {
      THROW_ARGOSEXCEPTION("requested bad individual (number " << m_mapProcessToIndividual[tid] << ")" );
   }

   individual->SetFitness( fitness );
   individual->SetEvaluated( true );
   bool new_best = m_pcFitnessLog->AddFitnessData( m_unNumGeneration, individual->GetFitness() );
   if( new_best ) {
      if( m_pcBestGenotype ) delete m_pcBestGenotype;
      m_pcBestGenotype = individual->Clone();
   }

   // reset the process2Individual map
   m_mapProcessToIndividual[tid] = -1;
}



/****************************************/
/****************************************/

void CParallelEvolution::SpawnParallelProcesses( string command, vector<string> &args ) {
   SInt32* tids = new SInt32[m_unNumProcesses];

   // build arguments vector
   char** c_args = new char*[args.size()+2];
   c_args[0] = strdup(command.c_str());
   for( UInt32 i = 0; i < args.size(); i++ ) {
      c_args[i+1] = strdup( args[i].c_str() );
   }
   c_args[args.size()+1] = NULL;

   // spawn processes
   switch( m_tParallelEnvironmentType ) {
   case PE_PVM: {
      SInt32 numt = pvm_spawn( (char*)command.c_str(), (char**)(c_args+1), PvmTaskDefault, NULL, m_unNumProcesses, tids );
      if( numt < (SInt32)m_unNumProcesses ) {
	 LOGERR << "[FATAL] error spawning processes" << endl;
	 for( UInt32 i = numt; i < m_unNumProcesses; i++ ) {
	    LOGERR << "[FATAL] could not spawn process " << i << ". Error code: " << tids[i] << endl;
	 }
	 
	 LOGERR.Flush();
	 LOG.Flush();
	 pvm_exit();
	 exit( 1 );
      }

      // initialise the map that associate to each spawned process the individual that it is evaluating
      for( UInt32 i = 0; i < m_unNumProcesses; i++ ) {
	 m_mapProcessToIndividual[tids[i]] = -1;
      }
      break;
   }

   case PE_MPI: {
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
      break;
   }
   default:
      break;
   }

   // free allocated memory
   for( UInt32 i = 0; i < args.size(); i++ ) {
      delete [] c_args[i];
   }
   delete [] tids;
}



/****************************************/
/****************************************/

void CParallelEvolution::StopParallelProcess( SInt32 n_tid ) {
   SInt32 stop = -1;
   switch( m_tParallelEnvironmentType ) {
   case PE_PVM:
      pvm_initsend( PvmDataRaw );
      pvm_pksint32( &stop, 1, 1 );
      pvm_send( n_tid, m_unMsgTag++ );
      break;

   case PE_MPI:
      m_cEvaluatorComm.Send(&stop, 1, MPI_INT, n_tid, 1);
      break;
   default:
      break;
   }
}


/****************************************/
/****************************************/

void CParallelEvolution::TerminateParallelEnvironment( void ) {
   switch( m_tParallelEnvironmentType ) {
   case PE_PVM:
      pvm_exit();
      break;
   case PE_MPI:
      MPI::Finalize();
      break;
   default:
      break;
   }
}
