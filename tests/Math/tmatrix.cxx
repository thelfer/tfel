/*!
 * \file   tests/Math/tmatrix.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   03 jui 2006
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

#include <cstdlib>
#include "TFEL/Math/tmatrix.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::math;
  //
  constexpr auto m2 = []() constexpr->tfel::math::tmatrix<2, 2, int> {
    auto m = tfel::math::tmatrix<2, 2, int>{};
    m(0, 0) = 0;
    m(0, 1) = 1;
    m(1, 0) = 2;
    m(1, 1) = 3;
    return 2 * m;
  }
  ();
  static_assert(m2(0, 0) == 0);
  static_assert(m2(0, 1) == 2);
  static_assert(m2(1, 0) == 4);
  static_assert(m2(1, 1) == 6);
  //
  constexpr auto m3 = []() constexpr->tfel::math::tmatrix<2, 2, int> {
    const auto m = tfel::math::tmatrix<2, 2, int>{0, 1,  //
                                                  2, 3};
    return 2 * m;
  }
  ();
  static_assert(m3(0, 0) == 0);
  static_assert(m3(0, 1) == 2);
  static_assert(m3(1, 0) == 4);
  static_assert(m3(1, 1) == 6);
  //
  constexpr auto m4 = []() constexpr->tfel::math::tmatrix<2, 2, int> {
    const auto m = tfel::math::tmatrix<2, 2, int>{{0, 1},  //
                                                  {2, 3}};
    return 2 * m;
  }
  ();
  static_assert(m4(0, 0) == 0);
  static_assert(m4(0, 1) == 2);
  static_assert(m4(1, 0) == 4);
  static_assert(m4(1, 1) == 6);
  //
  constexpr auto m5 = []() constexpr->tfel::math::tmatrix<2, 2, int> {
    const auto m = tfel::math::tmatrix{{0, 1},  //
                                       {2, 3}};
    return 2 * m;
  }
  ();
  static_assert(m5(0, 0) == 0);
  static_assert(m5(0, 1) == 2);
  static_assert(m5(1, 0) == 4);
  static_assert(m5(1, 1) == 6);
  //
  constexpr auto m6 = []() constexpr->tfel::math::tmatrix<2, 2, int> {
    auto m = tfel::math::tmatrix<2, 2, int>{};
    m = {{0, 1},  //
         {2, 3}};
    return 2 * m;
  }
  ();
  static_assert(m6(0, 0) == 0);
  static_assert(m6(0, 1) == 2);
  static_assert(m6(1, 0) == 4);
  static_assert(m6(1, 1) == 6);
  return EXIT_SUCCESS;
}
