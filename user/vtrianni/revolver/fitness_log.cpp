#include "fitness_log.h"

using namespace argos;

/****************************************/
/****************************************/

CFitnessLog::CFitnessLog( UInt32 un_sample_size, bool b_maximization, UInt32 un_num_iteration ) {
  m_unSampleSize = un_sample_size;  
  m_bMaximization = b_maximization;
  m_unCurrentIteration = un_num_iteration;
  m_pcCurrentBestFitness = new CFitnessData( m_unCurrentIteration, m_bMaximization );
  m_vecBestFitness.push_back( m_pcCurrentBestFitness );

  if( m_unSampleSize > 1 ) {
    m_pcCurrentAverageFitness = new CFitnessData( m_unCurrentIteration, m_bMaximization );
    m_vecAverageFitness.push_back( m_pcCurrentAverageFitness );
  }
  else {
    m_pcCurrentAverageFitness = NULL;
  }
}


/****************************************/
/****************************************/

CFitnessLog::~CFitnessLog() {
  for( UInt32 i = 0; i < m_vecBestFitness.size(); i++ ) {
    delete m_vecBestFitness[i];
  }

  if( m_unSampleSize > 1 ) {
    for( UInt32 i = 0; i < m_vecAverageFitness.size(); i++ ) {
      delete m_vecAverageFitness[i];
    }
  }
}


/****************************************/
/****************************************/

bool CFitnessLog::AddFitnessData( UInt32 un_iteration, Real fitness, UInt32 num_components, Real* components ) {
  if( un_iteration < m_unCurrentIteration ) {
    LOGERR << "[WARNING] iteration number smaller than current iteration in fitness log" << endl;
    return false;
  }

  // new geneartion - save old values and initialise new ones
  if( un_iteration > m_unCurrentIteration ) {
    m_unCurrentIteration = un_iteration;
    m_pcCurrentBestFitness = new CFitnessData( m_unCurrentIteration, m_bMaximization );
    m_vecBestFitness.push_back( m_pcCurrentBestFitness );

    if( m_unSampleSize > 1 ) {
      m_pcCurrentAverageFitness = new CFitnessData( m_unCurrentIteration, m_bMaximization );;
      m_vecAverageFitness.push_back( m_pcCurrentAverageFitness );
    }
  }


  // old iteration - check for a new best fitness, and update average
  bool bIsNewBest = m_bMaximization ? (fitness >= m_pcCurrentBestFitness->fitness) : (fitness <= m_pcCurrentBestFitness->fitness);
  if( bIsNewBest ) {
    m_pcCurrentBestFitness->fitness = fitness;
    for( UInt32 i = 0; i < num_components; i++ ) {
      if( m_pcCurrentBestFitness->components.size() <= i ) {
	m_pcCurrentBestFitness->components.push_back( components[i] );
      }
      else {
	m_pcCurrentBestFitness->components[i] = components[i];
      }
    }
  }


  if( m_unSampleSize > 1 ) {
    m_pcCurrentAverageFitness->fitness += fitness/m_unSampleSize;
    for( UInt32 i = 0; i < num_components; i++ ) {
      if( m_pcCurrentAverageFitness->components.size() <= i ) {
	m_pcCurrentAverageFitness->components.push_back( components[i]/m_unSampleSize );
      }
      else {
	m_pcCurrentAverageFitness->components[i] += components[i]/m_unSampleSize;
      }
    }
  }

  return bIsNewBest;
}


/****************************************/
/****************************************/

void CFitnessLog::Write( ostream& os ) {
  for( UInt32 i = 0; i < m_vecBestFitness.size(); i++ ) {
    os << m_vecBestFitness[i]->generation << " ";
    os << m_vecBestFitness[i]->fitness << " ";
    if( m_unSampleSize > 1 )
      os << m_vecAverageFitness[i]->fitness;

    for( UInt32 j = 0; j < m_vecBestFitness[i]->components.size(); j++ ) {
      os <<  " " << m_vecBestFitness[i]->components[j];
      if( m_unSampleSize > 1 )
	os <<  " " << m_vecAverageFitness[i]->components[j];
    }
    os << endl;
  }
}
