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
  constexpr auto eps = 100 * std::numeric_limits<T>::epsilon();
  auto m = matrix<T>(3, 3);
  auto rdiag = vector<T>(3);
  auto beta = vector<T>(3);
  auto b = vector<T>(3);
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
  QRDecomp::exe(m, rdiag, beta);
  QRDecomp::tq_product(b, m, beta);
  QRDecomp::back_substitute(b, m, rdiag);
  const auto e1 = std::abs(b(0) + 1);
  const auto e2 = std::abs(b(1) - 2);
  const auto e3 = std::abs(b(2) - 1);
  return ((e1 < eps) && (e2 < eps) && (e3 < eps));
}  // end of QRDecompTest

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  using Wrapper1 = TestFunctionWrapper<QRDecompTest<long double>>;
  using Wrapper2 = TestFunctionWrapper<QRDecompTest<double>>;
  using Wrapper3 = TestFunctionWrapper<QRDecompTest<float>>;
  auto& m = TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("qr.xml");
  m.addTest("QRDecomp",
            std::make_shared<Wrapper1>("QRDecompTest<long double>"));
  m.addTest("QRDecomp", std::make_shared<Wrapper2>("QRDecompTest<double>"));
  m.addTest("QRDecomp", std::make_shared<Wrapper3>("QRDecompTest<float>"));
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
