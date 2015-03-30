#include "post_evaluation.h"

#include <argos2/user/vtrianni/revolver/populations/mulambda_population.h>
#include <sstream>
#include <limits.h>

using namespace argos;


const string CONFIGURATION_POSTEVALUATION_NUM_TRIALS  = "num_trials";
const string CONFIGURATION_POSTEVALUATION_EXPERIMENT  = "experiment";
const string CONFIGURATION_POSTEVALUATION_NUM_BESTIND = "num_best_individuals";


/****************************************/
/****************************************/

CPostEvaluation::CPostEvaluation() :
  CEvolution(),
  m_cSimulator( CSimulator::GetInstance()) {
  m_unNumBestGenotypes   = 0;

  m_unRankBestIndividual = 0;
  m_pcBestGenotype       = NULL;
  m_pcBestGenotypeLog    = NULL;
  m_pcCurrentBestFitness = NULL;

  m_unNumTrials          = 0;
  m_unCurrentTrialNumber = 0;
  m_punTrialSeeds        = NULL;
  
  m_sExperimentConfig    = "";
  
  m_pcPopulation         = NULL;
  
  m_pcEvolutionInstance  = this;
}


/****************************************/
/****************************************/

CPostEvaluation::~CPostEvaluation() {
  //   for( UInt32 i = 0; i < m_vecBestGenotypes.size(); i++ ) {
  //     if( m_vecBestGenotypes[i] ) delete m_vecBestGenotypes[i];
  //   }
  m_vecBestGenotypes.clear();  
  
  for( UInt32 i = 0; i < m_vecBestGenotypesLog.size(); i++ ) {
    if( m_vecBestGenotypesLog[i] ) delete m_vecBestGenotypesLog[i];
  }
  m_vecBestGenotypesLog.clear();  

  if( m_pcCurrentBestFitness ) delete m_pcCurrentBestFitness;
  
  if( m_punTrialSeeds ) delete [] m_punTrialSeeds;

  if( m_pcPopulation ) delete m_pcPopulation;
}


/****************************************/
/****************************************/

void CPostEvaluation::Init( TConfigurationNode&  t_configuration_tree ) {
  CEvolution::Init( t_configuration_tree );

  // number of trials
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_POSTEVALUATION_NUM_TRIALS, m_unNumTrials );
  if( m_unNumTrials == 0 ) {
    THROW_ARGOSEXCEPTION("[FATAL] number of trials for post-evaluation initialised to 0");
  }
  m_punTrialSeeds = new UInt32[m_unNumTrials];
    

  // simulator configuration file
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_POSTEVALUATION_EXPERIMENT, m_sExperimentConfig );

  // number of individuals to post-evaluate
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_POSTEVALUATION_NUM_BESTIND, m_unNumBestGenotypes );
  

  // Read the population subtree
  InitPopulation( GetNode( t_configuration_tree, CONFIGURATION_EVOLUTION_POPULATION ) );

  // Load the population from the working dir
  ostringstream filename;
  filename.fill( '0' );
  filename.str("");
  filename << m_sWorkingDir << m_sEvolutionBaseFilename << ".log";
  m_pcPopulation->InitIndividuals( filename.str() );
  
  // check population size consistency with the number of genotypes to
  // evaluate
  if( m_pcPopulation->GetSize() < m_unNumBestGenotypes ) {
    THROW_ARGOSEXCEPTION("[FATAL] number of individuals for post-evaluation is larger than the population" );
  }


  //   // if 0 best genotypes are specified, search for the best individual for testing
  //   if( m_unNumBestGenotypes == 0 ) {
  //     ostringstream filename;
  //     filename.fill( '0' );
  //     filename.str("");
  //     filename << m_sWorkingDir << m_sBestIndividualBaseFilename << "_rank.log";
  
  //     ifstream bestind_in;
  //     bestind_in.open( filename.str().c_str(), ios::in );
  //     if( !bestind_in ) {
  //       CSwarmanoidLogger::LogErr( ) << "[FATAL] cannot find best individual rank file " << filename.str() << endl;
  //       return -1;
  //     }
  
  //     UInt32 bestind_rank;
  //     if( !(bestind_in >> bestind_rank) ) {
  //       CSwarmanoidLogger::LogErr( ) << "[FATAL] bad formatted rank in file " << filename.str() << endl;
  //       return -1;
  //     }
  //     bestind_in.close();
  
  //     m_pcPopulation->Rank( m_bMaximization );
  //     m_pcBestGenotype = m_pcPopulation->GetIndividual( bestind_rank );
  //   }
  


  // Initialise the general fitness log
  m_pcCurrentBestFitness = new CFitnessData( 0, m_bMaximization );
}


