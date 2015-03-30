/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file <argos2/simulator/space/space_multi_thread_h_dispatch.cpp>
 *
 * @brief This file provides the implementation of the Swarmanoid 3D Space.
 *
 * This file provides the implementation of the Swarmanoid 3D Space.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#include "space_multi_thread_h_dispatch.h"
#include <argos2/simulator/simulator.h>
#include <argos2/common/utility/profiler/profiler.h>

#include <cstdio>

namespace argos {

   /****************************************/
   /****************************************/

   struct SCleanupThreadData {
      pthread_mutex_t* StartSenseControlPhaseMutex;
      pthread_mutex_t* StartActPhaseMutex;
      pthread_mutex_t* StartPhysicsPhaseMutex;
      pthread_mutex_t* FetchTaskMutex;
   };

   static void CleanupThread(void* p_data) {
      CSimulator& cSimulator = CSimulator::GetInstance();
      if(cSimulator.IsProfiling()) {
         cSimulator.GetProfiler().CollectThreadResourceUsage();
      }
      SCleanupThreadData& sData =
         *reinterpret_cast<SCleanupThreadData*>(p_data);
      pthread_mutex_unlock(sData.FetchTaskMutex);
      pthread_mutex_unlock(sData.StartSenseControlPhaseMutex);
      pthread_mutex_unlock(sData.StartActPhaseMutex);
      pthread_mutex_unlock(sData.StartPhysicsPhaseMutex);
   }

