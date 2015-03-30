#ifndef TEST_LOOP_FUNCTIONS_H
#define TEST_LOOP_FUNCTIONS_H

#include <argos2/simulator/dynamic_linking/loop_functions.h>

using namespace argos;

class CTestLoopFunctions : public argos::CLoopFunctions {

public:

   CTestLoopFunctions();
   virtual ~CTestLoopFunctions();

   virtual void Init(argos::TConfigurationNode& t_tree);
   virtual void Reset();
   virtual void Destroy();

   virtual void PrePhysicsEngineStep();
   virtual void PostPhysicsEngineStep();
   virtual bool IsExperimentFinished();

   virtual argos::CColor GetFloorColor(const argos::CVector2& c_position_on_plane);

  CARGoSRandom::CRNG*  m_pcRNG;

private:

   SInt32 counter;
   CColor patterns[4];
};

#endif
