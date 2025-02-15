/*!
 * \file   tests/Math/stensor.cxx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
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

#include "TFEL/Math/stensor.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::typetraits;
  using stensor1 = stensor<1>;
  typedef stensor<1, int> stensor2;
  const stensor1 v1 = {4., 1., 25.};
  const stensor2 v2 = {2, 1, 5};
  const auto v3 = v1 + v2;
  assert(abs(v3(0) - 6) < 1.e-14);
  assert(abs(v3(1) - 2) < 1.e-14);
  assert(abs(v3(2) - 30) < 1.e-14);
  using res = UnaryResultType<stensor1, OpNeg>::type;
  static_assert(std::is_same<res, stensor1>::value, "type are differents");
  const auto v4 = -v1;
  assert(abs(v4(0) + 4) < 1.e-14);
  assert(abs(v4(1) + 1) < 1.e-14);
  assert(abs(v4(2) + 25) < 1.e-14);
  //
  constexpr stensor s = {1, 2, 3};
  static_assert(std::is_same_v<decltype(s), const stensor<1, int>>);
  static_assert(s[0] == 1);
  static_assert(s[1] == 2);
  static_assert(s[2] == 3);
  //
  constexpr auto zero = stensor<2u, int>::zero();
  static_assert(zero[0] == 0);
  static_assert(zero[1] == 0);
  static_assert(zero[2] == 0);
  static_assert(zero[3] == 0);
  return EXIT_SUCCESS;
}
