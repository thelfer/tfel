/*!
 * \file  mfront/src/CastemFiniteStrain.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <sstream>
#include <stdexcept>

#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/Stensor/StensorView.hxx"
#include "TFEL/Math/Stensor/ConstStensorView.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include "TFEL/Math/ST2toST2/ConstST2toST2View.hxx"
#include "TFEL/Math/ST2toST2/UmatNormaliseTangentOperator.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "MFront/Castem/CastemTangentOperator.hxx"
#include "MFront/Castem/CastemFiniteStrain.hxx"

namespace castem {

  static void CastemCheckNDIValue(const CastemInt NDI) {
    if ((NDI != 2) && (NDI != 0) && (NDI != -1) && (NDI != -2) && (NDI != -3) &&
        (NDI != 14)) {
      std::ostringstream msg;
      msg << "CastemCheckNDIValue : unsupported modelling hypothesis (" << NDI
          << ")";
      throw(std::runtime_error(msg.str()));
    }
  }

  void CastemFiniteStrain::computeGreenLagrangeStrain(CastemReal* const e,
                                                      const CastemReal* const F,
                                                      const CastemInt NTENS,
                                                      const CastemInt NDI) {
    CastemCheckNDIValue(NDI);
    // warning : F is given in the fortran convention
    // (F0 F3 F6
    //  F1 F4 F7
    //  F2 F5 F8)
    // maxima :
    // (0.5*(F2^2+F1^2+F0^2-1)  0.5*(F2*F5+F1*F4+F0*F3) 0.5*(F2*F8+F1*F7+F0*F6)
    //  0.5*(F2*F5+F1*F4+F0*F3) 0.5*(F5^2+F4^2+F3^2-1)  0.5*(F5*F8+F4*F7+F3*F6)
    //  0.5*(F2*F8+F1*F7+F0*F6) 0.5*(F5*F8+F4*F7+F3*F6) 0.5*(F8^2+F7^2+F6^2-1)
    e[0] = 0.5 * (F[0] * F[0] + F[1] * F[1] + F[2] * F[2] - 1);
    e[1] = 0.5 * (F[3] * F[3] + F[4] * F[4] + F[5] * F[5] - 1);
    if (NDI != -2) {
      e[2] = 0.5 * (F[6] * F[6] + F[7] * F[7] + F[8] * F[8] - 1);
    } else {
      e[2] = 0.;
    }
    // warning : e must be computed using Voigt
    // notations. Extradiagonals terms are mulitplied by two
    if (NTENS == 4) {
      e[3] = F[2] * F[5] + F[1] * F[4] + F[0] * F[3];
    } else if (NTENS == 6) {
      e[3] = F[2] * F[5] + F[1] * F[4] + F[0] * F[3];
      e[4] = F[2] * F[8] + F[1] * F[7] + F[0] * F[6];
      e[5] = F[5] * F[8] + F[4] * F[7] + F[3] * F[6];
    }
  }  // end of CastemFiniteStrain::computeGreenLagrangeStrain

  void CastemFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(
      CastemReal* const STRESS,
      const CastemReal* const F,
      const CastemInt NTENS,
      const CastemInt NDI,
      const CastemReal Fzz) {
    CastemCheckNDIValue(NDI);
    // warning F is given in the fortran convention
    // (F0 F3 F6
    //  F1 F4 F7
    //  F2 F5 F8)
    const CastemReal F0 = F[0];
    const CastemReal F1 = F[1];
    const CastemReal F2 = F[2];
    const CastemReal F3 = F[3];
    const CastemReal F4 = F[4];
    const CastemReal F5 = F[5];
    const CastemReal F6 = F[6];
    const CastemReal F7 = F[7];
    const CastemReal F8 = NDI != -2 ? F[8] : Fzz;
    // determinant
    const CastemReal J = F0 * (F4 * F8 - F5 * F7) - F3 * (F1 * F8 - F2 * F7) +
                         (F1 * F5 - F2 * F4) * F6;
    // inverse of the determinant
    const CastemReal iJ = 1 / J;
    // inverse of F
    // maxima          :     F4*F8-F5*F7
    const CastemReal iF0 = iJ * (F4 * F8 - F5 * F7);
    // maxima          :     F2*F7-F1*F8
    const CastemReal iF1 = iJ * (F2 * F7 - F1 * F8);
    // maxima          :     F1*F5-F2*F4
    const CastemReal iF2 = iJ * (F1 * F5 - F2 * F4);
    // maxima          :     F5*F6-F3*F8
    const CastemReal iF3 = iJ * (F5 * F6 - F3 * F8);
    // maxima          :     F0*F8-F2*F6
    const CastemReal iF4 = iJ * (F0 * F8 - F2 * F6);
    // maxima          :     F2*F3-F0*F5
    const CastemReal iF5 = iJ * (F2 * F3 - F0 * F5);
    // maxima          :     F3*F7-F4*F6
    const CastemReal iF6 = iJ * (F3 * F7 - F4 * F6);
    // maxima          :     F1*F6-F0*F7
    const CastemReal iF7 = iJ * (F1 * F6 - F0 * F7);
    // maxima          :     F0*F4-F1*F3
    const CastemReal iF8 = iJ * (F0 * F4 - F1 * F3);
    // sk2
    const CastemReal p0 = STRESS[0];
    const CastemReal p1 = STRESS[1];
    const CastemReal p2 = STRESS[2];
    const CastemReal p3 = (NTENS >= 4) ? STRESS[3] : CastemReal(0);
    const CastemReal p4 = (NTENS == 6) ? STRESS[4] : CastemReal(0);
    const CastemReal p5 = (NTENS == 6) ? STRESS[5] : CastemReal(0);
    STRESS[0] = J * (iF3 * (p5 * iF6 + p1 * iF3 + p3 * iF0) +
                     iF0 * (p4 * iF6 + p3 * iF3 + p0 * iF0) +
                     iF6 * (p2 * iF6 + p5 * iF3 + p4 * iF0));
    STRESS[1] = J * (iF4 * (p5 * iF7 + p1 * iF4 + p3 * iF1) +
                     iF1 * (p4 * iF7 + p3 * iF4 + p0 * iF1) +
                     iF7 * (p2 * iF7 + p5 * iF4 + p4 * iF1));
    STRESS[2] = J * (iF5 * (p5 * iF8 + p1 * iF5 + p3 * iF2) +
                     iF2 * (p4 * iF8 + p3 * iF5 + p0 * iF2) +
                     iF8 * (p2 * iF8 + p5 * iF5 + p4 * iF2));
    if (NTENS == 4) {
      if (NDI == -2) {
        STRESS[2] = 0;
      }
      STRESS[3] = J * (iF3 * (p5 * iF7 + p1 * iF4 + p3 * iF1) +
                       iF0 * (p4 * iF7 + p3 * iF4 + p0 * iF1) +
                       iF6 * (p2 * iF7 + p5 * iF4 + p4 * iF1));
    } else if (NTENS == 6) {
      STRESS[3] = J * (iF3 * (p5 * iF7 + p1 * iF4 + p3 * iF1) +
                       iF0 * (p4 * iF7 + p3 * iF4 + p0 * iF1) +
                       iF6 * (p2 * iF7 + p5 * iF4 + p4 * iF1));
      STRESS[4] = J * (iF3 * (p5 * iF8 + p1 * iF5 + p3 * iF2) +
                       iF0 * (p4 * iF8 + p3 * iF5 + p0 * iF2) +
                       iF6 * (p2 * iF8 + p5 * iF5 + p4 * iF2));
      STRESS[5] = J * (iF4 * (p5 * iF8 + p1 * iF5 + p3 * iF2) +
                       iF1 * (p4 * iF8 + p3 * iF5 + p0 * iF2) +
                       iF7 * (p2 * iF8 + p5 * iF5 + p4 * iF2));
    }
  }  // end of
     // CastemFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress

  void CastemFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(
      CastemReal* const sk2,
      const CastemReal* const STRESS,
      const CastemReal* const F,
      const CastemInt NTENS,
      const CastemInt NDI,
      const CastemReal Fzz) {
    sk2[0] = STRESS[0];
    sk2[1] = STRESS[1];
    sk2[2] = STRESS[2];
    if (NTENS == 4) {
      sk2[3] = STRESS[3];
    } else if (NTENS == 6) {
      sk2[3] = STRESS[3];
      sk2[4] = STRESS[4];
      sk2[5] = STRESS[5];
    }
    CastemFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(
        sk2, F, NTENS, NDI, Fzz);
  }  // end of
     // CastemFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress

  void CastemFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(
      CastemReal* const s,
      const CastemReal* const F,
      const CastemInt NTENS,
      const CastemInt NDI,
      const CastemReal Fzz) {
    CastemCheckNDIValue(NDI);
    // warning F is given in the fortran convention
    // (F0 F3 F6
    //  F1 F4 F7
    //  F2 F5 F8)
    const CastemReal F0 = F[0];
    const CastemReal F1 = F[1];
    const CastemReal F2 = F[2];
    const CastemReal F3 = F[3];
    const CastemReal F4 = F[4];
    const CastemReal F5 = F[5];
    const CastemReal F6 = F[6];
    const CastemReal F7 = F[7];
    const CastemReal F8 = NDI != -2 ? F[8] : Fzz;
    // determinant
    const CastemReal inv_J =
        1 / (F0 * (F4 * F8 - F5 * F7) - F3 * (F1 * F8 - F2 * F7) +
             (F1 * F5 - F2 * F4) * F6);
    const CastemReal p0 = s[0];
    const CastemReal p1 = s[1];
    const CastemReal p2 = s[2];
    const CastemReal p3 = (NTENS >= 4) ? s[3] : CastemReal(0);
    const CastemReal p4 = (NTENS == 6) ? s[4] : CastemReal(0);
    const CastemReal p5 = (NTENS == 6) ? s[5] : CastemReal(0);
    // maxima :
    // F3*(p5*F6+p1*F3+p3*F0)+F0*(p4*F6+p3*F3+p0*F0)+F6*(p2*F6+p5*F3+p4*F0)
    s[0] = inv_J * (F3 * (p5 * F6 + p1 * F3 + p3 * F0) +
                    F0 * (p4 * F6 + p3 * F3 + p0 * F0) +
                    F6 * (p2 * F6 + p5 * F3 + p4 * F0));
    // maxima :
    // F4*(p5*F7+p1*F4+p3*F1)+F1*(p4*F7+p3*F4+p0*F1)+F7*(p2*F7+p5*F4+p4*F1)
    s[1] = inv_J * (F4 * (p5 * F7 + p1 * F4 + p3 * F1) +
                    F1 * (p4 * F7 + p3 * F4 + p0 * F1) +
                    F7 * (p2 * F7 + p5 * F4 + p4 * F1));
    // maxima :
    // F5*(p5*F8+p1*F5+p3*F2)+F2*(p4*F8+p3*F5+p0*F2)+F8*(p2*F8+p5*F5+p4*F2)
    s[2] = inv_J * (F5 * (p5 * F8 + p1 * F5 + p3 * F2) +
                    F2 * (p4 * F8 + p3 * F5 + p0 * F2) +
                    F8 * (p2 * F8 + p5 * F5 + p4 * F2));
    if (NTENS == 4) {
      if (NDI == -2) {
        s[2] = 0;
      }
      // maxima :
      // F3*(p5*F7+p1*F4+p3*F1)+F0*(p4*F7+p3*F4+p0*F1)+F6*(p2*F7+p5*F4+p4*F1)
      s[3] = inv_J * (F3 * (p5 * F7 + p1 * F4 + p3 * F1) +
                      F0 * (p4 * F7 + p3 * F4 + p0 * F1) +
                      F6 * (p2 * F7 + p5 * F4 + p4 * F1));
    } else if (NTENS == 6) {
      // maxima :
      // F3*(p5*F7+p1*F4+p3*F1)+F0*(p4*F7+p3*F4+p0*F1)+F6*(p2*F7+p5*F4+p4*F1)
      s[3] = inv_J * (F3 * (p5 * F7 + p1 * F4 + p3 * F1) +
                      F0 * (p4 * F7 + p3 * F4 + p0 * F1) +
                      F6 * (p2 * F7 + p5 * F4 + p4 * F1));
      // maxima :
      // F3*(p5*F8+p1*F5+p3*F2)+F0*(p4*F8+p3*F5+p0*F2)+F6*(p2*F8+p5*F5+p4*F2)
      s[4] = inv_J * (F3 * (p5 * F8 + p1 * F5 + p3 * F2) +
                      F0 * (p4 * F8 + p3 * F5 + p0 * F2) +
                      F6 * (p2 * F8 + p5 * F5 + p4 * F2));
      // maxima :
      // F4*(p5*F8+p1*F5+p3*F2)+F1*(p4*F8+p3*F5+p0*F2)+F7*(p2*F8+p5*F5+p4*F2)
      s[5] = inv_J * (F4 * (p5 * F8 + p1 * F5 + p3 * F2) +
                      F1 * (p4 * F8 + p3 * F5 + p0 * F2) +
                      F7 * (p2 * F8 + p5 * F5 + p4 * F2));
    }
  }  // end of
     // CastemFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress

  void CastemFiniteStrain::convertCSEtoCauchyTruesdellRateModuli(
      CastemReal* const DDSDDE,
      const CastemReal* const STRESS,
      const CastemReal* const F,
      const CastemInt NTENS,
      const CastemInt NDI,
      const CastemReal FZZ) {
    using namespace tfel::math;
    using namespace tfel::material;
    using TangentOperator =
        tfel::material::FiniteStrainBehaviourTangentOperatorBase;
    CastemCheckNDIValue(NDI);
    if (NTENS == 3u) {
      using tensor = tensor<1u, CastemReal>;
      stensor<1u, CastemReal> s;
      s.importTab(STRESS);
      const auto F1 = tensor::buildFromFortranMatrix(F);
      const auto CSE =
          UmatNormaliseTangentOperator<1u, CastemReal>::exe(DDSDDE);
      ST2toST2View<1u, CastemReal> CT(DDSDDE);
      CT = convert<TangentOperator::C_TRUESDELL, TangentOperator::DS_DEGL>(
          CSE, tensor::Id(), F1, s);
      CastemTangentOperator::normalize(CT);
    } else if (NTENS == 4u) {
      using tensor = tensor<2u, CastemReal>;
      stensor<2u, CastemReal> s;
      s.importTab(STRESS);
      auto F1 = tensor::buildFromFortranMatrix(F);
      if (NDI == -2) {
        F1[2] = FZZ;
      }
      const auto CSE =
          UmatNormaliseTangentOperator<2u, CastemReal>::exe(DDSDDE);
      ST2toST2View<2u, CastemReal> CT(DDSDDE);
      CT = convert<TangentOperator::C_TRUESDELL, TangentOperator::DS_DEGL>(
          CSE, tensor::Id(), F1, s);
      CastemTangentOperator::normalize(CT);
    } else if (NTENS == 6u) {
      using tensor = tensor<3u, CastemReal>;
      stensor<3u, CastemReal> s;
      s.importTab(STRESS);
      const auto F1 = tensor::buildFromFortranMatrix(F);
      const auto CSE =
          UmatNormaliseTangentOperator<3u, CastemReal>::exe(DDSDDE);
      ST2toST2View<3u, CastemReal> CT(DDSDDE);
      CT = convert<TangentOperator::C_TRUESDELL, TangentOperator::DS_DEGL>(
          CSE, tensor::Id(), F1, s);
      CastemTangentOperator::normalize(CT);
    } else {
      throw(std::runtime_error(
          "CastemFiniteStrain::convertCSEtoCauchyTruesdellRateModuli: "
          "invalid NTENS value (" +
          std::to_string(NTENS) + ")"));
    }
  }  // end of CastemFiniteStrain::convertCSEtoCauchyTruesdellRateModuli

  static void CastemFiniteStrainComputeLogarithmicStrainAndDerivative1D(
      CastemReal* const P,
      CastemReal* const E,
      const CastemReal* const F,
      const CastemInt NDI) {
    CastemCheckNDIValue(NDI);
    E[0] = log(F[0]);
    E[1] = log(F[4]);
    E[2] = log(F[8]);
    P[0] = 1 / (F[0] * F[0]);
    P[1] = 1 / (F[4] * F[4]);
    P[2] = 1 / (F[8] * F[8]);
  }  // end of CastemFiniteStrainComputeLogarithmicStrainAndDerivative1D

  static void CastemFiniteStrainComputeLogarithmicStrainAndDerivative2D(
      CastemReal* const P,
      CastemReal* const E,
      const CastemReal* const F,
      const CastemInt NDI) {
    using namespace tfel::math;
    using std::log;
    constexpr const auto cste = Cste<CastemReal>::sqrt2;
    CastemCheckNDIValue(NDI);
    tvector<3u, CastemReal> vp;
    tvector<3u, CastemReal> log_vp;
    tmatrix<3u, 3u, CastemReal> m;
    stensor<2u, CastemReal> n0;
    stensor<2u, CastemReal> n1;
    stensor<2u, CastemReal> n2;
    tensor<2u, CastemReal> f;
    tensor<2u, CastemReal>::buildFromFortranMatrix(f, F);
    if (NDI == -2) {
      f(2) = 1.;
    }
    const stensor<2u, CastemReal> C = computeRightCauchyGreenTensor(f);
    C.computeEigenVectors(vp, m);
    log_vp(0) = log(vp(0));
    log_vp(1) = log(vp(1));
    if (NDI != -2) {
      log_vp(2) = log(vp(2));
    } else {
      log_vp(2) = 0.;
    }
    stensor<2u, CastemReal>::computeEigenTensors(n0, n1, n2, m);
    StensorView<2u, CastemReal>{E} = (log_vp(0) * n0 + log_vp(1) * n1) / 2;
    if (NDI == -2) {
      E[2] = 0;
    } else {
      E[2] = log_vp(2) / 2;
    }
    E[3] *= cste;
    // computing P
    const tvector<3u, CastemReal> v0 = m.column_view<0u>();
    const tvector<3u, CastemReal> v1 = m.column_view<1u>();
    const stensor<2u, CastemReal> n01 =
        stensor<2u, CastemReal>::buildFromVectorsSymmetricDiadicProduct(v0,
                                                                        v1) /
        cste;
    if (std::abs(vp(0) - vp(1)) > 1.e-12) {
      ST2toST2View<2u, CastemReal>{P} =
          (n0 ^ n0) / vp(0) + (n1 ^ n1) / vp(1) + (n2 ^ n2) / vp(2) +
          (log_vp(0) - log_vp(1)) / (vp(0) - vp(1)) * (n01 ^ n01);
    } else {
      ST2toST2View<2u, CastemReal>{P} = (n0 ^ n0) / vp(0) + (n1 ^ n1) / vp(1) +
                                        (n2 ^ n2) / vp(2) + (n01 ^ n01) / vp(0);
    }
  }  // end of CastemFiniteStrainComputeLogarithmicStrainAndDerivative2D

  static void CastemFiniteStrainComputeLogarithmicStrainAndDerivative3D(
      CastemReal* const P, CastemReal* const E, const CastemReal* const F) {
    using namespace tfel::math;
    using std::log;
    constexpr const auto cste = Cste<CastemReal>::sqrt2;
    tvector<3u, CastemReal> vp, log_vp;
    stensor<3u, CastemReal> n0, n1, n2;
    tmatrix<3u, 3u, CastemReal> m;
    tensor<3u, CastemReal> f;
    tensor<3u, CastemReal>::buildFromFortranMatrix(f, F);
    const stensor<3u, CastemReal> C = computeRightCauchyGreenTensor(f);
    C.computeEigenVectors(vp, m);
    log_vp(0) = log(vp(0));
    log_vp(1) = log(vp(1));
    log_vp(2) = log(vp(2));
    stensor<3u, CastemReal>::computeEigenTensors(n0, n1, n2, m);
    // logarithmic strain
    StensorView<3u, CastemReal>{E} =
        (log_vp(0) * n0 + log_vp(1) * n1 + log_vp(2) * n2) / 2;
    E[3] *= cste;
    E[4] *= cste;
    E[5] *= cste;
    // computing P
    if ((std::abs(vp(0) - vp(1)) < 1.e-12) &&
        (std::abs(vp(0) - vp(2)) < 1.e-12)) {
      CastemReal vpm = (vp(0) + vp(1) + vp(2)) / 3;
      ST2toST2View<3u, CastemReal>{P} = st2tost2<3u, CastemReal>::Id() / vpm;
    } else if (std::abs(vp(0) - vp(1)) < 1.e-12) {
      const tvector<3u, CastemReal> v0 = m.column_view<0u>();
      const tvector<3u, CastemReal> v1 = m.column_view<1u>();
      const tvector<3u, CastemReal> v2 = m.column_view<2u>();
      const stensor<3u, CastemReal> n01 =
          stensor<3u, CastemReal>::buildFromVectorsSymmetricDiadicProduct(v0,
                                                                          v1) /
          cste;
      const stensor<3u, CastemReal> n02 =
          stensor<3u, CastemReal>::buildFromVectorsSymmetricDiadicProduct(v0,
                                                                          v2) /
          cste;
      const stensor<3u, CastemReal> n12 =
          stensor<3u, CastemReal>::buildFromVectorsSymmetricDiadicProduct(v1,
                                                                          v2) /
          cste;
      CastemReal vpm = (vp(0) + vp(1)) / 2;
      ST2toST2View<3u, CastemReal>{P} =
          (((n0 ^ n0) + (n1 ^ n1) + (n01 ^ n01)) / vpm + (n2 ^ n2) / vp(2) +
           (log_vp(0) - log_vp(2)) / (vpm - vp(2)) *
               ((n02 ^ n02) + (n12 ^ n12)));
    } else if (std::abs(vp(0) - vp(2)) < 1.e-12) {
      const tvector<3u, CastemReal> v0 = m.column_view<0u>();
      const tvector<3u, CastemReal> v1 = m.column_view<1u>();
      const tvector<3u, CastemReal> v2 = m.column_view<2u>();
      const stensor<3u, CastemReal> n01 =
          stensor<3u, CastemReal>::buildFromVectorsSymmetricDiadicProduct(v0,
                                                                          v1) /
          cste;
      const stensor<3u, CastemReal> n02 =
          stensor<3u, CastemReal>::buildFromVectorsSymmetricDiadicProduct(v0,
                                                                          v2) /
          cste;
      const stensor<3u, CastemReal> n12 =
          stensor<3u, CastemReal>::buildFromVectorsSymmetricDiadicProduct(v1,
                                                                          v2) /
          cste;
      CastemReal vpm = (vp(0) + vp(2)) / 2;
      ST2toST2View<3u, CastemReal>{P} =
          (((n0 ^ n0) + (n2 ^ n2) + (n02 ^ n02)) / vpm + (n1 ^ n1) / vp(1) +
           (log_vp(0) - log_vp(1)) / (vpm - vp(1)) *
               ((n01 ^ n01) + (n12 ^ n12)));
    } else if (std::abs(vp(1) - vp(2)) < 1.e-12) {
      const tvector<3u, CastemReal> v0 = m.column_view<0u>();
      const tvector<3u, CastemReal> v1 = m.column_view<1u>();
      const tvector<3u, CastemReal> v2 = m.column_view<2u>();
      const stensor<3u, CastemReal> n01 =
          stensor<3u, CastemReal>::buildFromVectorsSymmetricDiadicProduct(v0,
                                                                          v1) /
          cste;
      const stensor<3u, CastemReal> n02 =
          stensor<3u, CastemReal>::buildFromVectorsSymmetricDiadicProduct(v0,
                                                                          v2) /
          cste;
      const stensor<3u, CastemReal> n12 =
          stensor<3u, CastemReal>::buildFromVectorsSymmetricDiadicProduct(v1,
                                                                          v2) /
          cste;
      CastemReal vpm = (vp(1) + vp(2)) / 2;
      ST2toST2View<3u, CastemReal>{P} =
          ((n0 ^ n0) / vp(0) + ((n1 ^ n1) + (n2 ^ n2) + (n12 ^ n12)) / vpm +
           ((log_vp(0) - log_vp(1)) / (vp(0) - vpm)) *
               ((n01 ^ n01) + (n02 ^ n02)));
    } else {
      st2tost2<3u, CastemReal> dn0_dC;
      st2tost2<3u, CastemReal> dn1_dC;
      st2tost2<3u, CastemReal> dn2_dC;
      stensor<3u, CastemReal>::computeEigenTensorsDerivatives(
          dn0_dC, dn1_dC, dn2_dC, vp, m, 1.e-12);
      ST2toST2View<3u, CastemReal>{P} = (n0 ^ n0) / vp(0) + log_vp(0) * dn0_dC +
                                        (n1 ^ n1) / vp(1) + log_vp(1) * dn1_dC +
                                        (n2 ^ n2) / vp(2) + log_vp(2) * dn2_dC;
    }
  }

  void CastemFiniteStrain::computeLogarithmicStrainAndDerivative(
      CastemReal* const P,
      CastemReal* const E,
      const CastemReal* const F,
      const CastemInt NTENS,
      const CastemInt NDI) {
    if (NTENS == 3u) {
      CastemFiniteStrainComputeLogarithmicStrainAndDerivative1D(P, E, F, NDI);
    } else if (NTENS == 4u) {
      CastemFiniteStrainComputeLogarithmicStrainAndDerivative2D(P, E, F, NDI);
    } else {
      CastemFiniteStrainComputeLogarithmicStrainAndDerivative3D(P, E, F);
    }
  }

  void CastemFiniteStrain::computeDualStressOfLogarithmicStrainFromCauchyStress(
      CastemReal* const s,
      const CastemReal* const STRESS,
      const CastemReal* const P,
      const CastemReal* const F,
      const CastemInt NTENS,
      const CastemInt NDI,
      const CastemReal Fzz) {
    using namespace tfel::math;
    constexpr const auto cste = Cste<CastemReal>::sqrt2;
    constexpr const auto icste = Cste<CastemReal>::isqrt2;
    // now, we compute the second Piolay Kirchhoff stress
    CastemReal sk2[6u];
    // first we compute the second Piola-Kirchhoff stress
    CastemFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(
        sk2, STRESS, F, NTENS, NDI, Fzz);
    // now the dual stress of the strain tensor
    if (NTENS == 3u) {
      s[0] = sk2[0] / P[0];
      s[1] = sk2[1] / P[1];
      s[2] = sk2[2] / P[2];
    } else if (NTENS == 4u) {
      sk2[3] *= cste;
      const st2tost2<2u, CastemReal> iP(
          invert(ConstST2toST2View<2u, CastemReal>{P}));
      StensorView<2u, CastemReal>{s} =
          ConstStensorView<2u, CastemReal>{sk2} | iP;
      s[3] *= icste;
    } else if (NTENS == 6u) {
      sk2[3] *= cste;
      sk2[4] *= cste;
      sk2[5] *= cste;
      //       std::cout << "P: " << P << std::endl;
      const st2tost2<3u, CastemReal> iP(
          invert(ConstST2toST2View<3u, CastemReal>{P}));
      StensorView<3u, CastemReal>{s} =
          ConstStensorView<3u, CastemReal>{sk2} | iP;
      s[3] *= icste;
      s[4] *= icste;
      s[5] *= icste;
    }
  }

  void CastemFiniteStrain::computeCauchyStressFromDualStressOfLogarithmicStrain(
      CastemReal* const STRESS,
      const CastemReal* const s,
      const CastemReal* const P,
      const CastemReal* const F,
      const CastemInt NTENS,
      const CastemInt NDI,
      const CastemReal Fzz) {
    using namespace tfel::math;
    using std::sqrt;
    constexpr const auto icste = Cste<CastemReal>::isqrt2;
    // first we compute the second Piola-Kirchhoff stress
    if (NTENS == 3u) {
      STRESS[0] = s[0] * P[0];
      STRESS[1] = s[1] * P[1];
      STRESS[2] = s[2] * P[2];
    } else if (NTENS == 4u) {
      stensor<2u, CastemReal> T;
      T.importTab(s);
      StensorView<2u, CastemReal>{STRESS} =
          T | ConstST2toST2View<2u, CastemReal>{P};
      STRESS[3] *= icste;
    } else {
      stensor<3u, CastemReal> T;
      T.importTab(s);
      StensorView<3u, CastemReal>{STRESS} =
          T | ConstST2toST2View<3u, CastemReal>{P};
      STRESS[3] *= icste;
      STRESS[4] *= icste;
      STRESS[5] *= icste;
    }
    // now, we compute the cauchy stress
    CastemFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(
        STRESS, F, NTENS, NDI, Fzz);
  }

}  // end of namespace castem
