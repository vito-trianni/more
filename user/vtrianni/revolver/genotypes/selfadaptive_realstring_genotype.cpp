#include "selfadaptive_realstring_genotype.h"
#include <argos2/user/vtrianni/revolver/evolution.h>
#include <math.h>

using namespace argos;

const string CONFIGURATION_SAREALSTRING_MAXVALUE = "max_value";
const string CONFIGURATION_SAREALSTRING_MINVALUE = "min_value";


/****************************************/
/****************************************/

CSelfAdaptiveRealStringGenotype::CSelfAdaptiveRealStringGenotype( UInt32 length ) {
  m_unLength   = length;
  m_fFitness   = 0.0;
  m_bEvaluated = false;
  
  m_pfGenes    = new Real[length];
  m_pfSigmas   = new Real[length];
  m_fTau       = 1.0/(sqrt(2.0*sqrt((Real) length)));;
  m_fTauP      = 1.0/(sqrt(2.0*length));
  m_fMaxValue  = 0.0;
  m_fMinValue  = 0.0;
}


/****************************************/
/****************************************/

CSelfAdaptiveRealStringGenotype::CSelfAdaptiveRealStringGenotype( const CSelfAdaptiveRealStringGenotype& s ) {
  m_unLength   = s.m_unLength;
  m_fFitness   = s.m_fFitness;
  m_bEvaluated = s.m_bEvaluated;

  m_pfGenes    = new Real[m_unLength];
  m_pfSigmas   = new Real[m_unLength];
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    m_pfGenes[i]  = s.m_pfGenes[i];
    m_pfSigmas[i] = s.m_pfSigmas[i];
  }

  m_fTau      = s.m_fTau;
  m_fTauP     = s.m_fTauP;
  m_fMaxValue = s.m_fMaxValue;
  m_fMinValue = s.m_fMinValue;
}



/****************************************/
/****************************************/

CSelfAdaptiveRealStringGenotype::~CSelfAdaptiveRealStringGenotype() {
  if( m_pfGenes )  delete [] m_pfGenes;
  if( m_pfSigmas ) delete [] m_pfSigmas;
}


/****************************************/
/****************************************/

void CSelfAdaptiveRealStringGenotype::Init( TConfigurationNode& t_configuration_tree ) {
  // max value
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_SAREALSTRING_MAXVALUE, m_fMaxValue );

  // min value
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_SAREALSTRING_MINVALUE, m_fMinValue );
}


/****************************************/
/****************************************/

void CSelfAdaptiveRealStringGenotype::InitGenes( TConfigurationNode& t_configuration_tree ) {
  CGenotype::InitGenes(t_configuration_tree);

  string genes;
  GetNodeAttribute(t_configuration_tree, "genes", genes);
  istringstream input_genes(genes);
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    input_genes >> m_pfGenes[i];
    input_genes >> m_pfSigmas[i];
  }
}


/****************************************/
/****************************************/

CGenotype* CSelfAdaptiveRealStringGenotype::Clone( void ) {
  return (new CSelfAdaptiveRealStringGenotype( *this ));
}



/****************************************/
/****************************************/

void CSelfAdaptiveRealStringGenotype::RandomInit( void ) {
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    m_pfGenes[i]  = CEvolution::GetRNG()->Uniform(CRange<Real>(m_fMinValue, m_fMaxValue));
    m_pfSigmas[i] = pow(CEvolution::GetRNG()->Gaussian(1,0), 2); // CHECK THIS!
  }
}



/****************************************/
/****************************************/

void CSelfAdaptiveRealStringGenotype::Mutation( Real probability, bool elitism ) {
  Real Ni = CEvolution::GetRNG()->Gaussian(1,0);
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    m_pfSigmas[i] *= exp( CEvolution::GetRNG()->Gaussian(m_fTauP) + m_fTau*Ni );
    m_pfGenes[i]  += CEvolution::GetRNG()->Gaussian( m_pfSigmas[i] );

    if( m_pfGenes[i] > m_fMaxValue ) m_pfGenes[i] = m_fMaxValue;
    if( m_pfGenes[i] < m_fMinValue ) m_pfGenes[i] = m_fMinValue;
  }
}



/****************************************/
/****************************************/

void CSelfAdaptiveRealStringGenotype::Crossover( const CGenotype* p1, const CGenotype* p2 ) {
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    if( CEvolution::GetRNG()->Bernoulli() ) {
      m_pfGenes[i] = ((CSelfAdaptiveRealStringGenotype*)p1)->m_pfGenes[i];
    }
    else {
      m_pfGenes[i] = ((CSelfAdaptiveRealStringGenotype*)p2)->m_pfGenes[i];
    }
  }
}



/****************************************/
/****************************************/

const Real* CSelfAdaptiveRealStringGenotype::GetGeneValues( void ) {
  return m_pfGenes;
}



/****************************************/
/****************************************/

const UInt32 CSelfAdaptiveRealStringGenotype::GetNumValues( void ) const {
  return m_unLength;
}



/****************************************/
/****************************************/

const void* CSelfAdaptiveRealStringGenotype::GetGenes( void ) const {
  return (void*) m_pfGenes;
}


/****************************************/
/****************************************/

void CSelfAdaptiveRealStringGenotype::WriteGenotype( ostream& os ) {
  os << "<" << CONFIGURATION_GENOTYPE_INDIVIDUAL << " ";
  os << CONFIGURATION_GENOTYPE_FITNESS   << " = \"" << m_fFitness << "\" ";
  os << CONFIGURATION_GENOTYPE_EVALUATED <<" = \"" << m_bEvaluated << "\" ";
  os << CONFIGURATION_GENOTYPE_GENES     << " = \"";
  for( UInt32 i = 0; i < m_unLength; i++ ) {
    os << m_pfGenes[i]  << " ";
    os << m_pfSigmas[i] << " ";
  }
  os << "\" />";
  os << endl;
}



/****************************************/
/****************************************/

void CSelfAdaptiveRealStringGenotype::Write( ostream& os ) {
  os << "\t\t\t" << CONFIGURATION_GENOTYPE_TYPE         << " = \"" << CONFIGURATION_SELFADAPTIVE_TYPE << "\"" << endl;
  os << "\t\t\t" << CONFIGURATION_GENOTYPE_LENGTH       << " = \"" << m_unLength                      << "\"" << endl;
  os << "\t\t\t" << CONFIGURATION_SAREALSTRING_MINVALUE << " = \"" << m_fMinValue                     << "\"" << endl;
  os << "\t\t\t" << CONFIGURATION_SAREALSTRING_MAXVALUE << " = \"" << m_fMaxValue                     << "\"" << endl;
}
