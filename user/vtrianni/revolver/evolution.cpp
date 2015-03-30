#include "evolution.h"

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace argos;

const string  CONFIGURATION_EVOLUTION_MAXGENERATION   = "max_generation";
const string  CONFIGURATION_EVOLUTION_SAVEGENERATION  = "save_generation_step";
const string  CONFIGURATION_EVOLUTION_NUMGENERATION   = "num_generation";
const string  CONFIGURATION_EVOLUTION_MAXIMISTAION    = "maximisation";
const string  CONFIGURATION_EVOLUTION_RANDOMSEED      = "random_seed";
const string  CONFIGURATION_EVOLUTION_EVOBASEFILE     = "evolution_basefile";
const string  CONFIGURATION_EVOLUTION_FITNESSBASEFILE = "fitness_basefile";
const string  CONFIGURATION_EVOLUTION_BESTINDBASEFILE = "bestind_basefile";

CEvolution* CEvolution::m_pcEvolutionInstance = NULL;
CARGoSRandom::CRNG* CEvolution::m_pcRNG = NULL;



/****************************************/
/****************************************/

CEvolution* CEvolution::GetInstance() {
  static auto_ptr<CEvolution> pc_evolution_instance( m_pcEvolutionInstance );

  return pc_evolution_instance.get();
}


/****************************************/
/****************************************/
/****************************************/
/****************************************/
/****************************************/
/****************************************/
/****************************************/
/****************************************/

CEvolution::CEvolution() {
  m_unNumGeneration = 0;
  m_unMaxGeneration = 0;
  m_unSaveGenerationStep = 1;
  m_bMaximization = true;
  
  m_sWorkingDir = ".";
  m_sEvolutionBaseFilename = "";
  m_sBestIndividualBaseFilename = "";
  m_sFitnessFilename = "";

  m_unRandomSeed = 0;
}



/****************************************/
/****************************************/

CEvolution::~CEvolution() {
}


/****************************************/
/****************************************/

void CEvolution::Init( TConfigurationNode& t_configuration_tree ) {
  ////////////////////////////////////////////////////////////////////////////////
  // evolutionary paramters
  ////////////////////////////////////////////////////////////////////////////////
  GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_EVOLUTION_MAXGENERATION, m_unMaxGeneration, m_unMaxGeneration );
  GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_EVOLUTION_SAVEGENERATION, m_unSaveGenerationStep, m_unSaveGenerationStep );
  GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_EVOLUTION_NUMGENERATION, m_unNumGeneration, m_unNumGeneration );
  GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_EVOLUTION_MAXIMISTAION, m_bMaximization, m_bMaximization );

  ////////////////////////////////////////////////////////////////////////////////
  // evolutionary base filenames
  ////////////////////////////////////////////////////////////////////////////////
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_EVOLUTION_FITNESSBASEFILE, m_sFitnessFilename );
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_EVOLUTION_BESTINDBASEFILE, m_sBestIndividualBaseFilename );
  GetNodeAttribute(t_configuration_tree, CONFIGURATION_EVOLUTION_EVOBASEFILE, m_sEvolutionBaseFilename );


  ////////////////////////////////////////////////////////////////////////////////
  // random number generator initialisation
  ////////////////////////////////////////////////////////////////////////////////
  GetNodeAttributeOrDefault(t_configuration_tree, CONFIGURATION_EVOLUTION_RANDOMSEED, m_unRandomSeed, m_unRandomSeed );

  InitRandom( m_unRandomSeed );
}



/****************************************/
/****************************************/

void CEvolution::InitRandom( UInt32 un_seed ) {
  CARGoSRandom::CreateCategory(RNG_EVOLUTION,un_seed);
  m_pcRNG = CARGoSRandom::CreateRNG( RNG_EVOLUTION );
}



/****************************************/
/****************************************/

void CEvolution::Write( ostream& os ) {
  os << "\t" << CONFIGURATION_EVOLUTION_MAXGENERATION   << " = \"" << m_unMaxGeneration                    << "\"" << endl;
  os << "\t" << CONFIGURATION_EVOLUTION_SAVEGENERATION  << " = \"" << m_unSaveGenerationStep               << "\"" << endl;
  os << "\t" << CONFIGURATION_EVOLUTION_NUMGENERATION   << " = \"" << m_unNumGeneration                    << "\"" << endl;
  os << "\t" << CONFIGURATION_EVOLUTION_MAXIMISTAION    << " = \"" << (m_bMaximization ? "true" : "false") << "\"" << endl;
  os << "\t" << CONFIGURATION_EVOLUTION_RANDOMSEED      << " = \"" << m_unRandomSeed                       << "\"" << endl;
  os << "\t" << CONFIGURATION_EVOLUTION_EVOBASEFILE     << " = \"" << m_sEvolutionBaseFilename             << "\"" << endl;
  os << "\t" << CONFIGURATION_EVOLUTION_FITNESSBASEFILE << " = \"" << m_sFitnessFilename                   << "\"" << endl;
  os << "\t" << CONFIGURATION_EVOLUTION_BESTINDBASEFILE << " = \"" << m_sBestIndividualBaseFilename        << "\"" << endl;
}

