/*!
 * \file   ThreadedTaskResult.hxx
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

#ifndef LIB_TFEL_SYSTEM_THREADEDTASKRESULT_HXX
#define LIB_TFEL_SYSTEM_THREADEDTASKRESULT_HXX

#include <optional>
#include <exception>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::system {

  //! non-template base class of the ThreadedTaskResult class
  struct TFELSYSTEM_VISIBILITY_EXPORT ThreadedTaskResultBase {
   protected:
    //! thr
    [[noreturn]] static void throwBadCastException();
    //! thr
    [[noreturn]] static void throwNullException();
  };  // end of struct ThreadedTaskResultBase

  //! \brief a class standing for the result of a taks
  template <typename T>
  struct ThreadedTaskResult : public ThreadedTaskResultBase {
    //! \brief default constructor
    ThreadedTaskResult();
    /*!
     * \brief constructor of T
     * \param[in] args: arguments to T constructor
     */
    template <typename... Args>
    ThreadedTaskResult(Args&&...);
    /*!
     * \brief constructor of T
     */
    ThreadedTaskResult(const T&);
    //! \brief move constructor
    ThreadedTaskResult(ThreadedTaskResult&&);
    //! \brief copy constructor
    ThreadedTaskResult(const ThreadedTaskResult&);
    //! \brief assignement
    ThreadedTaskResult& operator=(const ThreadedTaskResult&);
    //! \brief move assignement
    ThreadedTaskResult& operator=(ThreadedTaskResult&&);
    //! \brief assignement
    ThreadedTaskResult& operator=(const T&);
    //! \brief move assignement
    ThreadedTaskResult& operator=(T&&);
    //! \brief set current exception
    void setException(const std::exception_ptr&);
    //! \brief throw the catched exception
#ifndef _MSC_VER
    [[noreturn]] void rethrow();
#else  /* _MSC_VER */
    void rethrow();
#endif /* _MSC_VER */
       //! \brief conversion to bool
    operator bool() const;
    //! \brief conversion to underlying type
    T& operator*();
    //! \brief conversion to underlying type
    const T& operator*() const;
    //! \brief conversion to underlying type
    T* operator->();
    //! \brief conversion to underlying type
    const T* operator->() const;
    //! \brief destructor
    ~ThreadedTaskResult();

   private:
    //! \brief result of the ThreadedTaskResult
    std::optional<T> result;
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
    [[noreturn]] void rethrow();
    //! \brief conversion to bool
    operator bool() const;
    //! \brief destructor
    ~ThreadedTaskResult();

   private:
    //! exception ptr thrown during the task
    std::exception_ptr eptr;
  };

}  // end of namespace tfel::system

#include "TFEL/System/ThreadedTaskResult.ixx"

#endif /* LIB_TFEL_SYSTEM_THREADEDTASKRESULT_HXX */
