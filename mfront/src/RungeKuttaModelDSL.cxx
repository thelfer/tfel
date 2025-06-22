/*!
 * \file  mfront/src/RungeKuttaModelDSL.cxx
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

#include "MFront/RungeKuttaModelDSL.hxx"

namespace mfront {

  RungeKuttaModelDSL::RungeKuttaModelDSL() {
    this->mb.setDSLName("RungeKuttaModel");
    this->mb.declareAsGenericBehaviour();
    this->disableCallBack("@Behaviour");
    this->registerNewCallBack("@Model", &RungeKuttaModelDSL::treatModel2, true);
  }  // end of RungeKuttaModelDSL

  std::string RungeKuttaModelDSL::getName() {
    return "RungeKuttaModelDSL";
  }  // end of getName

  std::string RungeKuttaModelDSL::getDescription() {
    return "this dsl provides a model integrator based on runke-kutta "
           "methods.";
  }  // end of getDescription

  BehaviourDSLDescription RungeKuttaModelDSL::getBehaviourDSLDescription()
      const {
    auto d = mfront::getDefaultStrainBasedBehaviourDSLDescription();
    d.integrationScheme = IntegrationScheme::EXPLICITSCHEME;
    d.typicalCodeBlocks = {BehaviourData::ComputeDerivative};
    d.minimalMFrontFileBody = "@Derivative{}\n\n";
    return d;
  }  // end of getBehaviourDSLDescription

  RungeKuttaModelDSL::~RungeKuttaModelDSL() noexcept = default;

}  // end of namespace mfront
