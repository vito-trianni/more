/* -*- Mode:C++ -*- */

#ifndef MOMULAMBDAEVOLUTION_H
#define MOMULAMBDAEVOLUTION_H

#include "evolution.h"
#include <MOO-EALib/PopulationMOO.h>


using namespace std;
using namespace argos;

namespace more {
  
   enum TPostEvalType {POSTEVAL_SO, POSTEVAL_MO_RANK, POSTEVAL_MO_ND};

   class CMOMuLambdaEvolution : public CEvolution {
   private:
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
      PopulationMOO* m_pcPopulation;
      PopulationMOO* m_pcOffsprings;

      // number of objectives to optimise
      UInt32 m_unNumObjectives;

      // type of postevaluation to be performed
      TPostEvalType m_tPostEvalType;
      
   public:
      CMOMuLambdaEvolution();
      virtual ~CMOMuLambdaEvolution();
      
      // generic initialisation from the configuration tree, loaded from the xml file
      virtual void Init( TConfigurationNode& t_configuration_tree );
      
      // main evolutionary loop
      virtual void Evolve();

      // main loop for post evaluation
      virtual void PostEvaluation();

      // evaluate the population of genotypes
      virtual void EvaluatePopulation();

      // function for sending parameters to the fitness evaluation process
      virtual void SendIndividualParameters( UInt32 individualNumber );

      // function for receiveing fitness values from the fitness evaluation process
      virtual void ReceiveIndividualFitness();

      // functions for dumping into files
      virtual void DumpGeneration();
      virtual void DumpFitness();
      virtual void DumpBestIndividuals();
      virtual void DumpNonDominated();
      virtual void DumpIndividual( const UInt32& un_individual_number, const string& filename );
      virtual void DumpChromosome( const ChromosomeT<double>& chrom, const string& filename );

      // functions for loading from files
      inline virtual void LoadGeneration();
      inline virtual void LoadBestIndividuals();
   };
};

#endif
