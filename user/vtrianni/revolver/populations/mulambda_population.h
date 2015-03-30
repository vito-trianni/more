/* -*- Mode:C++ -*- */

#ifndef MULAMBDAPOPULATION_H
#define MULAMBDAPOPULATION_H

using namespace std;

#include <argos2/user/vtrianni/revolver/population.h>

namespace argos {
  
  class CMuLambdaPopulation: public CPopulation {
  private:
    UInt32 m_unLambda;
    UInt32 m_unEliteSize;
    Real   m_fMutationProbability;
    Real   m_fCrossoverProbability;
  
  public:
    CMuLambdaPopulation();
    ~CMuLambdaPopulation();

    virtual void Init( TConfigurationNode& t_configuration_tree );

    inline const UInt32 GetLambda( void ) { return m_unLambda; };
    inline void SetLambda( const UInt32 un_lambda ) { m_unLambda = un_lambda; };
    
    inline const UInt32 GetEliteSize( void ) { return m_unEliteSize; };
    inline void SetEliteSize( const UInt32 un_elite_size ) { m_unEliteSize = un_elite_size; };

    inline const Real GetMutationProbability( void ) { return m_fMutationProbability; };
    inline void SetMutationProbability( const Real f_mutation_probability ) { m_fMutationProbability = f_mutation_probability; };

    inline const Real GetCrossoverProbability( void ) { return m_fCrossoverProbability; };
    inline void SetCrossoverProbability( const Real f_crossover_probability ) { m_fCrossoverProbability = f_crossover_probability; };
  
    void Selection( bool maximization );
    void Reproduction( bool maximization );
  
    void Write( ostream& os );
  };

  const string CONFIGURATION_MULAMBDA_TYPE = "mulambda";

};

#endif
