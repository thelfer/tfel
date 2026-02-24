/*!
 * \file   mtest/src/AsterLogarithmicStrainBehaviourWrapper.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/01/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <variant>
#include <sstream>
#include "TFEL/Material/LogarithmicStrainHandler.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/AsterSmallStrainBehaviour.hxx"
#include "MTest/AsterLogarithmicStrainBehaviourWrapper.hxx"

namespace mtest {

  /*!
   * \brief structure used for the computation
   * of the prediction operator
   */
  template <unsigned short N>
  struct AsterComputePredictionLogarithmicStrainHandler
      : public tfel::material::LogarithmicStrainHandler<N, real> {
    /*!
     * \param[in] s: state to be modified
     */
    AsterComputePredictionLogarithmicStrainHandler(CurrentState& cs)
        : tfel::material::LogarithmicStrainHandler<N, real>(
              tfel::material::LogarithmicStrainHandlerBase::EULERIAN,
              tfel::math::tensor<N, real>{cs.e0.data()}),
          state(cs) {
      this->s0 = tfel::math::stensor<N, real>{this->state.s0.data()};
      const auto e0 = this->getHenckyLogarithmicStrain();
      this->T0 = this->convertFromCauchyStress(this->s0);
      this->state.e0.resize(e0.size());
      this->state.e1.resize(e0.size());
      std::copy(e0.begin(), e0.end(), this->state.e0.begin());
      std::copy(e0.begin(), e0.end(), this->state.e1.begin());
      std::copy(this->T0.begin(), this->T0.end(), this->state.s0.begin());
      std::copy(this->T0.begin(), this->T0.end(), this->state.s1.begin());
    }
    //
    void updateConsistentTangentOperator(real* const K) const {
      constexpr auto dT_delog = tfel::material::
          FiniteStrainBehaviourTangentOperatorBase::SPATIAL_MODULI;
      constexpr auto dtau_dF =
          tfel::material::FiniteStrainBehaviourTangentOperatorBase::DTAU_DF;
      constexpr auto dsig_dF =
          tfel::material::FiniteStrainBehaviourTangentOperatorBase::DSIG_DF;
      const auto Kl = tfel::math::st2tost2<N, real>{K};
      const auto Ks = this->convertToSpatialTangentModuli(Kl, this->T0);
      const auto Kt = tfel::material::convert<dtau_dF, dT_delog, N, real>(
          Ks, this->F, this->F, this->s0);
      tfel::math::map<tfel::math::t2tost2<N, real>>(K) =
          tfel::material::convert<dsig_dF, dtau_dF, N, real>(Kt, this->F,
                                                             this->F, this->s0);
    }  // end of updateConsistentTangentOperator

   private:
    CurrentState& state;
    tfel::math::stensor<N, real> s0;
    tfel::math::stensor<N, real> T0;
  };

  //! \brief structure using during the behaviour integration
  template <unsigned short N>
  struct AsterIntegrateLogarithmicStrainHandler {
    /*!
     * \param[in] s: state to be modified
     */
    AsterIntegrateLogarithmicStrainHandler(CurrentState& cs)
        : state(cs),
          h0(tfel::material::LogarithmicStrainHandlerBase::EULERIAN,
             tfel::math::tensor<N, real>{cs.e0.data()}),
          h1(tfel::material::LogarithmicStrainHandlerBase::EULERIAN,
             tfel::math::tensor<N, real>{cs.e1.data()}) {
      this->s0 = tfel::math::stensor<N, real>{this->state.s0.data()};
      const auto e0 = h0.getHenckyLogarithmicStrain();
      const auto e1 = h1.getHenckyLogarithmicStrain();
      this->T0 = h0.convertFromCauchyStress(this->s0);
      this->state.e0.resize(e0.size());
      this->state.e1.resize(e1.size());
      std::copy(e0.begin(), e0.end(), this->state.e0.begin());
      std::copy(e1.begin(), e1.end(), this->state.e1.begin());
      std::copy(this->T0.begin(), this->T0.end(), this->state.s0.begin());
      std::copy(this->T0.begin(), this->T0.end(), this->state.s1.begin());
    }
    //
    void postProcess(bool updateStress) {
      const auto& F0 = h0.getDeformationGradient();
      const auto& F1 = h1.getDeformationGradient();
      this->state.e0.resize(F0.size());
      this->state.e1.resize(F1.size());
      std::copy(F0.begin(), F0.end(), this->state.e0.begin());
      std::copy(F1.begin(), F1.end(), this->state.e1.begin());
      std::copy(this->s0.begin(), this->s0.end(), this->state.s0.begin());
      if (updateStress) {
        this->T1 = tfel::math::stensor<N, real>(this->state.s1.data());
        this->s1 = h1.convertToCauchyStress(T1);
        std::copy(this->s1.begin(), this->s1.end(), this->state.s1.begin());
      } else {
        std::copy(this->s0.begin(), this->s0.end(), this->state.s1.begin());
      }
    }
    //
    void updateConsistentTangentOperator(real* const K) const {
      constexpr auto dT_delog = tfel::material::
          FiniteStrainBehaviourTangentOperatorBase::SPATIAL_MODULI;
      constexpr auto dtau_dF =
          tfel::material::FiniteStrainBehaviourTangentOperatorBase::DTAU_DF;
      constexpr auto dsig_dF =
          tfel::material::FiniteStrainBehaviourTangentOperatorBase::DSIG_DF;
      const auto& F0 = h0.getDeformationGradient();
      const auto& F1 = h1.getDeformationGradient();
      const auto Kl = tfel::math::st2tost2<N, real>{K};
      const auto Ks = h1.convertToSpatialTangentModuli(Kl, this->T1);
      const auto Kt = tfel::material::convert<dtau_dF, dT_delog, N, real>(
          Ks, F0, F1, this->s1);
      tfel::math::map<tfel::math::t2tost2<N, real>>(K) =
          tfel::material::convert<dsig_dF, dtau_dF, N, real>(Kt, F0, F1,
                                                             this->s1);
    }  // end of updateConsistentTangentOperator

   private:
    CurrentState& state;
    tfel::material::LogarithmicStrainHandler<N, real> h0;
    tfel::material::LogarithmicStrainHandler<N, real> h1;
    tfel::math::stensor<N, real> s0;
    tfel::math::stensor<N, real> T0;
    tfel::math::stensor<N, real> s1;
    tfel::math::stensor<N, real> T1;
  };

  AsterLogarithmicStrainBehaviourWrapper::
      AsterLogarithmicStrainBehaviourWrapper(
          const std::shared_ptr<AsterSmallStrainBehaviour>& wb)
      : BehaviourWrapperBase(wb) {
    if (this->b->getHypothesis() == ModellingHypothesis::PLANESTRESS) {
      tfel::raise(
          "AsterLogarithmicStrainBehaviourWrapper: unsupported plane stress "
          "hypothesis");
    }
  }  // end of AsterLogarithmicStrainBehaviourWrapper

  AsterLogarithmicStrainBehaviourWrapper::Hypothesis
  AsterLogarithmicStrainBehaviourWrapper::getHypothesis() const {
    return this->b->getHypothesis();
  }  // end of getHypothesis

  tfel::material::MechanicalBehaviourBase::BehaviourType
  AsterLogarithmicStrainBehaviourWrapper::getBehaviourType() const {
    using tfel::material::MechanicalBehaviourBase;
    return MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR;
  }  // end of getBehaviourType

  tfel::material::MechanicalBehaviourBase::Kinematic
  AsterLogarithmicStrainBehaviourWrapper::getBehaviourKinematic() const {
    using tfel::material::MechanicalBehaviourBase;
    return MechanicalBehaviourBase::FINITESTRAINKINEMATIC_F_CAUCHY;
  }  // end of getBehaviourKinematic

  unsigned short AsterLogarithmicStrainBehaviourWrapper::getGradientsSize()
      const {
    return tfel::material::getTensorSize(this->getHypothesis());
  }  // end of getGradientsSize

  void AsterLogarithmicStrainBehaviourWrapper::getGradientsDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    std::fill(v.begin(), v.end(), real(0));
    v[0] = v[1] = v[2] = real(1);
  }  // end of getGradientsDefaultInitialValues

  unsigned short
  AsterLogarithmicStrainBehaviourWrapper::getThermodynamicForcesSize() const {
    return this->b->getThermodynamicForcesSize();
  }  // end of getThermodynamicForcesSize

  std::vector<std::string>
  AsterLogarithmicStrainBehaviourWrapper::getStensorComponentsSuffixes() const {
    return this->b->getStensorComponentsSuffixes();
  }  // end of getStensorComponentsSuffixes

  std::vector<std::string>
  AsterLogarithmicStrainBehaviourWrapper::getVectorComponentsSuffixes() const {
    return this->b->getVectorComponentsSuffixes();
  }  // end of getVectorComponentsSuffixes

  std::vector<std::string>
  AsterLogarithmicStrainBehaviourWrapper::getTensorComponentsSuffixes() const {
    return this->b->getTensorComponentsSuffixes();
  }  // end of getTensorComponentsSuffixes

  std::vector<std::string>
  AsterLogarithmicStrainBehaviourWrapper::getGradientsComponents() const {
    auto c = std::vector<std::string>{};
    for (const auto& s : this->getTensorComponentsSuffixes()) {
      c.push_back("F" + s);
    }
    return c;
  }  // end of getGradientsComponents

  std::vector<std::string>
  AsterLogarithmicStrainBehaviourWrapper::getThermodynamicForcesComponents()
      const {
    return this->b->getThermodynamicForcesComponents();
  }  // end of getThermodynamicForcesComponents

  unsigned short
  AsterLogarithmicStrainBehaviourWrapper::getGradientComponentPosition(
      const std::string& cname) const {
    const auto c = this->getGradientsComponents();
    auto p = std::find(c.begin(), c.end(), cname);
    if (p == c.end()) {
      std::ostringstream msg;
      msg << "AsterLogarithmicStrainBehaviourWrapper"
          << "::getGradientComponentPosition: "
          << "component '" << cname
          << "' is not valid. Valid components are:\n";
      std::copy(c.begin(), c.end(),
                std::ostream_iterator<std::string>(msg, " "));
      tfel::raise(msg.str());
    }
    return static_cast<unsigned short>(p - c.begin());
  }  // end of getGradientComponentPosition

  unsigned short AsterLogarithmicStrainBehaviourWrapper::
      getThermodynamicForceComponentPosition(const std::string& c) const {
    return this->b->getThermodynamicForceComponentPosition(c);
  }  // end of getThermodynamicForceComponentPosition

  size_t AsterLogarithmicStrainBehaviourWrapper::getTangentOperatorArraySize()
      const {
    return this->getThermodynamicForcesSize() * this->getGradientsSize();
  }  // end of getTangentOperatorArraySize

  std::vector<std::pair<std::string, std::string>>
  AsterLogarithmicStrainBehaviourWrapper::getTangentOperatorBlocks() const {
    return {{"Stress", "DeformationGradient"}};
  }  // end of getTangentOperatorBlocks

  std::vector<std::string>
  AsterLogarithmicStrainBehaviourWrapper::getInternalStateVariablesNames()
      const {
    return this->b->getInternalStateVariablesNames();
  }  // end of getInternalStateVariablesNames

  std::vector<std::string>
  AsterLogarithmicStrainBehaviourWrapper::expandInternalStateVariablesNames()
      const {
    return this->b->expandInternalStateVariablesNames();
  }  // end of expandInternalStateVariablesNames

  size_t AsterLogarithmicStrainBehaviourWrapper::getInternalStateVariablesSize()
      const {
    return this->b->getInternalStateVariablesSize();
  }  // end of getInternalStateVariablesSize

  std::vector<std::string> AsterLogarithmicStrainBehaviourWrapper::
      getInternalStateVariablesDescriptions() const {
    return this->b->getInternalStateVariablesDescriptions();
  }  // end of getInternalStateVariablesDescriptions

  unsigned short
  AsterLogarithmicStrainBehaviourWrapper::getInternalStateVariableType(
      const std::string& n) const {
    return this->b->getInternalStateVariableType(n);
  }  // end of getInternalStateVariableType

  unsigned short
  AsterLogarithmicStrainBehaviourWrapper::getInternalStateVariablePosition(
      const std::string& n) const {
    return this->b->getInternalStateVariablePosition(n);
  }  // end of getInternalStateVariablePosition

  std::vector<std::string>
  AsterLogarithmicStrainBehaviourWrapper::getExternalStateVariablesNames()
      const {
    return this->b->getExternalStateVariablesNames();
  }  // end of getExternalStateVariablesNames

  std::vector<std::string>
  AsterLogarithmicStrainBehaviourWrapper::expandExternalStateVariablesNames()
      const {
    return this->b->expandExternalStateVariablesNames();
  }  // end of expandExternalStateVariablesNames

  size_t AsterLogarithmicStrainBehaviourWrapper::getExternalStateVariablesSize()
      const {
    return this->b->getExternalStateVariablesSize();
  }  // end of getExternalStateVariablesSize

  unsigned short
  AsterLogarithmicStrainBehaviourWrapper::getExternalStateVariableType(
      const std::string& n) const {
    return this->b->getExternalStateVariableType(n);
  }  // end of getExternalStateVariableType

  unsigned short
  AsterLogarithmicStrainBehaviourWrapper::getExternalStateVariablePosition(
      const std::string& n) const {
    return this->b->getExternalStateVariablePosition(n);
  }  // end of getExternalStateVariablePosition

  void AsterLogarithmicStrainBehaviourWrapper::allocateCurrentState(
      CurrentState& s) const {
    BehaviourWrapperBase::allocateCurrentState(s);
    const auto ndv = this->getGradientsSize();
    s.e0.resize(ndv, 0.);
    s.e1.resize(ndv, 0.);
    s.e_th0.resize(ndv, 0.);
    s.e_th1.resize(ndv, 0.);
  }  // end of allocateCurrentState

  void AsterLogarithmicStrainBehaviourWrapper::allocateWorkSpace(
      BehaviourWorkSpace& wk) const {
    const auto ndv = this->getGradientsSize();
    const auto nth = this->getThermodynamicForcesSize();
    const auto nstatev = this->getInternalStateVariablesSize();
    const auto mpnames = this->getMaterialPropertiesNames();
    wk.kt.resize(nth, ndv);
    wk.k.resize(nth, ndv);
    wk.D.resize(nth, ndv);
    wk.mps.resize(mpnames.size() == 0 ? 1u : mpnames.size(), real(0));
    wk.ivs.resize(nstatev == 0 ? 1u : nstatev, real(0));
    wk.nk.resize(nth, ndv);
    wk.ne.resize(ndv);
    wk.ns.resize(nth);
    wk.nivs.resize(nstatev);
    this->allocateCurrentState(wk.cs);
  }  // end of allocateWorkSpace

  tfel::math::tmatrix<3u, 3u, real>
  AsterLogarithmicStrainBehaviourWrapper::getRotationMatrix(
      const tfel::math::vector<real>& mp,
      const tfel::math::tmatrix<3u, 3u, real>& r) const {
    return this->b->getRotationMatrix(mp, r);
  }  // end of getRotationMatrix

  bool AsterLogarithmicStrainBehaviourWrapper::doPackagingStep(
      CurrentState& s, BehaviourWorkSpace& wk) const {
    return this->b->doPackagingStep(s, wk);
  }  // end of doPackagingStep

  template <unsigned short N>
  using Handler = tfel::material::LogarithmicStrainHandler<N, real>;

  std::pair<bool, real>
  AsterLogarithmicStrainBehaviourWrapper::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    const auto N = tfel::material::getSpaceDimension(this->getHypothesis());
    //
    std::variant<std::monostate,
                 AsterComputePredictionLogarithmicStrainHandler<1>,
                 AsterComputePredictionLogarithmicStrainHandler<2>,
                 AsterComputePredictionLogarithmicStrainHandler<3>>
        handler_variant;
    // copy of the current state
    auto cs = s;
    if (N == 1) {
      handler_variant
          .emplace<AsterComputePredictionLogarithmicStrainHandler<1u>>(cs);
    } else if (N == 2) {
      handler_variant
          .emplace<AsterComputePredictionLogarithmicStrainHandler<2u>>(cs);
    } else if (N == 3) {
      handler_variant
          .emplace<AsterComputePredictionLogarithmicStrainHandler<3u>>(cs);
    } else {
      tfel::raise(
          "AsterLogarithmicStrainBehaviourWrapper::"
          "computePredictionOperator: unsupported space dimension");
    }
    const auto r = this->b->computePredictionOperator(wk, s, ktype);
    if (r.first) {
      if (N == 1) {
        const auto& h =
            std::get<AsterComputePredictionLogarithmicStrainHandler<1u>>(
                handler_variant);
        h.updateConsistentTangentOperator(wk.kt.data());
      } else if (N == 2) {
        const auto& h =
            std::get<AsterComputePredictionLogarithmicStrainHandler<2u>>(
                handler_variant);
        h.updateConsistentTangentOperator(wk.kt.data());
      } else if (N == 3) {
        const auto& h =
            std::get<AsterComputePredictionLogarithmicStrainHandler<3u>>(
                handler_variant);
        h.updateConsistentTangentOperator(wk.kt.data());
      } else {
        tfel::raise(
            "AsterLogarithmicStrainBehaviourWrapper::"
            "computePredictionOperator: unsupported space dimension");
      }
    }
    return r;
  }  // end of computePredictionOperator

  template <unsigned N>
  std::pair<bool, real> AsterLogarithmicStrainBehaviourWrapper::do_integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    AsterIntegrateLogarithmicStrainHandler<N> h(s);
    const auto r = this->b->integrate(s, wk, dt, ktype);
    h.postProcess(r.first);
    if ((r.first) && (ktype != StiffnessMatrixType::NOSTIFFNESS)) {
      h.updateConsistentTangentOperator(wk.k.data());
    }
    return r;
  }

  std::pair<bool, real> AsterLogarithmicStrainBehaviourWrapper::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    const auto N = tfel::material::getSpaceDimension(this->getHypothesis());
    auto r = std::pair<bool, real>{};
    if (N == 1) {
      r = this->do_integrate<1u>(s, wk, dt, ktype);
    } else if (N == 2) {
      r = this->do_integrate<2u>(s, wk, dt, ktype);
    } else if (N == 3) {
      r = this->do_integrate<3u>(s, wk, dt, ktype);
    } else {
      tfel::raise(
          "AsterLogarithmicStrainBehaviourWrapper::"
          "integrate: unsupported space dimension");
    }
    return r;
  }  // end of integrate

  AsterLogarithmicStrainBehaviourWrapper::
      ~AsterLogarithmicStrainBehaviourWrapper() = default;

}  // end of namespace mtest
