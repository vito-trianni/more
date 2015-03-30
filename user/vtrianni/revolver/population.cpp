#include "population.h"

#include <algorithm>

#include <argos2/user/vtrianni/revolver/genotypes/bitstring_genotype.h>
#include <argos2/user/vtrianni/revolver/genotypes/realstring_genotype.h>
#include <argos2/user/vtrianni/revolver/genotypes/selfadaptive_realstring_genotype.h>

#include <argos2/common/utility/logging/argos_log.h>

using namespace argos;

const string CONFIGURATION_POPULATION_GENOTYPE    = "genotype";
const string CONFIGURATION_POPULATION_INDIVIDUALS = "individuals";
const string CONFIGURATION_POPULATION_LOAD_FILE   = "load_file";


/****************************************/
/****************************************/

CPopulation::CPopulation() {
  m_unSize           = 0;
  m_pcSampleGenotype = NULL;
}


/****************************************/
/****************************************/

CPopulation::~CPopulation() {
  for( UInt32 i = 0; i < m_vecIndividuals.size(); i++ ) {
    delete m_vecIndividuals[i];
  }
  m_vecIndividuals.clear();
  
  for( UInt32 i = 0; i < m_vecOffspring.size(); i++ ) {
    delete m_vecOffspring[i];
  }
  m_vecOffspring.clear();
  

  if( m_pcSampleGenotype != NULL ) {
    delete m_pcSampleGenotype;
  }
}


/****************************************/
/****************************************/

void CPopulation::Init( TConfigurationNode& t_configuration_tree ) {
  ////////////////////////////////////////////////////////////////////////////////
  // population size
  ////////////////////////////////////////////////////////////////////////////////
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_POPULATION_SIZE, m_unSize );


  ////////////////////////////////////////////////////////////////////////////////
  // Read the genotype subtree
  ////////////////////////////////////////////////////////////////////////////////
  InitGenotype( GetNode(t_configuration_tree, CONFIGURATION_POPULATION_GENOTYPE) );

  ////////////////////////////////////////////////////////////////////////////////
  // Read the individuals subtree
  ////////////////////////////////////////////////////////////////////////////////
  InitIndividuals( t_configuration_tree );
}


/****************************************/
/****************************************/

void CPopulation::InitGenotype( TConfigurationNode& t_configuration_tree ) {
  ////////////////////////////////////////////////////////////////////////////////
  // genotype type
  ////////////////////////////////////////////////////////////////////////////////
  string s_genotype_type;
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_GENOTYPE_TYPE, s_genotype_type );

  ////////////////////////////////////////////////////////////////////////////////
  // genotype length
  ////////////////////////////////////////////////////////////////////////////////
  UInt32 un_genotype_length = 0;
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_GENOTYPE_LENGTH, un_genotype_length );

  ////////////////////////////////////////////////////////////////////////////////
  // build a sample genotype
  ////////////////////////////////////////////////////////////////////////////////
  if( s_genotype_type == CONFIGURATION_BITSTRING_TYPE ) {
    m_pcSampleGenotype = new CBitStringGenotype( un_genotype_length );
  }
  else if( s_genotype_type == CONFIGURATION_REALSTRING_TYPE ) {
    m_pcSampleGenotype = new CRealStringGenotype( un_genotype_length );
  }
  else if( s_genotype_type == CONFIGURATION_SELFADAPTIVE_TYPE ) {
    m_pcSampleGenotype = new CSelfAdaptiveRealStringGenotype( un_genotype_length );
  }
  else {
    THROW_ARGOSEXCEPTION("Cannot recognise genotype type from '"
			 << s_genotype_type << "' in <" << CONFIGURATION_GENOTYPE_TYPE 
			 << "> tag passed for genotype initialisation" );
  }
  
  m_pcSampleGenotype->Init( t_configuration_tree );
}


/****************************************/
/****************************************/

