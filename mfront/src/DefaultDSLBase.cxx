/*!
 * \file   mfront/src/DefaultDSLBase.cxx
 * \brief
 * \author Helfer Thomas
 * \date   22 jullet 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>

#include <string>
#include <fstream>
#include <stdexcept>

#include "MFront/AbstractBehaviourInterface.hxx"
#include "MFront/BehaviourInterfaceFactory.hxx"
#include "MFront/DefaultDSLBase.hxx"

namespace mfront {

  DefaultDSLBase::DefaultDSLBase() : BehaviourDSLBase<DefaultDSLBase>() {
    this->registerNewCallBack("@ProvidesTangentOperator",
                              &DefaultDSLBase::treatProvidesTangentOperator);
    this->registerNewCallBack(
        "@ProvidesSymmetricTangentOperator",
        &DefaultDSLBase::treatProvidesSymmetricTangentOperator);
    this->registerNewCallBack("@MaterialLaw",
                              &DefaultDSLBase::treatMaterialLaw);
    this->registerNewCallBack("@TangentOperator",
                              &DefaultDSLBase::treatTangentOperator);
    this->registerNewCallBack("@IsTangentOperatorSymmetric",
                              &DefaultDSLBase::treatIsTangentOperatorSymmetric);
    this->registerNewCallBack("@ComputeStiffnessTensor",
                              &DefaultDSLBase::treatComputeStiffnessTensor);
  }

  void DefaultDSLBase::writeBehaviourParserSpecificIncludes() {
    BehaviourDSLCommon::writeBehaviourParserSpecificIncludes();
  }  // end of DefaultDSLBase::writeBehaviourParserSpecificIncludes

  void DefaultDSLBase::endsInputFileProcessing() {
    BehaviourDSLCommon::endsInputFileProcessing();
    if (this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) {
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      auto D = VariableDescription("StiffnessTensor", "D", 1u, 0u);
      D.description =
          "stiffness tensor computed from elastic "
          "material properties";
      this->mb.addLocalVariable(h, D, BehaviourData::ALREADYREGISTRED);
    }
  }  // end of DefaultDSLBase::endsInputFileProcessing()

  void DefaultDSLBase::writeBehaviourLocalVariablesInitialisation(
      const Hypothesis h) {
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    Modifier ets = [](const MaterialPropertyInput& i) -> std::string {
      if ((i.type == MaterialPropertyInput::TEMPERATURE) ||
          (i.type ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.type == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+this->d" + i.name;
      } else if ((i.type == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.type == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else {
        throw(std::runtime_error(
            "DefaultDSLBase::writeBehaviourLocalVariablesInitialisation: "
            "unsupported input type for variable '" +
            i.name + "'"));
      }
    };
    if (this->mb.getAttribute(BehaviourDescription::computesStiffnessTensor,
                              false)) {
      this->behaviourFile
          << "// stiffness tensor at the end of the time step\n";
      this->writeStiffnessTensorComputation(this->behaviourFile, "this->D",
                                            ets);
    }
    BehaviourDSLCommon::writeBehaviourLocalVariablesInitialisation(h);
  }

  void DefaultDSLBase::treatProvidesTangentOperator() {
    std::set<Hypothesis> h;
    this->readHypothesesList(h);
    this->readSpecifiedToken(
        "BehaviourDSLBaseCommon::"
        "treatProvidesTangentOperator",
        ";");
    for (const auto& elem : h) {
      if (this->mb.hasAttribute(elem,
                                BehaviourData::hasConsistentTangentOperator)) {
        this->throwRuntimeError("DefaultDSLBase::treatProvidesTangentOperator",
                                "one of the key @ProvidesTangentOperator "
                                "or @ProvidesSymmetricTangentOperator "
                                "has already been called");
      }
      this->mb.setAttribute(elem, BehaviourData::hasConsistentTangentOperator,
                            true);
    }
  }  // end of DefaultDSLBase::treatProvidesTangentOperator

  void DefaultDSLBase::treatProvidesSymmetricTangentOperator() {
    std::set<Hypothesis> h;
    this->readHypothesesList(h);
    this->checkNotEndOfFile(
        "BehaviourDSLBaseCommon::"
        "treatProvidesSymmetricTangentOperator");
    this->readSpecifiedToken(
        "BehaviourDSLBaseCommon::"
        "treatProvidesSymmetricTangentOperator",
        ";");
    for (const auto& elem : h) {
      if (this->mb.hasAttribute(elem,
                                BehaviourData::hasConsistentTangentOperator)) {
        this->throwRuntimeError(
            "DefaultDSLBase::treatProvidesSymmetricTangentOperator",
            "one of the key @ProvidesTangentOperator "
            "or @ProvidesSymmetricTangentOperator "
            "has already been called");
      }
      this->mb.setAttribute(elem, BehaviourData::hasConsistentTangentOperator,
                            true);
      this->mb.setAttribute(
          elem, BehaviourData::isConsistentTangentOperatorSymmetric, true);
    }
  }  // end of DefaultDSLBase::treatProvidesSymmetricTangentOperator

  DefaultDSLBase::~DefaultDSLBase() = default;

}  // end of namespace mfront
