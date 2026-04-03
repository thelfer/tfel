/*!
 * \file   ComputePK1Derivative.cxx
 * \brief
 * \author Thomas Helfer
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
#include "TFEL/Math/T2toT2/ConvertToPK1Derivative.hxx"

struct ComputePK1DerivativeTest final : public tfel::tests::TestCase {
  ComputePK1DerivativeTest()
      : tfel::tests::TestCase("TFEL/Math", "ComputePK1DerivativeTest") {
  }  // end of ComputePK1DerivativeTest
  tfel::tests::TestResult execute() override {
    this->execute<1>();
    this->execute<2>();
    this->execute<3>();
    return this->result;
  }  // end of execute
  ~ComputePK1DerivativeTest() override = default;

 private:
  template <unsigned short N>
  void execute() {
    using namespace tfel::math;
    const auto lambda = 150e9;
    const auto mu = 70e9;
    const double eps = 1.e-5;
    const double prec = 3e-10 * lambda;
    const double values[9u] = {1.02, 0.85,  1.78,    0.18,   -0.16,
                               0.05, -0.08, -0.0243, 0.07654};
    const tensor<N, double> F{values};
    const auto D = eval(lambda * st2tost2<N, double>::IxI() +
                        2 * mu * st2tost2<N, double>::Id());
    const auto e = computeGreenLagrangeTensor(F);
    const auto S = lambda * trace(e) * stensor<N, double>::Id() + 2 * mu * e;
    const auto s = convertSecondPiolaKirchhoffStressToCauchyStress(S, F);
    const auto dPK1 =
        convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative(
            D, F, s);
    t2tot2<N, double> ndPK1;
    for (unsigned short i = 0; i != F.size(); ++i) {
      auto Fp = F;
      auto Fm = F;
      Fp[i] += eps;
      Fm[i] -= eps;
      const auto ep = computeGreenLagrangeTensor(Fp);
      const auto Sp =
          lambda * trace(ep) * stensor<N, double>::Id() + 2 * mu * ep;
      const auto sp = convertSecondPiolaKirchhoffStressToCauchyStress(Sp, Fp);
      const auto PK1p = convertCauchyStressToFirstPiolaKirchhoffStress(sp, Fp);
      const auto em = computeGreenLagrangeTensor(Fm);
      const auto Sm =
          lambda * trace(em) * stensor<N, double>::Id() + 2 * mu * em;
      const auto sm = convertSecondPiolaKirchhoffStressToCauchyStress(Sm, Fm);
      const auto PK1m = convertCauchyStressToFirstPiolaKirchhoffStress(sm, Fm);
      for (unsigned short j = 0; j != F.size(); ++j) {
        ndPK1(j, i) = (PK1p(j) - PK1m(j)) / (2 * eps);
      }
    }
    for (unsigned short i = 0; i != F.size(); ++i) {
      for (unsigned short j = 0; j != F.size(); ++j) {
        const auto err = std::abs(ndPK1(i, j) - dPK1(i, j));
        if (err > prec) {
          std::cout << i << " " << j << " " << ndPK1(i, j) << " " << dPK1(i, j)
                    << " " << err << " " << prec << std::endl;
        }
        TFEL_TESTS_ASSERT(std::abs(ndPK1(i, j) - dPK1(i, j)) < prec);
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(ComputePK1DerivativeTest, "ComputePK1DerivativeTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ComputePK1Derivative.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
