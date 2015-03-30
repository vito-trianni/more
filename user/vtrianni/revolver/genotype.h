/* -*- Mode:C++ -*- */

#ifndef GENOTYPE_H
#define GENOTYPE_H

#include <argos2/common/utility/configuration/base_configurable_resource.h>
#include <argos2/common/utility/datatypes/datatypes.h>
#include <argos2/common/utility/argos_random.h>

#include <argos2/common/utility/logging/argos_log.h>


using namespace std;

namespace argos {

  class CGenotype : public CBaseConfigurableResource {
  protected:
    UInt32                 m_unLength;
    Real                   m_fFitness;
    bool                   m_bEvaluated;
    
  public:
    virtual ~CGenotype();
    
    virtual void           Init( TConfigurationNode& t_configuration_tree ) = 0;
    virtual void           InitGenes( TConfigurationNode& t_configuration_tree );

    virtual void           Destroy( void ) {};

    inline const UInt32    GetLength( void ) const { return m_unLength; };
    inline void            SetLength( const UInt32 un_length ) { m_unLength = un_length; };

    inline const Real      GetFitness( void ) const { return m_fFitness; };
    inline void            SetFitness( const Real f_fitness ) { m_fFitness = f_fitness; };

    inline bool            IsEvaluated( void ) { return m_bEvaluated; };
    inline void            SetEvaluated( const bool b_eval ) { m_bEvaluated = b_eval; };
  
    virtual CGenotype*     Clone( void ) = 0;
    virtual void           RandomInit( void ) = 0;
    virtual void           Mutation( Real probability, bool elitism = false ) = 0;
    virtual void           Crossover( const CGenotype* p1, const CGenotype* p2 ) = 0;

    virtual const Real*    GetGeneValues( void ) = 0;
    virtual const UInt32   GetNumValues( void ) const = 0;
    virtual const void*    GetGenes( void ) const = 0;

    virtual void           WriteGenotype( ostream& os ) = 0;
    virtual void           Write( ostream& os ) = 0;
  };



  struct gtgenotype : public binary_function<CGenotype*, CGenotype*, bool> {
    bool operator()(CGenotype* x, CGenotype* y) { return (x->GetFitness() > y->GetFitness()); }
  };



  struct ltgenotype : public binary_function<CGenotype*, CGenotype*, bool> {
    bool operator()(CGenotype* x, CGenotype* y) { return (x->GetFitness() < y->GetFitness()); }
  };



  typedef vector<CGenotype*> TGenotypeVec;
  typedef vector<CGenotype*>::iterator TGenotypeIterator;


  const string CONFIGURATION_GENOTYPE_TYPE          = "type";
  const string CONFIGURATION_GENOTYPE_LENGTH        = "length";
  const string CONFIGURATION_GENOTYPE_INDIVIDUAL    = "individual";
  const string CONFIGURATION_GENOTYPE_FITNESS       = "fitness";
  const string CONFIGURATION_GENOTYPE_EVALUATED     = "eval";
  const string CONFIGURATION_GENOTYPE_GENES         = "genes";

};

#endif
