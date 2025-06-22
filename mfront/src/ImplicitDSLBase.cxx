/*!
 * \file   mfront/src/ImplicitDSLBase.cxx
 * \brief
 *
 * \author Thomas Helfer
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

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/UnicodeSupport/UnicodeSupport.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/NonLinearSystemSolverBase.hxx"
#include "MFront/NonLinearSystemSolverFactory.hxx"
#include "MFront/UserDefinedNonLinearSystemSolver.hxx"
#include "MFront/PerformanceProfiling.hxx"
#include "MFront/AbstractBehaviourBrick.hxx"
#include "MFront/ImplicitCodeGeneratorBase.hxx"
#include "MFront/ImplicitDSLBase.hxx"

namespace mfront {

  ImplicitDSLBase::ImplicitDSLBase(const DSLOptions& opts)
      : BehaviourDSLBase<ImplicitDSLBase>(opts) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    // dynamically allocated vectors are not yet allowed in implicit
    // parsers
    this->mb.areDynamicallyAllocatedVectorsAllowed(false);
    // parameters
    this->reserveName("epsilon");
    this->reserveName("\u03B5");
    this->reserveName("theta");
    this->reserveName("\u03B8");
    this->reserveName("iterMax");
    this->reserveName("numerical_jacobian_epsilon");
    this->reserveName("maximum_increment_value_per_iteration");
    this->reserveName("jacobianComparisonCriterion");
    this->reserveName("perturbatedSystemEvaluation");
    // additional reserve name
    this->reserveName("vect_e");
    this->reserveName("tzeros");
    this->reserveName("zeros_1");
    this->reserveName("jacobian_invert");
    this->reserveName("njacobian");
    this->reserveName("partial_jacobian");
    this->reserveName("jacobian_permutation");
    this->reserveName("error_p");
    this->reserveName("idx");
    this->reserveName("idx2");
    this->reserveName("idx3");
    this->reserveName("getPartialJacobianInvert");
    this->reserveName("GetPartialJacobianInvert");
    this->reserveName("TinyMatrixSolve");
    this->mb.registerMemberName(uh, "iter");
    this->mb.registerMemberName(uh, "zeros");
    this->mb.registerMemberName(uh, "delta_zeros");
    this->mb.registerMemberName(uh, "fzeros");
    this->mb.registerMemberName(uh, "jacobian");
    this->mb.registerMemberName(uh, "computePartialJacobianInvert");
    this->mb.registerMemberName(uh, "computeNumericalJacobian");
    this->mb.registerMemberName(uh, "additionalConvergenceChecks");
    // CallBacks
    this->registerNewCallBack(
        "@UsableInPurelyImplicitResolution",
        &ImplicitDSLBase::treatUsableInPurelyImplicitResolution);
    this->registerNewCallBack("@MaterialLaw",
                              &ImplicitDSLBase::treatMaterialLaw);
    this->registerNewCallBack("@Predictor", &ImplicitDSLBase::treatPredictor);
    this->registerNewCallBack("@Theta", &ImplicitDSLBase::treatTheta);
    this->registerNewCallBack("@Epsilon", &ImplicitDSLBase::treatEpsilon);
    this->registerNewCallBack(
        "@AdditionalConvergenceChecks",
        &ImplicitDSLBase::treatAdditionalConvergenceChecks);
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
    this->registerNewCallBack("@ProcessNewCorrection",
                              &ImplicitDSLBase::treatProcessNewCorrection);
    this->registerNewCallBack("@RejectCurrentCorrection",
                              &ImplicitDSLBase::treatRejectCurrentCorrection);
    this->registerNewCallBack("@ProcessNewEstimate",
                              &ImplicitDSLBase::treatProcessNewEstimate);
    this->registerNewCallBack(
        "@CompareToNumericalJacobian",
        &ImplicitDSLBase::treatCompareToNumericalJacobian);
    this->registerNewCallBack(
        "@JacobianComparisonCriterion",
        &ImplicitDSLBase::treatJacobianComparisonCriterion);
    this->registerNewCallBack(
        "@JacobianComparisonCriterium",
        &ImplicitDSLBase::treatJacobianComparisonCriterion);
    this->registerNewCallBack(
        "@MaximumIncrementValuePerIteration",
        &ImplicitDSLBase::treatMaximumIncrementValuePerIteration);
    this->registerNewCallBack("@IntegrationVariable",
                              &ImplicitDSLBase::treatIntegrationVariable);
    this->registerNewCallBack("@ComputeStiffnessTensor",
                              &ImplicitDSLBase::treatComputeStiffnessTensor);
    this->registerNewCallBack("@ElasticMaterialProperties",
                              &ImplicitDSLBase::treatElasticMaterialProperties);
    this->registerNewCallBack(
        "@NumericallyComputedJacobianBlocks",
        &ImplicitDSLBase::treatNumericallyComputedJacobianBlocks);
    this->registerNewCallBack("@HillTensor", &ImplicitDSLBase::treatHillTensor);
    this->disableCallBack("@ComputedVar");
    //    this->disableCallBack("@UseQt");
    this->mb.setIntegrationScheme(BehaviourDescription::IMPLICITSCHEME);
  }  // end of ImplicitDSLBase

  std::unique_ptr<AbstractBehaviourCodeGenerator>
  ImplicitDSLBase::getCodeGenerator() const {
    return std::make_unique<ImplicitCodeGeneratorBase>(
        this->fd, this->mb, this->interfaces, *(this->solver),
        this->jacobianPartsUsedInIntegrator,
        this->integrationVariablesIncrementsUsedInPredictor);
  }  // end of getCodeGenerator

  const NonLinearSystemSolver& ImplicitDSLBase::getSolver() const {
    if (this->solver == nullptr) {
      this->throwRuntimeError("ImplicitBase::getSolver", "no solver defined");
    }
    return *(this->solver);
  }

  std::string ImplicitDSLBase::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions& o) const {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (c == BehaviourData::ComputePredictionOperator) {
      return "@PredictionOperator{}\n";
    } else if (c == BehaviourData::ComputeThermodynamicForces) {
      return "@ComputeStress{}\n";
    } else if (c == BehaviourData::Integrator) {
      // implicit system
      auto ivs = this->mb.getBehaviourData(h).getIntegrationVariables();
      const auto mivs = [this] {
        auto livs = this->mb.getBehaviourData(h).getIntegrationVariables();
        // remove managed integration variables
        const auto mivs_names = [this] {
          auto lmivs = std::vector<std::string>{};
          for (const auto& pb : this->bricks) {
            //! \return a description of the brick
            const auto d = pb->getDescription();
            lmivs.insert(lmivs.end(), d.managedIntegrationVariables.begin(),
                         d.managedIntegrationVariables.end());
          }
          return lmivs;
        }();
        livs.erase(std::remove_if(
                       livs.begin(), livs.end(),
                       [&mivs_names](const VariableDescription& v) {
                         return std::find(mivs_names.begin(), mivs_names.end(),
                                          v.name) != mivs_names.end();
                       }),
                   livs.end());
        return livs;
      }();
      if (ivs.empty()) {
        return "@Integrator{}\n";
      }
      auto i = std::string("@Integrator{\n");
      for (const auto& v1 : mivs) {
        const auto v1n = o.useUnicodeSymbols ? displayName(v1) : v1.name;
        i += "// implicit equation associated with variable " + v1n + "\n";
        if (v1.arraySize == 1u) {
          i += "f" + v1n + " += ;\n";
          i += "// jacobian blocks\n";
          if ((this->solver != nullptr) && (this->solver->usesJacobian())) {
            for (const auto& v2 : ivs) {
              const auto v2n = o.useUnicodeSymbols ? displayName(v2) : v2.name;
              const auto j = o.useUnicodeSymbols
                                 ? "\u2202f" + v1n + "\u2215\u2202\u0394" + v2n
                                 : "df" + v1n + "_d" + v2n;
              const auto op = v1.name == v2.name ? "+=" : "=";
              if (v2.arraySize == 1u) {
                i += j + " " + op + " ;\n";
              } else {
                i += "for(ushort i = 0; i != " + std::to_string(v2.arraySize) +
                     ";++i){\n";
                i += j + "(i) " + op + " ;\n";
                i += "}\n";
              }
            }
          }
        } else {
          i += "for(ushort i = 0; i != " + std::to_string(v1.arraySize) +
               ";++i){\n";
          i += "f" + v1n + "(i) += ;\n";
          i += "// jacobian blocks\n";
          if ((this->solver != nullptr) && (this->solver->usesJacobian())) {
            for (const auto& v2 : ivs) {
              const auto v2n = o.useUnicodeSymbols ? displayName(v2) : v2.name;
              const auto j = o.useUnicodeSymbols
                                 ? "\u2202f" + v1n + "\u2215\u2202\u0394" + v2n
                                 : "df" + v1n + "_d" + v2n;
              const auto op = v1.name == v2.name ? "+=" : "=";
              if (v2.arraySize == 1u) {
                i += j + "(i) " + op + " ;\n";
              } else {
                i += "for(ushort j = 0; j != " + std::to_string(v2.arraySize) +
                     ";++j){\n";
                i += j + "(i,j) " + op + " ;\n";
                i += "}\n";
              }
            }
          }
          i += "}\n";
        }
      }
      i += "}\n";
      return i;
    } else if (c == BehaviourData::ComputeTangentOperator) {
      return "@TangentOperator{}\n";
    }
    return "";
  }  // end of getCodeBlockTemplate

  void ImplicitDSLBase::treatUnknownKeyword() {
    --(this->current);
    const auto& key = this->current->value;
    ++(this->current);
    if (this->solver != nullptr) {
      const auto r = this->solver->treatSpecificKeywords(
          this->mb, key, this->current, this->tokens.end());
      if (r.first) {
        this->current = r.second;
        return;
      }
    }
    BehaviourDSLCommon::treatUnknownKeyword();
  }  // end of treatUnknownKeyword

  void ImplicitDSLBase::treatProcessNewCorrection() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::ProcessNewCorrection, m, true, true);
  }  // end of treatProcessNewCorrection

  void ImplicitDSLBase::treatRejectCurrentCorrection() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::RejectCurrentCorrection, m, true, true);
  }  // end of treatRejectCurrentCorrection

  void ImplicitDSLBase::treatProcessNewEstimate() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::ProcessNewEstimate, m, true, true);
  }  // end of treatProcessNewEstimate

  void ImplicitDSLBase::treatStateVariable() {
    VariableDescriptionContainer v;
    auto hs = std::set<Hypothesis>{};
    this->readVariableList(v, hs, &BehaviourDescription::addStateVariables,
                           true);
    for (const auto h : hs) {
      for (const auto& iv : v) {
        this->mb.reserveName(h, "f" + iv.name);
        this->mb.reserveName(h, iv.name + "_offset");
        this->mb.reserveName(h, "delta_d" + iv.name);
      }
    }
  }

  void ImplicitDSLBase::treatIntegrationVariable() {
    VariableDescriptionContainer v;
    auto hs = std::set<Hypothesis>{};
    this->readVariableList(
        v, hs, &BehaviourDescription::addIntegrationVariables, true);
    for (const auto h : hs) {
      for (const auto& iv : v) {
        this->mb.reserveName(h, "f" + iv.name);
        this->mb.reserveName(h, iv.name + "_offset");
        this->mb.reserveName(h, "delta_d" + iv.name);
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
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::InitializeJacobian, m, true, true);
  }  // end of treatInitJacobian

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
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::InitializeJacobianInvert,  //
                         m, true, true);
  }  // end of treatInitJacobianInvert

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
        ++(this->current);
        this->readSpecifiedToken("ImplicitDSLBase::treatUnknowVariableMethod",
                                 ")");
        this->mb.setAttribute(h, n + "_normalisation_factor", var);
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
        ++(this->current);
        this->checkNotEndOfFile("ImplicitDSLBase::treatUnknowVariableMethod");
        this->readSpecifiedToken("ImplicitDSLBase::treatUnknowVariableMethod",
                                 ")");
        VariableDescription miv(
            "real", n + "_maximum_increment_value_per_iteration", 1u, 0u);
        miv.description =
            "maximum increment allowed per iteration for variable '" + n + "'";
        this->mb.addParameter(h, miv);
        this->mb.setParameterDefaultValue(
            h, n + "_maximum_increment_value_per_iteration", value);
        return;
      }
    }
    BehaviourDSLCommon::treatUnknownVariableMethod(h, n);
  }  // end of treatUnknowVariableMethod

  bool ImplicitDSLBase::isCallableVariable(const Hypothesis h,
                                           const std::string& n) const {
    if (n.empty()) {
      this->throwRuntimeError("ImplicitDSLBase::isCallableVariable",
                              "empty variable name '" + n + "'");
    }
    if ((n[0] == 'f') && (this->mb.isIntegrationVariableName(h, n.substr(1)))) {
      return true;
    }
    return BehaviourDSLCommon::isCallableVariable(h, n);
  }  // end of isCallableVariable

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
  }  // end of treatCompareToNumericalJacobian

  void ImplicitDSLBase::treatJacobianComparisonCriterion() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (!this->mb.getAttribute(h, BehaviourData::compareToNumericalJacobian,
                               false)) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatJacobianComparisonCriterion",
          "must call '@CompareToNumericalJacobian' first");
    }
    this->checkNotEndOfFile("ImplicitDSLBase::treatJacobianComparisonCriterion",
                            "Cannot read jacobianComparisonCriterion value.");
    double jacobianComparisonCriterion = this->readDouble();
    if (jacobianComparisonCriterion < 0) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatJacobianComparisonCriterion",
          "JacobianComparisonCriterion value must be positive.");
    }
    this->readSpecifiedToken(
        "ImplicitDSLBase::treatJacobianComparisonCriterion", ";");
    this->mb.addParameter(
        h, VariableDescription("real", "jacobianComparisonCriterion", 1u, 0u),
        BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "jacobianComparisonCriterion",
                                      jacobianComparisonCriterion);
  }  // ImplicitDSLBase::treatJacobianComparisonCriterion

  void ImplicitDSLBase::setNonLinearSolver(
      std::shared_ptr<NonLinearSystemSolver> s, const std::string& name) {
    this->solver = s;
    for (const auto& n : this->solver->getReservedNames()) {
      this->reserveName(n);
    }
    for (const auto& n : this->solver->getMemberNames()) {
      constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      this->mb.registerMemberName(uh, n);
    }
    this->mb.setAttribute(BehaviourData::algorithm, name, false);
  }  // end of setNonLinearSolver

  void ImplicitDSLBase::setNonLinearSolver(const std::string& s) {
    const auto& f =
        NonLinearSystemSolverFactory::getNonLinearSystemSolverFactory();
    this->setNonLinearSolver(f.getSolver(s), s);
  }  // end of setNonLinearSolver

  void ImplicitDSLBase::treatAlgorithm() {
    if (this->solver != nullptr) {
      this->throwRuntimeError("ImplicitDSLBase::treatAlgorithm",
                              "an algorithm has already been defined.");
    }
    this->checkNotEndOfFile("ImplicitDSLBase::treatAlgorithm",
                            "Cannot read algorithm name.");
    const auto& s = this->current->value;
    ++this->current;
    this->checkNotEndOfFile("ImplicitDSLBase::treatAlgorithm");
    if (s == "UserDefined") {
      const auto d = [this] {
        using namespace tfel::utilities;
        DataParsingOptions o;
        o.allowMultipleKeysInMap = true;
        const auto opts =
            Data::read(this->current, this->tokens.end(), o).get<DataMap>();
        return opts;
      }();
      this->setNonLinearSolver(
          std::make_shared<UserDefinedNonLinearSystemSolver>(d), "UserDefined");
      this->readSpecifiedToken("ImplicitDSLBase::treatAlgorithm", ";");
    } else {
      this->readSpecifiedToken("ImplicitDSLBase::treatAlgorithm", ";");
      this->setNonLinearSolver(s);
    }
  }  // end of treatAlgorithm

  void ImplicitDSLBase::treatTheta() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile("ImplicitDSLBase::treatTheta",
                            "Cannot read theta value.");
    const auto theta = this->readDouble();
    if ((theta < 0.) || (theta > 1.)) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatTheta",
          "Theta value must be positive and smaller than 1.");
    }
    this->readSpecifiedToken("ImplicitDSLBase::treatTheta", ";");
    VariableDescription tv("NumericType", "\u03B8", "theta", 1u, 0u);
    tv.description = "theta value used by the implicit scheme";
    this->mb.addParameter(h, tv, BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "theta", theta);
    this->mb.setEntryName(h, "theta", "theta");
  }  // end of treatTheta

  void ImplicitDSLBase::treatEpsilon() {
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile("ImplicitDSLBase::treatEpsilon",
                            "Cannot read epsilon value.");
    const auto epsilon = this->readDouble();
    if (epsilon < 0) {
      this->throwRuntimeError("ImplicitDSLBase::treatEpsilon",
                              "Epsilon value must be positive.");
    }
    this->readSpecifiedToken("ImplicitDSLBase::treatEpsilon", ";");
    VariableDescription e("NumericType", "\u03B5", "epsilon", 1u, 0u);
    e.description =
        "value used to stop the iteration of the implicit algorithm";
    this->mb.addParameter(uh, e, BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(uh, "epsilon", epsilon);
    this->mb.setEntryName(uh, "epsilon", "epsilon");
  }  // ImplicitDSLBase::treatEpsilon

  void ImplicitDSLBase::treatAdditionalConvergenceChecks() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::AdditionalConvergenceChecks,  //
                         m, true, true);
  }  // end of treatAdditionalConvergenceChecks()

  void
  ImplicitDSLBase::treatPerturbationValueForNumericalJacobianComputation() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile(
        "ImplicitDSLBase::"
        "treatPerturbationValueForNumericalJacobianComputation",
        "Cannot read epsilon value.");
    const auto epsilon = this->readDouble();
    if (epsilon < 0) {
      this->throwRuntimeError(
          "ImplicitDSLBase::"
          "treatPerturbationValueForNumericalJacobianComputation",
          "Epsilon value must be positive.");
    }
    this->readSpecifiedToken(
        "ImplicitDSLBase::"
        "treatPerturbationValueForNumericalJacobianComputation",
        ";");
    VariableDescription e("NumericType", "numerical_jacobian_epsilon", 1u, 0u);
    e.description =
        "perturbation value used to compute a finite difference approximation "
        "of the jacobian";
    this->mb.addParameter(h, e, BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "numerical_jacobian_epsilon", epsilon);
  }  // ImplicitDSLBase::treatEpsilon

  void ImplicitDSLBase::treatIterMax() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto iterMax =
        this->readUnsignedShort("ImplicitDSLBase::treatIterMax");
    if (iterMax == 0) {
      this->throwRuntimeError("ImplicitDSLBase::treatIterMax",
                              "invalid value for parameter 'iterMax'");
    }
    this->readSpecifiedToken("ImplicitDSLBase::treatIterMax", ";");
    this->mb.addParameter(h, VariableDescription("ushort", "iterMax", 1u, 0u),
                          BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "iterMax", iterMax);
  }  // end of treatIterMax

  std::string ImplicitDSLBase::tangentOperatorVariableModifier(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    const auto& d = this->mb.getBehaviourData(h);
    if (d.isIntegrationVariableIncrementName(var)) {
      const auto v = var.substr(1);
      if (this->mb.hasAttribute(h, v + "_normalisation_factor")) {
        const auto& s = d.getStateVariableDescription(v);
        const auto& nf =
            this->mb.getAttribute<std::string>(h, v + "_normalisation_factor");
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
  }  // end of tangentOperatorVariableModifier

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
  }  // end of integratorVariableModifier

  std::string ImplicitDSLBase::computeThermodynamicForcesVariableModifier1(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    const auto& d = this->mb.getBehaviourData(h);
    if (this->mb.isGradientName(var) || (d.isExternalStateVariableName(var))) {
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
  }  // end of computeThermodynamicForcesVariableModifier1

  std::string ImplicitDSLBase::computeThermodynamicForcesVariableModifier2(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    const auto& d = this->mb.getBehaviourData(h);
    if ((this->mb.isGradientName(var)) ||
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
  }  // end of computeThermodynamicForcesVariableModifier2

  void ImplicitDSLBase::integratorAnalyser(const Hypothesis h,
                                           const std::string& w) {
    const auto is_jacobian_part = [this, h, &w] {
      const auto& d = this->mb.getBehaviourData(h);
      for (const auto& v1 : d.getIntegrationVariables()) {
        for (const auto& v2 : d.getIntegrationVariables()) {
          // symbolic names are not treated here, because they were already
          // treated previously
          if (w == "df" + v1.name + "_dd" + v2.name) {
            auto previous = this->current;
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
    }();
    if (is_jacobian_part) {
      this->jacobianPartsUsedInIntegrator.insert(w);
    }
  }  // end of integratorAnalyser

  void ImplicitDSLBase::predictorAnalyser(const Hypothesis h,
                                          const std::string& w) {
    const auto& d = this->mb.getBehaviourData(h);
    if (d.isIntegrationVariableIncrementName(w)) {
      this->integrationVariablesIncrementsUsedInPredictor.insert(w);
    }
  }  // end of predictorAnalyser

  void ImplicitDSLBase::treatIntegrator() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->integratorVariableModifier(h, sv, b);
        };
    std::function<void(CodeBlock&, const Hypothesis, const std::string&)> a =
        [this](CodeBlock&, const Hypothesis h, const std::string& sv) {
          this->integratorAnalyser(h, sv);
        };
    this->treatCodeBlock(BehaviourData::Integrator, m, a, true, true);
  }  // end of treatIntegrator

  void ImplicitDSLBase::treatPredictor() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    std::function<void(CodeBlock&, const Hypothesis, const std::string&)> a =
        [this](CodeBlock&, const Hypothesis h, const std::string& sv) {
          this->predictorAnalyser(h, sv);
        };
    this->treatCodeBlock(BehaviourData::ComputePredictor, m, a, true, true);
  }  // end of treatPredictor

  void ImplicitDSLBase::treatComputeThermodynamicForces() {
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
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m1 = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->computeThermodynamicForcesVariableModifier1(h, sv, b);
        };
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m2 = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->computeThermodynamicForcesVariableModifier2(h, sv, b);
        };
    this->treatCodeBlock(
        BehaviourData::ComputeThermodynamicForces,
        BehaviourData::ComputeFinalThermodynamicForcesCandidate,  //
        m1, m2, true, true);
  }  // end of treatComputeThermodynamicForces

  void ImplicitDSLBase::treatComputeFinalThermodynamicForces() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->computeThermodynamicForcesVariableModifier2(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::ComputeFinalThermodynamicForces, m,
                         true, true);
  }  // end of treatComputeFinalThermodynamicForces

  void ImplicitDSLBase::readTangentOperatorCodeBlock(const CodeBlockOptions& o,
                                                     const std::string& n) {
    this->treatUnsupportedCodeBlockOptions(o);
    // variable modifier
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& v, const bool b) {
          return this->tangentOperatorVariableModifier(h, v, b);
        };
    // analyser
    std::set<std::string> get_integration_variables_derivatives_names;
    std::map<std::string, VariableDescription>
        get_integration_variables_derivatives;
    for (const auto& to : this->mb.getTangentOperatorBlocks()) {
      const auto& name = to.second.name;
      get_integration_variables_derivatives
          ["getIntegrationVariablesDerivatives_" + name] = to.second;
      get_integration_variables_derivatives_names.insert(
          "getIntegrationVariablesDerivatives_" + name);
    }
    std::function<void(CodeBlock&, const Hypothesis, const std::string&)> a =
        [this, get_integration_variables_derivatives_names,
         get_integration_variables_derivatives](
            CodeBlock& cb, const Hypothesis h, const std::string& v) {
          using Derivative =
              std::pair<VariableDescription, VariableDescription>;
          const auto& d = this->mb.getBehaviourData(h);
          std::map<std::string, Derivative> implicit_equation_derivatives;
          std::map<std::string, Derivative> jacobian_invert_blocks;
          for (const auto& to : this->mb.getTangentOperatorBlocks()) {
            for (const auto& iv : d.getIntegrationVariables()) {
              const auto n1 = "df" + iv.name + "_dd" + to.second.name;
              const auto derivative = std::make_pair(iv, to.second);
              implicit_equation_derivatives.insert({n1, derivative});
            }
          }
          for (const auto& iv1 : d.getIntegrationVariables()) {
            for (const auto& iv2 : d.getIntegrationVariables()) {
              const auto n1 = "iJ_" + iv1.name + "_" + iv2.name;
              const auto derivative = std::make_pair(iv1, iv2);
              jacobian_invert_blocks.insert({n1, derivative});
            }
          }
          const auto an = CodeBlock::requires_jacobian_decomposition;
          const auto bn = CodeBlock::uses_get_partial_jacobian_invert;
          const auto cn = CodeBlock::used_get_integration_variables_derivatives;
          const auto dn = CodeBlock::
              used_implicit_equations_derivatives_with_respect_to_gradients_or_external_state_variables;
          const auto iJn = CodeBlock::used_jacobian_invert_blocks;
          const auto pd = implicit_equation_derivatives.find(v);
          const auto piJ = jacobian_invert_blocks.find(v);
          // the jacobian must be decomposed
          if ((v == "getPartialJacobianInvert") ||
              (pd != implicit_equation_derivatives.end()) ||
              (piJ != jacobian_invert_blocks.end()) ||
              (get_integration_variables_derivatives_names.count(v) != 0)) {
            if (cb.attributes.count(an) == 0) {
              cb.attributes[an] = true;
            } else {
              if (!cb.attributes[an].is<bool>()) {
                this->throwRuntimeError(
                    "ImplicitDSLBase::readTangentOperatorCodeBlock",
                    "invalid type for attribute '" + std::string(an) + "'");
              }
              if (!cb.attributes[an].get<bool>()) {
                this->throwRuntimeError(
                    "ImplicitDSLBase::readTangentOperatorCodeBlock",
                    "inconsistent value for attribute '" + std::string(an) +
                        "'");
              }
            }
          }
          //
          if (v == "getPartialJacobianInvert") {
            if (cb.attributes.count(bn) == 0) {
              cb.attributes[bn] = true;
            } else {
              if (!cb.attributes[bn].is<bool>()) {
                this->throwRuntimeError(
                    "ImplicitDSLBase::readTangentOperatorCodeBlock",
                    "invalid type for attribute '" + std::string(bn) + "'");
              }
              if (!cb.attributes[bn].get<bool>()) {
                this->throwRuntimeError(
                    "ImplicitDSLBase::readTangentOperatorCodeBlock",
                    "inconsistent value for attribute '" + std::string(bn) +
                        "'");
              }
            }
          }
          // a getIntegrationVariablesDerivative_X has been found
          if (get_integration_variables_derivatives_names.count(v) != 0) {
            const auto& dv = get_integration_variables_derivatives.at(v);
            // we check that at least one derivative of one implicit equation
            // has been defined
            if (cb.attributes.count(dn) == 0) {
              this->throwRuntimeError(
                  "ImplicitDSLBase::readTangentOperatorCodeBlock",
                  "no derivative of an implicit equation with respect "
                  "to '" +
                      displayName(dv) +
                      "' before the call to "
                      "'getIntegrationVariablesDerivatives_" +
                      displayName(dv) + "'");
            } else {
              if (!cb.attributes[dn].is<std::vector<Derivative>>()) {
                this->throwRuntimeError(
                    "ImplicitDSLBase::readTangentOperatorCodeBlock",
                    "invalid type for attribute '" + std::string(dn) + "'");
              }
              const auto& derivatives =
                  cb.attributes[dn].get<std::vector<Derivative>>();
              const auto p =
                  std::find_if(derivatives.begin(), derivatives.end(),
                               [&dv](const Derivative& d1) {
                                 return d1.second.name == dv.name;
                               });
              if (p == derivatives.end()) {
                this->throwRuntimeError(
                    "ImplicitDSLBase::readTangentOperatorCodeBlock",
                    "no derivative of an implicit equation with respect "
                    "to '" +
                        displayName(dv) +
                        "' before the call to "
                        "'getIntegrationVariablesDerivatives_" +
                        displayName(dv) + "'");
              }
            }
            if (cb.attributes.count(cn) == 0) {
              cb.attributes[cn] = std::vector<VariableDescription>(1u, dv);
            } else {
              if (!cb.attributes[cn].is<std::vector<VariableDescription>>()) {
                this->throwRuntimeError(
                    "ImplicitDSLBase::readTangentOperatorCodeBlock",
                    "invalid type for attribute '" + std::string(cn) + "'");
              }
              auto& variables =
                  cb.attributes[cn].get<std::vector<VariableDescription>>();
              const auto p = std::find_if(variables.begin(), variables.end(),
                                          [&dv](const VariableDescription& vv) {
                                            return vv.name == dv.name;
                                          });
              if (p == variables.end()) {
                variables.push_back(dv);
              }
            }
          }
          auto add_derivative_in_attribute = [this, &cb](
                                                 const char* const attribute,
                                                 const Derivative& derivative) {
            if (cb.attributes.count(attribute) == 0) {
              cb.attributes[attribute] =
                  std::vector<Derivative>(1u, derivative);
            } else {
              if (!cb.attributes[attribute].is<std::vector<Derivative>>()) {
                this->throwRuntimeError(
                    "ImplicitDSLBase::readTangentOperatorCodeBlock",
                    "invalid type for attribute '" + std::string(attribute) +
                        "'");
              }
              auto& derivatives =
                  cb.attributes[attribute].get<std::vector<Derivative>>();
              const auto p = std::find_if(
                  derivatives.begin(), derivatives.end(),
                  [&derivative](const Derivative& d1) {
                    return (d1.first.name == derivative.first.name) &&
                           (d1.second.name == derivative.second.name);
                  });
              if (p == derivatives.end()) {
                derivatives.push_back(derivative);
              }
            }
          };
          // a derivative of an implicit equation derivative has been found
          if (pd != implicit_equation_derivatives.end()) {
            add_derivative_in_attribute(dn, pd->second);
          }
          // the invert of a jacobian block has been found
          if (piJ != jacobian_invert_blocks.end()) {
            add_derivative_in_attribute(iJn, piJ->second);
          }
        };
    this->treatCodeBlock(o, n, m, a, true);
  }  // end of readTangentOperatorCodeBlock

  void ImplicitDSLBase::treatMaximumIncrementValuePerIteration() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile(
        "ImplicitDSLBase::treatMaximumIncrementValuePerIteration",
        "Cannot read value value.");
    const auto value = tfel::utilities::convert<double>(current->value);
    ++(this->current);
    if (value <= 0) {
      this->throwRuntimeError(
          "ImplicitDSLBase::treatMaximumIncrementValuePerIteration",
          "Value must be positive.");
    }
    this->readSpecifiedToken(
        "ImplicitDSLBase::MaximumIncrementValuePerIteration", ";");
    this->mb.addParameter(
        h,
        VariableDescription("real", "maximum_increment_value_per_iteration", 1u,
                            0u),
        BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(
        h, "maximum_increment_value_per_iteration", value);
  }  // end of treatMaximumIncrementValuePerIteration

  void ImplicitDSLBase::treatNumericallyComputedJacobianBlocks() {
    const std::string m =
        "ImplicitDSLBase::treatNumericallyComputedJacobianBlocks";
    auto throw_if = [this, m](const bool b, const std::string& msg) {
      if (b) {
        this->throwRuntimeError(m, msg);
      }
    };
    for (const auto& h : this->readHypothesesList()) {
      const auto as = this->readList(m, "{", "}", false);
      this->readSpecifiedToken(m, ";");
      throw_if(as.empty(), "no block defined");
      const auto jbs = [&as] {
        auto r = std::vector<std::string>(as.size());
        std::transform(as.begin(), as.end(), r.begin(),
                       [](const tfel::utilities::Token& t) { return t.value; });
        return r;
      }();
      std::for_each(jbs.begin(), jbs.end(), [throw_if](const std::string& jb) {
        throw_if(jb.empty(), "empty jacobian block");
        throw_if(jb.size() < 6, "invalid jacobian block '" + jb + "'");
        throw_if(jb[0] != 'd', "invalid jacobian block '" + jb + "'");
        throw_if(jb[1] != 'f', "invalid jacobian block '" + jb + "'");
        const auto p = jb.find('_');
        throw_if(p == std::string::npos, "invalid jacobian block '" + jb + "'");
        throw_if(p + 2 >= jb.size(), "invalid jacobian block '" + jb + "'");
        throw_if(jb[p + 1] != 'd', "invalid jacobian block '" + jb + "'");
        throw_if(jb[p + 2] != 'd', "invalid jacobian block '" + jb + "'");
        const auto n = jb.substr(2, p - 2);
        const auto d = jb.substr(p + 3);
        throw_if(n.empty(), "invalid jacobian block '" + jb + "'");
        throw_if(d.empty(), "invalid jacobian block '" + jb + "'");
      });
      for (const auto& jb : jbs) {
        throw_if(std::count(jbs.begin(), jbs.end(), jb) > 1,
                 "jacobian block '" + jb + "' multiply delcared");
      }
      if (this->mb.hasAttribute(
              h, BehaviourData::numericallyComputedJacobianBlocks)) {
        auto cjbs = this->mb.getAttribute<std::vector<std::string>>(
            h, BehaviourData::numericallyComputedJacobianBlocks);
        for (const auto& jb : jbs) {
          throw_if(std::find(cjbs.begin(), cjbs.end(), jb) != cjbs.end(),
                   "jacobian block '" + jb + "' multiply delcared");
          cjbs.push_back(jb);
        }
        this->mb.updateAttribute(
            h, BehaviourData::numericallyComputedJacobianBlocks, cjbs);
      } else {
        this->mb.setAttribute(
            h, BehaviourData::numericallyComputedJacobianBlocks, jbs);
      }
    }
  }  // end of treatNumericallyComputedJacobianBlocks

  void ImplicitDSLBase::completeVariableDeclaration() {
    using namespace tfel::glossary;
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (this->solver == nullptr) {
      this->setNonLinearSolver("NewtonRaphson");
    }
    BehaviourDSLCommon::completeVariableDeclaration();
    if (this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) {
      auto D = VariableDescription("StiffnessTensor", "D", 1u, 0u);
      D.description =
          "stiffness tensor computed from elastic "
          "material properties";
      this->mb.addLocalVariable(uh, D, BehaviourData::ALREADYREGISTRED);
      auto D_tdt =
          this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep()
              ? VariableDescription("StiffnessTensor&", "D_tdt", 1u, 0u)
              : VariableDescription("StiffnessTensor", "D_tdt", 1u, 0u);
      D_tdt.description =
          "stiffness tensor computed from elastic "
          "material properties";
      this->mb.addLocalVariable(uh, D_tdt, BehaviourData::ALREADYREGISTRED);
    }
    if ((!this->mb.getAttribute(BehaviourDescription::computesStiffnessTensor,
                                false)) &&
        (this->mb.getElasticSymmetryType() == ISOTROPIC) &&
        (this->mb.areElasticMaterialPropertiesDefined())) {
      auto add_lv = [this](BehaviourDescription& bd, const std::string& t,
                           const std::string& sn, const std::string& n,
                           const std::string& g, const std::string d) {
        auto r = bd.checkVariableExistence(n, "Parameter", false);
        if (!r.first) {
          auto v = (!sn.empty()) ? VariableDescription(t, sn, n, 1u, 0u)
                                 : VariableDescription(t, n, 1u, 0u);
          v.description = d;
          bd.addLocalVariable(uh, v, BehaviourData::UNREGISTRED);
        } else {
          if (!r.second) {
            this->throwRuntimeError(
                "ImplicitDSLBase::completeVariableDeclaration",
                "Parameter '" + n + "' is not defined for all hypotheses");
          }
          if (!g.empty()) {
            bd.checkVariableGlossaryName(n, g);
          }
        }
      };
      add_lv(this->mb, "stress", "", "young", Glossary::YoungModulus,
             "Young modulus at t+theta*dt");
      add_lv(this->mb, "real", "\u03BD", "nu", Glossary::PoissonRatio,
             "Poisson ratio at t+theta*dt");
      add_lv(this->mb, "stress", "\u03BB", "lambda",
             Glossary::FirstLameCoefficient,
             "first Lamé coefficient at t+theta*dt");
      add_lv(this->mb, "stress", "\u03BC", "mu", Glossary::ShearModulus,
             "shear modulus at t+theta*dt");
      add_lv(this->mb, "stress", "", "young_tdt", "", "Young modulus at t+dt");
      add_lv(this->mb, "real", "\u03BD\u2091\u209C\u209B", "nu_tdt", "",
             "Poisson ratio at t+dt");
      add_lv(this->mb, "stress", "\u03BB\u2091\u209C\u209B", "lambda_tdt", "",
             "first Lamé coefficient at t+dt");
      add_lv(this->mb, "stress", "\u03BC\u2091\u209C\u209B", "mu_tdt", "",
             "shear modulus at t+dt");
    }
    for (const auto& ht : this->mb.getHillTensors()) {
      if ((this->mb.getBehaviourType() !=
           BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (this->mb.getBehaviourType() !=
           BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
        this->throwRuntimeError(
            "ImplicitDSLBase::completeVariableDeclaration",
            "Hill tensors shall only be defined for finite strain "
            "or small strain behaviours");
      }
      if (ht.symbolic_form.empty()) {
        const auto hn = ht.name + "_tdt";
        auto H_tdt =
            this->mb.areMaterialPropertiesConstantDuringTheTimeStep(ht.c)
                ? VariableDescription("tfel::math::st2tost2<N,real>&", hn, 1u,
                                      0u)
                : VariableDescription("tfel::math::st2tost2<N,real>", hn, 1u,
                                      0u);
        H_tdt.description =
            "Hill tensor '" + ht.name + "' at the end of the time step";
        this->mb.addLocalVariable(uh, H_tdt);
      } else {
        const auto hsn = ht.symbolic_form + "\u2091\u209C\u209B";
        const auto hn = ht.name + "_tdt";
        auto H_tdt =
            this->mb.areMaterialPropertiesConstantDuringTheTimeStep(ht.c)
                ? VariableDescription("tfel::math::st2tost2<N,real>&", hsn, hn,
                                      1u, 0u)
                : VariableDescription("tfel::math::st2tost2<N,real>", hsn, hn,
                                      1u, 0u);
        H_tdt.description =
            "Hill tensor '" + hsn + "' at the end of the time step";
        this->mb.addLocalVariable(uh, H_tdt);
      }
    }
    // creating default parameters if not explicitely specified by the user
    if (!this->mb.hasParameter(uh, "epsilon")) {
      VariableDescription e("NumericType", "\u03B5", "epsilon", 1u, 0u);
      e.description =
          "value used to stop the iteration of the implicit algorithm";
      this->mb.addParameter(uh, e, BehaviourData::ALREADYREGISTRED);
      this->mb.setEntryName(uh, "epsilon", "epsilon");
      this->mb.setParameterDefaultValue(uh, "epsilon", 1.e-8);
    }
    if (!this->mb.hasParameter(uh, "theta")) {
      VariableDescription tv("NumericType", "\u03B8", "theta", 1u, 0u);
      tv.description = "theta value used by the implicit scheme";
      this->mb.addParameter(uh, tv, BehaviourData::ALREADYREGISTRED);
      this->mb.setEntryName(uh, "theta", "theta");
      this->mb.setParameterDefaultValue(uh, "theta", 0.5);
    }
    this->solver->completeVariableDeclaration(this->mb);
    if ((this->mb.getAttribute(uh, BehaviourData::compareToNumericalJacobian,
                               false)) ||
        (this->solver->usesJacobian())) {
      const std::string nje = "numerical_jacobian_epsilon";
      if (!this->mb.hasParameter(uh, nje)) {
        const auto eps = 0.1 * this->mb.getFloattingPointParameterDefaultValue(
                                   uh, "epsilon");
        VariableDescription v("NumericType", nje, 1u, 0u);
        v.description =
            "perturbation value used to compute a numerical "
            "approximation of the jacobian";
        this->mb.addParameter(uh, v, BehaviourData::ALREADYREGISTRED);
        this->mb.setParameterDefaultValue(uh, nje, eps);
      }
    }
    if (!this->mb.hasParameter(uh, "iterMax")) {
      unsigned short iterMax = 100u;
      VariableDescription v("ushort", "iterMax", 1u, 0u);
      v.description = "maximum number of iterations allowed";
      this->mb.addParameter(uh, v, BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(uh, "iterMax", iterMax);
    }
    if (this->mb.getAttribute(uh, BehaviourData::compareToNumericalJacobian,
                              false)) {
      if (!this->mb.hasParameter(uh, "jacobianComparisonCriterion")) {
        VariableDescription v("real", "jacobianComparisonCriterion", 1u, 0u);
        v.description =
            "criterion value used to compare the jacobian "
            "provided by the user to its numerical approximation";
        this->mb.addParameter(uh, v, BehaviourData::ALREADYREGISTRED);
        this->mb.setParameterDefaultValue(
            uh, "jacobianComparisonCriterion",
            this->mb.getFloattingPointParameterDefaultValue(uh, "epsilon"));
      }
    }
    // reserve names for jacobian blocks
    const auto mh = this->mb.getDistinctModellingHypotheses();
    for (const auto& h : mh) {
      const auto& d = this->mb.getBehaviourData(h);
      for (const auto& v1 : d.getIntegrationVariables()) {
        for (const auto& v2 : d.getIntegrationVariables()) {
          // Things are a bit tricky here because of the undefined hypothesis
          // case which forwards everything to specialised hypothesis.
          if ((h != uh) && (mh.find(uh) != mh.end()) &&  //
              ((this->mb.getBehaviourData(uh)
                    .getIntegrationVariables()
                    .contains(v1.name)) &&
               (this->mb.getBehaviourData(uh)
                    .getIntegrationVariables()
                    .contains(v2.name)))) {
            continue;
          }
          this->mb.reserveName(h, "df" + v1.name + "_dd" + v2.name);
        }
      }
    }
    std::set<std::string>
        gradients_or_external_state_variables_in_tangent_operator_blocks;
    for (const auto& to : mb.getTangentOperatorBlocks()) {
      gradients_or_external_state_variables_in_tangent_operator_blocks.insert(
          to.second.name);
    }
    for (const auto& vn :
         gradients_or_external_state_variables_in_tangent_operator_blocks) {
      this->reserveName("GetIntegrationVariablesDerivatives_" + vn);
      this->reserveName("getIntegrationVariablesDerivatives_" + vn);
      this->reserveName("dfzeros_dd" + vn);
      std::set<std::string> implicit_equation_derivatives;
      for (const auto h : this->mb.getDistinctModellingHypotheses()) {
        const auto& d = this->mb.getBehaviourData(h);
        for (const auto& v : d.getIntegrationVariables()) {
          implicit_equation_derivatives.insert("df" + v.name + "_dd" + vn);
        }
      }
      for (const auto& ieq_derivative : implicit_equation_derivatives) {
        this->mb.reserveName(uh, ieq_derivative);
      }
    }
    std::set<std::string> jacobian_invert_blocks;
    for (const auto h : this->mb.getDistinctModellingHypotheses()) {
      const auto& d = this->mb.getBehaviourData(h);
      for (const auto& iv1 : d.getIntegrationVariables()) {
        for (const auto& iv2 : d.getIntegrationVariables()) {
          jacobian_invert_blocks.insert("iJ_" + iv1.name + "_" + iv2.name);
        }
      }
    }
    for (const auto& iJb : jacobian_invert_blocks) {
      this->mb.reserveName(uh, iJb);
    }
  }  // end of completeVariableDeclaration()

  void ImplicitDSLBase::endsInputFileProcessing() {
    using namespace tfel::glossary;
    using namespace tfel::material;
    auto throw_if = [this](const bool c, const std::string& m) {
      if (c) {
        this->throwRuntimeError("ImplicitDSLBase::endsInputFileProcessing", m);
      }
    };
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    BehaviourDSLCommon::endsInputFileProcessing();
    // Supported modelling hypothesis
    const auto mh = this->mb.getDistinctModellingHypotheses();
    // check that at leas an integration variable is
    for (const auto& h : mh) {
      const auto n = mfront::getTypeSize(
          this->mb.getBehaviourData(h).getIntegrationVariables());
      if (n.isNull()) {
        tfel::raise(
            "ImplicitDSLBase::endsInputFileProcessing: "
            "no integration variables defined");
      }
    }
    // consistency checks
    if (this->mb.getAttribute(uh, BehaviourData::compareToNumericalJacobian,
                              false)) {
      throw_if((!this->solver->usesJacobian()) ||
                   (this->solver->requiresNumericalJacobian()),
               "@CompareToNumericalJacobian can only be used with solver using "
               "an analytical jacobian (or an approximation of it");
    }
    // create the compute final stress code is necessary
    this->setComputeFinalThermodynamicForcesFromComputeFinalThermodynamicForcesCandidateIfNecessary();
    // correct prediction to take into account normalisation factors
    for (const auto& h : mh) {
      if (this->mb.hasAttribute(
              h, BehaviourData::numericallyComputedJacobianBlocks)) {
        throw_if((!this->solver->usesJacobian()) ||
                     (this->solver->requiresNumericalJacobian()),
                 "numerically computed jacobian blocks can only be "
                 "used with solver using an analytical jacobian "
                 "(or an approximation of it");
        auto decompose =
            [throw_if](
                const std::string& jb) -> std::pair<std::string, std::string> {
          throw_if(jb.empty(), "empty jacobian block");
          throw_if(jb.size() < 6, "invalid jacobian block '" + jb + "'");
          throw_if(jb[0] != 'd', "invalid jacobian block '" + jb + "'");
          throw_if(jb[1] != 'f', "invalid jacobian block '" + jb + "'");
          const auto p = jb.find('_');
          throw_if(p == std::string::npos,
                   "invalid jacobian block '" + jb + "'");
          throw_if(p + 2 >= jb.size(), "invalid jacobian block '" + jb + "'");
          throw_if(jb[p + 1] != 'd', "invalid jacobian block '" + jb + "'");
          throw_if(jb[p + 2] != 'd', "invalid jacobian block '" + jb + "'");
          const auto n = jb.substr(2, p - 2);
          const auto d = jb.substr(p + 3);
          throw_if(n.empty(), "invalid jacobian block '" + jb + "'");
          throw_if(d.empty(), "invalid jacobian block '" + jb + "'");
          return {n, d};
        };
        const auto& jbs = this->mb.getAttribute<std::vector<std::string>>(
            h, BehaviourData::numericallyComputedJacobianBlocks);
        for (const auto& jb : jbs) {
          const auto nd = decompose(jb);
          throw_if(!this->mb.isIntegrationVariableName(h, nd.first),
                   "invalid jacobian block '" + jb +
                       "', "
                       "'" +
                       nd.first + "' is not an integration variable");
          throw_if(!this->mb.isIntegrationVariableName(h, nd.second),
                   "invalid jacobian block '" + jb +
                       "', "
                       "'" +
                       nd.second + "' is not an integration variable");
        }
      }
      if (this->mb.hasCode(h, BehaviourData::ComputePredictor)) {
        CodeBlock predictor;
        const auto& d = this->mb.getBehaviourData(h);
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
        this->mb.setCode(h, BehaviourData::ComputePredictor, predictor,
                         BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
      }
    }
    if (!this->mb.areAllMechanicalDataSpecialised()) {
      if (this->mb.hasCode(uh, BehaviourData::ComputePredictor)) {
        CodeBlock predictor;
        const auto& d = this->mb.getBehaviourData(uh);
        for (const auto& v : d.getIntegrationVariables()) {
          if (this->integrationVariablesIncrementsUsedInPredictor.find(
                  'd' + v.name) !=
              this->integrationVariablesIncrementsUsedInPredictor.end()) {
            if (this->mb.hasAttribute(uh, v.name + "_normalisation_factor")) {
              const auto& nf = this->mb.getAttribute<std::string>(
                  uh, v.name + "_normalisation_factor");
              predictor.code += "this->d" + v.name + " /= " + nf + ";\n";
            }
          }
        }
        this->mb.setCode(uh, BehaviourData::ComputePredictor, predictor,
                         BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
      }
    }
    // minimal tangent operator
    this->setMinimalTangentOperator();
  }  // end of endsInputFileProcessing()

  void ImplicitDSLBase::getSymbols(std::map<std::string, std::string>& symbols,
                                   const Hypothesis h,
                                   const std::string& n) {
    using namespace tfel::material;
    BehaviourDSLCommon::getSymbols(symbols, h, n);
    const auto& d = this->mb.getBehaviourData(h);
    for (const auto& v : d.getIntegrationVariables()) {
      getIncrementSymbol(symbols, v);
    }
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
    if (n == BehaviourData::Integrator) {
      for (const auto& v1 : d.getIntegrationVariables()) {
        if (!v1.symbolic_form.empty()) {
          addSymbol(symbols, "f" + v1.symbolic_form, "f" + v1.name);
        }
        for (const auto& v2 : d.getIntegrationVariables()) {
          const auto& s1 =
              !v1.symbolic_form.empty() ? v1.symbolic_form : v1.name;
          const auto& s2 =
              !v2.symbolic_form.empty() ? v2.symbolic_form : v2.name;
          addSymbol(symbols, "\u2202f" + s1 + "\u2215\u2202\u0394" + s2,
                    "df" + v1.name + "_dd" + v2.name);
        }
      }
    }
    if (n == BehaviourData::ProcessNewCorrection) {
      for (const auto& v : d.getIntegrationVariables()) {
        const auto& s = !v.symbolic_form.empty() ? v.symbolic_form : v.name;
        addSymbol(symbols, "\u03B4\u0394" + s, "delta_d" + v.name);
      }
    }
    const auto is_tangent_operator_code_block = [this, &n] {
      if (this->mb.getBehaviourType() ==
          BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        // all available tangent operators for finite strain  behaviours
        for (const auto& t : getFiniteStrainBehaviourTangentOperatorFlags()) {
          const auto ktype =
              convertFiniteStrainBehaviourTangentOperatorFlagToString(t);
          const auto bn =
              std::string(BehaviourData::ComputeTangentOperator) + '-' + ktype;
          if (n == bn) {
            return true;
          }
        }
        return false;
      } else {
        return n == BehaviourData::ComputeTangentOperator;
      }
    }();
    if (is_tangent_operator_code_block) {
      for (const auto& to : this->mb.getTangentOperatorBlocks()) {
        const auto& sname = to.second.symbolic_form;
        if (!sname.empty()) {
          const auto& name = to.second.name;
          addSymbol(symbols, "getIntegrationVariablesDerivatives_" + sname,
                    "getIntegrationVariablesDerivatives_" + name);
        }
        for (const auto& iv : d.getIntegrationVariables()) {
          const auto s1 = iv.symbolic_form.empty() ? iv.name : iv.symbolic_form;
          const auto s2 = to.second.symbolic_form.empty()
                              ? to.second.name
                              : to.second.symbolic_form;
          addSymbol(symbols, "\u2202f" + s1 + "\u2215\u2202\u0394" + s2,
                    "df" + iv.name + "_dd" + to.second.name);
        }
      }
      for (const auto& iv1 : d.getIntegrationVariables()) {
        for (const auto& iv2 : d.getIntegrationVariables()) {
          if ((iv1.symbolic_form.empty()) && (iv2.symbolic_form.empty())) {
            continue;
          }
          const auto s1 =
              iv1.symbolic_form.empty() ? iv1.name : iv1.symbolic_form;
          const auto s2 =
              iv2.symbolic_form.empty() ? iv2.name : iv2.symbolic_form;
          addSymbol(symbols, "iJ_" + s1 + "_" + s2,
                    "iJ_" + iv1.name + "_" + iv2.name);
        }
      }
    }
  }  // end of getSymbols

  ImplicitDSLBase::~ImplicitDSLBase() = default;

}  // end of namespace mfront
