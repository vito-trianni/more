#include "stick_pulling_lf.h"
#include "stick_pulling_nn.h"

const string CONFIGURATION_STICKPULLING_MAX_INIT_TRIALS        = "max_init_trials";
const string CONFIGURATION_STICKPULLING_NUM_ROBOTS             = "num_robots";
const string CONFIGURATION_STICKPULLING_MIN_INIT_DISTANCE      = "min_init_distance";
const string CONFIGURATION_STICKPULLING_NUM_ARENA_WALLS        = "num_arena_walls";
const string CONFIGURATION_STICKPULLING_MAX_ROBOT_SPEED        = "max_robot_speed";
const string CONFIGURATION_STICKPULLING_NUM_STICKS             = "num_sticks";
const string CONFIGURATION_STICKPULLING_STICK_HEIGHT           = "stick_height";
const string CONFIGURATION_STICKPULLING_STICK_RADIUS           = "stick_radius";
const string CONFIGURATION_STICKPULLING_STICK_PULLING_RADIUS   = "stick_pulling_radius";
const string CONFIGURATION_STICKPULLING_MIN_PULLING_ROBOTS     = "min_pulling_robots";

static const Real FOOTBOT_RADIUS = 0.1;

/****************************************/
/****************************************/

CStickPullingLoopFunctions::CStickPullingLoopFunctions() :
   CEvolutionaryLoopFunctions(),
   m_unMaxInitTrials(1000),
   m_unNumInitFootbots( 0 ),
   m_fMinInitDistance(0.25),
   m_fArenaRadius(2.0),
   m_unNumArenaWalls(12),
   m_fMaxRobotSpeed(10),
   m_unNumSticks( 0 ),
   m_fStickHeight( 0.15 ),
   m_fStickRadius( 0.05 ),
   m_fStickPullingRadius( 0.25 ),
   m_unMinPullingRobots( 2 ),
   m_unNumPulledSticks( 0 ),
   m_bCollisionDetected( false ),
   m_punStickVisitedPerRobot( NULL )
{
}

  
/****************************************/
/****************************************/

CStickPullingLoopFunctions::~CStickPullingLoopFunctions() {
   if( m_punStickVisitedPerRobot != NULL ) {
      for( UInt32 i = 0; i < m_unNumInitFootbots; ++i ) {
	 delete [] m_punStickVisitedPerRobot[i];
      }
      delete [] m_punStickVisitedPerRobot;
   }
}


/****************************************/
/****************************************/

