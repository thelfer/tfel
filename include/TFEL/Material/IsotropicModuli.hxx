/*!
 * \file   include/TFEL/Material/IsotropicModuli.hxx
 * \brief  This file defines various functions dealing with elastic coefficients.
 * \author Antoine MARTIN
 * \date   15 May 2025
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ISOTROPICMODULI_HXX
#define LIB_TFEL_MATERIAL_ISOTROPICMODULI_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Math/types.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/Lame.hxx"

namespace tfel::material {
 
  
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress, StressType>())
  struct IsotropicModuli {
    IsotropicModuli(){}; 
    virtual ~IsotropicModuli(){};
    virtual std::pair<StressType,types::real<StressType>> ToYoungNu() const& = 0;
    virtual std::pair<StressType,StressType> ToLambdaMu() const& = 0;
    virtual std::pair<StressType,StressType> ToKG() const& = 0;
  };
  
  
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress, StressType>())
  struct YoungNuModuli : IsotropicModuli<StressType> {
    StressType young;
    types::real<StressType> nu;
    YoungNuModuli(const StressType& young,const types::real<StressType>& nu) : IsotropicModuli<StressType>(), young(young), nu(nu){};
    virtual ~YoungNuModuli(){};
    
    std::pair<StressType,types::real<StressType>> ToYoungNu() const& {
      return {this->young,this->nu};
    };
    
    std::pair<StressType,StressType> ToLambdaMu() const& {
      const auto lambda = computeLambda<StressType>(this->young,this->nu);
      const auto mu = computeMu<StressType>(this->young,this->nu);
      return{lambda, mu};
    };
    
    std::pair<StressType,StressType> ToKG() const& {
      const auto young=this->young;
      const auto nu=this->nu;
      const auto kappa=young / 3. / (1 - 2 * nu);
      const auto mu=young / 2. / (1 + nu);
      return{kappa, mu};
    };
  };//end of YoungNuoduli
  
    
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress, StressType>())
  struct KGModuli : IsotropicModuli<StressType> {
    StressType kappa;
    StressType mu;
    KGModuli(const StressType& kappa,const StressType& mu) : IsotropicModuli<StressType>(), kappa(kappa), mu(mu){};
    virtual ~KGModuli(){};
    
    std::pair<StressType,types::real<StressType>> ToYoungNu() const& {
      const auto kappa = this->kappa;
      const auto mu = this->mu;
      const auto nu = (3 * kappa - 2 * mu) / (2 * mu + 6 * kappa);
      const auto young = 2 * mu * (1 + nu);
      return {young,nu};
    };
    
    std::pair<StressType,StressType> ToLambdaMu() const& {
      const auto kappa=this->kappa;
      const auto mu=this->mu;
      const auto lambda = kappa - 2 * mu /3;
      return{lambda, mu};
    };
    
    std::pair<StressType,StressType> ToKG() const& {
      return{this->kappa, this->mu};
    };
  };//end of KGModuli
  
  
   template <tfel::math::ScalarConcept StressType>
   requires(tfel::math::checkUnitCompatibility<
         tfel::math::unit::Stress, StressType>())
   struct LambdaMuModuli : IsotropicModuli<StressType> {
    StressType lambda;
    StressType mu;
    LambdaMuModuli(const StressType& lambda,const StressType& mu) : IsotropicModuli<StressType>(), lambda(lambda), mu(mu){}; 
    virtual ~LambdaMuModuli(){};
     std::pair<StressType,types::real<StressType>> ToYoungNu() const& {
      const auto lambda = this->lambda;
      const auto mu = this->mu;
      const auto nu = lambda / (2 * mu + 2 * lambda);
      const auto young = 2 * mu * (1 + nu);
      return {young,nu};
    };
    
    std::pair<StressType,StressType> ToLambdaMu() const& {
      return{this->lambda, this->mu};
    };
    
    std::pair<StressType,StressType> ToKG() const& {
      const auto lambda = this->lambda;
      const auto mu = this->mu;
      const auto kappa = lambda + 2 * mu/3;
      return{kappa, mu};
    };
  };//end of LambdaMuModuli
  
  

}  // end of namespace tfel::material

#endif /* LIB_TFEL_ISOTROPICMODULI_HXX */
