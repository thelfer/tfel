/*!
 * \file  tests/Math/invert2.cxx
 * \brief
 * \author Thomas Helfer
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
#include "TFEL/Math/TinyMatrixInvert.hxx"

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
struct TinyMatrixInvertTest2D final : public tfel::tests::TestCase {
  TinyMatrixInvertTest2D()
      : tfel::tests::TestCase(
            "TFEL/Math", "TinyMatrixInvertTest2D<" + Name<T>::getName() + ">") {
  }  // end of TinyMatrixInvertTest2D

  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    const auto eps = 100 * std::numeric_limits<T>::epsilon();
    const auto one_half = 1 / T(2);
    tmatrix<2, 2, T> m = {1, 2, 3, 4};
    TinyMatrixInvert<2, T>::exe(m);
    TFEL_TESTS_ASSERT(std::abs(m(0, 0) + 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(0, 1) - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(1, 0) - 3 * one_half) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(1, 1) + one_half) < eps);
    return this->result;
  }  // end of execute

};  // end of TinyMatrixInvertTest2D

template <typename T>
struct TinyMatrixInvertTest3D final : public tfel::tests::TestCase {
  TinyMatrixInvertTest3D()
      : tfel::tests::TestCase(
            "TFEL/Math", "TinyMatrixInvertTest3D<" + Name<T>::getName() + ">") {
  }  // end of TinyMatrixInvertTest3D

  tfel::tests::TestResult execute() override {
    using namespace tfel::math;
    const auto eps = 100 * std::numeric_limits<T>::epsilon();
    const auto one_half = 1 / T(2);
    tmatrix<3, 3, T> m = {0, 2, 1, 1, 0, 0, 3, 0, 1};
    TinyMatrixInvert<3, T>::exe(m);
    TFEL_TESTS_ASSERT(std::abs(m(0, 0)) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(0, 1) - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(0, 2) - 0) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(1, 0) - one_half) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(1, 1) - 3 * one_half) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(1, 2) + one_half) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(2, 0)) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(2, 1) + 3) < eps);
    TFEL_TESTS_ASSERT(std::abs(m(2, 2) - 1) < eps);
    return this->result;
  }  // end of execute

};  // end of TinyMatrixInvertTest3D

using FloatTinyMatrixInvertTest2D = TinyMatrixInvertTest2D<float>;
using DoubleTinyMatrixInvertTest2D = TinyMatrixInvertTest2D<double>;
using LongDoubleTinyMatrixInvertTest2D = TinyMatrixInvertTest2D<long double>;
using FloatTinyMatrixInvertTest3D = TinyMatrixInvertTest3D<float>;
using DoubleTinyMatrixInvertTest3D = TinyMatrixInvertTest3D<double>;
using LongDoubleTinyMatrixInvertTest3D = TinyMatrixInvertTest3D<long double>;

TFEL_TESTS_GENERATE_PROXY(FloatTinyMatrixInvertTest2D,
                          "TinyMatrixInvertTest2D<float>");
TFEL_TESTS_GENERATE_PROXY(DoubleTinyMatrixInvertTest2D,
                          "TinyMatrixInvertTest2D<double>");
TFEL_TESTS_GENERATE_PROXY(LongDoubleTinyMatrixInvertTest2D,
                          "TinyMatrixInvertTest2D<long double>");
TFEL_TESTS_GENERATE_PROXY(FloatTinyMatrixInvertTest3D,
                          "TinyMatrixInvertTest3D<float>");
TFEL_TESTS_GENERATE_PROXY(DoubleTinyMatrixInvertTest3D,
                          "TinyMatrixInvertTest3D<double>");
TFEL_TESTS_GENERATE_PROXY(LongDoubleTinyMatrixInvertTest3D,
                          "TinyMatrixInvertTest3D<long double>");

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
