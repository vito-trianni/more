/* -*- Mode:C++ -*- */

#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <fstream>
#include <unistd.h>

#include <argos2/common/utility/configuration/base_configurable_resource.h>
#include <argos2/simulator/simulator.h>
#include <argos2/simulator/space/space.h>
#include <argos2/common/utility/datatypes/color.h>
#include <argos2/common/utility/math/vector2.h>
#include <argos2/common/utility/math/vector3.h>
#include <argos2/common/utility/math/quaternion.h>
#include <argos2/common/utility/argos_random.h>

using namespace std;

#include "genotype.h"
#include "population.h"


namespace argos {
  
  // Fitness data helper class
  class CFitnessData {
  public:
    UInt32        generation;
    Real          fitness;
    vector<Real>  components;
    
    CFitnessData( UInt32 gen, bool max ) { generation = gen; fitness = (max ? 0.0 : 1e38); };
  };
  
  typedef vector<CFitnessData*> TVecFitnessData;
  typedef vector<CFitnessData*>::iterator TVecFitnessDataIterator;
  
  


  class CEvolution : public CBaseConfigurableResource {
  protected:
    UInt32 m_unNumGeneration;
    UInt32 m_unMaxGeneration;
    UInt32 m_unSaveGenerationStep;
    
    bool   m_bMaximization;
    UInt32 m_unVerbose;
    
    string m_sWorkingDir;
    string m_sEvolutionBaseFilename;
    string m_sBestIndividualBaseFilename;
    string m_sFitnessFilename;

    UInt32              m_unRandomSeed;
    static CARGoSRandom::CRNG* m_pcRNG;

    static CEvolution* m_pcEvolutionInstance;

  public:
    CEvolution();
    virtual ~CEvolution();
    
    static CEvolution*  GetInstance();

    virtual void        Init( TConfigurationNode& t_configuration_tree );
    virtual void        InitPopulation( TConfigurationNode& t_configuration_tree ) = 0;
    virtual void        InitRandom( UInt32 un_seed );
    inline  void        SetRandomSeed( UInt32 un_seed ) {m_pcRNG->SetSeed(un_seed);}

    static CARGoSRandom::CRNG* GetRNG( void ) {return m_pcRNG;}

    virtual void        Destroy( void ) {};
    
    inline const UInt32 GetNumGeneration( void ) { return m_unNumGeneration; };
    inline void         SetNumGeneration( const UInt32 un_num_generation ) { m_unNumGeneration = un_num_generation; };
    
    inline const UInt32 GetMaxGeneration( void ) { return m_unMaxGeneration; };
    inline void         SetMaxGeneration( const int un_max_generation ) { m_unMaxGeneration = un_max_generation; };
    
    inline const UInt32 GetSaveGenerationStep( void ) { return m_unSaveGenerationStep; };
    inline void         SetSaveGenerationStep( const int un_save_generation_step ) { m_unSaveGenerationStep = un_save_generation_step; };
    
    inline void         SetMaximization( const bool b_maximisation ) { m_bMaximization = b_maximisation; };
    inline void         SetVerbose( const UInt32 un_verbose ) { m_unVerbose = un_verbose; };
    //     inline void         SetEvolutionSeed( const unsigned long int seed ) { CARGoSRandom::InitRandom( seed ); };
    
    inline string       GetWorkingDir( void ) { return m_sWorkingDir; };
    inline void         SetWorkingDir( const string dir ) { m_sWorkingDir = dir; };
    inline void         SetEvolutionBaseFilename( const string base ) { m_sEvolutionBaseFilename = base; };
    inline void         SetBestIndividualBaseFilename( const string base ) { m_sBestIndividualBaseFilename= base; };
    //     inline void         SetRandomSeed( unsigned long int un_seed ) { m_unRandomSeed = un_seed; };

    virtual void        StartEvaluationProcess( void ) = 0;
    virtual void        Evolve( void ) = 0;
    virtual void        StopEvaluationProcess( void ) = 0;

    virtual void        Write( ostream& os );
    virtual void        WriteAll( void ) = 0;
    virtual void        WriteFitnessLogs( void ) = 0;
    virtual void        WriteBestIndividuals( UInt32 un_num_best ) = 0;

  };


  const string  CONFIGURATION_EVOLUTION_TYPE         = "type";
  const string  CONFIGURATION_EVOLUTION_EVOLUTION    = "evolution";
  const string  RNG_EVOLUTION                        = "evolution";

};

#endif
