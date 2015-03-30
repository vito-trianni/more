#include "mulambda_population.h"

using namespace argos;

const string CONFIGURATION_MULAMBDA_LAMBDA                = "lambda";
const string CONFIGURATION_MULAMBDA_ELITE_SIZE            = "elite_size";
const string CONFIGURATION_MULAMBDA_MUTATION_PROBABILITY  = "mutation_probability";
const string CONFIGURATION_MULAMBDA_CROSSOVER_PROBABILITY = "crossover_probability";

/****************************************/
/****************************************/

CMuLambdaPopulation::CMuLambdaPopulation() {
  m_unLambda = 0;
  m_unEliteSize = 0;
  m_fMutationProbability = 0.0;
  m_fCrossoverProbability = 0.0;
}


/****************************************/
/****************************************/

CMuLambdaPopulation::~CMuLambdaPopulation() {
  m_unLambda = 0;
  m_unEliteSize = 0;
  m_fMutationProbability = 0.0;
  m_fCrossoverProbability = 0.0;
}


/****************************************/
/****************************************/

void CMuLambdaPopulation::Init( TConfigurationNode& t_configuration_tree ) {
  // lambda
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_MULAMBDA_LAMBDA, m_unLambda );

  // elite size
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_MULAMBDA_ELITE_SIZE, m_unEliteSize );

  // mutation probability
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_MULAMBDA_MUTATION_PROBABILITY, m_fMutationProbability );

  // crossover probability
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_MULAMBDA_CROSSOVER_PROBABILITY, m_fCrossoverProbability );

  ////////////////////////////////////////////////////////////////////////////////
  // generic population configuration + genotype and individuals loading
  ////////////////////////////////////////////////////////////////////////////////
  CPopulation::Init( t_configuration_tree );


  ////////////////////////////////////////////////////////////////////////////////
  // check some loaded values
  ////////////////////////////////////////////////////////////////////////////////
  if( m_unLambda > m_unSize ) {
    THROW_ARGOSEXCEPTION("population size is smaller than selection size (lambda)");
  }

  if( m_unEliteSize > m_unLambda ) {
    THROW_ARGOSEXCEPTION("elite size is bigger than selection size (lambda)");
  }
}


/****************************************/
/****************************************/

void CMuLambdaPopulation::Write( ostream& os ) {
  os << "\t<"  << CONFIGURATION_EVOLUTION_POPULATION << endl;
  os << "\t\t" << CONFIGURATION_POPULATION_TYPE                << " = \"" << CONFIGURATION_MULAMBDA_TYPE << "\"" << endl;
  os << "\t\t" << CONFIGURATION_POPULATION_SIZE                << " = \"" << m_unSize                    << "\"" << endl;
  os << "\t\t" << CONFIGURATION_MULAMBDA_LAMBDA                << " = \"" << m_unLambda                  << "\"" << endl;
  os << "\t\t" << CONFIGURATION_MULAMBDA_ELITE_SIZE            << " = \"" << m_unEliteSize               << "\"" << endl;
  os << "\t\t" << CONFIGURATION_MULAMBDA_MUTATION_PROBABILITY  << " = \"" << m_fMutationProbability      << "\"" << endl;
  os << "\t\t" << CONFIGURATION_MULAMBDA_CROSSOVER_PROBABILITY << " = \"" << m_fCrossoverProbability     << "\"" << endl;
  os << "\t\t>" << endl;

  CPopulation::WritePopulation( os );

  os << "\t</"  << CONFIGURATION_EVOLUTION_POPULATION << ">" << endl;

}


/****************************************/
/****************************************/

void CMuLambdaPopulation::Selection( bool maximization ) {
  // select all offspring and discard the old individuals
  for( unsigned int i = 0; i < m_unSize; i++ ) {
    delete m_vecIndividuals[i];
    m_vecIndividuals[i] = m_vecOffspring[i];
  }
  m_vecOffspring.clear();
}


/****************************************/
/****************************************/

void CMuLambdaPopulation::Reproduction( bool b_maximization ) {
  // sort the population according to their fitness value
  Rank( b_maximization );

  // copy the elite into the offspring vector
  for( unsigned int i = 0; i < m_unEliteSize; i++ ) {
    CGenotype* pcClone = m_vecIndividuals[i]->Clone();
    pcClone->SetFitness( 0.0 );
    pcClone->SetEvaluated( false );

    m_vecOffspring.push_back( pcClone );
  }

  // generate the new offspring applying mutation (@todo: add crossover?)
  for( unsigned int i = 0; i < m_unSize - m_unEliteSize; i++ ) {
    int index = i%m_unLambda;
    CGenotype* pcClone = m_vecIndividuals[index]->Clone();
    pcClone->SetFitness( 0.0 );
    pcClone->SetEvaluated( false );
    pcClone->Mutation( m_fMutationProbability );    
    m_vecOffspring.push_back( pcClone );
  }
}
