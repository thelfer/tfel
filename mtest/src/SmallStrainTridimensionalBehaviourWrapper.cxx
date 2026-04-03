/*!
 * \file   mtest/src/SmallStrainTridimensionalBehaviourWrapper.cxx
 * \brief
 * \author Thomas Helfer
 * \date   8/11/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/FSAlgorithm/copy.hxx"
#include "MTest/RoundingMode.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/StandardBehaviourBase.hxx"
#include "MTest/SmallStrainTridimensionalBehaviourWrapper.hxx"

namespace mtest {

  static void projectTangenOperator(
      real* const K, const tfel::material::ModellingHypothesis::Hypothesis h) {
    const auto ss = tfel::material::getStensorSize(h);
    for (unsigned short i = 0; i != ss; ++i) {
      for (unsigned short j = 0; j != ss; ++j) {
        K[i * ss + j] = K[i * 6 + j];
      }
    }
  }  // end of projectTangenOperator

  SmallStrainTridimensionalBehaviourWrapper::
      SmallStrainTridimensionalBehaviourWrapper(
          const std::shared_ptr<Behaviour>& wb, const Hypothesis h)
      : BehaviourWrapperBase(wb), hypothesis(h) {
    using tfel::material::MechanicalBehaviourBase;
    const auto bh = this->b->getHypothesis();
    tfel::raise_if(
        ((this->hypothesis == ModellingHypothesis::PLANESTRESS) ||
         (this->hypothesis ==
          ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)),
        "SmallStrainTridimensionalBehaviourWrapper::"
        "SmallStrainTridimensionalBehaviourWrapper: "
        "reducing to plane stress hypotheses is not supported");
    tfel::raise_if(bh != ModellingHypothesis::TRIDIMENSIONAL,
                   "SmallStrainTridimensionalBehaviourWrapper::"
                   "SmallStrainTridimensionalBehaviourWrapper: "
                   "unsupported hypothesis '" +
                       ModellingHypothesis::toString(h) + "'");
    tfel::raise_if(
        this->b->getBehaviourType() !=
            MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,
        "SmallStrainTridimensionalBehaviourWrapper::"
        "SmallStrainTridimensionalBehaviourWrapper: "
        "the wrapped behaviour must be a standard strain based behaviour");
    tfel::raise_if(
        this->b->getBehaviourKinematic() !=
            MechanicalBehaviourBase::SMALLSTRAINKINEMATIC,
        "SmallStrainTridimensionalBehaviourWrapper::"
        "SmallStrainTridimensionalBehaviourWrapper: "
        "the wrapped behaviour must be a standard strain based behaviour");
  }

  SmallStrainTridimensionalBehaviourWrapper::Hypothesis
  SmallStrainTridimensionalBehaviourWrapper::getHypothesis() const {
    return this->hypothesis;
  }  // end of getHypothesis

  tfel::material::MechanicalBehaviourBase::BehaviourType
  SmallStrainTridimensionalBehaviourWrapper::getBehaviourType() const {
    using tfel::material::MechanicalBehaviourBase;
    return MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR;
  }  // end of getBehaviourType

  tfel::material::MechanicalBehaviourBase::Kinematic
  SmallStrainTridimensionalBehaviourWrapper::getBehaviourKinematic() const {
    using tfel::material::MechanicalBehaviourBase;
    return MechanicalBehaviourBase::SMALLSTRAINKINEMATIC;
  }  // end of getBehaviourKinematic

  unsigned short SmallStrainTridimensionalBehaviourWrapper::getGradientsSize()
      const {
    return tfel::material::getStensorSize(this->hypothesis);
  }  // end of getGradientsSize

  void
  SmallStrainTridimensionalBehaviourWrapper::getGradientsDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    std::fill(v.begin(), v.end(), real(0));
  }  // end of getGradientsDefaultInitialValues

  unsigned short
  SmallStrainTridimensionalBehaviourWrapper::getThermodynamicForcesSize()
      const {
    return tfel::material::getStensorSize(this->hypothesis);
  }  // end of getThermodynamicForcesSize

  std::vector<std::string>
  SmallStrainTridimensionalBehaviourWrapper::getVectorComponentsSuffixes()
      const {
    return StandardBehaviourBase::getVectorComponentsSuffixes(this->hypothesis);
  }  // end of getVectorComponentsSuffixes

  std::vector<std::string>
  SmallStrainTridimensionalBehaviourWrapper::getStensorComponentsSuffixes()
      const {
    return StandardBehaviourBase::getStensorComponentsSuffixes(
        this->hypothesis);
  }  // end of getStensorComponentsSuffixes

  std::vector<std::string>
  SmallStrainTridimensionalBehaviourWrapper::getTensorComponentsSuffixes()
      const {
    return StandardBehaviourBase::getTensorComponentsSuffixes(this->hypothesis);
  }  // end of getTensorComponentsSuffixes

  std::vector<std::string>
  SmallStrainTridimensionalBehaviourWrapper::getGradientsComponents() const {
    std::vector<std::string> c;
    for (const auto& e : this->getStensorComponentsSuffixes()) {
      c.push_back("E" + e);
    }
    return c;
  }  // end of getGradientsComponents

  std::vector<std::string>
  SmallStrainTridimensionalBehaviourWrapper::getThermodynamicForcesComponents()
      const {
    std::vector<std::string> c;
    for (const auto& e : this->getStensorComponentsSuffixes()) {
      c.push_back("S" + e);
    }
    return c;
  }  // end of getThermodynamicForcesComponents

  unsigned short
  SmallStrainTridimensionalBehaviourWrapper::getGradientComponentPosition(
      const std::string& c) const {
    const auto components = this->getGradientsComponents();
    const auto p = std::find(components.begin(), components.end(), c);
    if (p == components.end()) {
      tfel::raise(
          "SmallStrainTridimensionalBehaviourWrapper::"
          "getGradientComponentPosition: invalid component '" +
          c + "'");
    }
    return static_cast<unsigned short>(p - components.begin());
  }  // end of getGradientComponentPosition

  unsigned short SmallStrainTridimensionalBehaviourWrapper::
      getThermodynamicForceComponentPosition(const std::string& c) const {
    const auto components = this->getThermodynamicForcesComponents();
    const auto p = std::find(components.begin(), components.end(), c);
    if (p == components.end()) {
      tfel::raise(
          "SmallStrainTridimensionalBehaviourWrapper::"
          "getThermodynamicForceComponentPosition: invalid component '" +
          c + "'");
    }
    return static_cast<unsigned short>(p - components.begin());
  }  // end of getThermodynamicForceComponentPosition

  size_t
  SmallStrainTridimensionalBehaviourWrapper::getTangentOperatorArraySize()
      const {
    return static_cast<size_t>(this->getGradientsSize() *
                               this->getThermodynamicForcesSize());
  }  // end of getTangentOperatorArraySize

  std::vector<std::pair<std::string, std::string>>
  SmallStrainTridimensionalBehaviourWrapper::getTangentOperatorBlocks() const {
    return {{"Stress", "Strain"}};
  }  // end of getTangentOperatorBlocks()

  std::vector<std::string>
  SmallStrainTridimensionalBehaviourWrapper::getInternalStateVariablesNames()
      const {
    return this->b->expandInternalStateVariablesNames();
  }  // end of getInternalStateVariablesNames

  std::vector<std::string>
  SmallStrainTridimensionalBehaviourWrapper::expandInternalStateVariablesNames()
      const {
    return this->b->expandInternalStateVariablesNames();
  }  // end of expandInternalStateVariablesNames

  size_t
  SmallStrainTridimensionalBehaviourWrapper::getInternalStateVariablesSize()
      const {
    return this->b->expandInternalStateVariablesNames().size();
  }  // end of getInternalStateVariablesSize

  std::vector<std::string> SmallStrainTridimensionalBehaviourWrapper::
      getInternalStateVariablesDescriptions() const {
    return this->b->expandInternalStateVariablesNames();
  }  // end of getInternalStateVariablesDescriptions

  unsigned short
  SmallStrainTridimensionalBehaviourWrapper::getInternalStateVariableType(
      const std::string& n) const {
    const auto isvnames = this->getInternalStateVariablesNames();
    if (std::find(isvnames.begin(), isvnames.end(), n) == isvnames.end()) {
      tfel::raise(
          "SmallStrainTridimensionalBehaviourWrapper::"
          "getInternalStateVariableType: "
          "invalid state variable '" +
          n + "'");
    }
    return 0;
  }  // end of getInternalStateVariableType

  unsigned short
  SmallStrainTridimensionalBehaviourWrapper::getInternalStateVariablePosition(
      const std::string& n) const {
    const auto isvnames = this->getInternalStateVariablesNames();
    const auto p = std::find(isvnames.begin(), isvnames.end(), n);
    if (p == isvnames.end()) {
      tfel::raise(
          "SmallStrainTridimensionalBehaviourWrapper::"
          "getInternalStateVariablePosition: "
          "invalid state variable '" +
          n + "'");
    }
    return static_cast<unsigned short>(p - isvnames.begin());
  }  // end of getInternalStateVariablePosition

  std::vector<std::string>
  SmallStrainTridimensionalBehaviourWrapper::getExternalStateVariablesNames()
      const {
    return this->b->expandExternalStateVariablesNames();
  }  // end of getExternalStateVariablesNames

  std::vector<std::string>
  SmallStrainTridimensionalBehaviourWrapper::expandExternalStateVariablesNames()
      const {
    return this->b->expandExternalStateVariablesNames();
  }  // end of expandExternalStateVariablesNames

  size_t
  SmallStrainTridimensionalBehaviourWrapper::getExternalStateVariablesSize()
      const {
    return this->b->expandExternalStateVariablesNames().size();
  }  // end of getExternalStateVariablesSize

  unsigned short
  SmallStrainTridimensionalBehaviourWrapper::getExternalStateVariableType(
      const std::string& n) const {
    const auto esvnames = this->getExternalStateVariablesNames();
    if (std::find(esvnames.begin(), esvnames.end(), n) == esvnames.end()) {
      tfel::raise(
          "SmallStrainTridimensionalBehaviourWrapper::"
          "getExternalStateVariableType: "
          "invalid state variable '" +
          n + "'");
    }
    return 0;
  }  // end of getExternalStateVariableType

  unsigned short
  SmallStrainTridimensionalBehaviourWrapper::getExternalStateVariablePosition(
      const std::string& n) const {
    const auto esvnames = this->getExternalStateVariablesNames();
    const auto p = std::find(esvnames.begin(), esvnames.end(), n);
    if (p == esvnames.end()) {
      tfel::raise(
          "SmallStrainTridimensionalBehaviourWrapper::"
          "getExternalStateVariablePosition: "
          "invalid state variable '" +
          n + "'");
    }
    return static_cast<unsigned short>(p - esvnames.begin());
  }  // end of getExternalStateVariablePosition

  void SmallStrainTridimensionalBehaviourWrapper::allocateWorkSpace(
      BehaviourWorkSpace& wk) const {
    this->b->allocateWorkSpace(wk);
  }  // end of allocateWorkSpace

  tfel::math::tmatrix<3u, 3u, real>
  SmallStrainTridimensionalBehaviourWrapper::getRotationMatrix(
      const tfel::math::vector<real>& mp,
      const tfel::math::tmatrix<3u, 3u, real>& r) const {
    return this->b->getRotationMatrix(mp, r);
  }  // end of getRotationMatrix

  bool SmallStrainTridimensionalBehaviourWrapper::doPackagingStep(
      CurrentState& s, BehaviourWorkSpace& wk) const {
    return this->b->doPackagingStep(s, wk);
  }  // end of doPackagingStep

  std::pair<bool, real>
  SmallStrainTridimensionalBehaviourWrapper::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    const auto r = this->b->computePredictionOperator(wk, s, ktype);
    if (!r.first) {
      return r;
    }
    // modify wk.kp
    if ((ktype != StiffnessMatrixType::NOSTIFFNESS) &&
        (ktype != StiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE) &&
        (ktype != StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES)) {
      projectTangenOperator(wk.kt.data(), this->hypothesis);
    }
    return r;
  }  // end of computePredictionOperator

  std::pair<bool, real> SmallStrainTridimensionalBehaviourWrapper::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    const auto r = this->b->integrate(s, wk, dt, ktype);
    if ((r.first) && (ktype != StiffnessMatrixType::NOSTIFFNESS)) {
      projectTangenOperator(wk.kt.data(), this->hypothesis);
    }
    return r;
  }  // end of integrate

  SmallStrainTridimensionalBehaviourWrapper::
      ~SmallStrainTridimensionalBehaviourWrapper() = default;

}  // end of namespace mtest
