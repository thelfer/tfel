/*!
 * \file   include/TFEL/Math/RootFinding/Broyden2.ixx
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

#ifndef LIB_TFEL_BROYDEN2IXX
#define LIB_TFEL_BROYDEN2IXX

#ifdef TFEL_BROYDEN2_VERBOSE_MODE
#include <iostream>
#endif /* LIB_TFEL_BROYDEN2IXX */

#include "TFEL/Math/tmatrix.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N,
              typename T,
              const tvector<N, T> (*f)(const tvector<N, T>&)>
    const std::pair<bool, tvector<N, T>> broyden2(const tvector<N, T>& x0,
                                                  tmatrix<N, N, T>& A,
                                                  const T e,
                                                  const unsigned short n) {
      auto x = x0;
      auto vf = f(x);
      unsigned short i = 0;
      while ((i < n) && (norm(vf) / N > e)) {
#ifdef TFEL_BROYDEN2_VERBOSE_MODE
        std::cout << "iteration : " << i << std::endl;
        std::cout << "x         : " << x << std::endl;
        std::cout << "||f||     : " << norm(vf) << std::endl;
        std::cout << "A         : " << A << std::endl;
#endif /* LIB_TFEL_BROYDEN2IXX */
        const auto J = A;
        const auto vf2 = vf;
        const tvector<N, T> s = -A * vf;
        const tvector<N, T> y = vf - vf2;
        x += s;
        vf = f(x);
        const tvector<N, T> t = J * y;
        const tvector<N, T> t2 = s * J;
        A += ((s - t) ^ t2) / (s | t);
        ++i;
      }
      return {i != n, x};
    }  // end of function broyden2

    template <unsigned short N,
              typename T,
              const tvector<N, T> (*f)(const tvector<N, T>&)>
    const std::pair<bool, tvector<N, T>> broyden2(const tvector<N, T>& x0,
                                                  const T e,
                                                  const unsigned short n) {
      tmatrix<N, N, T> A(0.);
      for (unsigned short i = 0; i != N; ++i) {
        A(i, i) = 1.;
      }
      return broyden2<N, T, f>(x0, A, e, n);
    }  // end of function broyden2

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_BROYDEN2IXX */
