/*!
 * \file   tests/Math/VectorResultType.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   09 Oct 2006
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

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/tvector.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  using tfel::math::vector;
  vector<tvector<3>> v(3, tvector<3>(4.));
  vector<tvector<3>> v2(3, tvector<3>(2.));
  assert(abs((v + v2)(0)(0) - 6.) < 1.e-14);
  assert(abs((v + v2)(0)(1) - 6.) < 1.e-14);
  assert(abs((v + v2)(0)(2) - 6.) < 1.e-14);
  assert(abs((v + v2)(1)(0) - 6.) < 1.e-14);
  assert(abs((v + v2)(1)(1) - 6.) < 1.e-14);
  assert(abs((v + v2)(1)(2) - 6.) < 1.e-14);
  assert(abs((v + v2)(2)(0) - 6.) < 1.e-14);
  assert(abs((v + v2)(2)(1) - 6.) < 1.e-14);
  assert(abs((v + v2)(2)(2) - 6.) < 1.e-14);
  return EXIT_SUCCESS;
}
