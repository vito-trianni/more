/* -*- Mode:C++ -*- */

#ifndef STEADYSTATEPOPULATION_H
#define STEADYSTATEPOPULATION_H

#include "experiment_configuration.h"

using namespace std;
using namespace swarmanoid;

#include "revolver/population.h"

class CSteadyStatePopulation: public CPopulation {
private:
  unsigned int m_unTournamentSize;
  double m_fMutationProbability;
  double m_fCrossoverProbability;

  
public:
  CSteadyStatePopulation();
  ~CSteadyStatePopulation();

  virtual int Init( const TConfigurationTree& t_configuration_tree );

  inline const unsigned int GetTournamentSize( void ) { return m_unTournamentSize; };
  inline void SetTournamentSize( const unsigned int un_tournament_size ) { m_unTournamentSize = un_tournament_size; };

  inline const double GetMutationProbability( void ) { return m_fMutationProbability; };
  inline void SetMutationProbability( const double f_mutation_probability ) { m_fMutationProbability = f_mutation_probability; };

  inline const double GetCrossoverProbability( void ) { return m_fCrossoverProbability; };
  inline void SetCrossoverProbability( const double f_crossover_probability ) { m_fCrossoverProbability = f_crossover_probability; };
  
  void Selection( bool maximization );
  void Reproduction( bool maximization );
  
  void Write( ostream& os );
};

const string CONFIGURATION_STEADYSTATE_TYPE = "steadystate";

#endif
