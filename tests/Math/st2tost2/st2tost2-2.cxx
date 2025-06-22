/*!
 * \file   tests/Math/st2tost2/st2tost2-2.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   19 mar 2008
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

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/stensor.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::math;
  using namespace std;

  st2tost2<2> D(1.);
  stensor<2> s;
  stensor<2> s1;

  s(0) = 1.;
  s(1) = 2.;
  s(2) = 3.;
  s(3) = 4.;

  s1 = D * s;

  assert(abs(s1(0) - 10.) < 1.e-14);
  assert(abs(s1(1) - 10.) < 1.e-14);
  assert(abs(s1(2) - 10.) < 1.e-14);
  assert(abs(s1(3) - 10.) < 1.e-14);

  return EXIT_SUCCESS;
}