void CStickPullingLoopFunctions::Init( TConfigurationNode& t_tree ) {
   ////////////////////////////////////////////////////////////////////////////////
   // Create a new random number generator for the loop function
   ////////////////////////////////////////////////////////////////////////////////
   m_pcRNG = CARGoSRandom::CreateRNG( "argos" );

   ////////////////////////////////////////////////////////////////////////////////
   // Paramteres for creation and random positioning of the footbots and sticks
   ////////////////////////////////////////////////////////////////////////////////

   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_MAX_INIT_TRIALS, m_unMaxInitTrials, m_unMaxInitTrials );
   GetNodeAttribute(t_tree, CONFIGURATION_STICKPULLING_NUM_ROBOTS, m_unNumInitFootbots );
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_MIN_INIT_DISTANCE, m_fMinInitDistance, m_fMinInitDistance );
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_NUM_ARENA_WALLS, m_unNumArenaWalls, m_unNumArenaWalls );
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_MAX_ROBOT_SPEED, m_fMaxRobotSpeed, m_fMaxRobotSpeed );
   GetNodeAttribute(t_tree, CONFIGURATION_STICKPULLING_NUM_STICKS, m_unNumSticks );
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_STICK_HEIGHT, m_fStickHeight, m_fStickHeight );
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_STICK_RADIUS, m_fStickRadius, m_fStickRadius );
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_STICK_PULLING_RADIUS, m_fStickPullingRadius, m_fStickPullingRadius );
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_STICKPULLING_MIN_PULLING_ROBOTS, m_unMinPullingRobots, m_unMinPullingRobots );

   ////////////////////////////////////////////////////////////////////////////////
   // initialise the variables for computing the fitness
   ////////////////////////////////////////////////////////////////////////////////
   m_punStickVisitedPerRobot = new UInt32*[m_unNumInitFootbots];
   for( UInt32 i = 0; i < m_unNumInitFootbots; ++i ) {
      m_punStickVisitedPerRobot[i] = new UInt32[m_unNumSticks];
   }

   ostringstream entity_id;

   ////////////////////////////////////////////////////////////////////////////////
   // CREATION AND POSITIONING OF THE ARENA WALLS
   ////////////////////////////////////////////////////////////////////////////////
   CVector3 arena_size = m_cSpace.GetArenaSize();
   m_fArenaRadius = Min(arena_size[0],arena_size[1])/2;

   CRadians wall_angle = CRadians::TWO_PI/m_unNumArenaWalls;
   CVector3 wall_size(0.02, 2.0*m_fArenaRadius*Tan(CRadians::PI/m_unNumArenaWalls), 0.2);
   std::vector<std::string> vec_engines;
   vec_engines.push_back( "ground" );
   vector<SAdditionalLED> vec_additional_leds;
   for( UInt32 i = 0; i < m_unNumArenaWalls; i++ ) {
      entity_id.str("");
      entity_id << "wall_" << i;

      CRadians wall_rotation = wall_angle*i;
      CVector3 wall_position( m_fArenaRadius*Cos(wall_rotation), m_fArenaRadius*Sin(wall_rotation), 0.1 );
      CQuaternion wall_orientation;
      wall_orientation.FromEulerAngles( wall_rotation,  CRadians::ZERO, CRadians::ZERO );
      
      AddBox( entity_id.str(), wall_position, wall_orientation, wall_size, false, (Real)1.0, vec_engines );
   }


   ////////////////////////////////////////////////////////////////////////////////
   // CREATION OF THE STICKS
   ////////////////////////////////////////////////////////////////////////////////
   m_cEatenStickPosition = CVector3( arena_size.GetX()/2 + 2, arena_size.GetY()/2 + 0, m_fStickHeight/2 );
   for( UInt32 i = 0; i < m_unNumSticks; ++i ) {
      entity_id.str("");
      entity_id << "stick_" << i;
      CVector3 led_position = CVector3(0,0, m_fStickHeight/2+m_fStickRadius);
      vector<SAdditionalLED> vec_additional_leds;
      vec_additional_leds.push_back( SAdditionalLED(led_position, CColor::BLACK) );
      AddCylinder( entity_id.str(), m_cEatenStickPosition, CQuaternion(), m_fStickRadius, m_fStickHeight, false, 1, vec_engines, vec_additional_leds, true );
      m_cEatenStickPosition -= CVector3(0,m_fStickRadius*3,0);
   }
   try {
      m_tStickMap = m_cSpace.GetEntitiesByType("cylinder_entity");
   }
   catch (CARGoSException& ex) { 
      LOGERR << "continuing without stick to pull!" << endl;
   }


   ////////////////////////////////////////////////////////////////////////////////
   // CREATION OF THE ROBOTS
   ////////////////////////////////////////////////////////////////////////////////
   CVector3 c_init_position(1000,1000,0);
   for( UInt32 i = 0; i < m_unNumInitFootbots; ++i ) {
      entity_id.str("");
      entity_id << "footbot_" << i;
      AddFootBot( entity_id.str(), c_init_position + CVector3(i,0,0), CQuaternion(), "nn", "ground" );
   }
   m_tFootBotMap = m_cSpace.GetEntitiesByType("footbot_entity");
   
   // set the experiment
   while( !SetExperiment() ) {
      LOGERR << "[WARNING] max init trials exceeded: repeating initialisation" << endl;
   }
}


/****************************************/
/****************************************/

void CStickPullingLoopFunctions::Reset( void ) {
   CEvolutionaryLoopFunctions::Reset();

   // Reset the experiment
   while( !SetExperiment() ) {
      LOGERR << "[WARNING] max init trials exceeded: repeating initialisation" << endl;
   }
}


/****************************************/
/****************************************/

void CStickPullingLoopFunctions::Destroy( void ) {
}

/****************************************/
/****************************************/

