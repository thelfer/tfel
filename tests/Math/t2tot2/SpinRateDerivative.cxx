/*!
 * \file   SpinRateDerivative.cxx
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

struct SpinRateDerivativeTest final : public tfel::tests::TestCase {
  SpinRateDerivativeTest()
      : tfel::tests::TestCase("TFEL/Math", "SpinRateDerivativeTest") {
  }  // end of SpinRateDerivativeTest
  virtual tfel::tests::TestResult execute() override {
    this->execute<1>();
    this->execute<2>();
    this->execute<3>();
    this->execute2<1>();
    this->execute2<2>();
    this->execute2<3>();
    return this->result;
  }  // end of execute
  virtual ~SpinRateDerivativeTest() {}

 private:
  template <unsigned short N>
  void execute() {
    using namespace tfel::math;
    const double eps = 1.e-8;
    const double prec = 1.e-14;
    const double values[9u] = {1.02, 0.85,  1.78,    0.18,   -0.16,
                               0.05, -0.08, -0.0243, 0.07654};
    const tensor<N, double> F{values};
    const auto iF = invert(F);
    const auto dW = computeSpinRateDerivative(F);
    t2tot2<N, double> ndW;
    for (unsigned short i = 0; i != F.size(); ++i) {
      tensor<N, double> dFp(0.);
      tensor<N, double> dFm(0.);
      dFp[i] += eps;
      const tensor<N, double> Lp = dFp * iF;
      const tensor<N, double> Wp = (Lp - transpose(Lp)) / 2;
      dFm[i] -= eps;
      const tensor<N, double> Lm = dFm * iF;
      const tensor<N, double> Wm = (Lm - transpose(Lm)) / 2;
      for (unsigned short j = 0; j != F.size(); ++j) {
        ndW(j, i) = (Wp(j) - Wm(j)) / (2 * eps);
      }
    }
    for (unsigned short i = 0; i != F.size(); ++i) {
      for (unsigned short j = 0; j != F.size(); ++j) {
        if (std::abs(ndW(i, j) - dW(i, j)) > prec) {
          std::cout << i << " " << j << " " << ndW(i, j) << " " << dW(i, j)
                    << std::endl;
        }
        TFEL_TESTS_ASSERT(std::abs(ndW(i, j) - dW(i, j)) < prec);
      }
    }
  }
  // a simple test of the tprd method
  template <unsigned short N>
  void execute2() {
    using namespace tfel::math;
    const double eps = 1.e-8;
    const double prec = 1.e-14;
    const double values[9u] = {1.02, 0.85,  1.78,    0.18,   -0.16,
                               0.05, -0.08, -0.0243, 0.07654};
    const tensor<N, double> F{values};
    const auto iF = invert(F);
    const t2tot2<N, double> d = t2tot2<N, double>::tprd(
        transpose(iF), t2tot2<N, double>::transpose_derivative());
    t2tot2<N, double> nd;
    for (unsigned short i = 0; i != F.size(); ++i) {
      tensor<N, double> dFp(0.);
      tensor<N, double> dFm(0.);
      dFp[i] += eps;
      const tensor<N, double> p = transpose(iF) * transpose(dFp);
      dFm[i] -= eps;
      const tensor<N, double> m = transpose(iF) * transpose(dFm);
      for (unsigned short j = 0; j != F.size(); ++j) {
        nd(j, i) = (p(j) - m(j)) / (2 * eps);
      }
    }
    for (unsigned short i = 0; i != F.size(); ++i) {
      for (unsigned short j = 0; j != F.size(); ++j) {
        if (std::abs(nd(i, j) - d(i, j)) > prec) {
          std::cout << i << " " << j << " " << nd(i, j) << " " << d(i, j)
                    << std::endl;
        }
        TFEL_TESTS_ASSERT(std::abs(nd(i, j) - d(i, j)) < prec);
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(SpinRateDerivativeTest, "SpinRateDerivativeTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("SpinRateDerivative.xml");
  return manager.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
