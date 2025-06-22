/*!
 * \file  mfront/src/ImplicitDSL.cxx
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

#include "MFront/ImplicitDSL.hxx"

namespace mfront {

  ImplicitDSL::ImplicitDSL() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setDSLName("Implicit");
    // input variables
    this->mb.declareAsASmallStrainStandardBehaviour();
    // Default state variable
    VariableDescription eel("StrainStensor", "εᵉˡ", "eel", 1u, 0u);
    eel.description = "elastic strain";
    this->mb.addStateVariable(h, eel);
    this->mb.setGlossaryName(h, "eel", "ElasticStrain");
    this->registerNewCallBack("@ComputeStress",
                              &ImplicitDSL::treatComputeThermodynamicForces);
    this->registerNewCallBack(
        "@ComputeFinalStress",
        &ImplicitDSL::treatComputeFinalThermodynamicForces);
  }  // end of ImplicitDSL::ImplicitDSL

  std::string ImplicitDSL::getName() {
    return "Implicit";
  }  // end of ImplicitDSL::getName

  std::string ImplicitDSL::getDescription() {
    return "this parser provides a generic integrator based on a theta method.";
  }  // end of ImplicitDSL::getDescription

  std::string ImplicitDSL::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions& o) const {
    if (c == BehaviourData::ComputeThermodynamicForces) {
      return "@ComputeStress{}\n";
    }
    return ImplicitDSLBase::getCodeBlockTemplate(c, o);
  }  // end of ImplicitDSL::getCodeBlockTemplate

  BehaviourDSLDescription ImplicitDSL::getBehaviourDSLDescription() const {
    auto d = mfront::getDefaultStrainBasedBehaviourDSLDescription();
    d.integrationScheme = IntegrationScheme::IMPLICITSCHEME;
    d.typicalCodeBlocks = {BehaviourData::ComputePredictionOperator,
                           BehaviourData::ComputePredictor,
                           BehaviourData::ComputeThermodynamicForces,
                           BehaviourData::Integrator,
                           BehaviourData::ComputeTangentOperator};
    d.minimalMFrontFileBody = "@Integrator{}\n\n";
    return d;
  }  // end of ImplicitDSL::getBehaviourDSLDescription

  ImplicitDSL::~ImplicitDSL() = default;

}  // end of namespace mfront
