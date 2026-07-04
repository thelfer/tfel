/*!
 * \file  tests/Math/lu.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 14 avr 2009
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
#include <limits>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <tdls/tdls.hpp>
#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Tests/TestFunctionWrapper.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

template <typename T>
[[nodiscard]] bool TDLSSolveInPlaceTest() noexcept {
  constexpr auto eps = 10 * std::numeric_limits<T>::epsilon();
  auto m = tfel::math::tmatrix<3, 3, T>{0, 2, 1,  //
                                        1, 0, 0,  //
                                        3, 0, 1};
  auto b = tfel::math::tvector<3, T>{5, -1, -2};
  tfel::math::tvector<3, int> piv;
  tdls::solve_inplace(m, piv, b);
  return ((std::abs(b(0) + T{1}) < eps) &&  //
          (std::abs(b(1) - T{2}) < eps) &&  //
          (std::abs(b(2) - T{1}) < eps));
}  // end of TDLSSolveInPlaceTest

template <typename T>
[[nodiscard]] bool TDLSSolveInPlaceTest2() noexcept {
  constexpr auto eps = 10 * std::numeric_limits<T>::epsilon();
  auto m = tfel::math::tmatrix<3, 3, T>{3, -1, 2,  //
                                        1, 2,  3,  //
                                        2, -2, -1};
  auto b = tfel::math::tvector<3, T>{12, 11, 2};
  auto piv = tfel::math::tvector<3, int>{};
  tdls::solve_inplace(m, piv, b);
  return ((std::abs(b(0) - T{3}) < eps) &&  //
          (std::abs(b(1) - T{1}) < eps) &&  //
          (std::abs(b(2) - T{2}) < eps));
}

template <typename T>
[[nodiscard]] bool TDLSSolveInPlaceTest3() noexcept {
  constexpr auto eps = 100 * std::numeric_limits<T>::epsilon();
  auto m = tfel::math::tmatrix<4, 4, T>{0, 2,  0,  1,  //
                                        2, 2,  3,  2,  //
                                        4, -3, 0,  1,  //
                                        6, 1,  -6, -5};
  auto b = tfel::math::tvector<4, T>{0, -2, -7, 6};
  //
  auto m2 = m;
  auto b2 = tfel::math::tvector<4, T>{0, -12, -42, 36};
  //
  auto piv = tfel::math::tvector<4, int>{};
  tdls::solve_inplace(m, piv, b);
  tdls::solve_inplace(m2, piv, b2);
  return ((std::abs(b(0) + T(0.5)) < eps) &&    //
          (std::abs(b(1) - 1) < eps) &&         //
          (std::abs(b(2) - T(1) / 3) < eps) &&  //
          (std::abs(b(3) + 2) < eps) &&         //
          (std::abs(b2(0) + 3) < eps) &&        //
          (std::abs(b2(1) - 6) < eps) &&        //
          (std::abs(b2(2) - 2) < eps) &&        //
          (std::abs(b2(3) + 12) < eps));
}  // end of TinyMatrixSolveTest

template <typename T>
[[nodiscard]] bool TDLSSolveInPlaceTest4() noexcept {
  constexpr auto eps = 100 * std::numeric_limits<T>::epsilon();
  auto m = tfel::math::tmatrix<4, 4, T>{0, 2,  0,  1,  //
                                        2, 2,  3,  2,  //
                                        4, -3, 0,  1,  //
                                        6, 1,  -6, -5};
  const auto b = tfel::math::tvector<4, T>{0, -2, -7, 6};
  const auto b2 = tfel::math::tvector<4, T>{0, -12, -42, 36};
  auto piv = tfel::math::tvector<4, int>{};
  tdls::factorize(m, piv);
  auto x = tfel::math::tvector<4, T>{};
  auto x2 = tfel::math::tvector<4, T>{};
  tdls::substitute(m, piv, b, x);
  tdls::substitute(m, piv, b2, x2);
  return ((std::abs(x(0) + T(0.5)) < eps) &&    //
          (std::abs(x(1) - 1) < eps) &&         //
          (std::abs(x(2) - T(1) / 3) < eps) &&  //
          (std::abs(x(3) + 2) < eps) &&         //
          (std::abs(x2(0) + 3) < eps) &&        //
          (std::abs(x2(1) - 6) < eps) &&        //
          (std::abs(x2(2) - 2) < eps) &&        //
          (std::abs(x2(3) + 12) < eps));
}  // end of TinyMatrixSolveTest

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  using Wrapper1 = TestFunctionWrapper<TDLSSolveInPlaceTest<long double>>;
  using Wrapper2 = TestFunctionWrapper<TDLSSolveInPlaceTest<double>>;
  using Wrapper3 = TestFunctionWrapper<TDLSSolveInPlaceTest<float>>;
  using Wrapper4 = TestFunctionWrapper<TDLSSolveInPlaceTest2<double>>;
  using Wrapper5 = TestFunctionWrapper<TDLSSolveInPlaceTest2<float>>;
  using Wrapper6 = TestFunctionWrapper<TDLSSolveInPlaceTest3<long double>>;
  using Wrapper7 = TestFunctionWrapper<TDLSSolveInPlaceTest3<double>>;
  using Wrapper8 = TestFunctionWrapper<TDLSSolveInPlaceTest3<float>>;
  using Wrapper9 = TestFunctionWrapper<TDLSSolveInPlaceTest4<long double>>;
  using Wrapper10 = TestFunctionWrapper<TDLSSolveInPlaceTest4<double>>;
  using Wrapper11 = TestFunctionWrapper<TDLSSolveInPlaceTest4<float>>;
  //  typedef TestFunctionWrapper<test2> Wrapper2;
  auto& m = TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("lu-tdsl.xml");
  m.addTest("TDLSSolveInPlace",
            std::make_shared<Wrapper1>("TDLSSolveInPlaceTest<long double>"));
  m.addTest("TDLSSolveInPlace",
            std::make_shared<Wrapper2>("TDLSSolveInPlaceTest<double>"));
  m.addTest("TDLSSolveInPlace",
            std::make_shared<Wrapper3>("TDLSSolveInPlaceTest<float>"));
  m.addTest("TDLSSolveInPlace - 2",
            std::make_shared<Wrapper4>("TDLSSolveInPlaceTest2<double>"));
  m.addTest("TDLSSolveInPlace - 2",
            std::make_shared<Wrapper5>("TDLSSolveInPlaceTest2<float>"));
  m.addTest("TDLSSolveInPlace - 3",
            std::make_shared<Wrapper6>("TDLSSolveInPlaceTest3<long double>"));
  m.addTest("TDLSSolveInPlace - 3",
            std::make_shared<Wrapper7>("TDLSSolveInPlaceTest3<double>"));
  m.addTest("TDLSSolveInPlace - 3",
            std::make_shared<Wrapper8>("TDLSSolveInPlaceTest3<float>"));
  m.addTest("TDLSSolveInPlace - 3",
            std::make_shared<Wrapper9>("TDLSSolveInPlaceTest4<long double>"));
  m.addTest("TDLSSolveInPlace - 3",
            std::make_shared<Wrapper10>("TDLSSolveInPlaceTest4<double>"));
  m.addTest("TDLSSolveInPlace - 3",
            std::make_shared<Wrapper11>("TDLSSolveInPlaceTest4<float>"));
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
