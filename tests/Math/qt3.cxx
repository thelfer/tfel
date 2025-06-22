/*!
 * \file   tests/Math/qt3.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/08/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>

#include "TFEL/Math/qt.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::math;
  using namespace std;

  qt<Mass, int> a(-12);
  qt<Mass, int> b(14);

  assert(a <= b);
  assert(a < b);
  assert(b > a);
  assert(b >= a);
  assert(b != a);
  assert(!(b == a));
  assert(abs(a) == 12);

  return EXIT_SUCCESS;
}
