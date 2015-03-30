#include "evolutionary_lf.h"


/****************************************/
/****************************************/

CEvolutionaryLoopFunctions::CEvolutionaryLoopFunctions() :
   CLoopFunctions(),
   m_unTrialNumber( 0 ),
   m_bIsEvolution( false )
{
   ////////////////////////////////////////////////////////////////////////////////
   // Create a new random number generator for the loop function
   ////////////////////////////////////////////////////////////////////////////////
   m_pcRNG = CARGoSRandom::CreateRNG( "argos" );
   SetTrialNumber( m_pcRNG->Uniform(CRange<UInt32>(0,UINT_MAX)) );   
}

  
/****************************************/
/****************************************/

void CEvolutionaryLoopFunctions::Reset( void ) {
   // Get a random trial number
   if( !m_bIsEvolution ) {
      SetTrialNumber( m_pcRNG->Uniform(CRange<UInt32>(0,UINT_MAX)) );
   }
}


/****************************************/
/****************************************/
