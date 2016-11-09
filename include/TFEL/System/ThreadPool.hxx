/*!
 * \file   ThreadPool.hxx
 * \brief A ThreadPool implementation based on the initial
 * implementation of Jakob Progsch, Václav Zeman:
 * <https://github.com/progschj/ThreadPool>
 *
 * We added the possibility to handle exceptions through the
 * ThreadedTaskResult class.
 *
 * \author THOMAS HELFER
 * \date   19 juin 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef TFEL_SYSTEM_THREAD_POOL_HXX
#define TFEL_SYSTEM_THREAD_POOL_HXX

#include<queue>
#include<mutex>
#include<vector>
#include<thread>
#include<future>
#include<functional>
#include<condition_variable>
#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/System/ThreadedTaskResult.hxx"

namespace tfel{

  namespace system{
    
    /*!
     * structure handling a fixed-size pool of threads
     */
    struct TFELSYSTEM_VISIBILITY_EXPORT ThreadPool
    {
      /*!
       * \brief constructor
       * \param[in] n: number of thread to be created
       */
      ThreadPool(const size_t);
      /*!
       * \brief add a new task
       * \param[in] f: task
       * \param[in] a: arguments passed to the the task
       */
      template<typename F, typename... Args>
      std::future<ThreadedTaskResult<typename std::result_of<F(Args...)>::type>>
      addTask(F&&, Args&&...);
      //! destructor
      ~ThreadPool();
    private:
      //! wrapper around the given task
      template<typename F>
      struct Wrapper;
      // need to keep track of threads so we can join them
      std::vector<std::thread> workers;
      // the task queue
      std::queue<std::function<void()>> tasks;
      // synchronization
      std::mutex queue_mutex;
      std::condition_variable condition;
      bool stop = false;
    };

  } // end of namespace system

} // end of namespace tfel

#include "TFEL/System/ThreadPool.ixx"

#endif /* TFEL_SYSTEM_THREAD_POOL_HXX */
