/*!
 * \file   tests/Math/levenberg-marquardt2.cxx
 * \brief
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
  double v0 = cos(x * x);
  ;
  double v1 = exp(p(0) * v0);
  res.first = p(1) * v1;
  res.second(0) = p(1) * v0 * v1;
  res.second(1) = v1;
  return res;
}  // end of test

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;

  const double x_data[] = {0.840188, 0.783099, 0.911647, 0.335223, 0.277775,
                           0.477397, 0.364784, 0.952230, 0.635712, 0.141603};

  const double y_data[] = {2.12989, 2.29532, 1.93195, 2.72801, 2.71560,
                           2.66178, 2.69570, 1.89371, 2.52963, 2.72843};

  FSLevenbergMarquardt<1u, 2u> levmar(test);
  tvector<2u> p;
  unsigned short i;
  for (i = 0; i != 10; ++i) {
    levmar.addData(x_data[i], y_data[i]);
  }
  // initial guess
  p(0) = 1.2;
  p(1) = -0.2;
  levmar.setInitialGuess(p);
  // execute
  p = levmar.execute();
  cout << "res : " << p << endl;
  return EXIT_SUCCESS;
}  // end of main
