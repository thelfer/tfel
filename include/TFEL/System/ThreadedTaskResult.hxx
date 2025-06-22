/*!
 * \file   ThreadedTaskResult.hxx
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

#ifndef LIB_TFEL_SYSTEM_THREADEDTASKRESULT_HXX
#define LIB_TFEL_SYSTEM_THREADEDTASKRESULT_HXX

#include <exception>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace system {

    //! non-template base class of the ThreadedTaskResult class
    struct TFELSYSTEM_VISIBILITY_EXPORT ThreadedTaskResultBase {
     protected:
      //! thr
      TFEL_NORETURN static void throwBadCastException(void);
      //! thr
      TFEL_NORETURN static void throwNullException(void);
    };  // end of struct ThreadedTaskResultBase
    /*!
     * \brief a class standing for the result of a taks
     *
     * Its interface is loosely base on the interface of
     * std::optional (not available in C++11), but it also stores an
     * std::exception_ptr if .
     */
    template <typename T>
    struct ThreadedTaskResult : public ThreadedTaskResultBase {
      //! \brief default constructor
      TFEL_INLINE ThreadedTaskResult();
      /*!
       * \brief constructor of T
       * \param[in] args: arguments to T constructor
       */
      template <typename... Args>
      TFEL_INLINE ThreadedTaskResult(Args&&...);
      /*!
       * \brief constructor of T
       */
      TFEL_INLINE ThreadedTaskResult(const T&);
      //! \brief move constructor
      TFEL_INLINE ThreadedTaskResult(ThreadedTaskResult&&);
      //! \brief copy constructor
      TFEL_INLINE ThreadedTaskResult(const ThreadedTaskResult&);
      //! \brief assignement
      TFEL_INLINE ThreadedTaskResult& operator=(const ThreadedTaskResult&);
      //! \brief move assignement
      TFEL_INLINE ThreadedTaskResult& operator=(ThreadedTaskResult&&);
      //! \brief assignement
      TFEL_INLINE ThreadedTaskResult& operator=(const T&);
      //! \brief move assignement
      TFEL_INLINE ThreadedTaskResult& operator=(T&&);
      //! \brief set current exception
      TFEL_INLINE void setException(const std::exception_ptr&);
      //! \brief throw the catched exception
      TFEL_INLINE void rethrow(void);
      //! \brief conversion to bool
      TFEL_INLINE operator bool() const;
      //! \brief conversion to underlying type
      TFEL_INLINE T& operator*();
      //! \brief conversion to underlying type
      TFEL_INLINE const T& operator*() const;
      //! \brief conversion to underlying type
      TFEL_INLINE T* operator->();
      //! \brief conversion to underlying type
      TFEL_INLINE const T* operator->() const;
      //! \brief destructor
      TFEL_INLINE ~ThreadedTaskResult();

     private:
      /*!
       * build an object
       * \param[in] args: arguments to the constructor
       */
      template <typename... Args>
      TFEL_INLINE void build(Args&&...);
      //! \brief clear the underlying object destructor
      TFEL_INLINE void clear();
      //! \brief pointer the underlying object storage
      TFEL_INLINE T* get_pointer();
      //! \brief pointer the underlying object storage
      TFEL_INLINE const T* get_pointer() const;
      //! \brief if true, the ThreadedTaskResult contains a value
      bool initialized = false;
      //! \brief storage of the underlying value
      typename std::aligned_storage<sizeof(T), alignof(T)>::type storage;
      //! exception ptr thrown during the task
      std::exception_ptr eptr;
    };

    //! Partial specialisation for non-returning tasks
    template <>
    struct TFELSYSTEM_VISIBILITY_EXPORT ThreadedTaskResult<void>
        : public ThreadedTaskResultBase {
      //! \brief default constructor
      ThreadedTaskResult();
      //! \brief move constructor
      ThreadedTaskResult(ThreadedTaskResult&&);
      //! \brief copy constructor
      ThreadedTaskResult(const ThreadedTaskResult&);
      //! \brief assignement
      ThreadedTaskResult& operator=(const ThreadedTaskResult&);
      //! \brief move assignement
      ThreadedTaskResult& operator=(ThreadedTaskResult&&);
      //! \brief set current exception
      void setException(const std::exception_ptr&);
      //! \brief throw the catched exception
      void rethrow(void);
      //! \brief conversion to bool
      operator bool() const;
      //! \brief destructor
      ~ThreadedTaskResult();

     private:
      //! exception ptr thrown during the task
      std::exception_ptr eptr;
    };

  }  // end of namespace system

}  // end of namespace tfel

#include "TFEL/System/ThreadedTaskResult.ixx"

#endif /* LIB_TFEL_SYSTEM_THREADEDTASKRESULT_HXX_ */
