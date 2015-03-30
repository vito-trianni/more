#ifndef _CEVOLUTIONARYLOOPFUNCTION_H_
#define _CEVOLUTIONARYLOOPFUNCTION_H_

#include <limits.h>
#include <argos2/simulator/dynamic_linking/loop_functions.h>

using namespace std;
using namespace argos;


// loop function for simple foraging
class CEvolutionaryLoopFunctions : public CLoopFunctions {
 private:
   UInt32               m_unTrialNumber;
   bool                 m_bIsEvolution;

   // variables for the 
   CARGoSRandom::CRNG*  m_pcRNG;


 public:
   CEvolutionaryLoopFunctions();
   virtual ~CEvolutionaryLoopFunctions() {};

   virtual void Reset( void );

   virtual inline const UInt32 GetTrialNumber() {return m_unTrialNumber;};
   virtual inline void SetTrialNumber( UInt32 un_trial) {m_unTrialNumber = un_trial;};

   virtual inline const bool IsEvolution() {return m_bIsEvolution;};
   virtual inline void SetEvolution() {m_bIsEvolution = true;};
   virtual inline void ResetEvolution() {m_bIsEvolution = false;};
};



#endif
