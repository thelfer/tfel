/*!
 * \file   tests/Math/BissectionAlgorithmTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   11/10/2020
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
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/NonLinearSolvers/BissectionAlgorithmBase.hxx"

struct BissectionAlgorithmTest final : public tfel::tests::TestCase {
  BissectionAlgorithmTest()
      : tfel::tests::TestCase("TFEL/Math", "BissectionAlgorithmTest") {
  }  // end of BissectionAlgorithmTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    constexpr const auto eps = double(1.e-14);
    constexpr const auto x1 = double(7) / 5;
    constexpr const auto x2 = double(1.2);
    auto a = tfel::math::BissectionAlgorithmBase<double>();
    const auto& d = a.getData();
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.xmin));
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.xmax));
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.fmin));
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.fmax));
    a.updateBounds(1, 2);
    TFEL_TESTS_ASSERT(std::abs(d.xmin - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmin - 2) < eps);
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.xmax));
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.fmax));
    a.updateBounds(2, -3);
    TFEL_TESTS_ASSERT(std::abs(d.xmin - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmin - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.xmax - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmax + 3) < eps);
    auto x = double{3};
    a.iterate(x);
    TFEL_TESTS_ASSERT(std::abs(x - x1) < eps);
    a.updateBounds(1.4, -1);
    TFEL_TESTS_ASSERT(std::abs(d.xmin - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmin - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.xmax - 1.4) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmax + 1) < eps);
    x = x2;
    a.iterate(x);
    TFEL_TESTS_ASSERT(std::abs(x - x2) < eps);
  }  // end of execute
  void test2() {
    constexpr const auto eps = double(1.e-14);
    constexpr const auto x1 = double(7) / 5;
    constexpr const auto x2 = double(1.2);
    auto a = tfel::math::BissectionAlgorithmBase<double>();
    const auto& d = a.getData();
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.xmin));
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.xmax));
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.fmin));
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.fmax));
    a.updateBounds(2, -3);
    TFEL_TESTS_ASSERT(std::abs(d.xmin - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmin + 3) < eps);
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.xmax));
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.fmax));
    a.updateBounds(1, 2);
    TFEL_TESTS_ASSERT(std::abs(d.xmin - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmin - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.xmax - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmax + 3) < eps);
    auto x = double{3};
    a.iterate(x);
    TFEL_TESTS_ASSERT(std::abs(x - x1) < eps);
    a.updateBounds(1.4, -1);
    TFEL_TESTS_ASSERT(std::abs(d.xmin - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmin - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.xmax - 1.4) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmax + 1) < eps);
    x = x2;
    a.iterate(x);
    TFEL_TESTS_ASSERT(std::abs(x - x2) < eps);
  }  // end of execute
  void test3() {
    constexpr const auto eps = double(1.e-14);
    constexpr const auto x1 = double(7) / 5;
    constexpr const auto x2 = double(1.2);
    auto a = tfel::math::BissectionAlgorithmBase<double>();
    const auto& d = a.getData();
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.xmin));
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.xmax));
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.fmin));
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.fmax));
    a.updateBounds(6, 2);
    TFEL_TESTS_ASSERT(std::abs(d.xmin - 6) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmin - 2) < eps);
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.xmax));
    TFEL_TESTS_ASSERT(tfel::math::ieee754::isnan(d.fmax));
    a.updateBounds(1, 2);
    TFEL_TESTS_ASSERT(std::abs(d.xmin - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmin - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.xmax - 6) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmax - 2) < eps);
    a.updateBounds(2, -3);
    TFEL_TESTS_ASSERT(std::abs(d.xmin - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmin - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.xmax - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmax + 3) < eps);
    auto x = double{3};
    a.iterate(x);
    TFEL_TESTS_ASSERT(std::abs(x - x1) < eps);
    a.updateBounds(1.4, -1);
    TFEL_TESTS_ASSERT(std::abs(d.xmin - 1) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmin - 2) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.xmax - 1.4) < eps);
    TFEL_TESTS_ASSERT(std::abs(d.fmax + 1) < eps);
    x = x2;
    a.iterate(x);
    TFEL_TESTS_ASSERT(std::abs(x - x2) < eps);
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(BissectionAlgorithmTest, "BissectionAlgorithmTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("BissectionAlgorithm.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
