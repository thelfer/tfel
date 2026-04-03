/*!
 * \file   tests/Math/ScalarNewtonRaphsonTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   31/03/2020
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

#include <tuple>
#include <cmath>
#include <limits>
#include <cassert>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "TFEL/Math/ScalarNewtonRaphson.hxx"

struct ScalarNewtonRaphsonTest final : public tfel::tests::TestCase {
  ScalarNewtonRaphsonTest()
      : tfel::tests::TestCase("TFEL/Math", "ScalarNewtonRaphsonTest") {
  }  // end of ScalarNewtonRaphsonTest
  tfel::tests::TestResult execute() override {
    auto fdf = [](const double x) {
      return std::make_tuple(x * x - 13, 2 * x);
    };
    auto c = [](const double f, const double, const double, const int) {
      return std::abs(f) < 1.e-14;
    };
    const auto r = tfel::math::scalarNewtonRaphson(fdf, c, 0.1, 100);
    TFEL_TESTS_ASSERT(std::get<0>(r));
    TFEL_TESTS_ASSERT(std::abs(std::get<1>(r) - std::sqrt(double(13))) <
                      1.e-13);
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(ScalarNewtonRaphsonTest, "ScalarNewtonRaphsonTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ScalarNewtonRaphson.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
