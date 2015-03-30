/* -*- Mode:C++ -*- */

#ifndef TESTEVOLUTION_H
#define TESTEVOLUTION_H

using namespace std;

#include <argos2/user/vtrianni/revolver/evolution.h>
#include <argos2/user/vtrianni/revolver/population.h>
#include <argos2/user/vtrianni/revolver/genotype.h>
#include <argos2/user/vtrianni/revolver/fitness_log.h>

namespace argos {

  class CTestEvolution: public CEvolution {
  private:
    CPopulation* m_pcPopulation;
    CFitnessLog* m_pcFitnessLog;
    CGenotype*   m_pcBestGenotype;
  
  public:
    CTestEvolution();
    ~CTestEvolution();
  
    virtual void Init( TConfigurationNode& t_configuration_tree );
    virtual void InitPopulation( TConfigurationNode& t_configuration_tree );

    virtual void StartEvaluationProcess( void ) ;
    virtual void Evolve( void );
    virtual void StopEvaluationProcess( void ) ;
  
    virtual void Write( ostream& os );
    virtual void WriteAll( void );
    virtual void WriteFitnessLogs( void );
    virtual void WriteBestIndividuals( UInt32 un_num_best );
  
  private:
    void EvaluateIndividual( CGenotype* individual );
  };

  const string CONFIGURATION_TESTEVOLUTION_TYPE = "test-evolution";

};

#endif

