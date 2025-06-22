/*!
 * \file   mfront/src/DefaultModelDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <fstream>
#include <stdexcept>

#include "MFront/AbstractBehaviourInterface.hxx"
#include "MFront/BehaviourInterfaceFactory.hxx"
#include "MFront/DefaultModelDSL.hxx"

namespace mfront {

  DefaultModelDSL::DefaultModelDSL() {
    this->mb.setDSLName("DefaultModel");
    this->mb.declareAsGenericBehaviour();
    this->disableCallBack("@Behaviour");
    this->registerNewCallBack("@Model", &DefaultModelDSL::treatModel2, true);
  }  // end of DefaultModelDSL::DefaultModelDSL

  std::string DefaultModelDSL::getDescription() {
    return "this domain specific language is the most generic one as it does "
           "not make any restriction on the model or the integration method "
           "that may be used";
  }  // end of DefaultModelDSL::getDescription

  std::string DefaultModelDSL::getName() {
    return "DefaultModelDSL";
  }  // end of DefaultModelDSL::getName()

  BehaviourDSLDescription DefaultModelDSL::getBehaviourDSLDescription() const {
    auto d = BehaviourDSLDescription{};
    d.behaviourType = tfel::material::MechanicalBehaviourBase::GENERALBEHAVIOUR;
    d.integrationScheme = IntegrationScheme::USERDEFINEDSCHEME;
    d.typicalCodeBlocks = {BehaviourData::Integrator};
    d.minimalMFrontFileBody = "@Integrator{}\n\n";
    return d;
  }  // end of DefaultModelDSL::getBehaviourDSLDescription

  DefaultModelDSL::~DefaultModelDSL() = default;

}  // end of namespace mfront
