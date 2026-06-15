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
           StressType>()) TFEL_HOST_DEVICE std::array<types::compliance<StressType>,6> computeHillTensorComponents(
          const KGModuli<StressType>& kg0,
          const types::real<StressType> &e){
          
            const auto k0=kg0.kappa;
            const auto mu0=kg0.mu;
            const auto alpha0=1/mu0;
            const auto beta0=(6*k0+2*mu0)/(3*k0*mu0+4*mu0*mu0);
            const auto gammae=1/(1-e*e)-e/(1-e*e)/std::sqrt(std::abs(e*e-1))*std::acosh(e);
            const auto gamma=0.5*(1-gammae);
            const auto psi1=(3*gamma-1)/2/(1-e*e);
            const auto psi2=(e*e*(4*gamma-1)-gamma)/4/(1-e*e);
            const auto psi3=(e*e*(1-2*gamma)-gamma)/4/(1-e*e);
            
            const auto p1=alpha0*(1-2*gamma)+beta0*psi1;
            const auto p2=alpha0*gamma+beta0*psi2;
            const auto p3=beta0*psi3*sqrt(2);
            const auto p4=beta0*psi3*sqrt(2);
            const auto pF = alpha0*gamma+beta0*psi2/2;
            const auto pG =  alpha0*(1-gamma)/2+beta0*2*psi3;
            
            return {p1,p2,p3,p4,pF,pG};
          }
          

      template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
           tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE std::array<types::squared_compliance<StressType>,6> computeDerivativesOfHillTensorComponents(
          const KGModuli<StressType>& kg0,
          const types::real<StressType> &e,
          const std::array<types::real<StressType>,2>& dkg0){
          
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
            
            const auto dp1=dalpha0*(1-2*gamma)+dbeta0*psi1;
            const auto dp2=dalpha0*gamma+dbeta0*psi2;
            const auto dp3=dbeta0*psi3*sqrt(2);
            const auto dp4=dbeta0*psi3*sqrt(2);
            const auto dpF = dalpha0*gamma+dbeta0*psi2/2;
            const auto dpG =  dalpha0*(1-gamma)/2+dbeta0*2*psi3;
            
            return {dp1,dp2,dp3,dp4,dpF,dpG};
          }        
 

      template <tfel::math::ScalarConcept StressType>
      requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
           StressType>()) TFEL_HOST_DEVICE std::array<types::compliance<StressType>,6> computeDerivativesOfLocalisationTensorComponents(
          const KGModuli<StressType>& kg0,
          const KGModuli<StressType>& kgi,
          const types::real<StressType> &e,
          const std::array<types::real<StressType>,4>& dkg){
          
            const auto p=computeHillTensorComponents(kg0,e);
            const auto dp=computeDerivativesOfHillTensorComponents(kg0,e,{dkg[0],dkg[1]});
            
            const auto k0=kg0.kappa;
            const auto mu0=kg0.mu;
            const auto ki=kgi.kappa;
            const auto mui=kgi.mu;
            const auto dk = ki-k0;
            const auto dmu = mui-mu0;
            
            const auto ddk = dkg[2]-dkg[0];
            const auto ddmu = dkg[3]-dkg[1];
            
            using namespace tfel::math;
            const tmatrix<2,2> dP = {{dp[0],dp[2]},{dp[3],dp[1]}};
            const tmatrix<2,2> P = {{p[0],p[2]},{p[3],p[1]}};
            const tmatrix<2,2> J = {{1./3,sqrt(2)/3},{sqrt(2)/3,2./3}};
            const tmatrix<2,2> K = {{2./3,-sqrt(2)/3},{-sqrt(2)/3,1./3}};
            const tmatrix<2,2> dC = 3*dk*J+2*dmu*K;
            const tmatrix<2,2> ddC = 3*ddk*J+2*ddmu*K;
            
            const tmatrix<2,2> dB = dP*dC+P*ddC;
            const auto I = J+K;
            const tmatrix<2,2> B = I+P*dC;
            const auto Delta = det(B);
            
            const auto db1=dB(0,0);
            const auto db2=dB(1,1);
            const auto db3=dB(0,1);
            const auto db4=dB(1,0);
            const auto b1=B(0,0);
            const auto b2=B(1,1);
            const auto b3=B(0,1);
            const auto b4=B(1,0);
            
            const auto dDelta = db1*b2+b1*db2-db3*b4-b3*db4;
            
            const auto da1=(db2*Delta-b2*dDelta)/Delta/Delta;
            const auto da2=(db1*Delta-b1*dDelta)/Delta/Delta;
            const auto da3=-(db3*Delta-b3*dDelta)/Delta/Delta;
            const auto da4=-(db4*Delta-b4*dDelta)/Delta/Delta;
            
            const auto denF=1+2*p[4]*dmu;
            const auto denG=1+2*p[5]*dmu;
            const auto daF=-2*(p[4]*ddmu+dp[4]*dmu)/denF/denF;
            const auto daG=-2*(p[5]*ddmu+dp[5]*dmu)/denG/denG;
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
          if (std::abs(e - 1) < precision) {
            const auto k0=kg0.kappa;
            const auto mu0=kg0.mu;
            const auto ki=kgi.kappa;
            const auto mui=kgi.mu;
            const auto dk = ki-k0;
            const auto dmu = mui-mu0;
            const auto ddk = dkg[2]-dkg[0];
            const auto ddmu = dkg[3]-dkg[1];
            const auto dk0=dkg[0];
            const auto dmu0=dkg[1];
            
            const auto kP=k0+4*mu0/3;
            const auto muP=5*mu0/2*kP/(k0+2*mu0);
            const auto pJ = 1./3/kP;
            const auto pK = 1./2/muP;
            const auto dkP =dk0+4*dmu0/3;
            const auto dmuP=5*dmu0/2*kP/(k0+2*mu0)+5*mu0/2*(dkP*(k0+2*mu0)-kP*(dk0+2*dmu0))/(k0+2*mu0)/(k0+2*mu0);
            const auto dpJ = -dkP/3/kP/kP;
            const auto dpK = -dmuP/2/muP/muP;
            
            const auto denJ = 1+3*pJ*dk;
            const auto ddenJ = 3*dpJ*dk+3*pJ*ddk;
            const auto denK = 1+2*pK*dmu;
            const auto ddenK = 2*dpK*dmu+2*pK*ddmu;
            const auto dASphere =-ddenJ/denJ/denJ*tfel::math::st2tost2<3u,real>::J()-ddenK/denK/denK*tfel::math::st2tost2<3u,real>::K();
            return dASphere;
          } else{ 
            const auto da=internals::computeDerivativesOfLocalisationTensorComponents(kg0,kgi,e,dkg);
            using namespace tfel::math;
            return da[0]*TransverseIsotropicWalpoleBasis<real>::E1(n_)+da[1]*TransverseIsotropicWalpoleBasis<real>::E2(n_)+da[2]*TransverseIsotropicWalpoleBasis<real>::E3(n_)+da[3]*TransverseIsotropicWalpoleBasis<real>::E4(n_)+da[4]*TransverseIsotropicWalpoleBasis<real>::F(n_)+da[5]*TransverseIsotropicWalpoleBasis<real>::G(n_);      
          }
          }// end of computeDerivativesOfAxisymmetricalLocalisationTensor
 
}  // end of namespace tfel::material::homogenization::elasticity


#endif /* LIB_TFEL_MATERIAL_DERIVATIVESOFLOCALISATIONTENSORS_HXX */
