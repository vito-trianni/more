/* -*- Mode:C++ -*- */

#ifndef MULAMBDAEVOLUTION_H
#define MULAMBDAEVOLUTION_H

#include "evolution.h"
#include <EALib/PopulationT.h>


using namespace std;
using namespace argos;

namespace more {
  
   class CMuLambdaEvolution : public CEvolution {
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

   public:
      CMuLambdaEvolution();
      virtual ~CMuLambdaEvolution();
      
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
      virtual void DumpIndividual( const UInt32& un_individual_number, const string& filename );

      // functions for loading from files
      inline virtual void LoadGeneration();
      inline virtual void LoadBestIndividuals();
   };
};

#endif
