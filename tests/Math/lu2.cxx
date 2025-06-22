/*!
 * \file   tests/Math/lu2.cxx
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
#include "TFEL/Math/TinyMatrixSolve.hxx"

template <typename T>
bool TinyMatrixSolveTest() {
  using namespace std;
  using namespace tfel::math;
  tmatrix<3, 3, T> m;
  tvector<3, T> b;

  m(0, 0) = 0.;
  m(0, 1) = 2.;
  m(0, 2) = 1.;
  m(1, 0) = 1.;
  m(1, 1) = 0.;
  m(1, 2) = 0.;
  m(2, 0) = 3.;
  m(2, 1) = 0.;
  m(2, 2) = 1.;

  b(0) = 5;
  b(1) = -1;
  b(2) = -2;

  TinyMatrixSolve<3u, T>::exe(m, b);
  return ((abs(b(0) + 1) < 10 * numeric_limits<T>::epsilon()) &&
          (abs(b(1) - 2) < 10 * numeric_limits<T>::epsilon()) &&
          (abs(b(2) - 1) < 10 * numeric_limits<T>::epsilon()));

}  // end of TinyMatrixSolveTest

template <typename T>
bool TinyMatrixSolveTest2() {
  using namespace std;
  using namespace tfel::math;
  tmatrix<3, 3, T> m;
  tvector<3, T> b;

  m(0, 0) = 3.;
  m(0, 1) = -1.;
  m(0, 2) = 2.;
  m(1, 0) = 1.;
  m(1, 1) = 2.;
  m(1, 2) = 3.;
  m(2, 0) = 2.;
  m(2, 1) = -2.;
  m(2, 2) = -1.;

  b(0) = 12;
  b(1) = 11;
  b(2) = 2;

  TinyMatrixSolve<3u, T>::exe(m, b);
  return ((abs(b(0) - 3) < 10 * numeric_limits<T>::epsilon()) &&
          (abs(b(1) - 1) < 10 * numeric_limits<T>::epsilon()) &&
          (abs(b(2) - 2) < 10 * numeric_limits<T>::epsilon()));
}

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  using Wrapper1 = TestFunctionWrapper<TinyMatrixSolveTest<long double>>;
  using Wrapper2 = TestFunctionWrapper<TinyMatrixSolveTest<double>>;
  using Wrapper3 = TestFunctionWrapper<TinyMatrixSolveTest<float>>;
  using Wrapper4 = TestFunctionWrapper<TinyMatrixSolveTest2<long double>>;
  using Wrapper5 = TestFunctionWrapper<TinyMatrixSolveTest2<double>>;
  using Wrapper6 = TestFunctionWrapper<TinyMatrixSolveTest2<float>>;
  //  typedef TestFunctionWrapper<test2> Wrapper2;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("lu2.xml");
  manager.addTest(
      "TinyMatrixSolve",
      shared_ptr<Test>(new Wrapper1("TinyMatrixSolveTest<long double>")));
  manager.addTest(
      "TinyMatrixSolve",
      shared_ptr<Test>(new Wrapper2("TinyMatrixSolveTest<double>")));
  manager.addTest("TinyMatrixSolve",
                  shared_ptr<Test>(new Wrapper3("TinyMatrixSolveTest<float>")));
  manager.addTest(
      "TinyMatrixSolve - 2",
      shared_ptr<Test>(new Wrapper4("TinyMatrixSolveTest2<long double>")));
  manager.addTest(
      "TinyMatrixSolve - 2",
      shared_ptr<Test>(new Wrapper5("TinyMatrixSolveTest2<double>")));
  manager.addTest(
      "TinyMatrixSolve - 2",
      shared_ptr<Test>(new Wrapper6("TinyMatrixSolveTest2<float>")));
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
