/*!
 * \file   TransposeDerivative.cxx
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

struct TransposeDerivativeTest final : public tfel::tests::TestCase {
  TransposeDerivativeTest()
      : tfel::tests::TestCase("TFEL/Math", "TransposeDerivativeTest") {
  }  // end of TransposeDerivativeTest
  virtual tfel::tests::TestResult execute() override {
    this->execute<1>();
    this->execute<2>();
    this->execute<3>();
    return this->result;
  }  // end of execute
  virtual ~TransposeDerivativeTest() {}

 private:
  template <unsigned short N>
  void execute() {
    const double eps = 1.e-8;
    const double prec = 1.e-14;
    const double values[9u] = {1.02, 0.85,  1.78,    0.18,   -0.16,
                               0.05, -0.08, -0.0243, 0.07654};
    const tfel::math::tensor<N, double> F{values};
    const auto dt = tfel::math::t2tot2<N, double>::transpose_derivative();
    tfel::math::t2tot2<N, double> ndt;
    for (unsigned short i = 0; i != F.size(); ++i) {
      tfel::math::tensor<N, double> dF(0.);
      dF[i] += eps;
      const tfel::math::tensor<N, double> dtp = transpose(dF);
      dF[i] -= 2 * eps;
      const tfel::math::tensor<N, double> dtm = transpose(dF);
      for (unsigned short j = 0; j != F.size(); ++j) {
        ndt(j, i) = (dtp(j) - dtm(j)) / (2 * eps);
      }
    }
    for (unsigned short i = 0; i != F.size(); ++i) {
      for (unsigned short j = 0; j != F.size(); ++j) {
        if (std::abs(ndt(i, j) - dt(i, j)) > prec) {
          std::cout << i << " " << j << " " << ndt(i, j) << " " << dt(i, j)
                    << std::endl;
        }
        TFEL_TESTS_ASSERT(std::abs(ndt(i, j) - dt(i, j)) < prec);
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(TransposeDerivativeTest, "TransposeDerivativeTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("TransposeDerivative.xml");
  return manager.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
