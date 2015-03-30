#include "flocking_lf.h"
#include "flocking_nn.h"

const string CONFIGURATION_FLOCKING_NUM_ROBOTS             = "num_robots";
const string CONFIGURATION_FLOCKING_MIN_INIT_DISTANCE      = "min_init_distance";
const string CONFIGURATION_FLOCKING_INIT_RADIUS            = "init_radius";
const string CONFIGURATION_FLOCKING_MAX_ROBOT_SPEED        = "max_robot_speed";
const string CONFIGURATION_FLOCKING_COHESION_RANGE         = "cohesion_range";
const string CONFIGURATION_FLOCKING_GAMMA_WEIGHT           = "gamma_weight";

static const Real FOOTBOT_RADIUS = 0.1;

/****************************************/
/****************************************/

CFlockingLoopFunctions::CFlockingLoopFunctions() :
   CEvolutionaryLoopFunctions(),
   m_unNumInitFootbots( 0 ),
   m_fMinInitDistance(0.25),
   m_fInitRadius(2.0),
   m_fMaxRobotSpeed(10),
   m_cCohesionRange(1,3),
   m_fGammaWeight(0.5)
{}

  
/****************************************/
/****************************************/

CFlockingLoopFunctions::~CFlockingLoopFunctions() {
}


/****************************************/
/****************************************/

void CFlockingLoopFunctions::Init( TConfigurationNode& t_tree ) {
   ////////////////////////////////////////////////////////////////////////////////
   // Create a new random number generator for the loop function
   ////////////////////////////////////////////////////////////////////////////////
   m_pcRNG = CARGoSRandom::CreateRNG( "argos" );

   ////////////////////////////////////////////////////////////////////////////////
   // Paramteres for creation and random positioning of the footbots and preys
   ////////////////////////////////////////////////////////////////////////////////

   GetNodeAttribute(t_tree, CONFIGURATION_FLOCKING_NUM_ROBOTS, m_unNumInitFootbots );
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_FLOCKING_MIN_INIT_DISTANCE, m_fMinInitDistance, m_fMinInitDistance );
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_FLOCKING_INIT_RADIUS, m_fInitRadius, m_fInitRadius );
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_FLOCKING_MAX_ROBOT_SPEED, m_fMaxRobotSpeed, m_fMaxRobotSpeed );
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_FLOCKING_COHESION_RANGE, m_cCohesionRange, m_cCohesionRange);
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_FLOCKING_GAMMA_WEIGHT, m_fGammaWeight, m_fGammaWeight);

   ////////////////////////////////////////////////////////////////////////////////
   // CREATION AND POSITIONING OF THE FOOTBOTS
   ////////////////////////////////////////////////////////////////////////////////
   ostringstream entity_id;
   for( UInt32 i = 0; i < m_unNumInitFootbots; ++i ) {
      entity_id.str("");
      entity_id << "footbot_" << i;
      AddFootBot( entity_id.str(), CVector3(1000,1000,0), CQuaternion(), "nn", "ground" );
   }
   m_tFootBotMap = m_cSpace.GetEntitiesByType("footbot_entity");

   // Create and set the experiment
   SetExperiment();
}


/****************************************/
/****************************************/

void CFlockingLoopFunctions::Reset( void ) {
   // Reset the experiment
   SetExperiment();
}


/****************************************/
/****************************************/

void CFlockingLoopFunctions::Destroy( void ) {

}

/****************************************/
/****************************************/

