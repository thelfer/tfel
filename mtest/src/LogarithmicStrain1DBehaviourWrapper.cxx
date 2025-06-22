/*!
 * \file   LogarithmicStrain1DBehaviourWrapper.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28 déc. 2015
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
#include "MTest/LogarithmicStrain1DBehaviourWrapper.hxx"

namespace mtest {

  static void convertStiffness(tfel::math::matrix<real>& k,
                               const tfel::math::vector<real>& e,
                               const tfel::math::vector<real>& s) {
    k(0, 0) = (-s(0) + k(0, 0) / (1 + e(0))) / (1 + e(0));
    k(1, 1) = (-s(1) + k(1, 1) / (1 + e(1))) / (1 + e(1));
    k(2, 2) = (-s(2) + k(2, 2) / (1 + e(2))) / (1 + e(2));
    k(0, 1) = k(0, 1) / ((1 + e(1)) * (1 + e(0)));
    k(1, 0) = k(1, 0) / ((1 + e(0)) * (1 + e(1)));
    k(2, 0) = k(2, 0) / ((1 + e(2)) * (1 + e(0)));
    k(0, 2) = k(0, 2) / ((1 + e(0)) * (1 + e(2)));
    k(1, 2) = k(1, 2) / ((1 + e(2)) * (1 + e(1)));
    k(2, 1) = k(2, 1) / ((1 + e(1)) * (1 + e(2)));
  }

  LogarithmicStrain1DBehaviourWrapper::LogarithmicStrain1DBehaviourWrapper(
      const std::shared_ptr<Behaviour>& wb)
      : b(wb) {
    using tfel::material::MechanicalBehaviourBase;
    const auto h = this->b->getHypothesis();
    tfel::raise_if(
        h != ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,
        "LogarithmicStrain1DBehaviourWrapper::"
        "LogarithmicStrain1DBehaviourWrapper: "
        "unsupported hypothesis '" +
            ModellingHypothesis::toString(h) + "'");
    tfel::raise_if(this->b->getBehaviourType() !=
                       MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR,
                   "LogarithmicStrain1DBehaviourWrapper::"
                   "LogarithmicStrain1DBehaviourWrapper: "
                   "the underlying behaviour must be small strain");
  }

  std::string LogarithmicStrain1DBehaviourWrapper::getBehaviourName() const {
    return this->b->getBehaviourName();
  }  // end of getBehaviourName()

  LogarithmicStrain1DBehaviourWrapper::Hypothesis
  LogarithmicStrain1DBehaviourWrapper::getHypothesis() const {
    return this->b->getHypothesis();
  }  // end of getHypothesis

  tfel::material::MechanicalBehaviourBase::BehaviourType
  LogarithmicStrain1DBehaviourWrapper::getBehaviourType() const {
    using tfel::material::MechanicalBehaviourBase;
    return MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR;
  }  // end of getBehaviourType

  tfel::material::MechanicalBehaviourBase::Kinematic
  LogarithmicStrain1DBehaviourWrapper::getBehaviourKinematic() const {
    using tfel::material::MechanicalBehaviourBase;
    return MechanicalBehaviourBase::FINITESTRAINKINEMATIC_ETO_PK1;
  }  // end of getBehaviourKinematic

  unsigned short LogarithmicStrain1DBehaviourWrapper::getGradientsSize() const {
    return this->b->getGradientsSize();
  }  // end of getGradientsSize

  void LogarithmicStrain1DBehaviourWrapper::getGradientsDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    return this->b->getGradientsDefaultInitialValues(v);
  }  // end of getGradientsDefaultInitialValues

  unsigned short
  LogarithmicStrain1DBehaviourWrapper::getThermodynamicForcesSize() const {
    return this->b->getThermodynamicForcesSize();
  }  // end of getThermodynamicForcesSize

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getStensorComponentsSuffixes() const {
    return this->b->getStensorComponentsSuffixes();
  }  // end of getStensorComponentsSuffixes

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getVectorComponentsSuffixes() const {
    return this->b->getVectorComponentsSuffixes();
  }  // end of getVectorComponentsSuffixes

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getTensorComponentsSuffixes() const {
    return this->b->getTensorComponentsSuffixes();
  }  // end of getTensorComponentsSuffixes

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getGradientsComponents() const {
    return this->b->getGradientsComponents();
  }  // end of getGradientsComponents

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getThermodynamicForcesComponents()
      const {
    return this->b->getThermodynamicForcesComponents();
  }  // end of getThermodynamicForcesComponents

  unsigned short
  LogarithmicStrain1DBehaviourWrapper::getGradientComponentPosition(
      const std::string& c) const {
    return this->b->getGradientComponentPosition(c);
  }  // end of getGradientComponentPosition

  unsigned short
  LogarithmicStrain1DBehaviourWrapper::getThermodynamicForceComponentPosition(
      const std::string& c) const {
    return this->b->getThermodynamicForceComponentPosition(c);
  }  // end of getThermodynamicForceComponentPosition

  size_t LogarithmicStrain1DBehaviourWrapper::getTangentOperatorArraySize()
      const {
    return this->b->getTangentOperatorArraySize();
  }  // end of getTangentOperatorArraySize

  std::vector<std::pair<std::string, std::string>>
  LogarithmicStrain1DBehaviourWrapper::getTangentOperatorBlocks() const {
    return this->b->getTangentOperatorBlocks();
  }  // end of getTangentOperatorBlocks()

  unsigned short LogarithmicStrain1DBehaviourWrapper::getSymmetryType() const {
    return this->b->getSymmetryType();
  }  // end of getSymmetryType

  size_t LogarithmicStrain1DBehaviourWrapper::getMaterialPropertiesSize()
      const {
    return this->b->getMaterialPropertiesSize();
  }  // end of getMaterialPropertiesSize

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getMaterialPropertiesNames() const {
    return this->b->getMaterialPropertiesNames();
  }  // end of getMaterialPropertiesNames

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getOptionalMaterialProperties() const {
    return {};
  }  // end of getOptionalMaterialProperties()

  void LogarithmicStrain1DBehaviourWrapper::
      setOptionalMaterialPropertiesDefaultValues(
          EvolutionManager& mp, const EvolutionManager& evm) const {
    return this->b->setOptionalMaterialPropertiesDefaultValues(mp, evm);
  }  // end of setOptionalMaterialPropertiesDefaultValues

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getInternalStateVariablesNames() const {
    return this->b->getInternalStateVariablesNames();
  }  // end of getInternalStateVariablesNames

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::expandInternalStateVariablesNames()
      const {
    return this->b->expandInternalStateVariablesNames();
  }  // end of expandInternalStateVariablesNames

  size_t LogarithmicStrain1DBehaviourWrapper::getInternalStateVariablesSize()
      const {
    return this->b->getInternalStateVariablesSize();
  }  // end of getInternalStateVariablesSize

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getInternalStateVariablesDescriptions()
      const {
    return this->b->getInternalStateVariablesDescriptions();
  }  // end of getInternalStateVariablesDescriptions

  unsigned short
  LogarithmicStrain1DBehaviourWrapper::getInternalStateVariableType(
      const std::string& n) const {
    return this->b->getInternalStateVariableType(n);
  }  // end of getInternalStateVariableType

  unsigned short
  LogarithmicStrain1DBehaviourWrapper::getInternalStateVariablePosition(
      const std::string& n) const {
    return this->b->getInternalStateVariablePosition(n);
  }  // end of getInternalStateVariablePosition

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getExternalStateVariablesNames() const {
    return this->b->getExternalStateVariablesNames();
  }  // end of getExternalStateVariablesNames

  size_t LogarithmicStrain1DBehaviourWrapper::getExternalStateVariablesSize()
      const {
    return this->b->getExternalStateVariablesSize();
  }  // end of getExternalStateVariablesSize

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getParametersNames() const {
    return this->b->getParametersNames();
  }  // end of getParametersNames

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getIntegerParametersNames() const {
    return this->b->getIntegerParametersNames();
  }  // end of getIntegerParametersNames

  std::vector<std::string>
  LogarithmicStrain1DBehaviourWrapper::getUnsignedShortParametersNames() const {
    return this->b->getUnsignedShortParametersNames();
  }  // end of getUnsignedShortParametersNames

  double LogarithmicStrain1DBehaviourWrapper::getRealParameterDefaultValue(
      const std::string& p) const {
    return this->b->getRealParameterDefaultValue(p);
  }

  int LogarithmicStrain1DBehaviourWrapper::getIntegerParameterDefaultValue(
      const std::string& p) const {
    return this->b->getIntegerParameterDefaultValue(p);
  }

  unsigned short
  LogarithmicStrain1DBehaviourWrapper::getUnsignedShortParameterDefaultValue(
      const std::string& p) const {
    return this->b->getUnsignedShortParameterDefaultValue(p);
  }

  void LogarithmicStrain1DBehaviourWrapper::setOutOfBoundsPolicy(
      const tfel::material::OutOfBoundsPolicy p) const {
    return this->b->setOutOfBoundsPolicy(p);
  }  // end of setOutOfBoundsPolicy

  bool LogarithmicStrain1DBehaviourWrapper::hasBounds(
      const std::string& v) const {
    return this->b->hasBounds(v);
  }  // end of hasBounds

  bool LogarithmicStrain1DBehaviourWrapper::hasLowerBound(
      const std::string& v) const {
    return this->b->hasLowerBound(v);
  }  // end of hasLowerBound

  bool LogarithmicStrain1DBehaviourWrapper::hasUpperBound(
      const std::string& v) const {
    return this->b->hasUpperBound(v);
  }  // end of hasUpperBound

  long double LogarithmicStrain1DBehaviourWrapper::getLowerBound(
      const std::string& v) const {
    return this->b->getLowerBound(v);
  }  // end of getLowerBound

  long double LogarithmicStrain1DBehaviourWrapper::getUpperBound(
      const std::string& v) const {
    return this->b->getUpperBound(v);
  }  // end of getUpperBound

  bool LogarithmicStrain1DBehaviourWrapper::hasPhysicalBounds(
      const std::string& v) const {
    return this->b->hasPhysicalBounds(v);
  }  // end of hasPhysicalBounds

  bool LogarithmicStrain1DBehaviourWrapper::hasLowerPhysicalBound(
      const std::string& v) const {
    return this->b->hasLowerPhysicalBound(v);
  }  // end of hasLowerPhysicalBound

  bool LogarithmicStrain1DBehaviourWrapper::hasUpperPhysicalBound(
      const std::string& v) const {
    return this->b->hasUpperPhysicalBound(v);
  }  // end of hasUpperPhysicalBound

  long double LogarithmicStrain1DBehaviourWrapper::getLowerPhysicalBound(
      const std::string& v) const {
    return this->b->getLowerPhysicalBound(v);
  }  // end of getLowerPhysicalBound

  long double LogarithmicStrain1DBehaviourWrapper::getUpperPhysicalBound(
      const std::string& v) const {
    return this->b->getUpperPhysicalBound(v);
  }  // end of getUpperPhysicalBound

  void LogarithmicStrain1DBehaviourWrapper::setParameter(const std::string& n,
                                                         const real v) const {
    this->b->setParameter(n, v);
  }  // end of setParameter

  void LogarithmicStrain1DBehaviourWrapper::setIntegerParameter(
      const std::string& n, const int v) const {
    this->b->setIntegerParameter(n, v);
  }  // end of setIntegerParameter

  void LogarithmicStrain1DBehaviourWrapper::setUnsignedIntegerParameter(
      const std::string& n, const unsigned short v) const {
    this->b->setUnsignedIntegerParameter(n, v);
  }  // end of setUnsignedIntegerParameter

  void LogarithmicStrain1DBehaviourWrapper::allocate(
      BehaviourWorkSpace& wk) const {
    this->b->allocate(wk);
  }  // end of allocate

  StiffnessMatrixType
  LogarithmicStrain1DBehaviourWrapper::getDefaultStiffnessMatrixType() const {
    return this->b->getDefaultStiffnessMatrixType();
  }  // end of getDefaultStiffnessMatrixType

  tfel::math::tmatrix<3u, 3u, real>
  LogarithmicStrain1DBehaviourWrapper::getRotationMatrix(
      const tfel::math::vector<real>& mp,
      const tfel::math::tmatrix<3u, 3u, real>& r) const {
    return this->b->getRotationMatrix(mp, r);
  }  // end of getRotationMatrix

  bool LogarithmicStrain1DBehaviourWrapper::doPackagingStep(
      CurrentState& s, BehaviourWorkSpace& wk) const {
    return this->b->doPackagingStep(s, wk);
  }  // end of doPackagingStep

  std::pair<bool, real>
  LogarithmicStrain1DBehaviourWrapper::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    const auto eps = std::numeric_limits<real>::epsilon();
    auto cs = s;
    // logarithmic strains
    if ((1 + s.e0[0] < eps) || (1 + s.e0[1] < eps) || (1 + s.e0[2] < eps) ||
        (1 + s.e1[0] < eps) || (1 + s.e1[1] < eps) || (1 + s.e1[2] < eps) ||
        (1 + s.e_th0[0] < eps) || (1 + s.e_th0[1] < eps) ||
        (1 + s.e_th0[2] < eps) || (1 + s.e_th1[0] < eps) ||
        (1 + s.e_th1[1] < eps) || (1 + s.e_th1[2] < eps)) {
      return {false, 0.5};
    }
    cs.e0[0] = std::log1p(s.e0[0]);
    cs.e0[1] = std::log1p(s.e0[1]);
    cs.e0[2] = std::log1p(s.e0[2]);
    cs.e1[0] = cs.e0[0];
    cs.e1[1] = cs.e0[1];
    cs.e1[2] = cs.e0[2];
    cs.e_th0[0] = std::log1p(s.e_th0[0]);
    cs.e_th0[1] = std::log1p(s.e_th0[1]);
    cs.e_th0[2] = std::log1p(s.e_th0[2]);
    cs.e_th1[0] = std::log1p(s.e_th1[0]);
    cs.e_th1[1] = std::log1p(s.e_th1[1]);
    cs.e_th1[2] = std::log1p(s.e_th1[2]);
    // stresses
    cs.s0[0] = s.s0[0] * (1 + s.e0[0]);
    cs.s0[1] = s.s0[1] * (1 + s.e0[1]);
    cs.s0[2] = s.s0[2] * (1 + s.e0[2]);
    cs.s1[0] = cs.s0[0];
    cs.s1[1] = cs.s0[1];
    cs.s1[2] = cs.s0[2];
    const auto r = this->b->computePredictionOperator(wk, cs, ktype);
    if (!r.first) {
      return r;
    }
    // modify wk.kp
    if ((ktype != StiffnessMatrixType::NOSTIFFNESS) &&
        (ktype != StiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE) &&
        (ktype != StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES)) {
      convertStiffness(wk.kt, s.e0, s.s0);
    }
    return r;
  }  // end of computePredictionOperator

  std::pair<bool, real> LogarithmicStrain1DBehaviourWrapper::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    const auto eps = std::numeric_limits<real>::epsilon();
    // logarithmic strains
    if ((1 + s.e0[0] < eps) || (1 + s.e0[1] < eps) || (1 + s.e0[2] < eps) ||
        (1 + s.e1[0] < eps) || (1 + s.e1[1] < eps) || (1 + s.e1[2] < eps) ||
        (1 + s.e_th0[0] < eps) || (1 + s.e_th0[1] < eps) ||
        (1 + s.e_th0[2] < eps) || (1 + s.e_th1[0] < eps) ||
        (1 + s.e_th1[1] < eps) || (1 + s.e_th1[2] < eps)) {
      return {false, 0.5};
    }
    real e0[3];
    real e1[3];
    real e_th0[3];
    real e_th1[3];
    real s0[3];
    // logarithmic strains
    tfel::fsalgo::copy<3u>::exe(s.e0.begin(), e0);
    tfel::fsalgo::copy<3u>::exe(s.e1.begin(), e1);
    tfel::fsalgo::copy<3u>::exe(s.s0.begin(), s0);
    s.e0[0] = std::log1p(e0[0]);
    s.e0[1] = std::log1p(e0[1]);
    s.e0[2] = std::log1p(e0[2]);
    s.e1[0] = std::log1p(e1[0]);
    s.e1[1] = std::log1p(e1[1]);
    s.e1[2] = std::log1p(e1[2]);
    // stresses
    s.s0[0] = s0[0] * (1 + e0[0]);
    s.s0[1] = s0[1] * (1 + e0[1]);
    s.s0[2] = s0[2] * (1 + e0[2]);
    // thermal strain
    tfel::fsalgo::copy<3u>::exe(s.e_th0.begin(), e_th0);
    tfel::fsalgo::copy<3u>::exe(s.e_th1.begin(), e_th1);
    s.e_th0[0] = std::log1p(e_th0[0]);
    s.e_th0[1] = std::log1p(e_th0[1]);
    s.e_th0[2] = std::log1p(e_th0[2]);
    s.e_th1[0] = std::log1p(e_th1[0]);
    s.e_th1[1] = std::log1p(e_th1[1]);
    s.e_th1[2] = std::log1p(e_th1[2]);
    tfel::fsalgo::copy<3u>::exe(s.s0.begin(), s.s1.begin());
    setRoundingMode();
    const auto r = this->b->integrate(s, wk, dt, ktype);
    setRoundingMode();
    tfel::fsalgo::copy<3u>::exe(e0, s.e0.begin());
    tfel::fsalgo::copy<3u>::exe(e1, s.e1.begin());
    tfel::fsalgo::copy<3u>::exe(s0, s.s0.begin());
    tfel::fsalgo::copy<3u>::exe(e_th0, s.e_th0.begin());
    tfel::fsalgo::copy<3u>::exe(e_th1, s.e_th1.begin());
    if (!r.first) {
      tfel::fsalgo::copy<3u>::exe(s0, s.s1.begin());
      return r;
    }
    s.s1[0] /= 1 + e1[0];
    s.s1[1] /= 1 + e1[1];
    s.s1[2] /= 1 + e1[2];
    // modify wk.k
    if ((ktype != StiffnessMatrixType::NOSTIFFNESS) &&
        (ktype != StiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE) &&
        (ktype != StiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES)) {
      convertStiffness(wk.k, s.e1, s.s1);
    }
    return r;
  }  // end of integrate

  LogarithmicStrain1DBehaviourWrapper::~LogarithmicStrain1DBehaviourWrapper() =
      default;

}  // end of namespace mtest
