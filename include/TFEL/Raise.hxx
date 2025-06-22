/*!
 * \file   include/TFEL/Raise.hxx
 * \brief  declaration of the `raise` function.
 * \author Thomas Helfer
 * \date   19/09/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_RAISE_HXX
#define LIB_TFEL_RAISE_HXX

#include <utility>
#include <stdexcept>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  /*!
   * \brief a small wrapper used to build the exception outside the
   * `throw` statement. As most exception's classes constructors may
   * throw, this avoids undefined behaviour as reported by the
   * `cert-err60-cpp` warning of `clang-tidy` (thrown exception type
   * is not nothrow copy constructible).
   * \tparam Exception: time of the exception to be thrown.
   */
  template <typename Exception = std::runtime_error>
  TFEL_NORETURN TFEL_VISIBILITY_LOCAL TFEL_INLINE void raise();

  /*!
   * \brief a small wrapper used to build the exception outside the
   * `throw` statement. As most exception's classes constructors may
   * throw, this avoids undefined behaviour as reported by the
   * `cert-err60-cpp` warning of `clang-tidy` (thrown exception type
   * is not nothrow copy constructible).
   * \tparam Exception: time of the exception to be thrown.
   * \tparam Args: type of the arguments passed to the exception'
   * constructor.
   * \param[in] a: arguments passed to the exception' constructor.
   */
  template <typename Exception = std::runtime_error, typename... Args>
  TFEL_NORETURN TFEL_VISIBILITY_LOCAL TFEL_INLINE void raise(Args&&...);

  /*!
   * \brief raise an exception if the first argument is `true`.
   * \tparam Exception: time of the exception to be thrown.
   * \param[in] b: condition to be checked. If `true`, an exception is
   * thrown.
   */
  template <typename Exception = std::runtime_error>
  TFEL_VISIBILITY_LOCAL TFEL_INLINE void raise_if(const bool);

  /*!
   * \brief raise an exception if the first argument is `true`.
   * \tparam Exception: time of the exception to be thrown.
   * \tparam Args: type of the arguments passed to the exception'
   * constructor.
   * \param[in] b: condition to be checked. If `true`, an exception is
   * thrown.
   * \param[in] a: arguments passed to the exception' constructor.
   */
  template <typename Exception = std::runtime_error, typename... Args>
  TFEL_VISIBILITY_LOCAL TFEL_INLINE void raise_if(const bool, Args&&...);

}  // end of namespace tfel

#include "TFEL/Raise.ixx"

#endif /* LIB_TFEL_RAISE_HXX */
