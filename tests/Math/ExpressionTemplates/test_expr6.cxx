/*!
 * \file   tests/Math/ExpressionTemplates/test_expr6.cxx
 * \brief
 * \author Thomas Helfer
 * \date   13 Oct 2006
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

#include <cassert>
#include <cstdlib>

#include "TFEL/Math/tvector.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;

  tvector<3, tvector<2, double>> v1(tvector<2, double>(1.4));
  tvector<3, tvector<2, double>> v2(tvector<2, double>(3.));
  tvector<3, tvector<2, double>> v3(tvector<2, double>(1.2));

  assert(abs(v1(0)(0) - 1.4) < 1.e-14);
  assert(abs(v1(0)(1) - 1.4) < 1.e-14);
  assert(abs(v1(1)(0) - 1.4) < 1.e-14);
  assert(abs(v1(1)(1) - 1.4) < 1.e-14);
  assert(abs(v1(2)(0) - 1.4) < 1.e-14);
  assert(abs(v1(2)(1) - 1.4) < 1.e-14);

  assert(abs(v2(0)(0) - 3.) < 1.e-14);
  assert(abs(v2(0)(1) - 3.) < 1.e-14);
  assert(abs(v2(1)(0) - 3.) < 1.e-14);
  assert(abs(v2(1)(1) - 3.) < 1.e-14);
  assert(abs(v2(2)(0) - 3.) < 1.e-14);
  assert(abs(v2(2)(1) - 3.) < 1.e-14);

  assert(abs(v3(0)(0) - 1.2) < 1.e-14);
  assert(abs(v3(0)(1) - 1.2) < 1.e-14);
  assert(abs(v3(1)(0) - 1.2) < 1.e-14);
  assert(abs(v3(1)(1) - 1.2) < 1.e-14);
  assert(abs(v3(2)(0) - 1.2) < 1.e-14);
  assert(abs(v3(2)(1) - 1.2) < 1.e-14);

  assert(abs((v1 + v2)(0)(0) - 4.4) < 1.e-14);
  assert(abs((v1 + v2)(0)(1) - 4.4) < 1.e-14);
  assert(abs((v1 + v2)(1)(0) - 4.4) < 1.e-14);
  assert(abs((v1 + v2)(1)(1) - 4.4) < 1.e-14);
  assert(abs((v1 + v2)(2)(0) - 4.4) < 1.e-14);
  assert(abs((v1 + v2)(2)(1) - 4.4) < 1.e-14);

  assert(abs((v1 + v2 + v3)(0)(0) - 5.6) < 1.e-14);
  assert(abs((v1 + v2 + v3)(0)(1) - 5.6) < 1.e-14);
  assert(abs((v1 + v2 + v3)(1)(0) - 5.6) < 1.e-14);
  assert(abs((v1 + v2 + v3)(1)(1) - 5.6) < 1.e-14);
  assert(abs((v1 + v2 + v3)(2)(0) - 5.6) < 1.e-14);
  assert(abs((v1 + v2 + v3)(2)(1) - 5.6) < 1.e-14);

  return EXIT_SUCCESS;
}
