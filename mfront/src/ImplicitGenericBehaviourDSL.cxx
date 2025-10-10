/*!
 * \file  mfront/src/ImplicitGenericBehaviourDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 févr. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/ImplicitGenericBehaviourDSL.hxx"

namespace mfront {

  ImplicitGenericBehaviourDSL::ImplicitGenericBehaviourDSL(
      const DSLOptions& opts)
      : ImplicitDSLBase(opts) {
    this->mb.setDSLName("ImplicitGenericBehaviour");
    this->mb.declareAsGenericBehaviour();
    this->registerNewCallBack("@Gradient",
                              &ImplicitGenericBehaviourDSL::treatGradient);
    this->registerNewCallBack(
        "@ThermodynamicForce",
        &ImplicitGenericBehaviourDSL::treatThermodynamicForce);
    this->registerNewCallBack(
        "@Flux", &ImplicitGenericBehaviourDSL::treatThermodynamicForce);
    this->registerNewCallBack(
        "@TangentOperatorBlock",
        &ImplicitGenericBehaviourDSL::treatTangentOperatorBlock);
    this->registerNewCallBack(
        "@TangentOperatorBlocks",
        &ImplicitGenericBehaviourDSL::treatTangentOperatorBlocks);
    this->registerNewCallBack(
        "@AdditionalTangentOperatorBlock",
        &ImplicitGenericBehaviourDSL::treatAdditionalTangentOperatorBlock);
    this->registerNewCallBack(
        "@AdditionalTangentOperatorBlocks",
        &ImplicitGenericBehaviourDSL::treatAdditionalTangentOperatorBlocks);
    this->registerNewCallBack(
        "@ComputeThermodynamicForces",
        &ImplicitGenericBehaviourDSL::treatComputeThermodynamicForces);
    this->registerNewCallBack(
        "@ComputeFinalThermodynamicForces",
        &ImplicitGenericBehaviourDSL::treatComputeFinalThermodynamicForces);
  }  // end of ImplicitGenericBehaviourDSL::ImplicitGenericBehaviourDSL

  std::string ImplicitGenericBehaviourDSL::getName() {
    return "ImplicitGenericBehaviourDSL";
  }  // end of ImplicitGenericBehaviourDSL::getName

  std::string ImplicitGenericBehaviourDSL::getDescription() {
    return "this dsl provides a generic integrator based on a theta method.";
  }  // end of ImplicitGenericBehaviourDSL::getDescription

  std::string ImplicitGenericBehaviourDSL::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions& o) const {
    if (c == BehaviourData::ComputeThermodynamicForces) {
      return "@ComputeThermodynamicForces{}\n";
    }
    return ImplicitDSLBase::getCodeBlockTemplate(c, o);
  }  // end of ImplicitGenericBehaviourDSL::getCodeBlockTemplate

  BehaviourDSLDescription
  ImplicitGenericBehaviourDSL::getBehaviourDSLDescription() const {
    auto d = BehaviourDSLDescription();
    d.integrationScheme = IntegrationScheme::IMPLICITSCHEME;
    d.typicalCodeBlocks = {BehaviourData::ComputePredictionOperator,
                           BehaviourData::ComputePredictor,
                           BehaviourData::ComputeThermodynamicForces,
                           BehaviourData::Integrator,
                           BehaviourData::ComputeTangentOperator};
    d.minimalMFrontFileBody = "@Integrator{}\n\n";
    return d;
  }  // end of ImplicitGenericBehaviourDSL::getBehaviourDSLDescription

  ImplicitGenericBehaviourDSL::~ImplicitGenericBehaviourDSL() noexcept =
      default;

}  // end of namespace mfront
