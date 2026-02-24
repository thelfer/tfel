/*!
 * \file   tests/Math/tmatrix1.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   12 avr 2008
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
#include <cstdlib>
#include <cassert>

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/TinyMatrixSolve.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;

  tmatrix<2, 2, double> m;
  tmatrix<2, 2, double> m2;

  m(0, 0) = 0.;
  m(0, 1) = 1.;
  m(1, 0) = 2.;
  m(1, 1) = 3.;

  m2 = m;

  assert(abs(m(0, 0) - 0.) < 1.e-14);
  assert(abs(m(0, 1) - 1.) < 1.e-14);
  assert(abs(m(1, 0) - 2.) < 1.e-14);
  assert(abs(m(1, 1) - 3.) < 1.e-14);
  assert(abs(m2(0, 0) - 0.) < 1.e-14);
  assert(abs(m2(0, 1) - 1.) < 1.e-14);
  assert(abs(m2(1, 0) - 2.) < 1.e-14);
  assert(abs(m2(1, 1) - 3.) < 1.e-14);

  m2 = 2. * m;

  assert(abs(m(0, 0) - 0.) < 1.e-14);
  assert(abs(m(0, 1) - 1.) < 1.e-14);
  assert(abs(m(1, 0) - 2.) < 1.e-14);
  assert(abs(m(1, 1) - 3.) < 1.e-14);
  assert(abs(m2(0, 0) - 0.) < 1.e-14);
  assert(abs(m2(0, 1) - 2.) < 1.e-14);
  assert(abs(m2(1, 0) - 4.) < 1.e-14);
  assert(abs(m2(1, 1) - 6.) < 1.e-14);

  return EXIT_SUCCESS;
}
