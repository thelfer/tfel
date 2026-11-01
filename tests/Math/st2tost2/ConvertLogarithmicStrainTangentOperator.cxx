/*!
 * \file   ConvertLogarithmicStrainTangentOperatorTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date 29/08/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Material/Lame.hxx"
#include "TFEL/Math/ST2toST2/ConvertLogarithmicStrainTangentOperator.hxx"

struct ConvertLogarithmicStrainTangentOperatorTest
    : public tfel::tests::TestCase {
  ConvertLogarithmicStrainTangentOperatorTest()
      : tfel::tests::TestCase("TFEL/Math",
                              "ConvertLogarithmicStrainTangentOperator") {
  }  // end of ConvertLogarithmicStrainTangentOperatorTest

  tfel::tests::TestResult execute() override {
    this->test1();
    return this->result;
  }  // end of execute
 private:
  //! \brief comparison with a numerical derivative in 1D
  void test1() {
    using namespace tfel::math;
    using Stensor = stensor<1u, double>;
    using Stensor4 = st2tost2<1u, double>;
    constexpr auto h = 1e-6;
    // linear behaviour in the logarithmic strain space: T = K : Elog
    const auto K = Stensor4{3, 1, 0.5, 1, 2, 1, 0.5, 1, 4};
    const auto elog = [](const Stensor& E) {
      return Stensor{std::log1p(2 * E[0]) / 2, std::log1p(2 * E[1]) / 2,
                     std::log1p(2 * E[2]) / 2};
    };
    // second Piola-Kirchhoff stress as a function of the Green-Lagrange strain
    const auto S = [&K, &elog](const Stensor& E) {
      const auto T = Stensor{K * elog(E)};
      return Stensor{T[0] / (1 + 2 * E[0]), T[1] / (1 + 2 * E[1]),
                     T[2] / (1 + 2 * E[2])};
    };
    const auto E = Stensor{0.1, -0.05, 0.2};
    auto P = Stensor4{0.};
    for (unsigned short i = 0; i != 3; ++i) {
      P(i, i) = 1 / (1 + 2 * E[i]);
    }
    auto Cse = Stensor4{};
    ConvertLogarithmicStrainTangentOperator<1u, double>::exe(
        Cse, K, P, Stensor{K * elog(E)}, tmatrix<3u, 3u, double>{},
        tvector<3u, double>{});
    for (unsigned short j = 0; j != 3; ++j) {
      auto Ep = E;
      auto Em = E;
      Ep[j] += h;
      Em[j] -= h;
      const auto dS = Stensor{(S(Ep) - S(Em)) / (2 * h)};
      for (unsigned short i = 0; i != 3; ++i) {
        TFEL_TESTS_ASSERT(std::abs(Cse(i, j) - dS[i]) < 1e-8);
      }
    }
  }  // end of test1
};  // end of ConvertLogarithmicStrainTangentOperatorTest

TFEL_TESTS_GENERATE_PROXY(ConvertLogarithmicStrainTangentOperatorTest,
                          "ConvertLogarithmicStrainTangentOperatorTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ConvertLogarithmicStrainTangentOperator.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
