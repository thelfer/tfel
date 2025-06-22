/*!
 * \file  include/TFEL/Math/Forward/Complex.hxx
 * \brief This file introduces some forward declaration relative to
 * the Complex class.
 * \author Helfer Thomas
 * \brief 03 janv. 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_COMPLEX_H_
#define LIB_TFEL_MATH_FORWARD_COMPLEX_H_

// August 2015 : a simple trick: including this header has not effect
// in c++ other than defining some macros specific to the standard
// library implementation: here we want to detect if we use libc++
// which is a bit touchy with std::complex
#include <ciso646>

#if (defined _LIBCPP_VERSION)
#include <complex>
#else
namespace std {
  // forward declaration of std::complex
#ifndef _MSC_VER
  template <typename T>
  struct complex;
#else
  template <typename T>
  class complex;
#endif /* LIB_TFEL_MATH_FORWARD_COMPLEX_H_ */
}  // end of namespace std
#endif /* defined _LIBCPP_VERSION */

namespace tfel {

  namespace math {

    /*!
     * \brief an alias std::complex.
     * \param ValueType, underlying type.
     * \see std::complex.
     */
    template <typename ValueType>
    using Complex = std::complex<ValueType>;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_FORWARD_COMPLEX_H_ */
