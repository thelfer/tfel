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
  using namespace std;
  using namespace tfel::math;
  using tfel::math::vector;
  matrix<T> m(3, 3);
  vector<T> b(3);

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

  LUSolve::exe(m, b);
  return ((abs(b(0) + 1) < 10 * numeric_limits<T>::epsilon()) &&
          (abs(b(1) - 2) < 10 * numeric_limits<T>::epsilon()) &&
          (abs(b(2) - 1) < 10 * numeric_limits<T>::epsilon()));

}  // end of LUSolveTest

template <typename T>
bool LUSolveTest2() {
  using namespace std;
  using namespace tfel::math;
  using tfel::math::vector;
  matrix<double> m(3, 3);
  vector<double> b(3);

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

  LUSolve::exe(m, b);
  return ((abs(b(0) - 3) < 10 * numeric_limits<T>::epsilon()) &&
          (abs(b(1) - 1) < 10 * numeric_limits<T>::epsilon()) &&
          (abs(b(2) - 2) < 10 * numeric_limits<T>::epsilon()));
}

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  using Wrapper1 = TestFunctionWrapper<LUSolveTest<long double>>;
  using Wrapper2 = TestFunctionWrapper<LUSolveTest<double>>;
  using Wrapper3 = TestFunctionWrapper<LUSolveTest<float>>;
  using Wrapper4 = TestFunctionWrapper<LUSolveTest2<double>>;
  using Wrapper5 = TestFunctionWrapper<LUSolveTest2<float>>;
  //  typedef TestFunctionWrapper<test2> Wrapper2;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("lu.xml");
  manager.addTest("LUSolve",
                  shared_ptr<Test>(new Wrapper1("LUSolveTest<long double>")));
  manager.addTest("LUSolve",
                  shared_ptr<Test>(new Wrapper2("LUSolveTest<double>")));
  manager.addTest("LUSolve",
                  shared_ptr<Test>(new Wrapper3("LUSolveTest<float>")));
  manager.addTest("LUSolve - 2",
                  shared_ptr<Test>(new Wrapper4("LUSolveTest2<double>")));
  manager.addTest("LUSolve - 2",
                  shared_ptr<Test>(new Wrapper5("LUSolveTest2<float>")));
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
