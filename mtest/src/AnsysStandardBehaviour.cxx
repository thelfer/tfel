/*!
 * \file  mtest/src/AnsysStandardBehaviour.cxx
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
#include "MFront/Ansys/Ansys.hxx"
#include "MFront/Ansys/AnsysComputeStiffnessTensor.hxx"

#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/AnsysStandardBehaviour.hxx"

namespace mtest {

  std::string AnsysStandardBehaviour::getHypothesisSuffix(const Hypothesis h) {
    if (h == ModellingHypothesis::AXISYMMETRICAL) {
      return "_axis";
    } else if (h == ModellingHypothesis::PLANESTRAIN) {
      return "_pstrain";
    } else if (h == ModellingHypothesis::PLANESTRESS) {
      return "_pstress";
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      return "_3D";
    }
    tfel::raise(
        "AnsysStandardBehaviour::getHypothesisSuffix: "
        "invalid hypothesis.");
  }  // end of AnsysStandardBehaviour::getHypothesisSuffix

  std::string AnsysStandardBehaviour::getBehaviourName(const std::string& b,
                                                       const Hypothesis h) {
    auto ends = [&b](const std::string& s) {
      if (b.length() >= s.length()) {
        return b.compare(b.length() - s.length(), s.length(), s) == 0;
      }
      return false;
    };
    const auto s = AnsysStandardBehaviour::getHypothesisSuffix(h);
    tfel::raise_if(!ends(s),
                   "AnsysStandardBehaviour::AnsysStandardBehaviour: "
                   "invalid function name.");
    return {b.begin(), b.begin() + b.length() - s.length()};
  }

  AnsysStandardBehaviour::AnsysStandardBehaviour(const Hypothesis h,
                                                 const std::string& l,
                                                 const std::string& b)
      : StandardBehaviourBase(
            h, l, AnsysStandardBehaviour::getBehaviourName(b, h)) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "AnsysStandardBehaviour::AnsysStandardBehaviour: " + m);
    };
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    const auto bn = AnsysStandardBehaviour::getBehaviourName(b, h);
    throw_if(elm.getInterface(l, bn) != "Ansys",
             "invalid interface '" + elm.getInterface(l, bn) + "'");
    this->fct = elm.getAnsysExternalBehaviourFunction(l, b);
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
        tmp.insert(tmp.end(),
                   {"FirstOrthotropicAxis_1", "FirstOrthotropicAxis_2"});
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
        tmp.insert(tmp.end(),
                   {"FirstOrthotropicAxis_1", "FirstOrthotropicAxis_2",
                    "FirstOrthotropicAxis_3", "SecondOrthotropicAxis_1",
                    "SecondOrthotropicAxis_2", "SecondOrthotropicAxis_3"});
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

  tfel::math::tmatrix<3u, 3u, real> AnsysStandardBehaviour::getRotationMatrix(
      const tfel::math::vector<real>&,
      const tfel::math::tmatrix<3u, 3u, real>& r) const {
    return r;
  }  // end of AnsysStandardBehaviour::getRotationMatrix

  void AnsysStandardBehaviour::allocate(BehaviourWorkSpace& wk) const {
    const auto ndv = this->getGradientsSize();
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
  }  // end of AnsysStandardBehaviour::allocate

  StiffnessMatrixType AnsysStandardBehaviour::getDefaultStiffnessMatrixType()
      const {
    return StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }

  std::pair<bool, real> AnsysStandardBehaviour::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    if (ktype == StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES) {
      return {false, real(-1)};
    }
    wk.cs = s;
    return this->call_behaviour(wk.kt, wk.cs, wk, real(1), ktype, false);
  }

  std::pair<bool, real> AnsysStandardBehaviour::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    return this->call_behaviour(wk.k, s, wk, dt, ktype, true);
  }  // end of AnsysStandardBehaviour::integrate

  std::vector<std::string>::size_type
  AnsysStandardBehaviour::getOrthototropicAxesOffset() const {
    const auto b = this->mpnames.begin();
    const auto e = this->mpnames.end();
    const auto p = std::find(b, e, "FirstOrthotropicAxis_1");
    tfel::raise_if(p == this->mpnames.end(),
                   "AnsysStandardBehaviour::getOrthototropicAxesOffset: "
                   "orthotropic axes not found");
    return static_cast<std::vector<std::string>::size_type>(p - b);
  }  // end of AnsysStandardBehaviour::getOrthototropicAxesOffset

  void AnsysStandardBehaviour::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager& mp, const EvolutionManager& evm) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c,
                     "AnsysStandardBehaviour::"
                     "setOptionalMaterialPropertiesDefaultValues: " +
                         m);
    };
    if (this->stype == 1) {
      const auto h = this->getHypothesis();
      // orthotropic behaviour
      const bool bv1x = evm.find("FirstOrthotropicAxis_1") != evm.end();
      const bool bv1y = evm.find("FirstOrthotropicAxis_2") != evm.end();
      if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::PLANESTRAIN)) {
        throw_if(bv1x || bv1y,
                 "The components of the orthotropic basis shall not be"
                 "defined directly, use @RotationMatrix");
        Behaviour::setOptionalMaterialPropertyDefaultValue(
            mp, evm, "FirstOrthotropicAxis_1", 1.);
        Behaviour::setOptionalMaterialPropertyDefaultValue(
            mp, evm, "FirstOrthotropicAxis_2", 0.);
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        const bool bv1z = evm.find("FirstOrthotropicAxis_3") != evm.end();
        const bool bv2x = evm.find("SecondOrthotropicAxis_1") != evm.end();
        const bool bv2y = evm.find("SecondOrthotropicAxis_2") != evm.end();
        const bool bv2z = evm.find("SecondOrthotropicAxis_3") != evm.end();
        const bool b = bv1x || bv1y || bv1z || bv2x || bv2y || bv2z;
        throw_if(b,
                 "The components of the orthotropic basis shall not be"
                 "defined directly, use @RotationMatrix");
        Behaviour::setOptionalMaterialPropertyDefaultValue(
            mp, evm, "FirstOrthotropicAxis_1", 1.);
        Behaviour::setOptionalMaterialPropertyDefaultValue(
            mp, evm, "FirstOrthotropicAxis_2", 0.);
        Behaviour::setOptionalMaterialPropertyDefaultValue(
            mp, evm, "FirstOrthotropicAxis_3", 0.);
        Behaviour::setOptionalMaterialPropertyDefaultValue(
            mp, evm, "SecondOrthotropicAxis_1", 0.);
        Behaviour::setOptionalMaterialPropertyDefaultValue(
            mp, evm, "SecondOrthotropicAxis_2", 1.);
        Behaviour::setOptionalMaterialPropertyDefaultValue(
            mp, evm, "SecondOrthotropicAxis_3", 0.);
      } else {
        throw_if(true, "unsupported hypothesis");
      }
    }
  }  // end of
     // AnsysStandardBehaviour::setOptionalMaterialPropertiesDefaultValues

  AnsysStandardBehaviour::~AnsysStandardBehaviour() = default;

}  // end of namespace mtest
