/*!
 * \file  tests/Math/invert.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <limits>
#include <cassert>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/TinyMatrixSolve.hxx"

template <typename T>
struct Name;

template <>
struct Name<long double> {
  static std::string getName() { return "long double"; }
};

template <>
struct Name<double> {
  static std::string getName() { return "double"; }
};

template <>
struct Name<float> {
  static std::string getName() { return "float"; }
};

template <typename T>
struct InvertTest2D final : public tfel::tests::TestCase {
  InvertTest2D()
      : tfel::tests::TestCase("TFEL/Math",
                              "InvertTest2D<" + Name<T>::getName() + ">") {
  }  // end of InvertTest2D

  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    tmatrix<2, 2, T> m;
    tmatrix<2, 2, T> inv;
    TinyPermutation<2> p;
    typename tmatrix<2, 2, T>::size_type i, j;
    const T eps = 100 * std::numeric_limits<T>::epsilon();

    m(0, 0) = 1.;
    m(0, 1) = 2.;
    m(1, 0) = 3.;
    m(1, 1) = 4.;

    TinyMatrixSolve<2, T>::decomp(m, p);
    for (i = 0; i != 2; ++i) {
      tvector<2, T> e(T(0));
      e(i) = 1.;
      TinyMatrixSolve<2, T>::back_substitute(m, p, e);
      for (j = 0; j != 2; ++j) {
        inv(j, i) = e(j);
      }
    }

    TFEL_TESTS_ASSERT(abs(inv(0, 0) + 2) < eps);
    TFEL_TESTS_ASSERT(abs(inv(0, 1) - 1) < eps);

    TFEL_TESTS_ASSERT(abs(inv(1, 0) - 1.5) < eps);
    TFEL_TESTS_ASSERT(abs(inv(1, 1) + 0.5) < eps);

    return this->result;

  }  // end of execute

};  // end of InvertTest2D

template <typename T>
struct InvertTest3D final : public tfel::tests::TestCase {
  InvertTest3D()
      : tfel::tests::TestCase("TFEL/Math",
                              "InvertTest3D<" + Name<T>::getName() + ">") {
  }  // end of InvertTest3D

  virtual tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    tmatrix<3, 3, T> m;
    tmatrix<3, 3, T> inv;
    TinyPermutation<3> p;
    typename tmatrix<3, 3, T>::size_type i, j;
    const T eps = 100 * std::numeric_limits<T>::epsilon();

    m(0, 0) = 0.;
    m(0, 1) = 2.;
    m(0, 2) = 1.;
    m(1, 0) = 1.;
    m(1, 1) = 0.;
    m(1, 2) = 0.;
    m(2, 0) = 3.;
    m(2, 1) = 0.;
    m(2, 2) = 1.;

    TinyMatrixSolve<3, T>::decomp(m, p);
    for (i = 0; i != 3; ++i) {
      tvector<3, T> e(T(0));
      e(i) = 1.;
      TinyMatrixSolve<3, T>::back_substitute(m, p, e);
      for (j = 0; j != 3; ++j) {
        inv(j, i) = e(j);
      }
    }

    TFEL_TESTS_ASSERT(abs(inv(0, 0)) < eps);
    TFEL_TESTS_ASSERT(abs(inv(0, 1) - 1) < eps);
    TFEL_TESTS_ASSERT(abs(inv(0, 2) - 0) < eps);

    TFEL_TESTS_ASSERT(abs(inv(1, 0) - 0.5) < eps);
    TFEL_TESTS_ASSERT(abs(inv(1, 1) - 1.5) < eps);
    TFEL_TESTS_ASSERT(abs(inv(1, 2) + 0.5) < eps);

    TFEL_TESTS_ASSERT(abs(inv(2, 0)) < eps);
    TFEL_TESTS_ASSERT(abs(inv(2, 1) + 3) < eps);
    TFEL_TESTS_ASSERT(abs(inv(2, 2) - 1) < eps);

    return this->result;

  }  // end of execute

};  // end of InvertTest3D

using FloatInvertTest2D = InvertTest2D<float>;
using DoubleInvertTest2D = InvertTest2D<double>;
using LongDoubleInvertTest2D = InvertTest2D<long double>;
using FloatInvertTest3D = InvertTest3D<float>;
using DoubleInvertTest3D = InvertTest3D<double>;
using LongDoubleInvertTest3D = InvertTest3D<long double>;

TFEL_TESTS_GENERATE_PROXY(FloatInvertTest2D, "InvertTest2D<float>");
TFEL_TESTS_GENERATE_PROXY(DoubleInvertTest2D, "InvertTest2D<double>");
TFEL_TESTS_GENERATE_PROXY(LongDoubleInvertTest2D, "InvertTest2D<long double>");
TFEL_TESTS_GENERATE_PROXY(FloatInvertTest3D, "InvertTest3D<float>");
TFEL_TESTS_GENERATE_PROXY(DoubleInvertTest3D, "InvertTest3D<double>");
TFEL_TESTS_GENERATE_PROXY(LongDoubleInvertTest3D, "InvertTest3D<long double>");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("Invert.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
