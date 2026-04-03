/*!
 * \file  tests/Math/t2tost2/SaintVenantKirchhoffTangentOperator.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 08 juin 2014
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
#include <string>
#include <cstdlib>
#include <cassert>
#include <iostream>

#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/Lame.hxx"

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

template <unsigned short N>
struct SaintVenantKirchhoffTangentOperator final
    : public tfel::tests::TestCase {
  SaintVenantKirchhoffTangentOperator()
      : tfel::tests::TestCase(
            "TFEL/Math",
            "SaintVenantKirchhoffTangentOperator" +
                std::to_string(static_cast<unsigned int>(N))) {
  }  // end of SaintVenantKirchhoffTangentOperator
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::math;
    using namespace tfel::material;
    const double eps = 1.e-7;
    const double E = 150e9;
    const double prec1 = E * 1.e-7;
    const double prec2 = 1e-8;
    const double nu = 0.33;
    const double lambda = computeLambda(E, nu);
    const double mu = computeMu(E, nu);
    const st2tost2<N, double> D = lambda * st2tost2<N, double>::IxI() +
                                  2 * mu * st2tost2<N, double>::Id();
    tensor<N> F;
    for (typename tensor<N>::size_type i = 0; i != F.size(); ++i) {
      if (i < 3) {
        F[i] = 1 + 0.1 * i * i;
      } else {
        F[i] = 0.03 * (i - 2);
      }
    }
    const stensor<N> C = computeRightCauchyGreenTensor(F);
    const stensor<N> e_gl = 0.5 * (C - stensor<N>::Id());
    const stensor<N> S = D * e_gl;
    stensor<N> sig = convertSecondPiolaKirchhoffStressToCauchyStress(S, F);
    const t2tost2<N> dC = t2tost2<N>::dCdF(F);
    const t2tost2<N> dS = 0.5 * D * dC;
    t2tost2<N> dtau;
    computePushForwardDerivative(dtau, dS, S, F);
    tensor<N> dJ;
    computeDeterminantDerivative(dJ, F);
    t2tost2<N> dsig;
    computeCauchyStressDerivativeFromKirchhoffStressDerivative(dsig, dtau, sig,
                                                               F);
    t2tost2<N> ndtau;
    t2tost2<N> ndsig;
    t2tost2<N> ndS;
    tensor<N> ndJ;
    for (typename tensor<N>::size_type i = 0; i != F.size(); ++i) {
      tensor<N> Fe(F);
      Fe[i] += eps;
      const stensor<N> Cef = computeRightCauchyGreenTensor(Fe);
      const stensor<N> e_glef = 0.5 * (Cef - stensor<N>::Id());
      const stensor<N> Sef = D * e_glef;
      const double Jef = det(Fe);
      const stensor<N> tauef = pushForward(Sef, Fe);
      const stensor<N> sigef =
          convertSecondPiolaKirchhoffStressToCauchyStress(Sef, Fe);
      Fe[i] -= 2 * eps;
      const stensor<N> Ceb = computeRightCauchyGreenTensor(Fe);
      const stensor<N> e_gleb = 0.5 * (Ceb - stensor<N>::Id());
      const stensor<N> Seb = D * e_gleb;
      const double Jeb = det(Fe);
      const stensor<N> taueb = pushForward(Seb, Fe);
      const stensor<N> sigeb =
          convertSecondPiolaKirchhoffStressToCauchyStress(Seb, Fe);
      for (typename tensor<N>::size_type j = 0; j != sig.size(); ++j) {
        ndS(j, i) = (Sef(j) - Seb(j)) / (2 * eps);
        ndtau(j, i) = (tauef(j) - taueb(j)) / (2 * eps);
        ndsig(j, i) = (sigef(j) - sigeb(j)) / (2 * eps);
      }
      ndJ(i) = (Jef - Jeb) / (2 * eps);
    }
    for (typename tensor<N>::size_type i = 0; i != sig.size(); ++i) {
      for (typename tensor<N>::size_type j = 0; j != F.size(); ++j) {
        TFEL_TESTS_ASSERT(abs(ndS(i, j) - dS(i, j)) < prec1);
        if (abs(ndS(i, j) - dS(i, j)) > prec1) {
          cout << "dS error (" << N << "): " << i << " " << j << " "
               << ndS(i, j) << " " << dS(i, j) << " "
               << abs(ndS(i, j) - dS(i, j)) /
                      (max(abs(abs(ndS(i, j))), abs(dS(i, j))))
               << endl;
        }
        TFEL_TESTS_ASSERT(abs(ndtau(i, j) - dtau(i, j)) < prec1);
        if (abs(ndtau(i, j) - dtau(i, j)) > prec1) {
          cout << "dtau error (" << N << "): " << i << " " << j << " "
               << ndtau(i, j) << " " << dtau(i, j) << " "
               << abs(ndtau(i, j) - dtau(i, j)) /
                      (max(abs(abs(ndtau(i, j))), abs(dtau(i, j))))
               << endl;
        }
        TFEL_TESTS_ASSERT(abs(ndsig(i, j) - dsig(i, j)) < prec1);
        if (abs(ndsig(i, j) - dsig(i, j)) > prec1) {
          cout << "dsig error (" << N << "): " << i << " " << j << " "
               << ndsig(i, j) << " " << dsig(i, j) << " "
               << abs(ndsig(i, j) - dsig(i, j)) /
                      (max(abs(abs(ndsig(i, j))), abs(dsig(i, j))))
               << endl;
        }
      }
    }
    for (typename tensor<N>::size_type i = 0; i != F.size(); ++i) {
      TFEL_TESTS_ASSERT(abs(ndJ(i) - dJ(i)) < prec2);
      if (abs(ndJ(i) - dJ(i)) > prec2) {
        cout << "dJ error (" << N << "): " << i << " " << ndJ(i) << " " << dJ(i)
             << " " << abs(ndJ(i) - dJ(i)) << endl;
      }
    }
    return this->result;
  }  // end of execute
};

using SaintVenantKirchhoffTangentOperator_1D =
    SaintVenantKirchhoffTangentOperator<1U>;
using SaintVenantKirchhoffTangentOperator_2D =
    SaintVenantKirchhoffTangentOperator<2U>;
using SaintVenantKirchhoffTangentOperator_3D =
    SaintVenantKirchhoffTangentOperator<3U>;
TFEL_TESTS_GENERATE_PROXY(SaintVenantKirchhoffTangentOperator_1D,
                          "SaintVenantKirchhoffTangentOperator-1D");
TFEL_TESTS_GENERATE_PROXY(SaintVenantKirchhoffTangentOperator_2D,
                          "SaintVenantKirchhoffTangentOperator-2D");
TFEL_TESTS_GENERATE_PROXY(SaintVenantKirchhoffTangentOperator_3D,
                          "SaintVenantKirchhoffTangentOperator-3D");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("SaintVenantKirchhoffTangentOperator.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
