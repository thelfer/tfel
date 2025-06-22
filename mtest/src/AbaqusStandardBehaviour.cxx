/*!
 * \file  mfront/mtest/AbaqusStandardBehaviour.cxx
 * \brief
 * \author Helfer Thomas
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

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/Abaqus/Abaqus.hxx"
#include "MFront/Abaqus/AbaqusComputeStiffnessTensor.hxx"

#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/AbaqusStandardBehaviour.hxx"

namespace mtest {

  std::string AbaqusStandardBehaviour::getBehaviourName(const std::string& b,
                                                        const Hypothesis h) {
    auto ends = [&b, h](const std::string& s) {
      if (b.length() >= s.length()) {
        return b.compare(b.length() - s.length(), s.length(), s) == 0;
      }
      return false;
    };
    const auto s = [h]() -> std::string {
      if (h == ModellingHypothesis::AXISYMMETRICAL) {
        return "_AXIS";
      } else if (h == ModellingHypothesis::PLANESTRAIN) {
        return "_PSTRAIN";
      } else if (h == ModellingHypothesis::PLANESTRESS) {
        return "_PSTRESS";
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return "_3D";
      }
      throw(std::runtime_error(
          "AbaqusStandardBehaviour::AbaqusStandardBehaviour: "
          "invalid hypothesis."));
    }();
    if (!ends(s)) {
      throw(std::runtime_error(
          "AbaqusStandardBehaviour::AbaqusStandardBehaviour: "
          "invalid function name."));
    }
    return {b.begin(), b.begin() + b.length() - s.length()};
  }

  AbaqusStandardBehaviour::AbaqusStandardBehaviour(const Hypothesis h,
                                                   const std::string& l,
                                                   const std::string& b)
      : UmatBehaviourBase(
            h, l, AbaqusStandardBehaviour::getBehaviourName(b, h)) {
    auto throw_if = [](const bool c, const std::string& m) {
      if (c) {
        throw(
            std::runtime_error("AbaqusStandardBehaviour::"
                               "AbaqusStandardBehaviour: " +
                               m));
      }
    };
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    const auto bn = AbaqusStandardBehaviour::getBehaviourName(b, h);
    throw_if(elm.getInterface(l, bn) != "Abaqus",
             "invalid interface '" + elm.getInterface(l, bn) + "'");
    this->fct = elm.getAbaqusExternalBehaviourFunction(l, b);
    if (this->stype == 1u) {
      this->omp = elm.getAbaqusOrthotropyManagementPolicy(l, bn);
      if (this->omp == 2u) {
        auto aivs = std::vector<std::string>{};
        if ((h == ModellingHypothesis::PLANESTRESS) ||
            (h == ModellingHypothesis::PLANESTRAIN) ||
            (h == ModellingHypothesis::AXISYMMETRICAL) ||
            (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
          aivs = {"FirstOrthotropicDirection_1", "FirstOrthotropicDirection_2"};
        } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          aivs = {
              "FirstOrthotropicDirection_1",  "FirstOrthotropicDirection_2",
              "FirstOrthotropicDirection_3",  "SecondOrthotropicDirection_1",
              "SecondOrthotropicDirection_2", "SecondOrthotropicDirection_3"};
        } else {
          throw_if(true, "unsupported modelling hypothesis");
        }
        for (const auto& iv : aivs) {
          throw_if(std::find(this->ivnames.begin(), this->ivnames.end(), iv) !=
                       this->ivnames.end(),
                   iv + " is a reserved name");
          this->ivtypes.insert(this->ivtypes.begin(), 0);
        }
        this->ivnames.insert(this->ivnames.begin(), aivs.begin(), aivs.end());
      }
    }
    auto tmp = std::vector<std::string>{};
    if (this->etype == 0u) {
      if (this->requiresStiffnessTensor) {
        tmp.insert(tmp.end(), {"YoungModulus", "PoissonRatio"});
      }
      if (this->requiresThermalExpansionCoefficientTensor) {
        tmp.push_back("ThermalExpansion");
      }
    } else if (this->etype == 1u) {
      if ((h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::AXISYMMETRICAL)) {
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
      throw_if(true,
               "unsupported behaviour type "
               "(neither isotropic nor orthotropic)");
    }
    this->mpnames.insert(this->mpnames.begin(), tmp.begin(), tmp.end());
  }

  tfel::math::tmatrix<3u, 3u, real> AbaqusStandardBehaviour::getRotationMatrix(
      const tfel::math::vector<real>&,
      const tfel::math::tmatrix<3u, 3u, real>& r) const {
    return r;
  }  // end of AbaqusStandardBehaviour::getRotationMatrix

  void AbaqusStandardBehaviour::allocate(BehaviourWorkSpace& wk) const {
    const auto ndv = this->getDrivingVariablesSize();
    const auto nth = this->getThermodynamicForcesSize();
    const auto nstatev = this->getInternalStateVariablesSize();
    wk.D.resize(nth, nth);
    wk.kt.resize(nth, ndv);
    wk.k.resize(nth, ndv);
    wk.mps.resize(this->mpnames.size() == 0 ? 1u : this->mpnames.size(),
                  real(0));
    wk.ivs.resize(nstatev);
    wk.nk.resize(nth, ndv);
    wk.ne.resize(ndv);
    wk.ns.resize(nth);
    wk.nivs.resize(nstatev);
    mtest::allocate(wk.cs, this->shared_from_this());
  }  // end of AbaqusStandardBehaviour::allocate

  StiffnessMatrixType AbaqusStandardBehaviour::getDefaultStiffnessMatrixType()
      const {
    return StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }

  std::pair<bool, real> AbaqusStandardBehaviour::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    if (ktype == StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES) {
      return {false, real(-1)};
    }
    wk.cs = s;
    return this->call_behaviour(wk.kt, wk.cs, wk, real(1), ktype, false);
  }

  std::pair<bool, real> AbaqusStandardBehaviour::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    return this->call_behaviour(wk.k, s, wk, dt, ktype, true);
  }  // end of AbaqusStandardBehaviour::integrate

  AbaqusStandardBehaviour::~AbaqusStandardBehaviour() = default;

}  // end of namespace mtest
