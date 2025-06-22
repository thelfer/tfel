/*!
 * \file  mtest/src/AsterCohesiveZoneModel.cxx
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
#include <ostream>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/Aster/Aster.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/AsterCohesiveZoneModel.hxx"

namespace mtest {

  AsterCohesiveZoneModel::AsterCohesiveZoneModel(const Hypothesis h,
                                                 const std::string& l,
                                                 const std::string& b)
      : StandardBehaviourBase(h, l, b) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "AsterCohesiveZoneModel::AsterCohesiveZoneModel: " + m);
    };
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    throw_if(elm.getInterface(l, b) != "Aster",
             "invalid interface '" + elm.getInterface(l, b) + "'");
    this->fct = elm.getAsterFunction(l, b);
    this->emsg = elm.getAsterIntegrationErrorMessageFunction(l, b);
    const auto& nh = ModellingHypothesis::toString(h);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l, b, nh);
    throw_if(this->btype != 3u, "unsupported hypothesis");
    throw_if(this->stype != 0, "unsupported symmetry type");
  }

  tfel::math::tmatrix<3u, 3u, real> AsterCohesiveZoneModel::getRotationMatrix(
      const tfel::math::vector<real>&,
      const tfel::math::tmatrix<3u, 3u, real>&) const {
    tfel::raise("AsterCohesiveZoneModel::getRotationMatrix: invalid call");
  }  // end of AsterCohesiveZoneModel::getRotationMatrix

  void AsterCohesiveZoneModel::allocateWorkSpace(BehaviourWorkSpace& wk) const {
    const auto ndv = this->getGradientsSize();
    const auto nth = this->getThermodynamicForcesSize();
    const auto nstatev = this->getInternalStateVariablesSize();
    wk.kt.resize(nth, ndv);
    wk.k.resize(nth, ndv);
    wk.mps.resize(this->mpnames.size() == 0 ? 1u : this->mpnames.size(),
                  real(0));
    wk.ivs.resize(nstatev == 0 ? 1u : nstatev, real(0));
    wk.nk.resize(nth, ndv);
    wk.ne.resize(ndv);
    wk.ns.resize(nth);
    wk.nivs.resize(nstatev);
    this->allocateCurrentState(wk.cs);
  }

  void AsterCohesiveZoneModel::getGradientsDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    std::fill(v.begin(), v.end(), real(0));
  }  // end of AsterCohesiveZoneModel::setGradientsDefaultInitialValue

  StiffnessMatrixType AsterCohesiveZoneModel::getDefaultStiffnessMatrixType()
      const {
    return StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }

  std::pair<bool, real> AsterCohesiveZoneModel::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    if (ktype == StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES) {
      return {false, real(-1)};
    }
    wk.cs = s;
    return this->call_behaviour(wk.kt, wk.cs, wk, real(1), ktype, false);
  }  // end of AsterCohesiveZoneModel::computePredictionOperator

  std::pair<bool, real> AsterCohesiveZoneModel::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    return this->call_behaviour(wk.k, s, wk, dt, ktype, true);
  }  // end of AsterCohesiveZoneModel::integrate

  std::pair<bool, real> AsterCohesiveZoneModel::call_behaviour(
      tfel::math::matrix<real>& Kt,
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype,
      const bool b) const {
    using namespace tfel::math;
    using namespace aster;
    using tfel::math::vector;
    AsterInt ntens;
    AsterInt nprops =
        s.mprops1.size() == 0 ? 1 : static_cast<AsterInt>(s.mprops1.size());
    AsterInt nstatv;
    AsterInt nummod;
    const auto h = this->getHypothesis();
    if (h == ModellingHypothesis::AXISYMMETRICAL) {
      ntens = 4;
      nummod = 4u;
    } else if (h == ModellingHypothesis::PLANESTRESS) {
      ntens = 4;
      nummod = 5u;
    } else if (h == ModellingHypothesis::PLANESTRAIN) {
      ntens = 4;
      nummod = 6u;
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      ntens = 6;
      nummod = 3u;
    } else {
      tfel::raise(
          "AsterCohesiveZoneModel::call_behaviour: "
          "unsupported hypothesis");
    }
    std::fill(Kt.begin(), Kt.end(), 0.);
    // choosing the type of stiffness matrix
    if (b) {
      if (ktype == StiffnessMatrixType::NOSTIFFNESS) {
        // do nothing
      } else if (ktype == StiffnessMatrixType::ELASTIC) {
        Kt(0, 0) = real(1);
      } else if (ktype == StiffnessMatrixType::SECANTOPERATOR) {
        Kt(0, 0) = real(2);
      } else if (ktype == StiffnessMatrixType::TANGENTOPERATOR) {
        Kt(0, 0) = real(3);
      } else if (ktype == StiffnessMatrixType::CONSISTENTTANGENTOPERATOR) {
        Kt(0, 0) = real(4);
      } else {
        tfel::raise(
            "AsterCohesiveZoneModel::call_behaviour: "
            "invalid or unspecified stiffness matrix type");
      }
    } else {
      if (ktype == StiffnessMatrixType::ELASTIC) {
        Kt(0, 0) = real(-1);
      } else if (ktype == StiffnessMatrixType::SECANTOPERATOR) {
        Kt(0, 0) = real(-2);
      } else if (ktype == StiffnessMatrixType::TANGENTOPERATOR) {
        Kt(0, 0) = real(-3);
      } else {
        tfel::raise(
            "AsterCohesiveZoneModel::call_behaviour : "
            "invalid or unspecified stiffness matrix type");
      }
    }
    // using a local copy of material properties to handle the
    // case where s.mprops1 is empty
    std::copy(s.mprops1.begin(), s.mprops1.end(), wk.mps.begin());
    if (s.mprops1.empty()) {
      wk.mps[0] = real(0);
    }
    // using a local copy of internal state variables to handle the
    // case where s.iv0 is empty
    std::copy(s.iv0.begin(), s.iv0.end(), wk.ivs.begin());
    if (s.iv0.empty()) {
      wk.ivs[0] = real(0);
    }
    nstatv = static_cast<AsterInt>(wk.ivs.size());
    // rotation matrix
    tmatrix<3u, 3u, real> drot = transpose(s.r);
    tvector<3u, real> ue0(real(0));
    tvector<3u, real> ude(real(0));
    std::copy(s.e0.begin(), s.e0.end(), ue0.begin());
    tmatrix<3u, 3u, real>::size_type i;
    for (i = 0; i != s.e1.size(); ++i) {
      ude(i) = s.e1(i) - s.e0(i);
    }
    std::copy(s.s0.begin(), s.s0.end(), s.s1.begin());
    AsterReal ndt(1.);
    (this->fct)(&(s.s1(0)), &(wk.ivs(0)), &Kt(0, 0), &ue0(0), &ude(0), &dt,
                &(s.esv0(0)), &(s.desv(0)), &(s.esv0(0)) + 1, &(s.desv(0)) + 1,
                &ntens, &nstatv, &(wk.mps(0)), &nprops, &drot(0, 0), &ndt,
                &nummod);
    if (ndt < 1.) {
      if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
        if (this->emsg != nullptr) {
          mfront::getLogStream() << this->emsg() << std::endl;
        }
      }
      return {false, ndt};
    }
    if (b) {
      if (!s.iv0.empty()) {
        std::copy_n(wk.ivs.begin(), s.iv1.size(), s.iv1.begin());
      }
    }
    return {true, ndt};
  }

  std::vector<std::string>
  AsterCohesiveZoneModel::getOptionalMaterialProperties() const {
    return {};
  }  // end of AsterCohesiveZoneModel::getOptionalMaterialProperties

  void AsterCohesiveZoneModel::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager&, const EvolutionManager&) const {}  // end of
  // AsterCohesiveZoneModel::setOptionalMaterialPropertiesDefaultValues

  AsterCohesiveZoneModel::~AsterCohesiveZoneModel() = default;

}  // end of namespace mtest
