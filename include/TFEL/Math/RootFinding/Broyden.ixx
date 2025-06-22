/*!
 * \file   include/TFEL/Math/RootFinding/Broyden.ixx
 * \brief
 *
 * \author Helfer Thomas
 * \date   05 avr 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_BROYDEN_IXX_
#define LIB_TFEL_MATH_BROYDEN_IXX_

#ifdef TFEL_BROYDEN_VERBOSE_MODE
#include <iostream>
#endif /* LIB_TFEL_MATH_BROYDEN_IXX_ */

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/TinyMatrixSolve.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N,
              typename T,
              const tvector<N, T> (*f)(const tvector<N, T>&)>
    const std::pair<bool, tvector<N, T>> broyden(const tvector<N, T>& x0,
                                                 tmatrix<N, N, T>& A,
                                                 const T e,
                                                 const unsigned short n) {
      using namespace std;
      using namespace tfel::math;
      tvector<N, T> x = x0;
      tvector<N, T> vf = f(x);
      unsigned short i = 0;
      while ((i < n) && (norm(vf) / N > e)) {
#ifdef TFEL_BROYDEN_VERBOSE_MODE
        cout << "iteration : " << i << endl;
        cout << "x         : " << x << endl;
        cout << "||f||     : " << norm(vf) << endl;
#endif /* LIB_TFEL_MATH_BROYDEN_IXX_ */
        tvector<N, T> s = -vf;
        tmatrix<N, N, T> J = A;
        TinyMatrixSolve<N, T>::exe(J, s);
        x += s;
        const tvector<N, T> vf2 = vf;
        vf = f(x);
        const tvector<N, T> y = vf - vf2;
        J = A;
        const tvector<N, T> t = -J * s;
        A += ((y - t) ^ s) / (s | s);
        ++i;
      }
      return pair<bool, tvector<N, T>>(i != n, x);
    }  // end of function broyden

    template <unsigned short N,
              typename T,
              const tvector<N, T> (*f)(const tvector<N, T>&)>
    const std::pair<bool, tvector<N, T>> broyden(const tvector<N, T>& x0,
                                                 const T e,
                                                 const unsigned short n) {
      using namespace std;
      using namespace tfel::math;
      tmatrix<N, N, T> A(0.);
      unsigned short i;
      for (i = 0; i != N; ++i) {
        A(i, i) = 1.;
      }
      return broyden<N, T, f>(x0, A, e, n);
    }  // end of function broyden

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_BROYDEN_IXX_ */
