/*!
 * \file   tests/Math/tvector.cxx
 * \brief
 * \author Thomas Helfer
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

#include "TFEL/Math/qt.hxx"
#include "TFEL/Math/tvector.hxx"

/* coverity [root_function]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  using force = qt<Force>;
  using avector = tvector<5, acceleration>;
  using fvector = tvector<5, force>;
  constexpr mass m = mass(2);
  const avector a2 = {acceleration(2), acceleration(1), acceleration(4),
                      acceleration(2), acceleration(3)};
  avector a1;
  fvector f;
  a1 = 2 * (a2 + a2);
  assert(abs(a1(0).getValue() - 8) == 0);
  assert(abs(a1(1).getValue() - 4) == 0);
  assert(abs(a1(2).getValue() - 16) == 0);
  assert(abs(a1(3).getValue() - 8) == 0);
  assert(abs(a1(4).getValue() - 12) == 0);
  f = m * a1;
  assert(abs(f(0).getValue() - 16) < 1.e-14);
  assert(abs(f(1).getValue() - 8) < 1.e-14);
  assert(abs(f(2).getValue() - 32) < 1.e-14);
  assert(abs(f(3).getValue() - 16) < 1.e-14);
  assert(abs(f(4).getValue() - 24) < 1.e-14);
  return EXIT_SUCCESS;
}
