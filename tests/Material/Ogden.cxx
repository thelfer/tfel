/*!
 * \file   Ogden.cxx
 * \brief
 * \author Thomas Helfer
 * \date   20 déc. 2016
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

#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx"

template <unsigned short N, typename F, typename real>
static tfel::math::st2tost2<N, real> getNumericalApproximation(
    const F& f, const tfel::math::stensor<N, real>& C, const real e) {
  tfel::math::st2tost2<N, real> r;
  const auto size = tfel::math::StensorDimeToSize<N>::value;
  for (unsigned short j = 0; j != size; ++j) {
    auto Cp = C;
    auto Cm = C;
    Cp(j) += e;
    Cm(j) -= e;
    const auto Fp = f(Cp);
    const auto Fm = f(Cm);
    const auto dF = (Fp - Fm) / (2 * e);
    for (unsigned short i = 0; i != size; ++i) {
      r(i, j) = dF(i);
    }
  }
  return r;
}

struct Ogden final : public tfel::tests::TestCase {
  Ogden() : tfel::tests::TestCase("TFEL/Material", "Ogden") {}  // end of Ogden
  tfel::tests::TestResult execute() override {
    this->check<1u>();
    this->check<2u>();
    this->check<3u>();
    this->check2<1u>();
    this->check2<2u>();
    this->check2<3u>();
    return this->result;
  }  // end of execute
 private:
  template <unsigned short N>
  void check() {
    using namespace tfel::math;
    using real = double;
    using Stensor = stensor<N, real>;
    const real alpha = 28.8;
    const auto a = alpha / 2;
    auto f = [&a](const real x) { return a * pow(x, a - 1); };
    auto df = [&a](const real x) { return a * (a - 1) * pow(x, a - 2); };
    auto sf = [&f](const Stensor& C) -> Stensor {
      return C.computeIsotropicFunction(f);
    };
    const real values[] = {1.2, 0.95, 0.67, 0.324, -0.675, -0.2};
    for (const Stensor& C : {Stensor::Id(), Stensor{values}}) {
      const auto ndf_dC = getNumericalApproximation(sf, C, 1.e-6);
      /* invariants and derivatives */
      const auto df_dC = C.computeIsotropicFunctionDerivative(f, df, 1.e-12);
      const auto eps =
          1.e-9 * (*(std::max_element(df_dC.begin(), df_dC.end())));
      for (unsigned short i = 0; i != tfel::math::StensorDimeToSize<N>::value;
           ++i) {
        for (unsigned short j = 0; j != tfel::math::StensorDimeToSize<N>::value;
             ++j) {
          if (std::abs(df_dC(i, j) - ndf_dC(i, j)) > eps) {
            std::cout << i << " " << j << " " << df_dC(i, j) << " "
                      << ndf_dC(i, j) << " " << df_dC(i, j) - ndf_dC(i, j)
                      << " " << eps << std::endl;
          }
          TFEL_TESTS_ASSERT(std::abs(df_dC(i, j) - ndf_dC(i, j)) < eps);
        }
      }
    }
  }
  template <unsigned short N>
  void check2() {
    using namespace tfel::math;
    using real = double;
    using Stensor = stensor<N, real>;
    using Stensor4 = st2tost2<N, real>;
    const real alpha = 28.8;
    const real mu = 0.4e6;
    const auto a = alpha / 2;
    const auto f = [&a](const real x) { return pow(x, a); };
    auto df = [&a](const real x) { return a * pow(x, a - 1); };
    auto d2f = [&a](const real x) { return a * (a - 1) * pow(x, a - 2); };
    auto sif = [&mu, &a, &f, &df](const Stensor& C) -> Stensor {
      const auto id = Stensor::Id();
      const auto J = sqrt(det(C));
      /* invariants and derivatives */
      const auto C2 = square(C);
      const auto I1 = trace(C);
      const auto I2 = (I1 * I1 - trace(C2)) / 2;
      const auto I3 = J * J;
      const auto dI3_dC = C2 - I1 * C + I2 * id;
      const auto iJb = 1 / cbrt(I3);
      const auto iJb2 = power<2>(iJb);
      const auto iJb4 = iJb2 * iJb2;
      const auto diJb_dI3 = -iJb4 / 3;
      const auto diJb_dC = diJb_dI3 * dI3_dC;
      // eigen values
      Stensor n0, n1, n2;
      tvector<3u, real> vp;
      tmatrix<3u, 3u, real> m;
      std::tie(vp, m) = C.computeEigenVectors();
      std::tie(n0, n1, n2) = Stensor::computeEigenTensors(m);
      const auto fv = f(vp(0)) + f(vp(1)) + f(vp(2));
      const auto df_dC = df(vp(0)) * n0 + df(vp(1)) * n1 + df(vp(2)) * n2;
      const auto c = pow(iJb, a - 2);
      return mu * c * iJb * ((fv * diJb_dC + (iJb / a) * df_dC));
    };
    const real values[] = {1.2, 0.95, 0.67, 0.324, -0.675, -0.2};
    for (const auto& C : {Stensor::Id(), Stensor{values}}) {
      const auto ndSi_dC = getNumericalApproximation(sif, C, 1.e-6);
      /* invariants and derivatives */
      const auto id = Stensor::Id();
      const auto C2 = square(C);
      const auto I1 = trace(C);
      const auto I2 = (I1 * I1 - trace(C2)) / 2;
      const auto I3 = det(C);
      const auto dI3_dC = C2 - I1 * C + I2 * id;
      const auto d2I3_dC2 = computeDeterminantSecondDerivative(C);
      /* iscochoric part */
      const auto iJb = 1 / cbrt(I3);
      const auto iJb2 = power<2>(iJb);
      const auto iJb4 = iJb2 * iJb2;
      const auto iJb7 = iJb4 * power<3>(iJb);
      const auto diJb_dI3 = -iJb4 / 3;
      const auto d2iJb_dI32 = 4 * iJb7 / 9;
      const auto diJb_dC = diJb_dI3 * dI3_dC;
      // eigen values
      Stensor n0, n1, n2;
      tvector<3u, real> vp;
      tmatrix<3u, 3u, real> m;
      C.computeEigenVectors(vp, m);
      std::tie(n0, n1, n2) = Stensor::computeEigenTensors(m);
      const auto fv = f(vp(0)) + f(vp(1)) + f(vp(2));
      const auto df_dC = df(vp(0)) * n0 + df(vp(1)) * n1 + df(vp(2)) * n2;
      const auto c = pow(iJb, a - 2);
      const auto d2f_dC2 =
          Stensor::computeIsotropicFunctionDerivative(df, d2f, vp, m, 1.e-12);
      const auto d2iJb_dC2 =
          d2iJb_dI32 * (dI3_dC ^ dI3_dC) + diJb_dI3 * d2I3_dC2;
      const Stensor4 dSi_dC =
          mu * c *
          ((a - 1) * fv * (diJb_dC ^ diJb_dC) +
           iJb * (fv * d2iJb_dC2 + ((diJb_dC ^ df_dC) + (df_dC ^ diJb_dC)) +
                  iJb / a * d2f_dC2));
      const auto eps = 1.e-4 * mu;
      for (unsigned short i = 0; i != tfel::math::StensorDimeToSize<N>::value;
           ++i) {
        for (unsigned short j = 0; j != tfel::math::StensorDimeToSize<N>::value;
             ++j) {
          if (std::abs(dSi_dC(i, j) - ndSi_dC(i, j)) > eps) {
            std::cout << i << " " << j << " " << dSi_dC(i, j) << " "
                      << ndSi_dC(i, j) << " " << dSi_dC(i, j) - ndSi_dC(i, j)
                      << " " << eps << std::endl;
          }
          TFEL_TESTS_ASSERT(std::abs(dSi_dC(i, j) - ndSi_dC(i, j)) < eps);
        }
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(Ogden, "Ogden");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("Ogden.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
