/*!
 * \file   EuroplexusFiniteStrain.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   15 mars 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/Stensor/StensorView.hxx"
#include "TFEL/Math/Stensor/ConstStensorView.hxx"
#include "TFEL/Math/T2toST2/T2toST2View.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include "TFEL/Math/ST2toST2/ConstST2toST2View.hxx"
#include "MFront/Europlexus/EuroplexusFiniteStrain.hxx"

namespace epx {

  void computeGreenLagrangeStrain(EuroplexusReal* const eto,
                                  EuroplexusReal* const deto,
                                  const EuroplexusReal* const F0,
                                  const EuroplexusReal* const F1,
                                  const EuroplexusInt h) {
    using namespace tfel::math;
    using real = EuroplexusReal;
    if ((h == 3) || (h == 2) || (h == 1)) {
      StensorView<2u, real> e(eto);
      StensorView<2u, real> de(deto);
      e = computeGreenLagrangeTensor(tensor<2u, real>(F0));
      de = computeGreenLagrangeTensor(tensor<2u, real>(F1)) - e;
    } else if (h == 0) {
      StensorView<3u, real> e(eto);
      StensorView<3u, real> de(deto);
      e = computeGreenLagrangeTensor(tensor<3u, real>(F0));
      de = computeGreenLagrangeTensor(tensor<3u, real>(F1)) - e;
    } else {
      std::cerr << "epx::computeGreenLagrangeStrain: "
                << "unsupported hypothesis" << std::endl;
      ::exit(-1);
    }
  }

  void computeSecondPiolaKirchhoffStressFromCauchyStress(
      EuroplexusReal* const sig,
      const EuroplexusReal* const STRESS,
      const EuroplexusReal* const F0,
      const EuroplexusInt h) {
    using namespace tfel::math;
    using real = EuroplexusReal;
    if ((h == 3) || (h == 2) || (h == 1)) {
      StensorView<2u, real> s(sig);
      const ConstStensorView<2u, real> c(STRESS);
      s = convertCauchyStressToSecondPiolaKirchhoffStress(c,
                                                          tensor<2u, real>(F0));
    } else if (h == 0) {
      StensorView<3u, real> s(sig);
      const ConstStensorView<3u, real> c(STRESS);
      s = convertCauchyStressToSecondPiolaKirchhoffStress(c,
                                                          tensor<3u, real>(F0));
    } else {
      std::cerr << "epx::computeSecondPiolaKirchhoffStressFromCauchyStress: "
                << "unsupported hypothesis" << std::endl;
      ::exit(-1);
    }
  }

  void computeCauchyStressFromSecondPiolaKirchhoffStress(
      EuroplexusReal* const STRESS,
      const EuroplexusReal* const sig,
      const EuroplexusReal* const F1,
      const EuroplexusInt h) {
    using namespace tfel::math;
    using real = EuroplexusReal;
    if ((h == 3) || (h == 2) || (h == 1)) {
      const ConstStensorView<2u, real> s(sig);
      StensorView<2u, real> c(STRESS);
      c = convertSecondPiolaKirchhoffStressToCauchyStress(s,
                                                          tensor<2u, real>(F1));
    } else if (h == 0) {
      const ConstStensorView<3u, real> s(sig);
      StensorView<3u, real> c(STRESS);
      c = convertSecondPiolaKirchhoffStressToCauchyStress(s,
                                                          tensor<3u, real>(F1));
    } else {
      std::cerr << "epx::computeCauchyStressFromSecondPiolaKirchhoffStress: "
                << "unsupported hypothesis" << std::endl;
      ::exit(-1);
    }
  }

  void computeElasticModuli(EuroplexusReal* const D, const EuroplexusInt h) {
    using namespace tfel::math;
    using real = EuroplexusReal;
    if ((h == 3) || (h == 2) || (h == 1)) {
      const ConstST2toST2View<2u, real> De{D};
      const auto F = tensor<2u, real>::Id();
      const t2tost2<2u, real> dS = 0.5 * De * t2tost2<2u, real>::dCdF(F);
      T2toST2View<2u, real> ds{D};
      ds = dS;
    } else if (h == 0) {
      ST2toST2View<3u, real> De{D};
      const auto F = tensor<3u, real>::Id();
      const t2tost2<3u, real> dS = 0.5 * De * t2tost2<3u, real>::dCdF(F);
      T2toST2View<3u, real> ds{D};
      ds = dS;
    } else {
      std::cerr << "epx::"
                   "computeCauchyStressDerivativeFromSecondPiolaKirchhoffStress"
                   "Derivative: "
                << "unsupported hypothesis" << std::endl;
      ::exit(-1);
    }
  }

  template <unsigned short N>
  static void doComputeLogarithmicStrainAndDerivative(
      tfel::math::StensorView<N, EuroplexusReal>& e,
      tfel::math::ST2toST2View<N, EuroplexusReal>& p,
      const EuroplexusReal* const F) {
    TFEL_CONSTEXPR const EuroplexusReal eps =
        100 * std::numeric_limits<EuroplexusReal>::epsilon();
    using stensor = tfel::math::stensor<N, EuroplexusReal>;
    using tensor = tfel::math::tensor<N, EuroplexusReal>;
    const auto f = [](const EuroplexusReal x) { return std::log1p(x - 1) / 2; };
    const auto df = [](const EuroplexusReal x) { return 1 / (2 * x); };
    const auto C = computeRightCauchyGreenTensor(tensor(F));
    std::tie(e, p) = C.computeIsotropicFunctionAndDerivative(f, df, eps);
  }  // end of doComputeLogarithmicStrainAndDerivative

  void computeLogarithmicStrainAndDerivative(EuroplexusReal* const P0,
                                             EuroplexusReal* const P1,
                                             EuroplexusReal* const eto,
                                             EuroplexusReal* const deto,
                                             const EuroplexusReal* const F0,
                                             const EuroplexusReal* const F1,
                                             const EuroplexusInt h) {
    if ((h == 3) || (h == 2) || (h == 1)) {
      tfel::math::StensorView<2u, EuroplexusReal> e(eto);
      tfel::math::StensorView<2u, EuroplexusReal> de(deto);
      tfel::math::ST2toST2View<2u, EuroplexusReal> p0(P0);
      tfel::math::ST2toST2View<2u, EuroplexusReal> p1(P1);
      doComputeLogarithmicStrainAndDerivative(e, p0, F0);
      doComputeLogarithmicStrainAndDerivative(de, p1, F1);
      de -= e;
    } else if (h == 0) {
      tfel::math::StensorView<3u, EuroplexusReal> e(eto);
      tfel::math::StensorView<3u, EuroplexusReal> de(deto);
      tfel::math::ST2toST2View<3u, EuroplexusReal> p0(P0);
      tfel::math::ST2toST2View<3u, EuroplexusReal> p1(P1);
      doComputeLogarithmicStrainAndDerivative(e, p0, F0);
      doComputeLogarithmicStrainAndDerivative(de, p1, F1);
      de -= e;
    } else {
      std::cerr << "epx::computeLogarithmicStrainAndDerivative: "
                << "unsupported hypothesis" << std::endl;
      ::exit(-1);
    }
  }

  void computeDualStressOfLogarithmicStrainFromCauchyStress(
      EuroplexusReal* const Tv,
      const EuroplexusReal* const sv,
      const EuroplexusReal* const P0,
      const EuroplexusReal* const F0,
      const EuroplexusInt h) {
    using namespace tfel::math;
    using real = EuroplexusReal;
    // df is define as 2 times the derivative because
    // S = T|(dElog/dC*dC_dEgl)=T|(2*dElog/dC)
    if ((h == 3) || (h == 2) || (h == 1)) {
      const ConstStensorView<2u, real> s(sv);
      const auto iP0 = invert(ConstST2toST2View<2u, real>(P0));
      const auto S = convertCauchyStressToSecondPiolaKirchhoffStress(
          s, tensor<2u, real>(F0));
      StensorView<2u, real> T(Tv);
      T = (S | iP0) / 2;
    } else if (h == 0) {
      const ConstStensorView<3u, real> s(sv);
      const auto iP0 = invert(ConstST2toST2View<3u, real>(P0));
      const auto S = convertCauchyStressToSecondPiolaKirchhoffStress(
          s, tensor<3u, real>(F0));
      StensorView<3u, real> T(Tv);
      T = (S | iP0) / 2;
    } else {
      std::cerr << "epx::computeDualStressOfLogarithmicStrainFromCauchyStress: "
                << "unsupported hypothesis" << std::endl;
      ::exit(-1);
    }
  }

  void computeCauchyStressFromDualStressOfLogarithmicStrain(
      EuroplexusReal* const sv,
      const EuroplexusReal* const Tv,
      const EuroplexusReal* const P1v,
      const EuroplexusReal* const F1,
      const EuroplexusInt h) {
    using namespace tfel::math;
    using real = EuroplexusReal;
    // S = T|(dElog/dC*dC_dEgl)=T|(2*dElog/dC)
    if ((h == 3) || (h == 2) || (h == 1)) {
      const ConstStensorView<2u, real> T{Tv};
      const ConstST2toST2View<2u, real> P1{P1v};
      const stensor<2u, real> S = 2 * (T | P1);
      StensorView<2u, real> s{sv};
      s = convertSecondPiolaKirchhoffStressToCauchyStress(S,
                                                          tensor<2u, real>(F1));
    } else if (h == 0) {
      const ConstStensorView<3u, real> T{Tv};
      const ConstST2toST2View<3u, real> P1{P1v};
      const stensor<3u, real> S = 2 * (T | P1);
      StensorView<3u, real> s{sv};
      s = convertSecondPiolaKirchhoffStressToCauchyStress(S,
                                                          tensor<3u, real>(F1));
    } else {
      std::cerr << "epx::computeDualStressOfLogarithmicStrainFromCauchyStress: "
                << "unsupported hypothesis" << std::endl;
      ::exit(-1);
    }
  }

}  // end of namespace epx
