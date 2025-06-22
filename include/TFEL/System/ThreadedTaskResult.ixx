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

namespace tfel {

  namespace system {

    template <typename T>
    ThreadedTaskResult<T>::ThreadedTaskResult() = default;

    template <typename T>
    template <typename... Args>
    ThreadedTaskResult<T>::ThreadedTaskResult(Args&&... args) {
      this->build(args...);
    }  // end of ThreadedTaskResult<T>::ThreadedTaskResult

    template <typename T>
    ThreadedTaskResult<T>::ThreadedTaskResult(ThreadedTaskResult&& src) {
      this->eptr = std::move(src.eptr);
      if (src.initialized) {
        this->initialized = true;
        this->build(std::move(*(src.get_pointer())));
      }
    }

    template <typename T>
    ThreadedTaskResult<T>::ThreadedTaskResult(const ThreadedTaskResult& src) {
      this->eptr = src.eptr;
      if (src.initialized) {
        this->initialized = true;
        this->build(src.get_pointer());
      }
    }

    template <typename T>
    ThreadedTaskResult<T>& ThreadedTaskResult<T>::operator=(
        ThreadedTaskResult&& src) {
      if (this != &src) {
        this->eptr = std::move(src.eptr);
        if (src.initialized) {
          if (this->initialized) {
            *(this->get_pointer()) = std::move(*(src.get_pointer()));
          } else {
            this->build(std::move(*(src.get_pointer())));
          }
        } else {
          this->clear();
        }
      }
      return *this;
    }

    template <typename T>
    ThreadedTaskResult<T>& ThreadedTaskResult<T>::operator=(
        const ThreadedTaskResult& src) {
      if (this != &src) {
        this->eptr = src.eptr;
        if (src.initialized) {
          if (this->initialized) {
            *(this->get_pointer()) = *(src.get_pointer());
          } else {
            this->build(*(src.get_pointer()));
          }
        } else {
          this->clear();
        }
      }
      return *this;
    }

    template <typename T>
    ThreadedTaskResult<T>& ThreadedTaskResult<T>::operator=(const T& src) {
      if (this->initialized) {
        *(this->get_pointer()) = src;
      } else {
        this->build(src);
      }
      return *this;
    }

    template <typename T>
    ThreadedTaskResult<T>& ThreadedTaskResult<T>::operator=(T&& src) {
      if (this->initialized) {
        *(this->get_pointer()) = std::forward<T>(src);
      } else {
        this->build(std::forward<T>(src));
      }
      return *this;
    }

    template <typename T>
    ThreadedTaskResult<T>::operator bool() const {
      return this->initialized && (this->eptr == nullptr);
    }

    template <typename T>
    T& ThreadedTaskResult<T>::operator*() {
      if (this->eptr != nullptr) {
        this->rethrow();
      }
      if (!this->initialized) {
        ThreadedTaskResultBase::throwBadCastException();
      }
      return *(this->get_pointer());
    }

    template <typename T>
    const T& ThreadedTaskResult<T>::operator*() const {
      if (this->eptr != nullptr) {
        this->rethrow();
      }
      if (!this->initialized) {
        ThreadedTaskResultBase::throwBadCastException();
      }
      return *(this->get_pointer());
    }

    template <typename T>
    T* ThreadedTaskResult<T>::operator->() {
      if (this->eptr != nullptr) {
        this->rethrow();
      }
      if (!this->initialized) {
        ThreadedTaskResultBase::throwBadCastException();
      }
      return this->get_pointer();
    }

    template <typename T>
    const T* ThreadedTaskResult<T>::operator->() const {
      if (this->eptr != nullptr) {
        this->rethrow();
      }
      if (!this->initialized) {
        ThreadedTaskResultBase::throwBadCastException();
      }
      return this->get_pointer();
    }

    template <typename T>
    void ThreadedTaskResult<T>::setException(const std::exception_ptr& e) {
      this->clear();
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
    template <typename... Args>
    void ThreadedTaskResult<T>::build(Args&&... args) {
      this->clear();
      new (this->get_pointer()) T(args...);
      this->initialized = true;
    }

    template <typename T>
    void ThreadedTaskResult<T>::clear() {
      this->eptr = nullptr;
      if (this->initialized) {
        auto ptr = this->get_pointer();
        ptr->~T();
        this->initialized = false;
      }
    }  // end of clear

    template <typename T>
    T* ThreadedTaskResult<T>::get_pointer() {
      return reinterpret_cast<T*>(::std::addressof(this->storage));
    }  // end of get_pointer

    template <typename T>
    const T* ThreadedTaskResult<T>::get_pointer() const {
      return reinterpret_cast<const T*>(::std::addressof(this->storage));
    }  // end of get_pointer

    template <typename T>
    ThreadedTaskResult<T>::~ThreadedTaskResult() {
      this->clear();
    }

  }  // end of namespace system

}  // end of namespace tfel

#endif /* LIB_THREADEDTASKRESULTIXX */
