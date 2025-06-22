/*!
 * \file   ThreadedTaskResult.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   19 juin 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <typeinfo>
#include <stdexcept>
#include "TFEL/System/ThreadedTaskResult.hxx"

namespace tfel {

  namespace system {

    void ThreadedTaskResultBase::throwBadCastException(void) {
      throw(std::bad_cast());
    }  // end of ThreadPool::ResultBase::throwBadCastException

    void ThreadedTaskResultBase::throwNullException(void) {
      throw(
          std::runtime_error("ThreadPool::Result::rethrow: "
                             "no exception defined"));
    }  // end of ThreadPool::ResultBase::throwNullException

    ThreadedTaskResult<void>::ThreadedTaskResult() = default;
    ThreadedTaskResult<void>::ThreadedTaskResult(ThreadedTaskResult&&) =
        default;
    ThreadedTaskResult<void>::ThreadedTaskResult(const ThreadedTaskResult&) =
        default;
    ThreadedTaskResult<void>& ThreadedTaskResult<void>::operator=(
        ThreadedTaskResult&&) = default;
    ThreadedTaskResult<void>& ThreadedTaskResult<void>::operator=(
        const ThreadedTaskResult&) = default;
    ThreadedTaskResult<void>::~ThreadedTaskResult() = default;

    ThreadedTaskResult<void>::operator bool() const {
      return this->eptr == nullptr;
    }

    void ThreadedTaskResult<void>::setException(const std::exception_ptr& e) {
      this->eptr = e;
    }  // end of ThreadedTaskResult<void>::setException

    void ThreadedTaskResult<void>::rethrow() {
      if (this->eptr == nullptr) {
        ThreadedTaskResultBase::throwNullException();
      }
      std::rethrow_exception(eptr);
    }  // end of ThreadedTaskResult<void>::setException

  }  // end of namespace system

}  // end of namespace tfel
