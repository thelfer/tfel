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
   * \brief This class is a virtual class which represents isotropic moduli.
   * 3 formats are possible: (Young,Nu), (Lambda,Mu) and (K,Mu).
   */
   
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
  
   /*!
   * \brief This class is relative to the format (Young,Nu).
   * It can be converted to the other formats (Lambda,Mu) and (K,Mu).
   */
   
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
  

  /*!
   * \brief This class is relative to the format (K,Nu).
   * It can be converted to the other formats (Lambda,Mu) and (Young,Nu).
   */
   
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


   /*!
   * \brief This class is relative to the format (Lambda,Mu).
   * It can be converted to the other formats (Young,Nu) and (K,Mu).
   */
   
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
  template <typename T>
  std::pair<T, T> computeKappaMu(const tfel::math::st2tost2<3u, T> &A) {
    const auto siz = tfel::math::StensorDimeToSize<3u>::value;
    auto J = tfel::math::st2tost2<3u, tfel::types::real<T>>::J();
    auto K = tfel::math::st2tost2<3u, tfel::types::real<T>>::K();
    const T kappai = tfel::math::quaddot(A, J) / 3;
    const T mui = tfel::math::quaddot(A, K) / (siz - 1) / 2;
    return {kappai, mui};
  }  // end of computeKappaMu

  /*!
   * \brief This function computes the relative difference between a `st2tost2` C1,
   * relatively to a `st2tost2` C2. The precision can be given by the user.
   * \tparam real: underlying type
   * \tparam N: dimension
   * \tparam T: type of the tensors
   * \param C1,C2 st2tost2 objects
   * \return a real
   */
  template <unsigned short int N, typename real, typename T>
  real relative_error(const tfel::math::st2tost2<N, T> &C1,
                      const tfel::math::st2tost2<N, T> &C2,
                      const real eps = std::numeric_limits<real>::epsilon()) {
    real val = tfel::math::norm(C1 - C2) / tfel::math::norm(C2);
    return val;
  }  // end of relative_error

  /*!
   * \brief This function permits to know if a `st2tost2` is Isotropic
   * It first makes the projection on the subset of isotropic fourth-order
   * tensors using computeKappaMu and then compare the projected tensor with the
   * original \f$A_i\f$. The precision can be given by the user.
   * The implementation goes for dimension 3 only.
   * \tparam T: type of the `st2tost2`
   * \tparam real: underlying type
   * \param Ai : `st2tost2` \return a boolean
   */
  template <typename T,typename real>
  bool isIsotropic(const tfel::math::st2tost2<3u, T> &Ai, const real eps=std::numeric_limits<real>::epsilon()) {
    const auto pair = computeKappaMu<T>(Ai);
    const auto kappai = std::get<0>(pair);
    const auto mui = std::get<1>(pair);
    auto J = tfel::math::st2tost2<3u, tfel::types::real<T>>::J();
    auto K = tfel::math::st2tost2<3u, tfel::types::real<T>>::K();
    const auto A_comp = 3 * kappai * J + 2 * mui * K;
    const auto val = relative_error<3u, types::real<T>, T>(Ai, A_comp,eps);
    if (val > eps) {
      return false;
    }
    return true;
  }  // end of isIsotropic

}  // end of namespace tfel::material

#endif /* LIB_TFEL_ISOTROPICMODULI_HXX */
