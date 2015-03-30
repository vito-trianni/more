#include "test_evolution.h"
#include <argos2/user/vtrianni/revolver/populations/mulambda_population.h>
#include <sstream>
#include <limits.h>

using namespace argos;

/****************************************/
/****************************************/

CTestEvolution::CTestEvolution() :
  CEvolution() {
  m_pcPopulation   = NULL;
  m_pcFitnessLog   = NULL;
  m_pcBestGenotype = NULL;

  m_pcEvolutionInstance = this;
}



/****************************************/
/****************************************/

CTestEvolution::~CTestEvolution() {
  if( m_pcPopulation ) delete m_pcPopulation;
  if( m_pcFitnessLog ) delete m_pcFitnessLog;
  if( m_pcBestGenotype ) delete m_pcBestGenotype;
}



/****************************************/
/****************************************/

void CTestEvolution::Init( TConfigurationNode& t_configuration_tree ) {
  CEvolution::Init( t_configuration_tree );

  // Read the population subtree
  InitPopulation( GetNode( t_configuration_tree, CONFIGURATION_EVOLUTION_POPULATION ) );

  ////////////////////////////////////////////////////////////////////////////////
  // initialise the fitness log
  ////////////////////////////////////////////////////////////////////////////////
  m_pcFitnessLog = new CFitnessLog( m_pcPopulation->GetSize(), m_bMaximization, m_unNumGeneration );
}


/****************************************/
/****************************************/

void CTestEvolution::InitPopulation( TConfigurationNode& t_configuration_tree ) {
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

void CTestEvolution::StartEvaluationProcess( void ) {}


/****************************************/
/****************************************/

void CTestEvolution::Evolve( void ) {
  StartEvaluationProcess();

  while( m_unNumGeneration < m_unMaxGeneration ) {
    SetRandomSeed( m_unRandomSeed );
    CGenotype* individual = m_pcPopulation->FirstIndividual();
    while( individual != NULL ) {
      EvaluateIndividual( individual );
      bool new_best = m_pcFitnessLog->AddFitnessData( m_unNumGeneration, individual->GetFitness() );
      if( new_best ) {
	if( m_pcBestGenotype ) delete m_pcBestGenotype;
	m_pcBestGenotype = individual->Clone();
      }
      individual = m_pcPopulation->NextIndividual();
    }
    
    LOG << "[INFO] Generation " << m_unNumGeneration
	<< " max fitness: " << m_pcFitnessLog->GetCurrentBestFitness()
	<< " avg fitness: " << m_pcFitnessLog->GetCurrentAverageFitness() << endl;
    LOG.Flush();
    WriteAll();

    m_pcPopulation->Reproduction( m_bMaximization );
    m_pcPopulation->Selection( m_bMaximization );

    m_unRandomSeed = m_pcRNG->Uniform( CRange<UInt32>(0, UINT_MAX) );
    m_unNumGeneration += 1;
  }
}



/****************************************/
/****************************************/

void CTestEvolution::StopEvaluationProcess( void ) {

}


/****************************************/
/****************************************/

void CTestEvolution::EvaluateIndividual( CGenotype* pc_genotype ) {
  if( pc_genotype->IsEvaluated() ) {
    return;
  }

  UInt32 length = pc_genotype->GetNumValues();
  const Real* values = pc_genotype->GetGeneValues();
  if( values == NULL ) {
    pc_genotype->SetFitness( m_bMaximization ? 0.0 : 1.0 );
    return;
  }

  Real fitness = 0;
  for( UInt32 i = 0; i < length; i++ ) {
    fitness += values[i];
  }
  
  fitness = fitness/length;
  pc_genotype->SetFitness( fitness );
  pc_genotype->SetEvaluated( true );
}



/****************************************/
/****************************************/

void CTestEvolution::Write( ostream& os ) {
  os << "<?xml version=\"1.0\" ?>" << endl;
  os << "<revolver-configuration>" << endl;
  os << "<" << CONFIGURATION_EVOLUTION_EVOLUTION << endl;
  os << "\t" << CONFIGURATION_EVOLUTION_TYPE << " = \"" << CONFIGURATION_TESTEVOLUTION_TYPE << "\"" << endl;
  CEvolution::Write( os );
  os << "\t>" << endl;

  if( m_pcPopulation != NULL ) {
    m_pcPopulation->Write( os );
  }
  os << "</" << CONFIGURATION_EVOLUTION_EVOLUTION << ">" << endl;
  os << "</revolver-configuration>" << endl;
}



/****************************************/
/****************************************/

void CTestEvolution::WriteAll( void ) {
  ofstream out;
  ostringstream filename;
  filename.fill( '0' );
  
  // Write the population
  filename.str("");
  filename << m_sWorkingDir << m_sEvolutionBaseFilename << setw(4) << m_unNumGeneration << ".log";
  out.open( filename.str().c_str(), ios::out );
  Write( out );
  out.close();

  // write the best individual
  filename.str("");
  filename << m_sWorkingDir << m_sBestIndividualBaseFilename << setw(4) << m_unNumGeneration << ".log";
  out.open( filename.str().c_str(), ios::out );
  const UInt32 num_values = m_pcBestGenotype->GetNumValues();
  const Real* values = m_pcBestGenotype->GetGeneValues();
  for( UInt32 i = 0; i < num_values; i++ ) {
    out << values[i] << " ";
  }
  out << endl;
  out.close();


  // write the fitness value
  filename.str("");
  filename << m_sWorkingDir << m_sFitnessFilename;
  out.open( filename.str().c_str(), ios::out );
  m_pcFitnessLog->Write( out );
  out.close();
}


/****************************************/
/****************************************/

void CTestEvolution::WriteFitnessLogs( void ) {

}


/****************************************/
/****************************************/

void CTestEvolution::WriteBestIndividuals( UInt32 un_num_best ) {

}


