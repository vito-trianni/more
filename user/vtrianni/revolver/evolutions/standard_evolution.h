/* -*- Mode:C++ -*- */

#ifndef STANDARDEVOLUTION_H
#define STANDARDEVOLUTION_H

using namespace std;

#include <argos2/user/vtrianni/revolver/evolution.h>
#include <argos2/user/vtrianni/revolver/population.h>
#include <argos2/user/vtrianni/revolver/genotype.h>
#include <argos2/user/vtrianni/revolver/fitness_log.h>

#include <argos2/simulator/dynamic_linking/loop_functions.h>


namespace argos {

  class CStandardEvolution: public CEvolution {
  protected:
    CPopulation*    m_pcPopulation;
    CFitnessLog*    m_pcFitnessLog;
    CGenotype*      m_pcBestGenotype;

    UInt32          m_unNumSamples;
    UInt32          m_unCurrentSampleNumber;
    UInt32*         m_punSampleSeeds;
  
    string          m_sExperimentConfigurationFile;
    CSimulator&     m_cSimulator;
    // CLoopFunctions  m_cLoopFunctions;

  public:
    CStandardEvolution();
    ~CStandardEvolution();
  
    inline const UInt32 GetNumSamples( void ) { return m_unNumSamples; };
    inline void SetNumSamples( const UInt32 un_samples ) { m_unNumSamples = un_samples; };
    inline const UInt32 GetCurrentSampleNumber( void ) { return m_unCurrentSampleNumber; };

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
    void EvaluateIndividual( CGenotype* pc_genotype );
  };


  const string CONFIGURATION_STANDARDEVOLUTION_TYPE = "standard-evolution";
};


#endif
