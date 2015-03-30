#include "standard_evolution.h"

#include <argos2/user/vtrianni/revolver/populations/mulambda_population.h>
#include <sstream>
#include <limits.h>

using namespace argos;

const string CONFIGURATION_STANDARDEVOLUTION_NUMSAMPLES = "num_samples";
const string CONFIGURATION_STANDARDEVOLUTION_EXPERIMENT = "experiment";

/****************************************/
/****************************************/

CStandardEvolution::CStandardEvolution() :
  CEvolution(),
  m_cSimulator( CSimulator::GetInstance()) {
  m_pcPopulation = NULL;
  m_pcFitnessLog = NULL;
  m_pcBestGenotype = NULL;

  m_unNumSamples = 0;
  m_punSampleSeeds = NULL;
  
  m_sExperimentConfigurationFile = "";
  
  m_pcEvolutionInstance = this;
}



/****************************************/
/****************************************/

CStandardEvolution::~CStandardEvolution() {
  if( m_pcPopulation ) delete m_pcPopulation;
  if( m_pcFitnessLog ) delete m_pcFitnessLog;
  if( m_pcBestGenotype ) delete m_pcBestGenotype;
  if( m_punSampleSeeds ) delete [] m_punSampleSeeds;
}



/****************************************/
/****************************************/

void CStandardEvolution::Init( TConfigurationNode& t_configuration_tree ) {
  // standard initialisation
  CEvolution::Init( t_configuration_tree );

  // number of samples
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_STANDARDEVOLUTION_NUMSAMPLES, m_unNumSamples );
  if( m_unNumSamples > 0 ) {
    m_punSampleSeeds = new UInt32[m_unNumSamples];
  }

  // simulator configuration file
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_STANDARDEVOLUTION_EXPERIMENT, m_sExperimentConfigurationFile );

  // Read the population subtree
  InitPopulation( GetNode( t_configuration_tree, CONFIGURATION_EVOLUTION_POPULATION ) );

  // create the fitness log
  m_pcFitnessLog = new CFitnessLog( m_pcPopulation->GetSize(), m_bMaximization, m_unNumGeneration );
}


/****************************************/
/****************************************/

void CStandardEvolution::InitPopulation( TConfigurationNode& t_configuration_tree ) {
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

void CStandardEvolution::Evolve( void ) {
  StartEvaluationProcess();

  while( m_unNumGeneration < m_unMaxGeneration ) {
    // initialise random number generator for this generation
    SetRandomSeed( m_unRandomSeed );
    
    // compute new sampled seeds for this generation
    for( UInt32 i = 0; i < m_unNumSamples; i++ ) {
      m_punSampleSeeds[i] = m_pcRNG->Uniform( CRange<UInt32>(0, UINT_MAX) );
    }

    // sequentially evaluate all individuals in the population, store
    // fitness values in the logging class and store best individual
    CGenotype* individual = m_pcPopulation->FirstIndividual();
    UInt32     ind_counter = 0;
    while( individual != NULL ) {
      // CSwarmanoidLogger::SetVerbose( SIMULATORLOGGER_NOVERBOSE ); TODO
      EvaluateIndividual( individual );
      // CSwarmanoidLogger::SetVerbose( m_unVerbose );
      bool new_best = m_pcFitnessLog->AddFitnessData( m_unNumGeneration, individual->GetFitness() );
      if( new_best ) {
	if( m_pcBestGenotype ) delete m_pcBestGenotype;
	m_pcBestGenotype = individual->Clone();
      }
      individual = m_pcPopulation->NextIndividual();

      ind_counter++;
    }

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
    // end of non interruptible phase
  }

  StopEvaluationProcess();
}



/****************************************/
/****************************************/

void CStandardEvolution::StartEvaluationProcess( void ) {
  // read environment variables
  char* pch_ahss_install_dir = getenv( "ARGOSINSTALLDIR" );
  if ( pch_ahss_install_dir == NULL ) {
    LOGERR << "[FATAL] Unable to find the required environment variable ARGOSINSTALLDIR" << endl;
    LOGERR.Flush();
    exit( -1 );
  }

  // load the experiment
  m_cSimulator.SetInstallationDirectory( string(pch_ahss_install_dir)+"/" );
  m_cSimulator.SetExperimentFileName( pch_ahss_install_dir+m_sExperimentConfigurationFile );
  m_cSimulator.LoadExperiment();
}


/****************************************/
/****************************************/

void CStandardEvolution::StopEvaluationProcess( void ) {
  m_cSimulator.Destroy();
}


/****************************************/
/****************************************/

void CStandardEvolution::EvaluateIndividual( CGenotype* pc_genotype ) {
  Real       f_fitness  = 0;
  UInt32 num_components = 0;
  Real*      components = NULL;

  // get the loopfunction and set the control parameters for this genotype
  m_cSimulator.GetLoopFunctions().SetOnlineControlParameters( pc_genotype->GetNumValues(), pc_genotype->GetGeneValues() );

  for( m_unCurrentSampleNumber = 0; m_unCurrentSampleNumber < m_unNumSamples; m_unCurrentSampleNumber++ ) {
    // set the random seed
    m_cSimulator.SetRandomSeed(m_punSampleSeeds[m_unCurrentSampleNumber] );

    // reset experiment to the initial conditions
    m_cSimulator.Reset();

    // run the simulation
    m_cSimulator.Execute();

    // retrieve the fitness values
    Real fitness = m_cSimulator.GetLoopFunctions().ComputePerformanceInExperiment( &num_components, &components );
    f_fitness += fitness;
  }

  f_fitness = f_fitness/m_unNumSamples;
  pc_genotype->SetFitness( f_fitness );
  pc_genotype->SetEvaluated( true );

  if( components != NULL ) delete [] components;
}


/****************************************/
/****************************************/

void CStandardEvolution::Write( ostream& os ) {
  os << "<?xml version=\"1.0\" ?>" << endl;
  os << "<revolver-configuration>" << endl;
  os << "<" << CONFIGURATION_EVOLUTION_EVOLUTION << endl;
  os << "\t" << CONFIGURATION_EVOLUTION_TYPE << " = \"" << CONFIGURATION_STANDARDEVOLUTION_TYPE << "\"" << endl;

  CEvolution::Write( os );

  os << "\t" << CONFIGURATION_STANDARDEVOLUTION_NUMSAMPLES << " = \"" << m_unNumSamples                 << "\"" << endl;
  os << "\t" << CONFIGURATION_STANDARDEVOLUTION_EXPERIMENT << " = \"" << m_sExperimentConfigurationFile << "\"" << endl;
  os << ">" << endl;

  if( m_pcPopulation != NULL ) {
    m_pcPopulation->Write( os );
  }
  os << "</" << CONFIGURATION_EVOLUTION_EVOLUTION << ">" << endl;
  os << "</revolver-configuration>" << endl;
}



/****************************************/
/****************************************/

void CStandardEvolution::WriteAll( void ) {
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

void CStandardEvolution::WriteFitnessLogs( void ) {

}


/****************************************/
/****************************************/

void CStandardEvolution::WriteBestIndividuals( UInt32 un_num_best ) {

}


