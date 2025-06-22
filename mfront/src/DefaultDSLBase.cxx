/*!
 * \file   mfront/src/DefaultDSLBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22 jullet 2013
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
#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/AbstractBehaviourInterface.hxx"
#include "MFront/BehaviourInterfaceFactory.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/DefaultDSLBase.hxx"

namespace mfront {

  DefaultDSLBase::DefaultDSLBase() {
    this->mb.setIntegrationScheme(BehaviourDescription::USERDEFINEDSCHEME);
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

  std::string DefaultDSLBase::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions&) const {
    if (c == BehaviourData::ComputePredictionOperator) {
      return "@PredictionOperator{}\n";
    } else if (c == BehaviourData::Integrator) {
      return "@Integrator{}\n";
    } else if (c == BehaviourData::ComputeTangentOperator) {
      return "@TangentOperator{}\n";
    }
    return "";
  }  // end of DefaultDSLBase::getCodeBlockTemplate

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

  void DefaultDSLBase::completeVariableDeclaration() {
    BehaviourDSLCommon::completeVariableDeclaration();
    if (this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) {
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      auto D = VariableDescription("StiffnessTensor", "D", 1u, 0u);
      D.description =
          "stiffness tensor computed from elastic "
          "material properties";
      this->mb.addLocalVariable(h, D, BehaviourData::ALREADYREGISTRED);
    }
  }  // end of DefaultDSLBase::completeVariableDeclaration

  void DefaultDSLBase::writeBehaviourParserSpecificIncludes(
      std::ostream& os) const {
    BehaviourDSLCommon::writeBehaviourParserSpecificIncludes(os);
  }  // end of DefaultDSLBase::writeBehaviourParserSpecificIncludes

  void DefaultDSLBase::writeBehaviourLocalVariablesInitialisation(
      std::ostream& os, const Hypothesis h) const {
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    Modifier ets = [this](const MaterialPropertyInput& i) -> std::string {
      if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
          (i.category ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+this->d" + i.name;
      } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.category == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
        return this->mb.getClassName() + "::" + i.name;
      } else {
        tfel::raise(
            "DefaultDSLBase::writeBehaviourLocalVariablesInitialisation: "
            "unsupported input type for variable '" +
            i.name + "'");
      }
    };
    if (this->mb.getAttribute(BehaviourDescription::computesStiffnessTensor,
                              false)) {
      os << "// stiffness tensor at the end of the time step\n";
      this->writeStiffnessTensorComputation(os, "this->D", ets);
    }
    BehaviourDSLCommon::writeBehaviourLocalVariablesInitialisation(os, h);
  }

  void DefaultDSLBase::getSymbols(std::map<std::string, std::string>& symbols,
                                  const Hypothesis h,
                                  const std::string& n) {
    BehaviourDSLCommon::getSymbols(symbols, h, n);
    const auto& d = this->mb.getBehaviourData(h);
    getIncrementSymbols(symbols, d.getIntegrationVariables());
    for (const auto& mv : this->mb.getMainVariables()) {
      if (Gradient::isIncrementKnown(mv.first)) {
        getIncrementSymbol(symbols, mv.first);
      } else {
        mfront::addSymbol(symbols, displayName(mv.first) + "\u2080",
                          mv.first.name + "0");
        mfront::addSymbol(symbols, displayName(mv.first) + "\u2081",
                          mv.first.name + "1");
      }
    }
    mfront::getIncrementSymbols(symbols, d.getExternalStateVariables());
    mfront::addSymbol(symbols, "\u0394t", "dt");
  }  // end of DefaultDSLBase::getSymbols

  DefaultDSLBase::~DefaultDSLBase() = default;

}  // end of namespace mfront
