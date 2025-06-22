/*!
 * \file   SharedPtr.hxx
 * \brief This file works around a missing specialisation of the
 * get_pointer function for std::shared_ptr in old boost versions.
 * Proper support has been introduced in boost 1.53.
 * \author Thomas Helfer
 * \date   05 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_PYTHON_SHAREDPTR_HXX
#define LIB_TFEL_PYTHON_SHAREDPTR_HXX

#include <ciso646>
#include <boost/version.hpp>
#include <boost/bind.hpp>

#ifdef _MSC_VER
#ifdef and
#undef and
#endif
#ifdef xor
#undef xor
#endif
#ifdef or
#undef or
#endif
#endif /* _MSC_VER */

#if BOOST_VERSION < 105300
#define TFEL_PYTHON_GETPOINTER_OVERLOAD
#else
#ifdef __clang__
#ifndef _LIBCPP_VERSION
#if BOOST_VERSION < 105700
#define TFEL_PYTHON_GETPOINTER_OVERLOAD
#endif /* BOOST_VERSION */
#endif /* _LIBCPP_VERSION */
#endif /* __clang__ */
#endif /* BOOST_VERSION */

#include "TFEL/Config/TFELConfig.hxx"

#ifdef TFEL_PYTHON_GETPOINTER_OVERLOAD
/* make boost::python understand std::shared_ptr */
/* http://boost.2283326.n4.nabble.com/No-automatic-upcasting-with-std-shared-ptr-in-function-calls-td4573165.html
 */
namespace boost {
  template <typename T>
  TFEL_VISIBILITY_LOCAL TFEL_INLINE T *get_pointer(
      const std::shared_ptr<T> &p) {
    return p.get();
  }
}  // namespace boost
#endif

#endif /* LIB_TFEL_PYTHON_SHAREDPTR_HXX */
