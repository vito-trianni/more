#ifndef _CFLOCKINGLOOPFUNCTION_H_
#define _CFLOCKINGLOOPFUNCTION_H_

#include <argos2/user/vtrianni/loop_functions/evolutionary_lf/evolutionary_lf.h>

#include <argos2/common/utility/argos_random.h>
#include <argos2/simulator/space/entities/floor_entity.h>
#include <argos2/simulator/space/entities/footbot_entity.h>


using namespace std;
using namespace argos;


// loop function for simple foraging
class CFlockingLoopFunctions : public CEvolutionaryLoopFunctions {
 private:
   // variables for the robots initialisation
   UInt32               m_unNumInitFootbots;
   Real                 m_fMinInitDistance;
   Real                 m_fInitRadius;
   Real                 m_fMaxRobotSpeed;
   CRange<Real>         m_cCohesionRange;

   // variables for aggregate fitness computation
   Real                 m_fGammaWeight;

   // map for accessing the footbots
   CSpace::TAnyEntityMap m_tFootBotMap;

   // variables for the 
   CARGoSRandom::CRNG*  m_pcRNG;

 public:
   CFlockingLoopFunctions();
   virtual ~CFlockingLoopFunctions();

   virtual void     Init( TConfigurationNode& t_configuration_node );
   virtual void     Reset( void );
   virtual void     Destroy( void );

   virtual Real     ComputePerformanceInExperiment( unsigned int* pun_num_values = NULL, Real** pf_perf_values = NULL );
   virtual CColor   GetFloorColor( const CVector2& c_position_on_plane );
   void             SetExperiment( void );
};



#endif
