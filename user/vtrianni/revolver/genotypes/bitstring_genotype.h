/* -*- Mode:C++ -*- */

#ifndef BITSTRING_H
#define BITSTRING_H

#include <argos2/user/vtrianni/revolver/genotype.h>

namespace argos {

  class CBitStringGenotype: public CGenotype {
  private:
    int*    m_pnGenes;
    Real*   m_pfRealValuedGenes;

    UInt32  m_unBitPerValue;
    Real    m_fMaxValue;
    Real    m_fMinValue;

  public:
    CBitStringGenotype( UInt32 un_length );
    CBitStringGenotype( const CBitStringGenotype& s );
    ~CBitStringGenotype();

    virtual void Init( TConfigurationNode& t_configuration_tree );
    virtual void InitGenes( TConfigurationNode& t_configuration_tree );

    inline void  SetBitPerValue( UInt32 un_bits ) { m_unBitPerValue = un_bits; };
    inline void  SetMaxValue( Real f_value ) { m_fMaxValue = f_value; };
    inline void  SetMinValue( Real f_value ) { m_fMinValue = f_value; };
  

    CGenotype* Clone( void );
    void RandomInit( void );
    void Mutation( Real probability, bool elitism = false );
    void Crossover( const CGenotype* p1, const CGenotype* p2 );
  
    const Real*  GetGeneValues( void );
    const UInt32 GetNumValues( void ) const;
    const void*  GetGenes( void ) const ;

    void WriteGenotype( ostream& os );
    void Write( ostream& os );

    bool operator == ( const CGenotype& b );
  };

  const string CONFIGURATION_BITSTRING_TYPE = "bitstring";

};

#endif
