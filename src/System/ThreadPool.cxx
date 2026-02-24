/*!
 * \file   ThreadPool.cxx
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

#include <memory>
#include <stdexcept>
#include "TFEL/System/ThreadPool.hxx"

namespace tfel::system {

  ThreadPool::ThreadPool(const size_t n) {
    this->statuses.resize(n, ThreadPool::Status::IDLE);
    for (size_t i = 0; i < n; ++i) {
      auto f = [this, i] {
        for (;;) {
          std::function<void()> task;
          {
            std::unique_lock<std::mutex> lock(this->m);
            while (!(this->stop || !this->tasks.empty())) {
              this->c.wait(
                  lock, [this] { return this->stop || !this->tasks.empty(); });
            }
            if (this->stop && this->tasks.empty()) {
              return;
            }
            task = std::move(this->tasks.front());
            this->tasks.pop();
            this->statuses[i] = ThreadPool::Status::WORKING;
            this->c.notify_all();
          }
          task();
          {
            std::unique_lock<std::mutex> lock(this->m);
            this->statuses[i] = ThreadPool::Status::IDLE;
            this->c.notify_all();
          }
        }
      };
      this->workers.emplace_back(f);
    }
  }  // end of ThreadPool::ThreadPool

  ThreadPool::size_type ThreadPool::getNumberOfThreads() const {
    return this->workers.size();
  }  // end of ThreadPool::getNumberOfThreads

  void ThreadPool::wait() {
    std::unique_lock<std::mutex> lock(this->m);
    while (!this->tasks.empty()) {
      this->c.wait(lock, [this] { return this->tasks.empty(); });
    }
    for (size_t i = 0; i != this->statuses.size(); ++i) {
      while (this->statuses[i] != Status::IDLE) {
        this->c.wait(lock,
                     [this, i] { return this->statuses[i] == Status::IDLE; });
      }
    }
  }  // end of ThreadPool::wait()

  ThreadPool::~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(m);
      this->stop = true;
    }
    this->c.notify_all();
    // the destructor joins all threads
    for (auto &w : this->workers) {
      w.join();
    }
  }  // end of ThreadPool::~ThreadPool

}  // end of namespace tfel::system
