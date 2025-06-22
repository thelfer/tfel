/*!
 * \file   VelocityGradientDerivative.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   14 déc. 2015
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
#include "TFEL/Math/t2tot2.hxx"
#include "TFEL/Math/Tensor/TensorConceptIO.hxx"
#include "TFEL/Math/T2toT2/T2toT2ConceptIO.hxx"

struct VelocityGradientDerivativeTest final : public tfel::tests::TestCase {
  VelocityGradientDerivativeTest()
      : tfel::tests::TestCase("TFEL/Math", "VelocityGradientDerivativeTest") {
  }  // end of VelocityGradientDerivativeTest
  virtual tfel::tests::TestResult execute() override {
    this->execute<1>();
    this->execute<2>();
    this->execute<3>();
    this->execute2<1>();
    this->execute2<2>();
    this->execute2<3>();
    return this->result;
  }  // end of execute
  virtual ~VelocityGradientDerivativeTest() {}

 private:
  template <unsigned short N>
  void execute() {
    const double eps = 1.e-8;
    const double prec = 1.e-14;
    const double values[9u] = {1.02, 0.85,  1.78,    0.18,   -0.16,
                               0.05, -0.08, -0.0243, 0.07654};
    const tfel::math::tensor<N, double> F{values};
    const auto iF = invert(F);
    const auto dL = computeVelocityGradientDerivative(F);
    tfel::math::t2tot2<N, double> ndL;
    for (unsigned short i = 0; i != F.size(); ++i) {
      tfel::math::tensor<N, double> dF(0.);
      dF[i] += eps;
      const tfel::math::tensor<N, double> Lp = dF * iF;
      dF[i] -= 2 * eps;
      const tfel::math::tensor<N, double> Lm = dF * iF;
      for (unsigned short j = 0; j != F.size(); ++j) {
        ndL(j, i) = (Lp(j) - Lm(j)) / (2 * eps);
      }
    }
    for (unsigned short i = 0; i != F.size(); ++i) {
      for (unsigned short j = 0; j != F.size(); ++j) {
        if (std::abs(ndL(i, j) - dL(i, j)) > prec) {
          std::cout << i << " " << j << " " << ndL(i, j) << " " << dL(i, j)
                    << std::endl;
        }
        TFEL_TESTS_ASSERT(std::abs(ndL(i, j) - dL(i, j)) < prec);
      }
    }
  }
  // another simple test of t2tot2<N,double>::tprd
  template <unsigned short N>
  void execute2() {
    const double eps = 1.e-8;
    const double prec = 1.e-14;
    const double values[9u] = {1.02, 0.85,  1.78,    0.18,   -0.16,
                               0.05, -0.08, -0.0243, 0.07654};
    const tfel::math::tensor<N, double> F{values};
    const auto iF = invert(F);
    const auto dL = tfel::math::t2tot2<N, double>::tprd(iF);
    tfel::math::t2tot2<N, double> ndL;
    for (unsigned short i = 0; i != F.size(); ++i) {
      tfel::math::tensor<N, double> dF(0.);
      dF[i] += eps;
      const tfel::math::tensor<N, double> Lp = iF * dF;
      dF[i] -= 2 * eps;
      const tfel::math::tensor<N, double> Lm = iF * dF;
      for (unsigned short j = 0; j != F.size(); ++j) {
        ndL(j, i) = (Lp(j) - Lm(j)) / (2 * eps);
      }
    }
    for (unsigned short i = 0; i != F.size(); ++i) {
      for (unsigned short j = 0; j != F.size(); ++j) {
        if (std::abs(ndL(i, j) - dL(i, j)) > prec) {
          std::cout << i << " " << j << " " << ndL(i, j) << " " << dL(i, j)
                    << std::endl;
        }
        TFEL_TESTS_ASSERT(std::abs(ndL(i, j) - dL(i, j)) < prec);
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(VelocityGradientDerivativeTest,
                          "VelocityGradientDerivativeTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("VelocityGradientDerivative.xml");
  return manager.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
