/*!
 * \file   tests/Math/stensor_eigenvalues.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   03 jui 2006
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

#include <cmath>
#include <cassert>
#include <cstdlib>

#include "fenv.h"

#include "TFEL/Math/stensor.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  double vp1;
  double vp2;
  double vp3;
  stensor<3, double> s(0.);
  s.computeEigenValues(vp1, vp2, vp3);
  assert(abs(vp1 - 0) < 1.e-14);
  assert(abs(vp2 - 0) < 1.e-14);
  assert(abs(vp3 - 0) < 1.e-14);
  return EXIT_SUCCESS;
}
