/* -*- Mode:C++ -*- */

#ifndef PARALLELEVOLUTION_H
#define PARALLELEVOLUTION_H

using namespace std;

#include <argos2/user/vtrianni/revolver/pvm_compatibility.h>

#include <mpi.h>

#include <argos2/user/vtrianni/revolver/evolution.h>
#include <argos2/user/vtrianni/revolver/population.h>
#include <argos2/user/vtrianni/revolver/genotype.h>
#include <argos2/user/vtrianni/revolver/fitness_log.h>

namespace argos {

   typedef map<SInt32,SInt32> TMapProcToInd;
   typedef map<SInt32,SInt32>::iterator TMapProcToIndIterator;

   enum TPEType {PE_PVM, PE_MPI};

   class CParallelEvolution: public CEvolution {
   protected:
      CPopulation*    m_pcPopulation;
      CFitnessLog*    m_pcFitnessLog;
      CGenotype*      m_pcBestGenotype;

      UInt32          m_unNumSamples;
      UInt32          m_unCurrentSampleNumber;
      UInt32*         m_punSampleSeeds;

      UInt32          m_unNumProcesses;
      TPEType         m_tParallelEnvironmentType;
      TMapProcToInd   m_mapProcessToIndividual;
      UInt64          m_unMsgTag;
  
      string          m_sExperimentConfigurationFile;
      
      MPI::Intercomm  m_cEvaluatorComm;

   public:
      CParallelEvolution();
      ~CParallelEvolution();
  
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

   protected:
      void         EvaluatePopulation( void );

      void         SendIndividualParameters( UInt32 individualNumber, CGenotype* individual );
      void         ReceiveIndividualFitness( void );

      void         SpawnParallelProcesses( string command, vector<string> &args );
      void         StopParallelProcess( SInt32 n_tid );
      void         TerminateParallelEnvironment( void );

   };


   const string CONFIGURATION_PARALLELEVOLUTION_TYPE = "parallel-evolution";
};


#endif