/****************************************/
/****************************************/

void CPostEvaluation::InitPopulation( TConfigurationNode& t_configuration_tree ) {
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

void CPostEvaluation::Evolve( void ) {
  StartEvaluationProcess();
  
  // initialise random number generator for this generation
  SetRandomSeed( m_unRandomSeed );
  
  // compute new trial seeds for this post-evaluation
  for( UInt32 i = 0; i < m_unNumTrials; i++ ) {
    m_punTrialSeeds[i] = m_pcRNG->Uniform( CRange<UInt32>(0, UINT_MAX) );
  }

  //   // if only the best individual must be evaluated, proceed and return
  //   if( m_unNumBestGenotypes == 0 ) {
  //     m_pcBestGenotypeLog = new CFitnessLog( 1, m_bMaximization );
  //     Real fitness = EvaluateIndividual( m_pcBestGenotype, m_pcBestGenotypeLog );
  //     CSwarmanoidLogger::LogErr() << "[INFO] test results: overall fitness:  " << fitness << endl;
  
  //     delete m_pcBestGenotypeLog;  // not used for the time being
  //     return;
  //   }



  // sequentially evaluate the best individuals in the population,
  // store fitness values in the logging class and store best
  // individual
  m_pcPopulation->Rank( m_bMaximization );

  TGenotypeVec vec_genotypes = m_pcPopulation->GetIndividuals();
  for( UInt32 i = 0; i < m_unNumBestGenotypes; i++ ) {
    CGenotype*   individual    = vec_genotypes[i];
    CFitnessLog* individualLog = new CFitnessLog( 1, m_bMaximization );

    m_vecBestGenotypes.push_back(individual);
    m_vecBestGenotypesLog.push_back(individualLog);
    
    LOG << "[INFO] Evaluating individual " << setw(3) << i << endl;
    Real fitness = EvaluateIndividual( individual, individualLog );
    
    bool new_best = m_bMaximization ? (fitness >= m_pcCurrentBestFitness->fitness) : (fitness <= m_pcCurrentBestFitness->fitness);
    if( new_best ) {
      m_unRankBestIndividual = i;
      m_pcBestGenotype = individual;
      m_pcBestGenotypeLog = individualLog;
      m_pcCurrentBestFitness->fitness = fitness;
    }
  }

  LOG << "[INFO] post-evaluation results: best individual number is " << m_unRankBestIndividual << endl;
  LOG << "[INFO] post-evaluation results: best individual fitness:  " << m_pcCurrentBestFitness->fitness << endl;

  WriteAll();

  StopEvaluationProcess();
}



/****************************************/
/****************************************/

void CPostEvaluation::StartEvaluationProcess( void ) {
  char* pch_ahss_install_dir = getenv( "ARGOSINSTALLDIR" );
  if ( pch_ahss_install_dir == NULL ) {
    LOGERR << "[FATAL] Unable to find the required environment variable AHSSINSTALLDIR" << endl;
    LOGERR.Flush();
    exit( -1 );
  }

  // load the experiment
  m_cSimulator.SetInstallationDirectory( pch_ahss_install_dir );
  m_cSimulator.SetExperimentFileName( m_sExperimentConfig );
  m_cSimulator.LoadExperiment();
}


/****************************************/
/****************************************/

void CPostEvaluation::StopEvaluationProcess( void ) {
  m_cSimulator.Destroy();
}


/****************************************/
/****************************************/

Real CPostEvaluation::EvaluateIndividual( CGenotype* pc_genotype, CFitnessLog* pc_fitness_log ) {
  UInt32 num_components = 0;
  Real*      components = NULL;
  Real      avg_fitness = 0;
  
  // get the loopfunction and set the control parameters for this genotype
  m_cSimulator.GetLoopFunctions().SetOnlineControlParameters( pc_genotype->GetNumValues(), pc_genotype->GetGeneValues() );

  for( m_unCurrentTrialNumber = 0; m_unCurrentTrialNumber < m_unNumTrials; m_unCurrentTrialNumber++ ) {
    // set the random seed
    m_cSimulator.SetRandomSeed(m_punTrialSeeds[m_unCurrentTrialNumber] );

    // reset experiment to the initial conditions
    m_cSimulator.Reset();

    // run the simulation
    m_cSimulator.Execute();


    // retrieve the fitness values
    Real fitness = m_cSimulator.GetLoopFunctions().ComputePerformanceInExperiment( &num_components, &components );
    LOG << "[INFO]     trial " << setw(3) << m_unCurrentTrialNumber << " fitness: " << fitness << endl;
    
    // insert the values into the individual fitness log
    pc_fitness_log->AddFitnessData( m_unCurrentTrialNumber, fitness, num_components, components );

    // compute the average fitness
    avg_fitness += fitness;
  }

  avg_fitness /= m_unNumTrials;
  pc_genotype->SetFitness( avg_fitness );
  pc_genotype->SetEvaluated( true );
  
  return avg_fitness;
}


/****************************************/
/****************************************/

void CPostEvaluation::Write( ostream& os ) {
  os << "<?xml version=\"1.0\" ?>" << endl;
  os << "<revolver-configuration>" << endl;
  os << "<" << CONFIGURATION_EVOLUTION_EVOLUTION << ">" << endl;
  os << "\t" << CONFIGURATION_EVOLUTION_TYPE << " = \"" << CONFIGURATION_POSTEVALUATION_TYPE << "\"" << endl;

  CEvolution::Write( os );


  os << "\t" << CONFIGURATION_POSTEVALUATION_NUM_TRIALS  << " = \"" << m_unNumTrials        << "\"" << endl;
  os << "\t" << CONFIGURATION_POSTEVALUATION_EXPERIMENT  << " = \"" << m_sExperimentConfig  << "\"" << endl;
  os << "\t" << CONFIGURATION_POSTEVALUATION_NUM_BESTIND << " = \"" << m_unNumBestGenotypes << "\"" << endl;
  os << ">" << endl;

  if( m_pcPopulation != NULL ) {
    m_pcPopulation->Write( os );
  }

  os << "</" << CONFIGURATION_EVOLUTION_EVOLUTION << ">" << endl;
  os << "</revolver-configuration>" << endl;
}



/****************************************/
/****************************************/

void CPostEvaluation::WriteAll( void ) {
  WriteBestIndividuals( m_unNumBestGenotypes );
  WriteFitnessLogs();
}


/****************************************/
/****************************************/

void CPostEvaluation::WriteFitnessLogs( void ) {
  ofstream out;
  ostringstream filename;
  filename.fill( '0' );
  
  // write the best individual fitness log
  filename.str("");
  filename << m_sWorkingDir << m_sFitnessFilename << ".log";
  out.open( filename.str().c_str(), ios::out );
  m_pcBestGenotypeLog->Write( out );
  out.close();

  
  // for all tested individuals, write the respective data
  for( UInt32 i = 0; i < m_vecBestGenotypesLog.size(); i++ ) {
    CFitnessLog* individualLog = m_vecBestGenotypesLog[i];
    filename.str("");
    filename << m_sWorkingDir << m_sFitnessFilename << "_n" << setw(3) << i << ".log";
    out.open( filename.str().c_str(), ios::out );
    individualLog->Write( out );
    out.close();
  }
}


/****************************************/
/****************************************/

void CPostEvaluation::WriteBestIndividuals( UInt32 un_num_best ) {
  ofstream bestind_out;
  ostringstream bestind_filename;
  bestind_filename.fill( '0' );
  
  // write the best individual
  bestind_filename.str("");
  bestind_filename << m_sWorkingDir << m_sBestIndividualBaseFilename << ".log";
  bestind_out.open( bestind_filename.str().c_str(), ios::out );
  const UInt32 num_values = m_pcBestGenotype->GetNumValues();
  const Real* values = m_pcBestGenotype->GetGeneValues();
  
  bestind_out << num_values;
  for( UInt32 i = 0; i < num_values; i++ ) {
    bestind_out << " " << values[i];
  }
  bestind_out << endl;
  bestind_out.close();
  
  // write the rank of the best individual in a file
  bestind_filename.str("");
  bestind_filename << m_sWorkingDir << m_sBestIndividualBaseFilename << "_rank.log";
  bestind_out.open( bestind_filename.str().c_str(), ios::out );
  bestind_out << m_unRankBestIndividual;
  bestind_out << endl;
  bestind_out.close();
  
  // for all tested individuals, write the respective data
  for( UInt32 i = 0; i < m_vecBestGenotypes.size(); i++ ) {
    CGenotype*   individual    = m_vecBestGenotypes[i];
    bestind_filename.str("");
    bestind_filename << m_sWorkingDir << m_sBestIndividualBaseFilename << "_n" << setw(3) << i << ".log";
    bestind_out.open( bestind_filename.str().c_str(), ios::out );
    const UInt32 num_values = individual->GetNumValues();
    const Real* values = individual->GetGeneValues();
    
    bestind_out << num_values;
    for( UInt32 i = 0; i < num_values; i++ ) {
      bestind_out << " " << values[i];
    }
    bestind_out << endl;
    bestind_out.close();
  }
}


