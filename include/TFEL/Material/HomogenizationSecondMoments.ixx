/*!
 * \file   include/TFEL/Material/HomogenizationSecondMoments.ixx
 * \author Antoine Martin
 * \date   7 January 2026
 * \brief  This file gives the second moments with some classical homogenization schemes.
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All
 * rights reserved. This project is publicly released under either the GNU GPL
 * Licence or the CECILL-A licence. A copy of thoses licences are delivered with
 * the sources of TFEL. CEA or EDF may also distribute this project under
 * specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_HOMOGENIZATIONSECONDMOMENTS_IXX
#define LIB_TFEL_MATERIAL_HOMOGENIZATIONSECONDMOMENTS_IXX

namespace tfel::material::homogenization::elasticity {

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const std::pair<types::real<StressType>,types::real<StressType>> computeMeanSquaredEquivalentStrain(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IMi,
          const types::real<StressType>& em2,
          const types::real<StressType>& eeq2){
          const auto kg0=IM0.ToKG();
          const auto kgi=IMi.ToKG();
          const auto mu0=kg0.mu;
          const auto k0=kg0.kappa;
          const auto mui=kgi.mu;
          const auto ki=kgi.kappa;
          const auto muP = 5.*mu0/2.*(k0+4./3.*mu0)/(k0+2.*mu0);
          const auto kP = k0+4./3.*mu0;
          const auto dmuP_dmu0 = 5./2.*(k0+4./3.*mu0)/(k0+2.*mu0)+5.*mu0/2.*(4./3.*(k0+2.*mu0)-2*(k0+4./3.*mu0))/(k0+2.*mu0)/(k0+2.*mu0);
          const auto dkP_dmu0 = 4./3.;
          const auto den_mu = 1+(1-f)*(mui-mu0)/muP;
          const auto den_k = 1+(1-f)*(ki-k0)/kP;
          const auto dermuHS_mu0=1+f*(-den_mu-(mui-mu0)*(1-f)*(-muP-(mui-mu0)*dmuP_dmu0)/muP/muP)/den_mu/den_mu;
          const auto derkHS_mu0 = f*(-(ki-k0)*(1-f)*(-(ki-k0)*dkP_dmu0)/kP/kP)/den_k/den_k;
          const auto dermuHS_mui=f*(den_mu-(mui-mu0)*(1-f)/muP)/den_mu/den_mu;
          const auto eeq20=1./(1-f)*(3.*derkHS_mu0*em2+dermuHS_mu0*eeq2);
          const auto eeq2i=1./f*dermuHS_mui*eeq2;
          return {eeq20,eeq2i};
          
      }
          
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      const std::pair<types::real<StressType>,types::real<StressType>> computeMeanSquaredHydrostaticStrain(
          const IsotropicModuli<StressType>& IM0,
          const types::real<StressType>& f,
          const IsotropicModuli<StressType>& IMi,
          const types::real<StressType>& em2,
          const types::real<StressType>& eeq2){
          const auto kg0=IM0.ToKG();
          const auto kgi=IMi.ToKG();
          const auto mu0=kg0.mu;
          const auto k0=kg0.kappa;
          const auto mui=kgi.mu;
          const auto ki=kgi.kappa;
          const auto muP = 5.*mu0/2.*(k0+4./3.*mu0)/(k0+2.*mu0);
          const auto kP = k0+4./3.*mu0;
          const auto den_mu = 1+(1-f)*(mui-mu0)/muP;
          const auto den_k = 1+(1-f)*(ki-k0)/kP;
          const auto dmuP_dk0 = 5.*mu0/3.*mu0/(k0+2.*mu0)/(k0+2.*mu0);
          const auto dkP_dk0 = 1.;
          const auto dermuHS_k0 = f*(mui-mu0)*(1-f)*(mui-mu0)*dmuP_dk0/muP/muP/den_mu/den_mu;
          const auto derkHS_k0 = 1+f*(-den_k-(ki-k0)*(1-f)*(-kP-(ki-k0)*dkP_dk0)/kP/kP)/den_k/den_k;
          const auto derkHS_ki = f*(den_k-(ki-k0)*(1-f)/kP)/den_k/den_k;
          const auto em20=1./(1-f)*(derkHS_k0*em2+1./3.*dermuHS_k0*eeq2);
          const auto em2i=1./f*derkHS_ki*em2;
          return {em20,em2i};
     }
     
}  // end of namespace tfel::material::homogenization::elasticity

#endif /* LIB_TFEL_MATERIAL_HOMOGENIZATIONSECONDMOMENTS_IXX */
