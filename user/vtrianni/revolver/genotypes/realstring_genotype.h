/* -*- Mode:C++ -*- */

#ifndef REALSTRING_H
#define REALSTRING_H

#include <argos2/user/vtrianni/revolver/genotype.h>

namespace argos {

  class CRealStringGenotype: public CGenotype {
  private:
    Real* m_pfGenes;
  
    Real  m_fVariance;
    Real  m_fMaxValue;
    Real  m_fMinValue;

  
  public:
    CRealStringGenotype( UInt32 un_length );
    CRealStringGenotype( const CRealStringGenotype& s );
    ~CRealStringGenotype();
  
    virtual void Init( TConfigurationNode& t_configuration_tree );
    virtual void InitGenes( TConfigurationNode& t_configuration_tree );
    
    inline void  SetMutationVariance( Real f_value ) { m_fVariance = f_value; };
    inline void  SetMaxValue( Real f_value ) { m_fMaxValue = f_value; };
    inline void  SetMinValue( Real f_value ) { m_fMinValue = f_value; };


    CGenotype*   Clone( void );
    void         RandomInit( void );
    void         Mutation( Real probability, bool elitism = false );
    void         Crossover( const CGenotype* p1, const CGenotype* p2 );
  
    const Real*  GetGeneValues( void );
    const UInt32 GetNumValues( void ) const;
    const void*  GetGenes( void ) const;

    void         WriteGenotype( ostream& os );
    void         Write( ostream& os );

    bool         operator == ( const CGenotype& b );
  };

  const string CONFIGURATION_REALSTRING_TYPE = "realstring";

};

#endif
