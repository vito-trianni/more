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
 * @file <argos2/simulator/space/space_multi_thread_h_dispatch.h>
 *
 * @brief This file provides the definition of the Swarmanoid 3D Space.
 *
 * This file provides the definition of the Swarmanoid 3D Space.
 *
 * @author Carlo Pinciroli - <cpinciro@ulb.ac.be>
 */

#ifndef SPACE_MULTI_THREAD_H_DISPATCH_H
#define SPACE_MULTI_THREAD_H_DISPATCH_H

namespace argos {
   class CSpace;
}

#include <argos2/simulator/space/space.h>

namespace argos {

   class CSpaceMultiThreadHDispatch : public CSpace {

   public:

      CSpaceMultiThreadHDispatch() {}
      virtual ~CSpaceMultiThreadHDispatch() {}

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Destroy();

      virtual void SetPhysicsEngines(TPhysicsEngineList& t_engines);

      virtual void UpdateControllableEntities();
      virtual void UpdatePhysics();

   private:

      void StartThreads();
      void DispatchThread(UInt32 un_id);
      void SlaveThread(UInt32 un_id);
      friend void* LaunchThreadHDispatch(void* p_data);

   private:

      /** Thread date */
      struct SThreadLaunchData {
         UInt32 ThreadId;
         CSpaceMultiThreadHDispatch* Space;
         
         SThreadLaunchData(UInt32 un_thread_id,
                           CSpaceMultiThreadHDispatch* pc_space) :
            ThreadId(un_thread_id),
            Space(pc_space) {}
      };

      /** Data associated to a task to be executed */
      struct STaskData {
         /** All tasks in arrays. This is the current array index. */
         size_t Index;
         /** True when the current index was used already */
         bool Used;
         /** True when there are no more tasks to perform */
         bool Done;

         /** Resets the task data */
         void Reset();
      };

      /** The thread array. Thread #0 is the dispatch one and the others are the slave threads */
      pthread_t* m_ptThreads;

      /** Data structure needed to launch the threads */
      SThreadLaunchData** m_psThreadData;

      /** Data structure needed to coordinate the tasks */
      STaskData m_sTaskData;

      /** Mutex for the start of the sense/control phase */
      pthread_mutex_t m_tStartSenseControlPhaseMutex;
      /** Mutex for the start of the act phase */
      pthread_mutex_t m_tStartActPhaseMutex;
      /** Mutex for the start of the physics phase */
      pthread_mutex_t m_tStartPhysicsPhaseMutex;
      /** Mutex to fetch a task from the dispatcher */
      pthread_mutex_t m_tFetchTaskMutex;

      /** Conditional for the start of the sense/control phase */
      pthread_cond_t m_tStartSenseControlPhaseCond;
      /** Conditional for the start of the act phase */
      pthread_cond_t m_tStartActPhaseCond;
      /** Conditional for the start of the physics phase */
      pthread_cond_t m_tStartPhysicsPhaseCond;
      /** Conditional controlling task fetching from the dispatcher */
      pthread_cond_t m_tFetchTaskCond;

      /** How many threads are idle in the sense/control phase */
      UInt32 m_unSenseControlPhaseIdleCounter;
      /** How many threads are idle in the act phase */
      UInt32 m_unActPhaseIdleCounter;
      /** How many threads are idle in the physics phase */
      UInt32 m_unPhysicsPhaseIdleCounter;

   };

}

#endif
