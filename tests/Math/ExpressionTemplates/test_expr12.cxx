/*!
 * \file   tests/Math/ExpressionTemplates/test_expr12.cxx
 * \brief
 * \author Thomas Helfer
 * \date   16 Oct 2006
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

#include "TFEL/Math/tvector.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  tvector<3, double> v1(1.);
  tvector<3, double> v2(1.5);
  tvector<3, double> v3(1.76);
  assert(abs((v1 + v2 + v3)(0) - 4.26) < 1.e-14);
  assert(abs((v1 + v2 + v3)(1) - 4.26) < 1.e-14);
  assert(abs((v1 + v2 + v3)(2) - 4.26) < 1.e-14);
  return EXIT_SUCCESS;
}
