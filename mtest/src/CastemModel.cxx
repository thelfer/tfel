/*!
 * \file  mtest/src/CastemModel.cxx
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
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/Castem/Castem.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/CastemModel.hxx"

namespace mtest {

  CastemModel::CastemModel(const Hypothesis h,
                           const std::string& l,
                           const std::string& b)
      : StandardBehaviourBase(h, l, b) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "CastemModel::CastemModel: " + m);
    };
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    const auto i = elm.getInterface(l, b);
    throw_if((i != "Castem") && (i != "Castem21"),
             "invalid interface '" + elm.getInterface(l, b) + "'");
    const auto& nh = ModellingHypothesis::toString(h);
    this->fct = elm.getCastemExternalBehaviourFunction(l, b);
    throw_if(this->btype != 0u, "invalid behaviour type");
    this->mpnames.clear();
    const auto mps = elm.getUMATMaterialPropertiesNames(l, b, nh);
    this->mpnames.insert(this->mpnames.end(), mps.begin(), mps.end());
  }

  tfel::math::tmatrix<3u, 3u, real> CastemModel::getRotationMatrix(
      const tfel::math::vector<real>&,
      const tfel::math::tmatrix<3u, 3u, real>&) const {
    tfel::raise("CastemModel::getRotationMatrix: invalid call");
  }  // end of CastemModel::getRotationMatrix

  void CastemModel::allocateWorkSpace(BehaviourWorkSpace& wk) const {
#pragma "something needs to be done here"
    const auto ndv = this->getGradientsSize();
    const auto nth = this->getThermodynamicForcesSize();
    const auto nstatev = this->getInternalStateVariablesSize();
    wk.D.resize(nth, ndv);
    wk.kt.resize(nth, ndv);
    wk.k.resize(nth, ndv);
    wk.ivs.resize(nstatev == 0 ? 1u : nstatev, real(0));
    wk.nk.resize(nth, ndv);
    wk.ne.resize(ndv);
    wk.ns.resize(nth);
    wk.nivs.resize(nstatev);
    this->allocateCurrentState(wk.cs);
  }

  void CastemModel::getGradientsDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    if (v.size() != 0) {
      tfel::raise(
          "CastemModel::getGradientsDefaultInitialValues: "
          "the gradient vector shall be empty");
    }
  }  // end of CastemModel::setGradientsDefaultInitialValue

  StiffnessMatrixType CastemModel::getDefaultStiffnessMatrixType() const {
    return StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES;
  }

  std::pair<bool, real> CastemModel::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    tfel::raise(
        "CastemModel::computePredictionOperator: "
        "computation of the tangent operator "
        "is not supported");
  }  // end of CastemModel::computePredictionOperator

  std::pair<bool, real> CastemModel::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    using namespace tfel::math;
    using namespace castem;
    using tfel::math::vector;
    CastemInt ntens;
    CastemInt ndi;
    CastemInt nprops = static_cast<CastemInt>(s.mprops1.size());
    CastemInt nstatv;
    const auto h = this->getHypothesis();
    if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
      ndi = 14;
      ntens = 3;
    } else if (h == ModellingHypothesis::PLANESTRESS) {
      ndi = -2;
      ntens = 2;
    } else if (h == ModellingHypothesis::PLANESTRAIN) {
      ndi = -1;
      ntens = 2;
    } else if (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) {
      ndi = -3;
      ntens = 2;
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      ndi = 2;
      ntens = 3;
    } else {
      tfel::raise("CastemModel::integrate: unsupported hypothesis");
    }
    //     tfel::raise_if((wk.D.getNbRows() != wk.k.getNbRows()) ||
    //                        (wk.D.getNbCols() != wk.k.getNbCols()),
    //                    "CastemModel::integrate: "
    //                    "the memory has not been allocated correctly");
    tfel::raise_if(((s.iv0.size() == 0) && (wk.ivs.size() != 1u)) ||
                       ((s.iv0.size() != 0) && (s.iv0.size() != wk.ivs.size())),
                   "CastemModel::integrate: "
                   "the memory has not been allocated correctly");
    //    std::fill(wk.D.begin(), wk.D.end(), 0.);
    if (s.iv0.size() != 0) {
      std::copy(s.iv0.begin(), s.iv0.end(), wk.ivs.begin());
    }
    nstatv = static_cast<CastemInt>(wk.ivs.size());
    auto ndt = std::numeric_limits<CastemReal>::max();
    auto drot = tmatrix<3u, 3u, real>{1, 0, 0,  //
                                      0, 1, 0,  //
                                      0, 0, 1};
    auto kinc = CastemInt{1};
    auto k = CastemReal{0};
    (this->fct)(&s.s1(0), &wk.ivs(0), &k, nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &dt,
                &(s.esv0(0)), &(s.desv(0)), &(s.esv0(0)) + 1, &(s.desv(0)) + 1,
                nullptr, &ndi, nullptr, &ntens, &nstatv, &(s.mprops1(0)),
                &nprops, nullptr, &drot(0, 0), &ndt, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr, nullptr, &kinc, 0);
    if (kinc != 1) {
      return {false, ndt};
    }
    if (!s.iv1.empty()) {
      std::copy_n(wk.ivs.begin(), s.iv1.size(), s.iv1.begin());
    }
    return {true, ndt};
  }  // end of CastemModel::integrate

  std::vector<std::string> CastemModel::getOptionalMaterialProperties() const {
    return {};
  }  // end of CastemModel::getOptionalMaterialProperties

  void CastemModel::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager&, const EvolutionManager&) const {
  }  // end of setOptionalMaterialPropertiesDefaultValues

  CastemInterfaceVersion CastemModel::getCastemInterfaceVersion() const {
    return CastemInterfaceVersion::LEGACY_CASTEM_INTERFACE_VERSION;
  }  // end of getCastemInterfaceVersion

  CastemModel::~CastemModel() = default;

}  // end of namespace mtest
