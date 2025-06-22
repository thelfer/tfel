/*!
 * \file  mfront/src/ImplicitFiniteStrainDSL.cxx
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

#include "MFront/ImplicitFiniteStrainDSL.hxx"

namespace mfront {

  ImplicitFiniteStrainDSL::ImplicitFiniteStrainDSL() {
    this->mb.setDSLName("ImplicitFiniteStrain");
    this->mb.declareAsAFiniteStrainStandardBehaviour(false);
    this->registerNewCallBack(
        "@ComputeStress",
        &ImplicitFiniteStrainDSL::treatComputeThermodynamicForces);
    this->registerNewCallBack(
        "@ComputeFinalStress",
        &ImplicitFiniteStrainDSL::treatComputeFinalThermodynamicForces);
  }  // end of ImplicitFiniteStrainDSL::ImplicitFiniteStrainDSL

  std::string ImplicitFiniteStrainDSL::getName() {
    return "ImplicitFiniteStrain";
  }  // end of ImplicitFiniteStrainDSL::getName

  std::string ImplicitFiniteStrainDSL::getDescription() {
    return "this parser provides a generic integrator based on a theta method.";
  }  // end of ImplicitFiniteStrainDSL::getDescription

  std::string ImplicitFiniteStrainDSL::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions& o) const {
    if (c == BehaviourData::ComputeThermodynamicForces) {
      return "@ComputeStress{}\n";
    }
    return ImplicitDSLBase::getCodeBlockTemplate(c, o);
  }  // end of ImplicitFiniteStrainDSL::getCodeBlockTemplate

  BehaviourDSLDescription ImplicitFiniteStrainDSL::getBehaviourDSLDescription()
      const {
    auto d = mfront::getDefaultFiniteStrainBehaviourDSLDescription();
    d.integrationScheme = IntegrationScheme::IMPLICITSCHEME;
    d.typicalCodeBlocks = {BehaviourData::ComputePredictionOperator,
                           BehaviourData::ComputePredictor,
                           BehaviourData::ComputeThermodynamicForces,
                           BehaviourData::Integrator,
                           BehaviourData::ComputeTangentOperator};
    d.minimalMFrontFileBody = "@Integrator{}\n\n";
    return d;
  }  // end of ImplicitFiniteStrainDSL::getBehaviourDSLDescription

  ImplicitFiniteStrainDSL::~ImplicitFiniteStrainDSL() noexcept = default;

}  // end of namespace mfront
