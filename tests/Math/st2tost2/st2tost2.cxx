/*!
 * \file   tests/Math/st2tost2/st2tost2.cxx
 * \brief
 * \author Thomas Helfer
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
#include <type_traits>
#include "TFEL/Math/st2tost2.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

static void test1() {
  using namespace std;
  using namespace tfel::math;
  constexpr st2tost2<3> s1(1.5);
  constexpr st2tost2<3> s2(4.);
  constexpr st2tost2<3> s3 = s1 + 0.5 * s2;
  static_assert(my_abs(s3(0, 0) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(0, 1) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(0, 2) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(0, 3) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(1, 0) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(1, 1) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(1, 2) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(1, 3) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(2, 0) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(2, 1) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(2, 2) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(2, 3) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(3, 0) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(3, 1) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(3, 2) - 3.5) < 1.e-14);
  static_assert(my_abs(s3(3, 3) - 3.5) < 1.e-14);
}

static void test2() {
  using namespace tfel::math;
  constexpr auto s = st2tost2{{1, 0, 2},  //
                              {-1, 1, 6},
                              {3, 4, 1}};
  static_assert(std::is_same_v<decltype(s), const st2tost2<1u, int>>);
  static_assert(s(0, 0) == 1);
  static_assert(s(0, 1) == 0);
  static_assert(s(0, 2) == 2);
  static_assert(s(1, 0) == -1);
  static_assert(s(1, 1) == 1);
  static_assert(s(1, 2) == 6);
  static_assert(s(2, 0) == 3);
  static_assert(s(2, 1) == 4);
  static_assert(s(2, 2) == 1);
}

static void test3() {
  using namespace tfel::math;
  constexpr st2tost2<1u, int> s = {{1, 0, 2},  //
                                   {-1, 1, 6},
                                   {3, 4, 1}};
  static_assert(std::is_same_v<decltype(s), const st2tost2<1u, int>>);
  static_assert(s(0, 0) == 1);
  static_assert(s(0, 1) == 0);
  static_assert(s(0, 2) == 2);
  static_assert(s(1, 0) == -1);
  static_assert(s(1, 1) == 1);
  static_assert(s(1, 2) == 6);
  static_assert(s(2, 0) == 3);
  static_assert(s(2, 1) == 4);
  static_assert(s(2, 2) == 1);
}

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  test1();
  test2();
  test3();
  return EXIT_SUCCESS;
}
