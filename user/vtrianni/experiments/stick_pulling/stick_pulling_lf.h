#ifndef _CSTICKPULLINGLOOPFUNCTION_H_
#define _CSTICKPULLINGLOOPFUNCTION_H_

#include <argos2/user/vtrianni/loop_functions/evolutionary_lf/evolutionary_lf.h>

#include <argos2/common/utility/argos_random.h>
#include <argos2/simulator/space/entities/floor_entity.h>
#include <argos2/simulator/space/entities/footbot_entity.h>
#include <argos2/simulator/space/entities/cylinder_entity.h>


using namespace std;
using namespace argos;


// loop function for simple foraging
class CStickPullingLoopFunctions : public CEvolutionaryLoopFunctions {
 private:
   // variables for the robots initialisation
   UInt32               m_unMaxInitTrials;
   UInt32               m_unNumInitFootbots;
   Real                 m_fMinInitDistance;
   Real                 m_fArenaRadius;
   UInt32               m_unNumArenaWalls;
   Real                 m_fMaxRobotSpeed;
   UInt32               m_unNumSticks;
   Real                 m_fStickHeight;
   Real                 m_fStickRadius;
   Real                 m_fStickPullingRadius;
   UInt32               m_unMinPullingRobots;
   CVector3             m_cEatenStickPosition;
   

   // variables for computing the  fitness
   UInt32               m_unNumPulledSticks;
   bool                 m_bCollisionDetected;
   UInt32**             m_punStickVisitedPerRobot;


   // map for accessing the footbots
   CSpace::TAnyEntityMap m_tFootBotMap;

   // map for accessing the sticks
   CSpace::TAnyEntityMap m_tStickMap;

   // variables for the 
   CARGoSRandom::CRNG*  m_pcRNG;


 public:
   CStickPullingLoopFunctions();
   virtual ~CStickPullingLoopFunctions();

   virtual void     Init( TConfigurationNode& t_configuration_node );
   virtual void     Reset( void );
   virtual void     Destroy( void );

   virtual void     PostPhysicsEngineStep( void );
   virtual bool     IsExperimentFinished( void ) {return m_bCollisionDetected || (m_unNumPulledSticks == m_unNumSticks && m_unNumSticks != 0);};
   virtual Real     ComputePerformanceInExperiment( unsigned int* pun_num_values = NULL, Real** pf_perf_values = NULL );
   virtual CColor   GetFloorColor( const CVector2& c_position_on_plane );
   bool             SetExperiment( void );
};



#endif
