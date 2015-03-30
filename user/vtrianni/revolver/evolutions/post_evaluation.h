/* -*- Mode:C++ -*- */

#ifndef POSTEVALUATION_H
#define POSTEVALUATION_H

#include <argos2/user/vtrianni/revolver/evolution.h>
#include <argos2/user/vtrianni/revolver/population.h>
#include <argos2/user/vtrianni/revolver/genotype.h>
#include <argos2/user/vtrianni/revolver/fitness_log.h>

#include <argos2/simulator/dynamic_linking/loop_functions.h>

namespace argos {

  class CPostEvaluation: public CEvolution {
  protected:
    UInt32                m_unNumBestGenotypes;
    
    UInt32                m_unRankBestIndividual;
    CGenotype*            m_pcBestGenotype;
    CFitnessLog*          m_pcBestGenotypeLog;
    CFitnessData*         m_pcCurrentBestFitness;

    vector<CGenotype*>    m_vecBestGenotypes;
    vector<CFitnessLog*>  m_vecBestGenotypesLog;
  
    UInt32                m_unNumTrials;
    UInt32                m_unCurrentTrialNumber;
    UInt32*               m_punTrialSeeds;
  
    string                m_sExperimentConfig;

    CPopulation*          m_pcPopulation;
    CSimulator&           m_cSimulator;

  public:
    CPostEvaluation();
    ~CPostEvaluation();

    inline const UInt32 GetNumTrials( void ) { return m_unNumTrials; };
    inline void SetNumTrials( const UInt32 un_trials ) { m_unNumTrials = un_trials; };
    inline const UInt32 GetCurrentTrialNumber( void ) { return m_unCurrentTrialNumber; };

    virtual void Init( TConfigurationNode& t_configuration_tree );
    virtual void InitPopulation( TConfigurationNode& t_configuration_tree );

    virtual void StartEvaluationProcess( void );
    virtual void Evolve( void );
    virtual void StopEvaluationProcess( void );

    virtual void Write( ostream& os );
    virtual void WriteAll( void );
    virtual void WriteFitnessLogs( void );
    virtual void WriteBestIndividuals( UInt32 un_num_best );
  
  private:
    Real EvaluateIndividual( CGenotype* pc_genotype, CFitnessLog* pc_fitness_log );
  };


  const string CONFIGURATION_POSTEVALUATION_TYPE = "post-evaluation";

};

#endif
