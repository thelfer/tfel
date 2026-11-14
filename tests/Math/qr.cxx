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

/*!
 * \return if the transpose of the Q matrix of the QR decomposition of the
 * given matrix multiplied by this matrix gives the R matrix
 * \param[in] A: matrix
 */
template <typename T>
bool checkQRDecomposition(const tfel::math::matrix<T>& A) {
  using namespace tfel::math;
  using size_type = typename matrix<T>::size_type;
  constexpr auto eps = 100 * std::numeric_limits<T>::epsilon();
  const auto m = A.getNbRows();
  const auto n = A.getNbCols();
  auto a = A;
  auto rdiag = vector<T>(n);
  auto beta = vector<T>(n);
  QRDecomp::exe(a, rdiag, beta);
  for (size_type j = 0; j != n; ++j) {
    auto v = vector<T>(m);
    for (size_type i = 0; i != m; ++i) {
      v(i) = A(i, j);
    }
    QRDecomp::tq_product(v, a, beta);
    for (size_type i = 0; i != m; ++i) {
      const auto r = (i == j) ? rdiag(i) : ((i < j) ? a(i, j) : T(0));
      if (!(std::abs(v(i) - r) < eps)) {
        return false;
      }
    }
  }
  return true;
}  // end of checkQRDecomposition

template <typename T>
bool RectangularQRDecompTest() {
  using namespace tfel::math;
  constexpr auto eps = 100 * std::numeric_limits<T>::epsilon();
  // more columns than rows, with a regular or a null last pivot
  const auto m1 = matrix<T>{{2, 1, 4}, {1, 3, 5}};
  const auto m2 = matrix<T>{{1, 1, 2}, {0, 0, 3}};
  // more rows than columns: least squares fit of y = c0 + c1 * x
  // for x = 1, 2, 3 and y = 1, 2, 4
  auto m3 = matrix<T>{{1, 1}, {1, 2}, {1, 3}};
  if (!(checkQRDecomposition(m1) && checkQRDecomposition(m2) &&
        checkQRDecomposition(m3))) {
    return false;
  }
  auto rdiag = vector<T>(2);
  auto beta = vector<T>(2);
  auto b = vector<T>{1, 2, 4};
  QRDecomp::exe(m3, rdiag, beta);
  QRDecomp::tq_product(b, m3, beta);
  QRDecomp::back_substitute(b, m3, rdiag);
  const auto e1 = std::abs(b(0) + T(2) / T(3));
  const auto e2 = std::abs(b(1) - T(3) / T(2));
  return ((e1 < eps) && (e2 < eps));
}  // end of RectangularQRDecompTest

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  using Wrapper1 = TestFunctionWrapper<QRDecompTest<long double>>;
  using Wrapper2 = TestFunctionWrapper<QRDecompTest<double>>;
  using Wrapper3 = TestFunctionWrapper<QRDecompTest<float>>;
  using Wrapper4 = TestFunctionWrapper<RectangularQRDecompTest<long double>>;
  using Wrapper5 = TestFunctionWrapper<RectangularQRDecompTest<double>>;
  using Wrapper6 = TestFunctionWrapper<RectangularQRDecompTest<float>>;
  auto& m = TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("qr.xml");
  m.addTest("QRDecomp",
            std::make_shared<Wrapper1>("QRDecompTest<long double>"));
  m.addTest("QRDecomp", std::make_shared<Wrapper2>("QRDecompTest<double>"));
  m.addTest("QRDecomp", std::make_shared<Wrapper3>("QRDecompTest<float>"));
  m.addTest("QRDecomp",
            std::make_shared<Wrapper4>("RectangularQRDecompTest<long double>"));
  m.addTest("QRDecomp",
            std::make_shared<Wrapper5>("RectangularQRDecompTest<double>"));
  m.addTest("QRDecomp",
            std::make_shared<Wrapper6>("RectangularQRDecompTest<float>"));
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
