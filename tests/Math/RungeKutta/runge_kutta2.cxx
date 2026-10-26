/*!
 * \file   tests/Math/RungeKutta/runge_kutta2.cxx
 * \brief
 * \author Thomas Helfer
 * \date 20/06/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "runge_kutta2-header.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;

  test<float>();
  test<double>();
  test<long double>();
  // the global error of this second order scheme is about t h^2 / 6 =
  // 1.7e-4
  const auto success = (computeHarmonicOscillatorError<float>() < 1.e-3f) &&
                       (computeHarmonicOscillatorError<double>() < 1.e-3) &&
                       (computeHarmonicOscillatorError<long double>() < 1.e-3L);
  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
