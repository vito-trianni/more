#ifndef _COBSTACLEAVOIDANCELOOPFUNCTION_H_
#define _COBSTACLEAVOIDANCELOOPFUNCTION_H_

#include <argos2/simulator/dynamic_linking/loop_functions.h>

#include <argos2/common/utility/argos_random.h>
#include <argos2/simulator/space/entities/floor_entity.h>
#include <argos2/simulator/space/entities/footbot_entity.h>

// #include "revolver/evolutions/standard_evolution.h"
// #include "revolver/evolutions/post_evaluation.h"

using namespace std;
using namespace argos;

// Helper class for storing the points (segments) around which to
// intialise the robot
class TInitPoints {
 public:
   Real      m_fTotalLength;
   CSegment  m_cInitSegment;
   
   TInitPoints( const Real& f_length, const CSegment& c_segment );	 
   ~TInitPoints() {};
   
};

// fitness type
enum TFitnessType {
   T_SO_TRAD,
   T_SO_CIRC,
   T_SO_WGHT,
   T_MO };

// loop function for obstacle avoidance
class CObstacleAvoidanceLoopFunctions : public CLoopFunctions {
 private:
  // variables for the fitness computation
  TFitnessType         m_tFitnessType;
  Real                 m_fAvgWeight;

  // variables for the robots initialisation
  Real                 m_fArenaSize;
  
  vector<TInitPoints>  m_vecInitPoints;
  Real                 m_fInitLength;
  Real                 m_fInitRadius;

  Real                 m_fMaxSpeed;

  UInt32               m_unNumSteps;
  Real                 m_fIndividualFitness;
  Real                 m_fSpeedComponent;
  Real                 m_fProximityComponent;
  
  Real                 m_fPositionOnCircuit;
  Real                 m_fCoveredLengthOnCircuit;

  CARGoSRandom::CRNG*  m_pcRNG;

 public:
  CObstacleAvoidanceLoopFunctions();
  virtual ~CObstacleAvoidanceLoopFunctions();

  virtual void    Init( TConfigurationNode& t_configuration_node );
  virtual void    InitStartingPoints( TConfigurationNode& t_configuration_node );
  virtual void    Reset( void );
  virtual void    Destroy( void );

  virtual void    PostPhysicsEngineStep( void );
  virtual Real    ComputePerformanceInExperiment( unsigned int* pun_num_values = NULL, Real** pf_perf_values = NULL );
  virtual CColor  GetFloorColor( const CVector2& c_position_on_plane );

  virtual Real    GetPositionOnCircuit( const CVector2& c_position );
};



#endif
