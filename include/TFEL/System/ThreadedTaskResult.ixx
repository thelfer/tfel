/*!
 * \file   ThreadedTaskResult.ixx
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

#ifndef LIB_THREADEDTASKRESULTIXX
#define LIB_THREADEDTASKRESULTIXX

#include <memory>
#include <utility>

namespace tfel::system {

  template <typename T>
  ThreadedTaskResult<T>::ThreadedTaskResult() = default;

  template <typename T>
  template <typename... Args>
  ThreadedTaskResult<T>::ThreadedTaskResult(Args&&... args)
      : result(args...) {}  // end of ThreadedTaskResult

  template <typename T>
  ThreadedTaskResult<T>::ThreadedTaskResult(ThreadedTaskResult&&) = default;

  template <typename T>
  ThreadedTaskResult<T>::ThreadedTaskResult(const ThreadedTaskResult&) =
      default;

  template <typename T>
  ThreadedTaskResult<T>& ThreadedTaskResult<T>::operator=(
      ThreadedTaskResult&&) = default;

  template <typename T>
  ThreadedTaskResult<T>& ThreadedTaskResult<T>::operator=(
      const ThreadedTaskResult& src) = default;

  template <typename T>
  ThreadedTaskResult<T>& ThreadedTaskResult<T>::operator=(const T& src) {
    this->result = src;
    return *this;
  }

  template <typename T>
  ThreadedTaskResult<T>& ThreadedTaskResult<T>::operator=(T&& src) {
    this->result = std::forward<T>(src);
    return *this;
  }

  template <typename T>
  ThreadedTaskResult<T>::operator bool() const {
    return this->result.has_value() && (this->eptr == nullptr);
  }

  template <typename T>
  T& ThreadedTaskResult<T>::operator*() {
    if (this->eptr != nullptr) {
      this->rethrow();
    }
    if (!this->result.has_value()) {
      ThreadedTaskResultBase::throwBadCastException();
    }
    return *(this->result);
  }  // end of operator *

  template <typename T>
  const T& ThreadedTaskResult<T>::operator*() const {
    if (this->eptr != nullptr) {
      this->rethrow();
    }
    if (!this->result.has_value()) {
      ThreadedTaskResultBase::throwBadCastException();
    }
    return *(this->result);
  }

  template <typename T>
  T* ThreadedTaskResult<T>::operator->() {
    if (this->eptr != nullptr) {
      this->rethrow();
    }
    if (!this->result.has_value()) {
      ThreadedTaskResultBase::throwBadCastException();
    }
    return this->result.operator->();
  }

  template <typename T>
  const T* ThreadedTaskResult<T>::operator->() const {
    if (this->eptr != nullptr) {
      this->rethrow();
    }
    if (!this->result.has_value()) {
      ThreadedTaskResultBase::throwBadCastException();
    }
    return this->result.operator->();
  }

  template <typename T>
  void ThreadedTaskResult<T>::setException(const std::exception_ptr& e) {
    this->result.reset();
    this->eptr = e;
  }  // end of ThreadedTaskResult<T>::setException

  template <typename T>
  void ThreadedTaskResult<T>::rethrow() {
    if (this->eptr == nullptr) {
      ThreadedTaskResultBase::throwNullException();
    }
    std::rethrow_exception(eptr);
  }  // end of ThreadedTaskResult<T>::setException

  template <typename T>
  ThreadedTaskResult<T>::~ThreadedTaskResult() = default;

}  // end of namespace tfel::system

#endif /* LIB_THREADEDTASKRESULTIXX */
