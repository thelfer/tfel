/*!
 * \file   include/TFEL/Material/LocalisationTensor.ixx
 * \author Antoine Martin
 * \date   18 June 2025
 * \brief  This file defines the localisation tensor for an ellipsoidal
 * inclusion embedded in an isotropic matrix. \copyright Copyright (C) 2006-2018
 * CEA/DEN, EDF R&D. All rights reserved. This project is publicly released
 * under either the GNU GPL Licence or the CECILL-A licence. A copy of thoses
 * licences are delivered with the sources of TFEL. CEA or EDF may also
 * distribute this project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_LOCALISATIONTENSOR_IXX
#define LIB_TFEL_MATERIAL_LOCALISATIONTENSOR_IXX

#include <cmath>
#include <numbers>
#include <stdexcept>
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Material/LocalisationTensor.hxx"

namespace tfel::material::homogenization::elasticity {

  namespace internals {
    template <unsigned short int N, tfel::math::ScalarConcept StressType>
    requires(tfel::math::checkUnitCompatibility<
             tfel::math::unit::Stress,
             StressType>()) TFEL_HOST_DEVICE tfel::math::
        st2tost2<N, types::real<StressType>> computeLocalisationTensorBase(
            tfel::math::st2tost2<N, StressType>& C_0,
            const tfel::math::st2tost2<N, tfel::types::compliance<StressType>>&
                P0,
            const tfel::math::st2tost2<N, StressType>& C_i) {

      using namespace tfel::math;
      const auto C = C_i - C_0;
      const auto Pr = P0 * C;
      const auto A = invert(st2tost2<N, types::real<StressType>>::Id() + Pr);
      return A;
    }  // end of computeLocalisationTensorBase

  }  // namespace internals

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<2u, types::real<StressType>> computeDiskPlaneStrainLocalisationTensor(
          const IsotropicModuli<StressType>& IM_0,
          const tfel::math::st2tost2<2u, StressType>& C_i) {
          using real=tfel::types::real<StressType>;
	const auto P0 = computeDiskPlaneStrainHillTensor<StressType>(IM_0);
	const auto pair0 = IM_0.ToYoungNu();
	const auto young = std::get<0>(pair0);
	const auto nu = std::get<1>(pair0);
	if ((nu > real(0.5)) || (nu < real(-1))) {
	tfel::reportContractViolation("nu>0.5 or nu<-1");
	}
	tfel::math::st2tost2<2u, StressType> C_0;
	static constexpr auto value =
	  StiffnessTensorAlterationCharacteristic::UNALTERED;
	computeIsotropicStiffnessTensorII<2u, value, StressType, real>(C_0, young,
		                                                    nu);
    return internals::computeLocalisationTensorBase<2u, StressType>(C_0, P0,
                                                                    C_i);
  }  // end of computeDiskPlaneStrainLocalisationTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<2u, types::real<StressType>> computePlaneStrainLocalisationTensor(
          const IsotropicModuli<StressType>& IM_0,
          const tfel::math::st2tost2<2u, StressType>& C_i,
          const tfel::math::tvector<2u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const types::length<StressType>& b) {
          using real=tfel::types::real<StressType>;
	const auto P0 = computePlaneStrainHillTensor<StressType>(IM_0, n_a, a, b);
	const auto pair0 = IM_0.ToYoungNu();
	const auto young = std::get<0>(pair0);
	const auto nu = std::get<1>(pair0);
	if ((nu > real(0.5)) || (nu < real(-1))) {
	tfel::reportContractViolation("nu>0.5 or nu<-1");
	}
	tfel::math::st2tost2<2u, StressType> C_0;
	static constexpr auto value =
	  StiffnessTensorAlterationCharacteristic::UNALTERED;
	computeIsotropicStiffnessTensorII<2u, value, StressType, real>(C_0, young,
		                                                    nu);
    return internals::computeLocalisationTensorBase<2u, StressType>(C_0, P0,
                                                                    C_i);
  }  // end of computePlaneStrainLocalisationTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
      TFEL_HOST_DEVICE tfel::math::
          st2tost2<3u, types::real<StressType>> computeSphereLocalisationTensor(
              const StressType& young,
              const types::real<StressType>& nu,
              const StressType& young_i,
              const types::real<StressType>& nu_i) {
    using real = types::real<StressType>;

    if (not(young > StressType{0})) {
      tfel::reportContractViolation("E<=0");
    }
    if ((nu > real(0.5)) || (nu < real(-1))) {
      tfel::reportContractViolation("nu>0.5 or nu<-1");
    }
    const auto kaS = (1 + nu) / 9 / (1 - nu);
    const auto muS = 2 * (4 - 5 * nu) / 30 / (1 - nu);
    const auto k0 = young / 3 / (1 - 2 * nu);
    const auto mu0 = young / 2 / (1 + nu);
    const auto k_i = young_i / 3 / (1 - 2 * nu_i);
    const auto mu_i = young_i / 2 / (1 + nu_i);
    const auto mu = 1 / (2 + 4 * muS * (mu_i - mu0) / mu0);
    const auto ka = 1 / (3 + 9 * kaS * (k_i - k0) / k0);
    using namespace tfel::math;
    return 3 * ka * st2tost2<3u, real>::J() + 2 * mu * st2tost2<3u, real>::K();
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>())
  TFEL_HOST_DEVICE tfel::math::st2tost2<3u, types::real<StressType>>
  computeSphereLocalisationTensor(const IsotropicModuli<StressType>& IM0,
                                  const IsotropicModuli<StressType>& IM_i) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeSphereLocalisationTensor<StressType>(
        std::get<0>(Enu0), std::get<1>(Enu0), std::get<0>(Enui),
        std::get<1>(Enui));
  }  // end of function SphereLocalisationTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeAxisymmetricalEllipsoidLocalisationTensor(
          const StressType& young,
          const types::real<StressType>& nu,
          const StressType& young_i,
          const types::real<StressType>& nu_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::real<StressType>& e) {
    using real = types::real<StressType>;
    if ((nu > real(0.5)) || (nu < real(-1))) {
      tfel::reportContractViolation("nu>0.5 or nu<-1");
    }
    if (not(e > real(0))) {
      tfel::reportContractViolation("e<=0");
    }
    const auto P0 = computeAxisymmetricalHillPolarisationTensor<StressType>(
        young, nu, n_a, e);

    tfel::math::st2tost2<3u, StressType> C_0;
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_0, young,
                                                                   nu);
    tfel::math::st2tost2<3u, StressType> C_i;
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_i, young_i,
                                                                   nu_i);
    using namespace tfel::math;
    const st2tost2<3u, StressType> C = C_i - C_0;
    const auto Pr = P0 * C;
    const auto A = invert(st2tost2<3u, real>::Id() + Pr);
    return A;
  }

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeAxisymmetricalEllipsoidLocalisationTensor(
          const IsotropicModuli<StressType>& IM0,
          const IsotropicModuli<StressType>& IM_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::real<StressType>& e) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeAxisymmetricalEllipsoidLocalisationTensor<StressType>(
        std::get<0>(Enu0), std::get<1>(Enu0), std::get<0>(Enui),
        std::get<1>(Enui), n_a, e);
  }  // end of function computeAxisymmetricalEllipsoidLocalisationTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeEllipsoidLocalisationTensor(
          const IsotropicModuli<StressType>& IM0,
          const tfel::math::st2tost2<3u, StressType>& C_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    using real = types::real<StressType>;
    using LengthType = types::length<StressType>;

    const auto Enu0 = IM0.ToYoungNu();
    const auto young0 = std::get<0>(Enu0);
    const auto nu0 = std::get<1>(Enu0);

    if ((nu0 > real(0.5)) || (nu0 < real(-1))) {
      tfel::reportContractViolation("nu0>0.5 or nu0<-1");
    }
    if (not((a > LengthType{0}) and (b > LengthType{0}) and
            (c > LengthType{0}))) {
      tfel::reportContractViolation("a<=0 or b<=0 or c<=0");
    }
    const auto P0 = computeHillPolarisationTensor<StressType>(young0, nu0, n_a,
                                                              a, n_b, b, c);

    tfel::math::st2tost2<3u, StressType> C_0;
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    computeIsotropicStiffnessTensorII<3u, value, StressType, real>(C_0, young0,
                                                                   nu0);
    using namespace tfel::math;
    const auto C = C_i - C_0;
    const auto Pr = P0 * C;
    const auto A = invert(st2tost2<3u, real>::Id() + Pr);
    return A;
  }  // end of computeEllipsoidLocalisationTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeEllipsoidLocalisationTensor(
          const StressType& young,
          const types::real<StressType>& nu,
          const StressType& young_i,
          const types::real<StressType>& nu_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    tfel::math::st2tost2<3u, StressType> C_i;
    static constexpr auto value =
        StiffnessTensorAlterationCharacteristic::UNALTERED;
    computeIsotropicStiffnessTensorII<3u, value, StressType,
                                      types::real<StressType>>(C_i, young_i,
                                                               nu_i);
    const auto Enu0 = YoungNuModuli<StressType>(young, nu);
    return computeEllipsoidLocalisationTensor<StressType>(Enu0, C_i, n_a, a,
                                                          n_b, b, c);
  }  // end of overload of computeEllipsoidLocalisationTensor

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::real<StressType>> computeEllipsoidLocalisationTensor(
          const IsotropicModuli<StressType>& IM0,
          const IsotropicModuli<StressType>& IM_i,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const types::length<StressType>& a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const types::length<StressType>& b,
          const types::length<StressType>& c) {
    const auto Enu0 = IM0.ToYoungNu();
    const auto Enui = IM_i.ToYoungNu();
    return computeEllipsoidLocalisationTensor<StressType>(
        std::get<0>(Enu0), std::get<1>(Enu0), std::get<0>(Enui),
        std::get<1>(Enui), n_a, a, n_b, b, c);
  }  // end of second overload of computeEllipsoidLocalisationTensor
  
  
   template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>())
  struct IsotropicLocalisationTensor<2u, StressType> {
  static TFEL_HOST_DEVICE tfel::math::st2tost2<2u, types::real<StressType>> exe(const IsotropicModuli<StressType>& IM0,
          const tfel::math::st2tost2<2u, StressType>& C_i_loc,
          const tfel::math::tvector<2u, types::real<StressType>>& n_a,
          const std::array<types::length<StressType>,2u>& semiLengths) {
      return computePlainStrainLocalisationTensor<StressType>(IM0,C_i_loc,n_a,semiLengths[0],semiLengths[1]);
    }
  };//end of IsotropicLocalisationTensor<2u, StressType>
  
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>())
  struct IsotropicLocalisationTensor<3u, StressType> {
  static TFEL_HOST_DEVICE tfel::math::st2tost2<3u, types::real<StressType>> exe(const IsotropicModuli<StressType>& IM0,
          const tfel::math::st2tost2<3u, StressType>& C_i_loc,
          const tfel::math::tvector<3u, types::real<StressType>>& n_a,
          const tfel::math::tvector<3u, types::real<StressType>>& n_b,
          const std::array<types::length<StressType>,3u>& semiLengths) {
      return computeEllipsoidLocalisationTensor<StressType>(IM0,C_i_loc,n_a,semiLengths[0],n_b,semiLengths[1],semiLengths[2]);
    }
  };//end of IsotropicLocalisationTensor<3u, StressType>
  
  template <unsigned short int N,tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<N, types::real<StressType>> computeIsotropicLocalisationTensor(
          const IsotropicModuli<StressType>& IM0,
          const tfel::math::st2tost2<N, StressType>& C_i,
          const tfel::math::tvector<N, types::real<StressType>>& n_a,
          const tfel::math::tvector<N, types::real<StressType>>& n_b,
          const std::array<types::length<StressType>,N>& semiLengths){
     return IsotropicLocalisationTensor<N,StressType>::exe(IM0,C_i,n_a,n_b,semiLengths);     
    }// end of computeIsotropicLocalisationTensor

}  // end of namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_LOCALISATIONTENSOR_IXX */
