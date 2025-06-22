/*!
 * \file  tests/Math/krigeage.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 09 avr 2009
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

#include <fstream>
#include <cstdlib>

#include "TFEL/Math/General/floating_point_exceptions.hxx"
#include "TFEL/Math/FactorizedKriging.hxx"

#ifndef _MSC_VER
#define KRIGING_STATIC static
#else
#define KRIGING_STATIC
#endif

KRIGING_STATIC double rand(const double min, const double max) {
  return min + ((max - min) * rand()) / RAND_MAX;
}

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  FactorizedKriging<1, 1> k;
  double x;
  double t;
  unsigned int i;
  ofstream data("data.txt");
  ofstream out("out.txt");

  init_floating_point_exceptions();

  for (i = 0; i != 100; ++i) {
    x = rand(0., 1.);
    t = rand(0., 1.);
    double f = cos(x + t) * exp(x);
    k.addValue(x, t, f);
    data << x << " " << t << " " << f << endl;
  }
  k.buildInterpolation();
  for (x = -0.5; x < 1.5; x += 0.05) {
    for (t = -0.5; t < 1.5; t += 0.05) {
      out << x << " " << t << " " << k(x, t) << endl;
    }
  }

  return EXIT_SUCCESS;
}
