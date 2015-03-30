#include "test_loop_functions.h"
#include <argos2/simulator/space/entities/footbot_entity.h>
#include <argos2/simulator/space/entities/floor_entity.h>
#include <argos2/simulator/visualizations/render.h>
#include <argos2/common/utility/argos_random.h>

/****************************************/
/****************************************/

CTestLoopFunctions::CTestLoopFunctions() :
   counter(0) {
   LOG << "CTestLoopFunctions::CTestLoopFunctions()" << std::endl;
}
   
/****************************************/
/****************************************/

CTestLoopFunctions::~CTestLoopFunctions() {
   LOG << "~CTestLoopFunctions::CTestLoopFunctions()" << std::endl;
}
   
/****************************************/
/****************************************/

void CTestLoopFunctions::Init(TConfigurationNode& t_tree) {
   m_pcRNG = CARGoSRandom::CreateRNG( "argos" );

   LOG << "CTestLoopFunctions::Init()" << std::endl;
   LOG << t_tree << std::endl;

   counter = m_pcRNG->Uniform(CRange<SInt32>(0,3));
   LOG << "[INIT] Loading pattern " << counter << std::endl;

   patterns[0] = CColor::BLACK;
   patterns[1] = CColor::RED;
   patterns[2] = CColor::GREEN;
   patterns[3] = CColor::BLUE;

  // get the plane entity
   m_cSpace.GetFloorEntity().Reset();
}
   
/****************************************/
/****************************************/

void CTestLoopFunctions::Reset() {
   LOG << "CTestLoopFunctions::Reset()" << std::endl;
   counter = m_pcRNG->Uniform(CRange<SInt32>(0,3));
   LOG << "[RESET] Loading pattern " << counter << std::endl;
}

/****************************************/
/****************************************/

void CTestLoopFunctions::Destroy() {
   LOG << "CTestLoopFunctions::Destroy()" << std::endl;
}

/****************************************/
/****************************************/

void CTestLoopFunctions::PrePhysicsEngineStep() {
   LOG << "CTestLoopFunctions::PrePhysicsEngineStep()" << std::endl;
}
   
/****************************************/
/****************************************/

void CTestLoopFunctions::PostPhysicsEngineStep() {
  LOG << "CTestLoopFunctions::PostPhysicsEngineStep()" << std::endl;
}
   
/****************************************/
/****************************************/

bool CTestLoopFunctions::IsExperimentFinished() {
   LOG << "CTestLoopFunctions::IsExperimentFinished()" << std::endl;
   return false;
}
   
/****************************************/
/****************************************/

CColor CTestLoopFunctions::GetFloorColor(const CVector2& c_position_on_plane) {
  if(c_position_on_plane.GetX() > 0.0f) {
      if(c_position_on_plane.GetY() > 0.0f) {
	return patterns[counter%4];
      }
      else {
	return patterns[(counter+1)%4];
      }
   }
   else {
      if(c_position_on_plane.GetY() > 0.0f) {
	return patterns[(counter+2)%4];
      }
      else {
	return patterns[(counter+3)%4];
      }
   }
}

/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS(CTestLoopFunctions, "test_loop_functions");
