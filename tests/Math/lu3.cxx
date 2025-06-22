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
  tfel::math::tmatrix<3, 3, T> m;
  tfel::math::tmatrix<3, 2, T> b;

  m(0, 0) = 0.;
  m(0, 1) = 2.;
  m(0, 2) = 1.;
  m(1, 0) = 1.;
  m(1, 1) = 0.;
  m(1, 2) = 0.;
  m(2, 0) = 3.;
  m(2, 1) = 0.;
  m(2, 2) = 1.;

  b(0, 0) = 5;
  b(1, 0) = -1;
  b(2, 0) = -2;
  b(0, 1) = 5;
  b(1, 1) = -1;
  b(2, 1) = -2;

  tfel::math::TinyMatrixSolve<3u, T>::exe(m, b);
  return ((std::abs(b(0, 0) + 1) < 10 * std::numeric_limits<T>::epsilon()) &&
          (std::abs(b(1, 0) - 2) < 10 * std::numeric_limits<T>::epsilon()) &&
          (std::abs(b(2, 0) - 1) < 10 * std::numeric_limits<T>::epsilon()) &&
          (std::abs(b(0, 1) + 1) < 10 * std::numeric_limits<T>::epsilon()) &&
          (std::abs(b(1, 1) - 2) < 10 * std::numeric_limits<T>::epsilon()) &&
          (std::abs(b(2, 1) - 1) < 10 * std::numeric_limits<T>::epsilon()));

}  // end of TinyMatrixSolveTest

template <typename T>
bool TinyMatrixSolveTest2() {
  tfel::math::tmatrix<4, 4, T> m = {0, 2,  0,  1,  //
                                    2, 2,  3,  2,  //
                                    4, -3, 0,  1,  //
                                    6, 1,  -6, -5};
  tfel::math::tmatrix<4, 2, T> b = {0,  0,    //
                                    -2, -12,  //
                                    -7, -42,  //
                                    6,  36};

  tfel::math::TinyMatrixSolve<4u, T>::exe(m, b);

  return (
      (std::abs(b(0, 0) + T(0.5)) < 10 * std::numeric_limits<T>::epsilon()) &&
      (std::abs(b(1, 0) - 1) < 10 * std::numeric_limits<T>::epsilon()) &&
      (std::abs(b(2, 0) - T(1) / 3) < 10 * std::numeric_limits<T>::epsilon()) &&
      (std::abs(b(3, 0) + 2) < 10 * std::numeric_limits<T>::epsilon()) &&
      (std::abs(b(0, 1) + 3) < 100 * std::numeric_limits<T>::epsilon()) &&
      (std::abs(b(1, 1) - 6) < 100 * std::numeric_limits<T>::epsilon()) &&
      (std::abs(b(2, 1) - 2) < 100 * std::numeric_limits<T>::epsilon()) &&
      (std::abs(b(3, 1) + 12) < 100 * std::numeric_limits<T>::epsilon()));
}  // end of TinyMatrixSolveTest

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using Wrapper1 =
      tfel::tests::TestFunctionWrapper<TinyMatrixSolveTest<long double>>;
  using Wrapper2 =
      tfel::tests::TestFunctionWrapper<TinyMatrixSolveTest<double>>;
  using Wrapper3 = tfel::tests::TestFunctionWrapper<TinyMatrixSolveTest<float>>;
  using Wrapper4 =
      tfel::tests::TestFunctionWrapper<TinyMatrixSolveTest2<long double>>;
  using Wrapper5 =
      tfel::tests::TestFunctionWrapper<TinyMatrixSolveTest2<double>>;
  using Wrapper6 =
      tfel::tests::TestFunctionWrapper<TinyMatrixSolveTest2<float>>;
  //  typedef TestFunctionWrapper<test2> Wrapper2;
  auto& m = tfel::tests::TestManager::getTestManager();
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