void CStickPullingLoopFunctions::PostPhysicsEngineStep( void ) {
   // Computation of metrics
   UInt32 j = 0;
   for( CSpace::TAnyEntityMap::iterator stick_it = m_tStickMap.begin(); stick_it != m_tStickMap.end(); ++stick_it, ++j ) {
      CCylinderEntity& c_stick = *(any_cast<CCylinderEntity*>(stick_it->second));
      // skip the sticks that are already switched off
      if( c_stick.GetLEDEquippedEntity().GetLED(0).GetColor() == CColor::BLACK ) continue;


      CVector2 c_stick_position = c_stick.GetEmbodiedEntity().GetPosition().ProjectOntoXY();
      UInt32 un_footbot_count = 0;
      UInt32 i = 0;
      for( CSpace::TAnyEntityMap::iterator footbot_it = m_tFootBotMap.begin(); footbot_it != m_tFootBotMap.end(); ++footbot_it, ++i ) {
	 CFootBotEntity& c_footbot = *(any_cast<CFootBotEntity*>(footbot_it->second));
	 CVector2 c_footbot_position = c_footbot.GetEmbodiedEntity().GetPosition().ProjectOntoXY();
	 
	 if( (c_footbot_position - c_stick_position).Length() < m_fStickPullingRadius ) {
	    un_footbot_count += 1;
	    m_punStickVisitedPerRobot[i][j] = 1;

	    if( un_footbot_count >= m_unMinPullingRobots ) {
	       // switch off the LED of the stick
	       c_stick.GetLEDEquippedEntity().SetLedColor( 0, CColor::BLACK );
	       m_unNumPulledSticks += 1;
	       break;
	    }
	 }
      }
   }


   for( CSpace::TAnyEntityMap::iterator footbot_it = m_tFootBotMap.begin(); footbot_it != m_tFootBotMap.end(); ++footbot_it ) {
      CFootBotEntity& c_footbot = *(any_cast<CFootBotEntity*>(footbot_it->second));
      // check collisions
      m_bCollisionDetected = m_bCollisionDetected || c_footbot.GetEmbodiedEntity().IsCollisionDetected();
   }

}


/****************************************/
/****************************************/

Real CStickPullingLoopFunctions::ComputePerformanceInExperiment( UInt32* pun_num_values, Real** pf_perf_values ) {
   // compute the average number of visits per robot
   Real f_avg_num_visits = 0;
   for( UInt32 i = 0; i < m_unNumInitFootbots; ++i ) {
      UInt32 num_visits = 0;
      for( UInt32 j = 0; j < m_unNumSticks; ++j ) {
	 num_visits += m_punStickVisitedPerRobot[i][j];
      }
      f_avg_num_visits += (Real)num_visits/m_unNumSticks;
   }
   f_avg_num_visits /= m_unNumInitFootbots;



   Real f_pulled_sticks = m_bCollisionDetected ? 0 : (Real)m_unNumPulledSticks/(Real)m_unNumSticks;

   // set the two fitness components
   *pun_num_values = 2;
   *pf_perf_values = new Real[2];
   (*pf_perf_values)[0] = f_pulled_sticks;
   (*pf_perf_values)[1] = f_avg_num_visits;

   return f_pulled_sticks;
}


/****************************************/
/****************************************/

CColor CStickPullingLoopFunctions::GetFloorColor( const CVector2& c_position_on_plane ) {
   for( CSpace::TAnyEntityMap::iterator it = m_tStickMap.begin(); it != m_tStickMap.end(); ++it ) {
      CCylinderEntity& stick = *(any_cast<CCylinderEntity*>(it->second));
      CVector2 c_stick_position = stick.GetEmbodiedEntity().GetPosition().ProjectOntoXY();

      if( (c_position_on_plane - c_stick_position).Length() < m_fStickPullingRadius ) {
	 return CColor::GRAY80;
      }
   }

   return CColor::WHITE;
}



/****************************************/
/****************************************/

