/*!
 * \file  mfront/src/ImplicitDSLII.cxx
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

#include "MFront/ImplicitDSLII.hxx"

namespace mfront {

  ImplicitDSLII::ImplicitDSLII(const DSLOptions& opts) : ImplicitDSLBase(opts) {
    this->mb.declareAsASmallStrainStandardBehaviour();
    this->registerNewCallBack("@ComputeStress",
                              &ImplicitDSLII::treatComputeThermodynamicForces);
    this->registerNewCallBack(
        "@ComputeFinalStress",
        &ImplicitDSLII::treatComputeFinalThermodynamicForces);
  }  // end of ImplicitDSLII::ImplicitDSLII

  std::string ImplicitDSLII::getName() {
    return "ImplicitII";
  }  // end of ImplicitDSLII::getName

  std::string ImplicitDSLII::getDescription() {
    return "this parser provides a generic integrator based on a theta method.";
  }  // end of ImplicitDSLII::getDescription

  std::string ImplicitDSLII::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions& o) const {
    if (c == BehaviourData::ComputeThermodynamicForces) {
      return "@ComputeStress{}\n";
    }
    return ImplicitDSLBase::getCodeBlockTemplate(c, o);
  }  // end of ImplicitDSLII::getCodeBlockTemplate

  BehaviourDSLDescription ImplicitDSLII::getBehaviourDSLDescription() const {
    auto d = mfront::getDefaultStrainBasedBehaviourDSLDescription();
    d.integrationScheme = IntegrationScheme::IMPLICITSCHEME;
    d.typicalCodeBlocks = {BehaviourData::ComputePredictionOperator,
                           BehaviourData::ComputePredictor,
                           BehaviourData::ComputeThermodynamicForces,
                           BehaviourData::Integrator,
                           BehaviourData::ComputeTangentOperator};
    d.minimalMFrontFileBody = "@Integrator{}\n\n";
    return d;
  }  // end of ImplicitDSLII::getBehaviourDSLDescription

  ImplicitDSLII::~ImplicitDSLII() = default;

}  // end of namespace mfront
