/*!
 * \file   tests/Math/levenberg-marquardt3.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   19 nov 2008
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

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <utility>
#include <vector>

#include "TFEL/Math/Vector/tvectorIO.hxx"
#include "TFEL/Math/FSLevenbergMarquardt.hxx"

#ifndef _MSC_VER
#define LM_STATIC static
#else
#define LM_STATIC
#endif

LM_STATIC std::pair<double, tfel::math::tvector<2u, double>> test(
    const double& x, const tfel::math::tvector<2u, double>& p) {
  using namespace std;
  using namespace tfel::math;
  pair<double, tvector<2u, double>> res;
  double tmp = exp(-p(1) / x);
  res.first = p(0) * tmp;
  res.second(0) = tmp;
  res.second(1) = -p(0) / x * tmp;
  return res;
}  // end of test

LM_STATIC double f(const double x) { return 2.1033 * exp(-403.02 / x); }

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;

  FSLevenbergMarquardt<1u, 2u> levmar(test);
  double x;
  tvector<2u> p;
  // data
  x = 600.;
  levmar.addData(x, f(x));
  x = 700.;
  levmar.addData(x, f(x));
  x = 800.;
  levmar.addData(x, f(x));
  // initial guess
  p(0) = 1.;
  p(1) = 1.e2;
  levmar.setInitialGuess(p);
  // execute
  p = levmar.execute();
  cout << p << endl;
  return EXIT_SUCCESS;
}  // end of main
