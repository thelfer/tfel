/*!
 * \file   include/TFEL/Math/NonLinearSolvers/Broyden2.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   05 avr 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_BROYDEN2IXX
#define LIB_TFEL_MATH_BROYDEN2IXX

#include "TFEL/Math/General/IEEE754.hxx"

namespace tfel::math {

  template <unsigned short N,
            typename T,
            const tvector<N, T> (*f)(const tvector<N, T>&)>
  std::pair<bool, tvector<N, T>> broyden2(const tvector<N, T>& x0,
                                          tmatrix<N, N, T>& iJ,
                                          const T e,
                                          const unsigned short n) {
    auto x = x0;
    auto vf = f(x);
    auto c = tvector<N, T>{};
    auto df = tvector<N, T>{};
    unsigned short i = 0;
    while ((i < n) && (norm(vf) > e * N)) {
      if (i > 1) {
        // updating jacobian
        const tvector<N, T> c2 = iJ * df;
        const tvector<N, T> t2 = c * iJ;
        const auto nc = t2 | df;
        if (tfel::math::ieee754::fpclassify(nc) != FP_ZERO) {
          iJ += ((c - c2) ^ (t2)) / nc;
        }
      }
      c = -iJ * vf;
      x += c;
      df = -vf;
      vf = f(x);
      df += vf;
      ++i;
    }
    return {i != n, x};
  }  // end of function broyden2

  template <unsigned short N,
            typename T,
            const tvector<N, T> (*f)(const tvector<N, T>&)>
  std::pair<bool, tvector<N, T>> broyden2(const tvector<N, T>& x0,
                                          const T e,
                                          const unsigned short n) {
    auto iJ = tmatrix<N, N, T>::Id();
    return broyden2<N, T, f>(x0, iJ, e, n);
  }  // end of function broyden2

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_BROYDEN2IXX */
