#include "obstacle_avoidance_lf.h"
#include "obstacle_avoidance_nn.h"

const string CONFIGURATION_OBSTACLEAVOIDANCE_INIT_RADIUS            = "init_radius";
const string CONFIGURATION_OBSTACLEAVOIDANCE_MAX_SPEED              = "max_speed";
const string CONFIGURATION_OBSTACLEAVOIDANCE_FITNESS_TYPE           = "fitness_type";
const string CONFIGURATION_OBSTACLEAVOIDANCE_FITNESS_WEIGHT         = "fitness_weight";


/****************************************/
/****************************************/

TInitPoints::TInitPoints( const Real& f_length, const CSegment& c_segment ) :
   m_fTotalLength(f_length),
   m_cInitSegment(c_segment) {
}

/****************************************/
/****************************************/

CObstacleAvoidanceLoopFunctions::CObstacleAvoidanceLoopFunctions() :
   CLoopFunctions() {

   m_tFitnessType            = T_SO_TRAD;
   m_fAvgWeight              = 0;

   m_fArenaSize              = 2;

   m_fInitLength             = 0;
   m_fInitRadius             = 0;

   m_fMaxSpeed               = 20;

   m_unNumSteps              = 0;
   m_fIndividualFitness      = 0;
   m_fSpeedComponent         = 0;
   m_fProximityComponent     = 0;

   m_fPositionOnCircuit      = 0;
   m_fCoveredLengthOnCircuit = 0;
}

  
/****************************************/
/****************************************/

CObstacleAvoidanceLoopFunctions::~CObstacleAvoidanceLoopFunctions() {
}


/****************************************/
/****************************************/

void CObstacleAvoidanceLoopFunctions::Init( TConfigurationNode& t_tree ) {
   ////////////////////////////////////////////////////////////////////////////////
   // Create a new random number generator for the loop function
   ////////////////////////////////////////////////////////////////////////////////
   m_pcRNG = CARGoSRandom::CreateRNG( "argos" );

   ////////////////////////////////////////////////////////////////////////////////
   // Paramteres relative to the fitness function calculation
   ////////////////////////////////////////////////////////////////////////////////
   string s_fitness_type = "";
   GetNodeAttribute(t_tree, CONFIGURATION_OBSTACLEAVOIDANCE_FITNESS_TYPE, s_fitness_type );
   if( s_fitness_type == "so_traditional" ) {
      m_tFitnessType = T_SO_TRAD;
   }
   else if( s_fitness_type == "so_circuiting" ) {
      m_tFitnessType = T_SO_CIRC;
   }
   else if( s_fitness_type == "so_weighted" ) {
      m_tFitnessType = T_SO_WGHT;
      GetNodeAttribute(t_tree, CONFIGURATION_OBSTACLEAVOIDANCE_FITNESS_WEIGHT, m_fAvgWeight );	 
   }
   else if( s_fitness_type == "multi_objective" ) {
      m_tFitnessType = T_MO;
   }
   else {
      THROW_ARGOSEXCEPTION("[FATAL] fitness function type not recognised");
   }


   ////////////////////////////////////////////////////////////////////////////////
   // Paramteres for creation and random positioning of the footbots
   ////////////////////////////////////////////////////////////////////////////////

   // get the arena dimension (and check for square geometry)
   CVector3 arena_dim = m_cSpace.GetArenaSize();
   if( arena_dim[0] != arena_dim[1] ) {
      THROW_ARGOSEXCEPTION("[FATAL] the arena is not square, it has different x and y sizes");
   }
   m_fArenaSize = arena_dim[0];

   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_OBSTACLEAVOIDANCE_INIT_RADIUS, m_fInitRadius, m_fInitRadius );
   GetNodeAttributeOrDefault(t_tree, CONFIGURATION_OBSTACLEAVOIDANCE_MAX_SPEED, m_fMaxSpeed, m_fMaxSpeed );

   // change max speed in m/s
   m_fMaxSpeed *= 0.01f;


   InitStartingPoints( t_tree );


   ////////////////////////////////////////////////////////////////////////////////
   // CREATION AND POSITIONING OF THE FOOTBOT: the robot is created
   // randomly in the arena, oround the initialisation segments
   // defined in the XML
   ////////////////////////////////////////////////////////////////////////////////

   // get random length in between [0,m_fInitLength] and get the
   // corresponding segment
   Real random_length = m_pcRNG->Uniform(CRange<Real>(0, m_fInitLength));      
   UInt32 segment;
   for( segment = 0; segment < m_vecInitPoints.size(); ++segment ) {
      if( random_length < m_vecInitPoints[segment].m_fTotalLength ) break;
   }
      
   Real f_segment_fraction = 1.0 - (m_vecInitPoints[segment].m_fTotalLength - random_length)/m_vecInitPoints[segment].m_cInitSegment.GetLength();
   CVector2 position_on_segment;
   m_vecInitPoints[segment].m_cInitSegment.GetPoint( position_on_segment, f_segment_fraction );
   CVector3 random_position = CVector3(position_on_segment.GetX() + m_pcRNG->Uniform(CRange<Real>(-m_fInitRadius,m_fInitRadius)), 
				       position_on_segment.GetY() + m_pcRNG->Uniform(CRange<Real>(-m_fInitRadius,m_fInitRadius)), 
				       0);
   CQuaternion random_rotation;
   random_rotation.FromEulerAngles( CRadians(m_pcRNG->Uniform(CRange<Real>(-CRadians::PI.GetValue(),CRadians::PI.GetValue()))), CRadians::ZERO, CRadians::ZERO );
   
   AddFootBot( "footbot", random_position, random_rotation, "nn", "dyn2d" );

   // store the initial position
   m_fPositionOnCircuit = random_length;
   m_fCoveredLengthOnCircuit = 0;

   // reset the individual fitness variables
   m_fIndividualFitness  = 0.0;
   m_fSpeedComponent     = 0.0;
   m_fProximityComponent = 0.0;

   // reset the number of steps counter
   m_unNumSteps = 0;
}


