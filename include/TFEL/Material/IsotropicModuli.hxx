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

  template<tfel::math::ScalarConcept StressType>
  enum struct IsotropicModuli{
   KG,
   LambdaMu,
   YoungNu
  };
  
  template <>
  struct IsotropicModuli<StressType>::KG {
    StressType kappa;
    StressType mu;
  };
  
  
  template <>
  struct IsotropicModuli<StressType>::LambdaMu {
    StressType lambda;
    StressType mu;
  };
  
  template <>
  struct IsotropicModuli<StressType>::YoungNu {
    StressType young;
    types::real<StressType> nu;
  };
  
  
  template <tfel::math::ScalarConcept StressType>
  TFEL_HOST_DEVICE IsotropicModuli<StressType>::YoungNu KGtoYoungNu(
      const IsotropicModuli<StressType>::KG& KG) {
    const auto kappa = KG.kappa;
    const auto mu = KG.mu;
    const auto nu = (3 * kappa - 2 * mu) / (2 * mu + 6 * kappa);
    const auto young = 2 * mu * (1 + nu);
    const IsotropicModuli<StressType>::YoungNu YoungNu = {.young = young, .nu = nu};
    return YoungNu;
  }
  
  
  template <tfel::math::ScalarConcept StressType>
  TFEL_HOST_DEVICE IsotropicModuli<StressType>::KG YoungNutoKG(
      const IsotropicModuli<StressType>::YoungNu& YoungNu) {
    const auto young = YoungNu.young;
    const auto nu = YoungNu.nu;
    const IsotropicModuli<StressType>::KG KG = {.kappa = young / 3. / (1 - 2 * nu),
                                     .mu = young / 2. / (1 + nu)};
    return KG;
  }
  
  
  template <tfel::math::ScalarConcept StressType>
  TFEL_HOST_DEVICE IsotropicModuli<StressType>::LambdaMu KGtoLambdaMu(
      const IsotropicModuli<StressType>::KG& KG) {
    const auto kappa = KG.kappa;
    const auto mu = KG.mu;
    const auto lambda = kappa - 2 * mu /3;
    const IsotropicModuli<StressType>::LambdaMu LambdaMu = {.lambda = lambda, .mu = mu};
    return LambdaMu;
  }
  
  
  template <tfel::math::ScalarConcept StressType>
  TFEL_HOST_DEVICE IsotropicModuli<StressType>::KG LambdaMutoKG(
      const IsotropicModuli<StressType>::LambdaMu& LambdaMu) {
    const auto lambda = LambdaMu.lambda;
    const auto mu = LambdaMu.mu;
    const auto kappa = lambda + 2 * mu/3;
    const IsotropicModuli<StressType>::KG KG = {.kappa = kappa,
                                     .mu = mu};
    return KG;
  }
  
  template <tfel::math::ScalarConcept StressType>
  TFEL_HOST_DEVICE IsotropicModuli<StressType>::YoungNu LambdaMutoYoungNu(
      const IsotropicModuli<StressType>::LambdaMu& LambdaMu) {
    const auto lambda = LambdaMu.lambda;
    const auto mu = LambdaMu.mu;
    const auto nu = lambda / (2 * mu + 2 * lambda);
    const auto young = 2 * mu * (1 + nu);
    const IsotropicModuli<StressType>::YoungNu YoungNu = {.young = young, .nu = nu};
    return YoungNu;
  }
  
  
  template <tfel::math::ScalarConcept StressType>
  TFEL_HOST_DEVICE IsotropicModuli<StressType>::LambdaMu YoungNutoLambdaMu(
      const IsotropicModuli<StressType>::YoungNu& YoungNu) {
    const auto young = YoungNu.young;
    const auto nu = YoungNu.nu;
    const auto lambda = computeLambda<StressType>(young,nu);
    const auto mu = computeMu<StressType>(young,nu);
    const IsotropicModuli<StressType>::LambdaMu LambdaMu = {.lambda = lambda,
                                     .mu = mu};
    return LambdaMu;
  }
  

}  // end of namespace tfel::material

#endif /* LIB_TFEL_ISOTROPICMODULI_HXX */