void CPopulation::InitIndividuals( TConfigurationNode& t_configuration_tree ) {
  // check if a sample genotype was created
  if( m_pcSampleGenotype == NULL ) {
    THROW_ARGOSEXCEPTION("Attempting to generate a new population without a sample genotype");
  }

  // check if there exist a section for individuals
  if( NodeExists(t_configuration_tree, CONFIGURATION_POPULATION_INDIVIDUALS) ) {
    // TConfigurationNode& t_individuals_tree = GetNode(t_configuration_tree, CONFIGURATION_POPULATION_INDIVIDUALS)

    THROW_ARGOSEXCEPTION("FEATURE NOT YET IMPLEMENTED");

    //     // check if a different file for loading is specified
    //     bool   b_load_from_other_file = false;
    //     string s_load_from_other_file = it_root->GetNodeAttributeValue( CONFIGURATION_POPULATION_LOAD_FILE );
    //     if( (s_load_from_other_file != "") && (StringToBool(s_load_from_other_file, b_load_from_other_file ) == -1) ) {
    //       CSwarmanoidLogger::LogErr() << "[FATAL] Not a boolean value for attribute '" << CONFIGURATION_POPULATION_LOAD_FILE << "'" << endl;
    //       return -1;
    //     }
    
    //     if( b_load_from_other_file ) {
    //       CSwarmanoidLogger::LogErr() << "[INFO] Loading individual population from a different file" << endl;
    //       return 0;
    //     }
    
    
    //     // load from the current file
    //     TConfigurationTree::sibling_iterator it_individual = ++it_root;
    
    //     while( it_individual != t_configuration_tree.end ( it_individual ) ) {
    //       CGenotype* pc_genotype = m_pcSampleGenotype->Clone(); 
    //       if( pc_genotype->InitGenes( TConfigurationTree( it_individual ) ) == -1 ) return -1;
    //       m_vecIndividuals.push_back( pc_genotype );
    
    //       it_individual++;
    //     }
    
    //     // POPULATION SUCCESSFULLY LOADED - RETURN
    //     return 0;
    //   }

  }

  // no individuals section was found: generate the population
  // randomly from the sample genotype
  LOG << "[INFO] generating random population..." << endl;
  for( UInt32 i = 0; i < m_unSize; i++ ) {
    CGenotype* pc_genotype = m_pcSampleGenotype->Clone();
    pc_genotype->RandomInit();
    m_vecIndividuals.push_back( pc_genotype );
  }
}




/****************************************/
/****************************************/

void CPopulation::InitIndividuals( const string s_filename ) {
  // check if a sample genotype was created
  if( m_pcSampleGenotype == NULL ) {
    THROW_ARGOSEXCEPTION("[FATAL] Attempting to generate a new population without a sample genotype");
  }

  LOG << "[INFO] intialising population from " << s_filename << endl;
  LOGERR.Flush();
  LOG.Flush();
  
  ticpp::Document tConfiguration;
  tConfiguration.LoadFile(s_filename);
  TConfigurationNode tConfigurationRoot   = *tConfiguration.FirstChildElement();
  TConfigurationNode t_evolution_config   = GetNode(tConfigurationRoot, "evolution");
  TConfigurationNode t_population_config  = GetNode( t_evolution_config, CONFIGURATION_EVOLUTION_POPULATION );
  TConfigurationNode t_individuals_tree   = GetNode( t_population_config, CONFIGURATION_POPULATION_INDIVIDUALS );

  TConfigurationNodeIterator itIndividual;
  UInt32 i = 0;
  for(itIndividual  = itIndividual.begin(&t_individuals_tree);
      itIndividual != itIndividual.end();
      ++itIndividual, ++i) {
    //     string genes;
    //     GetNodeAttribute(*itIndividual, "genes", genes);
    //     LOG << "loading individual " << setw(3) << i << ": " << genes << endl;
    //     LOG.Flush();
    CGenotype* pc_genotype = m_pcSampleGenotype->Clone(); 
    pc_genotype->InitGenes( *itIndividual );
    m_vecIndividuals.push_back( pc_genotype );
  }
}




/****************************************/
/****************************************/

void CPopulation::Rank( bool b_maximisation ) {
  if( b_maximisation )
    sort( m_vecIndividuals.begin(), m_vecIndividuals.end(), gtgenotype() );
  else 
    sort( m_vecIndividuals.begin(), m_vecIndividuals.end(), ltgenotype() );
}
  


/****************************************/
/****************************************/

void CPopulation::WritePopulation( ostream& os ) {
  CGenotype* pc_genotype = FirstIndividual();
  if( pc_genotype == NULL ) {
    return;
  }
  
  os << "\t\t<" << CONFIGURATION_POPULATION_GENOTYPE << endl;
  pc_genotype->Write( os );
  os << "\t\t/>" << endl;

  os << "\t\t<" << CONFIGURATION_POPULATION_INDIVIDUALS << ">" << endl;
  while( pc_genotype != NULL ) {
    os << "\t\t\t";
    pc_genotype->WriteGenotype( os );
    pc_genotype = NextIndividual();
  }
  os << "\t\t</" << CONFIGURATION_POPULATION_INDIVIDUALS << ">" << endl;
}
