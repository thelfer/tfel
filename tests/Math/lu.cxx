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
#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Tests/TestFunctionWrapper.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/LUSolve.hxx"

template <typename T>
bool LUSolveTest() {
  using namespace tfel::math;
  constexpr auto eps = 10 * std::numeric_limits<T>::epsilon();
  matrix<T> m(3, 3);
  vector<T> b(3);
  m(0, 0) = 0;
  m(0, 1) = 2;
  m(0, 2) = 1;
  m(1, 0) = 1;
  m(1, 1) = 0;
  m(1, 2) = 0;
  m(2, 0) = 3;
  m(2, 1) = 0;
  m(2, 2) = 1;
  b(0) = 5;
  b(1) = -1;
  b(2) = -2;
  LUSolve::exe(m, b);
  return ((std::abs(b(0) + T{1}) < eps) &&  //
          (std::abs(b(1) - T{2}) < eps) &&  //
          (std::abs(b(2) - T{1}) < eps));
}  // end of LUSolveTest

template <typename T>
bool LUSolveTest2() {
  using namespace tfel::math;
  constexpr auto eps = 10 * std::numeric_limits<T>::epsilon();
  matrix<T> m(3, 3);
  vector<T> b(3);
  m(0, 0) = 3;
  m(0, 1) = -1;
  m(0, 2) = 2;
  m(1, 0) = 1;
  m(1, 1) = 2;
  m(1, 2) = 3;
  m(2, 0) = 2;
  m(2, 1) = -2;
  m(2, 2) = -1;
  b(0) = 12;
  b(1) = 11;
  b(2) = 2;
  LUSolve::exe(m, b);
  return ((std::abs(b(0) - T{3}) < eps) &&  //
          (std::abs(b(1) - T{1}) < eps) &&  //
          (std::abs(b(2) - T{2}) < eps));
}

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  using Wrapper1 = TestFunctionWrapper<LUSolveTest<long double>>;
  using Wrapper2 = TestFunctionWrapper<LUSolveTest<double>>;
  using Wrapper3 = TestFunctionWrapper<LUSolveTest<float>>;
  using Wrapper4 = TestFunctionWrapper<LUSolveTest2<double>>;
  using Wrapper5 = TestFunctionWrapper<LUSolveTest2<float>>;
  //  typedef TestFunctionWrapper<test2> Wrapper2;
  auto& m = TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("lu.xml");
  m.addTest("LUSolve", std::make_shared<Wrapper1>("LUSolveTest<long double>"));
  m.addTest("LUSolve", std::make_shared<Wrapper2>("LUSolveTest<double>"));
  m.addTest("LUSolve", std::make_shared<Wrapper3>("LUSolveTest<float>"));
  m.addTest("LUSolve - 2", std::make_shared<Wrapper4>("LUSolveTest2<double>"));
  m.addTest("LUSolve - 2", std::make_shared<Wrapper5>("LUSolveTest2<float>"));
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
