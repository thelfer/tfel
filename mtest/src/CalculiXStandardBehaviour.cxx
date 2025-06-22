/*!
 * \file  mtest/src/CalculiXStandardBehaviour.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  03/08/2017
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
#include "MFront/CalculiX/CalculiX.hxx"
#include "MFront/CalculiX/CalculiXComputeStiffnessTensor.hxx"

#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/CalculiXStandardBehaviour.hxx"

namespace mtest {

  CalculiXStandardBehaviour::CalculiXStandardBehaviour(const Hypothesis h,
                                                       const std::string& l,
                                                       const std::string& b)
      : StandardBehaviourBase(h, l, b) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c,
                     "CalculiXStandardBehaviour::"
                     "CalculiXStandardBehaviour: " +
                         m);
    };
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    throw_if(h != ModellingHypothesis::TRIDIMENSIONAL,
             "unsupported modelling hypothesis");
    throw_if(elm.getInterface(l, b) != "CalculiX",
             "invalid interface '" + elm.getInterface(l, b) + "'");
    this->fct = elm.getCalculiXExternalBehaviourFunction(l, b);
    auto tmp = std::vector<std::string>{};
    if (this->etype == 0u) {
      if (this->requiresStiffnessTensor) {
        tmp.insert(tmp.end(), {"YoungModulus", "PoissonRatio"});
      }
      if (this->requiresThermalExpansionCoefficientTensor) {
        tmp.push_back("ThermalExpansion");
      }
    } else if (this->etype == 1u) {
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
      throw_if(true,
               "unsupported behaviour type "
               "(neither isotropic nor orthotropic)");
    }
    this->mpnames.insert(this->mpnames.begin(), tmp.begin(), tmp.end());
  }

  tfel::math::tmatrix<3u, 3u, real>
  CalculiXStandardBehaviour::getRotationMatrix(
      const tfel::math::vector<real>&,
      const tfel::math::tmatrix<3u, 3u, real>& r) const {
    return r;
  }  // end of CalculiXStandardBehaviour::getRotationMatrix

  void CalculiXStandardBehaviour::allocate(BehaviourWorkSpace& wk) const {
    const auto ndv = this->getGradientsSize();
    const auto nth = this->getThermodynamicForcesSize();
    const auto nstatev = this->getInternalStateVariablesSize();
    wk.D.resize(21, 1);
    wk.kt.resize(nth, ndv);
    wk.k.resize(nth, ndv);
    wk.ivs.resize(nstatev);
    wk.nk.resize(nth, ndv);
    wk.ne.resize(ndv);
    wk.ns.resize(nth);
    wk.nivs.resize(nstatev);
    mtest::allocate(wk.cs, this->shared_from_this());
  }  // end of CalculiXStandardBehaviour::allocate

  StiffnessMatrixType CalculiXStandardBehaviour::getDefaultStiffnessMatrixType()
      const {
    return StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }

  std::pair<bool, real> CalculiXStandardBehaviour::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    if (ktype == StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES) {
      return {false, real(-1)};
    }
    wk.cs = s;
    return this->call_behaviour(wk.kt, wk.cs, wk, real(1), ktype, false);
  }

  std::pair<bool, real> CalculiXStandardBehaviour::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    return this->call_behaviour(wk.k, s, wk, dt, ktype, true);
  }  // end of CalculiXStandardBehaviour::integrate

  tfel::math::st2tost2<3u, real>
  CalculiXStandardBehaviour::convertTangentOperator(const real* const D) const {
    tfel::math::st2tost2<3u, real> K;
    constexpr const auto sqrt2 = tfel::math::Cste<real>::sqrt2;
    K(0, 0) = D[0];
    K(0, 1) = K(1, 0) = D[1];
    K(1, 1) = D[2];
    K(0, 2) = K(2, 0) = D[3];
    K(1, 2) = K(2, 1) = D[4];
    K(2, 2) = D[5];
    K(0, 3) = K(3, 0) = D[6] * sqrt2;
    K(1, 3) = K(3, 1) = D[7] * sqrt2;
    K(2, 3) = K(3, 2) = D[8] * sqrt2;
    K(3, 3) = D[9] * 2;
    K(0, 4) = K(4, 0) = D[10] * sqrt2;
    K(1, 4) = K(4, 1) = D[11] * sqrt2;
    K(2, 4) = K(4, 2) = D[12] * sqrt2;
    K(3, 4) = K(4, 3) = D[13] * 2;
    K(4, 4) = D[14] * 2;
    K(0, 5) = K(5, 0) = D[15] * sqrt2;
    K(1, 5) = K(5, 1) = D[16] * sqrt2;
    K(2, 5) = K(5, 2) = D[17] * sqrt2;
    K(3, 5) = K(5, 3) = D[18] * 2;
    K(4, 5) = K(5, 4) = D[19] * 2;
    K(5, 5) = D[20] * 2;
    return K;
  }  // end of CalculiXStandardBehaviour::convertTangentOperator

  CalculiXStandardBehaviour::~CalculiXStandardBehaviour() = default;

}  // end of namespace mtest
