/*!
 * \file   computeRateOfDeformationDerivative.cxx
 * \brief
 * \author Thomas Helfer
 * \date   14 août 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"
#include "TFEL/Math/Tensor/TensorConceptIO.hxx"
#include "TFEL/Math/T2toST2/T2toST2ConceptIO.hxx"

struct RateOfDeformationDerivativeTest final : public tfel::tests::TestCase {
  RateOfDeformationDerivativeTest()
      : tfel::tests::TestCase("TFEL/Math", "RateOfDeformationDerivativeTest") {
  }  // end of RateOfDeformationDerivativeTest
  tfel::tests::TestResult execute() override {
    this->execute<1>();
    this->execute<2>();
    this->execute<3>();
    return this->result;
  }  // end of execute
  ~RateOfDeformationDerivativeTest() override = default;

 private:
  template <unsigned short N>
  void execute() {
    const double values[9u] = {1.02, 0.85,  1.78,    0.18,   -0.16,
                               0.05, -0.08, -0.0243, 0.07654};
    const double values2[9u] = {1, 1.2, 1, 0, 0, 0, 0, 0, 0};
    this->execute2<N>(values);
    this->execute2<N>(values2);
  }
  template <unsigned short N>
  void execute2(const double* values) {
    using namespace tfel::math;
    const double eps = 1.e-8;
    const double prec = 1.e-14;
    const tensor<N, double> F{values};
    const auto iF = invert(F);
    const auto dD = computeRateOfDeformationDerivative(F);
    t2tost2<N, double> ndD;
    for (unsigned short i = 0; i != TensorDimeToSize<N>::value; ++i) {
      tensor<N, double> dFp(0.);
      tensor<N, double> dFm(0.);
      dFp[i] += eps;
      const stensor<N, double> Dp = syme(dFp * iF);
      dFm[i] -= eps;
      const stensor<N, double> Dm = syme(dFm * iF);
      for (unsigned short j = 0; j != StensorDimeToSize<N>::value; ++j) {
        ndD(j, i) = (Dp(j) - Dm(j)) / (2 * eps);
      }
    }
    for (unsigned short i = 0; i != StensorDimeToSize<N>::value; ++i) {
      for (unsigned short j = 0; j != TensorDimeToSize<N>::value; ++j) {
        if (std::abs(ndD(i, j) - dD(i, j)) > prec) {
          std::cout << i << " " << j << " " << ndD(i, j) << " " << dD(i, j)
                    << std::endl;
        }
        TFEL_TESTS_ASSERT(std::abs(ndD(i, j) - dD(i, j)) < prec);
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(RateOfDeformationDerivativeTest,
                          "RateOfDeformationDerivativeTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("RateOfDeformationDerivative.xml");
  return manager.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
