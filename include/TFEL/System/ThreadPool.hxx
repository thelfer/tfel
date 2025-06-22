/*!
 * \file   ThreadPool.hxx
 * \brief A ThreadPool implementation based on the initial
 * implementation of Jakob Progsch, Václav Zeman:
 * <https://github.com/progschj/ThreadPool>
 *
 * We added the possibility to handle exceptions through the
 * ThreadedTaskResult class.
 *
 * \author Thomas Helfer
 * \date   19 juin 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_SYSTEM_THREAD_POOL_HXX
#define TFEL_SYSTEM_THREAD_POOL_HXX

#include <queue>
#include <mutex>
#include <vector>
#include <thread>
#include <future>
#include <functional>
#include <condition_variable>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/ThreadedTaskResult.hxx"

namespace tfel {

  namespace system {

    /*!
     * \brief structure handling a fixed-size pool of threads
     */
    struct TFELSYSTEM_VISIBILITY_EXPORT ThreadPool {
      //! a simple alias
      using size_type = std::vector<std::thread>::size_type;
      /*!
       * \brief constructor
       * \param[in] n: number of thread to be created
       */
      ThreadPool(const size_type);
      /*!
       * \brief add a new task
       * \param[in] f: task
       * \param[in] a: arguments passed to the the task
       */
      template <typename F, typename... Args>
      std::future<ThreadedTaskResult<typename std::result_of<F(Args...)>::type>>
      addTask(F&&, Args&&...);
      //! \return the number of threads managed by the ppol
      size_type getNumberOfThreads() const;
      //! \brief wait for all tasks to be finished
      void wait();
      //! destructor
      ~ThreadPool();

     private:
      //! wrapper around the given task
      template <typename F>
      struct Wrapper;
      enum Status { WORKING, IDLE };  // end of enum Status
      std::vector<Status> statuses;
      //! list of threads
      std::vector<std::thread> workers;
      // the task queue
      std::queue<std::function<void()>> tasks;
      // synchronization
      std::mutex m;
      std::condition_variable c;
      bool stop = false;
    };

  }  // end of namespace system

}  // end of namespace tfel

#include "TFEL/System/ThreadPool.ixx"

#endif /* TFEL_SYSTEM_THREAD_POOL_HXX */
