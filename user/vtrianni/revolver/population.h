/* -*- Mode:C++ -*- */

#ifndef POPULATION_H
#define POPULATION_H

#include <argos2/common/utility/configuration/base_configurable_resource.h>
#include "genotype.h"

using namespace std;


/**
 * Abstract class implementing a population of genotypes.
 */
namespace argos {

  class CPopulation : public CBaseConfigurableResource {
  protected:
    UInt32                     m_unSize;
    
    CGenotype*                 m_pcSampleGenotype;

    TGenotypeVec               m_vecIndividuals;
    TGenotypeIterator          m_itIndividuals;

    TGenotypeVec               m_vecOffspring;
    TGenotypeIterator          m_itOffspring;

  public:
    CPopulation();
    virtual ~CPopulation();
  
    virtual void               Init( TConfigurationNode& t_configuration_tree );
    virtual void               InitGenotype( TConfigurationNode& t_configuration_tree );
    virtual void               InitIndividuals( TConfigurationNode& t_configuration_tree );
    virtual void               InitIndividuals( const string s_filename );

    virtual void               Destroy( void ) {};
    
    inline UInt32              GetSize( void ) { return m_unSize; };
    inline void                SetSize( UInt32 un_size );
  
    inline CGenotype*          GetSampleIndividual( void ) {return m_pcSampleGenotype;};
    inline CGenotype*          FirstIndividual( void ) {if( (m_itIndividuals = m_vecIndividuals.begin()) != m_vecIndividuals.end() ) return *m_itIndividuals; return NULL; };
    inline CGenotype*          NextIndividual( void ) {if( ++m_itIndividuals != m_vecIndividuals.end() ) return *m_itIndividuals; return NULL; };
    inline CGenotype*          FirstOffspring( void ) {if( (m_itOffspring = m_vecOffspring.begin()) != m_vecOffspring.end() ) return *m_itOffspring; return NULL; };
    inline CGenotype*          NextOffspring( void ) {if( ++m_itOffspring != m_vecOffspring.end() ) return *m_itOffspring; return NULL; };

    inline TGenotypeVec&       GetIndividuals( void ) { return m_vecIndividuals; };
    inline CGenotype*          GetIndividual( UInt32 i ) { return (m_vecIndividuals.size() > i) ? m_vecIndividuals[i] : NULL; };

    virtual void               Rank( bool b_maximisation );


    inline const TGenotypeVec* GetOffspring( void ) {return &m_vecOffspring; };
    inline CGenotype*          GetOffspring( UInt32 i ) { return (m_vecOffspring.size() > i) ? m_vecIndividuals[i] : NULL; };

    virtual void               Selection( bool maximization ) = 0;
    virtual void               Reproduction( bool maximization ) = 0;

    virtual void               Write( ostream& os ) = 0;
    virtual void               WritePopulation( ostream& os );
  };

  const string CONFIGURATION_POPULATION_TYPE        = "type";
  const string CONFIGURATION_POPULATION_SIZE        = "size";
  const string CONFIGURATION_EVOLUTION_POPULATION   = "population";

};


#endif


