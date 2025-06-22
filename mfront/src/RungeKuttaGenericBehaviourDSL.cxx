/*!
 * \file  mfront/src/RungeKuttaGenericBehaviourDSL.cxx
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

#include "MFront/RungeKuttaGenericBehaviourDSL.hxx"

namespace mfront {

  RungeKuttaGenericBehaviourDSL::RungeKuttaGenericBehaviourDSL(
      const DSLOptions& opts)
      : RungeKuttaDSLBase(opts) {
    this->mb.setDSLName("RungeKuttaGenericBehaviour");
    this->mb.declareAsGenericBehaviour();
    this->registerNewCallBack("@Gradient",
                              &RungeKuttaGenericBehaviourDSL::treatGradient);
    this->registerNewCallBack(
        "@ThermodynamicForce",
        &RungeKuttaGenericBehaviourDSL::treatThermodynamicForce);
    this->registerNewCallBack(
        "@Flux", &RungeKuttaGenericBehaviourDSL::treatThermodynamicForce);
    this->registerNewCallBack(
        "@TangentOperatorBlock",
        &RungeKuttaGenericBehaviourDSL::treatTangentOperatorBlock);
    this->registerNewCallBack(
        "@TangentOperatorBlocks",
        &RungeKuttaGenericBehaviourDSL::treatTangentOperatorBlocks);
    this->registerNewCallBack(
        "@AdditionalTangentOperatorBlock",
        &RungeKuttaGenericBehaviourDSL::treatAdditionalTangentOperatorBlock);
    this->registerNewCallBack(
        "@AdditionalTangentOperatorBlocks",
        &RungeKuttaGenericBehaviourDSL::treatAdditionalTangentOperatorBlocks);
    this->registerNewCallBack(
        "@ComputeThermodynamicForces",
        &RungeKuttaGenericBehaviourDSL::treatComputeThermodynamicForces);
    this->registerNewCallBack(
        "@ComputeFinalThermodynamicForces",
        &RungeKuttaGenericBehaviourDSL::treatComputeFinalThermodynamicForces);
  }  // end of RungeKuttaGenericBehaviourDSL::RungeKuttaGenericBehaviourDSL

  std::string RungeKuttaGenericBehaviourDSL::getName() {
    return "RungeKuttaGenericBehaviourDSL";
  }  // end of RungeKuttaGenericBehaviourDSL::getName

  std::string RungeKuttaGenericBehaviourDSL::getDescription() {
    return "this dsl provides a generic integrator based on runke-kutta "
           "methods.";
  }  // end of RungeKuttaGenericBehaviourDSL::getDescription

  std::string RungeKuttaGenericBehaviourDSL::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions& o) const {
    if (c == BehaviourData::ComputeThermodynamicForces) {
      return "@ComputeThermodynamicForces{}\n";
    }
    return RungeKuttaDSLBase::getCodeBlockTemplate(c, o);
  }  // end of RungeKuttaGenericBehaviourDSL::getCodeBlockTemplate

  BehaviourDSLDescription
  RungeKuttaGenericBehaviourDSL::getBehaviourDSLDescription() const {
    auto d = mfront::getDefaultStrainBasedBehaviourDSLDescription();
    d.integrationScheme = IntegrationScheme::EXPLICITSCHEME;
    d.typicalCodeBlocks = {BehaviourData::ComputePredictionOperator,
                           BehaviourData::ComputeThermodynamicForces,
                           BehaviourData::ComputeDerivative,
                           BehaviourData::ComputeTangentOperator};
    d.minimalMFrontFileBody =
        "@Derivative{}\n\n@ComputeFinalThermodynamicForces{};\n\n";
    return d;
  }  // end of RungeKuttaGenericBehaviourDSL::getBehaviourDSLDescription

  RungeKuttaGenericBehaviourDSL::~RungeKuttaGenericBehaviourDSL() noexcept =
      default;

}  // end of namespace mfront
