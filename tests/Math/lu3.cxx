/*!
 * \file   tests/Math/lu3.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  14 avr 2009
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

#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Tests/TestFunctionWrapper.hxx"

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Matrix/tmatrixIO.hxx"
#include "TFEL/Math/TinyMatrixSolve.hxx"

template <typename T>
bool TinyMatrixSolveTest() {
  constexpr auto eps = 10 * std::numeric_limits<T>::epsilon();
  auto m = tfel::math::tmatrix<3, 3, T>{0, 2, 1,  //
                                        1, 0, 0,  //
                                        3, 0, 1};
  auto b = tfel::math::tmatrix<3, 2, T>{5,  5,   //
                                        -1, -1,  //
                                        -2, -2};
  tfel::math::TinyMatrixSolve<3u, T>::exe(m, b);
  return ((std::abs(b(0, 0) + 1) < eps) &&  //
          (std::abs(b(1, 0) - 2) < eps) &&  //
          (std::abs(b(2, 0) - 1) < eps) &&  //
          (std::abs(b(0, 1) + 1) < eps) &&  //
          (std::abs(b(1, 1) - 2) < eps) &&  //
          (std::abs(b(2, 1) - 1) < eps));
}  // end of TinyMatrixSolveTest

template <typename T>
bool TinyMatrixSolveTest2() {
  constexpr auto eps = 100 * std::numeric_limits<T>::epsilon();
  auto m = tfel::math::tmatrix<4, 4, T>{0, 2,  0,  1,  //
                                        2, 2,  3,  2,  //
                                        4, -3, 0,  1,  //
                                        6, 1,  -6, -5};
  auto b = tfel::math::tmatrix<4, 2, T>{0,  0,    //
                                        -2, -12,  //
                                        -7, -42,  //
                                        6,  36};
  tfel::math::TinyMatrixSolve<4u, T>::exe(m, b);
  return ((std::abs(b(0, 0) + T(0.5)) < eps) &&    //
          (std::abs(b(1, 0) - 1) < eps) &&         //
          (std::abs(b(2, 0) - T(1) / 3) < eps) &&  //
          (std::abs(b(3, 0) + 2) < eps) &&         //
          (std::abs(b(0, 1) + 3) < eps) &&         //
          (std::abs(b(1, 1) - 6) < eps) &&         //
          (std::abs(b(2, 1) - 2) < eps) &&         //
          (std::abs(b(3, 1) + 12) < eps));
}  // end of TinyMatrixSolveTest

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  using Wrapper1 = TestFunctionWrapper<TinyMatrixSolveTest<long double>>;
  using Wrapper2 = TestFunctionWrapper<TinyMatrixSolveTest<double>>;
  using Wrapper3 = TestFunctionWrapper<TinyMatrixSolveTest<float>>;
  using Wrapper4 = TestFunctionWrapper<TinyMatrixSolveTest2<long double>>;
  using Wrapper5 = TestFunctionWrapper<TinyMatrixSolveTest2<double>>;
  using Wrapper6 = TestFunctionWrapper<TinyMatrixSolveTest2<float>>;
  //  typedef TestFunctionWrapper<test2> Wrapper2;
  auto& m = TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("lu3.xml");
  m.addTest("TinyMatrixSolve",
            std::make_shared<Wrapper1>("TinyMatrixSolveTest<long double>"));
  m.addTest("TinyMatrixSolve",
            std::make_shared<Wrapper2>("TinyMatrixSolveTest<double>"));
  m.addTest("TinyMatrixSolve",
            std::make_shared<Wrapper3>("TinyMatrixSolveTest<float>"));
  m.addTest("TinyMatrixSolve",
            std::make_shared<Wrapper4>("TinyMatrixSolveTest2<long double>"));
  m.addTest("TinyMatrixSolve",
            std::make_shared<Wrapper5>("TinyMatrixSolveTest2<double>"));
  m.addTest("TinyMatrixSolve",
            std::make_shared<Wrapper6>("TinyMatrixSolveTest2<float>"));
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