/****************************************/
/****************************************/

void CObstacleAvoidanceLoopFunctions::InitStartingPoints( TConfigurationNode& t_tree ) {
   // Get the base node for the starting points
   TConfigurationNode tStartingPoints;
   tStartingPoints = GetNode(t_tree, "starting_points");

   TConfigurationNodeIterator itInitPoints;
   m_fInitLength = 0;
   for( itInitPoints = itInitPoints.begin(&tStartingPoints); itInitPoints != itInitPoints.end(); ++itInitPoints ) {
      CVector2 c_start, c_end;
      GetNodeAttribute(*itInitPoints, "start", c_start);
      GetNodeAttribute(*itInitPoints, "end", c_end);

      LOG << "[INFO] Initialisation starting at " << c_start << " and ending at " << c_end << endl;
      CSegment starting_segment(c_start,c_end);
      m_fInitLength += starting_segment.GetLength();
      TInitPoints t_points( m_fInitLength, starting_segment );
      m_vecInitPoints.push_back( t_points );
   }
}


/****************************************/
/****************************************/

void CObstacleAvoidanceLoopFunctions::Reset( void ) {
   ////////////////////////////////////////////////////////////////////////////////
   // Resetting random positioning of the footbots
   ////////////////////////////////////////////////////////////////////////////////
   CFootBotEntity& footbot = dynamic_cast<CFootBotEntity&>(m_cSpace.GetEntity("footbot"));

   CVector3    random_position; 
   CQuaternion random_rotation;
   
   // get random length in between [0,m_fInitLength] and get the
   // corresponding segment
   Real random_length = m_pcRNG->Uniform(CRange<Real>(0, m_fInitLength));
   UInt32 segment;
   for( segment = 0; segment < m_vecInitPoints.size(); ++segment ) {
      if( random_length < m_vecInitPoints[segment].m_fTotalLength ) break;
   }
   
   Real f_segment_fraction = 1.0 - (m_vecInitPoints[segment].m_fTotalLength - random_length)/m_vecInitPoints[segment].m_cInitSegment.GetLength();
   CVector2 position_on_segment;
   m_vecInitPoints[segment].m_cInitSegment.GetPoint( position_on_segment, f_segment_fraction );
   
   random_position = CVector3(position_on_segment.GetX() + m_pcRNG->Uniform(CRange<Real>(-m_fInitRadius,m_fInitRadius)), 
			      position_on_segment.GetY() + m_pcRNG->Uniform(CRange<Real>(-m_fInitRadius,m_fInitRadius)), 
			      0);
   random_rotation.FromEulerAngles( CRadians(m_pcRNG->Uniform(CRange<Real>(-CRadians::PI.GetValue(),CRadians::PI.GetValue()))), CRadians::ZERO, CRadians::ZERO );
   
   MoveEntity( footbot.GetEmbodiedEntity(), random_position, random_rotation, false );

   // store the initial position
   m_fPositionOnCircuit = random_length;
   m_fCoveredLengthOnCircuit = 0;

   // reset the individual fitness variables
   m_fIndividualFitness  = 0.0;
   m_fSpeedComponent     = 0.0;
   m_fProximityComponent = 0.0;

   // reset the number of steps counter
   m_unNumSteps = 0;
}


/****************************************/
/****************************************/

void CObstacleAvoidanceLoopFunctions::Destroy( void ) {

}

