/*!
 * \file   mtest/src/DianaFEASmallStrainBehaviour.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <limits>
#include <cstring>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/DianaFEA/DianaFEA.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/DianaFEA/DianaFEAComputeStiffnessTensor.hxx"

#include "MTest/Evolution.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/DianaFEASmallStrainBehaviour.hxx"
#include "MTest/UmatNormaliseTangentOperator.hxx"

namespace mtest {

  DianaFEASmallStrainBehaviour::DianaFEASmallStrainBehaviour(
      const Hypothesis h, const std::string& l, const std::string& b)
      : StandardBehaviourBase(h, l, b) {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    this->fct = elm.getDianaFEAExternalBehaviourFunction(l, b);
    if (this->stype != 0) {
      tfel::raise(
          "DianaFEASmallStrainBehaviour::DianaFEASmallStrainBehaviour: "
          "only isotropic behaviours are supported");
    }
    auto tmp = std::vector<std::string>{};
    if (this->requiresStiffnessTensor) {
      tmp.insert(tmp.end(), {"YoungModulus", "PoissonRatio"});
    }
    if (this->requiresThermalExpansionCoefficientTensor) {
      tmp.push_back("ThermalExpansion");
    }
    this->mpnames.insert(this->mpnames.begin(), tmp.begin(), tmp.end());
  }  // end of DianaFEASmallStrainBehaviour

  void DianaFEASmallStrainBehaviour::allocateWorkSpace(
      BehaviourWorkSpace& wk) const {
    const auto ndv = this->getGradientsSize();
    const auto nth = this->getThermodynamicForcesSize();
    const auto nstatev = this->getInternalStateVariablesSize();
    wk.D.resize(nth, nth);
    wk.k.resize(nth, ndv);
    wk.kt.resize(nth, ndv);
    wk.ivs.resize(nstatev);
    wk.nk.resize(nth, ndv);
    wk.ne.resize(ndv);
    wk.ns.resize(nth);
    wk.nivs.resize(nstatev);
    wk.mps.resize(this->mpnames.size());
    this->allocateCurrentState(wk.cs);
  }  // end of allocateWorkSpace

  tfel::math::tmatrix<3u, 3u, real>
  DianaFEASmallStrainBehaviour::getRotationMatrix(
      const tfel::math::vector<real>&,
      const tfel::math::tmatrix<3u, 3u, real>& r) const {
    return r;
  }  // end of getRotationMatrix

  StiffnessMatrixType
  DianaFEASmallStrainBehaviour::getDefaultStiffnessMatrixType() const {
    return StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }  // end of getDefaultStiffnessMatrixType

  void DianaFEASmallStrainBehaviour::getGradientsDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    std::fill(v.begin(), v.end(), real(0));
  }  // end of setGradientsDefaultInitialValue

  std::pair<bool, real> DianaFEASmallStrainBehaviour::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    wk.cs = s;
    return this->call_behaviour(wk.kt, wk.cs, wk, real(1), ktype, false);
  }  // end of computePredictionOperator

  std::pair<bool, real> DianaFEASmallStrainBehaviour::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    return this->call_behaviour(wk.k, s, wk, dt, ktype, true);
  }  // end of integrate

  std::pair<bool, real> DianaFEASmallStrainBehaviour::call_behaviour(
      tfel::math::matrix<real>& Kt,
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype,
      const bool b) const {
    using namespace tfel::math;
    using namespace dianafea;
    using tfel::material::getSpaceDimension;
    using tfel::math::vector;
    constexpr auto sqrt2 = Cste<real>::sqrt2;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "DianaFEASmallStrainBehaviour::call_behaviour: " + m);
    };
    throw_if((wk.D.getNbRows() != Kt.getNbRows()) ||
                 (wk.D.getNbCols() != Kt.getNbCols()),
             "the memory has not been allocated correctly");
    throw_if(s.iv0.size() != wk.ivs.size(),
             "the memory has not been allocated correctly (2)");
    std::fill(wk.D.begin(), wk.D.end(), 0.);
    // choosing the type of stiffness matrix
    StandardBehaviourBase::initializeTangentOperator(wk.D, ktype, b);
    // state variable initial values
    std::copy(s.iv0.begin(), s.iv0.end(), wk.ivs.begin());
    // strain
    stensor<3u, real> ue0(real(0));
    stensor<3u, real> ude(real(0));
    std::copy(s.e0.begin(), s.e0.end(), ue0.begin());
    for (decltype(s.e1.size()) i = 0; i != s.e1.size(); ++i) {
      ude(i) = s.e1(i) - s.e0(i);
    }
    std::copy(s.s0.begin(), s.s0.end(), s.s1.begin());
    // thermal strain
    for (decltype(s.e1.size()) i = 0; i != s.e1.size(); ++i) {
      ue0(i) -= s.e_th0(i);
      ude(i) -= s.e_th1(i) - s.e_th0(i);
    }
    // convert from umat conventions
    for (decltype(s.e1.size()) i = 3; i != this->getThermodynamicForcesSize();
         ++i) {
      s.s1(i) /= sqrt2;
      ue0(i) *= sqrt2;
      ude(i) *= sqrt2;
    }
    const auto nprops = static_cast<DianaFEAInt>(s.mprops1.size());
    const auto ntens =
        static_cast<DianaFEAInt>(this->getThermodynamicForcesSize());
    const auto nstatv = static_cast<DianaFEAInt>(wk.ivs.size());
    (this->fct)(&(s.s1(0)), &(wk.D(0, 0)),
                wk.ivs.empty() ? nullptr : &wk.ivs(0), &ntens, &nprops, &nstatv,
                &ue0(0), &ude(0), &dt,
                s.mprops1.empty() ? nullptr : &s.mprops1(0), &(s.esv0(0)),
                &(s.desv(0)));
    // tangent operator
    if (ktype != StiffnessMatrixType::NOSTIFFNESS) {
      UmatNormaliseTangentOperator::exe(
          &Kt(0, 0), wk.D, getSpaceDimension(this->getHypothesis()));
    }
    std::copy(wk.ivs.begin(), wk.ivs.end(), s.iv1.begin());
    // turning things in standard conventions
    for (decltype(s.e1.size()) i = 3; i != this->getThermodynamicForcesSize();
         ++i) {
      s.s1(i) *= sqrt2;
    }
    return {true, std::numeric_limits<DianaFEAReal>::max()};
  }  // end of integrate

  std::vector<std::string>
  DianaFEASmallStrainBehaviour::getOptionalMaterialProperties() const {
    if (this->stype != 0) {
      tfel::raise(
          "DianaFEASmallStrainBehaviour::"
          "setOptionalMaterialProperties: "
          "unsupported symmetry type");
    }
    return std::vector<std::string>(1u, "ThermalExpansion");
  }  // end of getOptionalMaterialProperties

  void DianaFEASmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager& mp, const EvolutionManager& evm) const {
    if (this->stype == 0) {
      Behaviour::setOptionalMaterialPropertyDefaultValue(
          mp, evm, "ThermalExpansion", 0.);
    } else {
      tfel::raise(
          "DianaFEASmallStrainBehaviour::"
          "setOptionalMaterialPropertiesDefaultValues: "
          "unsupported symmetry type");
    }
  }  // end of
  // DianaFEASmallStrainBehaviour::setOptionalMaterialPropertiesDefaultValues

  DianaFEASmallStrainBehaviour::~DianaFEASmallStrainBehaviour() = default;

}  // end of namespace mtest
