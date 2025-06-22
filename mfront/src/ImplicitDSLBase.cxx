/*!
 * \file   mfront/src/ImplicitDSLBase.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   21 sep 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <limits>
#include <cstdlib>
#include <sstream>
#include <iomanip>

#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/NonLinearSystemSolverBase.hxx"
#include "MFront/NonLinearSystemSolverFactory.hxx"
#include "MFront/PerformanceProfiling.hxx"
#include "MFront/ImplicitDSLBase.hxx"

namespace mfront {

  ImplicitDSLBase::ImplicitDSLBase() : BehaviourDSLBase<ImplicitDSLBase>() {
    // dynamically allocated vectors are not yet allowed in implicit
    // parsers
    this->areDynamicallyAllocatedVectorsAllowed_ = false;
    // parameters
    this->reserveName("epsilon");
    this->reserveName("theta");
    this->reserveName("iterMax");
    this->reserveName("numerical_jacobian_epsilon");
    this->reserveName("maximum_increment_value_per_iteration");
    this->reserveName("jacobianComparisonCriterion");
    // additional reserve name
    this->reserveName("vect_e");
    this->reserveName("zeros");
    this->reserveName("tzeros");
    this->reserveName("zeros_1");
    this->reserveName("fzeros");
    this->reserveName("jacobian");
    this->reserveName("njacobian");
    this->reserveName("partial_jacobian");
    this->reserveName("t");
    this->reserveName("error_1");
    this->reserveName("error_p");
    this->reserveName("idx");
    this->reserveName("idx2");
    this->reserveName("idx3");
    this->mb.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                                "computeNumericalJacobian");
    this->reserveName("TinyMatrixSolve");
    // CallBacks
    this->registerNewCallBack(
        "@UsableInPurelyImplicitResolution",
        &ImplicitDSLBase::treatUsableInPurelyImplicitResolution);
    this->registerNewCallBack("@MaterialLaw",
                              &ImplicitDSLBase::treatMaterialLaw);
    this->registerNewCallBack("@ComputeStress",
                              &ImplicitDSLBase::treatComputeStress);
    this->registerNewCallBack("@ComputeFinalStress",
                              &ImplicitDSLBase::treatComputeFinalStress);
    this->registerNewCallBack("@Predictor", &ImplicitDSLBase::treatPredictor);
    this->registerNewCallBack("@Theta", &ImplicitDSLBase::treatTheta);
    this->registerNewCallBack("@Epsilon", &ImplicitDSLBase::treatEpsilon);
    this->registerNewCallBack(
        "@PerturbationValueForNumericalJacobianComputation",
        &ImplicitDSLBase::
            treatPerturbationValueForNumericalJacobianComputation);
    this->registerNewCallBack("@IterMax", &ImplicitDSLBase::treatIterMax);
    this->registerNewCallBack("@MaximumNumberOfIterations",
                              &ImplicitDSLBase::treatIterMax);
    this->registerNewCallBack("@Algorithm", &ImplicitDSLBase::treatAlgorithm);
    this->registerNewCallBack("@TangentOperator",
                              &ImplicitDSLBase::treatTangentOperator);
    this->registerNewCallBack(
        "@IsTangentOperatorSymmetric",
        &ImplicitDSLBase::treatIsTangentOperatorSymmetric);
    this->registerNewCallBack("@InitJacobian",
                              &ImplicitDSLBase::treatInitJacobian);
    this->registerNewCallBack("@InitializeJacobian",
                              &ImplicitDSLBase::treatInitJacobian);
    this->registerNewCallBack("@InitJacobianInvert",
                              &ImplicitDSLBase::treatInitJacobianInvert);
    this->registerNewCallBack("@InitializeJacobianInvert",
                              &ImplicitDSLBase::treatInitJacobianInvert);
    this->registerNewCallBack(
        "@CompareToNumericalJacobian",
        &ImplicitDSLBase::treatCompareToNumericalJacobian);
    this->registerNewCallBack(
        "@JacobianComparisonCriterion",
        &ImplicitDSLBase::treatJacobianComparisonCriterion);
    this->registerNewCallBack(
        "@JacobianComparisonCriterium",
        &ImplicitDSLBase::treatJacobianComparisonCriterion);
    this->registerNewCallBack("@RequireStiffnessTensor",
                              &ImplicitDSLBase::treatRequireStiffnessOperator);
    this->registerNewCallBack(
        "@MaximumIncrementValuePerIteration",
        &ImplicitDSLBase::treatMaximumIncrementValuePerIteration);
    this->registerNewCallBack("@IntegrationVariable",
                              &ImplicitDSLBase::treatIntegrationVariable);
    this->registerNewCallBack("@ComputeStiffnessTensor",
                              &ImplicitDSLBase::treatComputeStiffnessTensor);
    this->registerNewCallBack("@ComputeStiffnessTensor",
                              &ImplicitDSLBase::treatComputeStiffnessTensor);
    this->registerNewCallBack("@ElasticMaterialProperties",
                              &ImplicitDSLBase::treatElasticMaterialProperties);
    this->registerNewCallBack("@HillTensor", &ImplicitDSLBase::treatHillTensor);
    this->disableCallBack("@ComputedVar");
    this->disableCallBack("@UseQt");
    this->mb.setIntegrationScheme(BehaviourDescription::IMPLICITSCHEME);
  }  // end of ImplicitDSLBase::ImplicitDSLBase

  const NonLinearSystemSolver& ImplicitDSLBase::getSolver() const {
    if (this->solver.get() == nullptr) {
      this->throwRuntimeError("ImplicitBase::getSolver", "no solver defined");
    }
    return *(this->solver);
  }

  void ImplicitDSLBase::treatUnknownKeyword() {
    --(this->current);
    const auto& key = this->current->value;
    ++(this->current);
    if (this->solver.get() != nullptr) {
      const auto r = this->solver->treatSpecificKeywords(
          this->mb, key, this->current, this->tokens.end());
      if (r.first) {
        this->current = r.second;
        return;
      }
    }
    BehaviourDSLCommon::treatUnknownKeyword();
  }  // end of ImplicitDSLBase::treatUnknownKeyword

  void ImplicitDSLBase::writeBehaviourLocalVariablesInitialisation(
      const Hypothesis h) {
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    Modifier mts = [this](const MaterialPropertyInput& i) -> std::string {
      if ((i.type == MaterialPropertyInput::TEMPERATURE) ||
          (i.type ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.type == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+theta*(this->d" + i.name + ')';
      } else if ((i.type == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.type == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else {
        this->throwRuntimeError(
            "ImplicitDSLBase::writeBehaviourLocalVariablesInitialisation",
            "unsupported input type for variable '" + i.name + "'");
      }
    };
    Modifier ets = [this](const MaterialPropertyInput& i) -> std::string {
      if ((i.type == MaterialPropertyInput::TEMPERATURE) ||
          (i.type ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.type == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+this->d" + i.name;
      } else if ((i.type == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.type == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else {
        this->throwRuntimeError(
            "ImplicitDSLBase::writeBehaviourLocalVariablesInitialisation",
            "unsupported input type for variable '" + i.name + "'");
      }
    };
    if (this->mb.getAttribute(BehaviourDescription::computesStiffnessTensor,
                              false)) {
      this->behaviourFile << "// updating the stiffness tensor at the middle "
                             "of the time step\n";
      this->writeStiffnessTensorComputation(this->behaviourFile, "this->D",
                                            mts);
      if (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep()) {
        this->behaviourFile
            << "// stiffness tensor at the end of the time step\n";
        this->writeStiffnessTensorComputation(this->behaviourFile,
                                              "this->D_tdt", ets);
      }
    }
    for (const auto& ht : this->mb.getHillTensors()) {
      if ((this->mb.getBehaviourType() !=
           BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
          (this->mb.getBehaviourType() !=
           BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
        this->throwRuntimeError(
            "ImplicitDSLBase::writeBehaviourLocalVariablesInitialisation",
            "Hill tensors shall only be defined for finite strain "
            "or small strain behaviours");
      }
      this->writeHillTensorComputation(this->behaviourFile, "this->" + ht.name,
                                       ht, mts);
      if (!this->mb.areMaterialPropertiesConstantDuringTheTimeStep(ht.c)) {
        this->writeHillTensorComputation(this->behaviourFile,
                                         "this->" + ht.name + "_tdt", ht, ets);
      }
    }
    if ((!this->mb.getAttribute(BehaviourDescription::computesStiffnessTensor,
                                false)) &&
        (this->mb.getElasticSymmetryType() == ISOTROPIC) &&
        (this->mb.areElasticMaterialPropertiesDefined())) {
      const auto& emps = this->mb.getElasticMaterialProperties();
      if (emps.size() != 2u) {
        this->throwRuntimeError(
            "ImplicitDSLBase::writeBehaviourLocalVariablesInitialisation",
            "invalid number of material properties");
      }
      if (!emps[0].is<BehaviourDescription::ConstantMaterialProperty>()) {
        this->writeMaterialPropertyCheckBoundsEvaluation(this->behaviourFile,
                                                         emps[0], mts);
      }
      if (!emps[1].is<BehaviourDescription::ConstantMaterialProperty>()) {
        this->writeMaterialPropertyCheckBoundsEvaluation(this->behaviourFile,
                                                         emps[1], mts);
      }
      if (!emps[0].is<BehaviourDescription::ConstantMaterialProperty>()) {
        this->behaviourFile << "this->young=";
        this->writeMaterialPropertyEvaluation(this->behaviourFile, emps[0],
                                              mts);
        this->behaviourFile << ";\n";
      }
      if (!emps[1].is<BehaviourDescription::ConstantMaterialProperty>()) {
        this->behaviourFile << "this->nu=";
        this->writeMaterialPropertyEvaluation(this->behaviourFile, emps[1],
                                              mts);
        this->behaviourFile << ";\n";
      }
      this->behaviourFile << "this->lambda=computeLambda(young,nu);\n";
      this->behaviourFile << "this->mu=computeMu(young,nu);\n";
      if (!this->mb.isMaterialPropertyConstantDuringTheTimeStep(emps[0])) {
        this->writeMaterialPropertyCheckBoundsEvaluation(this->behaviourFile,
                                                         emps[0], ets);
        this->behaviourFile << "this->young_tdt=";
        this->writeMaterialPropertyEvaluation(this->behaviourFile, emps[0],
                                              ets);
        this->behaviourFile << ";\n";
      } else {
        this->behaviourFile << "this->young_tdt  = this->young;\n";
      }
      if (!this->mb.isMaterialPropertyConstantDuringTheTimeStep(emps[1])) {
        this->writeMaterialPropertyCheckBoundsEvaluation(this->behaviourFile,
                                                         emps[1], ets);
        this->behaviourFile << "this->nu_tdt=";
        this->writeMaterialPropertyEvaluation(this->behaviourFile, emps[1],
                                              ets);
        this->behaviourFile << ";\n";
      } else {
        this->behaviourFile << "this->nu_tdt     = this->nu;\n";
      }
      if (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep()) {
        this->behaviourFile
            << "this->lambda_tdt = computeLambda(young_tdt,nu_tdt);\n";
        this->behaviourFile
            << "this->mu_tdt     = computeMu(young_tdt,nu_tdt);\n";
      } else {
        this->behaviourFile << "this->lambda_tdt = this->lambda;\n";
        this->behaviourFile << "this->mu_tdt     = this->mu;\n";
      }
    }
    BehaviourDSLCommon::writeBehaviourLocalVariablesInitialisation(h);
  }  // end of writeBehaviourLocalVariablesInitialisation

  void ImplicitDSLBase::treatStateVariable() {
    VariableDescriptionContainer v;
    auto hs = std::set<Hypothesis>{};
    this->readVariableList(v, hs, &BehaviourDescription::addStateVariables,
                           true, false);
    for (const auto h : hs) {
      for (const auto& iv : v) {
        this->mb.reserveName(h, "f" + iv.name);
      }
    }
  }

  void ImplicitDSLBase::treatIntegrationVariable() {
    VariableDescriptionContainer v;
    auto hs = std::set<Hypothesis>{};
    this->readVariableList(
        v, hs, &BehaviourDescription::addIntegrationVariables, true, false);
    for (const auto h : hs) {
      for (const auto& iv : v) {
        this->mb.reserveName(h, "f" + iv.name);
      }
    }
  }

  void ImplicitDSLBase::treatInitJacobian() {
    if (this->solver == nullptr) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatInitJacobian",
          "undefined algorithm. Please use the '@Algorithm' "
          "keyword to define one.");
    }
    if (!this->solver->allowsJacobianInitialisation()) {
      this->throwRuntimeError("ImplicitDSLBase::treatInitJacobian",
                              "@InitJacobian can not be used with "
                              "the current algorithm.");
    }
    this->readCodeBlock(*this, BehaviourData::InitializeJacobian,
                        &ImplicitDSLBase::standardModifier, true, true);
  }  // end of ImplicitDSLBase::treatInitJacobian

  void ImplicitDSLBase::treatInitJacobianInvert() {
    if (this->solver == nullptr) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatInitJacobianInvert",
          "undefined algorithm. Please use the '@Algorithm' "
          "keyword to define one.");
    }
    if (!this->solver->allowsJacobianInvertInitialisation()) {
      this->throwRuntimeError("ImplicitDSLBase::treatInitJacobianInvert",
                              "@InitJacobianInvert can not be used with "
                              "the current algorithm.");
    }
    this->readCodeBlock(*this, BehaviourData::InitializeJacobianInvert,
                        &ImplicitDSLBase::standardModifier, true, true);
  }  // end of ImplicitDSLBase::treatInitJacobianInvert

  void ImplicitDSLBase::treatUnknownVariableMethod(const Hypothesis h,
                                                   const std::string& n) {
    if ((this->mb.isIntegrationVariableName(h, n)) ||
        ((n[0] == 'f') &&
         (this->mb.isIntegrationVariableName(h, n.substr(1))))) {
      if (this->current->value == "setNormalisationFactor") {
        auto var = std::string{};
        ++(this->current);
        this->checkNotEndOfFile("ImplicitDSLBase::treatUnknowVariableMethod");
        this->readSpecifiedToken("ImplicitDSLBase::treatUnknowVariableMethod",
                                 "(");
        this->checkNotEndOfFile("ImplicitDSLBase::treatUnknowVariableMethod");
        var = this->current->value;
        if ((this->mb.isMaterialPropertyName(h, var)) ||
            (this->mb.isLocalVariableName(h, var))) {
          var = "this->" + var;
        } else {
          // var shall be a number
          double value;
          std::istringstream flux(var);
          flux >> value;
          if (flux.fail()) {
            this->throwRuntimeError(
                "ImplicitDSLBase::treatUnknowVariableMethod",
                "Failed to read normalisation factor.");
          }
          if (value <= 0.) {
            this->throwRuntimeError(
                "ImplicitDSLBase::treatUnknowVariableMethod",
                "invalid normalisation factor.");
          }
        }
        this->mb.setAttribute(h, n + "_normalisation_factor", var);
        ++(this->current);
        return;
      } else if (this->current->value ==
                 "setMaximumIncrementValuePerIteration") {
        ++(this->current);
        this->checkNotEndOfFile("ImplicitDSLBase::treatUnknowVariableMethod");
        this->readSpecifiedToken("ImplicitDSLBase::treatUnknowVariableMethod",
                                 "(");
        this->checkNotEndOfFile("ImplicitDSLBase::treatUnknowVariableMethod");
        const auto var = this->current->value;
        double value;
        std::istringstream flux(var);
        flux >> value;
        if (flux.fail()) {
          this->throwRuntimeError(
              "ImplicitDSLBase::treatUnknowVariableMethod",
              "Failed to read maximum increment value per iteration from '" +
                  var + "'.");
        }
        if (value <= 0.) {
          this->throwRuntimeError(
              "ImplicitDSLBase::treatUnknowVariableMethod",
              "invalid maximum increment value per iteration.");
        }
        VariableDescription miv(
            "real", n + "_maximum_increment_value_per_iteration", 1u, 0u);
        miv.description =
            "maximum increment allowed per iteration for variable '" + n + "'";
        this->mb.addParameter(h, miv);
        this->mb.setParameterDefaultValue(
            h, n + "_maximum_increment_value_per_iteration", value);
        ++(this->current);
        return;
      }
    }
    BehaviourDSLCommon::treatUnknownVariableMethod(h, n);
  }  // end of ImplicitDSLBase::treatUnknowVariableMethod

  bool ImplicitDSLBase::isCallableVariable(const Hypothesis h,
                                           const std::string& n) const

  {
    if (n.empty()) {
      this->throwRuntimeError("ImplicitDSLBase::isCallableVariable",
                              "empty variable name '" + n + "'");
    }
    if ((n[0] == 'f') && (this->mb.isIntegrationVariableName(h, n.substr(1)))) {
      return true;
    }
    return BehaviourDSLCommon::isCallableVariable(h, n);
  }  // end of ImplicitDSLBase::isCallableVariable

  void ImplicitDSLBase::treatCompareToNumericalJacobian() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile(
        "ImplicitDSLBase::treatCompareToNumericalJacobian : ",
        "Expected 'true' or 'false'.");
    if (this->current->value == "true") {
      this->mb.setAttribute(h, BehaviourData::compareToNumericalJacobian, true);
    } else if (this->current->value == "false") {
      this->mb.setAttribute(h, BehaviourData::compareToNumericalJacobian,
                            false);
    } else {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatCompareToNumericalJacobian",
          "Expected to read 'true' or 'false' instead of '" +
              this->current->value + ".");
    }
    ++(this->current);
    this->readSpecifiedToken("ImplicitDSLBase::treatCompareToNumericalJacobian",
                             ";");
  }  // end of ImplicitDSLBase::treatCompareToNumericalJacobian

  void ImplicitDSLBase::treatJacobianComparisonCriterion() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (!this->mb.getAttribute(h, BehaviourData::compareToNumericalJacobian,
                               false)) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatJacobianComparisonCriterion",
          "must call '@CompareToNumericalJacobian' first");
    }
    double jacobianComparisonCriterion;
    this->checkNotEndOfFile("ImplicitDSLBase::treatJacobianComparisonCriterion",
                            "Cannot read jacobianComparisonCriterion value.");
    std::istringstream flux(current->value);
    flux >> jacobianComparisonCriterion;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatJacobianComparisonCriterion",
          "Failed to read jacobianComparisonCriterion value.");
    }
    if (jacobianComparisonCriterion < 0) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatJacobianComparisonCriterion",
          "JacobianComparisonCriterion value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken(
        "ImplicitDSLBase::treatJacobianComparisonCriterion", ";");
    this->mb.addParameter(
        h, VariableDescription("real", "jacobianComparisonCriterion", 1u, 0u),
        BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "jacobianComparisonCriterion",
                                      jacobianComparisonCriterion);
  }  // ImplicitDSLBase::treatJacobianComparisonCriterion

  void ImplicitDSLBase::treatAlgorithm() {
    const auto& f =
        NonLinearSystemSolverFactory::getNonLinearSystemSolverFactory();
    if (this->solver != nullptr) {
      this->throwRuntimeError("ImplicitDSLBase::treatAlgorithm",
                              "an algorithm has already been defined.");
    }
    this->checkNotEndOfFile("ImplicitDSLBase::treatAlgorithm",
                            "Cannot read algorithm name.");
    const auto& s = this->current->value;
    ++this->current;
    this->readSpecifiedToken("ImplicitDSLBase::treatAlgorithm", ";");
    this->solver = f.getSolver(s);
    for (const auto& n : this->solver->getReservedNames()) {
      this->reserveName(n);
    }
    this->mb.setAttribute(BehaviourData::algorithm, s, false);
  }  // end of ImplicitDSLBase::treatAlgorithm

  void ImplicitDSLBase::treatTheta() {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double theta;
    this->checkNotEndOfFile("ImplicitDSLBase::treatTheta",
                            "Cannot read theta value.");
    std::istringstream flux(current->value);
    flux >> theta;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError("ImplicitDSLBase::treatTheta",
                              "Failed to read theta value.");
    }
    if ((theta < 0.) || (theta > 1.)) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatTheta",
          "Theta value must be positive and smaller than 1.");
    }
    ++(this->current);
    this->readSpecifiedToken("ImplicitDSLBase::treatTheta", ";");
    VariableDescription tv("real", "theta", 1u, 0u);
    tv.description = "theta value used by the implicit scheme";
    this->mb.addParameter(h, tv, BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "theta", theta);
    this->mb.setEntryName(h, "theta", "theta");
  }  // end of ImplicitDSLBase::treatTheta

  void ImplicitDSLBase::treatEpsilon() {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double epsilon;
    this->checkNotEndOfFile("ImplicitDSLBase::treatEpsilon",
                            "Cannot read epsilon value.");
    std::istringstream flux(current->value);
    flux >> epsilon;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError("ImplicitDSLBase::treatEpsilon",
                              "Failed to read epsilon value.");
    }
    if (epsilon < 0) {
      this->throwRuntimeError("ImplicitDSLBase::treatEpsilon",
                              "Epsilon value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("ImplicitDSLBase::treatEpsilon", ";");
    VariableDescription e("real", "epsilon", 1u, 0u);
    e.description =
        "value used to stop the iteration of the implicit algorithm";
    this->mb.addParameter(h, e, BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "epsilon", epsilon);
    this->mb.setEntryName(h, "epsilon", "epsilon");
  }  // ImplicitDSLBase::treatEpsilon

  void
  ImplicitDSLBase::treatPerturbationValueForNumericalJacobianComputation() {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double epsilon;
    this->checkNotEndOfFile(
        "ImplicitDSLBase::"
        "treatPerturbationValueForNumericalJacobianComputation",
        "Cannot read epsilon value.");
    std::istringstream flux(current->value);
    flux >> epsilon;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError(
          "ImplicitDSLBase::"
          "treatPerturbationValueForNumericalJacobianComputation",
          "Failed to read epsilon value.");
    }
    if (epsilon < 0) {
      this->throwRuntimeError(
          "ImplicitDSLBase::"
          "treatPerturbationValueForNumericalJacobianComputation",
          "Epsilon value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken(
        "ImplicitDSLBase::"
        "treatPerturbationValueForNumericalJacobianComputation",
        ";");
    VariableDescription e("real", "numerical_jacobian_epsilon", 1u, 0u);
    e.description =
        "perturbation value used to compute a finite difference approximation "
        "of the jacobian";
    this->mb.addParameter(h, e, BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "numerical_jacobian_epsilon", epsilon);
  }  // ImplicitDSLBase::treatEpsilon

  void ImplicitDSLBase::treatIterMax() {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    unsigned short iterMax;
    iterMax = this->readUnsignedShort("ImplicitDSLBase::treatIterMax");
    if (iterMax == 0) {
      this->throwRuntimeError("ImplicitDSLBase::treatIterMax",
                              "invalid value for parameter 'iterMax'");
    }
    this->readSpecifiedToken("ImplicitDSLBase::treatIterMax", ";");
    this->mb.addParameter(h, VariableDescription("ushort", "iterMax", 1u, 0u),
                          BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "iterMax", iterMax);
  }  // end of ImplicitDSLBase::treatIterMax

  std::string ImplicitDSLBase::tangentOperatorVariableModifier(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
    if (d.isIntegrationVariableIncrementName(var)) {
      if (this->mb.hasAttribute(h, var.substr(1) + "_normalisation_factor")) {
        const auto& s = d.getStateVariableDescription(var.substr(1));
        const auto& nf = this->mb.getAttribute<string>(
            h, var.substr(1) + "_normalisation_factor");
        if (s.arraySize == 1u) {
          if (addThisPtr) {
            return "((" + nf + ")*(this->" + var + "))";
          } else {
            return "(" + nf + ")*var";
          }
        } else {
          if (addThisPtr) {
            return "(" + nf + ")*(this->" + var + ")";
          } else {
            return "(" + nf + ")*var";
          }
        }
      }
    }
    if (addThisPtr) {
      return "(this->" + var + ")";
    } else {
      return var;
    }
  }  // end of ImplicitDSLBase::tangentOperatorVariableModifier

  std::string ImplicitDSLBase::integratorVariableModifier(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    const auto& d = this->mb.getBehaviourData(h);
    if (d.isIntegrationVariableIncrementName(var)) {
      if (this->mb.hasAttribute(h, var.substr(1) + "_normalisation_factor")) {
        const auto& s = d.getStateVariableDescription(var.substr(1));
        const auto& nf = this->mb.getAttribute<std::string>(
            h, var.substr(1) + "_normalisation_factor");
        if (s.arraySize == 1u) {
          if (addThisPtr) {
            return "((" + nf + ")*(this->" + var + "))";
          } else {
            return "(" + nf + ")*var";
          }
        } else {
          if (addThisPtr) {
            return "(" + nf + ")*(this->" + var + ")";
          } else {
            return "(" + nf + ")*var";
          }
        }
      }
    }
    if (addThisPtr) {
      return "(this->" + var + ")";
    } else {
      return var;
    }
  }  // end of ImplicitDSLBase::integratorVariableModifier

  std::string ImplicitDSLBase::computeStressVariableModifier1(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    const auto& d = this->mb.getBehaviourData(h);
    if (this->mb.isDrivingVariableName(var) ||
        (var == "T") | (d.isExternalStateVariableName(var))) {
      if (addThisPtr) {
        return "(this->" + var + "+(this->theta)*(this->d" + var + "))";
      } else {
        return "(" + var + "+(this->theta)*d" + var + ")";
      }
    }
    if (d.isAuxiliaryStateVariableName(var)) {
      const auto& v = d.getAuxiliaryStateVariables().getVariable(var);
      if (v.getAttribute<bool>("ComputedByExternalModel", false)) {
        if (addThisPtr) {
          return "(this->" + var + "+(this->theta)*(this->d" + var + "))";
        } else {
          return "(" + var + "+(this->theta)*d" + var + ")";
        }
      }
    }
    if (d.isIntegrationVariableName(var)) {
      if (this->mb.hasAttribute(h, var + "_normalisation_factor")) {
        const auto& nf = this->mb.getAttribute<std::string>(
            h, var + "_normalisation_factor");
        if (addThisPtr) {
          return "(this->" + var + "+(this->theta)*((" + nf + ")*(this->d" +
                 var + ")))";
        } else {
          return "(" + var + "+(" + nf + ")*(this->theta)*d" + var + ")";
        }
      } else {
        if (addThisPtr) {
          return "(this->" + var + "+(this->theta)*(this->d" + var + "))";
        } else {
          return "(" + var + "+(this->theta)*d" + var + ")";
        }
      }
    }
    if ((d.isExternalStateVariableIncrementName(var)) || (var == "dT")) {
      this->mb
          .declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
              h, var.substr(1));
    }
    if (addThisPtr) {
      return "this->" + var;
    }
    return var;
  }  // end of ImplicitDSLBase::computeStressVariableModifier1

  std::string ImplicitDSLBase::computeStressVariableModifier2(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    const auto& d = this->mb.getBehaviourData(h);
    if ((this->mb.isDrivingVariableName(var)) || (var == "T") ||
        (d.isExternalStateVariableName(var))) {
      if (addThisPtr) {
        return "(this->" + var + "+this->d" + var + ")";
      } else {
        return "(" + var + "+d" + var + ")";
      }
    }
    if (d.isAuxiliaryStateVariableName(var)) {
      const auto& v = d.getAuxiliaryStateVariables().getVariable(var);
      if (v.getAttribute<bool>("ComputedByExternalModel", false)) {
        if (addThisPtr) {
          return "(this->" + var + "+this->d" + var + ")";
        } else {
          return "(" + var + "+d" + var + ")";
        }
      }
    }
    if ((d.isExternalStateVariableIncrementName(var)) || (var == "dT")) {
      this->mb
          .declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
              h, var.substr(1));
    }
    if (addThisPtr) {
      return "this->" + var;
    }
    return var;
  }  // end of ImplicitDSLBase::computeStressVariableModifier2

  bool ImplicitDSLBase::isJacobianPart(const Hypothesis h,
                                       const std::string& w) {
    const auto& d = this->mb.getBehaviourData(h);
    TokensContainer::const_iterator previous;
    VariableDescriptionContainer::const_iterator p;
    VariableDescriptionContainer::const_iterator p2;
    for (p = d.getIntegrationVariables().begin();
         p != d.getIntegrationVariables().end(); ++p) {
      for (p2 = d.getIntegrationVariables().begin();
           p2 != d.getIntegrationVariables().end(); ++p2) {
        if (w == "df" + p->name + "_dd" + p2->name) {
          previous = this->current;
          if (previous == this->begin()) {
            return true;
          }
          --previous;
          if (previous->value == ".") {
            return false;
          } else if (previous->value == "->") {
            if (previous == this->begin()) {
              return false;
            }
            --previous;
            return previous->value == "this";
          } else if (previous->value == "::") {
            if (previous == this->begin()) {
              return false;
            }
            --previous;
            return previous->value == this->mb.getClassName();
          }
          return true;
        }
      }
    }
    return false;
  }  // end of ImplicitDSLBase::isJacobianPart

  void ImplicitDSLBase::integratorAnalyser(const Hypothesis h,
                                           const std::string& w) {
    if (this->isJacobianPart(h, w)) {
      this->jacobianPartsUsedInIntegrator.insert(w);
    }
  }  // end of ImplicitDSLBase::integratorAnalyser

  void ImplicitDSLBase::predictorAnalyser(const Hypothesis h,
                                          const std::string& w) {
    const auto& d = this->mb.getBehaviourData(h);
    if (d.isIntegrationVariableIncrementName(w)) {
      this->integrationVariablesIncrementsUsedInPredictor.insert(w);
    }
  }  // end of ImplicitDSLBase::predictorAnalyser

  void ImplicitDSLBase::treatIntegrator() {
    this->readCodeBlock(*this, BehaviourData::Integrator,
                        &ImplicitDSLBase::integratorVariableModifier,
                        &ImplicitDSLBase::integratorAnalyser, true, true);
  }  // end of ImplicitDSLBase::treatIntegrator

  void ImplicitDSLBase::treatPredictor() {
    using namespace std;
    this->readCodeBlock(*this, BehaviourData::ComputePredictor,
                        &ImplicitDSLBase::standardModifier,
                        &ImplicitDSLBase::predictorAnalyser, true, true);
  }  // end of ImplicitDSLBase::treatPredictor

  void ImplicitDSLBase::treatComputeStress() {
    /*
     * Most behaviours will only rely the @ComputeStress keyword to
     * estimate stresses at the middle of the time step and at the
     * end.  However, some finite strains behaviours must compute a
     * stress measure during the iterations which is not the Cauchy
     * stress. Thus, @ComputeStress also defines an
     * ComputeFinalStressCandidate code block which will be used if
     * the user does not provide an alternative through the
     * @ComputeFinalStress
     */
    this->readCodeBlock(*this, BehaviourData::ComputeStress,
                        BehaviourData::ComputeFinalStressCandidate,
                        &ImplicitDSLBase::computeStressVariableModifier1,
                        &ImplicitDSLBase::computeStressVariableModifier2, true,
                        true);
  }  // end of ImplicitDSLBase::treatComputeStress

  void ImplicitDSLBase::treatComputeFinalStress() {
    this->readCodeBlock(*this, BehaviourData::ComputeFinalStress,
                        &ImplicitDSLBase::computeStressVariableModifier2, true,
                        true);
  }  // end of ImplicitDSLBase::treatComputeFinalStress

  void ImplicitDSLBase::treatMaximumIncrementValuePerIteration() {
    using namespace std;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    double value;
    this->checkNotEndOfFile(
        "ImplicitDSLBase::treatMaximumIncrementValuePerIteration",
        "Cannot read value value.");
    istringstream flux(current->value);
    flux >> value;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatMaximumIncrementValuePerIteration",
          "Failed to read value.");
    }
    if (value <= 0) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatMaximumIncrementValuePerIteration",
          "Value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken(
        "ImplicitDSLBase::MaximumIncrementValuePerIteration", ";");
    this->mb.addParameter(
        h,
        VariableDescription("real", "maximum_increment_value_per_iteration", 1u,
                            0u),
        BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(
        h, "maximum_increment_value_per_iteration", value);
  }  // end of ImplicitDSLBase::treatMaximumIncrementValuePerIteration

  void ImplicitDSLBase::writeBehaviourParserSpecificIncludes() {
    bool has_scalar = false;
    bool has_scalar_array = false;
    bool has_tvector = false;
    bool has_tvector_array = false;
    bool has_stensor = false;
    bool has_stensor_array = false;
    // checks
    const auto& mh = this->mb.getModellingHypotheses();
    for (const auto& h : mh) {
      const auto& d = this->mb.getBehaviourData(h);
      for (const auto& v : d.getIntegrationVariables()) {
        const auto flag = this->getTypeFlag(v.type);
        if (flag == SupportedTypes::Scalar) {
          has_scalar = true;
          if (v.arraySize >= 1) {
            has_scalar_array = true;
          }
        }
        if (flag == SupportedTypes::Stensor) {
          has_stensor = true;
          if (v.arraySize >= 1) {
            has_stensor_array = true;
          }
        }
        if (flag == SupportedTypes::TVector) {
          has_tvector = true;
          if (v.arraySize >= 1) {
            has_tvector_array = true;
          }
        }
      }
    }
    // output
    this->checkBehaviourFile();
    this->behaviourFile
        << "#include\"TFEL/Math/st2tost2.hxx\"\n"
        << "#include\"TFEL/Math/tmatrix.hxx\"\n"
        << "#include\"TFEL/Math/tvector.hxx\"\n"
        << "#include\"TFEL/Math/Matrix/tmatrixIO.hxx\"\n"
        << "#include\"TFEL/Math/Vector/tvectorIO.hxx\"\n"
        << "#include\"TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx\"\n";
    if (has_scalar_array) {
      this->behaviourFile
          << "#include\"TFEL/Math/Vector/TinyVectorFromTinyVectorView.hxx\"\n"
          << "#include\"TFEL/Math/Vector/"
             "TinyVectorFromTinyVectorViewIO.hxx\"\n";
    }
    // tiny vectors
    if (has_tvector) {
      this->behaviourFile
          << "#include\"TFEL/Math/Matrix/tmatrix_submatrix_view.hxx\"\n\n"
          << "#include\"TFEL/Math/Matrix/tmatrix_submatrix_view.hxx\"\n\n";
    }
    if ((has_scalar) && (has_tvector)) {
      this->behaviourFile
          << "#include\"TFEL/Math/Vector/"
             "TVectorFromTinyMatrixColumnView.hxx\"\n"
          << "#include\"TFEL/Math/Vector/TVectorFromTinyMatrixRowView.hxx\"\n"
          << "#include\"TFEL/Math/Vector/"
             "TVectorFromTinyMatrixColumnView2.hxx\"\n"
          << "#include\"TFEL/Math/Vector/TVectorFromTinyMatrixRowView2.hxx\"\n"
          << "#include\"TFEL/Math/Vector/TVectorFromTinyVectorView.hxx\"\n";
    }
    if (has_tvector_array) {
      this->behaviourFile
          << "#include\"TFEL/Math/Vector/"
             "TinyVectorOfTinyVectorFromTinyVectorView.hxx\"\n"
          << "#include\"TFEL/Math/Vector/"
             "TinyVectorOfTinyVectorFromTinyVectorViewIO.hxx\"\n";
    }
    // symmetric tensors
    if (has_stensor) {
      this->behaviourFile
          << "#include\"TFEL/Math/Stensor/StensorFromTinyVectorView.hxx\"\n"
          << "#include\"TFEL/Math/ST2toST2/ST2toST2FromTinyMatrixView.hxx\"\n\n"
          << "#include\"TFEL/Math/ST2toST2/"
             "ST2toST2FromTinyMatrixView2.hxx\"\n\n";
    }
    if ((has_scalar) && (has_stensor)) {
      this->behaviourFile
          << "#include\"TFEL/Math/Stensor/"
             "StensorFromTinyMatrixColumnView.hxx\"\n"
          << "#include\"TFEL/Math/Stensor/StensorFromTinyMatrixRowView.hxx\"\n"
          << "#include\"TFEL/Math/Stensor/"
             "StensorFromTinyMatrixColumnView2.hxx\"\n"
          << "#include\"TFEL/Math/Stensor/"
             "StensorFromTinyMatrixRowView2.hxx\"\n";
    }
    if (has_stensor_array) {
      this->behaviourFile << "#include\"TFEL/Math/Vector/"
                             "TinyVectorOfStensorFromTinyVectorView.hxx\"\n"
                          << "#include\"TFEL/Math/Vector/"
                             "TinyVectorOfStensorFromTinyVectorViewIO.hxx\"\n";
    }
  }  // end of ImplicitDSLBase::writeBehaviourParserSpecificIncludes(void)

  void ImplicitDSLBase::writeBehaviourParserSpecificTypedefs() {
    this->checkBehaviourFile();
    BehaviourDSLCommon::writeBehaviourParserSpecificTypedefs();
  }  // end of ImplicitDSLBase::writeBehaviourParserSpecificTypedefs(void)

  void ImplicitDSLBase::writeBehaviourParserSpecificMembers(
      const Hypothesis h) {
    BehaviourDSLCommon::writeBehaviourParserSpecificMembers(h);
    const auto& d = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    VariableDescriptionContainer::const_iterator p2;
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n3;
    // size of linear system
    for (p = d.getIntegrationVariables().begin();
         p != d.getIntegrationVariables().end(); ++p) {
      n3 += this->getTypeSize(p->type, p->arraySize);
    }
    for (p = d.getIntegrationVariables().begin();
         p != d.getIntegrationVariables().end(); ++p) {
      SupportedTypes::TypeSize n2;
      for (p2 = d.getIntegrationVariables().begin();
           p2 != d.getIntegrationVariables().end(); ++p2) {
        SupportedTypes::TypeFlag flag = this->getTypeFlag(p->type);
        SupportedTypes::TypeFlag flag2 = this->getTypeFlag(p2->type);
        if ((p->arraySize != 1u) || (p2->arraySize != 1u)) {
          this->behaviourFile << "/*!\n"
                              << " * \\return the part of the jacobian matrix "
                              << "corresponding to the derivative "
                              << "of variable " << p->name << " by variable "
                              << p2->name << "\n"
                              << " */\n";
        }
        if ((p->arraySize != 1u) && (p2->arraySize == 1u)) {
          if (flag == SupportedTypes::Scalar) {
            if (flag2 == SupportedTypes::Scalar) {
              this->behaviourFile << "real&\n"
                                  << "df" << p->name << "_dd" << p2->name
                                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                                  << ",real>& tjacobian,\n"
                                  << "const unsigned short idx){\n"
                                  << "return tjacobian(" << n << "+idx, " << n2
                                  << ");\n"
                                  << "}\n\n";
              this->behaviourFile << "real&\n"
                                  << "df" << p->name << "_dd" << p2->name
                                  << "(const unsigned short idx){\n"
                                  << "return this->jacobian(" << n << "+idx, "
                                  << n2 << ");\n"
                                  << "}\n\n";
            } else if (flag2 == SupportedTypes::TVector) {
              // Le résultat est un tenseur, une ligne dans la matrice
              // jacobienne
              this->behaviourFile
                  << "typename tfel::math::TVectorFromTinyMatrixRowView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                  << ",real>& tjacobian,\n"
                  << "const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename TVectorFromTinyMatrixRowView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(tjacobian,idx,0);\n"
                  << "}\n\n";
              this->behaviourFile
                  << "typename tfel::math::TVectorFromTinyMatrixRowView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename TVectorFromTinyMatrixRowView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(this->jacobian,idx,0);\n"
                  << "}\n\n";
            } else {
              // Le résultat est un tenseur, une ligne dans la matrice
              // jacobienne
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixRowView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                  << ",real>& tjacobian,\n"
                  << "const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixRowView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(tjacobian,idx,0);\n"
                  << "}\n\n";
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixRowView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixRowView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(this->jacobian,idx,0);\n"
                  << "}\n\n";
            }
          } else if (flag == SupportedTypes::TVector) {
            if (flag2 == SupportedTypes::Scalar) {
              // Le résultat est un tenseur, une colonne dans la matrice
              // jacobienne
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixColumnView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                  << ",real>& tjacobian,\n"
                  << "const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixColumnView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(tjacobian,idx,0);\n"
                  << "}\n\n";
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixColumnView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixColumnView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(this->jacobian,idx,0);\n"
                  << "}\n\n";
            } else if (flag2 == SupportedTypes::TVector) {
              // Le résultat est une sous-matrice
              this->behaviourFile
                  << "typename tfel::math::TMatrixFromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                  << ",real>& tjacobian,\n"
                  << "const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename TMatrixFromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(tjacobian,idx,0u);\n"
                  << "}\n\n";
              this->behaviourFile
                  << "typename tfel::math::TMatrixFromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename TMatrixFromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(this->jacobian,idx,0u);\n"
                  << "}\n\n";
            } else {
              this->throwRuntimeError(
                  "ImplicitDSLBase::writeBehaviourParserSpecificMembers",
                  "derivation of a vector by a tensor is not defined");
            }
          } else {
            if (flag2 == SupportedTypes::Scalar) {
              // Le résultat est un tenseur, une colonne dans la matrice
              // jacobienne
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixColumnView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                  << ",real>& tjacobian,\n"
                  << "const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixColumnView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(tjacobian,idx,0);\n"
                  << "}\n\n";
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixColumnView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixColumnView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(this->jacobian,idx,0);\n"
                  << "}\n\n";
            } else if (flag2 == SupportedTypes::Stensor) {
              // Le résultat est un tenseur d'ordre 4
              this->behaviourFile
                  << "typename tfel::math::ST2toST2FromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                  << ",real>& tjacobian,\n"
                  << "const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename ST2toST2FromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(tjacobian,idx,0u);\n"
                  << "}\n\n";
              this->behaviourFile
                  << "typename tfel::math::ST2toST2FromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename ST2toST2FromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(this->jacobian,idx,0u);\n"
                  << "}\n\n";
            } else {
              this->throwRuntimeError(
                  "ImplicitDSLBase::writeBehaviourParserSpecificMembers",
                  "derivation of a tensor by a vector is not defined");
            }
          }
        } else if ((p->arraySize == 1u) && (p2->arraySize != 1u)) {
          if (flag == SupportedTypes::Scalar) {
            if (flag2 == SupportedTypes::Scalar) {
              this->behaviourFile << "real&\n"
                                  << "df" << p->name << "_dd" << p2->name
                                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                                  << ",real>& tjacobian,\n"
                                  << "const unsigned short idx){\n"
                                  << "return tjacobian(" << n << ", " << n2
                                  << "+idx);\n"
                                  << "}\n\n";
              this->behaviourFile << "real&\n"
                                  << "df" << p->name << "_dd" << p2->name
                                  << "(const unsigned short idx){\n"
                                  << "return this->jacobian(" << n << ", " << n2
                                  << "+idx);\n"
                                  << "}\n\n";
            } else {
              // Le résultat est un tenseur, une ligne dans la matrice
              // jacobienne
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixRowView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                  << ",real>& tjacobian,\n"
                  << "const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixRowView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(tjacobian,0,idx);\n"
                  << "}\n\n";
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixRowView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixRowView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(this->jacobian,0,idx);\n"
                  << "}\n\n";
            }
          } else {
            if (flag2 == SupportedTypes::Scalar) {
              // Le résultat est un tenseur, une colonne dans la matrice
              // jacobienne
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixColumnView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                  << ",real>& tjacobian,\n"
                  << "const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixColumnView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(tjacobian,0,idx);\n"
                  << "}\n\n";
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixColumnView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixColumnView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(this->jacobian,0,idx);\n"
                  << "}\n\n";
            } else {
              this->behaviourFile
                  << "typename tfel::math::ST2toST2FromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                  << ",real>& tjacobian,\n"
                  << "const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename ST2toST2FromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(tjacobian,0,idx);\n"
                  << "}\n\n";
              this->behaviourFile
                  << "typename tfel::math::ST2toST2FromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(const unsigned short idx){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename ST2toST2FromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(this->jacobian,0,idx);\n"
                  << "}\n\n";
            }
          }
        } else if ((p->arraySize != 1u) && (p2->arraySize != 1u)) {
          if (flag == SupportedTypes::Scalar) {
            if (flag2 == SupportedTypes::Scalar) {
              this->behaviourFile << "real&\n"
                                  << "df" << p->name << "_dd" << p2->name
                                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                                  << ",real>& tjacobian,\n"
                                  << "const unsigned short idx,"
                                  << " const unsigned short idx2){\n"
                                  << "return tjacobian(" << n << "+idx, " << n2
                                  << "+idx2);\n"
                                  << "}\n\n";
              this->behaviourFile << "real&\n"
                                  << "df" << p->name << "_dd" << p2->name
                                  << "(const unsigned short idx,"
                                  << " const unsigned short idx2){\n"
                                  << "return this->jacobian(" << n << "+idx, "
                                  << n2 << "+idx2);\n"
                                  << "}\n\n";
            } else {
              // Le résultat est un tenseur, une ligne dans la matrice
              // jacobienne
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixRowView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                  << ",real>& tjacobian,\n"
                  << "const unsigned short idx,"
                  << " const unsigned short idx2){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixRowView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(tjacobian,idx,idx2);\n"
                  << "}\n\n";
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixRowView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(const unsigned short idx,"
                  << " const unsigned short idx2){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixRowView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(this->jacobian,idx,idx2);\n"
                  << "}\n\n";
            }
          } else {
            if (flag2 == SupportedTypes::Scalar) {
              // Le résultat est un tenseur, une colonne dans la matrice
              // jacobienne
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixColumnView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                  << ",real>& tjacobian,\n"
                  << "const unsigned short idx,"
                  << " const unsigned short idx2){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixColumnView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(tjacobian,idx,idx2);\n"
                  << "}\n\n";
              this->behaviourFile
                  << "typename tfel::math::StensorFromTinyMatrixColumnView2<N,"
                  << n3 << "," << n3 << "," << n << "," << n2
                  << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(const unsigned short idx,"
                  << " const unsigned short idx2){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename StensorFromTinyMatrixColumnView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(this->jacobian,idx,idx2);\n"
                  << "}\n\n";
            } else {
              this->behaviourFile
                  << "typename tfel::math::ST2toST2FromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(tfel::math::tmatrix<" << n3 << "," << n3
                  << ",real>& tjacobian,\n"
                  << "const unsigned short idx,"
                  << " const unsigned short idx2){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename ST2toST2FromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(tjacobian,idx,idx2);\n"
                  << "}\n\n";
              this->behaviourFile
                  << "typename tfel::math::ST2toST2FromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2 << ",real>::type\n"
                  << "df" << p->name << "_dd" << p2->name
                  << "(const unsigned short idx,"
                  << " const unsigned short idx2){\n"
                  << "using namespace tfel::math;\n"
                  << "return typename ST2toST2FromTinyMatrixView2<N," << n3
                  << "," << n3 << "," << n << "," << n2
                  << ",real>::type(this->jacobian,idx,idx2);\n"
                  << "}\n\n";
            }
          }
        }
        n2 += this->getTypeSize(p2->type, p2->arraySize);
      }
      n += this->getTypeSize(p->type, p->arraySize);
    }
    // size of linear system
    n = n3;
    if (this->solver->usesJacobian()) {
      this->behaviourFile << "// Jacobian\n";
      this->behaviourFile << "tfel::math::tmatrix<" << n << "," << n
                          << ",real> jacobian;\n";
    }
    if (this->solver->usesJacobianInvert()) {
      this->behaviourFile << "// Jacobian\n";
      this->behaviourFile << "tfel::math::tmatrix<" << n << "," << n
                          << ",real> inv_jacobian;\n";
    }
    this->solver->writeSpecificMembers(this->behaviourFile, this->mb, h);
    this->behaviourFile << "// zeros\n";
    this->behaviourFile << "tfel::math::tvector<" << n << ",real> zeros;\n\n";
    this->behaviourFile << "// previous zeros\n";
    this->behaviourFile << "tfel::math::tvector<" << n << ",real> zeros_1;\n\n";
    this->behaviourFile << "// function\n";
    this->behaviourFile << "tfel::math::tvector<" << n << ",real> fzeros;\n\n";
    this->behaviourFile << "// number of iterations\n";
    this->behaviourFile << "unsigned int iter = 0u;\n\n";
    //

    if (this->solver->usesJacobian()) {
      // compute the numerical part of the jacobian.  This method is
      // used to compute a numerical approximation of the jacobian for
      // the solvers based on it. For solvers providing an analytical
      // jacobian, or an approximation of it, this method is used to
      // compare the jacobain to its numerical approximation
      this->writeComputeNumericalJacobian(h);
    }
    if (this->solver->usesJacobian()) {
      // for solver based on the jacobian matrix, the get partial
      // jacobian invert method can be used to compute the tangent
      // operator.
      this->writeGetPartialJacobianInvert(h);
    }
    // compute stress
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "void\ncomputeStress(void){\n"
                          << "using namespace std;\n"
                          << "using namespace tfel::math;\n"
                          << "using std::vector;\n";
      writeMaterialLaws("ImplicitDSLBase::writeBehaviourParserSpecificMembers",
                        this->behaviourFile, this->mb.getMaterialLaws());
      this->behaviourFile << this->mb.getCode(h, BehaviourData::ComputeStress)
                          << "\n} // end of " << this->mb.getClassName()
                          << "::computeStress\n\n";
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeFinalStress)) {
      this->behaviourFile << "void\ncomputeFinalStress(void){\n"
                          << "using namespace std;\n"
                          << "using namespace tfel::math;\n"
                          << "using std::vector;\n";
      writeMaterialLaws("ImplicitDSLBase::writeBehaviourParserSpecificMembers",
                        this->behaviourFile, this->mb.getMaterialLaws());
      this->behaviourFile << this->mb.getCode(h,
                                              BehaviourData::ComputeFinalStress)
                          << "\n} // end of " << this->mb.getClassName()
                          << "::computeStress\n\n";
    }
  }  // end of ImplicitDSLBase::writeBehaviourParserSpecificMembers

  void ImplicitDSLBase::writeGetPartialJacobianInvert(const Hypothesis h) {
    using size_type = VariableDescriptionContainer::size_type;
    const auto& d = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    SupportedTypes::TypeSize n;
    for (const auto& v : d.getIntegrationVariables()) {
      n += this->getTypeSize(v.type, v.arraySize);
    }
    for (size_type i = 0; i != d.getIntegrationVariables().size(); ++i) {
      this->behaviourFile << "void\ngetPartialJacobianInvert(";
      for (size_type i2 = 0; i2 <= i;) {
        const auto& v = d.getIntegrationVariables()[i2];
        const auto flag = this->getTypeFlag(v.type);
        if (v.arraySize == 1u) {
          switch (flag) {
            case SupportedTypes::Scalar:
              this->behaviourFile << "Stensor& ";
              break;
            case SupportedTypes::Stensor:
              this->behaviourFile << "Stensor4& ";
              break;
            case SupportedTypes::TVector:
            case SupportedTypes::Tensor:
            default:
              this->throwRuntimeError(
                  "ImplicitDSLBase::writeGetPartialJacobianInvert",
                  "internal error, tag unsupported");
          }
        } else {
          switch (flag) {
            case SupportedTypes::Scalar:
              this->behaviourFile << "tfel::math::tvector<" << v.arraySize
                                  << "u,Stensor>& ";
              break;
            case SupportedTypes::Stensor:
              this->behaviourFile << "tfel::math::tvector<" << v.arraySize
                                  << "u,Stensor4>& ";
              break;
            case SupportedTypes::TVector:
            case SupportedTypes::Tensor:
            default:
              this->throwRuntimeError(
                  "ImplicitDSLBase::writeGetPartialJacobianInvert",
                  "internal error, tag unsupported");
          }
        }
        this->behaviourFile << "partial_jacobian_" << v.name;
        if (++i2 <= i) {
          this->behaviourFile << ",\n";
        }
      }
      this->behaviourFile << ")\n";
      this->behaviourFile << "{\n";
      this->behaviourFile << "using namespace tfel::math;\n";
      this->behaviourFile << "TinyPermutation<" << n << "> permuation;\n";
      this->behaviourFile << "TinyMatrixSolve<" << n
                          << ",real>::decomp(this->jacobian,permuation);\n";
      this->behaviourFile
          << "for(unsigned short idx=0;idx!=StensorSize;++idx){\n";
      this->behaviourFile << "tvector<" << n << ",real> vect_e(real(0));\n";
      this->behaviourFile << "vect_e(idx) = real(1);\n";
      this->behaviourFile
          << "TinyMatrixSolve<" << n
          << ",real>::back_substitute(this->jacobian,permuation,vect_e);\n";
      SupportedTypes::TypeSize n2;
      for (size_type i2 = 0; i2 <= i; ++i2) {
        const auto& v = d.getIntegrationVariables()[i2];
        const auto flag = this->getTypeFlag(v.type);
        if (flag == SupportedTypes::Scalar) {
          if (v.arraySize == 1u) {
            this->behaviourFile << "partial_jacobian_" << v.name
                                << "(idx)=vect_e(" << n2 << ");\n";
          } else {
            this->behaviourFile
                << "for(unsigned short idx2=0;idx2!=" << v.arraySize
                << ";++idx2){\n";
            this->behaviourFile << "partial_jacobian_" << v.name
                                << "(idx2)(idx)=vect_e(" << n2 << "+idx2);\n";
            this->behaviourFile << "}\n";
          }
          n2 += this->getTypeSize(v.type, v.arraySize);
        } else if (flag == SupportedTypes::TVector) {
          if (v.arraySize == 1u) {
            this->behaviourFile << "for(unsigned short idx2=" << n2;
            this->behaviourFile << ";idx2!=";
            n2 += this->getTypeSize(v.type, v.arraySize);
            this->behaviourFile << n2 << ";++idx2){\n";
            this->behaviourFile << "partial_jacobian_" << v.name
                                << "(idx2,idx)=vect_e(idx2);\n";
            this->behaviourFile << "}\n";
          } else {
            this->behaviourFile
                << "for(unsigned short idx2=0;idx2!=" << v.arraySize
                << ";++idx2){\n";
            this->behaviourFile
                << "for(unsigned short idx3=0;idx3!=TVectorSize;++idx3){\n";
            this->behaviourFile << "partial_jacobian_" << v.name
                                << "(idx2)(idx3,idx)=vect_e(" << n2
                                << "+idx3+idx2*TVectorSize);\n";
            this->behaviourFile << "}\n";
            this->behaviourFile << "}\n";
            n2 += this->getTypeSize(v.type, v.arraySize);
          }
        } else if (flag == SupportedTypes::Stensor) {
          if (v.arraySize == 1u) {
            this->behaviourFile << "for(unsigned short idx2=" << n2;
            this->behaviourFile << ";idx2!=";
            n2 += this->getTypeSize(v.type, v.arraySize);
            this->behaviourFile << n2 << ";++idx2){\n";
            this->behaviourFile << "partial_jacobian_" << v.name
                                << "(idx2,idx)=vect_e(idx2);\n";
            this->behaviourFile << "}\n";
          } else {
            this->behaviourFile
                << "for(unsigned short idx2=0;idx2!=" << v.arraySize
                << ";++idx2){\n";
            this->behaviourFile
                << "for(unsigned short idx3=0;idx3!=StensorSize;++idx3){\n";
            this->behaviourFile << "partial_jacobian_" << v.name
                                << "(idx2)(idx3,idx)=vect_e(" << n2
                                << "+idx3+idx2*StensorSize);\n";
            this->behaviourFile << "}\n";
            this->behaviourFile << "}\n";
            n2 += this->getTypeSize(v.type, v.arraySize);
          }
        } else {
          this->throwRuntimeError(
              "ImplicitDSLBase::writeGetPartialJacobianInvert",
              "internal error, tag unsupported");
        }
      }
      this->behaviourFile << "}\n";
      for (size_type i2 = 0; i2 <= i; ++i2) {
        const auto& v = d.getIntegrationVariables()[i2];
        if (this->mb.hasAttribute(h, v.name + "_normalisation_factor")) {
          const auto& nf = this->mb.getAttribute<std::string>(
              h, v.name + "_normalisation_factor");
          this->behaviourFile << "partial_jacobian_" << v.name << " /= " << nf
                              << ";\n";
        }
      }
      this->behaviourFile << "}\n\n";
    }
  }  // end of ImplicitDSLBase::writeGetPartialJacobianInvert

  void ImplicitDSLBase::writeComputeNumericalJacobian(const Hypothesis h) {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    for (p = d.getIntegrationVariables().begin();
         p != d.getIntegrationVariables().end(); ++p) {
      n += this->getTypeSize(p->type, p->arraySize);
    }
    this->checkBehaviourFile();
    this->behaviourFile
        << "void\ncomputeNumericalJacobian(tfel::math::tmatrix<" << n << ","
        << n << ",real>& njacobian)\n"
        << "{\n"
        << "using namespace std;\n"
        << "using namespace tfel::math;\n"
        << "tvector<" << n << ",real> tzeros(this->zeros);\n"
        << "tvector<" << n << ",real> tfzeros(this->fzeros);\n"
        << "tmatrix<" << n << "," << n << ",real> tjacobian(this->jacobian);\n"
        << "for(unsigned short idx = 0; idx!= " << n << ";++idx){\n"
        << "this->zeros(idx) -= this->numerical_jacobian_epsilon;\n";
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "this->computeStress();\n";
    }
    this->behaviourFile
        << "this->computeFdF();\n"
        << "this->zeros = tzeros;\n"
        << "tvector<" << n << ",real> tfzeros2(this->fzeros);\n"
        << "this->zeros(idx) += this->numerical_jacobian_epsilon;\n";
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "this->computeStress();\n";
    }
    this->behaviourFile << "this->computeFdF();\n"
                        << "this->fzeros = "
                           "(this->fzeros-tfzeros2)/"
                           "(real(2)*(this->numerical_jacobian_epsilon));\n"
                        << "for(unsigned short idx2 = 0; idx2!= " << n
                        << ";++idx2){\n"
                        << "njacobian(idx2,idx) = this->fzeros(idx2);\n"
                        << "}\n"
                        << "this->zeros    = tzeros;\n"
                        << "this->fzeros   = tfzeros;\n"
                        << "}\n"
                        << "if(&njacobian!=&(this->jacobian)){\n"
                        << "this->jacobian = tjacobian;\n"
                        << "}\n"
                        << "}\n\n";
  }

  std::string ImplicitDSLBase::getVectorMappingClass(
      const VariableDescription& v) const {
    const auto f = this->getTypeFlag(v.type);
    if (f == SupportedTypes::Stensor) {
      if (v.arraySize == 1u) {
        return "StensorFromTinyVectorView";
      } else {
        return "TinyVectorOfStensorFromTinyVectorView";
      }
    }
    this->throwRuntimeError("ImplicitDSLBase::getVectorMappingClass",
                            "unsupported type for variable '" + v.name + "'");
  }  // end of ImplicitDSLBase::getVectorMappingClass

  void ImplicitDSLBase::writeBehaviourIntegrator(const Hypothesis h) {
    const auto btype = this->mb.getBehaviourTypeFlag();
    const auto& d = this->mb.getBehaviourData(h);
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    SupportedTypes::TypeSize n3;
    for (const auto& v : d.getIntegrationVariables()) {
      n2 += this->getTypeSize(v.type, v.arraySize);
    }
    this->checkBehaviourFile();
    this->behaviourFile << "/*!\n";
    this->behaviourFile
        << "* \\brief Integrate behaviour law over the time step\n";
    this->behaviourFile << "*/\n";
    this->behaviourFile << "IntegrationResult\n";
    if (this->mb.hasAttribute(h, BehaviourData::hasConsistentTangentOperator)) {
      this->behaviourFile
          << "integrate(const SMFlag smflag,const SMType smt) override{\n";
    } else {
      if ((this->mb.getBehaviourType() ==
           BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
          (this->mb.getBehaviourType() ==
           BehaviourDescription::COHESIVEZONEMODEL)) {
        this->behaviourFile
            << "integrate(const SMFlag smflag,const SMType smt) override{\n";
      } else {
        this->behaviourFile
            << "integrate(const SMFlag,const SMType smt) override{\n";
      }
    }
    this->behaviourFile << "using namespace std;\n";
    this->behaviourFile << "using namespace tfel::math;\n";
    writeMaterialLaws("ImplicitDSLBase::writeBehaviourParserSpecificMembers",
                      this->behaviourFile, this->mb.getMaterialLaws());
    if ((this->mb.getBehaviourType() ==
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
        (this->mb.getBehaviourType() ==
         BehaviourDescription::COHESIVEZONEMODEL)) {
      if (this->mb.useQt()) {
        this->behaviourFile
            << "if(smflag!=MechanicalBehaviour<" << btype
            << ",hypothesis,Type,use_qt>::STANDARDTANGENTOPERATOR){\n"
            << "throw(runtime_error(\"invalid tangent operator flag\"));\n"
            << "}\n";
      } else {
        this->behaviourFile
            << "if(smflag!=MechanicalBehaviour<" << btype
            << ",hypothesis,Type,false>::STANDARDTANGENTOPERATOR){\n"
            << "throw(runtime_error(\"invalid tangent operator flag\"));\n"
            << "}\n";
      }
    }
    if (this->mb.hasCode(h, BehaviourData::ComputePredictor)) {
      this->behaviourFile << this->mb.getCode(h,
                                              BehaviourData::ComputePredictor)
                          << '\n';
    }
    if (this->mb.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfilingBegin(
          this->behaviourFile, mb.getClassName(), BehaviourData::Integrator);
    }
    this->solver->writeResolutionAlgorithm(this->behaviourFile, this->mb, h);
    this->behaviourFile << "if(this->iter==this->iterMax){\n";
    if (getDebugMode()) {
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : no convergence after \" "
          << "<< this->iter << \" iterations\"<< endl << endl;\n";
      this->behaviourFile << "cout << *this << endl;\n";
    }
    if (this->mb.useQt()) {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    if (getDebugMode()) {
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : convergence after \" "
          << "<< this->iter << \" iterations\"<< endl << endl;\n";
    }
    if (this->mb.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfilingEnd(this->behaviourFile);
    }
    for (const auto& v : d.getIntegrationVariables()) {
      if (this->mb.hasAttribute(h, v.name + "_normalisation_factor")) {
        const auto& nf = this->mb.getAttribute<std::string>(
            h, v.name + "_normalisation_factor");
        this->behaviourFile << "this->d" << v.name << " *= " << nf << ";\n";
      }
    }
    this->behaviourFile << "this->updateIntegrationVariables();\n";
    this->behaviourFile << "this->updateStateVariables();\n";
    if (this->mb.hasCode(h, BehaviourData::ComputeFinalStress)) {
      this->behaviourFile << "this->computeFinalStress();\n";
    }
    this->behaviourFile << "this->updateAuxiliaryStateVariables();\n";
    for (const auto& b : d.getBounds()) {
      if (b.varCategory == BoundsDescription::StateVariable) {
        b.writeBoundsChecks(this->behaviourFile);
      }
    }
    this->behaviourFile << "if(smt!=NOSTIFFNESSREQUESTED){\n";
    if (this->mb.hasAttribute(h, BehaviourData::hasConsistentTangentOperator)) {
      if (this->mb.getBehaviourType() ==
          BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
        this->behaviourFile
            << "if(!this->computeConsistentTangentOperator(smflag,smt)){\n";
      } else {
        this->behaviourFile
            << "if(!this->computeConsistentTangentOperator(smt)){\n";
      }
      if (this->mb.useQt()) {
        this->behaviourFile << "return MechanicalBehaviour<" << btype
                            << ",hypothesis,Type,use_qt>::FAILURE;\n";
      } else {
        this->behaviourFile << "return MechanicalBehaviour<" << btype
                            << ",hypothesis,Type,false>::FAILURE;\n";
      }
      this->behaviourFile << "}\n";
    } else {
      this->behaviourFile << "string msg(\"" << this->mb.getClassName()
                          << "::integrate : \");\n";
      this->behaviourFile << "msg +=\"unimplemented feature\";\n";
      this->behaviourFile << "throw(runtime_error(msg));\n";
    }
    this->behaviourFile << "}\n";
    if (this->mb.useQt()) {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,use_qt>::SUCCESS;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,false>::SUCCESS;\n";
    }
    this->behaviourFile << "} // end of " << this->mb.getClassName()
                        << "::integrate\n\n";
    this->writeComputeFdF(h);
  }

  void ImplicitDSLBase::writeComputeFdF(const Hypothesis h) {
    const auto& d = this->mb.getBehaviourData(h);
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    SupportedTypes::TypeSize n3;
    for (const auto& v : d.getIntegrationVariables()) {
      n2 += this->getTypeSize(v.type, v.arraySize);
    }
    this->behaviourFile << "/*!\n"
                        << "* \\brief compute fzeros and jacobian\n"
                        << "*/\n"
                        << "bool\n"
                        << "computeFdF(void){\n"
                        << "using namespace std;\n"
                        << "using namespace tfel::math;\n"
                        << "using std::vector;\n";
    if (this->mb.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfiling(this->behaviourFile, mb.getClassName(),
                                        "ComputeFdF");
    }
    writeMaterialLaws("ImplicitDSLBase::writeBehaviourParserSpecificMembers",
                      this->behaviourFile, this->mb.getMaterialLaws());
    n = SupportedTypes::TypeSize();
    for (const auto& v : d.getIntegrationVariables()) {
      if (this->getTypeFlag(v.type) == SupportedTypes::Scalar) {
        if (v.arraySize == 1u) {
          this->behaviourFile << "real& f" << v.name << "(this->fzeros(" << n
                              << "));\n";
        } else {
          this->behaviourFile
              << "typename tfel::math::TinyVectorFromTinyVectorView<"
              << v.arraySize << "," << n2 << "," << n << ",real,false>::type"
              << " f" << v.name << "(this->fzeros);\n";
        }
      } else {
        if (v.arraySize == 1u) {
          this->behaviourFile
              << "typename tfel::math::" << this->getVectorMappingClass(v)
              << "<N," << n2 << "," << n << ",real>::type"
              << " f" << v.name << "(this->fzeros);\n";
        } else {
          this->behaviourFile
              << "typename tfel::math::" << this->getVectorMappingClass(v)
              << "<N," << n2 << "," << n << "," << v.arraySize << ",real>::type"
              << " f" << v.name << "(this->fzeros);\n";
        }
      }
      n += this->getTypeSize(v.type, v.arraySize);
    }
    if ((this->solver->usesJacobian()) &&
        (!(this->solver->requiresNumericalJacobian()))) {
      n = SupportedTypes::TypeSize();
      for (const auto& v : d.getIntegrationVariables()) {
        n3 = SupportedTypes::TypeSize();
        for (const auto& v2 : d.getIntegrationVariables()) {
          if ((v.arraySize == 1u) && (v2.arraySize == 1u)) {
            this->behaviourFile << "// derivative of variable f" << v.name
                                << " by variable " << v2.name << "\n"
                                << NonLinearSystemSolverBase::getJacobianPart(
                                       this->mb, v, v2, n, n2, n3);
          }
          n3 += this->getTypeSize(v2.type, v2.arraySize);
        }
        n += this->getTypeSize(v.type, v.arraySize);
      }
    }
    if (this->solver
            ->requiresJacobianToBeReinitialisedToIdentityAtEachIterations()) {
      for (const auto& v : d.getIntegrationVariables()) {
        this->jacobianPartsUsedInIntegrator.insert("df" + v.name + "_dd" +
                                                   v.name);
      }
      this->behaviourFile
          << "// setting jacobian to identity\n"
          << "std::fill(this->jacobian.begin(),this->jacobian.end(),real(0));\n"
          << "for(unsigned short idx = 0; idx!=" << n << ";++idx){\n"
          << "this->jacobian(idx,idx)= real(1);\n"
          << "}\n";
    }
    this->behaviourFile << "// setting f values to zeros\n"
                        << "this->fzeros = this->zeros;\n";
    for (const auto& v : d.getIntegrationVariables()) {
      if (this->mb.hasAttribute(h, v.name + "_normalisation_factor")) {
        const auto& nf = this->mb.getAttribute<std::string>(
            h, v.name + "_normalisation_factor");
        this->behaviourFile << "f" << v.name << " *= " << nf << ";\n";
      }
    }
    this->behaviourFile
        << this->mb.getCodeBlock(h, BehaviourData::Integrator).code << "\n";
    for (const auto& v : d.getIntegrationVariables()) {
      if (this->mb.hasAttribute(h, 'f' + v.name + "_normalisation_factor")) {
        const auto& nf = this->mb.getAttribute<std::string>(
            h, 'f' + v.name + "_normalisation_factor");
        this->behaviourFile << "f" << v.name << "*= real(1)/(" << nf << ");\n";
      }
    }
    if ((this->solver->usesJacobian()) &&
        (!(this->solver->requiresNumericalJacobian()))) {
      for (const auto& v : d.getIntegrationVariables()) {
        for (const auto& v2 : d.getIntegrationVariables()) {
          if ((v.arraySize == 1u) && (v2.arraySize == 1u)) {
            this->behaviourFile << "static_cast<void>(df" << v.name << "_dd"
                                << v2.name
                                << "); /* suppress potential warnings */\n";
          }
          if (this->jacobianPartsUsedInIntegrator.find("df" + v.name + "_dd" +
                                                       v2.name) !=
              this->jacobianPartsUsedInIntegrator.end()) {
            const bool bfnf = this->mb.hasAttribute(
                h, 'f' + v.name + "_normalisation_factor");
            const bool bvnf =
                this->mb.hasAttribute(h, v2.name + "_normalisation_factor");
            if (bfnf) {
              const auto& fnf = this->mb.getAttribute<std::string>(
                  h, 'f' + v.name + "_normalisation_factor");
              if (bvnf) {
                const auto& vnf = this->mb.getAttribute<std::string>(
                    h, v2.name + "_normalisation_factor");
                if ((v.arraySize != 1u) && (v2.arraySize != 1u)) {
                  this->behaviourFile
                      << "for(unsigned short idx=0;idx!=" << v.arraySize
                      << ";++idx){\n";
                  this->behaviourFile
                      << "for(unsigned short idx2=0;idx2!=" << v2.arraySize
                      << ";++idx2){\n";
                  this->behaviourFile << "df" << v.name << "_dd" << v2.name
                                      << "(idx,idx2) *= " << vnf << "/(" << fnf
                                      << ");\n";
                  this->behaviourFile << "}\n";
                  this->behaviourFile << "}\n";
                } else if ((v.arraySize != 1u) && (v2.arraySize == 1u)) {
                  this->behaviourFile
                      << "for(unsigned short idx=0;idx!=" << v.arraySize
                      << ";++idx){\n";
                  this->behaviourFile << "df" << v.name << "_dd" << v2.name
                                      << "(idx) *= " << vnf << "/(" << fnf
                                      << ");\n";
                  this->behaviourFile << "}\n";
                } else if ((v.arraySize == 1u) && (v2.arraySize != 1u)) {
                  this->behaviourFile
                      << "for(unsigned short idx=0;idx!=" << v2.arraySize
                      << ";++idx){\n";
                  this->behaviourFile << "df" << v.name << "_dd" << v2.name
                                      << "(idx) *= " << vnf << "/(" << fnf
                                      << ");\n";
                  this->behaviourFile << "}\n";
                } else if ((v.arraySize == 1u) && (v2.arraySize == 1u)) {
                  this->behaviourFile << "df" << v.name << "_dd" << v2.name
                                      << " *= " << vnf << "/(" << fnf << ");\n";
                }
              } else {
                if ((v.arraySize != 1u) && (v2.arraySize != 1u)) {
                  this->behaviourFile
                      << "for(unsigned short idx=0;idx!=" << v.arraySize
                      << ";++idx){\n";
                  this->behaviourFile
                      << "for(unsigned short idx2=0;idx2!=" << v2.arraySize
                      << ";++idx2){\n";
                  this->behaviourFile << "df" << v.name << "_dd" << v2.name
                                      << "(idx,idx2) *= "
                                      << "real(1)/(" << fnf << ");\n";
                  this->behaviourFile << "}\n";
                  this->behaviourFile << "}\n";
                } else if ((v.arraySize != 1u) && (v2.arraySize == 1u)) {
                  this->behaviourFile
                      << "for(unsigned short idx=0;idx!=" << v.arraySize
                      << ";++idx){\n";
                  this->behaviourFile << "df" << v.name << "_dd" << v2.name
                                      << "(idx) *= "
                                      << "real(1)/(" << fnf << ");\n";
                  this->behaviourFile << "}\n";
                } else if ((v.arraySize == 1u) && (v2.arraySize != 1u)) {
                  this->behaviourFile
                      << "for(unsigned short idx=0;idx!=" << v2.arraySize
                      << ";++idx){\n";
                  this->behaviourFile << "df" << v.name << "_dd" << v2.name
                                      << "(idx) *= "
                                      << "real(1)/(" << fnf << ");\n";
                  this->behaviourFile << "}\n";
                } else if ((v.arraySize == 1u) && (v2.arraySize == 1u)) {
                  this->behaviourFile << "df" << v.name << "_dd" << v2.name
                                      << " *= "
                                      << "real(1)/(" << fnf << ");\n";
                }
              }
            } else {
              if (bvnf) {
                const auto& vnf = this->mb.getAttribute<std::string>(
                    h, v2.name + "_normalisation_factor");
                if ((v.arraySize != 1u) && (v2.arraySize != 1u)) {
                  this->behaviourFile
                      << "for(unsigned short idx=0;idx!=" << v.arraySize
                      << ";++idx){\n";
                  this->behaviourFile
                      << "for(unsigned short idx2=0;idx2!=" << v2.arraySize
                      << ";++idx2){\n";
                  this->behaviourFile << "df" << v.name << "_dd" << v2.name
                                      << "(idx,idx2) *= " << vnf << ";\n";
                  this->behaviourFile << "}\n";
                  this->behaviourFile << "}\n";
                } else if ((v.arraySize != 1u) && (v2.arraySize == 1u)) {
                  this->behaviourFile
                      << "for(unsigned short idx=0;idx!=" << v.arraySize
                      << ";++idx){\n";
                  this->behaviourFile << "df" << v.name << "_dd" << v2.name
                                      << "(idx) *= " << vnf << ";\n";
                  this->behaviourFile << "}\n";
                } else if ((v.arraySize == 1u) && (v2.arraySize != 1u)) {
                  this->behaviourFile
                      << "for(unsigned short idx=0;idx!=" << v2.arraySize
                      << ";++idx){\n";
                  this->behaviourFile << "df" << v.name << "_dd" << v2.name
                                      << "(idx) *= " << vnf << ";\n";
                  this->behaviourFile << "}\n";
                } else if ((v.arraySize == 1u) && (v2.arraySize == 1u)) {
                  this->behaviourFile << "df" << v.name << "_dd" << v2.name
                                      << " *= " << vnf << ";\n";
                }
              }
            }
          }
        }
      }
    }
    this->behaviourFile << "return true;\n"
                        << "}\n\n";
  }  // end of ImplicitDSLBase::writeBehaviourIntegrator

  std::string ImplicitDSLBase::getIntegrationVariablesIncrementsInitializers(
      const VariableDescriptionContainer& ivs, const bool) const {
    SupportedTypes::TypeSize n;
    std::ostringstream init;
    bool first = true;
    for (const auto& v : ivs) {
      const auto flag = getTypeFlag(v.type);
      if (!first) {
        init << ",\n";
      }
      if ((flag != SupportedTypes::Scalar) &&
          (flag != SupportedTypes::Stensor)) {
        this->throwRuntimeError("getIntegrationVariablesIncrementsInitializers",
                                "internal error, tag unsupported");
      }
      if ((flag == SupportedTypes::Scalar) && (v.arraySize == 1u)) {
        init << "d" << v.name << "(this->zeros(" << n << "))";
      } else {
        init << "d" << v.name << "(this->zeros)";
      }
      n += this->getTypeSize(v.type, v.arraySize);
      first = false;
    }
    return init.str();
  }  // end of ImplicitDSLBase::getIntegrationVariableIncrementsInitializers

  std::string ImplicitDSLBase::getBehaviourConstructorsInitializers(
      const Hypothesis h) {
    auto init = BehaviourDSLCommon::getBehaviourConstructorsInitializers(h);
    init += (!init.empty()) ? ",\n" : "";
    if (this->mb.getAttribute(BehaviourDescription::computesStiffnessTensor,
                              false)) {
      if (this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep()) {
        init += "D_tdt(D),\n";
      }
    }
    for (const auto& ht : this->mb.getHillTensors()) {
      if ((this->mb.getBehaviourType() !=
           BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
          (this->mb.getBehaviourType() !=
           BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
        this->throwRuntimeError(
            "ImplicitDSLBase::writeBehaviourLocalVariablesInitialisation",
            "Hill tensors shall only be defined for finite strain "
            "or small strain behaviours");
      }
      if (this->mb.areMaterialPropertiesConstantDuringTheTimeStep(ht.c)) {
        init += ht.name + "_tdt(" + ht.name + "),\n";
      }
    }
    init += "zeros(real(0)),\nfzeros(real(0))";
    return init;
  }

  void ImplicitDSLBase::writeBehaviourParserSpecificInitializeMethodPart(
      const Hypothesis h) {
    const auto& d = this->mb.getBehaviourData(h);
    SupportedTypes::TypeSize n;
    this->checkBehaviourFile();
    for (const auto& v : d.getIntegrationVariables()) {
      n += this->getTypeSize(v.type, v.arraySize);
    }
    this->solver->writeSpecificInitializeMethodPart(this->behaviourFile, mb, h);
  }

  void ImplicitDSLBase::writeBehaviourIntegrationVariablesIncrements(
      const Hypothesis h) {
    const auto& d = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    SupportedTypes::TypeSize n;
    SupportedTypes::TypeSize n2;
    for (const auto& v : d.getIntegrationVariables()) {
      n2 += this->getTypeSize(v.type, v.arraySize);
    }
    for (const auto& v : d.getIntegrationVariables()) {
      if ((!getDebugMode()) && (v.lineNumber != 0u)) {
        this->behaviourFile << "#line " << v.lineNumber << " \""
                            << this->fileName << "\"\n";
      }
      if (this->getTypeFlag(v.type) == SupportedTypes::Scalar) {
        if (v.arraySize == 1u) {
          this->behaviourFile << "real& d" << v.name << ";\n";
        } else {
          this->behaviourFile
              << "typename tfel::math::TinyVectorFromTinyVectorView<"
              << v.arraySize << "," << n2 << "," << n << ",real,false>::type"
              << " d" << v.name << ";\n";
        }
      } else {
        if (v.arraySize == 1u) {
          this->behaviourFile
              << "typename tfel::math::" << this->getVectorMappingClass(v)
              << "<N," << n2 << "," << n << ",real>::type"
              << " d" << v.name << ";\n";
        } else {
          this->behaviourFile
              << "typename tfel::math::" << this->getVectorMappingClass(v)
              << "<N," << n2 << "," << n << "," << v.arraySize << ",real>::type"
              << " d" << v.name << ";\n";
        }
      }
      n += this->getTypeSize(v.type, v.arraySize);
    }
    this->behaviourFile << '\n';
  }

  void ImplicitDSLBase::endsInputFileProcessing() {
    using namespace tfel::glossary;
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (this->solver == nullptr) {
      const auto& f =
          NonLinearSystemSolverFactory::getNonLinearSystemSolverFactory();
      this->solver = f.getSolver("NewtonRaphson");
    }
    BehaviourDSLCommon::endsInputFileProcessing();
    if (this->mb.getAttribute(h, BehaviourData::compareToNumericalJacobian,
                              false)) {
      if ((!this->solver->usesJacobian()) ||
          (this->solver->requiresNumericalJacobian())) {
        this->throwRuntimeError(
            "ImplicitDSLBase::endsInputFileProcessing",
            "@CompareToNumericalJacobian can only be used with solver using "
            "an analytical jacobian (or an approximation of it");
      }
    }
    if (this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) {
      auto D = VariableDescription("StiffnessTensor", "D", 1u, 0u);
      D.description =
          "stiffness tensor computed from elastic "
          "material properties";
      this->mb.addLocalVariable(h, D, BehaviourData::ALREADYREGISTRED);
      auto D_tdt =
          this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep()
              ? VariableDescription("StiffnessTensor&", "D_tdt", 1u, 0u)
              : VariableDescription("StiffnessTensor", "D_tdt", 1u, 0u);
      D_tdt.description =
          "stiffness tensor computed from elastic "
          "material properties";
      this->mb.addLocalVariable(h, D_tdt, BehaviourData::ALREADYREGISTRED);
    }
    if ((!this->mb.getAttribute(BehaviourDescription::computesStiffnessTensor,
                                false)) &&
        (this->mb.getElasticSymmetryType() == ISOTROPIC) &&
        (this->mb.areElasticMaterialPropertiesDefined())) {
      auto add_lv = [&h](BehaviourDescription& bd, const std::string& t,
                         const std::string& n, const std::string& g,
                         const std::string d) {
        auto r = bd.checkVariableExistence(n, "Parameter", false);
        if (!r.first) {
          VariableDescription v(t, n, 1u, 0u);
          v.description = d;
          bd.addLocalVariable(h, v, BehaviourData::UNREGISTRED);
        } else {
          if (!r.second) {
            throw(
                std::runtime_error("ImplicitDSLBase::endsInputFileProcessing: "
                                   "Parameter '" +
                                   n + "' is not defined for all hypotheses"));
          }
          if (!g.empty()) {
            bd.checkVariableGlossaryName(n, g);
          }
        }
      };
      add_lv(this->mb, "stress", "young", Glossary::YoungModulus,
             "Young modulus at t+theta*dt");
      add_lv(this->mb, "real", "nu", Glossary::PoissonRatio,
             "Poisson ratio at t+theta*dt");
      add_lv(this->mb, "stress", "lambda", Glossary::FirstLameCoefficient,
             "first Lamé coefficient at t+theta*dt");
      add_lv(this->mb, "stress", "mu", Glossary::ShearModulus,
             "shear modulus at t+theta*dt");
      add_lv(this->mb, "stress", "young_tdt", "", "Young modulus at t+dt");
      add_lv(this->mb, "real", "nu_tdt", "", "Poisson ratio at t+dt");
      add_lv(this->mb, "stress", "lambda_tdt", "",
             "first Lamé coefficient at t+dt");
      add_lv(this->mb, "stress", "mu_tdt", "", "shear modulus at t+dt");
    }
    for (const auto& ht : this->mb.getHillTensors()) {
      if ((this->mb.getBehaviourType() !=
           BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
          (this->mb.getBehaviourType() !=
           BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
        this->throwRuntimeError(
            "ImplicitDSLBase::writeBehaviourLocalVariablesInitialisation",
            "Hill tensors shall only be defined for finite strain "
            "or small strain behaviours");
      }
      const auto hn = ht.name + "_tdt";
      auto H_tdt = this->mb.areMaterialPropertiesConstantDuringTheTimeStep(ht.c)
                       ? VariableDescription("tfel::math::st2tost2<N,stress>&",
                                             hn, 1u, 0u)
                       : VariableDescription("tfel::math::st2tost2<N,stress>",
                                             hn, 1u, 0u);
      H_tdt.description =
          "Hill tensor '" + ht.name + "' at the end of the time step";
      this->mb.addLocalVariable(h, H_tdt);
    }
    // create the compute final stress code is necessary
    this->setComputeFinalStressFromComputeFinalStressCandidateIfNecessary();
    // creating default parameters if not explicitely specified by the user
    if (!this->mb.hasParameter(h, "epsilon")) {
      VariableDescription e("real", "epsilon", 1u, 0u);
      e.description =
          "value used to stop the iteration of the implicit algorithm";
      this->mb.addParameter(h, e, BehaviourData::ALREADYREGISTRED);
      this->mb.setEntryName(h, "epsilon", "epsilon");
      this->mb.setParameterDefaultValue(h, "epsilon", 1.e-8);
    }
    if (!this->mb.hasParameter(h, "theta")) {
      VariableDescription tv("real", "theta", 1u, 0u);
      tv.description = "theta value used by the implicit scheme";
      this->mb.addParameter(h, tv, BehaviourData::ALREADYREGISTRED);
      this->mb.setEntryName(h, "theta", "theta");
      this->mb.setParameterDefaultValue(h, "theta", 0.5);
    }
    this->solver->endsInputFileProcessing(this->mb);
    if ((this->mb.getAttribute(h, BehaviourData::compareToNumericalJacobian,
                               false)) ||
        (this->solver->usesJacobian())) {
      const std::string nje = "numerical_jacobian_epsilon";
      if (!this->mb.hasParameter(h, nje)) {
        const double eps =
            0.1 * this->mb.getFloattingPointParameterDefaultValue(h, "epsilon");
        VariableDescription v("real", nje, 1u, 0u);
        v.description =
            "perturbation value used to compute a numerical "
            "approximation of the jacobian";
        this->mb.addParameter(h, v, BehaviourData::ALREADYREGISTRED);
        this->mb.setParameterDefaultValue(h, nje, eps);
      }
    }
    if (!this->mb.hasParameter(h, "iterMax")) {
      unsigned short iterMax = 100u;
      VariableDescription v("ushort", "iterMax", 1u, 0u);
      v.description = "maximum number of iterations allowed";
      this->mb.addParameter(h, v, BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(h, "iterMax", iterMax);
    }
    if (this->mb.getAttribute(h, BehaviourData::compareToNumericalJacobian,
                              false)) {
      if (!this->mb.hasParameter(h, "jacobianComparisonCriterion")) {
        VariableDescription v("real", "jacobianComparisonCriterion", 1u, 0u);
        v.description =
            "criterion value used to compare the jacobian "
            "provided by the user to its numerical approximation";
        this->mb.addParameter(h, v, BehaviourData::ALREADYREGISTRED);
        this->mb.setParameterDefaultValue(
            h, "jacobianComparisonCriterion",
            this->mb.getFloattingPointParameterDefaultValue(h, "epsilon"));
      }
    }
    // correct prediction to take into account normalisation factors
    const std::set<Hypothesis> mh(this->mb.getDistinctModellingHypotheses());
    for (const auto& elem : mh) {
      if (this->mb.hasCode(elem, BehaviourData::ComputePredictor)) {
        CodeBlock predictor;
        const auto& d = this->mb.getBehaviourData(elem);
        for (const auto& v : d.getIntegrationVariables()) {
          if (this->integrationVariablesIncrementsUsedInPredictor.find(
                  'd' + v.name) !=
              this->integrationVariablesIncrementsUsedInPredictor.end()) {
            if (this->mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = this->mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              predictor.code += "this->d" + v.name + " /= " + nf + ";\n";
            }
          }
        }
        this->mb.setCode(elem, BehaviourData::ComputePredictor, predictor,
                         BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
      }
    }
    if (!this->mb.areAllMechanicalDataSpecialised()) {
      if (this->mb.hasCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                           BehaviourData::ComputePredictor)) {
        CodeBlock predictor;
        const auto& d =
            this->mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
        for (const auto& v : d.getIntegrationVariables()) {
          if (this->integrationVariablesIncrementsUsedInPredictor.find(
                  'd' + v.name) !=
              this->integrationVariablesIncrementsUsedInPredictor.end()) {
            if (this->mb.hasAttribute(h, v.name + "_normalisation_factor")) {
              const auto& nf = this->mb.getAttribute<std::string>(
                  h, v.name + "_normalisation_factor");
              predictor.code += "this->d" + v.name + " /= " + nf + ";\n";
            }
          }
        }
        this->mb.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                         BehaviourData::ComputePredictor, predictor,
                         BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
      }
    }
    // minimal tangent operator
    this->setMinimalTangentOperator();
  }  // end of ImplicitDSLBase::endsInputFileProcessing(void)

  ImplicitDSLBase::~ImplicitDSLBase() = default;

}  // end of namespace mfront
