/*!
 * \file   ThreadPool.ixx
 * \brief
 * \author Thomas Helfer
 * \date   19 juin 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_SYSTEM_THREAD_POOL_IXX
#define TFEL_SYSTEM_THREAD_POOL_IXX

#include <memory>
#include <type_traits>

namespace tfel {

  namespace system {

    template <typename F>
    struct ThreadPool::Wrapper {
      Wrapper(F&& f_) : f(f_) {}
      template <typename... Args>
      ThreadedTaskResult<typename std::result_of<F(Args...)>::type> operator()(
          Args&&... args) {
        using result = typename std::result_of<F(Args...)>::type;
        using apply =
            typename std::conditional<std::is_same<result, void>::value,
                                      GetVoid, Get<result>>::type;
        ThreadedTaskResult<result> r;
        apply::exe(r, f, std::forward<Args>(args)...);
        return r;
      }

     private:
      template <typename T, typename... Args>
      struct Get {
        static void exe(ThreadedTaskResult<T>& r, F& t, Args&&... args) {
          try {
            r = t(args...);
          } catch (...) {
            r.setException(std::current_exception());
          }
        }
      };
      struct GetVoid {
        template <typename... Args>
        static void exe(ThreadedTaskResult<void>& r, F& t, Args&&... args) {
          try {
            t(args...);
          } catch (...) {
            r.setException(std::current_exception());
          }
        }
      };
      // task to be performed
      F f;
    };

    // add new work item to the pool
    template <typename F, typename... Args>
    std::future<ThreadedTaskResult<typename std::result_of<F(Args...)>::type>>
    ThreadPool::addTask(F&& f, Args&&... a) {
      using return_type =
          ThreadedTaskResult<typename std::result_of<F(Args...)>::type>;
      using task = std::packaged_task<return_type()>;
      auto t = std::make_shared<task>(
          std::bind(Wrapper<F>(std::forward<F>(f)), std::forward<Args>(a)...));
      auto res = t->get_future();
      {
        std::unique_lock<std::mutex> lock(this->m);
        // don't allow enqueueing after stopping the pool
        if (this->stop) {
          throw std::runtime_error(
              "ThreadPool::addTask: "
              "enqueue on stopped ThreadPool");
        }
        this->tasks.emplace([t] { (*t)(); });
      }
      this->c.notify_one();
      return res;
    }

  }  // end of namespace system

}  // end of namespace tfel

#endif /* TFEL_SYSTEM_THREAD_POOL_IXX */
