/*!
 * \file   mfront/src/MFrontBehaviourInterface.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   15/01/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/BehaviourDescription.hxx"
#include "MFront/MFrontBehaviourInterface.hxx"

namespace mfront {

  std::string MFrontBehaviourInterface::getName() { return "mfront"; }

  std::string MFrontBehaviourInterface::getInterfaceName() const {
    return "MFront";
  }  // end of getInterfaceName

  std::string MFrontBehaviourInterface::getInterfaceVersion() const {
    return "";
  }  // end of getInterfaceVersion

  std::pair<bool, MFrontBehaviourInterface::tokens_iterator>
  MFrontBehaviourInterface::treatKeyword(BehaviourDescription &,
                                         const std::string &k,
                                         const std::vector<std::string> &i,
                                         tokens_iterator current,
                                         const tokens_iterator) {
    tfel::raise_if(std::find(i.begin(), i.end(), "mfront") != i.end(),
                   "MFrontMaterialPropertyInterface::treatKeyword: "
                   "unsupported key '" +
                       k + "'");
    return {false, current};
  }  // end of treatKeyword

  bool MFrontBehaviourInterface::isBehaviourConstructorRequired(
      const Hypothesis, const BehaviourDescription &) const {
    return false;
  }  // end of isBehaviourConstructorRequired

  std::set<MFrontBehaviourInterface::Hypothesis>
  MFrontBehaviourInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription &bd) const {
    return bd.getModellingHypotheses();
  }  // end of getModellingHypothesesToBeTreated

  std::pair<std::vector<BehaviourMaterialProperty>, SupportedTypes::TypeSize>
  MFrontBehaviourInterface::buildMaterialPropertiesList(
      const BehaviourDescription &bd, const Hypothesis h) const {
    const auto opts = BuildMaterialPropertiesListOptions{
        .useMaterialPropertiesToBuildStiffnessTensor = true,
        .useMaterialPropertiesToBuildThermalExpansionCoefficientTensor = true};
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      return mfront::buildMaterialPropertiesList(
          bd, this->getModellingHypothesesToBeTreated(bd), opts);
    }
    return mfront::buildMaterialPropertiesList(bd, h, opts);
  }  // end of buildMaterialPropertiesList

  void MFrontBehaviourInterface::writeInterfaceSpecificIncludes(
      std::ostream &, const BehaviourDescription &) const {
  }  // end of writeInterfaceSpecificIncludes

  void MFrontBehaviourInterface::exportMechanicalData(
      std::ostream &, const Hypothesis, const BehaviourDescription &) const {
  }  // end of exportMechanicalData

  void MFrontBehaviourInterface::writeBehaviourConstructorHeader(
      std::ostream &,
      const BehaviourDescription &,
      const Hypothesis,
      const std::string &) const {}  // end of writeBehaviourConstructorHeader

  void MFrontBehaviourInterface::writeBehaviourConstructorBody(
      std::ostream &, const BehaviourDescription &, const Hypothesis) const {
  }  // end of writeBehaviourConstructorBody

  void MFrontBehaviourInterface::writeBehaviourDataConstructor(
      std::ostream &, const Hypothesis, const BehaviourDescription &) const {
  }  // end of writeBehaviourDataConstructor

  void MFrontBehaviourInterface::writeBehaviourDataMainVariablesSetters(
      std::ostream &, const BehaviourDescription &) const {
  }  // end of writeBehaviourDataMainVariablesSetters

  void MFrontBehaviourInterface::writeIntegrationDataConstructor(
      std::ostream &, const Hypothesis, const BehaviourDescription &) const {
  }  // end of writeIntegrationDataConstructor

  void MFrontBehaviourInterface::writeIntegrationDataMainVariablesSetters(
      std::ostream &, const BehaviourDescription &) const {
  }  // end of writeIntegrationDataMainVariablesSetters

  void MFrontBehaviourInterface::writeBehaviourInitializeFunctions(
      std::ostream &, const BehaviourDescription &, const Hypothesis) const {
  }  // end of writeBehaviourInitializeFunctions

  void MFrontBehaviourInterface::writeBehaviourPostProcessings(
      std::ostream &, const BehaviourDescription &, const Hypothesis) const {
  }  // end of writeBehaviourPostProcessings

  void MFrontBehaviourInterface::endTreatment(const BehaviourDescription &,
                                              const FileDescription &) const {
  }  // end of endTreatment

  void MFrontBehaviourInterface::getTargetsDescription(
      TargetsDescription &, const BehaviourDescription &) {
  }  // end of getTargetsDescription

  MFrontBehaviourInterface::~MFrontBehaviourInterface() = default;

}  // end of namespace mfront
