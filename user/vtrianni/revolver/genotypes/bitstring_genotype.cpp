#include "bitstring_genotype.h"
#include <argos2/user/vtrianni/revolver/evolution.h>

using namespace argos;

const string CONFIGURATION_BITSTRING_BITPERVALUE   = "bitpervalue";
const string CONFIGURATION_BITSTRING_MAXVALUE      = "max_value";
const string CONFIGURATION_BITSTRING_MINVALUE      = "min_value";


/****************************************/
/****************************************/

CBitStringGenotype::CBitStringGenotype( UInt32 length ) {
  m_unLength = length;
  m_fFitness = 0.0;
  m_bEvaluated = false;

  m_pnGenes = new int[length];
  m_pfRealValuedGenes = NULL;
  m_unBitPerValue = 0;
  m_fMaxValue = 0.0;
  m_fMinValue = 0.0;
}


/****************************************/
/****************************************/

CBitStringGenotype::CBitStringGenotype( const CBitStringGenotype& s ) {
  m_unLength = s.m_unLength;
  m_fFitness = s.m_fFitness;
  m_bEvaluated = s.m_bEvaluated;

  m_pnGenes = new int[m_unLength];
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    m_pnGenes[i] = s.m_pnGenes[i];
  }

  m_pfRealValuedGenes = NULL;

  m_unBitPerValue = s.m_unBitPerValue;
  m_fMaxValue = s.m_fMaxValue;
  m_fMinValue = s.m_fMinValue;
}



/****************************************/
/****************************************/

CBitStringGenotype::~CBitStringGenotype() {
  if( m_pnGenes ) delete [] m_pnGenes;
  if( m_pfRealValuedGenes ) delete [] m_pfRealValuedGenes;
}


/****************************************/
/****************************************/

void CBitStringGenotype::Init( TConfigurationNode& t_configuration_tree ) {
  // number of bits per value
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_BITSTRING_BITPERVALUE, m_unBitPerValue );

  // max value
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_BITSTRING_MAXVALUE, m_fMaxValue );

  // min value
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_BITSTRING_MINVALUE, m_fMinValue );
}


/****************************************/
/****************************************/

void CBitStringGenotype::InitGenes( TConfigurationNode& t_configuration_tree ) {
  CGenotype::InitGenes(t_configuration_tree);

  string genes;
  GetNodeAttribute(t_configuration_tree, "genes", genes);
  istringstream input_genes(genes);
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    input_genes >> m_pnGenes[i];
  }
}


/****************************************/
/****************************************/

CGenotype* CBitStringGenotype::Clone( void ) {
  return (new CBitStringGenotype( *this ));
}



/****************************************/
/****************************************/

void CBitStringGenotype::RandomInit( void ) {
  for( UInt32 i = 0; i < m_unLength; i++ ) {
     m_pnGenes[i] = CEvolution::GetRNG()->Bernoulli() ? 1 : 0;
  }

  if( m_pfRealValuedGenes != NULL ) {
    delete [] m_pfRealValuedGenes;
    m_pfRealValuedGenes = NULL;
  }
}



/****************************************/
/****************************************/

void CBitStringGenotype::Mutation( Real probability, bool elitism ) {
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    if( CEvolution::GetRNG()->Uniform(CRange<Real>(0,1)) < probability ) {
      m_pnGenes[i] = 1 - m_pnGenes[i];
    }
  }

  if( m_pfRealValuedGenes != NULL ) {
    delete [] m_pfRealValuedGenes;
    m_pfRealValuedGenes = NULL;
  }
}



/****************************************/
/****************************************/

void CBitStringGenotype::Crossover( const CGenotype* p1, const CGenotype* p2 ) {
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    if( CEvolution::GetRNG()->Bernoulli() ) {
      m_pnGenes[i] = ((CBitStringGenotype*)p1)->m_pnGenes[i];
    }
    else {
      m_pnGenes[i] = ((CBitStringGenotype*)p2)->m_pnGenes[i];
    }
  }

  if( m_pfRealValuedGenes != NULL ) {
    delete [] m_pfRealValuedGenes;
    m_pfRealValuedGenes = NULL;
  }
}



/****************************************/
/****************************************/

const Real* CBitStringGenotype::GetGeneValues( void ) {
  if( m_pfRealValuedGenes != NULL )
    return m_pfRealValuedGenes;


  UInt32 num_values = m_unLength/m_unBitPerValue;
  m_pfRealValuedGenes = new Real[num_values];

  for( UInt32 i = 0; i < num_values; i++ ) {
    m_pfRealValuedGenes[i] = 0;
    UInt32 exp_factor = 1;
    for( UInt32 j = 0; j < m_unBitPerValue; j++ ) {
      m_pfRealValuedGenes[i]  += m_pnGenes[j+i*m_unBitPerValue]*exp_factor;
      exp_factor *= 2;
    }
    m_pfRealValuedGenes[i] = m_pfRealValuedGenes[i]/exp_factor * (m_fMaxValue-m_fMinValue) + m_fMinValue;
  }

  return m_pfRealValuedGenes;
}



/****************************************/
/****************************************/

const UInt32 CBitStringGenotype::GetNumValues( void ) const {
  return (m_unBitPerValue == 0) ? m_unLength : m_unLength/m_unBitPerValue;
}



/****************************************/
/****************************************/

const void* CBitStringGenotype::GetGenes( void ) const {
  return (void*) m_pnGenes;
}


/****************************************/
/****************************************/

void CBitStringGenotype::WriteGenotype( ostream& os ) {
  os << "<" << CONFIGURATION_GENOTYPE_INDIVIDUAL << " ";
  os << CONFIGURATION_GENOTYPE_FITNESS   << " = \"" << m_fFitness << "\" ";
  os << CONFIGURATION_GENOTYPE_EVALUATED << " = \"" << (m_bEvaluated ? "true" : "false") << "\" ";
  os << CONFIGURATION_GENOTYPE_GENES     << " = \"";
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    os << m_pnGenes[i] << " ";
  }
  os << "\" />";
  os << endl;
}



/****************************************/
/****************************************/

void CBitStringGenotype::Write( ostream& os ) {
  os << "\t\t\t" << CONFIGURATION_GENOTYPE_TYPE         << " = \"" << CONFIGURATION_BITSTRING_TYPE << "\"" << endl;
  os << "\t\t\t" << CONFIGURATION_GENOTYPE_LENGTH       << " = \"" << m_unLength                   << "\"" << endl;
  os << "\t\t\t" << CONFIGURATION_BITSTRING_BITPERVALUE << " = \"" << m_unBitPerValue              << "\"" << endl;
  os << "\t\t\t" << CONFIGURATION_BITSTRING_MINVALUE    << " = \"" << m_fMinValue                  << "\"" << endl;
  os << "\t\t\t" << CONFIGURATION_BITSTRING_MAXVALUE    << " = \"" << m_fMaxValue                  << "\"" << endl;
}
