/*!
 * \file   mfront/src/DefaultGenericBehaviourDSL.cxx
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
#include "MFront/DefaultGenericBehaviourDSL.hxx"

namespace mfront {

  DefaultGenericBehaviourDSL::DefaultGenericBehaviourDSL(const DSLOptions& opts)
      : DefaultDSLBase(opts) {
    this->mb.setDSLName("DefaultGenericBehaviour");
    this->mb.declareAsGenericBehaviour();
    this->registerNewCallBack("@Gradient",
                              &DefaultGenericBehaviourDSL::treatGradient);
    this->registerNewCallBack(
        "@ThermodynamicForce",
        &DefaultGenericBehaviourDSL::treatThermodynamicForce);
    this->registerNewCallBack(
        "@Flux", &DefaultGenericBehaviourDSL::treatThermodynamicForce);
    this->registerNewCallBack(
        "@TangentOperatorBlock",
        &DefaultGenericBehaviourDSL::treatTangentOperatorBlock);
    this->registerNewCallBack(
        "@TangentOperatorBlocks",
        &DefaultGenericBehaviourDSL::treatTangentOperatorBlocks);
    this->registerNewCallBack(
        "@AdditionalTangentOperatorBlock",
        &DefaultGenericBehaviourDSL::treatAdditionalTangentOperatorBlock);
    this->registerNewCallBack(
        "@AdditionalTangentOperatorBlocks",
        &DefaultGenericBehaviourDSL::treatAdditionalTangentOperatorBlocks);
  }  // end of DefaultGenericBehaviourDSL::DefaultGenericBehaviourDSL

  std::string DefaultGenericBehaviourDSL::getDescription() {
    return "this parser is the most generic one as it does not make any "
           "restriction "
           "on the behaviour or the integration method that may be used";
  }  // end of DefaultGenericBehaviourDSL::getDescription

  std::string DefaultGenericBehaviourDSL::getName() {
    return "DefaultGenericBehaviourDSL";
  }  // end of DefaultGenericBehaviourDSL::getName()

  BehaviourDSLDescription
  DefaultGenericBehaviourDSL::getBehaviourDSLDescription() const {
    auto d = BehaviourDSLDescription{};
    d.behaviourType = tfel::material::MechanicalBehaviourBase::GENERALBEHAVIOUR;
    d.integrationScheme = IntegrationScheme::USERDEFINEDSCHEME;
    d.typicalCodeBlocks = {BehaviourData::ComputePredictionOperator,
                           BehaviourData::Integrator,
                           BehaviourData::ComputeTangentOperator};
    d.minimalMFrontFileBody = "@Integrator{}\n\n";
    return d;
  }  // end of DefaultGenericBehaviourDSL::getBehaviourDSLDescription

  DefaultGenericBehaviourDSL::~DefaultGenericBehaviourDSL() = default;

}  // end of namespace mfront
