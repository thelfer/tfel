/*!
 * \file   include/TFEL/Material/DerivativesOfLocalisationTensors.hxx
 * \author Antoine Martin
 * \date   7 June 2026
 * \brief  This file computes the derivatives of a localisaiton tensor.
 * \copyright Copyright (C) 2006-2018 CEA/DEN,
 * EDF R&D. All rights reserved. This project is publicly released under either
 * the GNU GPL Licence with linking exception or the CECILL-A licence. A copy of
 * thoses licences are delivered with the sources of TFEL. CEA or EDF may also
 * distribute this project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_DERIVATIVESOFLOCALISATIONTENSORS_HXX
#define LIB_TFEL_MATERIAL_DERIVATIVESOFLOCALISATIONTENSORS_HXX

#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/IsotropicModuli.hxx"
#include "TFEL/Math/ST2toST2/WalpoleBasis.hxx"
#include "TFEL/Material/EshelbyTolerances.hxx"
#include <stdexcept>

namespace tfel::material::homogenization::elasticity {

  

    namespace internals{

      template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE std::array<types::squared_compliance<StressType>,6> computeDerivativesOfHillTensorComponents(
          const KGModuli<StressType>& kg0,
          const types::real<StressType> &e,
          const std::array<types::real<StressType>,2>& dkg0){
          using real=types::real<StressType>;
          
            const auto k0=kg0.kappa;
            const auto mu0=kg0.mu;
            const auto dk0=dkg0[0];
            const auto dmu0=dkg0[1];
            const auto dalpha0=-dmu0/mu0/mu0;
            const auto den = 3*k0*mu0+4*mu0*mu0 ; 
            const auto dbeta0=((6*dk0+2*dmu0)*den-(6*k0+2*mu0)*(3*k0*dmu0+3*dk0*mu0+8*mu0*dmu0))/den/den;
            const auto gammae=1/(1-e*e)-e/(1-e*e)/std::sqrt(std::abs(e*e-1))*std::acosh(e);
            const auto gamma=0.5*(1-gammae);
            const auto psi1=(3*gamma-1)/2/(1-e*e);
            const auto psi2=(e*e*(4*gamma-1)-gamma)/4/(1-e*e);
            const auto psi3=(e*e*(1-2*gamma)-gamma)/4/(1-e*e);
            
            const auto p1=dalpha0*(1-2*gamma)+dbeta0*psi1;
            const auto p2=dalpha0*gamma+dbeta0*psi2;
            const auto p3=dbeta0*psi3*sqrt(2);
            const auto p4=dbeta0*psi3*sqrt(2);
            const auto pF = dalpha0*gamma+dbeta0*psi2/2;
            const auto pG =  dalpha0*(1-gamma)/2+dbeta0*2*psi3;
            
            return {p1,p2,p3,p4,pF,pG};
          }        
 

      template <tfel::math::ScalarConcept StressType>
      requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE std::array<types::compliance<StressType>,6> computeDerivativesOfLocalisationTensorComponents(
          const KGModuli<StressType>& kg0,
          const KGModuli<StressType>& kgi,
          const types::real<StressType> &e,
          const std::array<types::real<StressType>,4>& dkg){
            const auto dp=computeDerivativesOfHillTensorComponents(kg0,e,{dkg[0],dkg[1]});
            const auto da1=1;
            const auto da2=1;
            const auto da3=1;
            const auto da4=1;
            const auto daF=1;
            const auto daG=1;
            return {da1,da2,da3,da4,daF,daG};
          }

    }// end of internals


  /*!
   * This function computes the derivatives of the Hill tensor
   * of a spheroid embedded in an isotropic matrix (the derivations
   * are computed w.r.t. the isotropic moduli of the matrix).
   * \return a st2tost2
   * \tparam StressType: type of the elastic constants
   * \param[in] kg0 : KGModuli of the matrix
   * \param[in] n_ : vector which defines the axis of the spheroid
   * \param[in] e: aspect ratio of the spheroid (e>1 : prolate, e<1 :
   * oblate)
   * \param[in] dkg0 : for a derivation w.r.t. the bulk modulus, this std::array<real,2>
   * must be {1,0}, and {0,1} for a derivation w.r.t. the shear modulus.
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::st2tost2<3u,types::squared_compliance<StressType>> computeDerivativesOfAxisymmetricalHillTensor(
          const KGModuli<StressType>& kg0,
          const tfel::math::tvector<3u, types::real<StressType>> &n_,
          const types::real<StressType> &e,
          const std::array<types::real<StressType>,2>& dkg0,
          const tfel::math::base_type<StressType> precision = tfel::material::EshelbyTolerances::get<tfel::math::base_type<StressType>>()){
          using real=types::real<StressType>;
          
          if (std::abs(e - 1) < precision) {
            const auto k0=kg0.kappa;
            const auto mu0=kg0.mu;
            const auto dk0=dkg0[0];
            const auto dmu0=dkg0[1];
            const auto kP=k0+4*mu0/3;
            const auto muP=5*mu0/2*kP/(k0+2*mu0);
            const auto dkP =dk0+4*dmu0/3;
            const auto dmuP=5*dmu0/2*kP/(k0+2*mu0)+5*mu0/2*(dkP*(k0+2*mu0)-kP*(dk0+2*dmu0))/(k0+2*mu0)/(k0+2*mu0);
            const auto dPSphere =-dkP/3/kP/kP*tfel::math::st2tost2<3u,real>::J()-dmuP/2/muP/muP*tfel::math::st2tost2<3u,real>::K();
            return dPSphere;
          } else{        
            const auto dp=internals::computeDerivativesOfHillTensorComponents(kg0,e,dkg0);
            using namespace tfel::math;
            return dp[0]*TransverseIsotropicWalpoleBasis<real>::E1(n_)+dp[1]*TransverseIsotropicWalpoleBasis<real>::E2(n_)+dp[2]*TransverseIsotropicWalpoleBasis<real>::E3(n_)+dp[3]*TransverseIsotropicWalpoleBasis<real>::E4(n_)+dp[4]*TransverseIsotropicWalpoleBasis<real>::F(n_)+dp[5]*TransverseIsotropicWalpoleBasis<real>::G(n_);
          }        
          }// end of computeDerivativesOfAxisymmetricalHillTensor
 
 /*!
   * This function computes the derivatives of the localisation tensor
   * of a locally isotropic spheroid embedded in an isotropic matrix
   * (the derivations are computed w.r.t. the isotropic moduli of the matrix
   * and of the spheroid).
   * \return a st2tost2
   * \tparam StressType: type of the elastic constants
   * \param[in] kg0 : KGModuli of the matrix
   * \param[in] kgi : KGModuli of the spheroid
   * \param[in] n_ : vector which defines the axis of the spheroid
   * \param[in] e: aspect ratio of the spheroid (e>1 : prolate, e<1 :
   * oblate)
   * \param[in] dkg : for a derivation w.r.t. the matrix bulk modulus, this std::array<real,4>
   * must be {1,0,0,0}; for a derivation w.r.t. the matrix shear modulus, it must be {0,1,0,0},
   * etc... 
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE tfel::math::
      st2tost2<3u, types::compliance<StressType>> computeDerivativesOfAxisymmetricalLocalisationTensor(
          const KGModuli<StressType>& kg0,
          const KGModuli<StressType>& kgi,
          const tfel::math::tvector<3u, types::real<StressType>> &n_,
          const types::real<StressType> &e,
          const std::array<types::real<StressType>,4>& dkg,
          const tfel::math::base_type<StressType> precision = tfel::material::EshelbyTolerances::get<tfel::math::base_type<StressType>>()){
          using real=types::real<StressType>;
          
            const auto da=internals::computeDerivativesOfLocalisationTensorComponents(kg0,kgi,e,dkg);
            using namespace tfel::math;
            return da[0]*TransverseIsotropicWalpoleBasis<real>::E1(n_)+da[1]*TransverseIsotropicWalpoleBasis<real>::E2(n_)+da[2]*TransverseIsotropicWalpoleBasis<real>::E3(n_)+da[3]*TransverseIsotropicWalpoleBasis<real>::E4(n_)+da[4]*TransverseIsotropicWalpoleBasis<real>::F(n_)+da[5]*TransverseIsotropicWalpoleBasis<real>::G(n_);      
          }// end of computeDerivativesOfAxisymmetricalLocalisationTensor
 
}  // end of namespace tfel::material::homogenization::elasticity


#endif /* LIB_TFEL_MATERIAL_DERIVATIVESOFLOCALISATIONTENSORS_HXX */