/****************************************/
/****************************************/
void CObstacleAvoidanceLoopFunctions::PostPhysicsEngineStep( void ) {
   // increment the step counter
   m_unNumSteps += 1;
  
   CRange<Real> unit(0,1);

   CFootBotEntity& footbot = dynamic_cast<CFootBotEntity&>(m_cSpace.GetEntity("footbot"));

   // get proximity readings
   CCI_FootBotProximitySensor*  pc_proximity_sensors = (CCI_FootBotProximitySensor*)(footbot.GetControllableEntity().GetController().GetRobot().GetSensor( "footbot_proximity" ));
   CCI_FootBotProximitySensor::TReadings vec_proximity = pc_proximity_sensors->GetReadings();
   Real max_proximity = 0;
   for( UInt32 s = 0; s < vec_proximity.size(); s++ ) {
      max_proximity = max( max_proximity, vec_proximity[s].Value );
   }
   unit.TruncValue(max_proximity);
   
   // get wheels' speed
   Real wheels_speed[2] = {0, 0};
   footbot.GetWheeledEntity().GetSpeed(wheels_speed);
   
   Real delta_speed = 1.0 - sqrt(fabs(wheels_speed[0] - wheels_speed[1])/(2*m_fMaxSpeed));
   Real omega_speed = (fabs(wheels_speed[0]) + fabs(wheels_speed[1]))/(2*m_fMaxSpeed);
   unit.TruncValue(delta_speed);
   unit.TruncValue(omega_speed);
   

   m_fSpeedComponent     += delta_speed*omega_speed;
   m_fProximityComponent += (1 - max_proximity);
   m_fIndividualFitness  += delta_speed*omega_speed*(1 - max_proximity);
   //    LOG << " Speed:     " << fixed << setw(5) << setprecision(2) << m_fSpeedComponent/m_unNumSteps;
   //    LOG << " Proximity: " << fixed << setw(5) << setprecision(2) << m_fProximityComponent/m_unNumSteps;
   //    LOG << " Fitness:   " << fixed << setw(5) << setprecision(2) << m_fIndividualFitness/m_unNumSteps << endl;


   Real current_position = GetPositionOnCircuit( footbot.GetEmbodiedEntity().GetPosition().ProjectOntoXY() );
   Real increment = current_position - m_fPositionOnCircuit;
   if( fabs(increment) > m_fInitLength/2 ) {
      if( current_position < m_fPositionOnCircuit ) 
	 increment = m_fInitLength + increment;
      else 
	 increment = increment - m_fInitLength;
   }

   m_fCoveredLengthOnCircuit += increment;
   m_fPositionOnCircuit = current_position;
}


/****************************************/
/****************************************/

Real  CObstacleAvoidanceLoopFunctions::ComputePerformanceInExperiment( UInt32* pun_num_values, Real** pf_perf_values ) {
   *pun_num_values = 4;
   *pf_perf_values = new Real[4];
   (*pf_perf_values)[0] = m_fSpeedComponent/(Real)m_unNumSteps;
   (*pf_perf_values)[1] = m_fProximityComponent/(Real)m_unNumSteps;
   (*pf_perf_values)[2] = fabs(m_fCoveredLengthOnCircuit/m_fInitLength);
   (*pf_perf_values)[3] = m_fIndividualFitness/(Real)m_unNumSteps;

   Real f_fitness = 0;
   switch(m_tFitnessType) {
   case T_SO_TRAD:
   case T_MO:
      f_fitness = (*pf_perf_values)[3];
      break;
   case T_SO_CIRC:
      f_fitness = (*pf_perf_values)[2];
      break;
   case T_SO_WGHT:
      f_fitness = m_fAvgWeight*(*pf_perf_values)[0] + (1.0 - m_fAvgWeight)*(*pf_perf_values)[1];
      break;
   }
   return f_fitness;
}


/****************************************/
/****************************************/

CColor CObstacleAvoidanceLoopFunctions::GetFloorColor( const CVector2& c_position_on_plane ) {
   CVector2 c_closest_point, c_closest_segment_point;

   for( UInt32 i = 0; i < m_vecInitPoints.size(); i++ ) {
      m_vecInitPoints[i].m_cInitSegment.GetMinimumDistancePoints(c_position_on_plane, c_closest_point, c_closest_segment_point);
      if( (c_closest_segment_point - c_position_on_plane).Length() < 0.01 ) {
	 return CColor::BLACK;
      }
   }


   return CColor::WHITE;
}


/****************************************/
/****************************************/

Real CObstacleAvoidanceLoopFunctions::GetPositionOnCircuit( const CVector2& c_position ) {
   // get the position on the segment that is closest to the given position
   Real min_distance = 1e32; // initialised to a very high value
   CVector2 closest_point_on_circuit(0,0);
   UInt32   closest_segment_index = 0;

   for( UInt32 i = 0; i < m_vecInitPoints.size(); i++ ) {
      CVector2 closest_point, closest_seg_point;
      m_vecInitPoints[i].m_cInitSegment.GetMinimumDistancePoints(c_position, closest_point, closest_seg_point);
      Real distance = (closest_seg_point-c_position).Length();
      if( distance < min_distance ) {
	 min_distance = distance;
	 closest_point_on_circuit = closest_seg_point;
	 closest_segment_index = i;
      }
   }

   Real previous_length = 0;
   if( closest_segment_index > 0 ) {
      previous_length = m_vecInitPoints[closest_segment_index-1].m_fTotalLength;
   }
   return previous_length + (closest_point_on_circuit - m_vecInitPoints[closest_segment_index].m_cInitSegment.GetStart()).Length();
}


/****************************************/
/****************************************/

REGISTER_LOOP_FUNCTIONS ( CObstacleAvoidanceLoopFunctions, "obstacle_avoidance_lf" );
