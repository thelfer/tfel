/*!
 * \file  mfront/src/ImplicitModelDSL.cxx
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

#include "MFront/ImplicitModelDSL.hxx"

namespace mfront {

  ImplicitModelDSL::ImplicitModelDSL() {
    this->mb.setDSLName("ImplicitModel");
    this->mb.declareAsGenericBehaviour();
    this->disableCallBack("@Behaviour");
    this->registerNewCallBack("@Model", &ImplicitModelDSL::treatModel2, true);
  }  // end of ImplicitModelDSL::ImplicitModelDSL

  std::string ImplicitModelDSL::getName() {
    return "ImplicitModelDSL";
  }  // end of ImplicitModelDSL::getName

  std::string ImplicitModelDSL::getDescription() {
    return "this dsl provides an integrator for models based on a theta "
           "method.";
  }  // end of ImplicitModelDSL::getDescription

  BehaviourDSLDescription ImplicitModelDSL::getBehaviourDSLDescription() const {
    auto d = BehaviourDSLDescription();
    d.integrationScheme = IntegrationScheme::IMPLICITSCHEME;
    d.typicalCodeBlocks = {BehaviourData::Integrator};
    d.minimalMFrontFileBody = "@Integrator{}\n\n";
    return d;
  }  // end of ImplicitModelDSL::getBehaviourDSLDescription

  ImplicitModelDSL::~ImplicitModelDSL() noexcept = default;

}  // end of namespace mfront