bool CStickPullingLoopFunctions::SetExperiment( void ) {
   vector<CVector3> vec_used_positions;
   ////////////////////////////////////////////////////////////////////////////////
   // POSITION OF THE STICKS
   ////////////////////////////////////////////////////////////////////////////////
   m_cEatenStickPosition = CVector3( m_fArenaRadius + 2,
				    m_fArenaRadius  + 0,
				    m_fStickHeight/2 );
   
   for( CSpace::TAnyEntityMap::iterator it = m_tStickMap.begin(); it != m_tStickMap.end(); ++it ) {
      CCylinderEntity& c_stick = *(any_cast<CCylinderEntity*>(it->second));
      
      CVector3    random_position;
      CQuaternion null_rotation;
      bool        distant_enough = false; 
      UInt32      un_init_trials = 0;
      while( !distant_enough && (++un_init_trials < m_unMaxInitTrials) ) {
	 Real rho   = m_pcRNG->Uniform(CRange<Real>(0,m_fArenaRadius-2*(m_fStickRadius+FOOTBOT_RADIUS)));
	 Real theta = m_pcRNG->Uniform(CRange<Real>(-CRadians::PI.GetValue(),CRadians::PI.GetValue()));
	 random_position = CVector3(rho*cos(theta), rho*sin(theta),0.0);
      
      	 // check distance from other sticks
	 distant_enough = true;
	 for( UInt32 j = 0; j < vec_used_positions.size(); ++j ) {
	    Real distance = (random_position - vec_used_positions[j]).Length();
	    if( distance < m_fMinInitDistance ) {
	       distant_enough = false;
	       break;
	    }
	 }
      }
      
      if( un_init_trials < m_unMaxInitTrials ) {
	 MoveEntity( c_stick.GetEmbodiedEntity(), random_position, null_rotation, false );
	 c_stick.GetLEDEquippedEntity().SetLedColor( 0, CColor::RED );
	 vec_used_positions.push_back( random_position );
      }
      else {
	 return false;
      }
   }



   ////////////////////////////////////////////////////////////////////////////////
   // RANDOM POSITIONING OF THE FOOTBOTS
   ////////////////////////////////////////////////////////////////////////////////
   UInt32 i = 0;
   for( CSpace::TAnyEntityMap::iterator it = m_tFootBotMap.begin(); it != m_tFootBotMap.end(); ++it, ++i ) {
      CFootBotEntity& c_footbot = *(any_cast<CFootBotEntity*>(it->second));

      CVector3    random_position;
      CQuaternion random_rotation;
      bool        distant_enough = false; 
      UInt32      un_init_trials = 0;
      while( !distant_enough && (++un_init_trials < m_unMaxInitTrials) ) {
	 Real rho   = m_pcRNG->Uniform(CRange<Real>(0,m_fArenaRadius-FOOTBOT_RADIUS));
	 Real theta = m_pcRNG->Uniform(CRange<Real>(-CRadians::PI.GetValue(),CRadians::PI.GetValue()));
	 random_position = CVector3(rho*cos(theta), rho*sin(theta),0.0);
	 random_rotation.FromEulerAngles( CRadians(m_pcRNG->Uniform(CRange<Real>(-CRadians::PI.GetValue(),CRadians::PI.GetValue()))), 
					  CRadians::ZERO, 
					  CRadians::ZERO );
	 
      	 // check distance from other footbots
	 distant_enough = true;
	 for( UInt32 j = 0; j < vec_used_positions.size(); ++j ) {
	    Real distance = (random_position - vec_used_positions[j]).Length();
	    if( distance < m_fMinInitDistance ) {
	       distant_enough = false;
	       break;
	    }
	 }
      }

      if( un_init_trials < m_unMaxInitTrials ) {
	 MoveEntity( c_footbot.GetEmbodiedEntity(), random_position, random_rotation, false );
	 vec_used_positions.push_back( random_position );
      }
      else {
	 return false;
      }
   }
   

   ////////////////////////////////////////////////////////////////////////////////
   // Reset fitness variables
   ////////////////////////////////////////////////////////////////////////////////
   m_unNumPulledSticks = 0;
   m_bCollisionDetected = false;
   
   for( UInt32 i = 0; i < m_unNumInitFootbots; ++i ) {
      for( UInt32 j = 0; j < m_unNumSticks; ++j ) {
	 m_punStickVisitedPerRobot[i][j] = 0;
      }
   }

   return true;
}


/****************************************/
/****************************************/




REGISTER_LOOP_FUNCTIONS ( CStickPullingLoopFunctions, "stick_pulling_lf" );
