/*!
 * \file  mfront/mtest/AsterStandardBehaviour.cxx
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
#include <sstream>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/Aster/Aster.hxx"
#include "MFront/Aster/AsterComputeStiffnessTensor.hxx"

#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/AsterStandardBehaviour.hxx"

namespace mtest {

  AsterStandardBehaviour::AsterStandardBehaviour(const Hypothesis h,
                                                 const std::string& l,
                                                 const std::string& b)
      : UmatBehaviourBase(h, l, b), savesTangentOperator(false) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "AsterStandardBehaviour::AsterStandardBehaviour: " + m);
    };
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    throw_if(elm.getInterface(l, b) != "Aster",
             "invalid interface '" + elm.getInterface(l, b) + "'");
    this->fct = elm.getAsterFunction(l, b);
    this->emsg = elm.getAsterIntegrationErrorMessageFunction(l, b);
    this->savesTangentOperator =
        elm.checkIfAsterBehaviourSavesTangentOperator(l, b);
    auto tmp = std::vector<std::string>{};
    if (this->etype == 0u) {
      if (this->requiresStiffnessTensor) {
        tmp.insert(tmp.end(), {"YoungModulus", "PoissonRatio"});
      }
      if (this->requiresThermalExpansionCoefficientTensor) {
        tmp.push_back("ThermalExpansion");
      }
    } else if (this->etype == 1u) {
      if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
        if (this->requiresStiffnessTensor) {
          tmp.insert(tmp.end(),
                     {"YoungModulus1", "YoungModulus2", "YoungModulus3",
                      "PoissonRatio12", "PoissonRatio23", "PoissonRatio13"});
        }
        if (this->requiresThermalExpansionCoefficientTensor) {
          tmp.insert(tmp.end(), {"ThermalExpansion1", "ThermalExpansion2",
                                 "ThermalExpansion3"});
        }
      } else if ((h == ModellingHypothesis::PLANESTRESS) ||
                 (h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
        if (this->requiresStiffnessTensor) {
          tmp.insert(tmp.end(),
                     {"YoungModulus1", "YoungModulus2", "YoungModulus3",
                      "PoissonRatio12", "PoissonRatio23", "PoissonRatio13",
                      "ShearModulus12"});
        }
        if (this->requiresThermalExpansionCoefficientTensor) {
          tmp.insert(tmp.end(), {"ThermalExpansion1", "ThermalExpansion2",
                                 "ThermalExpansion3"});
        }
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        if (this->requiresStiffnessTensor) {
          tmp.insert(tmp.end(),
                     {"YoungModulus1", "YoungModulus2", "YoungModulus3",
                      "PoissonRatio12", "PoissonRatio23", "PoissonRatio13",
                      "ShearModulus12", "ShearModulus23", "ShearModulus13"});
        }
        if (this->requiresThermalExpansionCoefficientTensor) {
          tmp.insert(tmp.end(), {"ThermalExpansion1", "ThermalExpansion2",
                                 "ThermalExpansion3"});
        }
      } else {
        throw_if(true, "unsupported modelling hypothesis");
      }
    } else {
      throw_if(
          true,
          "unsupported behaviour type (neither isotropic nor orthotropic)");
    }
    this->mpnames.insert(this->mpnames.begin(), tmp.begin(), tmp.end());
  }

  tfel::math::tmatrix<3u, 3u, real> AsterStandardBehaviour::getRotationMatrix(
      const tfel::math::vector<real>&,
      const tfel::math::tmatrix<3u, 3u, real>& r) const {
    return r;
  }  // end of AsterStandardBehaviour::getRotationMatrix

  size_t AsterStandardBehaviour::getInternalStateVariablesSize() const {
    size_t s(0);
    const auto h = this->getHypothesis();
    if (this->savesTangentOperator) {
      if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
        s = 9;
      } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::PLANESTRESS) ||
                 (h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
        s = 16;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        s = 36;
      } else {
        tfel::raise(
            "AsterStandardBehaviour::getInternalStateVariablesSize: "
            "invalid modelling hypothesis");
      }
    }
    return s + UmatBehaviourBase::getInternalStateVariablesSize();
  }  // end of AsterStandardBehaviour::getInternalStateVariablesSize() const

  std::vector<std::string>
  AsterStandardBehaviour::getInternalStateVariablesDescriptions() const {
    auto desc = UmatBehaviourBase::getInternalStateVariablesDescriptions();
    if (this->savesTangentOperator) {
      size_t s(0);
      const auto h = this->getHypothesis();
      if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) {
        s = 3;
      } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::PLANESTRESS) ||
                 (h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
        s = 4;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        s = 6;
      } else {
        tfel::raise(
            "AsterStandardBehaviour::getInternalStateVariablesDescriptions: "
            "invalid modelling hypothesis");
      }
      for (unsigned short i = 0; i != s; ++i) {
        for (unsigned short j = 0; j != s; ++j) {
          std::ostringstream n;
          n << "component (" << j << "," << i << ") of the tangent operator";
          desc.push_back(n.str());
        }
      }
    }
    return desc;
  }  // end of AsterStandardBehaviour::getInternalStateVariablesDescriptions

  void AsterStandardBehaviour::allocate(BehaviourWorkSpace& wk) const {
    const auto ndv = this->getDrivingVariablesSize();
    const auto nth = this->getThermodynamicForcesSize();
    const auto nstatev = this->getInternalStateVariablesSize();
    wk.kt.resize(nth, ndv);
    wk.k.resize(nth, ndv);
    wk.D.resize(nth, ndv);
    wk.mps.resize(this->mpnames.size() == 0 ? 1u : this->mpnames.size(),
                  real(0));
    wk.ivs.resize(nstatev == 0 ? 1u : nstatev, real(0));
    wk.nk.resize(nth, ndv);
    wk.ne.resize(ndv);
    wk.ns.resize(nth);
    wk.nivs.resize(nstatev);
    mtest::allocate(wk.cs, this->shared_from_this());
  }  // end of AsterStandardBehaviour::allocate

  StiffnessMatrixType AsterStandardBehaviour::getDefaultStiffnessMatrixType()
      const {
    return StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }

  std::pair<bool, real> AsterStandardBehaviour::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    if (ktype == StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES) {
      return {false, real(-1)};
    }
    wk.cs = s;
    return this->call_behaviour(wk.kt, wk.cs, wk, real(1), ktype, false);
  }

  std::pair<bool, real> AsterStandardBehaviour::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    return this->call_behaviour(wk.k, s, wk, dt, ktype, true);
  }  // end of AsterStandardBehaviour::integrate

  AsterStandardBehaviour::~AsterStandardBehaviour() = default;

}  // end of namespace mtest
