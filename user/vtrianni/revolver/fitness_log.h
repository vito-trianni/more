/* -*- Mode: C++ -*- */

#ifndef FITNESSLOG_H
#define FITNESSLOG_H

using namespace std;

#include "evolution.h" 

namespace argos {

  class CFitnessLog {
  private:

    UInt32          m_unCurrentIteration;
    CFitnessData*   m_pcCurrentBestFitness;
    CFitnessData*   m_pcCurrentAverageFitness;

    UInt32          m_unSampleSize;
    bool            m_bMaximization;

    TVecFitnessData m_vecBestFitness;
    TVecFitnessData m_vecAverageFitness;
  
  public:
    CFitnessLog( UInt32 un_sample_size, bool b_maximisation, UInt32 un_num_iteration = 0 );
    ~CFitnessLog();

    inline Real     GetCurrentBestFitness( void ) { return m_pcCurrentBestFitness->fitness; };
    inline Real     GetCurrentAverageFitness( void ) { return m_pcCurrentAverageFitness->fitness; };

    bool            AddFitnessData( UInt32 un_iteration, Real fitness, UInt32 num_components = 0, Real* components = NULL );
    void            Write( ostream& os );
  };

};

#endif