   void* LaunchThreadHDispatch(void* p_data) {
      /* Set up thread-safe buffers for this new thread */
      LOG.AddThreadSafeBuffer();
      LOGERR.AddThreadSafeBuffer();
      /* Make this thread cancellable */
      pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
      pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
      /* Get a handle to the thread launch data */
      CSpaceMultiThreadHDispatch::SThreadLaunchData* psData = reinterpret_cast<CSpaceMultiThreadHDispatch::SThreadLaunchData*>(p_data);
      /* Create cancellation data */
      SCleanupThreadData sCancelData;
      sCancelData.StartSenseControlPhaseMutex = &(psData->Space->m_tStartSenseControlPhaseMutex);
      sCancelData.StartActPhaseMutex = &(psData->Space->m_tStartActPhaseMutex);
      sCancelData.StartPhysicsPhaseMutex = &(psData->Space->m_tStartPhysicsPhaseMutex);
      sCancelData.FetchTaskMutex = &(psData->Space->m_tFetchTaskMutex);
      pthread_cleanup_push(CleanupThread, &sCancelData);
      if(psData->ThreadId == 0) {
         /* Execute the code for the dispatch thread */
         psData->Space->DispatchThread(psData->ThreadId);
      }
      else {
         /* Execute the code for a slave thread */
         psData->Space->SlaveThread(psData->ThreadId);
      }
      /* Dispose of cancellation data */
      pthread_cleanup_pop(1);
      return NULL;
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadHDispatch::STaskData::Reset() {
      Index = 0;
      Used = false;
      Done = false;
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadHDispatch::Init(TConfigurationNode& t_tree) {
      /* Initialize the space */
      CSpace::Init(t_tree);

      /* Initialize thread related structures */
      int nErrors;
      /* Init mutexes */
      if((nErrors = pthread_mutex_init(&m_tStartSenseControlPhaseMutex, NULL)) ||
         (nErrors = pthread_mutex_init(&m_tStartActPhaseMutex, NULL)) ||
         (nErrors = pthread_mutex_init(&m_tStartPhysicsPhaseMutex, NULL)) ||
         (nErrors = pthread_mutex_init(&m_tFetchTaskMutex, NULL))) {
         THROW_ARGOSEXCEPTION("Error creating thread mutexes " << ::strerror(nErrors));
      }
      /* Init conditionals */
      if((nErrors = pthread_cond_init(&m_tStartSenseControlPhaseCond, NULL)) ||
         (nErrors = pthread_cond_init(&m_tStartActPhaseCond, NULL)) ||
         (nErrors = pthread_cond_init(&m_tStartPhysicsPhaseCond, NULL)) ||
         (nErrors = pthread_cond_init(&m_tFetchTaskCond, NULL))) {
         THROW_ARGOSEXCEPTION("Error creating thread conditionals " << ::strerror(nErrors));
      }

      /* Reset the idle thread count */
      m_unSenseControlPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
      m_unActPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
      m_unPhysicsPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadHDispatch::Destroy() {
      /* Destroy the threads to update the controllable entities */
      int nErrors;
      if(m_ptThreads != NULL) {
         for(UInt32 i = 0; i <= CSimulator::GetInstance().GetNumThreads(); ++i) {
            if((nErrors = pthread_cancel(m_ptThreads[i]))) {
               THROW_ARGOSEXCEPTION("Error canceling threads " << ::strerror(nErrors));
            }
         }
         void** ppJoinResult = new void*[CSimulator::GetInstance().GetNumThreads()+1];
         for(UInt32 i = 0; i <= CSimulator::GetInstance().GetNumThreads(); ++i) {
            if((nErrors = pthread_join(m_ptThreads[i], ppJoinResult + i))) {
               THROW_ARGOSEXCEPTION("Error joining threads " << ::strerror(nErrors));
            }
            if(ppJoinResult[i] != PTHREAD_CANCELED) {
               LOGERR << "[WARNING] Thread #" << i<< " not canceled" << std::endl;
            }
         }
         delete[] ppJoinResult;
      }
      delete[] m_ptThreads;
      /* Destroy the thread launch info */
      if(m_psThreadData != NULL) {
         for(UInt32 i = 0; i <= CSimulator::GetInstance().GetNumThreads(); ++i) {
            delete m_psThreadData[i];
         }
      }
      delete[] m_psThreadData;
      pthread_mutex_destroy(&m_tStartSenseControlPhaseMutex);
      pthread_mutex_destroy(&m_tStartActPhaseMutex);
      pthread_mutex_destroy(&m_tStartPhysicsPhaseMutex);
      pthread_mutex_destroy(&m_tFetchTaskMutex);
      pthread_cond_destroy(&m_tStartSenseControlPhaseCond);
      pthread_cond_destroy(&m_tStartActPhaseCond);
      pthread_cond_destroy(&m_tStartPhysicsPhaseCond);
      pthread_cond_destroy(&m_tFetchTaskCond);

      /* Destroy the base space */
      CSpace::Destroy();
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadHDispatch::SetPhysicsEngines(TPhysicsEngineList& t_engines) {
      CSpace::SetPhysicsEngines(t_engines);
      StartThreads();
   }

   /****************************************/
   /****************************************/

#define MAIN_START_PHASE(PHASE)                             \
   pthread_mutex_lock(&m_tStart ## PHASE ## PhaseMutex);    \
   m_un ## PHASE ## PhaseIdleCounter = 0;                   \
   m_sTaskData.Reset();                                     \
   pthread_cond_broadcast(&m_tStart ## PHASE ## PhaseCond); \
   pthread_mutex_unlock(&m_tStart ## PHASE ## PhaseMutex);

#define MAIN_WAIT_FOR_END_OF(PHASE)                                                         \
   pthread_mutex_lock(&m_tStart ## PHASE ## PhaseMutex);                                    \
   while(m_un ## PHASE ## PhaseIdleCounter < CSimulator::GetInstance().GetNumThreads()) {   \
      pthread_cond_wait(&m_tStart ## PHASE ## PhaseCond, &m_tStart ## PHASE ## PhaseMutex); \
   }                                                                                        \
   pthread_mutex_unlock(&m_tStart ## PHASE ## PhaseMutex);

   void CSpaceMultiThreadHDispatch::UpdateControllableEntities() {
      /* Reset the idle thread count */
      m_unSenseControlPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
      m_unActPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
      m_unPhysicsPhaseIdleCounter = CSimulator::GetInstance().GetNumThreads();
      /* Sense/control phase */
      MAIN_START_PHASE(SenseControl);
      MAIN_WAIT_FOR_END_OF(SenseControl);
      /* Act phase */
      MAIN_START_PHASE(Act);
      MAIN_WAIT_FOR_END_OF(Act);
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadHDispatch::UpdatePhysics() {
      /* Physics phase */
      MAIN_START_PHASE(Physics);
      MAIN_WAIT_FOR_END_OF(Physics);
      /* Perform entity transfer from engine to engine, if needed */
      for(size_t i = 0; i < m_ptPhysicsEngines->size(); ++i) {
         if((*m_ptPhysicsEngines)[i]->IsEntityTransferNeeded()) {
            (*m_ptPhysicsEngines)[i]->TransferEntities();
         }
      }
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadHDispatch::StartThreads() {
      int nErrors;
      /* Create the threads to update the controllable entities */
      m_ptThreads = new pthread_t[CSimulator::GetInstance().GetNumThreads() + 1];
      m_psThreadData = new SThreadLaunchData*[CSimulator::GetInstance().GetNumThreads() + 1];
      for(UInt32 i = 0; i <= CSimulator::GetInstance().GetNumThreads(); ++i) {
         /* Create the struct with the info to launch the thread */
         m_psThreadData[i] = new SThreadLaunchData(i, this);
         /* Create the thread */
         if((nErrors = pthread_create(m_ptThreads + i,
                                      NULL,
                                      LaunchThreadHDispatch,
                                      reinterpret_cast<void*>(m_psThreadData[i])))) {
            THROW_ARGOSEXCEPTION("Error creating thread: " << ::strerror(nErrors));
         }
      }
   }

   /****************************************/
   /****************************************/

#define THREAD_WAIT_FOR_START_OF(PHASE)                                                     \
   pthread_mutex_lock(&m_tStart ## PHASE ## PhaseMutex);                                    \
   while(m_un ## PHASE ## PhaseIdleCounter == CSimulator::GetInstance().GetNumThreads()) {  \
      pthread_cond_wait(&m_tStart ## PHASE ## PhaseCond, &m_tStart ## PHASE ## PhaseMutex); \
   }                                                                                        \
   pthread_mutex_unlock(&m_tStart ## PHASE ## PhaseMutex);                                  \
   pthread_testcancel();

#define THREAD_DISPATCH_TASK(TASKVEC)                                 \
   if(! (TASKVEC).empty()) {                                          \
      while(! m_sTaskData.Done) {                                     \
         pthread_mutex_lock(&m_tFetchTaskMutex);                      \
         while(! m_sTaskData.Used) {                                  \
            pthread_cond_wait(&m_tFetchTaskCond, &m_tFetchTaskMutex); \
         }                                                            \
         ++m_sTaskData.Index;                                         \
         if(m_sTaskData.Index < (TASKVEC).size()) {                   \
            m_sTaskData.Used = false;                                 \
         }                                                            \
         else {                                                       \
            m_sTaskData.Done = true;                                  \
         }                                                            \
         pthread_cond_broadcast(&m_tFetchTaskCond);                   \
         pthread_mutex_unlock(&m_tFetchTaskMutex);                    \
         pthread_testcancel();                                        \
      }                                                               \
   }                                                                  \
   else {                                                             \
      m_sTaskData.Done = true;                                        \
      pthread_mutex_unlock(&m_tFetchTaskMutex);                       \
      pthread_testcancel();                                           \
   }

   /****************************************/
   /****************************************/

   void CSpaceMultiThreadHDispatch::DispatchThread(UInt32 un_id) {
      while(1) {
         THREAD_WAIT_FOR_START_OF(SenseControl);
         THREAD_DISPATCH_TASK(m_vecControllableEntities);
         THREAD_WAIT_FOR_START_OF(Act);
         THREAD_DISPATCH_TASK(m_vecControllableEntities);
         THREAD_WAIT_FOR_START_OF(Physics);
         THREAD_DISPATCH_TASK(*m_ptPhysicsEngines);
      }
   }

   /****************************************/
   /****************************************/

#define THREAD_PERFORM_TASK(PHASE, SNIPPET)                         \
   while(1) {                                                       \
      pthread_mutex_lock(&m_tFetchTaskMutex);                       \
      while(m_sTaskData.Used && ! m_sTaskData.Done) {               \
         pthread_cond_wait(&m_tFetchTaskCond, &m_tFetchTaskMutex);  \
      }                                                             \
      if(! m_sTaskData.Done) {                                      \
         unTaskIndex = m_sTaskData.Index;                           \
         m_sTaskData.Used = true;                                   \
         pthread_cond_broadcast(&m_tFetchTaskCond);                 \
         pthread_mutex_unlock(&m_tFetchTaskMutex);                  \
         pthread_testcancel();                                      \
         {                                                          \
            SNIPPET;                                                \
         }                                                          \
         pthread_testcancel();                                      \
      }                                                             \
      else {                                                        \
         pthread_mutex_unlock(&m_tFetchTaskMutex);                  \
         pthread_testcancel();                                      \
         pthread_mutex_lock(&m_tStart ## PHASE ## PhaseMutex);      \
         ++m_un ## PHASE ## PhaseIdleCounter;                       \
         pthread_cond_broadcast(&m_tStart ## PHASE ## PhaseCond);   \
         pthread_mutex_unlock(&m_tStart ## PHASE ## PhaseMutex);    \
         pthread_testcancel();                                      \
         break;                                                     \
      }                                                             \
   }                                                                \
   pthread_testcancel();

   void CSpaceMultiThreadHDispatch::SlaveThread(UInt32 un_id) {
      /* Task index */
      size_t unTaskIndex;
      while(1) {
         THREAD_WAIT_FOR_START_OF(SenseControl);
         THREAD_PERFORM_TASK(
            SenseControl,
            m_vecControllableEntities[unTaskIndex]->Sense();
            m_vecControllableEntities[unTaskIndex]->ControlStep();
            );
         THREAD_WAIT_FOR_START_OF(Act);
         THREAD_PERFORM_TASK(
            Act,
            m_vecControllableEntities[unTaskIndex]->Act();
            );
         THREAD_WAIT_FOR_START_OF(Physics);
         THREAD_PERFORM_TASK(
            Physics,
            (*m_ptPhysicsEngines)[unTaskIndex]->Update();
            );
      }
   }

   /****************************************/
   /****************************************/

}
