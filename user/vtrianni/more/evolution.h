/* -*- Mode:C++ -*- */

#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <argos2/common/utility/configuration/base_configurable_resource.h>
#include <argos2/common/utility/datatypes/datatypes.h>

#include <fstream>
#include <sstream>
#include <iomanip>


#include <mpi.h>
#include <map>
#include <limits.h>

#include <Rng/GlobalRng.h>


using namespace std;
using namespace argos;

enum TEvoType {EVO_SO, EVO_MO, POST_SO, POST_MO, EVO_NONE, EVO_SO_SINGLE};


#ifdef ARGOS_DOUBLE_PRECISION
#define MPI_ARGOSREAL MPI_DOUBLE
#else
#define MPI_ARGOSREAL MPI_FLOAT
#endif



namespace more {
  
   typedef map<SInt32,SInt32> TMapProcessToIndividual;
   typedef map<SInt32,SInt32>::iterator TMapProcessToIndividualIterator;
   
   class CEvolution : public CBaseConfigurableResource {
   protected:
      // verbose flag
      bool m_bVerbose;
      
      // verbose flag
      bool m_bEvolutionFlag;
      
      // random seed of the evolutionary run
      UInt32 m_unRandomSeed;

      // maximisation/minimisation flax
      bool m_bMaximization;

      // counter and limits for evolutionary generations
      UInt32 m_unNumGeneration;
      UInt32 m_unMaxGeneration;
      UInt32 m_unSaveGenerationStep;
    
      // filenames and directories to store the evolutionary results
      string m_sWorkingDir;
      string m_sEvolutionBaseFilename;
      string m_sBestIndividualBaseFilename;
      string m_sFitnessBaseFilename;

      // random trials for the fitness evaluations
      UInt32 m_unNumSamples;
      UInt32 m_unCurrentSampleNumber;
      
      // variables for parallel environment
      UInt32 m_unNumProcesses;
      string m_sExperimentConfigurationFile;
      UInt64 m_unMsgTag;
      TMapProcessToIndividual m_mapProcessToIndividual;
      MPI::Intercomm m_cEvaluatorComm;


   public:
      CEvolution();
      virtual ~CEvolution();
      
      // generic initialisation from the configuration tree, loaded from the xml file
      virtual void Init( TConfigurationNode& t_configuration_tree );
      
      // start the evaluation process with initialisation operations
      virtual void StartEvaluationProcess();

      // stop the evaluation process, with termination operations
      virtual void StopEvaluationProcess();

      // main evolutionary loop
      virtual void Evolve() = 0;

      // main loop for post evaluation
      virtual void PostEvaluation() = 0;

      // evaluate the population of genotypes
      virtual void EvaluatePopulation() = 0;

      // function for sending parameters to the fitness evaluation process
      virtual void SendIndividualParameters( UInt32 individualNumber ) = 0;

      // function for receiveing fitness values from the fitness evaluation process
      virtual void ReceiveIndividualFitness() = 0;

      // functions for dumping into files
      virtual void DumpGeneration() = 0;
      virtual void DumpFitness() = 0;
      virtual void DumpBestIndividuals() = 0;

      // functions for loading from files
      virtual void LoadGeneration() = 0;
      virtual void LoadBestIndividuals() = 0;

      // destroy function
      inline virtual void Destroy() {};
      

      ////////////////////////////////////////////////////////////////////////////////
      // get/set functions
      ////////////////////////////////////////////////////////////////////////////////

      // get/set verbose flag
      inline const bool GetVerbose() { return m_bVerbose; };
      inline void SetVerbose( const bool& b_verbose ) { m_bVerbose = b_verbose; };
      
      // get/set post evaluation flag
      inline const bool GetEvolutionFlag() { return m_bEvolutionFlag; };
      inline void SetEvolutionFlag( const bool& b_evolution ) { m_bEvolutionFlag = b_evolution; };
      
      // get/set random seed
      inline const UInt32 GetRandomSeed() { return m_unRandomSeed; };
      inline void SetRandomSeed( const UInt32& un_seed ) { m_unRandomSeed = un_seed; Rng::seed(un_seed); };
      
      // get/set maximistaion flag
      inline const bool GetMaximization() { return m_bMaximization; };
      inline void SetMaximization( const bool& b_maximisation ) { m_bMaximization = b_maximisation; };
      
      // get/set generation number
      inline const UInt32 GetNumGeneration() { return m_unNumGeneration; };
      inline void SetNumGeneration( const UInt32& un_num_generation ) { m_unNumGeneration = un_num_generation; };
    
      // get/set max generation number
      inline const UInt32 GetMaxGeneration() { return m_unMaxGeneration; };
      inline void SetMaxGeneration( const UInt32& un_max_generation ) { m_unMaxGeneration = un_max_generation; };
    
      // get/set number of step of generations to jump beofre saving
      inline const UInt32 GetSaveGenerationStep() { return m_unSaveGenerationStep; };
      inline void SetSaveGenerationStep( const UInt32& un_save_generation_step ) { m_unSaveGenerationStep = un_save_generation_step; };

      // get/set working dir
      inline const string GetWorkingDir() { return m_sWorkingDir; };
      inline void SetWorkingDir( const string& dir ) { m_sWorkingDir = dir; };

      // get/set base filename for full generation dump
      inline const string GetEvolutionBaseFilename() { return m_sEvolutionBaseFilename; };
      inline void SetEvolutionBaseFilename( const string& base ) { m_sEvolutionBaseFilename = base; };

      // get/set base filename for the best individual/pareto front
      inline const string GetBestIndividualBaseFilename() { return m_sBestIndividualBaseFilename; };
      inline void SetBestIndividualBaseFilename( const string& base ) { m_sBestIndividualBaseFilename = base; };

      // get/set number of samples
      inline const UInt32 GetNumSamples() { return m_unNumSamples; };
      inline void SetNumSamples( const UInt32& un_samples ) { m_unNumSamples = un_samples; };

      // get/set current sample number
      inline const UInt32 GetCurrentSampleNumber() { return m_unCurrentSampleNumber; };
      inline void SetCurrentSampleNumber( const UInt32& un_current ) { m_unCurrentSampleNumber = un_current; };

      // get/set number of parallel processes
      inline const UInt32 GetNumProcesses() { return m_unNumProcesses; };
      inline void SetNumProcesses( const UInt32& un_num_processes ) { m_unNumProcesses = un_num_processes; };

      // get/set experiment configuration file
      inline const string GetExperimentConfigurationFile() { return m_sExperimentConfigurationFile; };
      inline void SetExperimentConfigurationFile( const string& s_config ) { m_sExperimentConfigurationFile = s_config; };

   };
};

#endif
