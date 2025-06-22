/*!
 * \file   mfront/src/RungeKuttaDSLBase.cxx
 * \brief
 * \author Helfer Thomas
 * \date   10 Nov 2006
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

#include "MFront/DSLUtilities.hxx"
#include "MFront/DSLFactory.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/PerformanceProfiling.hxx"
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

  static std::set<std::string> getVariablesUsedDuringIntegration(
      const BehaviourDescription& mb, const RungeKuttaDSLBase::Hypothesis h) {
    const auto& d = mb.getBehaviourData(h);
    //! all registred members used in this block
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
    if (d.hasCode(BehaviourData::ComputeStress)) {
      const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).members;
      uvs.insert(uvs2.begin(), uvs2.end());
    }
    // variables used to compute the stiffness tensor
    if ((mb.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                               false)) &&
        (!mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      const auto& mps = mb.getElasticMaterialProperties();
      for (const auto& mp : mps) {
        if (mp.is<BehaviourDescription::ComputedMaterialProperty>()) {
          const auto& cmp =
              mp.get<BehaviourDescription::ComputedMaterialProperty>();
          for (const auto& i : mb.getMaterialPropertyInputs(*(cmp.mpd))) {
            uvs.insert(i.name);
          }
        }
      }
    }
    return uvs;
  }

  static void writeExternalVariablesCurrentValues(
      std::ostream& f,
      const BehaviourDescription& mb,
      const RungeKuttaDSLBase::Hypothesis h,
      const std::string& p) {
    const auto t = ((p == "0") ? "(t/this->dt)"
                               : ((p == "1") ? "((t+dt_)/this->dt)"
                                             : "((t+" + p + "*dt_)/this->dt)"));
    const auto& d = mb.getBehaviourData(h);
    const auto uvs = getVariablesUsedDuringIntegration(mb, h);
    for (const auto& mv : mb.getMainVariables()) {
      const auto& dv = mv.first;
      if (uvs.find(dv.name) != uvs.end()) {
        if (dv.increment_known) {
          f << "this->" + dv.name + "_ = this->" + dv.name + "+(this->d" +
                   dv.name + ")*"
            << t << ";\n";
        } else {
          f << "this->" + dv.name + "_ = this->" + dv.name + "0+(this->" +
                   dv.name + "1-this->" + dv.name + "0)*"
            << t << ";\n";
        }
      }
    }
    for (const auto& v : d.getAuxiliaryStateVariables()) {
      if (!v.getAttribute<bool>("ComputedByExternalModel", false)) {
        continue;
      }
      if (uvs.find(v.name) != uvs.end()) {
        f << "this->" << v.name << "_ = this->" << v.name << "+(this->d"
          << v.name << ")*" << t << ";\n";
      }
    }
    if (uvs.find("T") != uvs.end()) {
      f << "this->T_   = this->T+(this->dT)*" << t << ";\n";
    }
    for (const auto& v : d.getExternalStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        f << "this->" << v.name << "_ = this->" << v.name << "+(this->d"
          << v.name << ")*" << t << ";\n";
      }
    }
  }

  static void writeExternalVariableCurrentValue2(std::ostream& f,
                                                 const std::string& n,
                                                 const std::string& p,
                                                 const bool b) {
    if (p == "0") {
      f << "this->" << n << "_ = this->" << n << ";\n";
    } else if (p == "1") {
      if (b) {
        f << "this->" << n << "_ = this->" << n << "+this->d" << n << ";\n";
      } else {
        f << "this->" << n << "_ = this->" << n << "1;\n";
      }
    } else {
      if (b) {
        f << "this->" << n << "_ = this->" << n << "+" << p << "*(this->d" << n
          << ");\n";
      } else {
        f << "this->" << n << "_ = this->" << n << "+" << p << "*(this->" << n
          << "1-this->" << n << "0);\n";
      }
    }
  }

  /*!
   * \brief modifier used for evaluating the stiffness tensor during the
   * the time step
   */
  static std::function<
      std::string(const BehaviourDescription::MaterialPropertyInput&)>&
  modifyVariableForStiffnessTensorComputation() {
    using MaterialPropertyInput = BehaviourDescription::MaterialPropertyInput;
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    static Modifier m =
        [](const BehaviourDescription::MaterialPropertyInput& i) {
          if ((i.type == MaterialPropertyInput::TEMPERATURE) ||
              (i.type == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
            return "this->" + i.name + "_";
          } else if (i.type == MaterialPropertyInput::
                                   AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) {
            return "(this->" + i.name + "+(t/(this->dt))*(this->d" + i.name +
                   "))";
          } else if ((i.type == MaterialPropertyInput::MATERIALPROPERTY) ||
                     (i.type == MaterialPropertyInput::PARAMETER)) {
            return "this->" + i.name;
          } else {
            throw(std::runtime_error(
                "modifyVariableForStiffnessTensorComputation: "
                "unsupported input type for "
                "variable '" +
                i.name + "'"));
          }
        };
    return m;
  }  // end of modifyVariableForStiffnessTensorComputation

  /*!
   * \brief modifier used for evaluating the stiffness tensor at the end of
   * the time step.
   */
  static std::function<
      std::string(const BehaviourDescription::MaterialPropertyInput&)>&
  modifyVariableForStiffnessTensorComputation2() {
    using MaterialPropertyInput = BehaviourDescription::MaterialPropertyInput;
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    static Modifier m = [](const BehaviourDescription::MaterialPropertyInput&
                               i) {
      if ((i.type == MaterialPropertyInput::TEMPERATURE) ||
          (i.type ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.type == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+this->d" + i.name;
      } else if ((i.type == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.type == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else {
        throw(
            std::runtime_error("modifyVariableForStiffnessTensorComputation2: "
                               "unsupported input type for "
                               "variable '" +
                               i.name + "'"));
      }
    };
    return m;
  }  // end of modifyVariableForStiffnessTensorComputation2

  static void writeExternalVariablesCurrentValues2(
      std::ostream& f,
      const BehaviourDescription& mb,
      const RungeKuttaDSLBase::Hypothesis h,
      const std::string& p) {
    const auto& d = mb.getBehaviourData(h);
    const auto uvs = getVariablesUsedDuringIntegration(mb, h);
    for (const auto& mv : mb.getMainVariables()) {
      const auto& dv = mv.first;
      if (uvs.find(dv.name) != uvs.end()) {
        writeExternalVariableCurrentValue2(f, dv.name, p, dv.increment_known);
      }
    }
    for (const auto& v : d.getAuxiliaryStateVariables()) {
      if (!v.getAttribute<bool>("ComputedByExternalModel", false)) {
        continue;
      }
      if (uvs.find(v.name) != uvs.end()) {
        writeExternalVariableCurrentValue2(f, v.name, p, true);
      }
    }
    if (uvs.find("T") != uvs.end()) {
      writeExternalVariableCurrentValue2(f, "T", p, true);
    }
    for (const auto& v : d.getExternalStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        writeExternalVariableCurrentValue2(f, v.name, p, true);
      }
    }
  }  // end of writeExternalVariablesCurrentValues2

  RungeKuttaDSLBase::RungeKuttaDSLBase()
      : BehaviourDSLBase<RungeKuttaDSLBase>() {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->useStateVarTimeDerivative = true;
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
    this->reserveName("t");
    this->mb.addLocalVariable(h,
                              VariableDescription("temperature", "T_", 1u, 0u));
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
    this->registerNewCallBack("@ComputeStress",
                              &RungeKuttaDSLBase::treatComputeStress);
    this->registerNewCallBack("@ComputeFinalStress",
                              &RungeKuttaDSLBase::treatComputeFinalStress);
    this->registerNewCallBack("@Epsilon", &RungeKuttaDSLBase::treatEpsilon);
    this->registerNewCallBack("@MinimalTimeStep",
                              &RungeKuttaDSLBase::treatMinimalTimeStep);
    this->registerNewCallBack(
        "@RequireStiffnessTensor",
        &RungeKuttaDSLBase::treatRequireStiffnessOperator);
    this->disableCallBack("@Integrator");
    this->disableCallBack("@ComputedVar");
    this->registerNewCallBack(
        "@ElasticMaterialProperties",
        &RungeKuttaDSLBase::treatElasticMaterialProperties);
    this->registerNewCallBack("@ComputeStiffnessTensor",
                              &RungeKuttaDSLBase::treatComputeStiffnessTensor);
    this->mb.setIntegrationScheme(BehaviourDescription::EXPLICITSCHEME);
  }

  void RungeKuttaDSLBase::writeBehaviourParserSpecificIncludes() {
    auto b1 = false;
    auto b2 = false;
    this->checkBehaviourFile();
    this->behaviourFile << "#include\"TFEL/Math/General/Abs.hxx\"\n\n";
    this->mb.requiresTVectorOrVectorIncludes(b1, b2);
    if (b1) {
      this->behaviourFile << "#include\"TFEL/Math/tvector.hxx\"\n";
    }
    if (b2) {
      this->behaviourFile << "#include\"TFEL/Math/vector.hxx\"\n";
    }
  }

  void RungeKuttaDSLBase::treatUpdateAuxiliaryStateVariables() {
    this->readCodeBlock(*this, RungeKuttaUpdateAuxiliaryStateVariables,
                        &RungeKuttaDSLBase::standardModifier, true, true);
  }  // end of RungeKuttaDSLBase::treatUpdateAuxiliaryStateVarBase

  void RungeKuttaDSLBase::treatComputeFinalStress() {
    this->readCodeBlock(*this, BehaviourData::ComputeFinalStress,
                        &RungeKuttaDSLBase::standardModifier, true, true);
  }  // end of RungeKuttaDSLBase::treatUpdateAuxiliaryStateVarBase

  std::string RungeKuttaDSLBase::computeStressVariableModifier1(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    const auto& d = this->mb.getBehaviourData(h);
    if ((this->mb.isDrivingVariableName(var)) || (var == "T") ||
        (this->mb.isDrivingVariableIncrementName(var)) ||
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
  }  // end of RungeKuttaDSLBase::computeStressVariableModifier1

  std::string RungeKuttaDSLBase::computeStressVariableModifier2(
      const Hypothesis h, const std::string& var, const bool addThisPtr) {
    const auto& d = this->mb.getBehaviourData(h);
    if ((this->mb.isDrivingVariableName(var)) || (var == "T") ||
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
        (this->mb.isDrivingVariableIncrementName(var))) {
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
  }  // end of RungeKuttaDSLBase::computeStressVariableModifier2

  void RungeKuttaDSLBase::treatComputeStress() {
    this->readCodeBlock(
        *this, BehaviourData::ComputeStress, BehaviourData::ComputeFinalStress,
        &RungeKuttaDSLBase::computeStressVariableModifier1,
        &RungeKuttaDSLBase::computeStressVariableModifier2, true, true);
  }  // end of RungeKuttaDSLBase::treatComputeStress

  void RungeKuttaDSLBase::treatUnknownVariableMethod(const Hypothesis h,
                                                     const std::string& n) {
    using namespace std;
    if (this->mb.isIntegrationVariableName(h, n)) {
      if (this->current->value == "setErrorNormalisationFactor") {
        string var;
        ++(this->current);
        this->checkNotEndOfFile("RungeKuttaDSLBase::treatUnknowVariableMethod");
        this->readSpecifiedToken("RungeKuttaDSLBase::treatUnknowVariableMethod",
                                 "(");
        this->checkNotEndOfFile("RungeKuttaDSLBase::treatUnknowVariableMethod");
        var = this->current->value;
        if ((this->mb.isMaterialPropertyName(h, var)) ||
            (this->mb.isLocalVariableName(h, var))) {
          var = "this->" + var;
        } else {
          // var shall be a number
          double value;
          istringstream flux(var);
          flux >> value;
          if (flux.fail()) {
            this->throwRuntimeError(
                "RungeKuttaDSLBase::treatUnknowVariableMethod",
                "Failed to read error normalisation factor.");
          }
          if (value < 0.) {
            this->throwRuntimeError(
                "RungeKuttaDSLBase::treatUnknowVariableMethod",
                "invalid error normalisation factor.");
          }
        }
        if (!this->enf.insert(make_pair(n, var)).second) {
          this->throwRuntimeError(
              "RungeKuttaDSLBase::treatUnknowVariableMethod",
              "Error normalisation factor already defined for variable '" + n +
                  "'.");
        }
        ++(this->current);
        this->checkNotEndOfFile("RungeKuttaDSLBase::treatUnknowVariableMethod");
        return;
      }
    }
    BehaviourDSLCommon::treatUnknownVariableMethod(h, n);
  }  // end of RungeKuttaDSLBase::treatUnknowVariableMethod

  void RungeKuttaDSLBase::treatDerivative() {
    this->readCodeBlock(*this, BehaviourData::ComputeDerivative,
                        &RungeKuttaDSLBase::computeStressVariableModifier1,
                        true, true);
  }  // end of RungeKuttaDSLBase::treatDerivative

  void RungeKuttaDSLBase::treatEpsilon() {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
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
  }  // end of RungeKuttaDSLBase::treatEpsilon

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
  }  // end of RungeKuttaDSLBase::treatEpsilon

  void RungeKuttaDSLBase::setDefaultAlgorithm() {
    using ushort = unsigned short;
    this->mb.setAttribute(BehaviourData::algorithm,
                          std::string("RungeKutta5/4"), false);
    this->mb.setAttribute(BehaviourData::numberOfEvaluations, ushort(6u),
                          false);
  }  // end of RungeKuttaDSLBase::setDefaultAlgorithm

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

  void RungeKuttaDSLBase::endsInputFileProcessing() {
    BehaviourDSLCommon::endsInputFileProcessing();
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    CodeBlock
        ib;  // code inserted at before of the local variable initialisation
    CodeBlock
        ie;  // code inserted at the end of the local variable initialisation
    if (!this->mb.hasAttribute(BehaviourData::algorithm)) {
      this->setDefaultAlgorithm();
    }
    const auto& algorithm =
        this->mb.getAttribute<std::string>(BehaviourData::algorithm);
    const auto n = this->mb.getAttribute<unsigned short>(
        BehaviourData::numberOfEvaluations);
    // some checks
    const auto& bh = this->mb.getDistinctModellingHypotheses();
    for (const auto& elem : bh) {
      const auto& d = this->mb.getBehaviourData(elem);
      if (!d.hasCode(BehaviourData::ComputeFinalStress)) {
        this->throwRuntimeError("RungeKuttaDSLBase::endsInputFileProcessing",
                                "@ComputeFinalStress was not defined.");
      }
      if (!d.hasCode(BehaviourData::ComputeDerivative)) {
        this->throwRuntimeError("RungeKuttaDSLBase::endsInputFileProcessing",
                                "@Derivative was not defined.");
      }
      auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
      if (d.hasCode(BehaviourData::ComputeStress)) {
        const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).members;
        uvs.insert(uvs2.begin(), uvs2.end());
      }
      CodeBlock icb;  // code inserted at the beginning of the local variable
                      // initialisation
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
            if (elem == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
              log << " for default hypothesis\n";
            } else {
              log << " for the '" << ModellingHypothesis::toString(elem)
                  << "' hypothesis\n";
            }
          }
          this->mb.addLocalVariable(
              elem,
              VariableDescription(iv.type, currentVarName, iv.arraySize, 0u));
        }
        if (uvs.find(iv.name) != uvs.end()) {
          const auto currentVarName = iv.name + "_";
          if (getVerboseMode() >= VERBOSE_DEBUG) {
            auto& log = getLogStream();
            log << "registring variable '" << currentVarName << "'";
            if (elem == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
              log << " for default hypothesis\n";
            } else {
              log << " for the '" << ModellingHypothesis::toString(elem)
                  << "' hypothesis\n";
            }
          }
          this->mb.addLocalVariable(
              elem,
              VariableDescription(iv.type, currentVarName, iv.arraySize, 0u));
          if (this->useDynamicallyAllocatedVector(iv.arraySize)) {
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
      // driving variables
      if ((algorithm != "RungeKutta4/2") && (algorithm != "RungeKutta5/4")) {
        for (const auto& vm : this->mb.getMainVariables()) {
          const auto& dv = vm.first;
          if (uvs.find(dv.name) != uvs.end()) {
            ib.code += "this->" + dv.name + "_ = this->" + dv.name + ";\n";
          }
        }
        if (uvs.find("T") != uvs.end()) {
          ib.code += "this->T_ = this->T;\n";
        }
      }
      for (const auto& ev : evs) {
        if (uvs.find(ev.name) != uvs.end()) {
          const auto currentVarName = ev.name + "_";
          if (getVerboseMode() >= VERBOSE_DEBUG) {
            auto& log = getLogStream();
            log << "registring variable '" << currentVarName << "'";
            if (elem == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
              log << " for default hypothesis\n";
            } else {
              log << " for the '" << ModellingHypothesis::toString(elem)
                  << "' hypothesis\n";
            }
          }
          this->mb.addLocalVariable(
              elem,
              VariableDescription(ev.type, currentVarName, ev.arraySize, 0u));
          if (this->useDynamicallyAllocatedVector(ev.arraySize)) {
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
      this->mb.setCode(elem, BehaviourData::BeforeInitializeLocalVariables, icb,
                       BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
    }
    // create the compute final stress code is necessary
    this->setComputeFinalStressFromComputeFinalStressCandidateIfNecessary();
    // declare the precision used by the algorithm
    if (!this->mb.hasParameter(h, "epsilon")) {
      this->mb.addParameter(h, VariableDescription("real", "epsilon", 1u, 0u),
                            BehaviourData::ALREADYREGISTRED);
      this->mb.setParameterDefaultValue(h, "epsilon", 1.e-8);
    }
    if (this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) {
      auto D = VariableDescription("StiffnessTensor", "D", 1u, 0u);
      D.description =
          "stiffness tensor computed from elastic "
          "material properties";
      this->mb.addLocalVariable(h, D, BehaviourData::ALREADYREGISTRED);
    }
    // minimal time step
    if (this->mb.hasParameter(h, "dtmin")) {
      ib.code += "if(this->dt<" + this->mb.getClassName() + "::dtmin){\n";
      ib.code += "this->dt=" + this->mb.getClassName() + "::dtmin;\n";
      ib.code += "}\n";
    } else {
      ib.code += "if(this->dt<100*numeric_limits<time>::min()){\n";
      ib.code += "string msg(\"" + this->mb.getClassName() +
                 "::" + this->mb.getClassName() + "\");\n";
      ib.code += "msg += \"time step too small.\";\n";
      ib.code += "throw(runtime_error(msg));\n";
      ib.code += "}\n";
    }
    this->mb.setCode(h, BehaviourData::BeforeInitializeLocalVariables, ib,
                     BehaviourData::CREATEORAPPEND,
                     BehaviourData::AT_BEGINNING);
    // part introduced at the end of the initialize local variables
    for (const auto& vm : this->mb.getMainVariables()) {
      const auto& dv = vm.first;
      if (dv.increment_known) {
        ie.code +=
            "this->d" + dv.name + "_ = (this->d" + dv.name + ")/(this->dt);\n";
      } else {
        ie.code += "this->d" + dv.name + "_ = (this->" + dv.name + "1-this->" +
                   dv.name + "0)/(this->dt);\n";
      }
    }
    this->mb.setCode(h, BehaviourData::AfterInitializeLocalVariables, ie,
                     BehaviourData::CREATEORAPPEND, BehaviourData::BODY);
    // minimal tangent operator if mandatory
    this->setMinimalTangentOperator();
  }  // end of RungeKuttaDSLBase::endsInputFileProcessing

  void RungeKuttaDSLBase::writeBehaviourLocalVariablesInitialisation(
      const Hypothesis h) {
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    if (this->mb.getAttribute(BehaviourDescription::computesStiffnessTensor,
                              false)) {
      this->behaviourFile
          << "// the stiffness tensor at the beginning of the time step\n";
      Modifier bts = [](const MaterialPropertyInput& i) {
        if ((i.type == MaterialPropertyInput::TEMPERATURE) ||
            (i.type ==
             MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
            (i.type == MaterialPropertyInput::EXTERNALSTATEVARIABLE) ||
            (i.type == MaterialPropertyInput::MATERIALPROPERTY) ||
            (i.type == MaterialPropertyInput::PARAMETER)) {
          return "this->" + i.name;
        } else {
          throw(std::runtime_error(
              "RungeKuttaDSLBase::writeBehaviourLocalVariablesInitialisation: "
              "unsupported input type for variable '" +
              i.name + "'"));
        }
      };
      this->writeStiffnessTensorComputation(this->behaviourFile, "this->D",
                                            bts);
    }
    BehaviourDSLCommon::writeBehaviourLocalVariablesInitialisation(h);
  }  // end of RungeKuttaDSLBase::writeBehaviourLocalVariablesInitialisation

  void RungeKuttaDSLBase::writeBehaviourParserSpecificTypedefs() {
    this->checkBehaviourFile();
  }

  void RungeKuttaDSLBase::writeBehaviourParserSpecificMembers(
      const Hypothesis h) {
    this->checkBehaviourFile();
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "bool\ncomputeStress(void){\n"
                          << "using namespace std;\n"
                          << "using namespace tfel::math;\n";
      writeMaterialLaws(
          "RungeKuttaDSLBase::writeBehaviourParserSpecificMembers",
          this->behaviourFile, this->mb.getMaterialLaws());
      this->behaviourFile << this->mb.getCode(h, BehaviourData::ComputeStress)
                          << '\n'
                          << "return true;\n"
                          << "} // end of " << this->mb.getClassName()
                          << "::computeStress\n\n";
    }
    this->behaviourFile << "bool\ncomputeFinalStress(void){\n"
                        << "using namespace std;\n"
                        << "using namespace tfel::math;\n";
    writeMaterialLaws("RungeKuttaDSLBase::writeBehaviourParserSpecificMembers",
                      this->behaviourFile, this->mb.getMaterialLaws());
    this->behaviourFile << this->mb.getCode(h,
                                            BehaviourData::ComputeFinalStress)
                        << '\n'
                        << "return true;\n"
                        << "} // end of " << this->mb.getClassName()
                        << "::computeFinalStress\n\n"
                        << "bool\ncomputeDerivative(void){\n"
                        << "using namespace std;\n"
                        << "using namespace tfel::math;\n";
    writeMaterialLaws("RungeKuttaDSLBase::writeBehaviourParserSpecificMembers",
                      this->behaviourFile, this->mb.getMaterialLaws());
    this->behaviourFile << this->mb.getCode(h, BehaviourData::ComputeDerivative)
                        << '\n'
                        << "return true;\n"
                        << "} // end of " << this->mb.getClassName()
                        << "::computeDerivative\n\n";
  }  // end of writeBehaviourParserSpecificMembers

  void RungeKuttaDSLBase::writeBehaviourUpdateStateVariables(const Hypothesis) {
    // Disabled (makes no sense for this parser)
  }  // end of writeBehaviourUpdateStateVariables

  void RungeKuttaDSLBase::writeBehaviourUpdateAuxiliaryStateVariables(
      const Hypothesis h) {
    BehaviourDSLCommon::writeBehaviourUpdateAuxiliaryStateVariables(h);
    if (this->mb.hasCode(
            h, RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      this->behaviourFile << "/*!\n"
                          << "* \\brief Update auxiliary state variables at "
                             "end of integration\n"
                          << "*/\n"
                          << "void\n"
                          << "updateAuxiliaryStateVariables(const real dt_)"
                          << "{\n"
                          << "static_cast<void>(dt_);\n"
                          << "using namespace std;\n"
                          << "using namespace tfel::math;\n";
      writeMaterialLaws(
          "RungeKuttaDSLBase::writeBehaviourUpdateAuxiliaryStateVariables",
          this->behaviourFile, this->mb.getMaterialLaws());
      this->behaviourFile
          << this->mb.getCode(
                 h, RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)
          << '\n'
          << "}\n\n";
    }
  }  // end of  RungeKuttaDSLBase::writeBehaviourUpdateAuxiliaryStateVariables

  void RungeKuttaDSLBase::writeBehaviourEulerIntegrator(const Hypothesis h) {
    const auto btype = this->mb.getBehaviourTypeFlag();
    const auto& d = this->mb.getBehaviourData(h);
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "this->computeStress();\n";
    }
    this->behaviourFile << "if(!this->computeDerivative()){\n\n";
    if (this->mb.useQt()) {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->" << v.name << " += "
                          << "this->dt*(this->d" << v.name << ");\n";
    }
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile
          << "// updating stiffness tensor at the end of the time step\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation2());
    }
    this->behaviourFile << "// Update stress field\n"
                        << "this->computeFinalStress();\n";
    if (d.hasCode(RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      this->behaviourFile << "this->updateAuxiliaryStateVariables(this->dt);\n";
    }
  }  // end of writeBehaviourEulerIntegrator

  void RungeKuttaDSLBase::writeBehaviourRK2Integrator(const Hypothesis h) {
    const auto btype = this->mb.getBehaviourTypeFlag();
    const auto& d = this->mb.getBehaviourData(h);
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
    if (d.hasCode(BehaviourData::ComputeStress)) {
      const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).members;
      uvs.insert(uvs2.begin(), uvs2.end());
    }
    this->behaviourFile
        << "TFEL_CONSTEXPR const auto cste1_2 = real{1}/real{2};\n"
        << "// Compute K1's values\n";
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "this->computeStress();\n";
    }
    this->behaviourFile << "if(!this->computeDerivative()){\n\n";
    if (this->mb.useQt()) {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K1 = (this->dt)*(this->d"
                          << v.name << ");\n";
    }
    for (const auto& iv : d.getStateVariables()) {
      if (uvs.find(iv.name) != uvs.end()) {
        this->behaviourFile << "this->" << iv.name << "_ += cste1_2*(this->d"
                            << iv.name << "_K1);\n";
      }
    }
    writeExternalVariablesCurrentValues2(this->behaviourFile, this->mb, h,
                                         "cste1_2");
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "this->computeStress();\n";
    }
    this->behaviourFile << "if(!this->computeDerivative()){\n\n";
    if (this->mb.useQt()) {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "// Final Step\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->" << v.name << " += "
                          << "this->dt*(this->d" << v.name << ");\n";
    }
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile
          << "// updating stiffness tensor at the end of the time step\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation2());
    }
    this->behaviourFile << "// Update stress field\n"
                        << "this->computeFinalStress();\n";
    if (d.hasCode(RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      this->behaviourFile << "this->updateAuxiliaryStateVariables(this->dt);\n";
    }
  }  // end of writeBehaviourRK2Integrator

  void RungeKuttaDSLBase::writeBehaviourRK54Integrator(const Hypothesis h) {
    const auto& d = this->mb.getBehaviourData(h);
    auto shallUpdateInternalStateValues = [this, d, h]() {
      const auto uvs = getVariablesUsedDuringIntegration(this->mb, h);
      for (const auto& v : d.getIntegrationVariables()) {
        if (uvs.find(v.name) != uvs.end()) {
          return true;
        }
      }
      return false;
    }();
    auto shallUpdateExternalStateValues = [this, d, h]() {
      const auto uvs = getVariablesUsedDuringIntegration(this->mb, h);
      for (const auto& mv : this->mb.getMainVariables()) {
        const auto& dv = mv.first;
        if (uvs.find(dv.name) != uvs.end()) {
          return true;
        }
      }
      if (uvs.find("T") != uvs.end()) {
        return true;
      }
      for (const auto& v : d.getExternalStateVariables()) {
        if (uvs.find(v.name) != uvs.end()) {
          return true;
        }
      }
      return false;
    }();
    //! all registred variables used in ComputeDerivatives and ComputeStress
    //! blocks
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
    if (d.hasCode(BehaviourData::ComputeStress)) {
      const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).members;
      uvs.insert(uvs2.begin(), uvs2.end());
    }
    ErrorEvaluation eev;
    auto svsize = this->getTotalSize(d.getStateVariables());
    if (svsize.getScalarSize() + svsize.getTVectorSize() +
            3u * svsize.getStensorSize() + 3u * svsize.getTensorSize() >=
        20) {
      eev = MAXIMUMVALUEERROREVALUATION;
    } else {
      eev = ERRORSUMMATIONEVALUATION;
    }
    if (shallUpdateExternalStateValues) {
      this->behaviourFile
          << "TFEL_CONSTEXPR const auto cste1_2         = real{1}/real{2};\n"
          << "TFEL_CONSTEXPR const auto cste3_8         = real{3}/real{8};\n"
          << "TFEL_CONSTEXPR const auto cste12_13       = Type(12)/Type(13);\n";
    }
    if (shallUpdateInternalStateValues) {
      this->behaviourFile
          << "TFEL_CONSTEXPR const auto cste3544_2565   = "
             "Type(3544)/Type(2565);\n"
          << "TFEL_CONSTEXPR const auto cste11_40       = Type(11)/Type(40);\n"
          << "TFEL_CONSTEXPR const auto cste1859_4104   = "
             "Type(1859)/Type(4104);\n"
          << "TFEL_CONSTEXPR const auto cste8_27        = Type(8)/Type(27);\n"
          << "TFEL_CONSTEXPR const auto cste845_4104    = "
             "Type(845)/Type(4104);\n"
          << "TFEL_CONSTEXPR const auto cste3680_513    = "
             "Type(3680)/Type(513);\n"
          << "TFEL_CONSTEXPR const auto cste439_216     = "
             "Type(439)/Type(216);\n"
          << "TFEL_CONSTEXPR const auto cste7296_2197   = "
             "Type(7296)/Type(2197);\n"
          << "TFEL_CONSTEXPR const auto cste7200_2197   = "
             "Type(7200)/Type(2197);\n"
          << "TFEL_CONSTEXPR const auto cste3_32        = real{3}/real{32};\n"
          << "TFEL_CONSTEXPR const auto cste1932_2197   = "
             "Type(1932)/Type(2197);\n";
    }
    this->behaviourFile
        << "TFEL_CONSTEXPR const auto cste1_4         = real{1}/real{4};\n"
        << "TFEL_CONSTEXPR const auto cste16_135      = Type(16)/Type(135);\n"
        << "TFEL_CONSTEXPR const auto cste6656_12825  = "
           "Type(6656)/Type(12825);\n"
        << "TFEL_CONSTEXPR const auto cste28561_56430 = "
           "Type(28561)/Type(56430);\n"
        << "TFEL_CONSTEXPR const auto cste9_50        = Type(9)/Type(50);\n"
        << "TFEL_CONSTEXPR const auto cste2_55        = Type(2)/Type(55);\n"
        << "TFEL_CONSTEXPR const auto cste1_360       = Type(1)/Type(360);\n"
        << "TFEL_CONSTEXPR const auto cste128_4275    = Type(128)/Type(4275);\n"
        << "TFEL_CONSTEXPR const auto cste2197_75240  = "
           "Type(2197)/Type(75240);\n"
        << "TFEL_CONSTEXPR const auto cste1_50        = Type(1)/Type(50);\n"
        << "time t      = time(0);\n"
        << "time dt_    = this->dt;\n"
        << "time dtprec = 100*this->dt*numeric_limits<time>::epsilon();\n"
        << "Type error;\n"
        << "bool converged = false;\n";
    if (getDebugMode()) {
      this->behaviourFile
          << "cout << endl << \"" << this->mb.getClassName()
          << "::integrate() : beginning of resolution\" << endl;\n";
    }
    this->behaviourFile << "while(!converged){\n";
    if (getDebugMode()) {
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : from \" << t <<  \" to \" << "
                             "t+dt_ << \" with time step \" << dt_ << endl;\n";
    }
    this->behaviourFile << "bool failed = false;\n";
    this->behaviourFile << "// Compute K1's values\n";
    writeExternalVariablesCurrentValues(this->behaviourFile, this->mb, h, "0");
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ = this->" << v.name
                            << ";\n";
      }
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n";
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K1's stress\" << endl;\n";
      this->behaviourFile << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n";
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K1's "
                             "derivative\" << endl;\n";
      this->behaviourFile << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K1 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "// Compute K2's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ += cste1_4*(this->d"
                            << v.name << "_K1);\n";
      }
    }
    writeExternalVariablesCurrentValues(this->behaviourFile, this->mb, h,
                                        "cste1_4");
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    this->behaviourFile << "// Update stress field\n";
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n";
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K2's stress\" << endl;\n";
      this->behaviourFile << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n";
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K2's "
                             "derivative\" << endl;\n";
      this->behaviourFile << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K2 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "}\n"
                        << "}\n"
                        << "}\n\n"
                        << "if(!failed){\n"
                        << "// Compute K3's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ = this->" << v.name
                            << "+cste3_32*(this->d" << v.name
                            << "_K1+3*(this->d" << v.name << "_K2));\n";
      }
    }
    writeExternalVariablesCurrentValues(this->behaviourFile, this->mb, h,
                                        "cste3_8");
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "// Update stress field\n"
                          << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n";
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K3's stress\" << endl;\n";
      this->behaviourFile << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n";
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K3's "
                             "derivative\" << endl;\n";
      this->behaviourFile << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K3 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";

    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "// Compute K4's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ = this->" << v.name
                            << "+cste1932_2197*(this->d" << v.name << "_K1)"
                            << "-cste7200_2197*(this->d" << v.name << "_K2)"
                            << "+cste7296_2197*(this->d" << v.name << "_K3);\n";
      }
    }
    writeExternalVariablesCurrentValues(this->behaviourFile, this->mb, h,
                                        "cste12_13");
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    this->behaviourFile << "// Update stress field\n";
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n";
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K4's stress\" << endl;\n";
      this->behaviourFile << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n";
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K4's "
                             "derivatives\" << endl;\n";
      this->behaviourFile << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K4 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "}\n"
                        << "}\n"
                        << "}\n\n"
                        << "if(!failed){\n"
                        << "// Compute K5's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ = this->" << v.name
                            << "+cste439_216*(this->d" << v.name << "_K1)"
                            << "-8*(this->d" << v.name << "_K2)"
                            << "+cste3680_513*(this->d" << v.name << "_K3)"
                            << "-cste845_4104*(this->d" << v.name << "_K4);\n";
      }
    }
    writeExternalVariablesCurrentValues(this->behaviourFile, this->mb, h, "1");
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    this->behaviourFile << "// Update stress field\n";
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n";
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K5's stress\" << endl;\n";
      this->behaviourFile << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n";
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K5's "
                             "derivatives\" << endl;\n";
      this->behaviourFile << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K5 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "}\n"
                        << "}\n"
                        << "}\n\n"
                        << "if(!failed){\n"
                        << "// Compute K6's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ = this->" << v.name
                            << "-cste8_27*(this->d" << v.name << "_K1)"
                            << "+2*(this->d" << v.name << "_K2)"
                            << "-cste3544_2565*(this->d" << v.name << "_K3)"
                            << "+cste1859_4104*(this->d" << v.name << "_K4)"
                            << "-cste11_40*(this->d" << v.name << "_K5);\n";
      }
    }
    writeExternalVariablesCurrentValues(this->behaviourFile, this->mb, h,
                                        "cste1_2");
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "// Update stress field\n"
                          << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile
          << "if(failed){\n"
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K6's stress\" << endl;\n"
          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n"
                          << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K6's "
                             "derivatives\" << endl;\n"
                          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K6 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "}\n"
                        << "}\n"
                        << "}\n\n"
                        << "if(!failed){\n"
                        << "// Computing the error\n";
    if (eev == ERRORSUMMATIONEVALUATION) {
      for (auto p = std::begin(d.getStateVariables());
           p != std::end(d.getStateVariables()); ++p) {
        const auto& v = *p;
        if (v.arraySize == 1u) {
          if (p == d.getStateVariables().begin()) {
            this->behaviourFile << "error  = ";
          } else {
            this->behaviourFile << "error += ";
          }
          if (enf.find(v.name) != enf.end()) {
            this->behaviourFile << "(";
          }
          this->behaviourFile << "tfel::math::abs(";
          this->behaviourFile << "cste1_360*(this->d" << v.name << "_K1)"
                              << "-cste128_4275*(this->d" << v.name << "_K3)"
                              << "-cste2197_75240*(this->d" << v.name << "_K4)"
                              << "+cste1_50*(this->d" << v.name << "_K5)"
                              << "+cste2_55*(this->d" << v.name << "_K6))";
          if (enf.find(v.name) != enf.end()) {
            this->behaviourFile << ")/(" << enf.find(v.name)->second << ")";
          }
          this->behaviourFile << ";\n";
        } else {
          if (this->useDynamicallyAllocatedVector(v.arraySize)) {
            if (p == d.getStateVariables().begin()) {
              this->behaviourFile << "error  = Type(0);\n";
            }
            this->behaviourFile
                << "for(unsigned short idx=0;idx!=" << v.arraySize
                << ";++idx){\n";
            this->behaviourFile << "error += ";
            if (enf.find(v.name) != enf.end()) {
              this->behaviourFile << "(";
            }
            this->behaviourFile << "tfel::math::abs(";
            this->behaviourFile
                << "cste1_360*(this->d" << v.name << "_K1[idx])"
                << "-cste128_4275*(this->d" << v.name << "_K3[idx])"
                << "-cste2197_75240*(this->d" << v.name << "_K4[idx])"
                << "+cste1_50*(this->d" << v.name << "_K5[idx])"
                << "+cste2_55*(this->d" << v.name << "_K6[idx]))";
            if (enf.find(v.name) != enf.end()) {
              this->behaviourFile << ")/(" << enf.find(v.name)->second << ")";
            }
            this->behaviourFile << ";\n";
            this->behaviourFile << "}\n";
          } else {
            for (unsigned short i = 0; i != v.arraySize; ++i) {
              if ((p == d.getStateVariables().begin()) && (i == 0)) {
                this->behaviourFile << "error  = ";
              } else {
                this->behaviourFile << "error += ";
              }
              if (enf.find(v.name) != enf.end()) {
                this->behaviourFile << "(";
              }
              this->behaviourFile
                  << "tfel::math::abs("
                  << "cste1_360*(this->d" << v.name << "_K1[" << i << "])"
                  << "-cste128_4275*(this->d" << v.name << "_K3[" << i << "])"
                  << "-cste2197_75240*(this->d" << v.name << "_K4[" << i << "])"
                  << "+cste1_50*(this->d" << v.name << "_K5[" << i << "])"
                  << "+cste2_55*(this->d" << v.name << "_K6[" << i << "]))";
              if (enf.find(v.name) != enf.end()) {
                this->behaviourFile << ")/(" << enf.find(v.name)->second << ")";
              }
              this->behaviourFile << ";\n";
            }
          }
        }
      }
      this->behaviourFile << "error/=" << svsize << ";\n";
    } else if (eev == MAXIMUMVALUEERROREVALUATION) {
      this->behaviourFile
          << "error  = Type(0);\n"
          << "auto rk_update_error = [&error](const real rk_error){\n"
          << "if(!std::isfinite(error)){return;}\n"
          << "if(!std::isfinite(rk_error)){\n"
          << "error = rk_error;\n"
          << "return;\n"
          << "}\n"
          << "error = std::max(error, rk_error);\n"
          << "};\n";
      for (const auto& v : d.getStateVariables()) {
        if (v.arraySize == 1u) {
          this->behaviourFile << "rk_update_error(";
          if (enf.find(v.name) != enf.end()) {
            this->behaviourFile << "(";
          }
          this->behaviourFile << "tfel::math::abs("
                              << "cste1_360*(this->d" << v.name << "_K1)"
                              << "-cste128_4275*(this->d" << v.name << "_K3)"
                              << "-cste2197_75240*(this->d" << v.name << "_K4)"
                              << "+cste1_50*(this->d" << v.name << "_K5)"
                              << "+cste2_55*(this->d" << v.name << "_K6))";
          if (enf.find(v.name) != enf.end()) {
            this->behaviourFile << ")/(" << enf.find(v.name)->second << ")";
          }
          this->behaviourFile << ");\n";
        } else {
          if (this->useDynamicallyAllocatedVector(v.arraySize)) {
            this->behaviourFile
                << "for(unsigned short idx=0;idx!=" << v.arraySize
                << ";++idx){\n";
            this->behaviourFile << "rk_update_error(";
            if (enf.find(v.name) != enf.end()) {
              this->behaviourFile << "(";
            }
            this->behaviourFile
                << "tfel::math::abs("
                << "cste1_360*(this->d" << v.name << "_K1[idx])"
                << "-cste128_4275*(this->d" << v.name << "_K3[idx])"
                << "-cste2197_75240*(this->d" << v.name << "_K4[idx])"
                << "+cste1_50*(this->d" << v.name << "_K5[idx])"
                << "+cste2_55*(this->d" << v.name << "_K6[idx]))";
            if (enf.find(v.name) != enf.end()) {
              this->behaviourFile << ")/(" << enf.find(v.name)->second << ")";
            }
            this->behaviourFile << ");\n";
            this->behaviourFile << "}\n";
          } else {
            for (unsigned short i = 0; i != v.arraySize; ++i) {
              this->behaviourFile << "error  = std::max(error,";
              if (enf.find(v.name) != enf.end()) {
                this->behaviourFile << "(";
              }
              this->behaviourFile
                  << "tfel::math::abs("
                  << "cste1_360*(this->d" << v.name << "_K1[" << i << "])"
                  << "-cste128_4275*(this->d" << v.name << "_K3[" << i << "])"
                  << "-cste2197_75240*(this->d" << v.name << "_K4[" << i << "])"
                  << "+cste1_50*(this->d" << v.name << "_K5[" << i << "])"
                  << "+cste2_55*(this->d" << v.name << "_K6[" << i << "]))";
              if (enf.find(v.name) != enf.end()) {
                this->behaviourFile << ")/(" << enf.find(v.name)->second << ")";
              }
              this->behaviourFile << ");\n";
            }
          }
        }
      }
    } else {
      this->throwRuntimeError("RungeKuttaDSLBase::writeBehaviourRK54Integrator",
                              "internal error, unsupported error evaluation.");
    }
    this->behaviourFile << "if(std::isnan(error)){\n"
                        << "throw(tfel::material::DivergenceException());\n"
                        << "}\n";
    if (getDebugMode()) {
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : error \" << error << endl;\n";
    }
    this->behaviourFile << "// test for convergence\n"
                        << "if(error<this->epsilon){\n"
                        << "// Final Step\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->" << v.name << " += cste16_135*(this->d"
                          << v.name << "_K1)"
                          << "+cste6656_12825*(this->d" << v.name << "_K3)"
                          << "+cste28561_56430*(this->d" << v.name << "_K4)"
                          << "-cste9_50*(this->d" << v.name << "_K5)"
                          << "+cste2_55*(this->d" << v.name << "_K6);\n";
    }
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile
          << "// updating stiffness tensor at the end of the time step\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    this->behaviourFile << "// Update stress field\n"
                        << "this->computeFinalStress();\n";
    if (d.hasCode(RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      this->behaviourFile << "this->updateAuxiliaryStateVariables(dt_);\n";
    }
    this->behaviourFile << "t += dt_;\n"
                        << "if(abs(this->dt-t)<dtprec){\n"
                        << "converged=true;\n"
                        << "}\n"
                        << "}\n"
                        << "if(!converged){\n"
                        << "// time multiplier\n"
                        << "real corrector;\n"
                        << "if(error<100*std::numeric_limits<real>::min()){\n"
                        << "corrector=real(10);\n"
                        << "} else {\n"
                        << "corrector = 0.8*pow(this->epsilon/error,0.2);\n"
                        << "}\n"
                        << "if(corrector<real(0.1f)){\n";
    if (getDebugMode()) {
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : reducing time step by a factor 10\" << endl;\n";
    }
    this->behaviourFile << "dt_ *= real(0.1f);\n";
    this->behaviourFile << "} else if(corrector>real(10)){\n";
    if (getDebugMode()) {
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : increasing time step by a factor 10\" << endl;\n";
    }
    this->behaviourFile << "dt_ *= real(10);\n";
    this->behaviourFile << "} else {\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(corrector<1){\n";
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : reducing time step by a factor "
                             "\"   << corrector << endl;\n";
      this->behaviourFile << "} else {\n";
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : increasing time step by a factor "
                             "\" << corrector << endl;\n";
      this->behaviourFile << "}\n";
    }
    this->behaviourFile
        << "dt_ *= corrector;\n"
        << "}\n"
        << "if(dt_<dtprec){\n"
        << "throw(tfel::material::DivergenceException());\n"
        << "}\n"
        << "if((abs(this->dt-t-dt_)<2*dtprec)||(t+dt_>this->dt)){\n"
        << "dt_=this->dt-t;\n"
        << "}\n"
        << "}\n"
        << "} else {\n";
    if (getDebugMode()) {
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failure detected, reducing time "
                             "step by a factor 10\" << endl;";
    }
    this->behaviourFile << "// failed is true\n"
                        << "dt_ *= real(0.1f);\n"
                        << "if(dt_<dtprec){\n"
                        << "throw(tfel::material::DivergenceException());\n"
                        << "}\n"
                        << "}\n"
                        << "}\n";
  }  // end of RungeKuttaDSLBase::writeBehaviourRK54Integrator

  void RungeKuttaDSLBase::writeBehaviourRKCastemIntegrator(const Hypothesis h) {
    using namespace std;
    const auto& d = this->mb.getBehaviourData(h);
    //! all registred variables used in ComputeDerivatives and ComputeStress
    //! blocks
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
    if (d.hasCode(BehaviourData::ComputeStress)) {
      const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).members;
      uvs.insert(uvs2.begin(), uvs2.end());
    }
    SupportedTypes::TypeSize stateVarsSize;
    for (const auto& v : d.getStateVariables()) {
      stateVarsSize += this->getTypeSize(v.type, v.arraySize);
    }
    this->behaviourFile
        << "TFEL_CONSTEXPR const auto cste1_2 = real{1}/real{2};\n"
        << "TFEL_CONSTEXPR const auto cste1_4 = real{1}/real{4};\n"
        << "TFEL_CONSTEXPR const auto cste1_6 = Type(1)/Type(6);\n"
        << "time t   = time(0);\n"
        << "time dt_ = this->dt;\n"
        << "StressStensor sigf;\n"
        << "real errabs;\n"
        << "real asig;\n"
        << "bool failed = false\n;";
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "failed=!this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n"
                          << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing initial "
                             "stress\" << endl;\n"
                          << "}\n";
    }
    this->behaviourFile
        << "if(failed){\n"
        << "throw(tfel::material::DivergenceException());\n"
        << "}\n"
        << "asig = sqrt((this->sig)|(this->sig));\n"
        << "if ((this->young)*Type(1.e-3)>asig){\n"
        << "  errabs = (this->young)*Type(1.e-3)*(this->epsilon);\n"
        << "}else{\n"
        << "  errabs = (this->epsilon)*asig;\n}\n\n"
        << "time dtprec = 100*this->dt*numeric_limits<time>::epsilon();\n"
        << "bool converged = false;\n";
    if (getDebugMode()) {
      this->behaviourFile
          << "cout << endl << \"" << this->mb.getClassName()
          << "::integrate() : beginning of resolution\" << endl;\n";
    }
    this->behaviourFile
        << "while(!converged){\n"
        << "if(dt_< dtprec){\n"
        << "cout<<\" dt \"<<this->dt<<\" t \"<<t<<\" dt_ \"<<dt_<<endl;\n"
        << "string msg(\"" << this->mb.getClassName() << "\");\n"
        << "msg += \" time step too small \"; \n"
        << "throw(runtime_error(msg)); \n"
        << "} \n";
    if (getDebugMode()) {
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : from \" << t <<  \" to \" << "
                             "t+dt_ << \" with time step \" << dt_ << endl;\n";
    }
    this->behaviourFile << "// Compute K1's values => y in castem \n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ = this->" << v.name
                            << ";\n";
      }
    }
    writeExternalVariablesCurrentValues(this->behaviourFile, this->mb, h, "0");
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile
          << "if(failed){\n"
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K1's stress\" << endl;\n"
          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n"
                          << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K1's "
                             "derivatives\" << endl;\n"
                          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "// Compute y'1*dt=f(y)*dt in castem\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K1 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "}\n";
    this->behaviourFile << "}\n\n";
    this->behaviourFile << "if(!failed){\n";
    this->behaviourFile << "// Compute K2's values => y1 in castem\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ += cste1_2*(this->d"
                            << v.name << "_K1);\n";
      }
    }
    writeExternalVariablesCurrentValues(this->behaviourFile, this->mb, h,
                                        "cste1_2");
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "// Update stress field\n"
                          << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile
          << "if(failed){\n"
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K2's stress\" << endl;\n"
          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n"
                          << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K2's "
                             "derivatives\" << endl;\n"
                          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "// Compute y'2*dt=f(y1)*dt in castem\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K2 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "}\n"
                        << "}\n"
                        << "}\n\n"
                        << "// Compute K3's values => y12 in castem\n"
                        << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ = this->" << v.name
                            << "+cste1_4*(this->d" << v.name << "_K1 + this->d"
                            << v.name << "_K2);\n";
      }
    }
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "// Update stress field\n"
                          << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile
          << "if(failed){\n"
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K3's stress\" << endl;\n"
          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n"
                          << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K3's "
                             "derivatives\" << endl;\n"
                          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "// Compute y'3*dt=f(y12)*dt in castem\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K3 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile
        << "}\n"
        << "}\n"
        << "}\n\n"
        << "if(!failed){\n"
        << "// Compute K4's values => y13 = y12+y'3*dt/2 in castem\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ += cste1_2*(this->d"
                            << v.name << "_K3);\n";
      }
    }
    writeExternalVariablesCurrentValues(this->behaviourFile, this->mb, h, "1");
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "// Update stress field\n"
                          << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile
          << "if(failed){\n"
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K4's stress\" << endl;\n"
          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n"
                          << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K4's "
                             "derivatives\" << endl;\n"
                          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "// Compute y'4*dt=f(y13)*dt in castem\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K4 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile
        << "}\n"
        << "}\n"
        << "}\n\n"
        << "if(!failed){\n"
        << "// Compute K5's values => yf = y12+0.5*(y'3+y'4)*dt/2 in castem\n"
        << "//                     => yf = y+0.5*(y'1+y'2+y'3+y'4)*dt/2 in "
           "castem\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ = this->" << v.name
                            << "+cste1_4*(this->d" << v.name << "_K1 + this->d"
                            << v.name << "_K2 + this->d" << v.name
                            << "_K3 + this->d" << v.name << "_K4);\n";
      }
    }
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "// Update stress field\n"
                          << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile
          << "if(failed){\n"
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K5's stress\" << endl;\n"
          << "}\n";
    }
    this->behaviourFile << "// Saving stresses obtained with yf\n"
                        << "sigf=this->sig;\n"
                        << "if(!failed){\n"
                        << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n"
                          << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K5's "
                             "derivatives\" << endl;\n"
                          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "// Compute y'5*dt=f(yf)*dt in castem\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K5 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "}\n"
                        << "}\n"
                        << "}\n\n"
                        << "if(!failed){\n"
                        << "// Compute K6's values => y5 = "
                           "y+1/6*(y'1+4*y'3+y'5)*dt in castem\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ = this->" << v.name
                            << "+cste1_6*(this->d" << v.name
                            << "_K1 + Type(4)*this->d" << v.name
                            << "_K3 + this->d" << v.name << "_K5);\n";
      }
    }
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "// Update stress field\n"
                          << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n"
                          << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing criterium "
                             "stress\" << endl;\n"
                          << "}\n";
    }
    this->behaviourFile
        << "}\n\n"
        << "if(!failed){\n"
        << "real ra;\n"
        << "real sqra;\n"
        << "// Computing the error\n"

        << "ra = sqrt(((sigf)-(this->sig))|((sigf)-(this->sig)))/errabs;\n"
        << "sqra = sqrt(ra);\n"
        << "// test for convergence\n"
        << "if ((sqra>" << this->mb.getClassName()
        << "::rkcastem_div)||(std::isnan(ra))){\n"
        << "dt_ /= " << this->mb.getClassName() << "::rkcastem_div;\n";
    if (getDebugMode()) {
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : multiplying time step by a factor \" << 1/("
          << this->mb.getClassName() << "::rkcastem_div) << endl;\n";
    }
    this->behaviourFile << "} else if (ra> " << this->mb.getClassName()
                        << "::rkcastem_borne){\n"
                        << "dt_ /= sqra;\n";
    if (getDebugMode()) {
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : multiplying time step by a "
                             "factor \" << 1/sqra << endl;\n";
    }
    this->behaviourFile << "}else{\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->" << v.name << " += cste1_4*(this->d"
                          << v.name << "_K1 + this->d" << v.name
                          << "_K2 + this->d" << v.name << "_K3 + this->d"
                          << v.name << "_K4);\n";
    }
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile
          << "// updating stiffness tensor at the end of the time step\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    this->behaviourFile << "this->computeFinalStress();\n";
    if (this->mb.hasCode(
            h, RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      this->behaviourFile << "this->updateAuxiliaryStateVariables(dt_);\n";
    }
    this->behaviourFile << "t += dt_;\n"
                        << "if(abs(this->dt-t)<dtprec){\n"
                        << "converged=true;\n"
                        << "}\n"
                        << "if(!converged){\n"
                        << "if ((" << this->mb.getClassName()
                        << "::rkcastem_fac)*sqra<1.){\n"
                        << "dt_ *= " << this->mb.getClassName()
                        << "::rkcastem_fac;\n";
    if (getDebugMode()) {
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : multiplying time step by a factor \" << "
          << this->mb.getClassName() << "::rkcastem_fac << endl;\n";
    }
    this->behaviourFile << "}else if ((sqra< " << this->mb.getClassName()
                        << "::rkcastem_rmin)||"
                        << "(sqra>" << this->mb.getClassName()
                        << "::rkcastem_rmax)){\n";
    this->behaviourFile << "dt_ /= sqra;\n";
    if (getDebugMode()) {
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : multiplying time step by a "
                             "factor \" << 1/sqra << endl;\n";
    }
    this->behaviourFile << "}\n"
                        << "}\n"
                        << "}\n"
                        << "} else { \n"
                        << "dt_ /=  " << this->mb.getClassName()
                        << "::rkcastem_div;\n";
    if (getDebugMode()) {
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : multiplying time step by a factor \" << 1/("
          << this->mb.getClassName() << "::rkcastem_fac) << endl;\n";
    }
    this->behaviourFile
        << "}\n"
        << "if(dt_<dtprec){\n"
        << "throw(tfel::material::DivergenceException());\n"
        << "}\n"
        << "if(!converged){\n"
        << "if((abs(this->dt-t-dt_)<2*dtprec)||(t+dt_>this->dt)){\n"
        << "dt_=this->dt-t;\n"
        << "}\n"
        << "}\n"
        << "}\n";
  }  // end of RungeKuttaDSLBase::writeBehaviourRKCastemIntegrator

  void RungeKuttaDSLBase::writeBehaviourRK42Integrator(const Hypothesis h) {
    const auto& d = this->mb.getBehaviourData(h);
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
    if (d.hasCode(BehaviourData::ComputeStress)) {
      const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).members;
      uvs.insert(uvs2.begin(), uvs2.end());
    }
    ErrorEvaluation eev;
    auto svsize = this->getTotalSize(d.getStateVariables());
    if (svsize.getScalarSize() + svsize.getTVectorSize() +
            3u * svsize.getStensorSize() + 3u * svsize.getTensorSize() >=
        20) {
      eev = MAXIMUMVALUEERROREVALUATION;
    } else {
      eev = ERRORSUMMATIONEVALUATION;
    }
    SupportedTypes::TypeSize stateVarsSize;
    for (const auto& v : d.getStateVariables()) {
      stateVarsSize += this->getTypeSize(v.type, v.arraySize);
    }
    this->behaviourFile
        << "TFEL_CONSTEXPR const auto cste1_2 = real{1}/real{2};\n"
        << "TFEL_CONSTEXPR const auto cste1_6  = Type(1)/Type(6);\n"
        << "TFEL_CONSTEXPR const auto cste1_3  = Type(1)/Type(3);\n"
        << "time t   = time(0);\n"
        << "time dt_ = this->dt;\n"
        << "time dtprec = 100*this->dt*numeric_limits<time>::epsilon();\n"
        << "Type error;\n"
        << "bool converged = false;\n";
    if (getDebugMode()) {
      this->behaviourFile
          << "cout << endl << \"" << this->mb.getClassName()
          << "::integrate() : beginning of resolution\" << endl;\n";
    }
    this->behaviourFile << "while(!converged){\n";
    if (getDebugMode()) {
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : from \" << t <<  \" to \" << "
                             "t+dt_ << \" with time step \" << dt_ << endl;\n";
    }
    this->behaviourFile << "bool failed = false;\n";
    this->behaviourFile << "// Compute K1's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ = this->" << v.name
                            << ";\n";
      }
    }
    writeExternalVariablesCurrentValues(this->behaviourFile, this->mb, h, "0");
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile
          << "if(failed){\n"
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K1's stress\" << endl;\n"
          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n"
                          << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K1's "
                             "derivatives\" << endl;\n"
                          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K1 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "}\n"
                        << "}\n\n"
                        << "if(!failed){\n"
                        << "// Compute K2's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ += cste1_2*(this->d"
                            << v.name << "_K1);\n";
      }
    }
    writeExternalVariablesCurrentValues(this->behaviourFile, this->mb, h,
                                        "cste1_2");
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "// Update stress field\n"
                          << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile
          << "if(failed){\n"
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K2's stress\" << endl;\n"
          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n"
                          << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K2's "
                             "derivatives\" << endl;\n"
                          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K2 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "}\n"
                        << "}\n"
                        << "}\n\n"
                        << "// Compute K3's values\n"
                        << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ = this->" << v.name
                            << "+cste1_2*(this->d" << v.name << "_K2);\n";
      }
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "// Update stress field\n"
                          << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile
          << "if(failed){\n"
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K3's stress\" << endl;\n"
          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n"
                          << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K3's "
                             "derivatives\" << endl;\n"
                          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K3 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "}\n"
                        << "}\n"
                        << "}\n\n"
                        << "if(!failed){\n"
                        << "// Compute K4's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ = this->" << v.name
                            << "+this->d" << v.name << "_K3;\n";
      }
    }
    writeExternalVariablesCurrentValues(this->behaviourFile, this->mb, h, "1");
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "// Update stress field\n"
                          << "failed = !this->computeStress();\n";
    }
    if (getDebugMode()) {
      this->behaviourFile
          << "if(failed){\n"
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : failed while computing K4's stress\" << endl;\n"
          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n"
                        << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(failed){\n"
                          << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : failed while computing K4's "
                             "derivatives\" << endl;\n"
                          << "}\n";
    }
    this->behaviourFile << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K4 = (dt_)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "}\n"
                        << "}\n"
                        << "}\n\n"
                        << "if(!failed){\n"
                        << "// Computing the error\n";
    if (eev == ERRORSUMMATIONEVALUATION) {
      bool first = true;
      for (const auto& v : d.getStateVariables()) {
        if (v.arraySize == 1u) {
          if (first) {
            this->behaviourFile << "error  = ";
            first = false;
          } else {
            this->behaviourFile << "error += ";
          }
          this->behaviourFile << "tfel::math::abs(";
          if (enf.find(v.name) != enf.end()) {
            this->behaviourFile << "(";
          }
          this->behaviourFile << "cste1_6*(this->d" << v.name << "_K1+"
                              << "this->d" << v.name << "_K4-"
                              << "this->d" << v.name << "_K2-"
                              << "this->d" << v.name << "_K3))";
          if (enf.find(v.name) != enf.end()) {
            this->behaviourFile << ")/(" << enf.find(v.name)->second << ")";
          }
          this->behaviourFile << ";\n";
        } else {
          if (this->useDynamicallyAllocatedVector(v.arraySize)) {
            if (first) {
              this->behaviourFile << "error  = Type(0);\n";
              first = false;
            }
            this->behaviourFile
                << "for(unsigned short idx=0;idx!=" << v.arraySize
                << ";++idx){\n"
                << "error += tfel::math::abs(";
            if (enf.find(v.name) != enf.end()) {
              this->behaviourFile << "(";
            }
            this->behaviourFile << "cste1_6*(this->d" << v.name << "_K1[idx]+"
                                << "this->d" << v.name << "_K4[idx]-"
                                << "this->d" << v.name << "_K2[idx]-"
                                << "this->d" << v.name << "_K3[idx]))";
            if (enf.find(v.name) != enf.end()) {
              this->behaviourFile << ")/(" << enf.find(v.name)->second << ")";
            }
            this->behaviourFile << ";\n";
            this->behaviourFile << "}\n";
          } else {
            for (unsigned short i = 0; i != v.arraySize; ++i) {
              if (first) {
                this->behaviourFile << "error  = ";
                first = false;
              } else {
                this->behaviourFile << "error += ";
              }
              this->behaviourFile << "tfel::math::abs(";
              if (enf.find(v.name) != enf.end()) {
                this->behaviourFile << "(";
              }
              this->behaviourFile
                  << "cste1_6*(this->d" << v.name << "_K1[" << i << "]+"
                  << "this->d" << v.name << "_K4[" << i << "]-"
                  << "this->d" << v.name << "_K2[" << i << "]-"
                  << "this->d" << v.name << "_K3[" << i << "]))";
              if (enf.find(v.name) != enf.end()) {
                this->behaviourFile << ")/(" << enf.find(v.name)->second << ")";
              }
              this->behaviourFile << ";\n";
            }
          }
        }
      }
      this->behaviourFile << "error/=" << stateVarsSize << ";\n";
    } else if (eev == MAXIMUMVALUEERROREVALUATION) {
      this->behaviourFile
          << "error  = Type(0);\n"
          << "auto rk_update_error = [&error](const real rk_error){\n"
          << "if(!std::isfinite(error)){return;}\n"
          << "if(!std::isfinite(rk_error)){\n"
          << "error = rk_error;\n"
          << "return;\n"
          << "}\n"
          << "error = std::max(error, rk_error);\n"
          << "};\n";
      for (const auto& v : d.getStateVariables()) {
        if (v.arraySize == 1u) {
          this->behaviourFile << "rk_update_error(tfel::math::abs(";
          if (enf.find(v.name) != enf.end()) {
            this->behaviourFile << "(";
          }
          this->behaviourFile << "cste1_6*(this->d" << v.name << "_K1+"
                              << "this->d" << v.name << "_K4-"
                              << "this->d" << v.name << "_K2-"
                              << "this->d" << v.name << "_K3))";
          if (enf.find(v.name) != enf.end()) {
            this->behaviourFile << ")/(" << enf.find(v.name)->second << ")";
          }
          this->behaviourFile << ");\n";
        } else {
          if (this->useDynamicallyAllocatedVector(v.arraySize)) {
            this->behaviourFile
                << "for(unsigned short idx=0;idx!=" << v.arraySize
                << ";++idx){\n";
            this->behaviourFile << "rk_update_error(tfel::math::abs(";
            if (enf.find(v.name) != enf.end()) {
              this->behaviourFile << "(";
            }
            this->behaviourFile << "cste1_6*(this->d" << v.name << "_K1[idx]+"
                                << "this->d" << v.name << "_K4[idx]-"
                                << "this->d" << v.name << "_K2[idx]-"
                                << "this->d" << v.name << "_K3[idx]))";
            if (enf.find(v.name) != enf.end()) {
              this->behaviourFile << ")/(" << enf.find(v.name)->second << ")";
            }
            this->behaviourFile << ");\n";
            this->behaviourFile << "}\n";
          } else {
            for (unsigned short i = 0; i != v.arraySize; ++i) {
              this->behaviourFile << "rk_update_error(tfel::math::abs(";
              if (enf.find(v.name) != enf.end()) {
                this->behaviourFile << "(";
              }
              this->behaviourFile
                  << "cste1_6*(this->d" << v.name << "_K1[" << i << "]+"
                  << "this->d" << v.name << "_K4[" << i << "]-"
                  << "this->d" << v.name << "_K2[" << i << "]-"
                  << "this->d" << v.name << "_K3[" << i << "]))";
              if (enf.find(v.name) != enf.end()) {
                this->behaviourFile << ")/(" << enf.find(v.name)->second << ")";
              }
              this->behaviourFile << ");\n";
            }
          }
        }
      }
    } else {
      this->throwRuntimeError("RungeKuttaDSLBase::writeBehaviourRK42Integrator",
                              "internal error, unsupported error evaluation");
    }
    this->behaviourFile << "if(std::isnan(error)){\n"
                        << "throw(tfel::material::DivergenceException());\n"
                        << "}\n";
    if (getDebugMode()) {
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : error \" << error << endl;\n";
    }
    this->behaviourFile << "// test for convergence\n"
                        << "if(error<this->epsilon){\n"
                        << "// Final Step\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->" << v.name << " += cste1_6*(this->d"
                          << v.name << "_K1 + this->d" << v.name << "_K4)+"
                          << "    cste1_3*(this->d" << v.name << "_K3 + this->d"
                          << v.name << "_K2);\n";
    }
    this->behaviourFile << "this->computeFinalStress();\n";
    if (this->mb.hasCode(
            h, RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      this->behaviourFile << "this->updateAuxiliaryStateVariables(dt_);\n";
    }
    this->behaviourFile << "t += dt_;\n"
                        << "if(abs(this->dt-t)<dtprec){\n"
                        << "converged=true;\n"
                        << "}\n"
                        << "}\n"
                        << "if(!converged){"
                        << "// time multiplier\n"
                        << "real corrector;\n"
                        << "if(error<100*std::numeric_limits<real>::min()){\n"
                        << "corrector=real(10.);\n"
                        << "} else {\n"
                        << "corrector = 0.8*pow((this->epsilon)/error,1./3.);\n"
                        << "}\n"
                        << "if(corrector<=real(0.1f)){\n"
                        << "dt_ *= real(0.1f);\n";
    if (getDebugMode()) {
      this->behaviourFile
          << "cout << \"" << this->mb.getClassName()
          << "::integrate() : reducing time step by a factor 10\" << endl;\n";
    }
    this->behaviourFile << "} else if(corrector>real(10)){\n";
    this->behaviourFile << "dt_ *= real(10);\n";
    if (getDebugMode()) {
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : multiplying time step by a "
                             "factor 10\" << endl;\n";
    }
    this->behaviourFile << "} else {\n";
    this->behaviourFile << "dt_ *= corrector;\n";
    if (getDebugMode()) {
      this->behaviourFile << "if(corrector<1){\n";
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : reducing time step by a factor "
                             "\"   << corrector << endl;\n";
      this->behaviourFile << "} else {\n";
      this->behaviourFile << "cout << \"" << this->mb.getClassName()
                          << "::integrate() : increasing time step by a factor "
                             "\" << corrector << endl;\n";
      this->behaviourFile << "}\n";
    }
    this->behaviourFile
        << "}\n"
        << "if(dt_<dtprec){\n"
        << "throw(tfel::material::DivergenceException());\n"
        << "}\n"
        << "if((abs(this->dt-t-dt_)<2*dtprec)||(t+dt_>this->dt)){\n"
        << "dt_=this->dt-t;\n"
        << "}\n"
        << "}\n"
        << "} else {\n"
        << "// failed is true\n"
        << "dt_ *= real(0.1f);\n"
        << "if(dt_<dtprec){\n"
        << "throw(tfel::material::DivergenceException());\n"
        << "}\n"
        << "}\n"
        << "}\n";
  }  // end of RungeKuttaDSLBase::writeBehaviourRK42Integrator

  void RungeKuttaDSLBase::writeBehaviourRK4Integrator(const Hypothesis h) {
    const auto btype = this->mb.getBehaviourTypeFlag();
    const auto& d = this->mb.getBehaviourData(h);
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
    if (d.hasCode(BehaviourData::ComputeStress)) {
      const auto& uvs2 = d.getCodeBlock(BehaviourData::ComputeStress).members;
      uvs.insert(uvs2.begin(), uvs2.end());
    }
    this->behaviourFile
        << "TFEL_CONSTEXPR const auto cste1_2 = real{1}/real{2};\n"
        << "// Compute K1's values\n";
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "this->computeStress();\n";
    }
    this->behaviourFile << "if(!this->computeDerivative()){\n\n";
    if (this->mb.useQt()) {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K1 = (this->dt)*(this->d"
                          << v.name << ");\n";
    }
    writeExternalVariablesCurrentValues2(this->behaviourFile, this->mb, h,
                                         "cste1_2");
    for (const auto& iv : d.getStateVariables()) {
      if (uvs.find(iv.name) != uvs.end()) {
        this->behaviourFile << "this->" << iv.name << "_ += cste1_2*(this->d"
                            << iv.name << "_K1);\n";
      }
    }
    if ((this->mb.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      this->behaviourFile << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(
          this->behaviourFile, "this->D",
          modifyVariableForStiffnessTensorComputation());
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "// Update stress field\n"
                          << "this->computeStress();\n\n";
    }
    this->behaviourFile << "// Compute K2's values\n"
                        << "if(!this->computeDerivative()){\n\n";
    if (this->mb.useQt()) {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K2 = (this->dt)*(this->d"
                          << v.name << ");\n";
    }
    for (const auto& iv : d.getStateVariables()) {
      if (uvs.find(iv.name) != uvs.end()) {
        this->behaviourFile << "this->" << iv.name << "_ = "
                            << "this->" << iv.name << "+ cste1_2*(this->d"
                            << iv.name << "_K2);\n";
      }
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "// Update stress field\n"
                          << "this->computeStress();\n\n";
    }
    this->behaviourFile << "// Compute K3's values\n"
                        << "if(!this->computeDerivative()){\n\n";
    if (this->mb.useQt()) {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K3 = (this->dt)*(this->d"
                          << v.name << ");\n";
    }
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        this->behaviourFile << "this->" << v.name << "_ = "
                            << "this->" << v.name << "+ (this->d" << v.name
                            << "_K3);\n";
      }
    }
    if (this->mb.hasCode(h, BehaviourData::ComputeStress)) {
      this->behaviourFile << "// Update stress field\n"
                          << "this->computeStress();\n\n";
    }
    this->behaviourFile << "// Compute K4's values\n"
                        << "if(!this->computeDerivative()){\n\n";
    if (this->mb.useQt()) {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype
                          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    this->behaviourFile << "}\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->d" << v.name << "_K4 = (this->dt)*(this->d"
                          << v.name << ");\n";
    }
    this->behaviourFile << "// Final Step\n";
    for (const auto& v : d.getStateVariables()) {
      this->behaviourFile << "this->" << v.name << " += "
                          << "(this->d" << v.name << "_K1+this->d" << v.name
                          << "_K4)/6+\n";
      this->behaviourFile << "(this->d" << v.name << "_K2+this->d" << v.name
                          << "_K3)/3;\n";
    }
    this->behaviourFile << "// Update stress field\n"
                        << "this->computeFinalStress();\n";
    if (this->mb.hasCode(
            h, RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      this->behaviourFile << "this->updateAuxiliaryStateVariables(this->dt);\n";
    }
  }  // end of RungeKuttaDSLBase::writeBehaviourRK4Integrator

  void RungeKuttaDSLBase::writeBehaviourIntegrator(const Hypothesis h) {
    const auto btype = this->mb.getBehaviourTypeFlag();
    const auto& algorithm =
        this->mb.getAttribute<std::string>(BehaviourData::algorithm);
    const auto& d = this->mb.getBehaviourData(h);
    this->checkBehaviourFile();
    this->behaviourFile
        << "/*!\n"
        << "* \\brief Integrate behaviour law over the time step\n"
        << "*/\n"
        << "IntegrationResult\n";
    if (this->mb.hasAttribute(h, BehaviourData::hasConsistentTangentOperator)) {
      this->behaviourFile << "integrate(const SMFlag smflag,const SMType smt) "
                             "override final{\n";
    } else {
      if ((this->mb.getBehaviourType() ==
           BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
          (this->mb.getBehaviourType() ==
           BehaviourDescription::COHESIVEZONEMODEL)) {
        this->behaviourFile
            << "integrate(const SMFlag smflag,const SMType smt) "
            << "override final{\n";
      } else {
        this->behaviourFile << "integrate(const SMFlag,const SMType smt) "
                            << "override final{\n";
      }
    }
    this->behaviourFile << "using namespace std;\n"
                        << "using namespace tfel::math;\n";
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
    if (this->mb.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfilingBegin(
          this->behaviourFile, mb.getClassName(), BehaviourData::Integrator);
    }
    if (algorithm == "Euler") {
      this->writeBehaviourEulerIntegrator(h);
    } else if (algorithm == "RungeKutta2") {
      this->writeBehaviourRK2Integrator(h);
    } else if (algorithm == "RungeKutta4/2") {
      this->writeBehaviourRK42Integrator(h);
    } else if (algorithm == "RungeKutta5/4") {
      this->writeBehaviourRK54Integrator(h);
    } else if (algorithm == "RungeKuttaCastem") {
      this->writeBehaviourRKCastemIntegrator(h);
    } else if (algorithm == "RungeKutta4") {
      this->writeBehaviourRK4Integrator(h);
    } else {
      this->throwRuntimeError(
          "RungeKuttaDSLBase::writeBehaviourIntegrator",
          "internal error\n'" + algorithm +
              "' is not a known algorithm. "
              "This shall not happen at this stage."
              " Please contact MFront developper to help them debug this.");
    }
    this->behaviourFile << "this->updateAuxiliaryStateVariables();\n";
    for (const auto& b : d.getBounds()) {
      if (b.varCategory == BoundsDescription::StateVariable) {
        b.writeBoundsChecks(this->behaviourFile);
      }
    }
    if (this->mb.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfilingEnd(this->behaviourFile);
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
                          << "::integrate : \");\n"
                          << "msg +=\"unimplemented feature\";\n"
                          << "throw(runtime_error(msg));\n";
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
  }  // end of void RungeKuttaDSLBase::writeBehaviourIntegrator(void)

  RungeKuttaDSLBase::~RungeKuttaDSLBase() = default;

}  // end of namespace mfront
