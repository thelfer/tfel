/*!
 * \file  tests/Math/qr.cxx
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
#include "TFEL/Math/QR/QRDecomp.hxx"

template <typename T>
bool QRDecompTest() {
  using namespace tfel::math;
  matrix<T> m(3, 3);
  vector<T> rdiag(3);
  vector<T> beta(3);
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
  QRDecomp::exe(m, rdiag, beta);
  QRDecomp::tq_product(b, m, beta);
  QRDecomp::back_substitute(b, m, rdiag);
  const auto e1 = std::abs(b(0) + 1);
  const auto e2 = std::abs(b(1) - 2);
  const auto e3 = std::abs(b(2) - 1);
  return ((e1 < 100 * std::numeric_limits<T>::epsilon()) &&
          (e2 < 100 * std::numeric_limits<T>::epsilon()) &&
          (e3 < 100 * std::numeric_limits<T>::epsilon()));
}  // end of QRDecompTest

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  using Wrapper1 = TestFunctionWrapper<QRDecompTest<long double>>;
  using Wrapper2 = TestFunctionWrapper<QRDecompTest<double>>;
  using Wrapper3 = TestFunctionWrapper<QRDecompTest<float>>;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("qr.xml");
  manager.addTest("QRDecomp",
                  shared_ptr<Test>(new Wrapper1("QRDecompTest<long double>")));
  manager.addTest("QRDecomp",
                  shared_ptr<Test>(new Wrapper2("QRDecompTest<double>")));
  manager.addTest("QRDecomp",
                  shared_ptr<Test>(new Wrapper3("QRDecompTest<float>")));
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