Real CFlockingLoopFunctions::ComputePerformanceInExperiment( UInt32* pun_num_values, Real** pf_perf_values ) {
   // compute position of the center of mass
   CVector3 c_com_position(0,0,0);
   for( CSpace::TAnyEntityMap::iterator it = m_tFootBotMap.begin(); it != m_tFootBotMap.end(); ++it ) {
      CFootBotEntity& c_footbot = *(any_cast<CFootBotEntity*>(it->second));
      c_com_position += c_footbot.GetEmbodiedEntity().GetPosition();
   }
   c_com_position /= m_unNumInitFootbots;


   Real f_avg_com_distance = 0;
   Real f_max_footbot_distance = 0;
   CVector3 c_group_main_axis(0,0,0);
   CVector3 c_group_polarization(0,0,0);
   Real flock_direction = 0;
   for( CSpace::TAnyEntityMap::iterator it = m_tFootBotMap.begin(); it != m_tFootBotMap.end(); ++it ) {
      CFootBotEntity& c_footbot = *(any_cast<CFootBotEntity*>(it->second));
      CVector3 c_position = c_footbot.GetEmbodiedEntity().GetPosition();

      // compute average distance from center of mass
      f_avg_com_distance += (c_com_position - c_position).Length();

      // compute group polarization
      CVector3 footbot_direction(1.0, 0, 0 );
      footbot_direction.Rotate( c_footbot.GetEmbodiedEntity().GetOrientation() );
      c_group_polarization += footbot_direction;

      // compute the larger axis of the flock
      for( CSpace::TAnyEntityMap::iterator it2 = m_tFootBotMap.begin(); it2 != m_tFootBotMap.end(); ++it2 ) {
	 if( it == it2 ) continue;
	 
	 CFootBotEntity& c_footbot_2 = *(any_cast<CFootBotEntity*>(it2->second));
	 CVector3 c_position_2 = c_footbot_2.GetEmbodiedEntity().GetPosition();
	 
	 Real f_distance = (c_position_2 - c_position).Length();
	 if( f_distance > f_max_footbot_distance ) {
	    f_max_footbot_distance = f_distance;
	    c_group_main_axis = (c_position_2 - c_position);
	 }
      }
   }
   f_avg_com_distance /= m_unNumInitFootbots;
   c_group_main_axis.Normalize();
   c_group_polarization.Normalize();


   Real f_motion = c_com_position.Length()/(m_fMaxRobotSpeed/1000*m_cSimulator.GetMaxSimulationClock());
   Real f_cohesion = 1.0 - m_cCohesionRange.NormalizeValue(f_avg_com_distance);
   Real f_dot_product = fabs(c_group_main_axis.DotProduct( c_group_polarization ));
   Real f_cross_product = fabs((c_group_main_axis.CrossProduct( c_group_polarization )).Length());


   *pun_num_values = 2;
   *pf_perf_values = new Real[2];
   (*pf_perf_values)[0] = f_motion;
   (*pf_perf_values)[1] = f_cohesion;

   return m_fGammaWeight*f_motion + (1.0-m_fGammaWeight)*f_cohesion;
}


/****************************************/
/****************************************/

CColor CFlockingLoopFunctions::GetFloorColor( const CVector2& c_position_on_plane ) {
   if( c_position_on_plane.Length() < m_fInitRadius )
      return CColor::WHITE;

   return CColor::GRAY70;
}



/****************************************/
/****************************************/

void CFlockingLoopFunctions::SetExperiment( void ) {
   vector<CVector3> used_positions;

   // Initalise the footbots
   UInt32 i = 0;
   for( CSpace::TAnyEntityMap::iterator it = m_tFootBotMap.begin(); it != m_tFootBotMap.end(); ++it, ++i ) {
      CFootBotEntity& footbot = *(any_cast<CFootBotEntity*>(it->second));
      
      CVector3    random_position;
      CQuaternion random_rotation;
      bool        distant_enough = false; 
      while( !distant_enough ) {
	 Real rho   = m_pcRNG->Uniform(CRange<Real>(0,m_fInitRadius));
	 Real theta = m_pcRNG->Uniform(CRange<Real>(-CRadians::PI.GetValue(),CRadians::PI.GetValue()));
	 random_position = CVector3(rho*cos(theta), rho*sin(theta),0.0);
	 random_rotation.FromEulerAngles( CRadians(m_pcRNG->Uniform(CRange<Real>(-CRadians::PI.GetValue(),CRadians::PI.GetValue()))), 
					  CRadians::ZERO, 
					  CRadians::ZERO );
      
      	 // check distance from other footbots
	 distant_enough = true;
	 for( UInt32 j = 0; j < used_positions.size(); ++j ) {
	    Real distance = (random_position - used_positions[j]).Length();
	    if( distance < m_fMinInitDistance ) {
	       distant_enough = false;
	       break;
	    }
	 }
      }
      
      MoveEntity( footbot.GetEmbodiedEntity(), random_position, random_rotation, false );
      used_positions.push_back( random_position );
   }
}

/****************************************/
/****************************************/


REGISTER_LOOP_FUNCTIONS ( CFlockingLoopFunctions, "flocking_lf" );
