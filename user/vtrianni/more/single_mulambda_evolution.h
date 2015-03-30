/* -*- Mode:C++ -*- */

#ifndef SINGLEMULAMBDAEVOLUTION_H
#define SINGLEMULAMBDAEVOLUTION_H

#include "evolution.h"
#include <EALib/PopulationT.h>

#include <argos2/simulator/argos_command_line_arg_parser.h>
#include <argos2/simulator/simulator.h>
#include <argos2/user/vtrianni/loop_functions/evolutionary_lf/evolutionary_lf.h>

using namespace std;
using namespace argos;

namespace more {
  
   class CSingleMuLambdaEvolution : public CEvolution {
   protected:
      // size of the evolving population
      UInt32 m_unPopulationSize;
      UInt32 m_unPopulationSizeMu;
      UInt32 m_unEliteSize;

      // size of the genotype
      UInt32 m_unGenotypeSize;
      
      // range of the genotype
      double m_fGenotypeMinValue;
      double m_fGenotypeMaxValue;

      // mutation variance
      double m_fMutationVariance;

      // the population of parents/offsprings
      PopulationT<double> *m_pcPopulation;
      PopulationT<double> *m_pcOffsprings;

      // a structure to store the fitness (and its components) for the whole population
      map< int, vector<double> > m_mapPopulationFitness;

      // number of components of the fitness 
      UInt32 m_unNumObjectives;

      // running the internal evaluation
      CSimulator* m_pcSimulator;
      UInt32*     m_punSampleSeeds;

   public:
      CSingleMuLambdaEvolution();
      virtual ~CSingleMuLambdaEvolution();
      
      // generic initialisation from the configuration tree, loaded from the xml file
      virtual void Init( TConfigurationNode& t_configuration_tree );
      
      // start the evaluation process with initialisation operations
      virtual void StartEvaluationProcess();

      // stop the evaluation process, with termination operations
      virtual void StopEvaluationProcess();

      // main evolutionary loop
      virtual void Evolve();

      // main loop for post evaluation
      virtual void PostEvaluation();

      // evaluate the population of genotypes
      virtual void EvaluatePopulation();


      // function for sending parameters to the fitness evaluation process
      virtual void SendIndividualParameters( UInt32 individualNumber ) {}

      // function for receiveing fitness values from the fitness evaluation process
      virtual void ReceiveIndividualFitness() {}

      // functions for dumping into files
      virtual void DumpGeneration();
      virtual void DumpFitness();
      virtual void DumpBestIndividuals();
      virtual void DumpIndividual( const UInt32& un_individual_number, const string& filename );

      // functions for loading from files
      inline virtual void LoadGeneration();
      inline virtual void LoadBestIndividuals();
   };
};

#endif
