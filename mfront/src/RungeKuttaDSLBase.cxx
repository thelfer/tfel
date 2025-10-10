/*!
 * \file   mfront/src/RungeKuttaDSLBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   10/11/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <sstream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/DSLFactory.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/PerformanceProfiling.hxx"
#include "MFront/RungeKuttaCodeGeneratorBase.hxx"
#include "MFront/RungeKuttaDSLBase.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace mfront {

  const char* const RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables =
      "RungeKuttaUpdateAuxiliaryStateVariables";

  RungeKuttaDSLBase::RungeKuttaDSLBase(const DSLOptions& opts)
      : BehaviourDSLBase<RungeKuttaDSLBase>(opts) {
    // parameters
    this->reserveName("dtmin");
    this->reserveName("epsilon");
    // Default state vars
    this->reserveName("dt_");
    this->reserveName("corrector");
    this->reserveName("dtprec");
    this->reserveName("converged");
    this->reserveName("error");
    this->reserveName("failed");
    this->reserveName("cste1_2");
    this->reserveName("cste1_4");
    this->reserveName("cste3_8");
    this->reserveName("cste3_32");
    this->reserveName("cste12_13");
    this->reserveName("cste1932_2197");
    this->reserveName("cste7200_2197");
    this->reserveName("cste7296_2197");
    this->reserveName("cste439_216");
    this->reserveName("cste3680_513");
    this->reserveName("cste845_4104");
    this->reserveName("cste8_27");
    this->reserveName("cste3544_2565");
    this->reserveName("cste1859_4104");
    this->reserveName("cste11_40");
    this->reserveName("cste16_135");
    this->reserveName("cste6656_12825");
    this->reserveName("cste28561_56430");
    this->reserveName("cste9_50");
    this->reserveName("cste2_55");
    this->reserveName("cste1_360");
    this->reserveName("cste128_4275");
    this->reserveName("cste2197_75240");
    this->reserveName("cste1_50");
    this->reserveName("rk_update_error");
    this->reserveName("rk_error");
    // CallBacks
    this->registerNewCallBack(
        "@UsableInPurelyImplicitResolution",
        &RungeKuttaDSLBase::treatUsableInPurelyImplicitResolution);
    this->registerNewCallBack("@MaterialLaw",
                              &RungeKuttaDSLBase::treatMaterialLaw);
    this->registerNewCallBack("@Algorithm", &RungeKuttaDSLBase::treatAlgorithm);
    this->registerNewCallBack("@TangentOperator",
                              &RungeKuttaDSLBase::treatTangentOperator);
    this->registerNewCallBack(
        "@IsTangentOperatorSymmetric",
        &RungeKuttaDSLBase::treatIsTangentOperatorSymmetric);
    this->registerNewCallBack("@Derivative",
                              &RungeKuttaDSLBase::treatDerivative);
    this->registerNewCallBack("@Epsilon", &RungeKuttaDSLBase::treatEpsilon);
    this->registerNewCallBack(
        "@StressErrorNormalizationFactor",
        &RungeKuttaDSLBase::treatStressErrorNormalizationFactor);
    this->registerNewCallBack(
        "@StressErrorNormalisationFactor",
        &RungeKuttaDSLBase::treatStressErrorNormalizationFactor);
    this->registerNewCallBack("@MinimalTimeStep",
                              &RungeKuttaDSLBase::treatMinimalTimeStep);
    this->disableCallBack("@Integrator");
    this->disableCallBack("@ComputedVar");
    this->registerNewCallBack("@ComputeStiffnessTensor",
                              &RungeKuttaDSLBase::treatComputeStiffnessTensor);
    this->mb.setIntegrationScheme(BehaviourDescription::EXPLICITSCHEME);
  }

  std::unique_ptr<AbstractBehaviourCodeGenerator>
  RungeKuttaDSLBase::getCodeGenerator() const {
    return std::make_unique<RungeKuttaCodeGeneratorBase>(this->fd, this->mb,
                                                         this->interfaces);
  }  // end of getCodeGenerator

  std::string RungeKuttaDSLBase::getCodeBlockTemplate(
      const std::string& c, const MFrontTemplateGenerationOptions& o) const {
    constexpr auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (c == BehaviourData::ComputePredictionOperator) {
      return "@PredictionOperator{}\n";
    } else if (c == BehaviourData::ComputeThermodynamicForces) {
      return "@ComputeStress{}\n";
    } else if (c == BehaviourData::ComputeDerivative) {
      const auto ivs = this->mb.getBehaviourData(h).getIntegrationVariables();
      if (ivs.empty()) {
        return "@Derivative{}\n";
      } else {
        auto i = std::string("@Derivative{\n");
        for (const auto& v : ivs) {
          if (o.useUnicodeSymbols) {
            i += "d\u209C" + displayName(v) + " = ;\n";
          } else {
            i += "d" + v.name + " = ;\n";
          }
        }
        i += "}\n";
        return i;
      }
    } else if (c == BehaviourData::ComputeTangentOperator) {
      return "@TangentOperator{}\n";
    }
    return "";
  }  // end of getCodeBlockTemplate

  void RungeKuttaDSLBase::treatUpdateAuxiliaryStateVariables() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(
        RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables,  //
        m, true, true);
  }  // end of treatUpdateAuxiliaryStateVariables

  void RungeKuttaDSLBase::treatComputeFinalThermodynamicForces() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->standardModifier(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::ComputeFinalThermodynamicForces,  //
                         m, true, true);
  }  // end of treatComputeFinalThermodynamicForces

  std::string RungeKuttaDSLBase::computeThermodynamicForcesVariableModifier1(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    const auto& d = this->mb.getBehaviourData(h);
    if ((this->mb.isGradientName(var)) ||
        (this->mb.isGradientIncrementName(var)) ||
        (d.isIntegrationVariableName(var)) ||
        (d.isExternalStateVariableName(var))) {
      if (addThisPtr) {
        return "this->" + var + "_";
      } else {
        return var + "_";
      }
    }
    if (d.isAuxiliaryStateVariableName(var)) {
      const auto& v = d.getAuxiliaryStateVariables().getVariable(var);
      if (v.getAttribute<bool>("ComputedByExternalModel", false)) {
        if (addThisPtr) {
          return "this->" + var + "_";
        } else {
          return var + "_";
        }
      }
    }
    auto treat_variable_rate =
        [addThisPtr, &var](const VariableDescription& v) -> std::string {
      if (v.arraySize > 1) {
        if (addThisPtr) {
          return "(real(1)/(this->dt)) * (this->" + var + ")";
        }
        return "(real(1)/(this->dt)) * " + var;
      }
      if (addThisPtr) {
        return "(this->" + var + ")/(this->dt)";
      }
      return "(" + var + ")/(this->dt)";
    };
    for (const auto& v : d.getAuxiliaryStateVariables()) {
      if ((v.getAttribute<bool>("ComputedByExternalModel", false)) &&
          (var == "d" + v.name)) {
        return treat_variable_rate(v);
      }
    }
    if (var == "dT") {
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
          h, var.substr(1));
      if (addThisPtr) {
        return "(this->" + var + ")/(this->dt)";
      } else {
        return "(" + var + ")/(this->dt)";
      }
    }
    if (this->mb.isExternalStateVariableIncrementName(h, var)) {
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
          h, var.substr(1));
      const auto& v = d.getExternalStateVariables().getVariable(var.substr(1));
      return treat_variable_rate(v);
    }
    if (addThisPtr) {
      return "this->" + var;
    }
    return var;
  }  // end of computeThermodynamicForcesVariableModifier1

  std::string RungeKuttaDSLBase::computeThermodynamicForcesVariableModifier2(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    const auto& d = this->mb.getBehaviourData(h);
    if ((this->mb.isGradientName(var)) ||
        (d.isExternalStateVariableName(var))) {
      if (addThisPtr) {
        return "this->" + var + "+this->d" + var;
      } else {
        return var + "+d" + var;
      }
    }
    if (d.isAuxiliaryStateVariableName(var)) {
      const auto& v = d.getAuxiliaryStateVariables().getVariable(var);
      if (v.getAttribute<bool>("ComputedByExternalModel", false)) {
        if (addThisPtr) {
          return "this->" + var + "+this->d" + var;
        } else {
          return var + " + d" + var;
        }
      }
    }
    for (const auto& v : d.getAuxiliaryStateVariables()) {
      if ((v.getAttribute<bool>("ComputedByExternalModel", false)) &&
          (var == "d" + v.name)) {
        if (addThisPtr) {
          return "(this->" + var + ")/(this->dt)";
        } else {
          return "(" + var + ")/(this->dt)";
        }
      }
    }
    if ((d.isExternalStateVariableIncrementName(var)) || (var == "dT") ||
        (this->mb.isGradientIncrementName(var))) {
      if ((d.isExternalStateVariableIncrementName(var)) || (var == "dT")) {
        this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
            h, var.substr(1));
      }
      if (addThisPtr) {
        return "(this->" + var + ")/(this->dt)";
      } else {
        return "(" + var + ")/(this->dt)";
      }
    }
    if (addThisPtr) {
      return "this->" + var;
    }
    return var;
  }  // end of computeThermodynamicForcesVariableModifier2

  void RungeKuttaDSLBase::treatComputeThermodynamicForces() {
    if (this->mb.getMainVariables().empty()) {
      this->throwRuntimeError(
          "RungeKuttaDSLBase::treatComputeThermodynamicForces",
          "no thermodynamic force defined");
    }
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m1 = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->computeThermodynamicForcesVariableModifier1(h, sv, b);
        };
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m2 = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->computeThermodynamicForcesVariableModifier2(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::ComputeThermodynamicForces,
                         BehaviourData::ComputeFinalThermodynamicForces, m1, m2,
                         true, true);
  }  // end of treatComputeThermodynamicForces

  void RungeKuttaDSLBase::treatUnknownVariableMethod(const Hypothesis h,
                                                     const std::string& n) {
    auto throw_if = [this](const bool b, const std::string& m) {
      if (b) {
        this->throwRuntimeError("RungeKuttaDSLBase::treatUnknowVariableMethod",
                                m);
      }
    };
    if (this->mb.isIntegrationVariableName(h, n)) {
      if (this->current->value == "setErrorNormalisationFactor") {
        ++(this->current);
        this->checkNotEndOfFile("RungeKuttaDSLBase::treatUnknowVariableMethod");
        this->readSpecifiedToken("RungeKuttaDSLBase::treatUnknowVariableMethod",
                                 "(");
        this->checkNotEndOfFile("RungeKuttaDSLBase::treatUnknowVariableMethod");
        auto enf = this->current->value;
        ++(this->current);
        if ((this->mb.isMaterialPropertyName(h, enf)) ||
            (this->mb.isLocalVariableName(h, enf))) {
          enf = "this->" + enf;
        } else {
          // the current value shall be a number
          auto value = double{};
          try {
            value = tfel::utilities::convert<double>(enf);
          } catch (...) {
            throw_if(true, "Failed to read error normalisation factor.");
          }
          throw_if(value < 0., "invalid error normalisation factor.");
        }
        this->checkNotEndOfFile("RungeKuttaDSLBase::treatUnknowVariableMethod");
        this->readSpecifiedToken("RungeKuttaDSLBase::treatUnknowVariableMethod",
                                 ")");
        try {
          this->mb.setVariableAttribute(
              h, n, VariableDescription::errorNormalisationFactor,
              VariableAttribute(enf), false);
        } catch (std::exception& e) {
          throw_if(true,
                   "error normalisation factor can't be set "
                   "for variable '" +
                       n + "' (" + std::string(e.what()) + ").");
        }
        this->checkNotEndOfFile("RungeKuttaDSLBase::treatUnknowVariableMethod");
        return;
      }
    }
    BehaviourDSLCommon::treatUnknownVariableMethod(h, n);
  }  // end of treatUnknowVariableMethod

  void RungeKuttaDSLBase::treatDerivative() {
    std::function<std::string(const Hypothesis, const std::string&, const bool)>
        m = [this](const Hypothesis h, const std::string& sv, const bool b) {
          return this->computeThermodynamicForcesVariableModifier1(h, sv, b);
        };
    this->treatCodeBlock(BehaviourData::ComputeDerivative, m, true, true);
  }  // end of treatDerivative

  void RungeKuttaDSLBase::treatEpsilon() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (this->mb.hasParameter(h, "epsilon")) {
      this->throwRuntimeError("RungeKuttaDSLBase::treatEpsilon",
                              "value already specified.");
    }
    double epsilon;
    this->checkNotEndOfFile("RungeKuttaDSLBase::treatEpsilon",
                            "Cannot read epsilon value.");
    std::istringstream flux(current->value);
    flux >> epsilon;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError("RungeKuttaDSLBase::treatEpsilon",
                              "Failed to read epsilon value.");
    }
    if (epsilon < 0) {
      this->throwRuntimeError("RungeKuttaDSLBase::treatEpsilon",
                              "Epsilon value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("RungeKuttaDSLBase::treatEpsilon", ";");
    this->mb.addParameter(h, VariableDescription("real", "epsilon", 1u, 0u),
                          BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "epsilon", epsilon);
  }  // end of treatEpsilon

  void RungeKuttaDSLBase::treatStressErrorNormalizationFactor() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (!this->mb.hasAttribute(BehaviourData::algorithm)) {
      this->throwRuntimeError(
          "RungeKuttaDSLBase::treatStressErrorNormalizationFactor",
          "the Runge-Kutta algorithm has not been set.");
    }
    const auto& algorithm =
        this->mb.getAttribute<std::string>(BehaviourData::algorithm);
    if (algorithm != "RungeKuttaCastem") {
      this->throwRuntimeError(
          "RungeKuttaDSLBase::treatStressErrorNormalizationFactor",
          "defining the normalization factor for the stress error is only "
          "meaningful if the Runge-Kutta algorithm has not been set.");
    }
    if (this->mb.hasParameter(h, "stress_error_normalization_factor")) {
      this->throwRuntimeError(
          "RungeKuttaDSLBase::treatStressErrorNormalizationFactor",
          "value already specified.");
    }
    double stress_error_normalization_factor;
    this->checkNotEndOfFile(
        "RungeKuttaDSLBase::treatStressErrorNormalizationFactor",
        "Cannot read the normalization factor for the stress error value.");
    std::istringstream flux(current->value);
    flux >> stress_error_normalization_factor;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError(
          "RungeKuttaDSLBase::treatStressErrorNormalizationFactor",
          "Failed to read the normalization factor for the stress error "
          "value.");
    }
    if (stress_error_normalization_factor < 0) {
      this->throwRuntimeError(
          "RungeKuttaDSLBase::treatStressErrorNormalizationFactor",
          "StressErrorNormalizationFactor value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken(
        "RungeKuttaDSLBase::treatStressErrorNormalizationFactor", ";");
    auto v = VariableDescription("stress", "stress_error_normalization_factor",
                                 1u, 0u);
    this->mb.addParameter(h, v);
    this->mb.setParameterDefaultValue(h, "stress_error_normalization_factor",
                                      stress_error_normalization_factor);
  }  // end of treatStressErrorNormalizationFactor

  void RungeKuttaDSLBase::treatMinimalTimeStep() {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (this->mb.hasParameter(h, "dtmin")) {
      this->throwRuntimeError("RungeKuttaDSLBase::treatMinimalTimeStep",
                              "value already specified.");
    }
    double dtmin;
    this->checkNotEndOfFile("RungeKuttaDSLBase::treatMinimalTimeStep",
                            "Cannot read dtmin value.");
    std::istringstream flux(current->value);
    flux >> dtmin;
    if ((flux.fail()) || (!flux.eof())) {
      this->throwRuntimeError("RungeKuttaDSLBase::treatMinimalTimeStep",
                              "Failed to read dtmin value.");
    }
    if (dtmin < 0) {
      this->throwRuntimeError("RungeKuttaDSLBase::treatMinimalTimeStep",
                              "MinimalTimeStep value must be positive.");
    }
    ++(this->current);
    this->readSpecifiedToken("RungeKuttaDSLBase::treatMinimalTimeStep", ";");
    this->mb.addParameter(h, VariableDescription("real", "dtmin", 1u, 0u),
                          BehaviourData::ALREADYREGISTRED);
    this->mb.setParameterDefaultValue(h, "dtmin", dtmin);
  }  // end of treatEpsilon

  void RungeKuttaDSLBase::setDefaultAlgorithm() {
    using ushort = unsigned short;
    this->mb.setAttribute(BehaviourData::algorithm,
                          std::string("RungeKutta5/4"), false);
    this->mb.setAttribute(BehaviourData::numberOfEvaluations, ushort(6u),
                          false);
  }  // end of setDefaultAlgorithm

  void RungeKuttaDSLBase::treatAlgorithm() {
    using ushort = unsigned short;
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->checkNotEndOfFile("RungeKuttaDSLBase::treatAlgorithm",
                            "Cannot read algorithm name.");
    if (this->mb.hasAttribute(BehaviourData::algorithm)) {
      this->throwRuntimeError(
          "RungeKuttaDSLBase::treatAlgorithm",
          "algorith already specified. This may be the second "
          "time that the @Algorithm keyword is used, or the default "
          "algorithm was selected when registring a state variable "
          "(keyword @StateVariable)");
    }
    if (this->current->value == "euler") {
      this->mb.setAttribute(BehaviourData::algorithm, std::string("Euler"),
                            false);
      this->mb.setAttribute(BehaviourData::numberOfEvaluations, ushort(0u),
                            false);
    } else if (this->current->value == "rk2") {
      this->mb.setAttribute(BehaviourData::algorithm,
                            std::string("RungeKutta2"), false);
      this->mb.setAttribute(BehaviourData::numberOfEvaluations, ushort(1u),
                            false);
    } else if (this->current->value == "rk4") {
      this->mb.setAttribute(BehaviourData::algorithm,
                            std::string("RungeKutta4"), false);
      this->mb.setAttribute(BehaviourData::numberOfEvaluations, ushort(4u),
                            false);
    } else if (this->current->value == "rk42") {
      this->mb.setAttribute(BehaviourData::algorithm,
                            std::string("RungeKutta4/2"), false);
      this->mb.setAttribute(BehaviourData::numberOfEvaluations, ushort(4u),
                            false);
    } else if (this->current->value == "rk54") {
      this->setDefaultAlgorithm();
    } else if (this->current->value == "rkCastem") {
      const auto bt = this->mb.getBehaviourType();
      if ((bt != BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (bt != BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
        this->throwRuntimeError("RungeKuttaDSLBase::treatAlgorithm",
                                "the rkCastem algorithm is only meaningful for "
                                "small strain and finite strain behaviours.");
      }
      this->reserveName("ra");
      this->reserveName("sqra");
      this->reserveName("errabs");
      this->reserveName("asig");
      this->reserveName("sigf");
      this->mb.addStaticVariable(
          h, StaticVariableDescription("real", "rkcastem_div", 0u, 7L));
      this->mb.addStaticVariable(
          h, StaticVariableDescription("real", "rkcastem_rmin", 0u, 0.7L));
      this->mb.addStaticVariable(
          h, StaticVariableDescription("real", "rkcastem_rmax", 0u, 1.3L));
      this->mb.addStaticVariable(
          h, StaticVariableDescription("real", "rkcastem_fac", 0u, 3L));
      this->mb.addStaticVariable(
          h, StaticVariableDescription("real", "rkcastem_borne", 0u, 2.L));
      this->mb.setAttribute(BehaviourData::algorithm,
                            std::string("RungeKuttaCastem"), false);
      this->mb.setAttribute(BehaviourData::numberOfEvaluations, ushort(5u),
                            false);
    } else {
      this->throwRuntimeError("RungeKuttaDSLBase::treatAlgorithm",
                              this->current->value +
                                  " is not a valid algorithm name"
                                  "Supported algorithms are : 'euler', 'rk2',"
                                  " 'rk4', 'rk42' , 'rk54' and 'rkCastem'");
    }
    ++this->current;
    this->readSpecifiedToken("RungeKuttaDSLBase::treatAlgorithm", ";");
  }

  void RungeKuttaDSLBase::completeVariableDeclaration() {
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream()
          << "RungeKuttaDSLBase::completeVariableDeclaration: begin\n";
    }
    BehaviourDSLCommon::completeVariableDeclaration();
    // driving variables
    for (const auto& v : this->mb.getMainVariables()) {
      const auto& dv = v.first;
      this->mb.addLocalVariable(
          uh, VariableDescription(dv.type, dv.name + "_", 1u, 0u));
      this->mb.addLocalVariable(
          uh,
          VariableDescription(SupportedTypes::getTimeDerivativeType(dv.type),
                              "d" + dv.name + "_", 1u, 0u));
    }
    // algorithm
    if (!this->mb.hasAttribute(BehaviourData::algorithm)) {
      this->setDefaultAlgorithm();
    }
    const auto n = this->mb.getAttribute<unsigned short>(
        BehaviourData::numberOfEvaluations);
    // some checks
    for (const auto& h : this->mb.getDistinctModellingHypotheses()) {
      const auto& d = this->mb.getBehaviourData(h);
      // creating local variables
      const auto& ivs = d.getStateVariables();
      const auto& aivs = d.getAuxiliaryStateVariables();
      const auto& evs = d.getExternalStateVariables();
      for (const auto& iv : ivs) {
        for (unsigned short i = 0u; i != n; ++i) {
          const auto currentVarName =
              "d" + iv.name + "_K" + std::to_string(i + 1u);
          if (getVerboseMode() >= VERBOSE_DEBUG) {
            auto& log = getLogStream();
            log << "registring variable '" << currentVarName << "'";
            if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
              log << " for default hypothesis\n";
            } else {
              log << " for the '" << ModellingHypothesis::toString(h)
                  << "' hypothesis\n";
            }
          }
          this->mb.addLocalVariable(
              h,
              VariableDescription(iv.type, currentVarName, iv.arraySize, 0u));
        }
        const auto currentVarName = iv.name + "_";
        if (getVerboseMode() >= VERBOSE_DEBUG) {
          auto& log = getLogStream();
          log << "registring variable '" << currentVarName << "'";
          if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
            log << " for default hypothesis\n";
          } else {
            log << " for the '" << ModellingHypothesis::toString(h)
                << "' hypothesis\n";
          }
        }
        this->mb.addLocalVariable(
            h, VariableDescription(iv.type, currentVarName, iv.arraySize, 0u));
      }
      for (const auto& iv : aivs) {
        if (!iv.getAttribute<bool>("ComputedByExternalModel", false)) {
          continue;
        }
        const auto currentVarName = iv.name + "_";
        if (getVerboseMode() >= VERBOSE_DEBUG) {
          auto& log = getLogStream();
          log << "registring variable '" << currentVarName << "'";
          if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
            log << " for default hypothesis\n";
          } else {
            log << " for the '" << ModellingHypothesis::toString(h)
                << "' hypothesis\n";
          }
        }
        this->mb.addLocalVariable(
            h, VariableDescription(iv.type, currentVarName, iv.arraySize, 0u));
      }
      for (const auto& ev : evs) {
        const auto currentVarName = ev.name + "_";
        if (getVerboseMode() >= VERBOSE_DEBUG) {
          auto& log = getLogStream();
          log << "registring variable '" << currentVarName << "'";
          if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
            log << " for default hypothesis\n";
          } else {
            log << " for the '" << ModellingHypothesis::toString(h)
                << "' hypothesis\n";
          }
        }
        this->mb.addLocalVariable(
            h, VariableDescription(ev.type, currentVarName, ev.arraySize, 0u));
      }
    }
    // declare the precision used by the algorithm
    if (!this->mb.hasParameter(uh, "epsilon")) {
      this->mb.addParameter(uh, VariableDescription("real", "epsilon", 1u, 0u),
                            BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(uh, "epsilon", 1.e-8);
    }
    if (this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) {
      auto D = VariableDescription("StiffnessTensor", "D", 1u, 0u);
      D.description =
          "stiffness tensor computed from elastic "
          "material properties";
      this->mb.addLocalVariable(uh, D, BehaviourData::ALREADYREGISTRED);
    }
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "RungeKuttaDSLBase::completeVariableDeclaration: end\n";
    }
  }

  void RungeKuttaDSLBase::endsInputFileProcessing() {
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "RungeKuttaDSLBase::endsInputFileProcessing: begin\n";
    }
    BehaviourDSLCommon::endsInputFileProcessing();
    const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    CodeBlock
        ib;  // code inserted at before of the local variable initialisation
    CodeBlock
        ie;  // code inserted at the end of the local variable initialisation
    if (!this->mb.hasAttribute(BehaviourData::algorithm)) {
      this->setDefaultAlgorithm();
    }
    const auto& algorithm =
        this->mb.getAttribute<std::string>(BehaviourData::algorithm);
    if (algorithm == "RungeKuttaCastem") {
      const auto bt = this->mb.getBehaviourType();
      if ((bt != BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (bt != BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
        this->throwRuntimeError("RungeKuttaDSLBase::endsInputFileProcessing",
                                "the rkCastem algorithm is only meaningful for "
                                "small strain and finite strain behaviours.");
      }
    }
    // some checks
    for (const auto& h : this->mb.getDistinctModellingHypotheses()) {
      const auto& d = this->mb.getBehaviourData(h);
      if (!this->mb.getMainVariables().empty()) {
        if (!d.hasCode(BehaviourData::ComputeFinalThermodynamicForces)) {
          this->throwRuntimeError(
              "RungeKuttaDSLBase::endsInputFileProcessing",
              "@ComputeFinalThermodynamicForces was not defined.");
        }
      }
      if (!d.hasCode(BehaviourData::ComputeDerivative)) {
        this->throwRuntimeError("RungeKuttaDSLBase::endsInputFileProcessing",
                                "@Derivative was not defined.");
      }
      auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
      if (d.hasCode(BehaviourData::ComputeThermodynamicForces)) {
        const auto& uvs2 =
            d.getCodeBlock(BehaviourData::ComputeThermodynamicForces).members;
        uvs.insert(uvs2.begin(), uvs2.end());
      }
      CodeBlock icb;  // code inserted at the beginning of the local variable
                      // initialisation
      // creating local variables
      const auto& ivs = d.getStateVariables();
      const auto& evs = d.getExternalStateVariables();
      for (const auto& iv : ivs) {
        if (uvs.find(iv.name) != uvs.end()) {
          const auto currentVarName = iv.name + "_";
          if (this->mb.useDynamicallyAllocatedVector(iv.arraySize)) {
            icb.code += "this->" + currentVarName + ".resize(" +
                        std::to_string(iv.arraySize) + ");\n";
          }
          if ((algorithm != "RungeKutta4/2") &&
              (algorithm != "RungeKutta5/4")) {
            icb.code +=
                "this->" + currentVarName + " = this->" + iv.name + ";\n";
          }
        }
      }
      // driving variables
      if ((algorithm != "RungeKutta4/2") && (algorithm != "RungeKutta5/4")) {
        for (const auto& vm : this->mb.getMainVariables()) {
          const auto& dv = vm.first;
          if (uvs.find(dv.name) != uvs.end()) {
            ib.code += "this->" + dv.name + "_ = this->" + dv.name + ";\n";
          }
        }
      }
      for (const auto& ev : evs) {
        if (uvs.find(ev.name) != uvs.end()) {
          const auto currentVarName = ev.name + "_";
          if (this->mb.useDynamicallyAllocatedVector(ev.arraySize)) {
            icb.code += "this->" + currentVarName + ".resize(" +
                        std::to_string(ev.arraySize) + ");\n";
          }
          if ((algorithm != "RungeKutta4/2") &&
              (algorithm != "RungeKutta5/4")) {
            icb.code +=
                "this->" + currentVarName + " = this->" + ev.name + ";\n";
          }
        }
      }
      this->mb.setCode(h, BehaviourData::BeforeInitializeLocalVariables, icb,
                       BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
    }
    // create the compute final stress code is necessary
    this->setComputeFinalThermodynamicForcesFromComputeFinalThermodynamicForcesCandidateIfNecessary();
    // minimal time step
    if (this->mb.hasParameter(uh, "dtmin")) {
      ib.code += "if(this->dt < " + this->mb.getClassName() + "::dtmin){\n";
      ib.code += "this->dt = " + this->mb.getClassName() + "::dtmin;\n";
      ib.code += "}\n";
    } else {
      ib.code += "if(this->dt < 100 * std::numeric_limits<time>::min()){\n";
      ib.code += "string msg(\"" + this->mb.getClassName() +
                 "::" + this->mb.getClassName() + "\");\n";
      ib.code += "msg += \"time step too small.\";\n";
      ib.code += "throw(runtime_error(msg));\n";
      ib.code += "}\n";
    }
    this->mb.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, ib,
                     BehaviourData::CREATEORAPPEND,
                     BehaviourData::AT_BEGINNING);
    // part introduced at the end of the initialize local variables
    for (const auto& vm : this->mb.getMainVariables()) {
      const auto& dv = vm.first;
      if (Gradient::isIncrementKnown(dv)) {
        ie.code += "this->d" + dv.name + "_ = (this->d" + dv.name +
                   ") / (this->dt);\n";
      } else {
        ie.code += "this->d" + dv.name + "_ = (this->" + dv.name + "1-this->" +
                   dv.name + "0)/(this->dt);\n";
      }
    }
    this->mb.setCode(uh, BehaviourData::AfterInitializeLocalVariables, ie,
                     BehaviourData::CREATEORAPPEND, BehaviourData::BODY);
    // minimal tangent operator if mandatory
    this->setMinimalTangentOperator();
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "RungeKuttaDSLBase::endsInputFileProcessing: end\n";
    }
  }  // end of endsInputFileProcessing

  void RungeKuttaDSLBase::getSymbols(
      std::map<std::string, std::string>& symbols,
      const Hypothesis h,
      const std::string& n) {
    BehaviourDSLCommon::getSymbols(symbols, h, n);
    const auto& d = this->mb.getBehaviourData(h);
    if (n == BehaviourData::ComputeDerivative) {
      for (const auto& mv : this->mb.getMainVariables()) {
        getTimeDerivativeSymbol(symbols, mv.first);
      }
      getTimeDerivativeSymbols(symbols, d.getIntegrationVariables());
      getTimeDerivativeSymbols(symbols, d.getExternalStateVariables());
    } else {
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
    }
  }  // end of getSymbols

  RungeKuttaDSLBase::~RungeKuttaDSLBase() = default;

}  // end of namespace mfront
