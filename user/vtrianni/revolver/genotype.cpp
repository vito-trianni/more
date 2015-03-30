#include "genotype.h"

using namespace argos;

/****************************************/
/****************************************/

CGenotype::~CGenotype() {
  m_unLength  = 0;
  m_fFitness  = 0.0;
  m_bEvaluated = false;
}


/****************************************/
/****************************************/

void CGenotype::InitGenes( TConfigurationNode& t_configuration_tree ) {
  // fitness initialisation
  GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_GENOTYPE_FITNESS, m_fFitness, m_fFitness );
  GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_GENOTYPE_EVALUATED, m_bEvaluated, m_bEvaluated );
}


/****************************************/
/****************************************/
