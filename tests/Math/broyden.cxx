/*!
 * \file   tests/Math/broyden.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   09 avr 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cstdlib>
#include <cmath>

#include "TFEL/Math/Broyden.hxx"

#ifndef _MSC_VER
#define BROYDEN_STATIC static
#else
#define BROYDEN_STATIC
#endif

BROYDEN_STATIC const tfel::math::tvector<2, double> f(
    const tfel::math::tvector<2, double>& x) {
  using namespace tfel::math;
  tvector<2, double> y;
  y(0) = 1 + x(0) * (2 * x(0) - 4) - 2 * x(1) * x(1) * x(1);
  y(1) = -4 + x(0) * x(0) * x(0) * x(0) + 4 * x(1) * (1 + x(1) * x(1) * x(1));
  return y;
}

BROYDEN_STATIC void test() {
  using namespace std;
  using namespace tfel::math;
  tvector<2, double> res;
  tvector<2, double> x0;
  tvector<2, double> vf;
  tmatrix<2, 2, double> A;
  x0(0) = 1.5;
  x0(1) = -0.5;
  A(0, 0) = 4 * (x0(0) - 1);
  A(0, 1) = -6 * x0(1) * x0(1);
  A(1, 0) = 4 * x0(0) * x0(0) * x0(0);
  A(1, 1) = 4 * (4 * x0(1) * x0(1) * x0(1) + 1);
  res = broyden<2, double, f>(x0, A, 1.e-11, 20).second;
  vf = f(res);
  A(0, 0) = 4 * (res(0) - 1);
  A(0, 1) = -6 * res(1) * res(1);
  A(1, 0) = 4 * res(0) * res(0) * res(0);
  A(1, 1) = 4 * (4 * res(1) * res(1) * res(1) + 1);
}

BROYDEN_STATIC void test2() {
  using namespace std;
  using namespace tfel::math;
  tvector<2, double> res;
  tvector<2, double> x0;
  tvector<2, double> vf;
  x0(0) = 1.5;
  x0(1) = -0.5;
  res = broyden<2, double, f>(x0, 1.e-11, 20).second;
  vf = f(res);
}

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  test();
  test2();
  return EXIT_SUCCESS;
}
