#include "steadystate_population.h"
#include "string_utilities.h"

const string CONFIGURATION_STEADYSTATE_TOURNAMENT_SIZE       = "tournament_size";
const string CONFIGURATION_STEADYSTATE_MUTATION_PROBABILITY  = "mutation_probability";
const string CONFIGURATION_STEADYSTATE_CROSSOVER_PROBABILITY = "crossover_probability";

/****************************************/
/****************************************/

CSteadyStatePopulation::CSteadyStatePopulation() {
  m_unTournamentSize = 0;
  m_fMutationProbability = 0.0;
  m_fCrossoverProbability = 0.0;
}


/****************************************/
/****************************************/

CSteadyStatePopulation::~CSteadyStatePopulation() {
  m_unTournamentSize = 0;
  m_fMutationProbability = 0.0;
  m_fCrossoverProbability = 0.0;
}


/****************************************/
/****************************************/

int CSteadyStatePopulation::Init( const TConfigurationTree& t_configuration_tree ) {
  ////////////////////////////////////////////////////////////////////////////////
  // tournament size
  ////////////////////////////////////////////////////////////////////////////////
  string s_tournament_size = CExperimentConfiguration::GetNodeValue( t_configuration_tree, CONFIGURATION_STEADYSTATE_TOURNAMENT_SIZE );
  if( s_tournament_size == "" ) {
    CSwarmanoidLogger::LogErr() << "[WARNING] No <" << CONFIGURATION_STEADYSTATE_TOURNAMENT_SIZE
				<< "> passed for population initialisation" << endl;
    return -1;
  }

  int n_tournament_size = 0;
  if( StringToInt( s_tournament_size, n_tournament_size ) != 0 ) {
    CSwarmanoidLogger::LogErr() << "[FATAL] Wrong value "  << s_tournament_size
				<< " in <" << CONFIGURATION_STEADYSTATE_TOURNAMENT_SIZE
				<< "> tag passed for population initialisation" << endl;
    return -1;
  }
  m_unTournamentSize = (unsigned int) n_tournament_size;


  ////////////////////////////////////////////////////////////////////////////////
  // mutation probability
  ////////////////////////////////////////////////////////////////////////////////
  string s_mutation_probability = CExperimentConfiguration::GetNodeValue( t_configuration_tree, CONFIGURATION_STEADYSTATE_MUTATION_PROBABILITY );
  if( s_mutation_probability == "" ) {
    CSwarmanoidLogger::LogErr() << "[WARNING] No <" << CONFIGURATION_STEADYSTATE_MUTATION_PROBABILITY
				<< "> passed for population initialisation" << endl;
    return -1;
  }

  if( StringToDouble( s_mutation_probability, m_fMutationProbability ) != 0 ) {
    CSwarmanoidLogger::LogErr() << "[FATAL] Wrong value "  << s_mutation_probability
				<< " in <" << CONFIGURATION_STEADYSTATE_MUTATION_PROBABILITY
				<< "> tag passed for population initialisation" << endl;
    return -1;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // crossover probability
  ////////////////////////////////////////////////////////////////////////////////
  string s_crossover_probability = CExperimentConfiguration::GetNodeValue( t_configuration_tree, CONFIGURATION_STEADYSTATE_CROSSOVER_PROBABILITY );
  if( s_crossover_probability == "" ) {
    CSwarmanoidLogger::LogErr() << "[WARNING] No <" << CONFIGURATION_STEADYSTATE_CROSSOVER_PROBABILITY
				<< "> passed for population initialisation" << endl;
    return -1;
  }

  if( StringToDouble( s_crossover_probability, m_fCrossoverProbability ) != 0 ) {
    CSwarmanoidLogger::LogErr() << "[FATAL] Wrong value "  << s_crossover_probability
				<< " in <" << CONFIGURATION_STEADYSTATE_CROSSOVER_PROBABILITY
				<< "> tag passed for population initialisation" << endl;
    return -1;
  }


  ////////////////////////////////////////////////////////////////////////////////
  // generic population configuration + genotype and individuals loading
  ////////////////////////////////////////////////////////////////////////////////
  if( CPopulation::Init( t_configuration_tree ) != 0 ) return -1;


  ////////////////////////////////////////////////////////////////////////////////
  // check some loaded values
  ////////////////////////////////////////////////////////////////////////////////
  //   if( m_unTournamentSize < 2 ) {
  //     cout << "[FATAL] tournament size is too small" << endl;
  //     return -1;
  //   }
  

  return 0;
}


/****************************************/
/****************************************/

void CSteadyStatePopulation::Write( ostream& os ) {
  os << "\t<" << CONFIGURATION_POPULATION_TYPE << ">" << CONFIGURATION_STEADYSTATE_TYPE << "</" << CONFIGURATION_POPULATION_TYPE << ">" << endl;
  os << "\t<" << CONFIGURATION_POPULATION_SIZE << ">" << m_unSize << "</" << CONFIGURATION_POPULATION_SIZE << ">" << endl;

  os << "\t<" << CONFIGURATION_STEADYSTATE_TOURNAMENT_SIZE       << ">" << m_unTournamentSize      << "</" << CONFIGURATION_STEADYSTATE_TOURNAMENT_SIZE       << ">" << endl;
  os << "\t<" << CONFIGURATION_STEADYSTATE_MUTATION_PROBABILITY  << ">" << m_fMutationProbability  << "</" << CONFIGURATION_STEADYSTATE_MUTATION_PROBABILITY  << ">" << endl;
  os << "\t<" << CONFIGURATION_STEADYSTATE_CROSSOVER_PROBABILITY << ">" << m_fCrossoverProbability << "</" << CONFIGURATION_STEADYSTATE_CROSSOVER_PROBABILITY << ">" << endl;

  CPopulation::WritePopulation( os );
}


/****************************************/
/****************************************/

void CSteadyStatePopulation::Selection( bool maximization ) {
  // TODO...
}


/****************************************/
/****************************************/

void CSteadyStatePopulation::Reproduction( bool b_maximization ) {
  // TODO...
}
