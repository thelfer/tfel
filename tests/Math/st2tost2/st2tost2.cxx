/*!
 * \file   tests/Math/st2tost2/st2tost2.cxx
 * \brief
 * \author Helfer Thomas
 * \date   30 jan 2007
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

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::math;
  using namespace std;

  st2tost2<3> s1(1.5);
  st2tost2<3> s2(4.);

  st2tost2<3> s3;
  s3 = s1 + 0.5 * s2;

  assert(abs(s3(0, 0) - 3.5) < 1.e-14);
  assert(abs(s3(0, 1) - 3.5) < 1.e-14);
  assert(abs(s3(0, 2) - 3.5) < 1.e-14);
  assert(abs(s3(0, 3) - 3.5) < 1.e-14);

  assert(abs(s3(1, 0) - 3.5) < 1.e-14);
  assert(abs(s3(1, 1) - 3.5) < 1.e-14);
  assert(abs(s3(1, 2) - 3.5) < 1.e-14);
  assert(abs(s3(1, 3) - 3.5) < 1.e-14);

  assert(abs(s3(2, 0) - 3.5) < 1.e-14);
  assert(abs(s3(2, 1) - 3.5) < 1.e-14);
  assert(abs(s3(2, 2) - 3.5) < 1.e-14);
  assert(abs(s3(2, 3) - 3.5) < 1.e-14);

  assert(abs(s3(3, 0) - 3.5) < 1.e-14);
  assert(abs(s3(3, 1) - 3.5) < 1.e-14);
  assert(abs(s3(3, 2) - 3.5) < 1.e-14);
  assert(abs(s3(3, 3) - 3.5) < 1.e-14);

  return EXIT_SUCCESS;
}
