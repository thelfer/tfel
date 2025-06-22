/*!
 * \file   include/TFEL/Math/General/Complex.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10/12/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_COMPLEX_HXX
#define LIB_TFEL_MATH_COMPLEX_HXX

#include <complex>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Forward/Complex.hxx"
#include "TFEL/Math/General/UnaryResultType.hxx"

namespace tfel::math {

  template <int N, unsigned int D>
  struct Power;

  template <int N, unsigned int D>
  class UnaryResultType<Complex<float>, Power<N, D>> {
   public:
    typedef Complex<float> type;
  };

  template <int N, unsigned int D>
  class UnaryResultType<Complex<double>, Power<N, D>> {
   public:
    typedef Complex<double> type;
  };

  template <int N, unsigned int D>
  class UnaryResultType<Complex<long double>, Power<N, D>> {
   public:
    typedef Complex<long double> type;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_COMPLEX_HXX */
