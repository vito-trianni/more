#include "realstring_genotype.h"

#include <argos2/user/vtrianni/revolver/evolution.h>

using namespace argos;


const string CONFIGURATION_REALSTRING_VARIANCE = "variance";
const string CONFIGURATION_REALSTRING_MAXVALUE = "max_value";
const string CONFIGURATION_REALSTRING_MINVALUE = "min_value";


/****************************************/
/****************************************/

CRealStringGenotype::CRealStringGenotype( UInt32 length ) {
  m_unLength   = length;
  m_fFitness   = 0.0;
  m_bEvaluated = false;
  
  m_pfGenes    = new Real[length];
  m_fVariance  = 0.0;
  m_fMaxValue  = 0.0;
  m_fMinValue  = 0.0;
}


/****************************************/
/****************************************/

CRealStringGenotype::CRealStringGenotype( const CRealStringGenotype& s ) {
  m_unLength   = s.m_unLength;
  m_fFitness   = s.m_fFitness;
  m_bEvaluated = s.m_bEvaluated;
  m_pfGenes    = new Real[m_unLength];
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    m_pfGenes[i] = s.m_pfGenes[i];
  }

  m_fVariance = s.m_fVariance;
  m_fMaxValue = s.m_fMaxValue;
  m_fMinValue = s.m_fMinValue;
}



/****************************************/
/****************************************/

CRealStringGenotype::~CRealStringGenotype() {
  if( m_pfGenes != NULL ) delete [] m_pfGenes;
}


/****************************************/
/****************************************/

void CRealStringGenotype::Init( TConfigurationNode& t_configuration_tree ) {
  // mutation variance
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_REALSTRING_VARIANCE, m_fVariance );

  // max value
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_REALSTRING_MAXVALUE, m_fMaxValue );

  // min value
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_REALSTRING_MINVALUE, m_fMinValue );
}


/****************************************/
/****************************************/

void CRealStringGenotype::InitGenes( TConfigurationNode& t_configuration_tree ) {
  CGenotype::InitGenes(t_configuration_tree);

  string genes;
  GetNodeAttribute(t_configuration_tree, "genes", genes);
  istringstream input_genes(genes);
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    input_genes >> m_pfGenes[i];
  }
}


/****************************************/
/****************************************/

CGenotype* CRealStringGenotype::Clone( void ) {
  return (new CRealStringGenotype( *this ));
}



/****************************************/
/****************************************/

void CRealStringGenotype::RandomInit( void ) {
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    m_pfGenes[i] = CEvolution::GetRNG()->Uniform(CRange<Real>(m_fMinValue,m_fMaxValue));
  }
}



/****************************************/
/****************************************/

void CRealStringGenotype::Mutation( Real probability, bool elitism ) {
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    if( CEvolution::GetRNG()->Uniform(CRange<Real>(0,1)) < probability ) {
      m_pfGenes[i] += CEvolution::GetRNG()->Gaussian(m_fVariance);

      if( m_pfGenes[i] > m_fMaxValue ) m_pfGenes[i] = m_fMaxValue;
      if( m_pfGenes[i] < m_fMinValue ) m_pfGenes[i] = m_fMinValue;
    }
  }
}



/****************************************/
/****************************************/

void CRealStringGenotype::Crossover( const CGenotype* p1, const CGenotype* p2 ) {
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    if( CEvolution::GetRNG()->Bernoulli() ) {
      m_pfGenes[i] = ((CRealStringGenotype*)p1)->m_pfGenes[i];
    }
    else {
      m_pfGenes[i] = ((CRealStringGenotype*)p2)->m_pfGenes[i];
    }
  }
}



/****************************************/
/****************************************/

const Real* CRealStringGenotype::GetGeneValues( void ) {
  return m_pfGenes;
}



/****************************************/
/****************************************/

const UInt32 CRealStringGenotype::GetNumValues( void ) const {
  return m_unLength;
}



/****************************************/
/****************************************/

const void* CRealStringGenotype::GetGenes( void ) const {
  return (void*) m_pfGenes;
}


/****************************************/
/****************************************/

void CRealStringGenotype::WriteGenotype( ostream& os ) {
  os << "<" << CONFIGURATION_GENOTYPE_INDIVIDUAL << " ";
  os << CONFIGURATION_GENOTYPE_FITNESS   << " = \"" << m_fFitness << "\" ";
  os << CONFIGURATION_GENOTYPE_EVALUATED << " = \"" << m_bEvaluated << "\" ";
  os << CONFIGURATION_GENOTYPE_GENES     << " = \"";
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    os << m_pfGenes[i] << " ";
  }
  os << "\" />";
  os << endl;
}



/****************************************/
/****************************************/

void CRealStringGenotype::Write( ostream& os ) {
  os << "\t\t\t" << CONFIGURATION_GENOTYPE_TYPE       << " = \"" << CONFIGURATION_REALSTRING_TYPE << "\"" << endl;
  os << "\t\t\t" << CONFIGURATION_GENOTYPE_LENGTH     << " = \"" << m_unLength                    << "\"" << endl;
  os << "\t\t\t" << CONFIGURATION_REALSTRING_VARIANCE << " = \"" << m_fVariance                   << "\"" << endl;
  os << "\t\t\t" << CONFIGURATION_REALSTRING_MINVALUE << " = \"" << m_fMinValue                   << "\"" << endl;
  os << "\t\t\t" << CONFIGURATION_REALSTRING_MAXVALUE << " = \"" << m_fMaxValue                   << "\"" << endl;
}
