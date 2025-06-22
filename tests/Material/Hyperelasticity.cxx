/*!
 * \file   Hyperelasticity.cxx
 * \brief
 * \author Thomas Helfer
 * \date   19 déc. 2016
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
#include "TFEL/Math/st2tost2.hxx"

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

struct Hyperelasticity final : public tfel::tests::TestCase {
  Hyperelasticity()
      : tfel::tests::TestCase("TFEL/Material", "Hyperelasticity") {
  }  // end of Hyperelasticity
  tfel::tests::TestResult execute() override {
    this->check<1u>();
    this->check<2u>();
    this->check<3u>();
    return this->result;
  }  // end of execute
 private:
  template <unsigned short N>
  void check() {
    this->template check_volumetric_part<N>();
    this->template check_isochoric_part<N>();
  }
  // check that the perturbation approximation is ok
  template <unsigned short N>
  void check_volumetric_part() {
    using namespace tfel::math;
    using real = double;
    using stensor = stensor<N, real>;
    const real K = 2.939e9;
    auto svf = [&K](const stensor& C) -> stensor {
      const auto id = stensor::Id();
      const auto C2 = square(C);
      const auto I1 = trace(C);
      const auto I2 = (I1 * I1 - trace(C2)) / 2;
      const auto I3 = det(C);
      const auto dI3_dC = C2 - I1 * C + I2 * id;
      const auto J = std::sqrt(I3);
      /* volumetric part */
      // Pv = K*(J-1)*(J-1)/2
      const auto dPv_dJ = K * (J - 1);
      return dPv_dJ / J * dI3_dC;
    };
    const auto s1 = stensor::Id();
    const auto s2 = stensor{1.2, 0.95, 0.67, 0.324, -0.675, -0.2};
    for (const auto& C : {s1, s2}) {
      const auto id = stensor::Id();
      const auto C2 = square(C);
      const auto I1 = trace(C);
      const auto I2 = (I1 * I1 - trace(C2)) / 2;
      const auto I3 = det(C);
      const auto dI3_dC = C2 - I1 * C + I2 * id;
      const auto J = std::sqrt(I3);
      const auto dPv_dJ = K * (J - 1);
      const auto ndSv_dC = getNumericalApproximation(svf, C, 1.e-8);
      const auto d2Pv_dJ2 = K;
      const auto d2I3_dC2 = computeDeterminantSecondDerivative(C);
      const auto dSv_dC =
          (d2Pv_dJ2 - dPv_dJ / J) / (2 * I3) * (dI3_dC ^ dI3_dC) +
          dPv_dJ / J * d2I3_dC2;
      const auto eps = 1.e-7 * K;
      for (unsigned short i = 0; i != tfel::math::StensorDimeToSize<N>::value;
           ++i) {
        for (unsigned short j = 0; j != tfel::math::StensorDimeToSize<N>::value;
             ++j) {
          if (std::abs(dSv_dC(i, j) - ndSv_dC(i, j)) > eps) {
            std::cout << dSv_dC(i, j) << " " << ndSv_dC(i, j) << " "
                      << dSv_dC(i, j) - ndSv_dC(i, j) << " " << eps
                      << std::endl;
          }
          TFEL_TESTS_ASSERT(std::abs(dSv_dC(i, j) - ndSv_dC(i, j)) < eps);
        }
      }
    }
  }
  template <unsigned short N>
  void check_isochoric_part() {
    using namespace tfel::math;
    using real = double;
    using stensor = stensor<N, real>;
    using Stensor4 = st2tost2<N, real>;
    const real C01 = 2.668e6;
    const real C02 = 0.446e6;
    const real C10 = 0.271e6;
    auto sif = [&C01, &C02, &C10](const stensor& C) -> stensor {
      const auto id = stensor::Id();
      const auto C2 = square(C);
      const auto I1 = trace(C);
      const auto I2 = (I1 * I1 - trace(C2)) / 2;
      const auto I3 = det(C);
      const auto dI2_dC = I1 * id - C;
      const auto dI3_dC = C2 - I1 * C + I2 * id;
      const auto iJb = 1 / cbrt(I3);
      const auto iJb2 = power<2>(iJb);
      const auto iJb4 = iJb2 * iJb2;
      const auto diJb_dI3 = -iJb4 / 3;
      const auto diJb_dC = diJb_dI3 * dI3_dC;
      const auto I1b = I1 * iJb;
      const auto dI1b_dC = iJb * id + I1 * diJb_dC;
      const auto dI2b_dC = iJb2 * dI2_dC + 2 * I2 * iJb * diJb_dC;
      const auto dPi_dI1b = C01 + 2 * C02 * (I1b - 3);
      const auto dPi_dI2b = C10;
      return 2 * (dPi_dI1b * dI1b_dC + dPi_dI2b * dI2b_dC);
    };
    const auto s1 = stensor::Id();
    const auto s2 = stensor{1.2, 0.95, 0.67, 0.324, -0.675, -0.2};
    for (const auto& C : {s1, s2}) {
      const auto ndSi_dC = getNumericalApproximation(sif, C, 1.e-6);
      const auto id = stensor::Id();
      const auto C2 = square(C);
      const auto I1 = trace(C);
      const auto I2 = (I1 * I1 - trace(C2)) / 2;
      const auto I3 = det(C);
      const auto dI2_dC = I1 * id - C;
      const auto dI3_dC = C2 - I1 * C + I2 * id;
      const auto iJb = 1 / cbrt(I3);
      const auto iJb2 = power<2>(iJb);
      const auto iJb4 = iJb2 * iJb2;
      const auto diJb_dI3 = -iJb4 / 3;
      const auto diJb_dC = diJb_dI3 * dI3_dC;
      const auto I1b = I1 * iJb;
      const auto dI1b_dC = iJb * id + I1 * diJb_dC;
      const auto dI2b_dC = iJb2 * dI2_dC + 2 * I2 * iJb * diJb_dC;
      const auto d2I2_dC2 = (id ^ id) - Stensor4::Id();
      const auto d2I3_dC2 = computeDeterminantSecondDerivative(C);
      const auto iJb7 = iJb4 * power<3>(iJb);
      const auto d2iJb_dI32 = 4 * iJb7 / 9;
      const auto d2iJb_dC2 =
          d2iJb_dI32 * (dI3_dC ^ dI3_dC) + diJb_dI3 * d2I3_dC2;
      const auto d2I1b_dC2 = (id ^ diJb_dC) + (diJb_dC ^ id) + I1 * d2iJb_dC2;
      const auto d2I2b_dC2 = 2 * iJb * (dI2_dC ^ diJb_dC) + iJb2 * d2I2_dC2 +
                             2 * iJb * (diJb_dC ^ dI2_dC) +
                             2 * I2 * (diJb_dC ^ diJb_dC) +
                             2 * I2 * iJb * d2iJb_dC2;
      const auto dPi_dI1b = C01 + 2 * C02 * (I1b - 3);
      const auto dPi_dI2b = C10;
      const auto d2Pi_dI1b2 = 2 * C02;
      const auto d2Pi_dI2b2 = 0;
      const auto dSi_dC =
          2 * (d2Pi_dI1b2 * (dI1b_dC ^ dI1b_dC) + dPi_dI1b * d2I1b_dC2 +
               d2Pi_dI2b2 * (dI2b_dC ^ dI2b_dC) + dPi_dI2b * d2I2b_dC2);
      const auto eps = 1.e-7 * C01;
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

TFEL_TESTS_GENERATE_PROXY(Hyperelasticity, "Hyperelasticity");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("Hyperelasticity.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
