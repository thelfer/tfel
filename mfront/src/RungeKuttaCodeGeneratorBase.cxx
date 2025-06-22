/*!
 * \file   mfront/src/RungeKuttaDSLBaseCodeGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/08/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include <string_view>
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/PerformanceProfiling.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/RungeKuttaDSLBase.hxx"
#include "MFront/RungeKuttaCodeGeneratorBase.hxx"

namespace mfront {

  static std::set<std::string> getVariablesUsedDuringIntegration(
      const BehaviourDescription& mb,
      const RungeKuttaCodeGeneratorBase::Hypothesis h) {
    const auto& d = mb.getBehaviourData(h);
    //! all registred members used in this block
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
    if (d.hasCode(BehaviourData::ComputeThermodynamicForces)) {
      const auto& uvs2 =
          d.getCodeBlock(BehaviourData::ComputeThermodynamicForces).members;
      uvs.insert(uvs2.begin(), uvs2.end());
    }
    // variables used to compute the stiffness tensor
    if ((mb.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                               false)) &&
        (!mb.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      const auto& mps = mb.getElasticMaterialProperties();
      for (const auto& mp : mps) {
        if (mp.is<BehaviourDescription::ExternalMFrontMaterialProperty>()) {
          const auto& cmp =
              mp.get<BehaviourDescription::ExternalMFrontMaterialProperty>();
          for (const auto& i : mb.getMaterialPropertyInputs(*(cmp.mpd))) {
            uvs.insert(i.name);
          }
        }
        if (mp.is<BehaviourDescription::AnalyticMaterialProperty>()) {
          const auto& amp =
              mp.get<BehaviourDescription::AnalyticMaterialProperty>();
          for (const auto& i :
               mb.getMaterialPropertyInputs(amp.getVariablesNames())) {
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
      const RungeKuttaCodeGeneratorBase::Hypothesis h,
      const std::string& p) {
    const auto t = ((p == "0") ? "(t/this->dt)"
                               : ((p == "1") ? "((t+dt_)/this->dt)"
                                             : "((t+" + p + "*dt_)/this->dt)"));
    const auto& d = mb.getBehaviourData(h);
    const auto uvs = getVariablesUsedDuringIntegration(mb, h);
    for (const auto& mv : mb.getMainVariables()) {
      const auto& dv = mv.first;
      if (uvs.find(dv.name) != uvs.end()) {
        if (Gradient::isIncrementKnown(dv)) {
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
  modifyVariableForStiffnessTensorComputation(const std::string& cn) {
    using MaterialPropertyInput = BehaviourDescription::MaterialPropertyInput;
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    static Modifier m =
        [cn](const BehaviourDescription::MaterialPropertyInput& i) {
          if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
              (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
            return "this->" + i.name + "_";
          } else if (i.category ==
                     MaterialPropertyInput::
                         AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) {
            return "(this->" + i.name + "+(t/(this->dt))*(this->d" + i.name +
                   "))";
          } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                     (i.category == MaterialPropertyInput::PARAMETER)) {
            return "this->" + i.name;
          } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
            return cn + "::" + i.name;
          } else {
            tfel::raise(
                "modifyVariableForStiffnessTensorComputation: "
                "unsupported input type for "
                "variable '" +
                i.name + "'");
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
  modifyVariableForStiffnessTensorComputation2(const std::string& cn) {
    using MaterialPropertyInput = BehaviourDescription::MaterialPropertyInput;
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    static Modifier m = [cn](const BehaviourDescription::MaterialPropertyInput&
                                 i) {
      if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
          (i.category ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+this->d" + i.name;
      } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.category == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
        return cn + "::" + i.name;
      } else {
        tfel::raise(
            "modifyVariableForStiffnessTensorComputation2: "
            "unsupported input type for "
            "variable '" +
            i.name + "'");
      }
    };
    return m;
  }  // end of modifyVariableForStiffnessTensorComputation2

  static void writeExternalVariablesCurrentValues2(
      std::ostream& f,
      const BehaviourDescription& mb,
      const RungeKuttaCodeGeneratorBase::Hypothesis h,
      const std::string& p) {
    const auto& d = mb.getBehaviourData(h);
    const auto uvs = getVariablesUsedDuringIntegration(mb, h);
    for (const auto& mv : mb.getMainVariables()) {
      const auto& dv = mv.first;
      if (uvs.find(dv.name) != uvs.end()) {
        writeExternalVariableCurrentValue2(f, dv.name, p,
                                           Gradient::isIncrementKnown(dv));
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
    for (const auto& v : d.getExternalStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        writeExternalVariableCurrentValue2(f, v.name, p, true);
      }
    }
  }  // end of writeExternalVariablesCurrentValues2

  static void writeReturnSuccess(std::ostream& os,
                                 const BehaviourDescription& bd) {
    const auto btype = bd.getBehaviourTypeFlag();
    if (bd.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,use_qt>::SUCCESS;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,false>::SUCCESS;\n";
    }
  }  // end of writeReturnSuccess

  static void writeReturnFailure(std::ostream& os,
                                 const BehaviourDescription& bd) {
    const auto btype = bd.getBehaviourTypeFlag();
    if (bd.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,use_qt>::FAILURE;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType,false>::FAILURE;\n";
    }
  }  // end of writeReturnFailure

  static void writeAlgorithmConstant(std::ostream& os,
                                     const std::string_view n,
                                     const std::string_view d) {
    os << "constexpr auto cste" << n << "_" << d << " = "  //
       << "NumericType{" << n << "}/NumericType{" << d << "};\n";
  }  // end of writeAlgorithmConstant

  RungeKuttaCodeGeneratorBase::RungeKuttaCodeGeneratorBase(
      const FileDescription& f,
      const BehaviourDescription& d,
      const BehaviourInterfaceMap& bim)
      : BehaviourCodeGeneratorBase(f, d, bim) {
  }  // end of RungeKuttaCodeGeneratorBase

  bool RungeKuttaCodeGeneratorBase::usesStateVariableTimeDerivative() const {
    return true;
  }  // end of usesStateVariableTimeDerivative

  void RungeKuttaCodeGeneratorBase::writeBehaviourParserSpecificIncludes(
      std::ostream& os) const {
    auto b1 = false;
    auto b2 = false;
    this->checkBehaviourFile(os);
    os << "#include\"TFEL/Math/General/Abs.hxx\"\n\n";
    this->bd.requiresTVectorOrVectorIncludes(b1, b2);
    if (b1) {
      os << "#include\"TFEL/Math/tvector.hxx\"\n";
    }
    if (b2) {
      os << "#include\"TFEL/Math/vector.hxx\"\n";
    }
  }

  void RungeKuttaCodeGeneratorBase::writeBehaviourLocalVariablesInitialisation(
      std::ostream& os, const Hypothesis h) const {
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    if (this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                              false)) {
      os << "// the stiffness tensor at the beginning of the time step\n";
      Modifier bts = [this](const MaterialPropertyInput& i) {
        if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
            (i.category ==
             MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
            (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE) ||
            (i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
            (i.category == MaterialPropertyInput::PARAMETER)) {
          return "this->" + i.name;
        } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
          return this->bd.getClassName() + "::" + i.name;
        } else {
          this->throwRuntimeError(
              "RungeKuttaCodeGeneratorBase::"
              "writeBehaviourLocalVariablesInitialisation",
              "unsupported input type for variable '" + i.name + "'");
        }
      };
      this->writeStiffnessTensorComputation(os, "this->D", bts);
    }
    BehaviourCodeGeneratorBase::writeBehaviourLocalVariablesInitialisation(os,
                                                                           h);
  }  // end of writeBehaviourLocalVariablesInitialisation

  void RungeKuttaCodeGeneratorBase::writeBehaviourParserSpecificTypedefs(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
  }

  void RungeKuttaCodeGeneratorBase::writeBehaviourParserSpecificMembers(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "[[nodiscard]] bool\ncomputeThermodynamicForces(){\n"
         << "using namespace std;\n"
         << "using namespace tfel::math;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      os << this->bd.getCode(h, BehaviourData::ComputeThermodynamicForces)
         << '\n'
         << "return true;\n"
         << "} // end of " << this->bd.getClassName()
         << "::computeThermodynamicForces\n\n";
    }
    if (!this->bd.getMainVariables().empty()) {
      os << "[[nodiscard]] bool\ncomputeFinalThermodynamicForces(){\n"
         << "using namespace std;\n"
         << "using namespace tfel::math;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      os << this->bd.getCode(h, BehaviourData::ComputeFinalThermodynamicForces)
         << '\n'
         << "return true;\n"
         << "} // end of " << this->bd.getClassName()
         << "::computeFinalThermodynamicForces\n\n";
    }
    os << "[[nodiscard]] bool\ncomputeDerivative(){\n"
       << "using namespace std;\n"
       << "using namespace tfel::math;\n";
    writeMaterialLaws(os, this->bd.getMaterialLaws());
    os << this->bd.getCode(h, BehaviourData::ComputeDerivative) << '\n'
       << "return true;\n"
       << "} // end of " << this->bd.getClassName()
       << "::computeDerivative\n\n";
  }  // end of writeBehaviourParserSpecificMembers

  void RungeKuttaCodeGeneratorBase::writeBehaviourUpdateStateVariables(
      std::ostream&, const Hypothesis) const {
    // Disabled (makes no sense for this parser)
  }  // end of writeBehaviourUpdateStateVariables

  void RungeKuttaCodeGeneratorBase::writeBehaviourUpdateAuxiliaryStateVariables(
      std::ostream& os, const Hypothesis h) const {
    BehaviourCodeGeneratorBase::writeBehaviourUpdateAuxiliaryStateVariables(os,
                                                                            h);
    if (this->bd.hasCode(
            h, RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      os << "/*!\n"
         << "* \\brief Update auxiliary state variables at end of integration\n"
         << "*/\n"
         << "TFEL_HOST_DEVICE void\n"
         << "updateAuxiliaryStateVariables(const time dt_)"
         << "{\n"
         << "static_cast<void>(dt_);\n"
         << "using namespace std;\n"
         << "using namespace tfel::math;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      os << this->bd.getCode(
                h, RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)
         << '\n'
         << "}\n\n";
    }
  }  // end of writeBehaviourUpdateAuxiliaryStateVariables

  void RungeKuttaCodeGeneratorBase::writeBehaviourEulerIntegrator(
      std::ostream& os, const Hypothesis h) const {
    const auto btype = this->bd.getBehaviourTypeFlag();
    const auto& d = this->bd.getBehaviourData(h);
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "if(!this->computeThermodynamicForces()){\n";
      writeReturnFailure(os, this->bd);
      os << "}\n";
    }
    os << "if(!this->computeDerivative()){\n";
    writeReturnFailure(os, this->bd);
    os << "}\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->" << v.name << " += "
         << "this->dt*(this->d" << v.name << ");\n";
    }
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      os << "// updating stiffness tensor at the end of the time step\n";
      auto md =
          modifyVariableForStiffnessTensorComputation2(this->bd.getClassName());
      this->writeStiffnessTensorComputation(os, "this->D", md);
    }
    if (!this->bd.getMainVariables().empty()) {
      os << "// update the thermodynamic forces\n"
         << "if(!this->computeFinalThermodynamicForces()){\n";
      writeReturnFailure(os, this->bd);
      os << "}\n";
    }
    if (d.hasCode(RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      os << "this->updateAuxiliaryStateVariables(this->dt);\n";
    }
  }  // end of writeBehaviourEulerIntegrator

  void RungeKuttaCodeGeneratorBase::writeBehaviourRK2Integrator(
      std::ostream& os, const Hypothesis h) const {
    const auto btype = this->bd.getBehaviourTypeFlag();
    const auto& d = this->bd.getBehaviourData(h);
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
    if (d.hasCode(BehaviourData::ComputeThermodynamicForces)) {
      const auto& uvs2 =
          d.getCodeBlock(BehaviourData::ComputeThermodynamicForces).members;
      uvs.insert(uvs2.begin(), uvs2.end());
    }
    writeAlgorithmConstant(os, "1", "2");
    os << "// Compute K1's values\n";
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "if(!this->computeThermodynamicForces()){\n";
      writeReturnFailure(os, this->bd);
      os << "}\n";
    }
    os << "if(!this->computeDerivative()){\n";
    writeReturnFailure(os, this->bd);
    os << "}\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K1 = (this->dt)*(this->d" << v.name
         << ");\n";
    }
    for (const auto& iv : d.getStateVariables()) {
      if (uvs.find(iv.name) != uvs.end()) {
        os << "this->" << iv.name << "_ += cste1_2*(this->d" << iv.name
           << "_K1);\n";
      }
    }
    writeExternalVariablesCurrentValues2(os, this->bd, h, "cste1_2");
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      os << "// updating the stiffness tensor\n";
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "if(!this->computeThermodynamicForces()){\n";
      writeReturnFailure(os, this->bd);
      os << "}\n";
    }
    os << "if(!this->computeDerivative()){\n";
    writeReturnFailure(os, this->bd);
    os << "}\n";
    os << "// Final Step\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->" << v.name << " += "
         << "this->dt*(this->d" << v.name << ");\n";
    }
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      os << "// updating stiffness tensor at the end of the time step\n";
      auto m =
          modifyVariableForStiffnessTensorComputation2(this->bd.getClassName());
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (!this->bd.getMainVariables().empty()) {
      os << "// update the thermodynamic forces\n"
         << "if(!this->computeFinalThermodynamicForces()){\n";
      writeReturnFailure(os, this->bd);
      os << "}\n";
    }
    if (d.hasCode(RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      os << "this->updateAuxiliaryStateVariables(this->dt);\n";
    }
  }  // end of writeBehaviourRK2Integrator

  void RungeKuttaCodeGeneratorBase::writeBehaviourRK54Integrator(
      std::ostream& os, const Hypothesis h) const {
    auto get_enf = [](const VariableDescription& v) {
      return v.getAttribute<std::string>(
          VariableDescription::errorNormalisationFactor);
    };
    const auto& d = this->bd.getBehaviourData(h);
    auto shallUpdateInternalStateValues = [this, d, h] {
      const auto uvs = getVariablesUsedDuringIntegration(this->bd, h);
      for (const auto& v : d.getIntegrationVariables()) {
        if (uvs.find(v.name) != uvs.end()) {
          return true;
        }
      }
      return false;
    }();
    auto shallUpdateExternalStateValues = [this, d, h] {
      const auto uvs = getVariablesUsedDuringIntegration(this->bd, h);
      for (const auto& mv : this->bd.getMainVariables()) {
        const auto& dv = mv.first;
        if (uvs.find(dv.name) != uvs.end()) {
          return true;
        }
      }
      for (const auto& v : d.getExternalStateVariables()) {
        if (uvs.find(v.name) != uvs.end()) {
          return true;
        }
      }
      return false;
    }();
    //! all registred variables used in ComputeDerivatives and
    //! ComputeThermodynamicForces blocks
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
    if (d.hasCode(BehaviourData::ComputeThermodynamicForces)) {
      const auto& uvs2 =
          d.getCodeBlock(BehaviourData::ComputeThermodynamicForces).members;
      uvs.insert(uvs2.begin(), uvs2.end());
    }
    ErrorEvaluation eev;
    auto svsize = d.getStateVariables().getTypeSize();
    if (svsize.getValueForDimension(1) >= 20) {
      eev = MAXIMUMVALUEERROREVALUATION;
    } else {
      eev = ERRORSUMMATIONEVALUATION;
    }
    if (shallUpdateExternalStateValues) {
      writeAlgorithmConstant(os, "1", "2");
      writeAlgorithmConstant(os, "3", "8");
      writeAlgorithmConstant(os, "12", "13");
    }
    if (shallUpdateInternalStateValues) {
      writeAlgorithmConstant(os, "3544", "2565");
      writeAlgorithmConstant(os, "11", "40");
      writeAlgorithmConstant(os, "1859", "4104");
      writeAlgorithmConstant(os, "8", "27");
      writeAlgorithmConstant(os, "845", "4104");
      writeAlgorithmConstant(os, "3680", "513");
      writeAlgorithmConstant(os, "439", "216");
      writeAlgorithmConstant(os, "7296", "2197");
      writeAlgorithmConstant(os, "7200", "2197");
      writeAlgorithmConstant(os, "3", "32");
      writeAlgorithmConstant(os, "1932", "2197");
    }
    writeAlgorithmConstant(os, "1", "4");
    writeAlgorithmConstant(os, "16", "135");
    writeAlgorithmConstant(os, "6656", "12825");
    writeAlgorithmConstant(os, "28561", "56430");
    writeAlgorithmConstant(os, "9", "50");
    writeAlgorithmConstant(os, "2", "55");
    writeAlgorithmConstant(os, "1", "360");
    writeAlgorithmConstant(os, "128", "4275");
    writeAlgorithmConstant(os, "2197", "75240");
    writeAlgorithmConstant(os, "1", "50");
    os << "time t      = time(0);\n"
       << "time dt_    = this->dt;\n"
       << "time dtprec = 100* (this->dt) * "
       << "std::numeric_limits<NumericType>::epsilon();\n"
       << "auto error = NumericType{};\n"
       << "bool converged = false;\n";
    if (getDebugMode()) {
      os << "cout << endl << \"" << this->bd.getClassName()
         << "::integrate() : beginning of resolution\" << endl;\n";
    }
    os << "while(!converged){\n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : from \" << t <<  \" to \" << t+dt_ << \" with "
            "time step \" << dt_ << endl;\n";
    }
    os << "bool failed = false;\n";
    os << "// Compute K1's values\n";
    writeExternalVariablesCurrentValues(os, this->bd, h, "0");
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      os << "// updating the stiffness tensor\n";
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ = this->" << v.name << ";\n";
      }
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K1's stress\" << endl;\n";
      os << "}\n";
    }
    os << "if(!failed){\n";
    os << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K1's derivative\" << "
            "endl;\n";
      os << "}\n";
    }
    os << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K1 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n";
    os << "}\n\n";
    os << "if(!failed){\n";
    os << "// Compute K2's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ += cste1_4*(this->d" << v.name
           << "_K1);\n";
      }
    }
    writeExternalVariablesCurrentValues(os, this->bd, h, "cste1_4");
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    os << "// update the thermodynamic forces\n";
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K2's stress\" << endl;\n";
      os << "}\n";
    }
    os << "if(!failed){\n";
    os << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K2's derivative\" << "
            "endl;\n";
      os << "}\n";
    }
    os << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K2 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n"
       << "}\n"
       << "}\n\n"
       << "if(!failed){\n"
       << "// Compute K3's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ = this->" << v.name
           << "+cste3_32*(this->d" << v.name << "_K1+3*(this->d" << v.name
           << "_K2));\n";
      }
    }
    writeExternalVariablesCurrentValues(os, this->bd, h, "cste3_8");
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "// update the thermodynamic forces\n"
         << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K3's stress\" << endl;\n";
      os << "}\n";
    }
    os << "if(!failed){\n";
    os << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K3's derivative\" << "
            "endl;\n";
      os << "}\n";
    }
    os << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K3 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n";
    os << "}\n";
    os << "}\n\n";

    os << "if(!failed){\n";
    os << "// Compute K4's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ = this->" << v.name
           << "+cste1932_2197*(this->d" << v.name << "_K1)"
           << "-cste7200_2197*(this->d" << v.name << "_K2)"
           << "+cste7296_2197*(this->d" << v.name << "_K3);\n";
      }
    }
    writeExternalVariablesCurrentValues(os, this->bd, h, "cste12_13");
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    os << "// update the thermodynamic forces\n";
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K4's stress\" << endl;\n";
      os << "}\n";
    }
    os << "if(!failed){\n";
    os << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K4's derivatives\" << "
            "endl;\n";
      os << "}\n";
    }
    os << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K4 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n"
       << "}\n"
       << "}\n\n"
       << "if(!failed){\n"
       << "// Compute K5's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ = this->" << v.name
           << "+cste439_216*(this->d" << v.name << "_K1)"
           << "-8*(this->d" << v.name << "_K2)"
           << "+cste3680_513*(this->d" << v.name << "_K3)"
           << "-cste845_4104*(this->d" << v.name << "_K4);\n";
      }
    }
    writeExternalVariablesCurrentValues(os, this->bd, h, "1");
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    os << "// update the thermodynamic forces\n";
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K5's stress\" << endl;\n";
      os << "}\n";
    }
    os << "if(!failed){\n";
    os << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K5's derivatives\" << "
            "endl;\n";
      os << "}\n";
    }
    os << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K5 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n"
       << "}\n"
       << "}\n\n"
       << "if(!failed){\n"
       << "// Compute K6's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ = this->" << v.name
           << "-cste8_27*(this->d" << v.name << "_K1)"
           << "+2*(this->d" << v.name << "_K2)"
           << "-cste3544_2565*(this->d" << v.name << "_K3)"
           << "+cste1859_4104*(this->d" << v.name << "_K4)"
           << "-cste11_40*(this->d" << v.name << "_K5);\n";
      }
    }
    writeExternalVariablesCurrentValues(os, this->bd, h, "cste1_2");
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "// update the thermodynamic forces\n"
         << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K6's stress\" << endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K6's derivatives\" << "
            "endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K6 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n"
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
            os << "error  = ";
          } else {
            os << "error += ";
          }
          if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
            os << "(";
          }
          os << "tfel::math::base_type_cast(tfel::math::abs("
             << "cste1_360*(this->d" << v.name << "_K1)"
             << "-cste128_4275*(this->d" << v.name << "_K3)"
             << "-cste2197_75240*(this->d" << v.name << "_K4)"
             << "+cste1_50*(this->d" << v.name << "_K5)"
             << "+cste2_55*(this->d" << v.name << "_K6)))";
          if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
            os << "))/(" << get_enf(v) << ")";
          }
          os << ";\n";
        } else {
          if (this->bd.useDynamicallyAllocatedVector(v.arraySize)) {
            if (p == d.getStateVariables().begin()) {
              os << "error  = NumericType(0);\n";
            }
            os << "for(unsigned short idx=0;idx!=" << v.arraySize
               << ";++idx){\n";
            os << "error += ";
            if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
              os << "(";
            }
            os << "tfel::math::base_type_cast(tfel::math::abs(";
            os << "cste1_360*(this->d" << v.name << "_K1[idx])"
               << "-cste128_4275*(this->d" << v.name << "_K3[idx])"
               << "-cste2197_75240*(this->d" << v.name << "_K4[idx])"
               << "+cste1_50*(this->d" << v.name << "_K5[idx])"
               << "+cste2_55*(this->d" << v.name << "_K6[idx])))";
            if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
              os << ")/(" << get_enf(v) << ")";
            }
            os << ";\n";
            os << "}\n";
          } else {
            for (unsigned short i = 0; i != v.arraySize; ++i) {
              if ((p == d.getStateVariables().begin()) && (i == 0)) {
                os << "error  = ";
              } else {
                os << "error += ";
              }
              if (v.hasAttribute(
                      VariableDescription::errorNormalisationFactor)) {
                os << "(";
              }
              os << "tfel::math::base_type_case(tfel::math::abs("
                 << "cste1_360*(this->d" << v.name << "_K1[" << i << "])"
                 << "-cste128_4275*(this->d" << v.name << "_K3[" << i << "])"
                 << "-cste2197_75240*(this->d" << v.name << "_K4[" << i << "])"
                 << "+cste1_50*(this->d" << v.name << "_K5[" << i << "])"
                 << "+cste2_55*(this->d" << v.name << "_K6[" << i << "])))";
              if (v.hasAttribute(
                      VariableDescription::errorNormalisationFactor)) {
                os << ")/(" << get_enf(v) << ")";
              }
              os << ";\n";
            }
          }
        }
      }
      os << "error/=" << svsize << ";\n";
    } else if (eev == MAXIMUMVALUEERROREVALUATION) {
      os << "error  = NumericType(0);\n"
         << "auto rk_update_error = [&error](const auto rk_error){\n"
         << "if(!ieee754::isfinite(error)){return;}\n"
         << "if(!ieee754::isfinite(rk_error)){\n"
         << "error = tfel::math::base_type_cast(rk_error);\n"
         << "return;\n"
         << "}\n"
         << "error = std::max(error, tfel::math::base_type_cast(rk_error));\n"
         << "};\n";
      for (const auto& v : d.getStateVariables()) {
        if (v.arraySize == 1u) {
          os << "rk_update_error(";
          if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
            os << "(";
          }
          os << "tfel::math::abs("
             << "cste1_360*(this->d" << v.name << "_K1)"
             << "-cste128_4275*(this->d" << v.name << "_K3)"
             << "-cste2197_75240*(this->d" << v.name << "_K4)"
             << "+cste1_50*(this->d" << v.name << "_K5)"
             << "+cste2_55*(this->d" << v.name << "_K6))";
          if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
            os << ")/(" << get_enf(v) << ")";
          }
          os << ");\n";
        } else {
          if (this->bd.useDynamicallyAllocatedVector(v.arraySize)) {
            os << "for(unsigned short idx=0;idx!=" << v.arraySize
               << ";++idx){\n";
            os << "rk_update_error(";
            if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
              os << "(";
            }
            os << "tfel::math::abs("
               << "cste1_360*(this->d" << v.name << "_K1[idx])"
               << "-cste128_4275*(this->d" << v.name << "_K3[idx])"
               << "-cste2197_75240*(this->d" << v.name << "_K4[idx])"
               << "+cste1_50*(this->d" << v.name << "_K5[idx])"
               << "+cste2_55*(this->d" << v.name << "_K6[idx]))";
            if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
              os << ")/(" << get_enf(v) << ")";
            }
            os << ");\n";
            os << "}\n";
          } else {
            for (unsigned short i = 0; i != v.arraySize; ++i) {
              os << "rk_update_error(";
              if (v.hasAttribute(
                      VariableDescription::errorNormalisationFactor)) {
                os << "(";
              }
              os << "tfel::math::abs("
                 << "cste1_360*(this->d" << v.name << "_K1[" << i << "])"
                 << "-cste128_4275*(this->d" << v.name << "_K3[" << i << "])"
                 << "-cste2197_75240*(this->d" << v.name << "_K4[" << i << "])"
                 << "+cste1_50*(this->d" << v.name << "_K5[" << i << "])"
                 << "+cste2_55*(this->d" << v.name << "_K6[" << i << "]))";
              if (v.hasAttribute(
                      VariableDescription::errorNormalisationFactor)) {
                os << ")/(" << get_enf(v) << ")";
              }
              os << ");\n";
            }
          }
        }
      }
    } else {
      this->throwRuntimeError(
          "RungeKuttaCodeGeneratorBase::writeBehaviourRK54Integrator",
          "internal error, unsupported error evaluation.");
    }
    os << "if(!ieee754::isfinite(error)){\n"
       << "throw(tfel::material::DivergenceException());\n"
       << "}\n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : error \" << error << endl;\n";
    }
    os << "// test for convergence\n"
       << "if(error<this->epsilon){\n"
       << "// Final Step\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->" << v.name << " += cste16_135*(this->d" << v.name << "_K1)"
         << "+cste6656_12825*(this->d" << v.name << "_K3)"
         << "+cste28561_56430*(this->d" << v.name << "_K4)"
         << "-cste9_50*(this->d" << v.name << "_K5)"
         << "+cste2_55*(this->d" << v.name << "_K6);\n";
    }
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating stiffness tensor at the end of the time step\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (!this->bd.getMainVariables().empty()) {
      os << "// update the thermodynamic forces\n"
         << "if(!this->computeFinalThermodynamicForces()){\n";
      writeReturnFailure(os, this->bd);
      os << "}\n";
    }
    if (d.hasCode(RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      os << "this->updateAuxiliaryStateVariables(dt_);\n";
    }
    os << "t += dt_;\n"
       << "if(tfel::math::abs(this->dt-t)<dtprec){\n"
       << "converged=true;\n"
       << "}\n"
       << "}\n"
       << "if(!converged){\n"
       << "// time multiplier\n"
       << "real corrector;\n"
       << "if(error < 100*std::numeric_limits<real>::min()){\n"
       << "corrector=real(10);\n"
       << "} else {\n"
       << "corrector = 0.8*pow(this->epsilon/error,0.2);\n"
       << "}\n"
       << "if(corrector<real(0.1f)){\n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : reducing time step by a factor 10\" << endl;\n";
    }
    os << "dt_ *= real(0.1f);\n";
    os << "} else if(corrector>real(10)){\n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : increasing time step by a factor 10\" << endl;\n";
    }
    os << "dt_ *= real(10);\n";
    os << "} else {\n";
    if (getDebugMode()) {
      os << "if(corrector<1){\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : reducing time step by a factor \"   << corrector "
            "<< endl;\n";
      os << "} else {\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : increasing time step by a factor \" << corrector "
            "<< endl;\n";
      os << "}\n";
    }
    os << "dt_ *= corrector;\n"
       << "}\n"
       << "if(dt_<dtprec){\n"
       << "throw(tfel::material::DivergenceException());\n"
       << "}\n"
       << "if((tfel::math::abs(this->dt-t-dt_)<2*dtprec)||(t+dt_>this->dt)){\n"
       << "dt_=this->dt-t;\n"
       << "}\n"
       << "}\n"
       << "} else {\n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failure detected, reducing time step by a factor "
            "10\" << endl;";
    }
    os << "// failed is true\n"
       << "dt_ *= real(0.1f);\n"
       << "if(dt_<dtprec){\n"
       << "throw(tfel::material::DivergenceException());\n"
       << "}\n"
       << "}\n"
       << "}\n";
  }  // end of writeBehaviourRK54Integrator

  // Older versions of TFEL assumed that a variable called young was always
  // defined to define the factor used to normalize the stress error in
  // the rkCastem algorithm.
  //
  // For backward compatibility, the findErrorNormalizationFactor looks if such
  // a variable exists and makes some consistency checks.
  //
  // Otherwise, one checks if the stiffness matrix is computed.
  //
  // see Issue 183 for details (https://github.com/thelfer/tfel/issues/183)
  static std::string findStressErrorNormalizationFactor(
      const BehaviourDescription& bd) {
    using tfel::glossary::Glossary;
    //
    auto checkVariableDefinition = [](const auto& v) {
      if (v.getTypeFlag() != SupportedTypes::SCALAR) {
        tfel::raise("findStressErrorNormalizationFactor: the `" + v.name +
                    "` variable is not "
                    "defined as a scalar.");
      }
      if ((getPedanticMode()) && (v.type != "stress")) {
        auto& log = getLogStream();
        log << "findStressErrorNormalizationFactor: "
            << "inconsistent type for variable '" << v.name << "' ('" << v.type
            << "' vs 'stress')\n";
      }
    };
    auto testInGivenVariableCategory = [&bd, &checkVariableDefinition](
                                           const std::string& k,
                                           const std::string& n,
                                           const std::string& e) {
      const auto exists = bd.checkVariableExistence(n, k, false);
      if (exists.first) {
        if (!exists.second) {
          tfel::raise("findStressErrorNormalizationFactor: the `" + n +
                      "` variable is not "
                      "defined for all modelling hypotheses as a '" +
                      k + "'.");
        }
        const auto& mh = bd.getDistinctModellingHypotheses();
        for (const auto& h : mh) {
          const auto& v = bd.getBehaviourData(h).getVariables(k).getVariable(n);
          checkVariableDefinition(v);
          if (!e.empty()) {
            if ((getPedanticMode()) && (v.getExternalName() != e)) {
              auto& log = getLogStream();
              log << "findStressErrorNormalizationFactor: "
                  << "inconsistent external name for variable '" << v.name
                  << "' ('" << v.type << "' vs '" << e << "')\n";
            }
          }
        }
        return true;
      }
      return false;
    };
    auto checkVariable = [bd, testInGivenVariableCategory,
                          checkVariableDefinition](const std::string& n) {
      const auto exists = bd.checkVariableExistence(n);
      if (exists.first) {
        if (!exists.second) {
          tfel::raise(
              "findStressErrorNormalizationFactor: the `" + n +
              "` variable is not defined for all modelling hypotheses.");
        }
        if (testInGivenVariableCategory("MaterialProperty", n,
                                        Glossary::YoungModulus)) {
          return "this->" + n;
        }
        if (testInGivenVariableCategory("Parameter", n,
                                        Glossary::YoungModulus)) {
          return "this->" + n;
        }
        tfel::raise(
            "findStressErrorNormalizationFactor: the `" + n +
            "` variable is not defined as a material property or a parameter");
      }
      // look if young is defined as a local variable
      const auto& mh = bd.getDistinctModellingHypotheses();
      const auto found_as_local_variable =
          bd.isLocalVariableName(*(mh.begin()), n);
      for (const auto& h : mh) {
        const auto is_local_variable = bd.isLocalVariableName(h, n);
        if (found_as_local_variable != is_local_variable) {
          tfel::raise("findStressErrorNormalizationFactor: the `" + n +
                      "` variable is not defined as a local variable in all "
                      "modelling hypotheses");
        }
        if (is_local_variable) {
          const auto& v =
              bd.getBehaviourData(h).getLocalVariables().getVariable(n);
          checkVariableDefinition(v);
        }
      }
      if (found_as_local_variable) {
        return "this->" + n;
      }
      // look if young is defined as a static variable
      const auto found_as_static_variable =
          bd.isStaticVariableName(*(mh.begin()), n);
      for (const auto& h : mh) {
        const auto is_static_variable = bd.isStaticVariableName(h, n);
        if (found_as_static_variable != is_static_variable) {
          tfel::raise("findStressErrorNormalizationFactor: the `" + n +
                      "` variable is not defined as a static variable in all "
                      "modelling hypotheses");
        }
        if (is_static_variable) {
          const auto& v = bd.getBehaviourData(h).getStaticVariables().get(n);
          checkVariableDefinition(v);
        }
      }
      if (found_as_static_variable) {
        return bd.getClassName() + "::" + n;
      }
      tfel::raise("findStressErrorNormalizationFactor: the `" + n +
                  "` variable is not defined as a material property, a static "
                  "variable, a parameter or a local variable");
    };
    //
    if (bd.isNameReserved("stress_error_normalization_factor")) {
      const auto senf_exists =
          bd.checkVariableExistence("stress_error_normalization_factor");
      if (!senf_exists.first) {
        tfel::raise(
            "findStressErrorNormalizationFactor: the "
            "`stress_error_normalization_factor` variable is not defined "
            "as a paramter.");
      }
      if (!senf_exists.second) {
        tfel::raise(
            "findStressErrorNormalizationFactor: the "
            "`stress_error_normalization_factor` variable is not defined "
            "for all modelling hypotheses.");
      }
      if (testInGivenVariableCategory(
              "Parameter", "stress_error_normalization_factor", "")) {
        return "this->stress_error_normalization_factor";
      }
      tfel::raise(
          "findStressErrorNormalizationFactor: the "
          "`stress_error_normalization_factor` variable is not "
          "defined as a material property or a parameter");
    }
    //
    if (bd.isNameReserved("young")) {
      checkVariable("young");
    }
    // look if the stiffness tensor shall be provided by the solver
    if (bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor,
                              false)) {
      return "this->D(0,0)";
    }
    // look if the stiffness tensor is computed
    if (bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                              false)) {
      return "this->D(0,0)";
    }
    // look if a variable with the YoungModulus glossary name is defined
    auto contains = [](const VariableDescriptionContainer& v) {
      return findByExternalName(v, Glossary::YoungModulus) != v.end();
    };
    const auto& mhs = bd.getDistinctModellingHypotheses();
    const auto& bdata = bd.getBehaviourData(*(mhs.begin()));
    if (contains(bdata.getMaterialProperties())) {
      const auto& v = bdata.getMaterialProperties().getVariableByExternalName(
          Glossary::YoungModulus);
      checkVariable(v.name);
      return "this->" + v.name;
    }
    if (contains(bdata.getParameters())) {
      const auto& v = bdata.getParameters().getVariableByExternalName(
          Glossary::YoungModulus);
      checkVariable(v.name);
      return "this->" + v.name;
    }
    if (contains(bdata.getLocalVariables())) {
      const auto& v = bdata.getLocalVariables().getVariableByExternalName(
          Glossary::YoungModulus);
      checkVariable(v.name);
      return "this->" + v.name;
    }
    // nothing worked...
    tfel::raise(
        "findStressErrorNormalizationFactor: "
        "no appropriate error normalization factor found");
  }  // end of findStressErrorNormalizationFactor

  void RungeKuttaCodeGeneratorBase::writeBehaviourRKCastemIntegrator(
      std::ostream& os, const Hypothesis h) const {
    using namespace std;
    const auto& d = this->bd.getBehaviourData(h);
    //! all registred variables used in ComputeDerivatives and
    //! ComputeThermodynamicForces blocks
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
    if (d.hasCode(BehaviourData::ComputeThermodynamicForces)) {
      const auto& uvs2 =
          d.getCodeBlock(BehaviourData::ComputeThermodynamicForces).members;
      uvs.insert(uvs2.begin(), uvs2.end());
    }
    SupportedTypes::TypeSize stateVarsSize;
    for (const auto& v : d.getStateVariables()) {
      stateVarsSize += SupportedTypes::getTypeSize(v.type, v.arraySize);
    }
    writeAlgorithmConstant(os, "1", "2");
    writeAlgorithmConstant(os, "1", "4");
    writeAlgorithmConstant(os, "1", "6");
    os << "time t   = time(0);\n"
       << "time dt_ = this->dt;\n"
       << "StressStensor sigf;\n"
       << "stress errabs;\n"
       << "bool failed = false\n;";
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "failed=!this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing initial stress\" << endl;\n"
         << "}\n";
    }
    os << "if(failed){\n"
       << "throw(tfel::material::DivergenceException());\n"
       << "}\n"
       << "const auto asig = "
       << "tfel::math::power<1, 2>((this->sig) | (this->sig));\n";
    const auto error_normalization_factor =
        findStressErrorNormalizationFactor(this->bd);
    os << "if ((" << error_normalization_factor
       << ")*NumericType(1.e-3)>asig){\n"
       << "  errabs = (" << error_normalization_factor
       << ") * NumericType(1.e-3) * (this->epsilon);\n"
       << "}else{\n"
       << "  errabs = (this->epsilon) * asig;\n"
       << "}\n\n"
       << "time dtprec = 100 * (this->dt) * "
          "std::numeric_limits<NumericType>::epsilon();\n"
       << "bool converged = false;\n";
    if (getDebugMode()) {
      os << "cout << endl << \"" << this->bd.getClassName()
         << "::integrate() : beginning of resolution\" << endl;\n";
    }
    os << "while(!converged){\n"
       << "if(dt_< dtprec){\n"
       << "cout<<\" dt \"<<this->dt<<\" t \"<<t<<\" dt_ \"<<dt_<<endl;\n"
       << "string msg(\"" << this->bd.getClassName() << "\");\n"
       << "msg += \" time step too small \"; \n"
       << "throw(runtime_error(msg)); \n"
       << "} \n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : from \" << t <<  \" to \" << t+dt_ << \" with "
            "time step \" << dt_ << endl;\n";
    }
    os << "// Compute K1's values => y in castem \n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ = this->" << v.name << ";\n";
      }
    }
    writeExternalVariablesCurrentValues(os, this->bd, h, "0");
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K1's stress\" << endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K1's derivatives\" << "
            "endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "// Compute y'1*dt=f(y)*dt in castem\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K1 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n";
    os << "}\n\n";
    os << "if(!failed){\n";
    os << "// Compute K2's values => y1 in castem\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ += cste1_2*(this->d" << v.name
           << "_K1);\n";
      }
    }
    writeExternalVariablesCurrentValues(os, this->bd, h, "cste1_2");
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "// update the thermodynamic forces\n"
         << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K2's stress\" << endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K2's derivatives\" << "
            "endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "// Compute y'2*dt=f(y1)*dt in castem\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K2 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n"
       << "}\n"
       << "}\n\n"
       << "// Compute K3's values => y12 in castem\n"
       << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ = this->" << v.name
           << "+cste1_4*(this->d" << v.name << "_K1 + this->d" << v.name
           << "_K2);\n";
      }
    }
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "// update the thermodynamic forces\n"
         << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K3's stress\" << endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K3's derivatives\" << "
            "endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "// Compute y'3*dt=f(y12)*dt in castem\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K3 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n"
       << "}\n"
       << "}\n\n"
       << "if(!failed){\n"
       << "// Compute K4's values => y13 = y12+y'3*dt/2 in castem\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ += cste1_2*(this->d" << v.name
           << "_K3);\n";
      }
    }
    writeExternalVariablesCurrentValues(os, this->bd, h, "1");
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "// update the thermodynamic forces\n"
         << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K4's stress\" << endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K4's derivatives\" << "
            "endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "// Compute y'4*dt=f(y13)*dt in castem\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K4 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n"
       << "}\n"
       << "}\n\n"
       << "if(!failed){\n"
       << "// Compute K5's values => yf = y12+0.5*(y'3+y'4)*dt/2 in castem\n"
       << "//                     => yf = y+0.5*(y'1+y'2+y'3+y'4)*dt/2 in "
          "castem\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ = this->" << v.name
           << "+cste1_4*(this->d" << v.name << "_K1 + this->d" << v.name
           << "_K2 + this->d" << v.name << "_K3 + this->d" << v.name
           << "_K4);\n";
      }
    }
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "// update the thermodynamic forces\n"
         << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K5's stress\" << endl;\n"
         << "}\n";
    }
    os << "// Saving stresses obtained with yf\n"
       << "sigf=this->sig;\n"
       << "if(!failed){\n"
       << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K5's derivatives\" << "
            "endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "// Compute y'5*dt=f(yf)*dt in castem\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K5 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n"
       << "}\n"
       << "}\n\n"
       << "if(!failed){\n"
       << "// Compute K6's values => y5 = y+1/6*(y'1+4*y'3+y'5)*dt in castem\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ = this->" << v.name
           << "+cste1_6*(this->d" << v.name << "_K1 + NumericType(4)*this->d"
           << v.name << "_K3 + this->d" << v.name << "_K5);\n";
      }
    }
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "// update the thermodynamic forces\n"
         << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing criterium stress\" << "
            "endl;\n"
         << "}\n";
    }
    os << "}\n\n"
       << "if(!failed){\n"
       << "// Computing the error\n"
       << "const auto ra = "
       << "tfel::math::power<1, "
       << "2>(((sigf)-(this->sig))|((sigf)-(this->sig))) / errabs;\n"
       << "const auto sqra = tfel::math::power<1, 2>(ra);\n"
       << "// test for convergence\n"
       << "if ((sqra>" << this->bd.getClassName()
       << "::rkcastem_div)||(!ieee754::isfinite(ra))){\n"
       << "dt_ /= " << this->bd.getClassName() << "::rkcastem_div;\n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : multiplying time step by a factor \" << 1/("
         << this->bd.getClassName() << "::rkcastem_div) << endl;\n";
    }
    os << "} else if (ra> " << this->bd.getClassName() << "::rkcastem_borne){\n"
       << "dt_ /= sqra;\n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : multiplying time step by a factor \" << 1/sqra << "
            "endl;\n";
    }
    os << "}else{\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->" << v.name << " += cste1_4*(this->d" << v.name
         << "_K1 + this->d" << v.name << "_K2 + this->d" << v.name
         << "_K3 + this->d" << v.name << "_K4);\n";
    }
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating stiffness tensor at the end of the time step\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (!this->bd.getMainVariables().empty()) {
      os << "if(!this->computeFinalThermodynamicForces()){\n";
      writeReturnFailure(os, this->bd);
      os << "}\n";
    }
    if (this->bd.hasCode(
            h, RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      os << "this->updateAuxiliaryStateVariables(dt_);\n";
    }
    os << "t += dt_;\n"
       << "if(tfel::math::abs(this->dt-t)<dtprec){\n"
       << "converged=true;\n"
       << "}\n"
       << "if(!converged){\n"
       << "if ((" << this->bd.getClassName() << "::rkcastem_fac)*sqra < 1){\n"
       << "dt_ *= " << this->bd.getClassName() << "::rkcastem_fac;\n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : multiplying time step by a factor \" << "
         << this->bd.getClassName() << "::rkcastem_fac << endl;\n";
    }
    os << "}else if ((sqra< " << this->bd.getClassName() << "::rkcastem_rmin)||"
       << "(sqra>" << this->bd.getClassName() << "::rkcastem_rmax)){\n";
    os << "dt_ /= sqra;\n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : multiplying time step by a factor \" << 1/sqra << "
            "endl;\n";
    }
    os << "}\n"
       << "}\n"
       << "}\n"
       << "} else { \n"
       << "dt_ /=  " << this->bd.getClassName() << "::rkcastem_div;\n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : multiplying time step by a factor \" << 1/("
         << this->bd.getClassName() << "::rkcastem_fac) << endl;\n";
    }
    os << "}\n"
       << "if(dt_<dtprec){\n"
       << "throw(tfel::material::DivergenceException());\n"
       << "}\n"
       << "if(!converged){\n"
       << "if((tfel::math::abs(this->dt-t-dt_)<2*dtprec) || "
          "(t+dt_>this->dt)){\n"
       << "dt_=this->dt-t;\n"
       << "}\n"
       << "}\n"
       << "}\n";
  }  // end of writeBehaviourRKCastemIntegrator

  void RungeKuttaCodeGeneratorBase::writeBehaviourRK42Integrator(
      std::ostream& os, const Hypothesis h) const {
    auto get_enf = [](const VariableDescription& v) {
      return v.getAttribute<std::string>(
          VariableDescription::errorNormalisationFactor);
    };
    const auto& d = this->bd.getBehaviourData(h);
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
    if (d.hasCode(BehaviourData::ComputeThermodynamicForces)) {
      const auto& uvs2 =
          d.getCodeBlock(BehaviourData::ComputeThermodynamicForces).members;
      uvs.insert(uvs2.begin(), uvs2.end());
    }
    ErrorEvaluation eev;
    auto svsize = d.getStateVariables().getTypeSize();
    if (svsize.getValueForDimension(1) >= 20) {
      eev = MAXIMUMVALUEERROREVALUATION;
    } else {
      eev = ERRORSUMMATIONEVALUATION;
    }
    SupportedTypes::TypeSize stateVarsSize;
    for (const auto& v : d.getStateVariables()) {
      stateVarsSize += SupportedTypes::getTypeSize(v.type, v.arraySize);
    }
    writeAlgorithmConstant(os, "1", "2");
    writeAlgorithmConstant(os, "1", "6");
    writeAlgorithmConstant(os, "1", "3");
    os << "time t   = time(0);\n"
       << "time dt_ = this->dt;\n"
       << "time dtprec = 100 * (this->dt) * "
          "std::numeric_limits<NumericType>::epsilon();\n"
       << "auto error = NumericType{};\n"
       << "bool converged = false;\n";
    if (getDebugMode()) {
      os << "cout << endl << \"" << this->bd.getClassName()
         << "::integrate() : beginning of resolution\" << endl;\n";
    }
    os << "while(!converged){\n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : from \" << t <<  \" to \" << t+dt_ << \" with "
            "time step \" << dt_ << endl;\n";
    }
    os << "bool failed = false;\n";
    os << "// Compute K1's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ = this->" << v.name << ";\n";
      }
    }
    writeExternalVariablesCurrentValues(os, this->bd, h, "0");
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K1's stress\" << endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K1's derivatives\" << "
            "endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K1 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n"
       << "}\n\n"
       << "if(!failed){\n"
       << "// Compute K2's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ += cste1_2*(this->d" << v.name
           << "_K1);\n";
      }
    }
    writeExternalVariablesCurrentValues(os, this->bd, h, "cste1_2");
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "// update the thermodynamic forces\n"
         << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K2's stress\" << endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K2's derivatives\" << "
            "endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K2 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n"
       << "}\n"
       << "}\n\n"
       << "// Compute K3's values\n"
       << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ = this->" << v.name
           << "+cste1_2*(this->d" << v.name << "_K2);\n";
      }
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "// update the thermodynamic forces\n"
         << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K3's stress\" << endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K3's derivatives\" << "
            "endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K3 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n"
       << "}\n"
       << "}\n\n"
       << "if(!failed){\n"
       << "// Compute K4's values\n";
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ = this->" << v.name << "+this->d"
           << v.name << "_K3;\n";
      }
    }
    writeExternalVariablesCurrentValues(os, this->bd, h, "1");
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "// update the thermodynamic forces\n"
         << "failed = !this->computeThermodynamicForces();\n";
    }
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K4's stress\" << endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n"
       << "failed = !this->computeDerivative();\n";
    if (getDebugMode()) {
      os << "if(failed){\n"
         << "cout << \"" << this->bd.getClassName()
         << "::integrate() : failed while computing K4's derivatives\" << "
            "endl;\n"
         << "}\n";
    }
    os << "if(!failed){\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K4 = (dt_)*(this->d" << v.name << ");\n";
    }
    os << "}\n"
       << "}\n"
       << "}\n\n"
       << "if(!failed){\n"
       << "// Computing the error\n";
    if (eev == ERRORSUMMATIONEVALUATION) {
      bool first = true;
      for (const auto& v : d.getStateVariables()) {
        if (v.arraySize == 1u) {
          if (first) {
            os << "error  = ";
            first = false;
          } else {
            os << "error += ";
          }
          os << "tfel::math::base_type_cast(tfel::math::abs(";
          if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
            os << "(";
          }
          os << "cste1_6*(this->d" << v.name << "_K1+"
             << "this->d" << v.name << "_K4-"
             << "this->d" << v.name << "_K2-"
             << "this->d" << v.name << "_K3)))";
          if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
            os << ")/(" << get_enf(v) << ")";
          }
          os << ";\n";
        } else {
          if (this->bd.useDynamicallyAllocatedVector(v.arraySize)) {
            if (first) {
              os << "error  = NumericType(0);\n";
              first = false;
            }
            os << "for(unsigned short idx=0;idx!=" << v.arraySize
               << ";++idx){\n"
               << "error += tfel::math::base_type_cast(tfel::math::abs(";
            if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
              os << "(";
            }
            os << "cste1_6*(this->d" << v.name << "_K1[idx]+"
               << "this->d" << v.name << "_K4[idx]-"
               << "this->d" << v.name << "_K2[idx]-"
               << "this->d" << v.name << "_K3[idx])))";
            if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
              os << ")/(" << get_enf(v) << ")";
            }
            os << ";\n";
            os << "}\n";
          } else {
            for (unsigned short i = 0; i != v.arraySize; ++i) {
              if (first) {
                os << "error  = ";
                first = false;
              } else {
                os << "error += ";
              }
              os << "tfel::math::base_type_cast(tfel::math::abs(";
              if (v.hasAttribute(
                      VariableDescription::errorNormalisationFactor)) {
                os << "(";
              }
              os << "cste1_6*(this->d" << v.name << "_K1[" << i << "]+"
                 << "this->d" << v.name << "_K4[" << i << "]-"
                 << "this->d" << v.name << "_K2[" << i << "]-"
                 << "this->d" << v.name << "_K3[" << i << "])))";
              if (v.hasAttribute(
                      VariableDescription::errorNormalisationFactor)) {
                os << ")/(" << get_enf(v) << ")";
              }
              os << ";\n";
            }
          }
        }
      }
      os << "error/=" << stateVarsSize << ";\n";
    } else if (eev == MAXIMUMVALUEERROREVALUATION) {
      os << "error  = NumericType(0);\n"
         << "auto rk_update_error = [&error](const auto rk_error){\n"
         << "if(!ieee754::isfinite(error)){return;}\n"
         << "if(!ieee754::isfinite(rk_error)){\n"
         << "error = rk_error;\n"
         << "return;\n"
         << "}\n"
         << "error = std::max(error, tfel::math::base_type_cast(rk_error));\n"
         << "};\n";
      for (const auto& v : d.getStateVariables()) {
        if (v.arraySize == 1u) {
          os << "rk_update_error(tfel::math::abs(";
          if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
            os << "(";
          }
          os << "cste1_6*(this->d" << v.name << "_K1+"
             << "this->d" << v.name << "_K4-"
             << "this->d" << v.name << "_K2-"
             << "this->d" << v.name << "_K3))";
          if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
            os << ")/(" << get_enf(v) << ")";
          }
          os << ");\n";
        } else {
          if (this->bd.useDynamicallyAllocatedVector(v.arraySize)) {
            os << "for(unsigned short idx=0;idx!=" << v.arraySize
               << ";++idx){\n";
            os << "rk_update_error(tfel::math::abs(";
            if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
              os << "(";
            }
            os << "cste1_6*(this->d" << v.name << "_K1[idx]+"
               << "this->d" << v.name << "_K4[idx]-"
               << "this->d" << v.name << "_K2[idx]-"
               << "this->d" << v.name << "_K3[idx]))";
            if (v.hasAttribute(VariableDescription::errorNormalisationFactor)) {
              os << ")/(" << get_enf(v) << ")";
            }
            os << ");\n";
            os << "}\n";
          } else {
            for (unsigned short i = 0; i != v.arraySize; ++i) {
              os << "rk_update_error(tfel::math::abs(";
              if (v.hasAttribute(
                      VariableDescription::errorNormalisationFactor)) {
                os << "(";
              }
              os << "cste1_6*(this->d" << v.name << "_K1[" << i << "]+"
                 << "this->d" << v.name << "_K4[" << i << "]-"
                 << "this->d" << v.name << "_K2[" << i << "]-"
                 << "this->d" << v.name << "_K3[" << i << "]))";
              if (v.hasAttribute(
                      VariableDescription::errorNormalisationFactor)) {
                os << ")/(" << get_enf(v) << ")";
              }
              os << ");\n";
            }
          }
        }
      }
    } else {
      this->throwRuntimeError(
          "RungeKuttaCodeGeneratorBase::writeBehaviourRK42Integrator",
          "internal error, unsupported error evaluation");
    }
    os << "if(!ieee754::isfinite(error)){\n"
       << "throw(tfel::material::DivergenceException());\n"
       << "}\n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : error \" << error << endl;\n";
    }
    os << "// test for convergence\n"
       << "if(error<this->epsilon){\n"
       << "// Final Step\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->" << v.name << " += cste1_6*(this->d" << v.name
         << "_K1 + this->d" << v.name << "_K4)+"
         << "    cste1_3*(this->d" << v.name << "_K3 + this->d" << v.name
         << "_K2);\n";
    }
    if (!this->bd.getMainVariables().empty()) {
      os << "if(!this->computeFinalThermodynamicForces()){\n";
      writeReturnFailure(os, this->bd);
      os << "}\n";
    }
    if (this->bd.hasCode(
            h, RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      os << "this->updateAuxiliaryStateVariables(dt_);\n";
    }
    os << "t += dt_;\n"
       << "if(tfel::math::abs(this->dt-t) < dtprec){\n"
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
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : reducing time step by a factor 10\" << endl;\n";
    }
    os << "} else if(corrector>real(10)){\n";
    os << "dt_ *= real(10);\n";
    if (getDebugMode()) {
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : multiplying time step by a factor 10\" << endl;\n";
    }
    os << "} else {\n";
    os << "dt_ *= corrector;\n";
    if (getDebugMode()) {
      os << "if(corrector<1){\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : reducing time step by a factor \"   << corrector "
            "<< endl;\n";
      os << "} else {\n";
      os << "cout << \"" << this->bd.getClassName()
         << "::integrate() : increasing time step by a factor \" << corrector "
            "<< endl;\n";
      os << "}\n";
    }
    os << "}\n"
       << "if(dt_<dtprec){\n"
       << "throw(tfel::material::DivergenceException());\n"
       << "}\n"
       << "if((tfel::math::abs(this->dt-t-dt_)<2*dtprec)||(t+dt_>this->dt)){\n"
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
  }  // end of writeBehaviourRK42Integrator

  void RungeKuttaCodeGeneratorBase::writeBehaviourRK4Integrator(
      std::ostream& os, const Hypothesis h) const {
    const auto btype = this->bd.getBehaviourTypeFlag();
    const auto& d = this->bd.getBehaviourData(h);
    auto uvs = d.getCodeBlock(BehaviourData::ComputeDerivative).members;
    if (d.hasCode(BehaviourData::ComputeThermodynamicForces)) {
      const auto& uvs2 =
          d.getCodeBlock(BehaviourData::ComputeThermodynamicForces).members;
      uvs.insert(uvs2.begin(), uvs2.end());
    }
    writeAlgorithmConstant(os, "1", "2");
    os << "// Compute K1's values\n";
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "if(!this->computeThermodynamicForces()){\n";
      writeReturnFailure(os, this->bd);
      os << "}\n";
    }
    os << "if(!this->computeDerivative()){\n";
    writeReturnFailure(os, this->bd);
    os << "}\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K1 = (this->dt)*(this->d" << v.name
         << ");\n";
    }
    writeExternalVariablesCurrentValues2(os, this->bd, h, "cste1_2");
    for (const auto& iv : d.getStateVariables()) {
      if (uvs.find(iv.name) != uvs.end()) {
        os << "this->" << iv.name << "_ += cste1_2*(this->d" << iv.name
           << "_K1);\n";
      }
    }
    if ((this->bd.getAttribute<bool>(
            BehaviourDescription::computesStiffnessTensor, false)) &&
        (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep())) {
      auto m =
          modifyVariableForStiffnessTensorComputation(this->bd.getClassName());
      os << "// updating the stiffness tensor\n";
      this->writeStiffnessTensorComputation(os, "this->D", m);
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "// update the thermodynamic forces\n"
         << "if(!this->computeThermodynamicForces()){\n";
      writeReturnFailure(os, this->bd);
      os << "}\n";
    }
    os << "// Compute K2's values\n"
       << "if(!this->computeDerivative()){\n";
    writeReturnFailure(os, this->bd);
    os << "}\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K2 = (this->dt)*(this->d" << v.name
         << ");\n";
    }
    for (const auto& iv : d.getStateVariables()) {
      if (uvs.find(iv.name) != uvs.end()) {
        os << "this->" << iv.name << "_ = "
           << "this->" << iv.name << "+ cste1_2*(this->d" << iv.name
           << "_K2);\n";
      }
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "// update the thermodynamic forces\n"
         << "if(!this->computeThermodynamicForces()){\n";
      writeReturnFailure(os, this->bd);
      os << "}\n";
    }
    os << "// Compute K3's values\n"
       << "if(!this->computeDerivative()){\n";
    writeReturnFailure(os, this->bd);
    os << "}\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K3 = (this->dt)*(this->d" << v.name
         << ");\n";
    }
    for (const auto& v : d.getStateVariables()) {
      if (uvs.find(v.name) != uvs.end()) {
        os << "this->" << v.name << "_ = "
           << "this->" << v.name << "+ (this->d" << v.name << "_K3);\n";
      }
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "// update the thermodynamic forces\n"
         << "if(!this->computeThermodynamicForces()){\n";
      writeReturnFailure(os, this->bd);
      os << "}\n";
    }
    os << "// Compute K4's values\n"
       << "if(!this->computeDerivative()){\n";
    writeReturnFailure(os, this->bd);
    os << "}\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->d" << v.name << "_K4 = (this->dt)*(this->d" << v.name
         << ");\n";
    }
    os << "// Final Step\n";
    for (const auto& v : d.getStateVariables()) {
      os << "this->" << v.name << " += "
         << "(this->d" << v.name << "_K1+this->d" << v.name << "_K4)/6+\n";
      os << "(this->d" << v.name << "_K2+this->d" << v.name << "_K3)/3;\n";
    }
    if (!this->bd.getMainVariables().empty()) {
      os << "// update the thermodynamic forces\n"
         << "if(!this->computeFinalThermodynamicForces()){\n";
      writeReturnFailure(os, this->bd);
      os << "}\n";
    }
    if (this->bd.hasCode(
            h, RungeKuttaDSLBase::RungeKuttaUpdateAuxiliaryStateVariables)) {
      os << "this->updateAuxiliaryStateVariables(this->dt);\n";
    }
  }  // end of writeBehaviourRK4Integrator

  void RungeKuttaCodeGeneratorBase::writeBehaviourIntegrator(
      std::ostream& os, const Hypothesis h) const {
    const auto btype = this->bd.getBehaviourTypeFlag();
    const auto& algorithm =
        this->bd.getAttribute<std::string>(BehaviourData::algorithm);
    const auto& d = this->bd.getBehaviourData(h);
    this->checkBehaviourFile(os);
    os << "/*!\n"
       << "* \\brief Integrate behaviour law over the time step\n"
       << "*/\n"
       << "[[nodiscard]] IntegrationResult\n";
    if (this->bd.hasAttribute(h, BehaviourData::hasConsistentTangentOperator)) {
      os << "integrate(const SMFlag smflag,const SMType smt) override final{\n";
    } else {
      if ((this->bd.getBehaviourType() ==
           BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
          (this->bd.getBehaviourType() ==
           BehaviourDescription::COHESIVEZONEMODEL)) {
        os << "integrate(const SMFlag smflag, const SMType smt) "
           << "override final{\n";
      } else {
        os << "integrate(const SMFlag, const SMType smt) override final{\n";
      }
    }
    os << "using namespace std;\n"
       << "using namespace tfel::math;\n";
    if ((this->bd.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
        (this->bd.getBehaviourType() ==
         BehaviourDescription::COHESIVEZONEMODEL)) {
      if (this->bd.useQt()) {
        os << "if(smflag!=MechanicalBehaviour<" << btype
           << ",hypothesis, NumericType, "
              "use_qt>::STANDARDTANGENTOPERATOR){\n";
        writeReturnFailure(os, this->bd);
        os << "}\n";
      } else {
        os << "if(smflag!=MechanicalBehaviour<" << btype
           << ",hypothesis, NumericType, false>::STANDARDTANGENTOPERATOR){\n";
        writeReturnFailure(os, this->bd);
        os << "}\n";
      }
    }
    if (this->bd.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfilingBegin(os, this->bd.getClassName(),
                                             BehaviourData::Integrator);
    }
    if (algorithm == "Euler") {
      this->writeBehaviourEulerIntegrator(os, h);
    } else if (algorithm == "RungeKutta2") {
      this->writeBehaviourRK2Integrator(os, h);
    } else if (algorithm == "RungeKutta4/2") {
      this->writeBehaviourRK42Integrator(os, h);
    } else if (algorithm == "RungeKutta5/4") {
      this->writeBehaviourRK54Integrator(os, h);
    } else if (algorithm == "RungeKuttaCastem") {
      this->writeBehaviourRKCastemIntegrator(os, h);
    } else if (algorithm == "RungeKutta4") {
      this->writeBehaviourRK4Integrator(os, h);
    } else {
      this->throwRuntimeError(
          "RungeKuttaCodeGeneratorBase::writeBehaviourIntegrator",
          "internal error\n'" + algorithm +
              "' is not a known algorithm. "
              "This shall not happen at this stage."
              " Please contact MFront developper to help them debug this.");
    }
    os << "this->updateAuxiliaryStateVariables();\n";
    if (!areRuntimeChecksDisabled(this->bd)) {
      for (const auto& v : d.getPersistentVariables()) {
        this->writePhysicalBoundsChecks(os, v, false);
      }
      for (const auto& v : d.getPersistentVariables()) {
        this->writeBoundsChecks(os, v, false);
      }
    }
    if (this->bd.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfilingEnd(os);
    }
    os << "if(smt!=NOSTIFFNESSREQUESTED){\n";
    if (this->bd.hasAttribute(h, BehaviourData::hasConsistentTangentOperator)) {
      if (this->bd.getBehaviourType() ==
          BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        os << "if(!this->computeConsistentTangentOperator(smflag,smt)){\n";
      } else {
        os << "if(!this->computeConsistentTangentOperator(smt)){\n";
      }
      writeReturnFailure(os, this->bd);
      os << "}\n";
    } else {
      os << "string msg(\"" << this->bd.getClassName() << "::integrate : \");\n"
         << "msg +=\"unimplemented feature\";\n"
         << "throw(runtime_error(msg));\n";
    }
    os << "}\n";
    writeReturnSuccess(os, this->bd);
    os << "} // end of " << this->bd.getClassName() << "::integrate\n\n";
  }  // end of writeBehaviourIntegrator

  RungeKuttaCodeGeneratorBase::~RungeKuttaCodeGeneratorBase() = default;

}  // end of namespace mfront
