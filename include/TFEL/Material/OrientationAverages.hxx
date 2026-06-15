/*!
 * \file   include/TFEL/Material/OrientationAverages.hxx
 * \author Antoine Martin
 * \date   13 May 2026
 * \brief  This file makes the averages of orthotropic and transverse
 * isotropic tensors on different orientations.
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ORIENTATIONAVERAGES_HXX
#define LIB_TFEL_MATERIAL_ORIENTATIONAVERAGES_HXX

#include "TFEL/Math/General/Compare.hxx"
#include "TFEL/Material/LocalisationTensor.hxx"
#include "TFEL/Material/IsotropicModuli.hxx"
#include "TFEL/Material/EshelbyTolerances.hxx"
#include "TFEL/Math/ST2toST2/WalpoleBasis.hxx"

namespace tfel::material::homogenization::elasticity {

  template <unsigned short N, tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) struct EllipsoidMeanLocalisator {
           
    using real = types::real<StressType>;
    using LengthType = types::length<StressType>;
    static constexpr auto eps =
        tfel::material::EshelbyTolerances::get<tfel::math::base_type<real>>();

    TFEL_HOST_DEVICE static std::pair<real, real> Isotropic(
        const StressType& young,
        const real& nu,
        const StressType& young_i,
        const real& nu_i,
        const LengthType& a,
        const LengthType& b,
        const LengthType& c) {
      if (!((a > LengthType{0}) && (b > LengthType{0}) &&
              (c > LengthType{0}))) {
        tfel::reportContractViolation("a<=0 or b<=0 or c<=0");
      }

      const tfel::math::tvector<3u, real> n_1 = {1., 0., 0.};
      const tfel::math::tvector<3u, real> n_2 = {0., 1., 0.};
      real mu;
      real ka;
      using namespace tfel::math;
      if (areAlmostEqual(eps, b / a, real(1)) &&
          areAlmostEqual(eps, b / c, real(1))) {
        const auto A = computeSphereLocalisationTensor<StressType>(
            young, nu, young_i, nu_i);
        mu = A(3, 3) / 2;
        ka = A(0, 0) - 4 * mu / 3;
      } else if (areAlmostEqual(eps, b / a, real(1)) ||
                 areAlmostEqual(eps, a / c, real(1)) ||
                 areAlmostEqual(eps, b / c, real(1))) {
        st2tost2<3u, real> A_;
        if (areAlmostEqual(eps, (b - a) / c, real(0))) {
          A_ = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
              young, nu, young_i, nu_i, n_1, c / a);
        } else if (areAlmostEqual(eps, (c - a) / b, real(0))) {
          A_ = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
              young, nu, young_i, nu_i, n_1, b / a);
        } else {
          A_ = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
              young, nu, young_i, nu_i, n_1, a / b);
        }
        const auto A1111 = 8 * A_(1, 1) / 15 + A_(0, 0) / 5 +
                           2 * (A_(2, 0) + A_(0, 2) + 2 * A_(4, 4)) / 15;
        const auto A1122 = A_(1, 1) / 15 + A_(0, 0) / 15 + A_(1, 2) / 3 +
                           4 * A_(0, 1) / 15 + 4 * A_(1, 0) / 15 -
                           2 * A_(3, 3) / 15;
        mu = (A1111 - A1122) / 2;
        ka = (A1111 + 2 * A1122) / 3;
      } else {
        const auto A_ = computeEllipsoidLocalisationTensor<StressType>(
            young, nu, young_i, nu_i, n_1, a, n_2, b, c);
        const auto A1111 = A_(0, 0) / 5 + A_(1, 1) / 5 + A_(2, 2) / 5 +
                           (A_(0, 1) + A_(1, 0) + 2 * A_(3, 3)) / 15 +
                           (A_(0, 2) + A_(2, 0) + 2 * A_(4, 4)) / 15 +
                           (A_(1, 2) + A_(2, 1) + 2 * A_(5, 5)) / 15;
        const auto A1122 =
            A_(0, 0) / 15 + A_(1, 1) / 15 + A_(2, 2) / 15 + 2 * A_(0, 1) / 15 -
            2 * A_(3, 3) / 30 + 2 * A_(1, 0) / 15 + 2 * A_(2, 1) / 15 -
            2 * A_(5, 5) / 30 + 2 * A_(1, 2) / 15 + 2 * A_(2, 0) / 15 -
            2 * A_(4, 4) / 30 + 2 * A_(0, 2) / 15;
        mu = (A1111 - A1122) / 2;
        ka = (A1111 + 2 * A1122) / 3;
      }
      return {ka, mu};
    }  // end of Isotropic

    // overloading of the function, for IsotropicModuli objects
    TFEL_HOST_DEVICE static std::pair<real, real> Isotropic(
        const IsotropicModuli<StressType>& IM0,
        const IsotropicModuli<StressType>& IM_i,
        const LengthType& a,
        const LengthType& b,
        const LengthType& c) {
      const auto Enu0 = IM0.ToYoungNu();
      const auto Enui = IM_i.ToYoungNu();
      return Isotropic(Enu0.young, Enu0.nu, Enui.young, Enui.nu, a, b, c);
    }  // end of overloading of Isotropic

    // overloading of the function, for 2d ellipses
    TFEL_HOST_DEVICE static tfel::math::st2tost2<2u, real> Isotropic(
        const IsotropicModuli<StressType>& IM0,
        const IsotropicModuli<StressType>& IM_i,
        const LengthType& a,
        const LengthType& b) {
      const auto Enui = IM_i.ToYoungNu();
      const auto young_i = Enui.young;
      const auto nu_i = Enui.nu;
      using namespace tfel::math;
      st2tost2<3u, StressType> C_i;
      static constexpr auto value =
          StiffnessTensorAlterationCharacteristic::UNALTERED;
      computeIsotropicStiffnessTensorII<2u, value, StressType, real>(
          C_i, young_i, nu_i);
      tvector<2u, real> n_a = {1., 0.};
      const st2tost2<2u, real> A_ =
          computePlaneStrainLocalisationTensor<StressType>(IM0, C_i, n_a, a, b);

      const auto zero = real(0);
      const auto A11 = 3 * A_(0, 0) / 4 + A_(0, 1) / 4 + A_(3, 3) / 4;
      const auto A12 = A_(0, 0) / 4 + 3 * A_(0, 1) / 4 - A_(3, 3) / 4;
      const auto A13 = A_(0, 2);
      const auto A31 = A_(2, 0);
      const auto A33 = A_(2, 2);
      const auto A44 = A_(3, 3) / 4 + A_(0, 0) / 4 - A_(0, 1) / 4;

      const st2tost2<2u, real> A_moy = {A11,  A12,  A13,  zero, A12, A11,
                                        A13,  zero, A31,  A31,  A33, zero,
                                        zero, zero, zero, A44};
      return A_moy;
    }  // end of overloading of Isotropic

    // overloading of the function for semiLengths argument in 2d
    TFEL_HOST_DEVICE static tfel::math::st2tost2<2u, real> Isotropic(
        const IsotropicModuli<StressType>& IM0,
        const IsotropicModuli<StressType>& IM_i,
        const std::array<types::length<StressType>, 2u>& semiLengths) {
      return Isotropic(IM0, IM_i, semiLengths[0], semiLengths[1]);
    }  // end of Isotropic

    // overloading of the function for semiLengths argument in 3d
    TFEL_HOST_DEVICE static tfel::math::st2tost2<3u, real> Isotropic(
        const IsotropicModuli<StressType>& IM0,
        const IsotropicModuli<StressType>& IM_i,
        const std::array<types::length<StressType>, 3u>& semiLengths) {
      auto KG =
          Isotropic(IM0, IM_i, semiLengths[0], semiLengths[1], semiLengths[2]);
      auto kappaA = std::get<0>(KG);
      auto muA = std::get<1>(KG);
      return 3 * kappaA * tfel::math::st2tost2<3u, real>::J() +
             2 * muA * tfel::math::st2tost2<3u, real>::K();
    }  // end of Isotropic

    TFEL_HOST_DEVICE static tfel::math::st2tost2<3u, real>
    TransverseIsotropic(const StressType& young,
                        const real& nu,
                        const StressType& young_i,
                        const real& nu_i,
                        const tfel::math::tvector<3u, real>& n_a,
                        const LengthType& a,
                        const LengthType& b,
                        const LengthType& c) {
      if (!((a > LengthType{0}) && (b > LengthType{0}) &&
              (c > LengthType{0}))) {
        tfel::reportContractViolation("a<=0 or b<=0 or c<=0");
      }
      if (tfel::math::ieee754::fpclassify(norm(n_a)) == FP_ZERO) {
        tfel::reportContractViolation("n_a is null");
      }
      using namespace tfel::math;
      tvector<3u, real> n_;
      const tvector<3u, real> n_a_ = n_a / norm(n_a);
      if ((tfel::math::ieee754::fpclassify(n_a[1]) != FP_ZERO) ||
          (tfel::math::ieee754::fpclassify(n_a[2]) != FP_ZERO)) {
        n_ = {1., 0., 0.};
      } else {
        n_ = {0., 1., 0.};
      }
      auto n_2 = cross_product(n_a_, n_);
      n_2 = n_2 / norm(n_2);
      const auto n_3 = cross_product(n_a_, n_2);
      const tvector<3u, real> n_x = {1., 0., 0.};
      const tvector<3u, real> n_y = {0., 1., 0.};
      const tvector<3u, real> n_z = {0., 0., 1.};
      st2tost2<3u, real> A;
      if (areAlmostEqual(eps, b / a, real(1)) &&
          areAlmostEqual(eps, c / b, real(1))) {
        A = computeSphereLocalisationTensor<StressType>(young, nu, young_i,
                                                        nu_i);
      } else if (areAlmostEqual(eps, (c - b) / a, real(0))) {
        const auto A_ =
            computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
                young, nu, young_i, nu_i, n_z, a / b);
        const rotation_matrix<real> r = {n_2[0],  n_2[1],  n_2[2],
                                         n_3[0],  n_3[1],  n_3[2],
                                         n_a_[0], n_a_[1], n_a_[2]};
        A = change_basis(A_, r);
      } else {
        st2tost2<3u, real> A_;
        if (areAlmostEqual(eps, (a - b) / c, real(0))) {
          A_ = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
              young, nu, young_i, nu_i, n_y, c / a);
        } else if (areAlmostEqual(eps, (a - c) / b, real(0))) {
          A_ = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
              young, nu, young_i, nu_i, n_x, b / a);
        } else {
          A_ = computeEllipsoidLocalisationTensor<StressType>(
              young, nu, young_i, nu_i, n_z, a, n_x, b, c);
        }
        const auto A11 = 3 * (A_(0, 0) + A_(1, 1)) / 8 +
                         (A_(0, 1) + A_(1, 0) + 2 * A_(3, 3)) / 8;
        const auto A22 = A11;
        const auto A33 = A_(2, 2);
        const auto A12 = (A_(0, 0) + A_(1, 1)) / 8 +
                         3 * (A_(0, 1) + A_(1, 0)) / 8 - 2 * A_(3, 3) / 8;
        const auto A21 = A12;
        const auto A13 = (A_(0, 2) + A_(1, 2)) / 2;
        const auto A23 = A13;
        const auto A31 = (A_(2, 1) + A_(2, 0)) / 2;
        const auto A32 = A31;
        const auto A44 = 2 * ((A_(0, 0) + A_(1, 1) - A_(0, 1) - A_(1, 0)) / 8 +
                              A_(3, 3) / 4);
        const auto A55 = 2 * (A_(4, 4) / 4 + A_(5, 5) / 4);
        const auto A66 = A55;
        const auto zero = real{0};
        const st2tost2<3u, real> A_moy = {
            A11,  A12,  A13,  zero, zero, zero, A21,  A22,  A23,
            zero, zero, zero, A31,  A32,  A33,  zero, zero, zero,
            zero, zero, zero, A44,  zero, zero, zero, zero, zero,
            zero, A55,  zero, zero, zero, zero, zero, zero, A66};
        const rotation_matrix<real> r = {n_2[0],  n_2[1],  n_2[2],
                                         n_3[0],  n_3[1],  n_3[2],
                                         n_a_[0], n_a_[1], n_a_[2]};
        A = change_basis(A_moy, r);
      }
      return A;
    }  // end of TransverseIsotropic

    // overloading of the function, for IsotropicModuli objects
    TFEL_HOST_DEVICE static tfel::math::st2tost2<3u, real>
    TransverseIsotropic(const IsotropicModuli<StressType>& IM0,
                        const IsotropicModuli<StressType>& IM_i,
                        const tfel::math::tvector<3u, real>& n_a,
                        const LengthType& a,
                        const LengthType& b,
                        const LengthType& c) {
      const auto Enu0 = IM0.ToYoungNu();
      const auto Enui = IM_i.ToYoungNu();
      return TransverseIsotropic(Enu0.young, Enu0.nu, Enui.young, Enui.nu, n_a,
                                 a, b, c);
    }  // end of overloading of TransverseIsotropic

    TFEL_HOST_DEVICE static tfel::math::st2tost2<3u, real> Oriented(
        const StressType& young,
        const real& nu,
        const StressType& young_i,
        const real& nu_i,
        const tfel::math::tvector<3u, real>& n_a,
        const LengthType& a,
        const tfel::math::tvector<3u, real>& n_b,
        const LengthType& b,
        const LengthType& c) {
      if (!((a > LengthType{0}) && (b > LengthType{0}) &&
              (c > LengthType{0}))) {
        tfel::reportContractViolation("a<=0 or b<=0 or c<=0");
      }
      if (!(tfel::math::ieee754::fpclassify(n_a | n_b) == FP_ZERO)) {
        tfel::reportContractViolation("n_a and n_b not normals");
      }
      if (tfel::math::ieee754::fpclassify(norm(n_a)) == FP_ZERO) {
        tfel::reportContractViolation("n_a is null");
      }
      if (tfel::math::ieee754::fpclassify(norm(n_b)) == FP_ZERO) {
        tfel::reportContractViolation("n_b is null");
      }
      using namespace tfel::math;
      st2tost2<3u, real> A;
      if (areAlmostEqual(eps, b / a, real(1)) and
          areAlmostEqual(eps, b / c, real(1))) {
        A = computeSphereLocalisationTensor<StressType>(young, nu, young_i,
                                                        nu_i);
      } else if (areAlmostEqual(eps, (a - b) / c, real(0))) {
        tvector<3u, real> n_1 = tfel::math::cross_product(n_a, n_b);
        n_1 = n_1 / norm(n_1);
        A = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
            young, nu, young_i, nu_i, n_1, c / a);
      } else if (areAlmostEqual(eps, (a - c) / b, real(0))) {
        A = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
            young, nu, young_i, nu_i, n_b, b / a);
      } else if (areAlmostEqual(eps, (c - b) / a, real(0))) {
        A = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
            young, nu, young_i, nu_i, n_a, a / b);
      } else {
        A = computeEllipsoidLocalisationTensor<StressType>(
            young, nu, young_i, nu_i, n_a, a, n_b, b, c);
      }
      return A;
    }  // end of Oriented

    // overloading of the function, for IsotropicModuli objects
    TFEL_HOST_DEVICE static tfel::math::st2tost2<3u, real> Oriented(
        const IsotropicModuli<StressType>& IM0,
        const IsotropicModuli<StressType>& IM_i,
        const tfel::math::tvector<3u, real>& n_a,
        const LengthType& a,
        const tfel::math::tvector<3u, real>& n_b,
        const LengthType& b,
        const LengthType& c) {
      const auto Enu0 = IM0.ToYoungNu();
      const auto Enui = IM_i.ToYoungNu();
      return Oriented(Enu0.young, Enu0.nu, Enui.young, Enui.nu, n_a, a, n_b, b,
                      c);
    }  // end of overloading of Oriented


    TFEL_HOST_DEVICE static tfel::math::st2tost2<3u,real> UserDefinedDistributionOfSpheroids(
        const IsotropicModuli<StressType>& IM0,
        const IsotropicModuli<StressType>& IMi,
        const LengthType& a,
        const LengthType& b,
        const tfel::math::stensor<3u, real>& A2,
        const tfel::math::st2tost2<3u, real>& A4) {
      if (!((a > LengthType{0}) && (b > LengthType{0}))) {
        tfel::reportContractViolation("a<=0 or b<=0");
      }

      tfel::math::st2tost2<3u,real> A;
      const tfel::math::tvector<3u, real> n_1 = {1., 0., 0.};
      
      using namespace tfel::math;
      if (areAlmostEqual(eps, b / a, real(1))) {
        A = computeSphereLocalisationTensor<StressType>(
            IM0, IMi);
      } else {
        const auto A_ = computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
        IM0, IMi, n_1, a/b);  
        const auto AW = TransverseIsotropicWalpoleBasis<real>::components(n_1,A_);
        const auto I = tfel::math::st2tost2<3u,real>::Id();
        const auto J = tfel::math::st2tost2<3u,real>::J();
        const auto id = stensor<3u,real>::Id();
        const auto E1_=A4;
        const auto A2i = A2^id;
        const auto iA2 = id^A2;
        const auto E2_ = 0.5*(3*J+A4-A2i-iA2);
        const auto E3_=1/sqrt(2)*(A2i-A4);
        const auto E4_=1/sqrt(2)*(iA2-A4);
      const auto F_=I-TransverseIsotropicWalpoleBasis<real>::dyadic_ov(id,A2)-TransverseIsotropicWalpoleBasis<real>::dyadic_ov(A2,id)-0.5*(3*J-A4-A2i-iA2);
      const auto G_=TransverseIsotropicWalpoleBasis<real>::dyadic_ov(id,A2)+TransverseIsotropicWalpoleBasis<real>::dyadic_ov(A2,id)-2*A4;
        A = AW[0]*E1_+AW[1]*E2_+AW[2]*E3_+AW[3]*E4_+AW[4]*F_+AW[5]*G_;
      }
      return A;
    }  // end of UserDefinedDistributionOfSpheroids

  };  // end of struct EllipsoidMeanLocalisator ;
  
}  // end of namespace tfel::material::homogenization::elasticity


#endif /* LIB_TFEL_MATERIAL_ORIENTATIONAVERAGES_HXX */
