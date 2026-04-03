/*!
 * \file   mtest/src/BehaviourWrapperBase.cxx
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

#include "TFEL/Raise.hxx"
#include "MTest/BehaviourWrapperBase.hxx"

namespace mtest {

  BehaviourWrapperBase::BehaviourWrapperBase(
      const std::shared_ptr<Behaviour>& wb)
      : b(wb) {
    if (b == nullptr) {
      tfel::raise(
          "BehaviourWrapperBase::BehaviourWrapperBase: "
          "invalid behaviour");
    }
  }  // end of BehaviourWrapperBase

  void BehaviourWrapperBase::allocateCurrentState(CurrentState& s) const {
    this->b->allocateCurrentState(s);
  }  // end of allocateCurrentState

  std::string BehaviourWrapperBase::getBehaviourName() const {
    return this->b->getBehaviourName();
  }  // end of getBehaviourName

  unsigned short BehaviourWrapperBase::getSymmetryType() const {
    return this->b->getSymmetryType();
  }  // end of getSymmetryType

  StiffnessMatrixType BehaviourWrapperBase::getDefaultStiffnessMatrixType()
      const {
    return this->b->getDefaultStiffnessMatrixType();
  }  // end of getDefaultStiffnessMatrixType

  size_t BehaviourWrapperBase::getMaterialPropertiesSize() const {
    return this->b->getMaterialPropertiesSize();
  }  // end of getMaterialPropertiesSize

  std::vector<std::string> BehaviourWrapperBase::getMaterialPropertiesNames()
      const {
    return this->b->getMaterialPropertiesNames();
  }  // end of getMaterialPropertiesNames

  std::vector<std::string> BehaviourWrapperBase::getOptionalMaterialProperties()
      const {
    return {};
  }  // end of getOptionalMaterialProperties()

  void BehaviourWrapperBase::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager& mp, const EvolutionManager& evm) const {
    return this->b->setOptionalMaterialPropertiesDefaultValues(mp, evm);
  }  // end of setOptionalMaterialPropertiesDefaultValues

  std::vector<std::string> BehaviourWrapperBase::getParametersNames() const {
    return this->b->getParametersNames();
  }  // end of getParametersNames

  std::vector<std::string> BehaviourWrapperBase::getIntegerParametersNames()
      const {
    return this->b->getIntegerParametersNames();
  }  // end of getIntegerParametersNames

  std::vector<std::string>
  BehaviourWrapperBase::getUnsignedShortParametersNames() const {
    return this->b->getUnsignedShortParametersNames();
  }  // end of getUnsignedShortParametersNames

  double BehaviourWrapperBase::getRealParameterDefaultValue(
      const std::string& p) const {
    return this->b->getRealParameterDefaultValue(p);
  }

  int BehaviourWrapperBase::getIntegerParameterDefaultValue(
      const std::string& p) const {
    return this->b->getIntegerParameterDefaultValue(p);
  }

  unsigned short BehaviourWrapperBase::getUnsignedShortParameterDefaultValue(
      const std::string& p) const {
    return this->b->getUnsignedShortParameterDefaultValue(p);
  }

  void BehaviourWrapperBase::setOutOfBoundsPolicy(
      const tfel::material::OutOfBoundsPolicy p) const {
    return this->b->setOutOfBoundsPolicy(p);
  }  // end of setOutOfBoundsPolicy

  bool BehaviourWrapperBase::hasBounds(const std::string& v) const {
    return this->b->hasBounds(v);
  }  // end of hasBounds

  bool BehaviourWrapperBase::hasLowerBound(const std::string& v) const {
    return this->b->hasLowerBound(v);
  }  // end of hasLowerBound

  bool BehaviourWrapperBase::hasUpperBound(const std::string& v) const {
    return this->b->hasUpperBound(v);
  }  // end of hasUpperBound

  long double BehaviourWrapperBase::getLowerBound(const std::string& v) const {
    return this->b->getLowerBound(v);
  }  // end of getLowerBound

  long double BehaviourWrapperBase::getUpperBound(const std::string& v) const {
    return this->b->getUpperBound(v);
  }  // end of getUpperBound

  bool BehaviourWrapperBase::hasPhysicalBounds(const std::string& v) const {
    return this->b->hasPhysicalBounds(v);
  }  // end of hasPhysicalBounds

  bool BehaviourWrapperBase::hasLowerPhysicalBound(const std::string& v) const {
    return this->b->hasLowerPhysicalBound(v);
  }  // end of hasLowerPhysicalBound

  bool BehaviourWrapperBase::hasUpperPhysicalBound(const std::string& v) const {
    return this->b->hasUpperPhysicalBound(v);
  }  // end of hasUpperPhysicalBound

  long double BehaviourWrapperBase::getLowerPhysicalBound(
      const std::string& v) const {
    return this->b->getLowerPhysicalBound(v);
  }  // end of getLowerPhysicalBound

  long double BehaviourWrapperBase::getUpperPhysicalBound(
      const std::string& v) const {
    return this->b->getUpperPhysicalBound(v);
  }  // end of getUpperPhysicalBound

  void BehaviourWrapperBase::setParameter(const std::string& n,
                                          const real v) const {
    this->b->setParameter(n, v);
  }  // end of setParameter

  void BehaviourWrapperBase::setIntegerParameter(const std::string& n,
                                                 const int v) const {
    this->b->setIntegerParameter(n, v);
  }  // end of setIntegerParameter

  void BehaviourWrapperBase::setUnsignedIntegerParameter(
      const std::string& n, const unsigned short v) const {
    this->b->setUnsignedIntegerParameter(n, v);
  }  // end of setUnsignedIntegerParameter

  BehaviourWrapperBase::~BehaviourWrapperBase() = default;

}  // end of namespace mtest
