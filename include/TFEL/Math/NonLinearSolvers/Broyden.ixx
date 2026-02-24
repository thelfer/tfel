/*!
 * \file   include/TFEL/Math/NonLinearSolvers/Broyden.ixx
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

#ifndef LIB_TFEL_MATH_MATH_BROYDENIXX
#define LIB_TFEL_MATH_MATH_BROYDENIXX

#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/TinyMatrixSolve.hxx"

namespace tfel::math {

  template <unsigned short N,
            typename T,
            const tvector<N, T> (*f)(const tvector<N, T>&)>
  std::pair<bool, tvector<N, T>> broyden(const tvector<N, T>& x0,
                                         tmatrix<N, N, T>& J,
                                         const T e,
                                         const unsigned short n) {
    auto x = x0;
    auto vf = f(x);
    unsigned short i = 0;
    auto vf_1 = tfel::math::tvector<N, T>{};
    auto c = tfel::math::tvector<N, T>{};
    while ((i < n) && (norm(vf) > N * e)) {
      if (i > 1) {
        // update the jacobian
        const tvector<N, T> t = J * c;
        const auto nc = (c | c);
        if (tfel::math::ieee754::fpclassify(nc) != FP_ZERO) {
          J += ((vf - vf_1 - t) ^ c) / nc;
        }
      }
      auto A = J;
      c = -vf;
      TinyMatrixSolve<N, T>::exe(A, c);
      x += c;
      vf_1 = vf;
      vf = f(x);
      ++i;
    }
    return std::pair<bool, tvector<N, T>>(i != n, x);
  }  // end of function broyden

  template <unsigned short N,
            typename T,
            const tvector<N, T> (*f)(const tvector<N, T>&)>
  std::pair<bool, tvector<N, T>> broyden(const tvector<N, T>& x0,
                                         const T e,
                                         const unsigned short n) {
    auto A = tmatrix<N, N, T>::Id();
    return broyden<N, T, f>(x0, A, e, n);
  }  // end of function broyden

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_MATH_BROYDENIXX */
