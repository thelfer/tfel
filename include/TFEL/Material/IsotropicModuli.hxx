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
#include "TFEL/Math/ST2toST2/ST2toST2Concept.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel::material {

  /*!
   * \brief This class is an abstract class which represents isotropic moduli.
   * 3 formats are possible: (Young,Nu), (Lambda,Mu) and (K,Mu).
   */
  template <tfel::math::ScalarConcept StressType>
  requires(
      tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                         StressType>()) struct IsotropicModuli;

  /*!
   * \brief This class is relative to the format (K,Nu).
   * It can be converted to the other formats (Lambda,Mu) and (Young,Nu).
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) struct KGModuli;

  /*!
   * \brief This class is relative to the format (Lambda,Mu).
   * It can be converted to the other formats (Young,Nu) and (K,Mu).
   */
  template <tfel::math::ScalarConcept StressType>
  requires(
      tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                         StressType>()) struct LambdaMuModuli;

  /*!
   * \brief This class is relative to the format (Young,Nu).
   * It can be converted to the other formats (Lambda,Mu) and (K,Mu).
   */
  template <tfel::math::ScalarConcept StressType>
  requires(
      tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                         StressType>()) struct YoungNuModuli;

  template <tfel::math::ScalarConcept StressType>
  requires(
      tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                         StressType>()) struct IsotropicModuli {
    
    virtual ~IsotropicModuli() = default;
    virtual YoungNuModuli<StressType> ToYoungNu() const = 0;
    virtual LambdaMuModuli<StressType> ToLambdaMu() const = 0;
    virtual KGModuli<StressType> ToKG() const = 0;
  };

  template <tfel::math::ScalarConcept StressType>
  requires(
      tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                         StressType>()) struct YoungNuModuli
      : public IsotropicModuli<StressType> {
    StressType young;
    types::real<StressType> nu;
    
    YoungNuModuli() = default;
    YoungNuModuli(const YoungNuModuli<StressType>& ) = default;
    YoungNuModuli(const StressType& Young, const types::real<StressType>& Nu)
        : IsotropicModuli<StressType>(), young(Young), nu(Nu) {}

    using IsotropicModuli<StressType>::operator=;

    YoungNuModuli<StressType> ToYoungNu() const override {
      const auto Young = this->young;
      const auto Nu = this->nu;
      return YoungNuModuli<StressType>(Young, Nu);
    }

    LambdaMuModuli<StressType> ToLambdaMu() const override {
      const auto lambda = computeLambda<StressType>(this->young, this->nu);
      const auto mu = computeMu<StressType>(this->young, this->nu);
      return LambdaMuModuli<StressType>(lambda, mu);
    }

    KGModuli<StressType> ToKG() const override {
      const auto Young = this->young;
      const auto Nu = this->nu;
      const auto Kappa = Young / (3 * (1 - 2 * Nu));
      const auto Mu = Young / (2 * (1 + Nu));
      return KGModuli<StressType>(Kappa, Mu);
    }

  };  // end of YoungNuoduli

  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) struct KGModuli
      : public IsotropicModuli<StressType> {
    StressType kappa;
    StressType mu;
    
    KGModuli() = default;
    KGModuli(const KGModuli<StressType>& ) = default;
    KGModuli(const StressType& Kappa, const StressType& Mu)
        : IsotropicModuli<StressType>(), kappa(Kappa), mu(Mu) {}

    using IsotropicModuli<StressType>::operator=;

    YoungNuModuli<StressType> ToYoungNu() const override {
      const auto Kappa = this->kappa;
      const auto Mu = this->mu;
      const auto Nu = (3 * Kappa - 2 * Mu) / (2 * Mu + 6 * Kappa);
      const auto Young = 2 * Mu * (1 + Nu);
      return YoungNuModuli<StressType>(Young, Nu);
    }

    LambdaMuModuli<StressType> ToLambdaMu() const override {
      const auto Kappa = this->kappa;
      const auto Mu = this->mu;
      const auto Lambda = Kappa - 2 * Mu / 3;
      return LambdaMuModuli<StressType>(Lambda, Mu);
    }

    KGModuli<StressType> ToKG() const override {
      const auto K = this->kappa;
      const auto G = this->mu;
      return KGModuli<StressType>(K, G);
    }
  };  // end of KGModuli

  template <tfel::math::ScalarConcept StressType>
  requires(
      tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                         StressType>()) struct LambdaMuModuli
      : public IsotropicModuli<StressType> {
    StressType lambda;
    StressType mu;
    
    LambdaMuModuli() = default;
    LambdaMuModuli(const LambdaMuModuli<StressType>& ) = default;
    LambdaMuModuli(const StressType& Lambda, const StressType& Mu)
        : IsotropicModuli<StressType>(), lambda(Lambda), mu(Mu) {}

    using IsotropicModuli<StressType>::operator=;

    YoungNuModuli<StressType> ToYoungNu() const override {
      const auto Lambda = this->lambda;
      const auto Mu = this->mu;
      const auto Nu = Lambda / (2 * Mu + 2 * Lambda);
      const auto Young = 2 * Mu * (1 + Nu);
      return YoungNuModuli<StressType>(Young, Nu);
    }

    LambdaMuModuli<StressType> ToLambdaMu() const override {
      const auto Lambda = this->lambda;
      const auto G = this->mu;
      return LambdaMuModuli<StressType>(Lambda, G);
    }

    KGModuli<StressType> ToKG() const override {
      const auto Lambda = this->lambda;
      const auto Mu = this->mu;
      const auto Kappa = Lambda + 2 * Mu / 3;
      return KGModuli<StressType>(Kappa, Mu);
    }
  };  // end of LambdaMuModuli

  /*!
   * \brief This function takes a IsotropicModuli and
   * returns the corresponding st2tost2
   * \tparam StressType: type of the moduli
   * \param IM : `IsotropicModuli`  \return a `st2tost2`
   */
  template <tfel::math::ScalarConcept StressType>
  requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                              StressType>()) TFEL_HOST_DEVICE
      constexpr tfel::math::
          st2tost2<3u, StressType> computeIsotropicStiffnessTensor(
              const IsotropicModuli<StressType>&);

  /*!
   * This function makes the projection of a `st2tost2`
   * on the subset of isotropic fourth-order tensors.
   * It uses the quadruple contraction of \f$C_i\f$ with \f$J\f$
   * and \f$K\f$. It returns the moduli \f$k\f$ and \f$mu\f$.
   * The implementation goes for dimension 3 only.
   * \tparam T: type of the `st2tost2`
   * \param A : `st2tost2`
   * \return a std::pair<T,T> (kappa,mu)
   */
  template <tfel::math::ScalarConcept T>
  TFEL_HOST_DEVICE constexpr std::pair<T, T> computeKappaMu(
      const tfel::math::st2tost2<3u, T>&);

  /*!
   * This function returns the isotropized moduli of a st2tost2
   * It uses computeKappaMu
   * The implementation goes for dimension 3 only.
   * \tparam T: type of the `st2tost2`
   * \param A : `st2tost2`
   * \return a KGModuli
   */
  template <tfel::math::ScalarConcept StressType>
  TFEL_HOST_DEVICE constexpr KGModuli<StressType> computeKGModuli(
      const tfel::math::st2tost2<3u, StressType>&);

  /*!
   * \brief This function computes the relative difference between a `st2tost2`
   * C1, relatively to a `st2tost2` C2. \tparam N: dimension \tparam T: type of
   * the tensors \param C1,C2 st2tost2 objects \return a real
   */
  template <unsigned short int N, tfel::math::ScalarConcept T>
  TFEL_HOST_DEVICE constexpr tfel::math::base_type<T> relative_error(
      const tfel::math::st2tost2<N, T>&, const tfel::math::st2tost2<N, T>&);

  /*!
   * \brief This function permits to know if a `st2tost2` is Isotropic
   * It first makes the projection on the subset of isotropic fourth-order
   * tensors using computeKappaMu and then compare the projected tensor with the
   * original \f$A_i\f$. The precision can be given by the user.
   * The implementation goes for dimension 3 only.
   * \tparam T: type of the `st2tost2`
   * \param Ai : `st2tost2` \return a boolean
   */
  template <tfel::math::ScalarConcept T>
  TFEL_HOST_DEVICE constexpr bool isIsotropic(
      const tfel::math::st2tost2<3u, T>&, const tfel::math::base_type<T>);

}  // end of namespace tfel::material

#include "TFEL/Material/IsotropicModuli.ixx"

#endif /* LIB_TFEL_ISOTROPICMODULI_HXX */
