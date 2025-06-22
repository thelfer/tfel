/*!
 * \file   include/TFEL/Material/IsotropicModuli.hxx
 * \brief  This file defines various functions dealing with elastic
 * coefficients. \author Antoine MARTIN \date   15 May 2025 \copyright Copyright
 * (C) 2006-2025 CEA/DEN, EDF R&D. All rights reserved. This project is publicly
 * released under either the GNU GPL Licence with linking exception or the
 * CECILL-A licence. A copy of thoses licences are delivered with the sources of
 * TFEL. CEA or EDF may also distribute this project under specific licensing
 * conditions.
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
  requires(
      tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                         StressType>()) struct IsotropicModuli {
    IsotropicModuli(){};
    virtual ~IsotropicModuli(){};
    virtual std::pair<StressType, types::real<StressType>> ToYoungNu()
        const& = 0;
    virtual std::pair<StressType, StressType> ToLambdaMu() const& = 0;
    virtual std::pair<StressType, StressType> ToKG() const& = 0;
  };

  template <tfel::math::ScalarConcept StressType>
  requires(
      tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                         StressType>()) struct YoungNuModuli
      : IsotropicModuli<StressType> {
    StressType young;
    types::real<StressType> nu;
    YoungNuModuli(const StressType& Young, const types::real<StressType>& Nu)
        : IsotropicModuli<StressType>(), young(Young), nu(Nu){};
    virtual ~YoungNuModuli(){};

    std::pair<StressType, types::real<StressType>> ToYoungNu() const& override {
      return {this->young, this->nu};
    };

    std::pair<StressType, StressType> ToLambdaMu() const& override {
      const auto lambda = computeLambda<StressType>(this->young, this->nu);
      const auto mu = computeMu<StressType>(this->young, this->nu);
      return {lambda, mu};
    };

    std::pair<StressType, StressType> ToKG() const& override {
      const auto Young = this->young;
      const auto Nu = this->nu;
      const auto Kappa = Young / 3. / (1 - 2 * Nu);
      const auto Mu = Young / 2. / (1 + Nu);
      return {Kappa, Mu};
    };
  };  // end of YoungNuoduli

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) struct KGModuli
      : IsotropicModuli<StressType> {
    StressType kappa;
    StressType mu;
    KGModuli(const StressType& Kappa, const StressType& Mu)
        : IsotropicModuli<StressType>(), kappa(Kappa), mu(Mu){};
    virtual ~KGModuli(){};

    std::pair<StressType, types::real<StressType>> ToYoungNu() const& override {
      const auto Kappa = this->kappa;
      const auto Mu = this->mu;
      const auto Nu = (3 * Kappa - 2 * Mu) / (2 * Mu + 6 * Kappa);
      const auto Young = 2 * Mu * (1 + Nu);
      return {Young, Nu};
    };

    std::pair<StressType, StressType> ToLambdaMu() const& override {
      const auto Kappa = this->kappa;
      const auto Mu = this->mu;
      const auto Lambda = Kappa - 2 * Mu / 3;
      return {Lambda, Mu};
    };

    std::pair<StressType, StressType> ToKG() const& override {
      return {this->kappa, this->mu};
    };
  };  // end of KGModuli

  template <tfel::math::ScalarConcept StressType>
  requires(
      tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                         StressType>()) struct LambdaMuModuli
      : IsotropicModuli<StressType> {
    StressType lambda;
    StressType mu;
    LambdaMuModuli(const StressType& Lambda, const StressType& Mu)
        : IsotropicModuli<StressType>(), lambda(Lambda), mu(Mu){};
    virtual ~LambdaMuModuli(){};
    std::pair<StressType, types::real<StressType>> ToYoungNu() const& override {
      const auto Lambda = this->lambda;
      const auto Mu = this->mu;
      const auto Nu = Lambda / (2 * Mu + 2 * Lambda);
      const auto Young = 2 * Mu * (1 + Nu);
      return {Young, Nu};
    };

    std::pair<StressType, StressType> ToLambdaMu() const& override {
      return {this->lambda, this->mu};
    };

    std::pair<StressType, StressType> ToKG() const& override {
      const auto Lambda = this->lambda;
      const auto Mu = this->mu;
      const auto Kappa = Lambda + 2 * Mu / 3;
      return {Kappa, Mu};
    };
  };  // end of LambdaMuModuli

}  // end of namespace tfel::material

#endif /* LIB_TFEL_ISOTROPICMODULI_HXX */
