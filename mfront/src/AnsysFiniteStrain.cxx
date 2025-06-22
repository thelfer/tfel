/*!
 * \file  mfront/src/AnsysFiniteStrain.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  21/08/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
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
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "MFront/Ansys/AnsysTangentOperator.hxx"
#include "MFront/Ansys/AnsysFiniteStrain.hxx"

namespace ansys {

  void AnsysFiniteStrain::computeGreenLagrangeStrain(AnsysReal* const e,
                                                     const AnsysReal* const F,
                                                     const AnsysInt NTENS,
                                                     const bool ps) {
    // warning : F is given in the fortran convention
    // (F0 F3 F6
    //  F1 F4 F7
    //  F2 F5 F8)
    // maxima :
    // (0.5*(F2^2+F1^2+F0^2-1)  0.5*(F2*F5+F1*F4+F0*F3) 0.5*(F2*F8+F1*F7+F0*F6)
    //  0.5*(F2*F5+F1*F4+F0*F3) 0.5*(F5^2+F4^2+F3^2-1)  0.5*(F5*F8+F4*F7+F3*F6)
    //  0.5*(F2*F8+F1*F7+F0*F6) 0.5*(F5*F8+F4*F7+F3*F6) 0.5*(F8^2+F7^2+F6^2-1)
    e[0] = (F[0] * F[0] + F[1] * F[1] + F[2] * F[2] - 1) / 2;
    e[1] = (F[3] * F[3] + F[4] * F[4] + F[5] * F[5] - 1) / 2;
    if (!ps) {
      e[2] = (F[6] * F[6] + F[7] * F[7] + F[8] * F[8] - 1) / 2;
    } else {
      e[2] = 0.;
    }
    if (NTENS == 4) {
      e[3] = (F[2] * F[5] + F[1] * F[4] + F[0] * F[3]) / 2;
    } else if (NTENS == 6) {
      e[3] = (F[2] * F[5] + F[1] * F[4] + F[0] * F[3]) / 2;
      // Ansys ordering conventions differs from TFEL' ones
      e[5] = (F[2] * F[8] + F[1] * F[7] + F[0] * F[6]) / 2;
      e[4] = (F[5] * F[8] + F[4] * F[7] + F[3] * F[6]) / 2;
    }
  }  // end of AnsysFiniteStrain::computeGreenLagrangeStrain

  void AnsysFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(
      AnsysReal* const STRESS,
      const AnsysReal* const F,
      const AnsysInt NTENS,
      const bool ps,
      const AnsysReal Fzz) {
    // warning F is given in the fortran convention
    // (F0 F3 F6
    //  F1 F4 F7
    //  F2 F5 F8)
    const AnsysReal F0 = F[0];
    const AnsysReal F1 = F[1];
    const AnsysReal F2 = F[2];
    const AnsysReal F3 = F[3];
    const AnsysReal F4 = F[4];
    const AnsysReal F5 = F[5];
    const AnsysReal F6 = F[6];
    const AnsysReal F7 = F[7];
    const AnsysReal F8 = !ps ? F[8] : Fzz;
    // determinant
    const AnsysReal J = F0 * (F4 * F8 - F5 * F7) - F3 * (F1 * F8 - F2 * F7) +
                        (F1 * F5 - F2 * F4) * F6;
    // inverse of the determinant
    const AnsysReal iJ = 1 / J;
    // inverse of F
    // maxima          :     F4*F8-F5*F7
    const AnsysReal iF0 = iJ * (F4 * F8 - F5 * F7);
    // maxima          :     F2*F7-F1*F8
    const AnsysReal iF1 = iJ * (F2 * F7 - F1 * F8);
    // maxima          :     F1*F5-F2*F4
    const AnsysReal iF2 = iJ * (F1 * F5 - F2 * F4);
    // maxima          :     F5*F6-F3*F8
    const AnsysReal iF3 = iJ * (F5 * F6 - F3 * F8);
    // maxima          :     F0*F8-F2*F6
    const AnsysReal iF4 = iJ * (F0 * F8 - F2 * F6);
    // maxima          :     F2*F3-F0*F5
    const AnsysReal iF5 = iJ * (F2 * F3 - F0 * F5);
    // maxima          :     F3*F7-F4*F6
    const AnsysReal iF6 = iJ * (F3 * F7 - F4 * F6);
    // maxima          :     F1*F6-F0*F7
    const AnsysReal iF7 = iJ * (F1 * F6 - F0 * F7);
    // maxima          :     F0*F4-F1*F3
    const AnsysReal iF8 = iJ * (F0 * F4 - F1 * F3);
    // sk2
    const AnsysReal p0 = STRESS[0];
    const AnsysReal p1 = STRESS[1];
    const AnsysReal p2 = STRESS[2];
    const AnsysReal p3 = (NTENS >= 4) ? STRESS[3] : AnsysReal(0);
    const AnsysReal p4 = (NTENS == 6) ? STRESS[5] : AnsysReal(0);
    const AnsysReal p5 = (NTENS == 6) ? STRESS[4] : AnsysReal(0);
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
      if (ps) {
        STRESS[2] = 0;
      }
      STRESS[3] = J * (iF3 * (p5 * iF7 + p1 * iF4 + p3 * iF1) +
                       iF0 * (p4 * iF7 + p3 * iF4 + p0 * iF1) +
                       iF6 * (p2 * iF7 + p5 * iF4 + p4 * iF1));
    } else if (NTENS == 6) {
      STRESS[3] = J * (iF3 * (p5 * iF7 + p1 * iF4 + p3 * iF1) +
                       iF0 * (p4 * iF7 + p3 * iF4 + p0 * iF1) +
                       iF6 * (p2 * iF7 + p5 * iF4 + p4 * iF1));
      STRESS[5] = J * (iF3 * (p5 * iF8 + p1 * iF5 + p3 * iF2) +
                       iF0 * (p4 * iF8 + p3 * iF5 + p0 * iF2) +
                       iF6 * (p2 * iF8 + p5 * iF5 + p4 * iF2));
      STRESS[4] = J * (iF4 * (p5 * iF8 + p1 * iF5 + p3 * iF2) +
                       iF1 * (p4 * iF8 + p3 * iF5 + p0 * iF2) +
                       iF7 * (p2 * iF8 + p5 * iF5 + p4 * iF2));
    }
  }  // end of
     // AnsysFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress

  void AnsysFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(
      AnsysReal* const sk2,
      const AnsysReal* const STRESS,
      const AnsysReal* const F,
      const AnsysInt NTENS,
      const bool ps,
      const AnsysReal Fzz) {
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
    AnsysFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress(
        sk2, F, NTENS, ps, Fzz);
  }  // end of
     // AnsysFiniteStrain::computeSecondPiolaKirchhoffStressFromCauchyStress

  void AnsysFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress(
      AnsysReal* const s,
      const AnsysReal* const F,
      const AnsysInt NTENS,
      const bool ps,
      const AnsysReal Fzz) {
    // warning F is given in the fortran convention
    // (F0 F3 F6
    //  F1 F4 F7
    //  F2 F5 F8)
    const AnsysReal F0 = F[0];
    const AnsysReal F1 = F[1];
    const AnsysReal F2 = F[2];
    const AnsysReal F3 = F[3];
    const AnsysReal F4 = F[4];
    const AnsysReal F5 = F[5];
    const AnsysReal F6 = F[6];
    const AnsysReal F7 = F[7];
    const AnsysReal F8 = !ps ? F[8] : Fzz;
    // determinant
    const AnsysReal inv_J =
        1 / (F0 * (F4 * F8 - F5 * F7) - F3 * (F1 * F8 - F2 * F7) +
             (F1 * F5 - F2 * F4) * F6);
    // pk2 values using standard TFEL ordering
    const AnsysReal p0 = s[0];
    const AnsysReal p1 = s[1];
    const AnsysReal p2 = s[2];
    const AnsysReal p3 = (NTENS >= 4) ? s[3] : AnsysReal(0);
    const AnsysReal p4 = (NTENS == 6) ? s[5] : AnsysReal(0);
    const AnsysReal p5 = (NTENS == 6) ? s[4] : AnsysReal(0);
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
      if (ps) {
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
      // Ansys ordering conventions differs from TFEL' ones
      // maxima :
      // F3*(p5*F8+p1*F5+p3*F2)+F0*(p4*F8+p3*F5+p0*F2)+F6*(p2*F8+p5*F5+p4*F2)
      s[5] = inv_J * (F3 * (p5 * F8 + p1 * F5 + p3 * F2) +
                      F0 * (p4 * F8 + p3 * F5 + p0 * F2) +
                      F6 * (p2 * F8 + p5 * F5 + p4 * F2));
      // maxima :
      // F4*(p5*F8+p1*F5+p3*F2)+F1*(p4*F8+p3*F5+p0*F2)+F7*(p2*F8+p5*F5+p4*F2)
      s[4] = inv_J * (F4 * (p5 * F8 + p1 * F5 + p3 * F2) +
                      F1 * (p4 * F8 + p3 * F5 + p0 * F2) +
                      F7 * (p2 * F8 + p5 * F5 + p4 * F2));
    }
  }  // end of
     // AnsysFiniteStrain::computeCauchyStressFromSecondPiolaKirchhoffStress

  void AnsysFiniteStrain::computeAnsysTangentOperatorFromCSE(
      AnsysReal* const D,
      const AnsysReal* const CSE,
      const AnsysReal* const F,
      const AnsysReal* const STRESS,
      const AnsysInt NTENS,
      const bool ps) {
    using namespace tfel::math;
    using namespace tfel::material;
    using TangentOperator = FiniteStrainBehaviourTangentOperatorBase;
    if (ps) {
      std::cout << "AnsysFiniteStrain::computeAnsysTangentOperatorFromCSE: "
                << "planeStress support is not yet implemented" << std::endl;
      std::exit(-1);
    }
    if (NTENS == 4) {
      using tensor = tensor<2u, AnsysReal>;
      stensor<2u, AnsysReal> s;
      s.importTab(STRESS);
      auto F1 = tensor::buildFromFortranMatrix(F);
      const auto C = AnsysTangentOperator<AnsysReal>::convert2D(CSE);
      ST2toST2View<2u, AnsysReal> Ca(D);
      Ca = convert<TangentOperator::ABAQUS, TangentOperator::DS_DEGL>(
          C, tensor::Id(), F1, s);
      // now convert to ansys
      AnsysTangentOperator<AnsysReal>::normalize(Ca);
    } else if (NTENS == 6) {
      using tensor = tensor<3u, AnsysReal>;
      stensor<3u, AnsysReal> s;
      s.importTab(STRESS);
      auto F1 = tensor::buildFromFortranMatrix(F);
      const auto C = AnsysTangentOperator<AnsysReal>::convert3D(CSE);
      ST2toST2View<3u, AnsysReal> Ca(D);
      Ca = convert<TangentOperator::ABAQUS, TangentOperator::DS_DEGL>(
          C, tensor::Id(), F1, s);
      // now convert to ansys
      AnsysTangentOperator<AnsysReal>::normalize(Ca);
    }
  }  // end of AnsysFiniteStrain::computeAnsysTangentOperatorFromCSE

  void AnsysFiniteStrain::applyNativeFiniteStrainCorrection(
      AnsysReal* DDSDDE,
      const AnsysReal* const F1,
      const AnsysReal* const s,
      const AnsysInt NTENS) {
    using namespace tfel::math;
    // 1/J d(Js)/dde = ds/dde+1/J*(s^dJ_dde)
    auto add = [&DDSDDE, NTENS](const AnsysInt i, const AnsysInt j,
                                const AnsysReal v) {
      *(DDSDDE + i + j * NTENS) += v;
    };
    if (NTENS == 3u) {
      // plane stress
      const auto F = tensor<2u, AnsysReal>::buildFromFortranMatrix(F1);
      const auto iJ = 1 / det(F);
      add(0, 0, (*(s + 0)) * iJ);
      add(1, 0, (*(s + 1)) * iJ);
      add(2, 0, (*(s + 2)) * iJ);
      add(0, 1, (*(s + 0)) * iJ);
      add(1, 1, (*(s + 1)) * iJ);
      add(2, 1, (*(s + 2)) * iJ);
    } else if (NTENS == 4u) {
      const auto F = tensor<2u, AnsysReal>::buildFromFortranMatrix(F1);
      const auto iJ = 1 / det(F);
      // plane strain, axisymmetrical
      add(0, 0, (*(s + 0)) * iJ);
      add(1, 0, (*(s + 1)) * iJ);
      add(2, 0, (*(s + 2)) * iJ);
      add(3, 0, (*(s + 3)) * iJ);
      add(0, 1, (*(s + 0)) * iJ);
      add(1, 1, (*(s + 1)) * iJ);
      add(2, 1, (*(s + 2)) * iJ);
      add(3, 1, (*(s + 3)) * iJ);
      add(0, 2, (*(s + 0)) * iJ);
      add(1, 2, (*(s + 1)) * iJ);
      add(2, 2, (*(s + 2)) * iJ);
      add(3, 2, (*(s + 3)) * iJ);
    } else if (NTENS == 6u) {
      // tridimensional
      const auto F = tensor<3u, AnsysReal>::buildFromFortranMatrix(F1);
      const auto iJ = 1 / det(F);
      add(0, 0, (*(s + 0)) * iJ);
      add(1, 0, (*(s + 1)) * iJ);
      add(2, 0, (*(s + 2)) * iJ);
      add(3, 0, (*(s + 3)) * iJ);
      add(4, 0, (*(s + 4)) * iJ);
      add(5, 0, (*(s + 5)) * iJ);
      add(0, 1, (*(s + 0)) * iJ);
      add(1, 1, (*(s + 1)) * iJ);
      add(2, 1, (*(s + 2)) * iJ);
      add(3, 1, (*(s + 3)) * iJ);
      add(4, 1, (*(s + 4)) * iJ);
      add(5, 1, (*(s + 5)) * iJ);
      add(0, 2, (*(s + 0)) * iJ);
      add(1, 2, (*(s + 1)) * iJ);
      add(2, 2, (*(s + 2)) * iJ);
      add(3, 2, (*(s + 3)) * iJ);
      add(4, 2, (*(s + 4)) * iJ);
      add(5, 2, (*(s + 5)) * iJ);
    } else {
      tfel::raise(
          "AnsysFiniteStrain::applyNativeFiniteStrainCorrection: "
          "invalid NTENS value (" +
          std::to_string(NTENS) + ")");
    }
  }  // end of AnsysFiniteStrain::applyNativeFiniteStrainCorrection

}  // end of namespace ansys
