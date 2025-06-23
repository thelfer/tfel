/*!
 * \file   mfront/src/ImplicitCodeGeneratorBase.cxx
 * \brief
 * \author Thomas Heler
 * \date   04/08/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include <sstream>
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/PedanticMode.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/PerformanceProfiling.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/NonLinearSystemSolverBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/ImplicitCodeGeneratorBase.hxx"

namespace mfront {

  static void writeVariablesOffsets(
      std::ostream& os, const VariableDescriptionContainer& variables) {
    auto n = SupportedTypes::TypeSize();
    for (const auto& v : variables) {
      os << "constexpr auto " << v.name << "_offset = " << n << ";\n";
      n += SupportedTypes::getTypeSize(v.type, v.arraySize);
    }
  }  // end of writeVariablesOffsets

  static void writeIgnoreVariablesOffsets(
      std::ostream& os, const VariableDescriptionContainer& variables) {
    for (const auto& v : variables) {
      os << "static_cast<void>(" << v.name << "_offset);\n";
    }
  }  // end of writeIgnoreVariablesOffsets

  static void declareViewsFromArrayOfVariables(
      std::ostream& os,
      const VariableDescriptionContainer& variables,
      const std::function<std::string(const std::string&)>&
          variable_name_generator,
      const std::string& array_name,
      const bool use_qt) {
    auto n = SupportedTypes::TypeSize();
    for (const auto& v : variables) {
      const auto& vname = variable_name_generator(v.name);
      if (v.arraySize == 1u) {
        if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::SCALAR) {
          if (use_qt) {
            os << "tfel::math::scalar_view<" << v.type << "> " << vname << "("
               << array_name << "(" << n << "));\n";
          } else {
            os << "real& " << vname << "(" << array_name << "(" << n << "));\n";
          }
        } else {
          os << "auto " << vname << " = tfel::math::map<" << v.type << ", " << n
             << ">(" << array_name << ");\n";
        }
      } else {
        os << "auto " << vname << " = tfel::math::map<" << v.arraySize << ", "
           << v.type << ", " << n << ">(" << array_name << ");\n";
      }
      n += SupportedTypes::getTypeSize(v.type, v.arraySize);
    }
  }  // end of declareViewsFromArrayOfVariables

  ImplicitCodeGeneratorBase::ImplicitCodeGeneratorBase(
      const FileDescription& f,
      const BehaviourDescription& d,
      const BehaviourInterfaceMap& bim,
      const NonLinearSystemSolver& s,
      const std::set<std::string>& s1,
      const std::set<std::string>& s2)
      : BehaviourCodeGeneratorBase(f, d, bim),
        solver(s),
        jacobianPartsUsedInIntegrator(s1),
        integrationVariablesIncrementsUsedInPredictor(s2) {
  }  // end of ImplicitCodeGeneratorBase

  void ImplicitCodeGeneratorBase::
      writeBehaviourParserSpecificInheritanceRelationship(
          std::ostream& os, const Hypothesis h) const {
    os << ",\npublic "
       << this->solver.getExternalAlgorithmClassName(this->bd, h);
  }  // end of writeBehaviourParserSpecificInheritanceRelationship

  void ImplicitCodeGeneratorBase::writeBehaviourFriends(
      std::ostream& os, const Hypothesis h) const {
    BehaviourCodeGeneratorBase::writeBehaviourFriends(os, h);
    const auto hn = [&h]() -> std::string {
      if (h == tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        return "hypothesis";
      }
      return "ModellingHypothesis::" +
             tfel::material::ModellingHypothesis::toUpperCaseString(h);
    }();
    const auto n =
        mfront::getTypeSize(
            this->bd.getBehaviourData(h).getIntegrationVariables())
            .asString({"ModellingHypothesisToSpaceDimension<" + hn + ">::value",
                       "ModellingHypothesisToStensorSize<" + hn + ">::value",
                       "ModellingHypothesisToTensorSize<" + hn + ">::value"});
    os << "friend struct tfel::math::TinyNonLinearSolverBase<" << n
       << ", NumericType, " << this->bd.getClassName() << ">;\n";
  }  // end of writeBehaviourFriends

  void ImplicitCodeGeneratorBase::writeDerivativeView(
      std::ostream& os, const DerivativeViewDescription& d) const {
    const auto v1 = d.first_variable;
    const auto v2 = d.second_variable;
    const auto dr = d.derivative_row_position;
    const auto dc = d.derivative_column_position;
    if ((v1.arraySize == 1u) && (v2.arraySize == 1u)) {
      os << "auto&& " << d.derivative_name << " = tfel::math::map_derivative<"
         << dr << ", " << dc << ", " << v1.type << ", " << v2.type << ">("
         << d.matrix_name << ");\n";
    } else if ((v1.arraySize != 1u) && (v2.arraySize == 1u)) {
      os << "auto " << d.derivative_name << " = [&" << d.matrix_name
         << "](const ushort mfront_idx){\n"
         << "return tfel::math::map_derivative<" << v1.type << ", " << v2.type
         << ">(" << d.matrix_name << ", " << dr << " + mfront_idx * "
         << SupportedTypes::getTypeSize(v1.type, 1u) << ", " << dc << ");\n"
         << "};\n";
    } else if ((v1.arraySize == 1u) && (v2.arraySize != 1u)) {
      os << "auto " << d.derivative_name << " = [&" << d.matrix_name
         << "](const ushort mfront_idx){\n"
         << "return tfel::math::map_derivative<" << v1.type << ", " << v2.type
         << ">(" << d.matrix_name << ", " << dr << ", " << dc
         << " + mfront_idx * " << SupportedTypes::getTypeSize(v2.type, 1u)
         << ");\n"
         << "};\n";
    } else {
      os << "auto " << d.derivative_name << " = [&" << d.matrix_name
         << "](const ushort mfront_idx, const ushort "
            "mfront_idx2){\n"
         << "return tfel::math::map_derivative<" << v1.type << ", " << v2.type
         << ">(" << d.matrix_name << ", " << dr << " + mfront_idx * "
         << SupportedTypes::getTypeSize(v1.type, 1u) << ", " << dc
         << " + mfront_idx2 * " << SupportedTypes::getTypeSize(v2.type, 1u)
         << ");\n"
         << "};\n";
    }
  }  // end of writeDerivativeView

  void ImplicitCodeGeneratorBase::writeBehaviourLocalVariablesInitialisation(
      std::ostream& os, const Hypothesis h) const {
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    Modifier mts = [this](const MaterialPropertyInput& i) -> std::string {
      if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
          (i.category ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+(this->theta)*(this->d" + i.name + ')';
      } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.category == MaterialPropertyInput::STATEVARIABLE) ||
                 (i.category == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
        return this->bd.getClassName() + "::" + i.name;
      } else {
        this->throwRuntimeError(
            "ImplicitCodeGeneratorBase::"
            "writeBehaviourLocalVariablesInitialisation",
            "unsupported input type for variable '" + i.name + "'");
      }
    };
    Modifier ets = [this](const MaterialPropertyInput& i) -> std::string {
      if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
          (i.category ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+this->d" + i.name;
      } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.category == MaterialPropertyInput::STATEVARIABLE) ||
                 (i.category == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
        return this->bd.getClassName() + "::" + i.name;
      } else {
        this->throwRuntimeError(
            "ImplicitCodeGeneratorBase::"
            "writeBehaviourLocalVariablesInitialisation",
            "unsupported input type for variable '" + i.name + "'");
      }
    };
    if (this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                              false)) {
      os << "// updating the stiffness tensor at the middle of the time "
            "step\n";
      this->writeStiffnessTensorComputation(os, "this->D", mts);
      if (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep()) {
        os << "// stiffness tensor at the end of the time step\n";
        this->writeStiffnessTensorComputation(os, "this->D_tdt", ets);
      }
    }
    for (const auto& ht : this->bd.getHillTensors()) {
      if ((this->bd.getBehaviourType() !=
           BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (this->bd.getBehaviourType() !=
           BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
        this->throwRuntimeError(
            "ImplicitCodeGeneratorBase::"
            "writeBehaviourLocalVariablesInitialisation",
            "Hill tensors shall only be defined for finite strain "
            "or small strain behaviours");
      }
      this->writeHillTensorComputation(os, "this->" + ht.name, ht, mts);
      if (!this->bd.areMaterialPropertiesConstantDuringTheTimeStep(ht.c)) {
        this->writeHillTensorComputation(os, "this->" + ht.name + "_tdt", ht,
                                         ets);
      }
    }
    if ((!this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                                false)) &&
        (this->bd.getElasticSymmetryType() == ISOTROPIC) &&
        (this->bd.areElasticMaterialPropertiesDefined())) {
      const auto& emps = this->bd.getElasticMaterialProperties();
      if (emps.size() != 2u) {
        this->throwRuntimeError(
            "ImplicitCodeGeneratorBase::"
            "writeBehaviourLocalVariablesInitialisation",
            "invalid number of material properties");
      }
      if (!emps[0].is<BehaviourDescription::ConstantMaterialProperty>()) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[0], mts);
      }
      if (!emps[1].is<BehaviourDescription::ConstantMaterialProperty>()) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[1], mts);
      }
      if (!emps[0].is<BehaviourDescription::ConstantMaterialProperty>()) {
        os << "this->young = stress(";
        this->writeMaterialPropertyEvaluation(os, emps[0], mts);
        os << ");\n";
      }
      if (!emps[1].is<BehaviourDescription::ConstantMaterialProperty>()) {
        os << "this->nu = ";
        this->writeMaterialPropertyEvaluation(os, emps[1], mts);
        os << ";\n";
      }
      os << "this->lambda = computeLambda(young,nu);\n";
      os << "this->mu = computeMu(young,nu);\n";
      if (!this->bd.isMaterialPropertyConstantDuringTheTimeStep(emps[0])) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[0], ets);
        os << "this->young_tdt = stress(";
        this->writeMaterialPropertyEvaluation(os, emps[0], ets);
        os << ");\n";
      } else {
        os << "this->young_tdt  = this->young;\n";
      }
      if (!this->bd.isMaterialPropertyConstantDuringTheTimeStep(emps[1])) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[1], ets);
        os << "this->nu_tdt = ";
        this->writeMaterialPropertyEvaluation(os, emps[1], ets);
        os << ";\n";
      } else {
        os << "this->nu_tdt     = this->nu;\n";
      }
      if (!this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep()) {
        os << "this->lambda_tdt = computeLambda(young_tdt,nu_tdt);\n";
        os << "this->mu_tdt     = computeMu(young_tdt,nu_tdt);\n";
      } else {
        os << "this->lambda_tdt = this->lambda;\n";
        os << "this->mu_tdt     = this->mu;\n";
      }
    }
    BehaviourCodeGeneratorBase::writeBehaviourLocalVariablesInitialisation(os,
                                                                           h);
  }  // end of writeBehaviourLocalVariablesInitialisation

  void ImplicitCodeGeneratorBase::writeBehaviourParserSpecificIncludes(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    os << "#include\"TFEL/Math/Array/ViewsArray.hxx\"\n"
       << "#include\"TFEL/Math/tvector.hxx\"\n"
       << "#include\"TFEL/Math/Vector/tvectorIO.hxx\"\n"
       << "#include\"TFEL/Math/tmatrix.hxx\"\n"
       << "#include\"TFEL/Math/Matrix/tmatrixIO.hxx\"\n"
       << "#include\"TFEL/Math/st2tost2.hxx\"\n"
       << "#include\"TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx\"\n";
    for (const auto& h : this->solver.getSpecificHeaders()) {
      os << "#include\"" << h << "\"\n";
    }
  }  // end of writeBehaviourParserSpecificIncludes()

  void ImplicitCodeGeneratorBase::writeBehaviourParserSpecificTypedefs(
      std::ostream& os) const {
    this->checkBehaviourFile(os);
    BehaviourCodeGeneratorBase::writeBehaviourParserSpecificTypedefs(os);
  }  // end of writeBehaviourParserSpecificTypedefs()

  void ImplicitCodeGeneratorBase::writeBehaviourParserSpecificMembers(
      std::ostream& os, const Hypothesis h) const {
    using Modifier = std::function<std::string(const MaterialPropertyInput&)>;
    Modifier mts = [this](const MaterialPropertyInput& i) -> std::string {
      if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
          (i.category ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.category == MaterialPropertyInput::STATEVARIABLE) ||
          (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + " + (this->theta) * (this->d" + i.name + ')';
      } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.category == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
        return this->bd.getClassName() + "::" + i.name;
      } else {
        this->throwRuntimeError(
            "ImplicitCodeGeneratorBase::"
            "writeBehaviourLocalVariablesInitialisation",
            "unsupported input type for variable '" + i.name + "'");
      }
    };
    Modifier ets = [this](const MaterialPropertyInput& i) -> std::string {
      if ((i.category == MaterialPropertyInput::TEMPERATURE) ||
          (i.category ==
           MaterialPropertyInput::AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL) ||
          (i.category == MaterialPropertyInput::STATEVARIABLE) ||
          (i.category == MaterialPropertyInput::EXTERNALSTATEVARIABLE)) {
        return "this->" + i.name + "+this->d" + i.name;
      } else if ((i.category == MaterialPropertyInput::MATERIALPROPERTY) ||
                 (i.category == MaterialPropertyInput::PARAMETER)) {
        return "this->" + i.name;
      } else if (i.category == MaterialPropertyInput::STATICVARIABLE) {
        return this->bd.getClassName() + "::" + i.name;
      } else {
        this->throwRuntimeError(
            "ImplicitCodeGeneratorBase::"
            "writeBehaviourLocalVariablesInitialisation",
            "unsupported input type for variable '" + i.name + "'");
      }
    };
    //
    BehaviourCodeGeneratorBase::writeBehaviourParserSpecificMembers(os, h);
    // updating material properties
    os << "// updating material properties, in mandatory\n"
       << "TFEL_HOST_DEVICE void "
          "updateMaterialPropertiesDependantOnStateVariables(){\n"
       << "using namespace std;\n"
       << "using namespace tfel::math;\n"
       << "using std::vector;\n";
    writeMaterialLaws(os, this->bd.getMaterialLaws());
    if (this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                              false)) {
      if (this->bd.areElasticMaterialPropertiesDependantOnStateVariables()) {
        os << "// updating the stiffness tensor at the middle of the time "
              "step\n";
        this->writeStiffnessTensorComputation(os, "this->D", mts);
        os << "// stiffness tensor at the end of the time step\n";
        this->writeStiffnessTensorComputation(os, "this->D_tdt", ets);
      }
    }
    for (const auto& ht : this->bd.getHillTensors()) {
      if ((this->bd.getBehaviourType() !=
           BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (this->bd.getBehaviourType() !=
           BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
        this->throwRuntimeError(
            "ImplicitCodeGeneratorBase::"
            "writeBehaviourLocalVariablesInitialisation",
            "Hill tensors shall only be defined for finite strain "
            "or small strain behaviours");
      }
      if (this->bd.areMaterialPropertiesDependantOnStateVariables(ht.c)) {
        this->writeHillTensorComputation(os, "this->" + ht.name, ht, mts);
        this->writeHillTensorComputation(os, "this->" + ht.name + "_tdt", ht,
                                         ets);
      }
    }
    if ((!this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                                false)) &&
        (this->bd.getElasticSymmetryType() == ISOTROPIC) &&
        (this->bd.areElasticMaterialPropertiesDefined())) {
      const auto& emps = this->bd.getElasticMaterialProperties();
      if (emps.size() != 2u) {
        this->throwRuntimeError(
            "ImplicitCodeGeneratorBase::"
            "writeBehaviourLocalVariablesInitialisation",
            "invalid number of material properties");
      }
      if (this->bd.isMaterialPropertyDependantOnStateVariables(emps[0])) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[0], mts);
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[0], ets);
        os << "this->young = stress(";
        this->writeMaterialPropertyEvaluation(os, emps[0], mts);
        os << ");\n";
        os << "this->young_tdt = stress(";
        this->writeMaterialPropertyEvaluation(os, emps[0], ets);
        os << ");\n";
      }
      if (this->bd.isMaterialPropertyDependantOnStateVariables(emps[1])) {
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[1], mts);
        this->writeMaterialPropertyCheckBoundsEvaluation(os, emps[1], ets);
        os << "this->nu = ";
        this->writeMaterialPropertyEvaluation(os, emps[1], mts);
        os << ";\n";
        os << "this->nu_tdt = ";
        this->writeMaterialPropertyEvaluation(os, emps[1], ets);
        os << ";\n";
      }
      if ((this->bd.isMaterialPropertyDependantOnStateVariables(emps[0])) ||
          (this->bd.isMaterialPropertyDependantOnStateVariables(emps[1]))) {
        os << "this->lambda=computeLambda(young,nu);\n"
           << "this->mu=computeMu(young,nu);\n"
           << "this->lambda_tdt = computeLambda(young_tdt,nu_tdt);\n"
           << "this->mu_tdt     = computeMu(young_tdt,nu_tdt);\n";
      }
    }
    os << "}\n\n";
    //
    const auto& d = this->bd.getBehaviourData(h);
    SupportedTypes::TypeSize n;
    const auto n3 = d.getIntegrationVariables().getTypeSize();
    if ((this->solver.usesJacobian()) &&
        (!this->solver.requiresNumericalJacobian())) {
      for (const auto& v : d.getIntegrationVariables()) {
        SupportedTypes::TypeSize n2;
        for (const auto& v2 : d.getIntegrationVariables()) {
          if ((v.arraySize == 1u) && (v2.arraySize == 1u)) {
            n2 += SupportedTypes::getTypeSize(v2.type, v2.arraySize);
            continue;
          }
          os << "/*!\n"
             << " * \\return the part of the jacobian matrix "
             << "corresponding to the derivative "
             << "of variable " << v.name << " by variable " << v2.name << "\n"
             << " */\n";
          if ((v.arraySize != 1u) && (v2.arraySize == 1u)) {
            os << "tfel::math::derivative_view_from_tiny_matrix<" << n3 << ", "
               << v.type << "," << v2.type << "> "
               << "df" << v.name << "_dd" << v2.name << "(tfel::math::tmatrix<"
               << n3 << "," << n3 << ", NumericType>& tjacobian,"
               << "const ushort mfront_idx){\n"
               << "return tfel::math::map_derivative<" << v.type << ","
               << v2.type << ">(tjacobian, " << n << " + mfront_idx * ("
               << SupportedTypes::getTypeSize(v.type, 1u) << "), " << n2
               << ");\n"
               << "}\n\n";
            os << "tfel::math::derivative_view_from_tiny_matrix<" << n3 << ", "
               << v.type << "," << v2.type << "> "
               << "df" << v.name << "_dd" << v2.name
               << "(const ushort mfront_idx){\n"
               << "return tfel::math::map_derivative<" << v.type << ","
               << v2.type << ">(this->jacobian, " << n << " + mfront_idx * ("
               << SupportedTypes::getTypeSize(v.type, 1u) << "), " << n2
               << ");\n"
               << "}\n\n";
          } else if ((v.arraySize == 1u) && (v2.arraySize != 1u)) {
            os << "tfel::math::derivative_view_from_tiny_matrix<" << n3 << ", "
               << v.type << "," << v2.type << "> "
               << "df" << v.name << "_dd" << v2.name << "(tfel::math::tmatrix<"
               << n3 << "," << n3 << ", NumericType>& tjacobian,"
               << "const ushort mfront_idx){\n"
               << "return tfel::math::map_derivative<" << v.type << ","
               << v2.type << ">(tjacobian, " << n << ", " << n2
               << " + mfront_idx * ("
               << SupportedTypes::getTypeSize(v2.type, 1u) << "));\n"
               << "}\n\n";
            os << "tfel::math::derivative_view_from_tiny_matrix<" << n3 << ", "
               << v.type << "," << v2.type << "> "
               << "df" << v.name << "_dd" << v2.name
               << "(const ushort mfront_idx){\n"
               << "return tfel::math::map_derivative<" << v.type << ","
               << v2.type << ">(this->jacobian, " << n << ", " << n2
               << " + mfront_idx * ("
               << SupportedTypes::getTypeSize(v2.type, 1u) << "));\n"
               << "}\n\n";
          } else {
            os << "tfel::math::derivative_view_from_tiny_matrix<" << n3 << ", "
               << v.type << "," << v2.type << "> "
               << "df" << v.name << "_dd" << v2.name << "(tfel::math::tmatrix<"
               << n3 << "," << n3 << ", NumericType>& tjacobian,"
               << "const ushort mfront_idx, "
               << "const ushort mfront_idx2){\n"
               << "return tfel::math::map_derivative<" << v.type << ","
               << v2.type << ">(tjacobian, " << n << " + mfront_idx * ("
               << SupportedTypes::getTypeSize(v.type, 1u) << "), " << n2
               << " + mfront_idx2 * ("
               << SupportedTypes::getTypeSize(v2.type, 1u) << "));\n"
               << "}\n\n";
            os << "tfel::math::derivative_view_from_tiny_matrix<" << n3 << ", "
               << v.type << "," << v2.type << "> "
               << "df" << v.name << "_dd" << v2.name
               << "(const ushort mfront_idx, "
               << "const ushort mfront_idx2){\n"
               << "return tfel::math::map_derivative<" << v.type << ","
               << v2.type << ">(this->jacobian, " << n << " + mfront_idx * ("
               << SupportedTypes::getTypeSize(v.type, 1u) << "), " << n2
               << " + mfront_idx2 * ("
               << SupportedTypes::getTypeSize(v2.type, 1u) << "));\n"
               << "}\n\n";
          }
          n2 += SupportedTypes::getTypeSize(v2.type, v2.arraySize);
        }
        n += SupportedTypes::getTypeSize(v.type, v.arraySize);
      }
    }
    // size of linear system
    n = n3;
    this->solver.writeSpecificMembers(os, this->bd, h);
    os << "SMType stiffness_matrix_type;\n";
    //
    if (this->solver.usesJacobian()) {
      // compute the numerical part of the jacobian.  This method is
      // used to compute a numerical approximation of the jacobian for
      // the solvers based on it. For solvers providing an analytical
      // jacobian, or an approximation of it, this method is used to
      // compare the jacobain to its numerical approximation
      this->writeComputeNumericalJacobian(os, h);
    }
    if (this->solver.usesJacobian()) {
      // for solver based on the jacobian matrix, the get partial
      // jacobian invert method can be used to compute the tangent
      // operator.
      this->writeComputePartialJacobianInvert(os, h);
    }
    // additional convergence checks
    if (this->bd.hasCode(h, BehaviourData::AdditionalConvergenceChecks)) {
      os << "TFEL_HOST_DEVICE void additionalConvergenceChecks(bool& "
            "converged, "
         << "const NumericType& error){\n"
         << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using std::vector;\n"
         << "const auto smt = this->stiffness_matrix_type;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      os << this->bd.getCode(h, BehaviourData::AdditionalConvergenceChecks)
         << '\n'
         << "static_cast<void>(converged);\n"
         << "static_cast<void>(error);\n"
         << "static_cast<void>(smt);\n"
         << "} // end of additionalConvergenceChecks\n\n";
    }
    // compute stress
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "TFEL_HOST_DEVICE void computeThermodynamicForces(){\n"
         << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using std::vector;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      os << this->bd.getCode(h, BehaviourData::ComputeThermodynamicForces)
         << "\n} // end of " << this->bd.getClassName()
         << "::computeThermodynamicForces\n\n";
    }
    if (this->bd.hasCode(h, BehaviourData::ComputeFinalThermodynamicForces)) {
      os << "TFEL_HOST_DEVICE void computeFinalThermodynamicForces(){\n"
         << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using std::vector;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      os << this->bd.getCode(h, BehaviourData::ComputeFinalThermodynamicForces)
         << "\n} // end of " << this->bd.getClassName()
         << "::computeFinalThermodynamicForces\n\n";
    }
  }  // end of writeBehaviourParserSpecificMembers

  static std::string getDerivativeTypeHolder(const VariableDescription& v1,
                                             const VariableDescription& v2) {
    if (v2.arraySize != 1u) {
      tfel::raise("getDerivativeType: unsupported case");
    }
    const auto dtype =
        "tfel::math::derivative_type<" + v1.type + ", " + v2.type + ">";
    if (v1.arraySize == 1u) {
      return dtype;
    }
    return "tfel::math::tvector<" + std::to_string(v1.arraySize) + ", " +
           dtype + ">";
  }  // end of getVariableDerivativeTypeHolder

  void ImplicitCodeGeneratorBase::writeBehaviourComputeTangentOperatorBody(
      std::ostream& os, const Hypothesis h, const std::string& n) const {
    using Derivative = std::pair<VariableDescription, VariableDescription>;
    const auto& d = this->bd.getBehaviourData(h);
    const auto& isvs = d.getIntegrationVariables();
    const auto nivs = mfront::getTypeSize(isvs);
    const auto& cb = this->bd.getCodeBlock(h, n);
    if (getAttribute(cb, CodeBlock::requires_jacobian_decomposition, false)) {
      os << "TinyPermutation<" << nivs << "> jacobian_permutation;\n"
         << "if(!TinyMatrixSolve<" << nivs << ", NumericType, false>"
         << "::decomp(this->jacobian,jacobian_permutation)){\n"
         << "return false;\n"
         << "}\n";
    }
    if (hasAttribute<std::vector<Derivative>>(
            cb, CodeBlock::used_jacobian_invert_blocks)) {
      const auto& invert_jacobian_blocks =
          getAttribute<std::vector<Derivative>>(
              cb, CodeBlock::used_jacobian_invert_blocks);
      os << "auto jacobian_invert = "
         << "tfel::math::tmatrix<" << nivs << ", " << nivs
         << ", NumericType>{};\n"
         << "std::fill(jacobian_invert.begin(), jacobian_invert.end(), "
            "NumericType(0));\n"
         << "for(typename tfel::math::tmatrix<" << nivs << ", " << nivs
         << ", NumericType>::size_type mfront_idx=0; mfront_idx != " << nivs
         << "; ++mfront_idx){\n"
         << "jacobian_invert(mfront_idx, mfront_idx) = NumericType(1);\n"
         << "}\n"
         << "if(!TinyMatrixSolve<" << nivs
         << ", NumericType, false>::back_substitute("
         << "this->jacobian, jacobian_permutation, jacobian_invert)){\n"
         << "return false;\n"
         << "}\n";
      auto cr = SupportedTypes::TypeSize{};  // current row
      for (const auto& iv1 : isvs) {
        auto cc = SupportedTypes::TypeSize{};  // current column
        for (const auto& iv2 : isvs) {
          if (std::find_if(invert_jacobian_blocks.begin(),
                           invert_jacobian_blocks.end(),
                           [&iv1, &iv2](const Derivative& derivative) {
                             return (derivative.first.name == iv1.name) &&
                                    (derivative.second.name == iv2.name);
                           }) == invert_jacobian_blocks.end()) {
            continue;
          }
          auto derivative_view = DerivativeViewDescription{};
          derivative_view.derivative_name = "iJ_" + iv1.name + "_" + iv2.name;
          derivative_view.matrix_name = "jacobian_invert";
          derivative_view.first_variable = iv1;
          derivative_view.second_variable = iv2;
          derivative_view.matrix_number_of_rows = nivs;
          derivative_view.matrix_number_of_columns = nivs;
          derivative_view.derivative_row_position = cr;
          derivative_view.derivative_column_position = cc;
          this->writeDerivativeView(os, derivative_view);
          // update the column position
          cc += mfront::getTypeSize(iv2);
        }
        // update the row position
        cr += mfront::getTypeSize(iv1);
      }
    }
    // treating getPartialJacobianInvert
    if (getAttribute(cb, CodeBlock::uses_get_partial_jacobian_invert, false)) {
      if (!getAttribute(cb, CodeBlock::requires_jacobian_decomposition,
                        false)) {
        tfel::raise(
            "writeBehaviourComputeTangentOperatorBody: "
            "inconsistent attribute declarations for code block '" +
            n +
            "'. If the attribute `uses_get_partial_jacobian_invert` "
            "is set to true, then the attribute "
            "`requires_jacobian_decomposition` must also be set to true");
      }
      const auto v2 = VariableDescription{"StrainStensor", "\u03B5\u1D57\u1D52",
                                          "eto", 1u, 0u};
      os << "struct TFEL_VISIBILITY_LOCAL GetPartialJacobianInvert{\n"
         << "TFEL_HOST_DEVICE GetPartialJacobianInvert("
         << this->bd.getClassName() << "& mfront_behaviour_argument,\n"
         << "const tfel::math::TinyPermutation<" << nivs
         << ">& mfront_permutation_argument,\n"
         << "bool& mfront_success_argument)\n"
         << ": mfront_get_partial_jacobian_invert_behaviour("
         << "mfront_behaviour_argument),\n"
         << "mfront_get_partial_jacobian_invert_permutation("
         << "mfront_permutation_argument),\n"
         << "mfront_success_reference(mfront_success_argument)"
         << "{}\n";
      for (std::size_t i = 0; i != isvs.size(); ++i) {
        os << "TFEL_HOST_DEVICE void operator()(";
        for (std::size_t i2 = 0; i2 <= i;) {
          const auto& v = isvs[i2];
          os << getDerivativeTypeHolder(v, v2) << "& ";
          os << "partial_jacobian_" << v.name;
          if (++i2 <= i) {
            os << ",\n";
          }
        }
        os << "){\n"
           << "if(!this->mfront_get_partial_jacobian_invert_behaviour."
           << "computePartialJacobianInvert("
           << "this->mfront_get_partial_jacobian_invert_permutation, ";
        for (std::size_t i2 = 0; i2 <= i;) {
          const auto& v = isvs[i2];
          os << "partial_jacobian_" << v.name;
          if (++i2 <= i) {
            os << ",\n";
          }
        }
        os << ")){\n"
           << "this->mfront_success_reference = false;\n"
           << "}\n"
           << "}\n";
      }
      os << "private:\n"
         << this->bd.getClassName() << "& "
         << "mfront_get_partial_jacobian_invert_behaviour;\n"
         << "const tfel::math::TinyPermutation<" << nivs << ">& "
         << "mfront_get_partial_jacobian_invert_permutation;\n"
         << "bool& mfront_success_reference;\n"
         << "}; // end of struct GetPartialJacobianInvert\n"
         << "GetPartialJacobianInvert "
            "getPartialJacobianInvert(*this, jacobian_permutation, "
            "mfront_success);\n";
    }
    const auto attr = CodeBlock::
        used_implicit_equations_derivatives_with_respect_to_gradients_or_external_state_variables;
    if (hasAttribute<std::vector<Derivative>>(cb, attr)) {
      std::vector<
          std::pair<VariableDescription, std::vector<VariableDescription>>>
          implicit_equations_derivatives;
      for (const auto& derivative :
           getAttribute<std::vector<Derivative>>(cb, attr)) {
        const auto p = std::find_if(
            implicit_equations_derivatives.begin(),
            implicit_equations_derivatives.end(),
            [&derivative](
                const std::pair<VariableDescription,
                                std::vector<VariableDescription>>& d2) {
              return derivative.second.name == d2.first.name;
            });
        if (p == implicit_equations_derivatives.end()) {
          implicit_equations_derivatives.push_back(
              {derivative.second,
               std::vector<VariableDescription>{1u, derivative.first}});
        } else {
          p->second.push_back(derivative.first);
        }
      }  //
      for (const auto& derivatives : implicit_equations_derivatives) {
        auto nc = mfront::getTypeSize(derivatives.first);
        const auto rhs_type = [&nivs, &nc] {
          std::ostringstream rhs_os;
          rhs_os << "tfel::math::tmatrix<" << nivs << ", " << nc
                 << ", NumericType>";
          return rhs_os.str();
        }();
        const auto m = "dfzeros_dd" + derivatives.first.name;
        os << rhs_type << m << ";\n"
           << "std::fill(" << m << ".begin(), " << m
           << ".end(), NumericType(0));\n";
        // creating the view for the derivatives
        auto cr = SupportedTypes::TypeSize{};  // current row
        for (const auto& iv : isvs) {
          if (std::find_if(derivatives.second.begin(), derivatives.second.end(),
                           [&iv](const VariableDescription& iv2) {
                             return iv2.name == iv.name;
                           }) == derivatives.second.end()) {
            cr += mfront::getTypeSize(iv);
            continue;
          }
          auto derivative_view = DerivativeViewDescription{};
          derivative_view.derivative_name =
              "df" + iv.name + "_dd" + derivatives.first.name;
          derivative_view.matrix_name = m;
          derivative_view.first_variable = iv;
          derivative_view.second_variable = derivatives.first;
          derivative_view.matrix_number_of_rows = nivs;
          derivative_view.matrix_number_of_columns = nc;
          derivative_view.derivative_row_position = cr;
          derivative_view.derivative_column_position =
              SupportedTypes::TypeSize{};
          this->writeDerivativeView(os, derivative_view);
          // update the row position
          cr += mfront::getTypeSize(iv);
        }
      }
    }
    // treating getIntegrationDerivatives
    if (hasAttribute<std::vector<VariableDescription>>(
            cb, CodeBlock::used_get_integration_variables_derivatives)) {
      const auto& get_integration_variables_derivatives =
          getAttribute<std::vector<VariableDescription>>(
              cb, CodeBlock::used_get_integration_variables_derivatives);
      if (!hasAttribute<std::vector<Derivative>>(cb, attr)) {
        this->throwRuntimeError(
            "ImplicitCodeGeneratorBase::"
            "writeBehaviourComputeTangentOperatorBody",
            "internal error, no implicit equation derivative with respect "
            "to "
            "a gradient or and external state variable detected");
      }
      const auto& implicit_equations_derivatives =
          getAttribute<std::vector<Derivative>>(cb, attr);
      for (const auto& givd : get_integration_variables_derivatives) {
        std::vector<Derivative> derivatives;
        std::copy_if(implicit_equations_derivatives.begin(),
                     implicit_equations_derivatives.end(),
                     std::back_inserter(derivatives),
                     [&givd](const Derivative& v) {
                       return givd.name == v.second.name;
                     });
        if (derivatives.empty()) {
          this->throwRuntimeError(
              "ImplicitCodeGeneratorBase::"
              "writeBehaviourComputeTangentOperatorBody",
              "internal error, no implicit equation derivative with "
              "respect to "
              "'" +
                  displayName(givd) + "' defined");
        }
        auto nc = mfront::getTypeSize(givd);
        const auto rhs_type = [&nivs, &nc] {
          std::ostringstream rhs_os;
          rhs_os << "tfel::math::tmatrix<" << nivs << ", " << nc
                 << ", NumericType>";
          return rhs_os.str();
        }();
        const auto m = "dfzeros_dd" + givd.name;
        os << "struct TFEL_VISIBILITY_LOCAL GetIntegrationVariablesDerivatives_"
           << givd.name << "{\n"
           << "TFEL_HOST_DEVICE GetIntegrationVariablesDerivatives_"
           << givd.name << "(" << this->bd.getClassName() << "& b,\n"
           << "const tfel::math::TinyPermutation<" << nivs << ">& "
           << "mfront_permutation_argument,\n"
           << rhs_type << "& mfront_rhs_argument,\n"
           << "bool& mfront_success_argument)\n"
           << ": behaviour(b),\n"
           << "mfront_local_permutation(mfront_permutation_argument)\n,"
           << "mfront_local_rhs(mfront_rhs_argument),\n"
           << "mfront_success_reference(mfront_success_argument)\n"
           << "{}\n";
        for (std::size_t i = 0; i != isvs.size(); ++i) {
          os << "TFEL_HOST_DEVICE void operator()(";
          for (std::size_t i2 = 0; i2 <= i;) {
            const auto& v = isvs[i2];
            os << getDerivativeTypeHolder(v, givd) << "& "
               << "integration_variable_derivative_d" << v.name << "_dd"
               << givd.name;
            if (++i2 <= i) {
              os << ",\n";
            }
          }
          os << "){\n"
             << rhs_type << " mfront_local_lhs(-(this->mfront_local_rhs));\n"
             << "if(!tfel::math::TinyMatrixSolve<" << nivs
             << ", NumericType, false>"
             << "::back_substitute(this->behaviour.jacobian, "
             << "this->mfront_local_permutation, mfront_local_lhs)){\n"
             << "this->mfront_success_reference=false;\n"
             << "}\n";
          auto cr = SupportedTypes::TypeSize{};  // current row
          for (std::size_t i2 = 0; i2 <= i; ++i2) {
            const auto& v = isvs[i2];
            auto derivative_view = DerivativeViewDescription{};
            derivative_view.derivative_name =
                "integration_variable_derivative_d" + v.name + "_dd" +
                givd.name + "_view";
            derivative_view.matrix_name = "mfront_local_lhs";
            derivative_view.first_variable = v;
            derivative_view.second_variable = givd;
            derivative_view.matrix_number_of_rows = nivs;
            derivative_view.matrix_number_of_columns = nc;
            derivative_view.derivative_row_position = cr;
            derivative_view.derivative_column_position =
                SupportedTypes::TypeSize{};
            this->writeDerivativeView(os, derivative_view);
            // update the row position
            cr += mfront::getTypeSize(v);
            // assign the view to the ouptut derivatives
            if (v.arraySize == 1u) {
              os << "integration_variable_derivative_d" << v.name << "_dd"
                 << givd.name << " = integration_variable_derivative_d"
                 << v.name << "_dd" << givd.name << "_view;\n";
            } else {
              os << "for(typename " << rhs_type
                 << "::size_type mfront_local_idx; mfront_local_idx !="
                 << v.arraySize << "; ++mfront_local_idx){\n"
                 << "integration_variable_derivative_d" << v.name << "_dd"
                 << givd.name
                 << "(mfront_local_idx) = integration_variable_derivative_d"
                 << v.name << "_dd" << givd.name << "_view(mfront_local_idx);"
                 << "}\n";
            }
          }
          os << "}\n";
        }
        os << "private:\n"
           << this->bd.getClassName() << "& behaviour;\n"
           << "const tfel::math::TinyPermutation<" << nivs
           << ">& mfront_local_permutation;\n"
           << rhs_type << "& mfront_local_rhs;\n"
           << "bool& mfront_success_reference;\n"
           << "};\n"
           << "GetIntegrationVariablesDerivatives_" << givd.name << " "
           << "getIntegrationVariablesDerivatives_" << givd.name
           << "(*this, jacobian_permutation," << m << ", mfront_success);\n";
      }
    }
    //
    BehaviourCodeGeneratorBase::writeBehaviourComputeTangentOperatorBody(os, h,
                                                                         n);
  }  // end of writeBehaviourComputeTangentOperatorBody

  void ImplicitCodeGeneratorBase::writeComputePartialJacobianInvert(
      std::ostream& os, const Hypothesis h) const {
    this->checkBehaviourFile(os);
    const auto& d = this->bd.getBehaviourData(h);
    const auto& isvs = d.getIntegrationVariables();
    const auto n = mfront::getTypeSize(isvs);
    const auto v2 = VariableDescription{"StrainStensor", "\u03B5\u1D57\u1D52",
                                        "eto", 1u, 0u};
    for (std::size_t i = 0; i != isvs.size(); ++i) {
      os << "[[nodiscard]] TFEL_HOST_DEVICE bool\ncomputePartialJacobianInvert("
            "const tfel::math::TinyPermutation<"
         << n << ">& jacobian_permutation, ";
      for (std::size_t i2 = 0; i2 <= i;) {
        const auto& v = isvs[i2];
        os << getDerivativeTypeHolder(v, v2) << "& ";
        os << "partial_jacobian_" << v.name;
        if (++i2 <= i) {
          os << ",\n";
        }
      }
      os << ")\n"
         << "{\n"
         << "using namespace tfel::math;\n"
         << "for(ushort mfront_idx=0; mfront_idx !=StensorSize; "
            "++mfront_idx){\n"
         << "tvector<" << n << ", NumericType> vect_e(NumericType(0));\n"
         << "vect_e(mfront_idx) = NumericType(1);\n"
         << "if(!TinyMatrixSolve<" << n << ", NumericType, false>"
         << "::back_substitute(this->jacobian, "
         << "jacobian_permutation, vect_e)){\n"
         << "return false;\n"
         << "}\n";
      SupportedTypes::TypeSize n2;
      for (std::size_t i2 = 0; i2 <= i; ++i2) {
        const auto& v = d.getIntegrationVariables()[i2];
        const auto flag = SupportedTypes::getTypeFlag(v.type);
        if (flag == SupportedTypes::SCALAR) {
          if (v.arraySize == 1u) {
            os << "partial_jacobian_" << v.name << "(mfront_idx)=vect_e(" << n2
               << ");\n";
          } else {
            os << "for(ushort mfront_idx2=0; mfront_idx2!=" << v.arraySize
               << "; ++mfront_idx2){\n"
               << "partial_jacobian_" << v.name
               << "(mfront_idx2)(mfront_idx)=vect_e(" << n2
               << "+mfront_idx2);\n"
               << "}\n";
          }
          n2 += SupportedTypes::getTypeSize(v.type, v.arraySize);
        } else if ((flag == SupportedTypes::TVECTOR) ||
                   (flag == SupportedTypes::STENSOR) ||
                   (flag == SupportedTypes::TENSOR)) {
          if (v.arraySize == 1u) {
            os << "for(ushort mfront_idx2=" << n2;
            os << "; mfront_idx2!=";
            n2 += SupportedTypes::getTypeSize(v.type, v.arraySize);
            os << n2 << "; ++mfront_idx2){\n";
            os << "partial_jacobian_" << v.name
               << "(mfront_idx2,mfront_idx)=vect_e(mfront_idx2);\n";
            os << "}\n";
          } else {
            const auto size = SupportedTypes::getTypeSize(v.type, 1u);
            os << "for(ushort mfront_idx2=0; mfront_idx2!=" << v.arraySize
               << "; ++mfront_idx2){\n"
               << "for(ushort mfront_idx3=0; mfront_idx3!= " << size
               << "; ++mfront_idx3){\n"
               << "partial_jacobian_" << v.name
               << "(mfront_idx2)(mfront_idx3,mfront_idx) = "
               << "vect_e(" << n2 << "+ mfront_idx3 + mfront_idx2 * " << size
               << ");\n"
               << "}\n"
               << "}\n";
            n2 += SupportedTypes::getTypeSize(v.type, v.arraySize);
          }
        } else {
          this->throwRuntimeError(
              "ImplicitCodeGeneratorBase::writeComputePartialJacobianInvert",
              "internal error, tag unsupported");
        }
      }
      os << "}\n";
      for (std::size_t i2 = 0; i2 <= i; ++i2) {
        const auto& v = d.getIntegrationVariables()[i2];
        if (this->bd.hasAttribute(h, v.name + "_normalisation_factor")) {
          const auto& nf = this->bd.getAttribute<std::string>(
              h, v.name + "_normalisation_factor");
          os << "partial_jacobian_" << v.name << " /= " << nf << ";\n";
        }
      }
      os << "return true;\n"
         << "}\n\n";
    }
  }  // end of writeComputePartialJacobianInvert

  void ImplicitCodeGeneratorBase::writeComputeNumericalJacobian(
      std::ostream& os, const Hypothesis h) const {
    const auto& d = this->bd.getBehaviourData(h);
    const auto n = d.getIntegrationVariables().getTypeSize();
    this->checkBehaviourFile(os);
    os << "TFEL_HOST_DEVICE void computeNumericalJacobian("
       << "tfel::math::tmatrix<" << n << "," << n
       << ", NumericType>& njacobian)\n"
       << "{\n"
       << "using namespace std;\n"
       << "using namespace tfel::math;\n"
       << "tvector<" << n << ", NumericType> tzeros(this->zeros);\n"
       << "tvector<" << n << ", NumericType> tfzeros(this->fzeros);\n"
       << "tmatrix<" << n << "," << n
       << ", NumericType> tjacobian(this->jacobian);\n"
       << "for(ushort mfront_idx = 0; mfront_idx != " << n
       << "; ++mfront_idx){\n"
       << "this->zeros(mfront_idx) -= this->numerical_jacobian_epsilon;\n";
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "this->computeThermodynamicForces();\n";
    }
    os << "this->computeFdF(true);\n"
       << "this->zeros = tzeros;\n"
       << "tvector<" << n << ", NumericType> tfzeros2(this->fzeros);\n"
       << "this->zeros(mfront_idx) += this->numerical_jacobian_epsilon;\n";
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "this->computeThermodynamicForces();\n";
    }
    os << "this->computeFdF(true);\n"
       << "this->fzeros = "
          "(this->fzeros-tfzeros2) / (2 * "
          "(this->numerical_jacobian_epsilon));\n"
       << "for(ushort mfront_idx2 = 0; mfront_idx2!= " << n
       << "; ++mfront_idx2){\n"
       << "njacobian(mfront_idx2,mfront_idx) = this->fzeros(mfront_idx2);\n"
       << "}\n"
       << "this->zeros    = tzeros;\n"
       << "this->fzeros   = tfzeros;\n"
       << "}\n"
       << "if(&njacobian!=&(this->jacobian)){\n"
       << "this->jacobian = tjacobian;\n"
       << "}\n"
       << "}\n\n";
  }  // end of writeComputeNumericalJacobian

  void ImplicitCodeGeneratorBase::writeBehaviourIntegrator(
      std::ostream& os, const Hypothesis h) const {
    const auto btype = this->bd.getBehaviourTypeFlag();
    const auto& d = this->bd.getBehaviourData(h);
    const auto n2 = mfront::getTypeSize(d.getIntegrationVariables());
    this->checkBehaviourFile(os);
    os << "/*!\n"
       << " * \\brief Integrate behaviour law over the time step\n"
       << " */\n"
       << "[[nodiscard]] TFEL_HOST_DEVICE IntegrationResult ";
    if (this->bd.hasAttribute(h, BehaviourData::hasConsistentTangentOperator)) {
      os << "integrate(const SMFlag smflag,const SMType smt) override{\n";
    } else {
      if (!this->bd.getTangentOperatorBlocks().empty()) {
        os << "integrate(const SMFlag smflag,const SMType smt) override{\n";
      } else {
        os << "integrate(const SMFlag,const SMType smt) override{\n";
      }
    }
    os << "using namespace std;\n";
    os << "using namespace tfel::math;\n";
    writeMaterialLaws(os, this->bd.getMaterialLaws());
    os << "this->stiffness_matrix_type = smt;" << '\n';
    if (!this->bd.getTangentOperatorBlocks().empty()) {
      if ((this->bd.getBehaviourType() ==
           BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
          (this->bd.getBehaviourType() ==
           BehaviourDescription::COHESIVEZONEMODEL) ||
          (this->bd.getBehaviourType() ==
           BehaviourDescription::GENERALBEHAVIOUR)) {
        if (this->bd.useQt()) {
          os << "tfel::raise_if(smflag!=MechanicalBehaviour<" << btype
             << ",hypothesis, NumericType, use_qt>::STANDARDTANGENTOPERATOR,\n"
             << "\"invalid tangent operator flag\");\n";
        } else {
          os << "tfel::raise_if(smflag!=MechanicalBehaviour<" << btype
             << ",hypothesis, NumericType, false>::STANDARDTANGENTOPERATOR,\n"
             << "\"invalid tangent operator flag\");\n";
        }
      }
    }
    if (this->bd.hasCode(h, BehaviourData::ComputePredictor)) {
      os << this->bd.getCode(h, BehaviourData::ComputePredictor) << '\n';
    }
    if (this->bd.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfilingBegin(os, this->bd.getClassName(),
                                             BehaviourData::Integrator);
    }
    this->solver.initializeNumericalParameters(os, this->bd, h);
    os << "if(!this->solveNonLinearSystem()){\n";
    if (this->bd.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType, use_qt>::FAILURE;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType, false>::FAILURE;\n";
    }
    os << "}\n";
    // We compute the numerical jacobian even if we converged since
    // most of the time, this tangent operator will be computed
    // using the partial jacobian invert. We consider very unlikely
    // that a user may use a numerical jacobian and provide an
    // analytic definition of the tangent operator
    os << "if (this->stiffness_matrix_type != NOSTIFFNESSREQUESTED){\n"
       << "this->updateOrCheckJacobian();\n"
       << "}\n";
    if (this->bd.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfilingEnd(os);
    }
    for (const auto& v : d.getIntegrationVariables()) {
      if (this->bd.hasAttribute(h, v.name + "_normalisation_factor")) {
        const auto& nf = this->bd.getAttribute<std::string>(
            h, v.name + "_normalisation_factor");
        os << "this->d" << v.name << " *= " << nf << ";\n";
      }
    }
    os << "this->updateIntegrationVariables();\n";
    os << "this->updateStateVariables();\n";
    if (this->bd.hasCode(h, BehaviourData::ComputeFinalThermodynamicForces)) {
      os << "this->computeFinalThermodynamicForces();\n";
    }
    os << "this->updateAuxiliaryStateVariables();\n";
    for (const auto& v : d.getPersistentVariables()) {
      this->writePhysicalBoundsChecks(os, v, false);
    }
    for (const auto& v : d.getPersistentVariables()) {
      this->writeBoundsChecks(os, v, false);
    }
    if (!this->bd.getTangentOperatorBlocks().empty()) {
      os << "if(smt!=NOSTIFFNESSREQUESTED){\n";
      if (this->bd.hasAttribute(h,
                                BehaviourData::hasConsistentTangentOperator)) {
        if (this->bd.getBehaviourType() ==
            BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
          os << "if(!this->computeConsistentTangentOperator(smflag,smt)){\n";
        } else {
          os << "if(!this->computeConsistentTangentOperator(smt)){\n";
        }
        if (this->bd.useQt()) {
          os << "return MechanicalBehaviour<" << btype
             << ",hypothesis, NumericType, use_qt>::FAILURE;\n";
        } else {
          os << "return MechanicalBehaviour<" << btype
             << ",hypothesis, NumericType, false>::FAILURE;\n";
        }
        os << "}\n";
      } else {
        os << "string msg(\"" << this->bd.getClassName()
           << "::integrate : \");\n";
        os << "msg +=\"unimplemented feature\";\n";
        os << "throw(runtime_error(msg));\n";
      }
      os << "}\n";
    }
    if (this->bd.useQt()) {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType, use_qt>::SUCCESS;\n";
    } else {
      os << "return MechanicalBehaviour<" << btype
         << ",hypothesis, NumericType, false>::SUCCESS;\n";
    }
    os << "} // end of " << this->bd.getClassName() << "::integrate\n\n";
    //
    os << "TFEL_HOST_DEVICE bool computeResidual(){\n";
    if (this->bd.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      os << "this->computeThermodynamicForces();\n";
    }
    os << "return this->computeFdF(false);\n"
       << "}\n";
    //
    os << "//! \\return the norm of the residual\n"
       << "TFEL_HOST_DEVICE NumericType computeResidualNorm() {\n"
       << "return tfel::math::norm(this->fzeros) / (" << n2 << ");\n"
       << "}\n"
       << "/*!\n"
       << " * \\brief check the convergence of the method\n"
       << " * \\param[in] error: current error\n"
       << " */\n"
       << "TFEL_HOST_DEVICE bool checkConvergence(const NumericType error) {\n"
       << "auto converged = error < this->epsilon;\n";
    if (this->bd.hasCode(h, BehaviourData::AdditionalConvergenceChecks)) {
      os << "auto mfront_internals_converged = converged;\n"
         << "this->additionalConvergenceChecks(converged, error);\n"
         << "if((mfront_internals_converged) && (!converged)){\n"
         << "this->is_delta_zeros_defined = false;\n"
         << "}\n";
    }
    os << "return converged;\n"
       << "}\n"
       << "/*!\n"
       << " * \\brief update the jacobian matrix if required.\n"
       << " * \\param[in] converged: boolean stating if the method is\n"
       << " * called after the convergence of the algorithm or before\n"
       << " * the computation of the next correction.\n"
       << " *\n"
       << " * This method can be used to compute the jacobian or part\n"
       << " * of the jacobian numerically. If the jacobian was computed\n"
       << " * in `computeResidual`, this method can be used to compare it\n"
       << " * to a numerical approximation.\n"
       << " */\n"
       << "TFEL_HOST_DEVICE void updateOrCheckJacobian(){\n";
    if (this->solver.requiresNumericalJacobian()) {
      os << "this->computeNumericalJacobian(this->jacobian);\n";
    } else {
      NonLinearSystemSolverBase::writeEvaluateNumericallyComputedBlocks(
          os, this->bd, h);
      if (this->bd.hasAttribute(h, BehaviourData::compareToNumericalJacobian)) {
        os << "tfel::math::tmatrix<" << n2 << ", " << n2
           << ", NumericType> njacobian;\n";
        NonLinearSystemSolverBase::writeComparisonToNumericalJacobian(
            os, this->bd, h, "njacobian");
      }
    }
    os << "}\n"
       << "/*!\n"
       << " * \\brief method meant to set bounds on some components\n"
       << " * of the current Newton correction or to implement a line\n"
       << " * search.\n"
       << " */\n"
       << "TFEL_HOST_DEVICE void processNewCorrection()\n"
       << "{\n";
    if (this->bd.hasCode(h, BehaviourData::ProcessNewCorrection)) {
      os << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using std::vector;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      writeVariablesOffsets(os, d.getIntegrationVariables());
      declareViewsFromArrayOfVariables(
          os, d.getIntegrationVariables(),
          [](const std::string& n) { return "delta_d" + n; },
          "this->delta_zeros", this->bd.useQt());
    }
    NonLinearSystemSolverBase::writeLimitsOnIncrementValues(
        os, this->bd, h, "this->delta_zeros");
    if (this->bd.hasCode(h, BehaviourData::ProcessNewCorrection)) {
      os << this->bd.getCodeBlock(h, BehaviourData::ProcessNewCorrection).code
         << "\n";
      writeIgnoreVariablesOffsets(os, d.getIntegrationVariables());
      for (const auto& v : d.getIntegrationVariables()) {
        os << "static_cast<void>(delta_d" << v.name << ");\n";
      }
    }
    os << "}\n";
    if (this->bd.hasCode(h, BehaviourData::RejectCurrentCorrection)) {
      os << "/*!\n"
         << " * \\brief method called when the current Newton is rejected\n"
         << " */\n"
         << "TFEL_HOST_DEVICE void rejectCurrentCorrection()\n"
         << "{\n"
         << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using std::vector;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
      writeVariablesOffsets(os, d.getIntegrationVariables());
      declareViewsFromArrayOfVariables(
          os, d.getIntegrationVariables(),
          [](const std::string& n) { return "delta_d" + n; },
          "this->delta_zeros", this->bd.useQt());
      os << this->bd.getCodeBlock(h, BehaviourData::RejectCurrentCorrection)
                .code
         << "\n";
      writeIgnoreVariablesOffsets(os, d.getIntegrationVariables());
      for (const auto& v : d.getIntegrationVariables()) {
        os << "static_cast<void>(delta_d" << v.name << ");\n";
      }
      os << "}\n";
    }
    os << "/*!\n"
       << " * \\brief method meant to process the new estimate.\n"
       << " *\n"
       << " * This method may be called to apply bounds on the\n"
       << " * new estimate and update material properties\n"
       << " * dependending on the current state.\n"
       << " */\n"
       << "TFEL_HOST_DEVICE void processNewEstimate(){\n";
    if (this->bd.hasCode(h, BehaviourData::ProcessNewEstimate)) {
      os << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using std::vector;\n";
      writeMaterialLaws(os, this->bd.getMaterialLaws());
    }
    NonLinearSystemSolverBase::
        writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(
            os, this->bd, h);
    NonLinearSystemSolverBase::
        writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(
            os, this->bd, h);
    if (this->bd.hasCode(h, BehaviourData::ProcessNewEstimate)) {
      os << this->bd.getCodeBlock(h, BehaviourData::ProcessNewEstimate).code
         << "\n";
    }
    os << "this->updateMaterialPropertiesDependantOnStateVariables();\n"
       << "}\n";
    if (getDebugMode()) {
      os << "//! \\brief method called when the resolution begins\n"
         << "void reportBeginningOfResolution() const {\n"
         << "std::cout << '\\n' << \"" << this->bd.getClassName()
         << "::integrate() : beginning of resolution\\n\";\n"
         << "}\n"
         << "//! \\brief method called when the resolution succeeds\n"
         << "void reportSuccess() const {\n"
         << "std::cout << \"" << this->bd.getClassName()
         << "::integrate() : convergence after \" "
         << "<< this->iter << \" iterations\\n\\n\";\n"
         << "}\n"
         << "//! \brief method called when the resolution fails\n"
         << "void reportFailure() const {\n"
         << "std::cout << \"" << this->bd.getClassName()
         << "::integrate() : no convergence after \" "
         << "<< this->iter << \" iterations\\n\\n\";\n"
         << "std::cout << *this << '\\n';\n"
         << "}\n"
         << "/*!\n"
         << " * \\brief method called with the evaluation of the\n"
         << " * residual failed.\n "
         << " */\n"
         << "void reportInvalidResidualEvaluation() const {"
         << "std::cout << '\\n' << \"" << this->bd.getClassName()
         << "::integrate() : computFdF returned false or the norm of the "
         << "residual is not finite, dividing increment by "
         << "two\\n\\n\";\n"
         << "}\n"
         << "/*!\n"
         << " * \brief method called when the computation of a new correction\n"
         << " * failed.\n"
         << " */\n"
         << "void reportNewCorrectionComputationFailure() const {\n"
         << "std::cout << '\\n' << \"" << this->bd.getClassName()
         << "::integrate(): failed to compute a new correction\";\n"
         << "}\n"
         << "/*!\n"
         << " * \\brief method called after a standard Newton step\n"
         << " * \\param[in] e: error\n"
         << " */\n"
         << "void reportStandardIteration(const NumericType error) const {\n"
         << "std::cout << \"" << this->bd.getClassName()
         << "::integrate() : iteration \" "
         << "<< this->iter << \" : \" << error << '\\n';\n"
         << "}\n";
    }
    os << "/*!\n"
       << " * \\brief solve linear system\n"
       << " * \\return true on success\n"
       << " * \\param[in] m: matrix\n"
       << " * \\param[in,out] v: right hand side on input, solution on output\n"
       << " */\n"
       << "TFEL_HOST_DEVICE bool solveLinearSystem("
       << "tfel::math::tmatrix<" << n2 << ", " << n2
       << ", NumericType>& mfront_matrix,"
       << "tfel::math::tvector<" << n2 << ", NumericType>& mfront_vector)"
       << "const noexcept{\n"
       << "auto mfront_success = true;\n";
    if (this->bd.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfilingBegin(os, this->bd.getClassName(),
                                             "TinyMatrixSolve", "lu");
    }
    os << "mfront_success = "
       << this->solver.getExternalAlgorithmClassName(this->bd, h)
       << "::solveLinearSystem(mfront_matrix, mfront_vector);\n";
    if (this->bd.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfilingEnd(os);
    }
    os << "return mfront_success;\n"
       << "}\n";
    this->writeComputeFdF(os, h);
  }  // end of writeBehaviourIntegrator

  void ImplicitCodeGeneratorBase::writeComputeFdF(std::ostream& os,
                                                  const Hypothesis h) const {
    const auto& d = this->bd.getBehaviourData(h);
    auto jp(this->jacobianPartsUsedInIntegrator);
    if (this->solver
            .requiresJacobianToBeReinitialisedToIdentityAtEachIterations()) {
      for (const auto& v : d.getIntegrationVariables()) {
        jp.insert("df" + v.name + "_dd" + v.name);
      }
    }
    //
    os << "/*!\n"
       << "* \\brief compute fzeros and jacobian\n"
       << "*/\n"
       << "TFEL_HOST_DEVICE bool computeFdF(const bool "
          "perturbatedSystemEvaluation){\n"
       << "using namespace std;\n"
       << "using namespace tfel::math;\n"
       << "using std::vector;\n";
    if (this->bd.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfiling(os, this->bd.getClassName(),
                                        "ComputeFdF");
    }
    writeMaterialLaws(os, this->bd.getMaterialLaws());
    os << "// silent compiler warning\n"
       << "static_cast<void>(perturbatedSystemEvaluation); \n";
    writeVariablesOffsets(os, d.getIntegrationVariables());
    declareViewsFromArrayOfVariables(
        os, d.getIntegrationVariables(),
        [](const std::string& n) { return "f" + n; }, "this->fzeros",
        this->bd.useQt());
    if ((this->solver.usesJacobian()) &&
        (!(this->solver.requiresNumericalJacobian()))) {
      auto n = SupportedTypes::TypeSize();
      for (const auto& v : d.getIntegrationVariables()) {
        auto n2 = SupportedTypes::TypeSize();
        for (const auto& v2 : d.getIntegrationVariables()) {
          if ((v.arraySize == 1u) && (v2.arraySize == 1u)) {
            os << "// derivative of residual f" << v.name
               << " with respect to variable " << v2.name << "\n"
               << NonLinearSystemSolverBase::getJacobianPart(v, v2, n, n2);
          }
          n2 += SupportedTypes::getTypeSize(v2.type, v2.arraySize);
        }
        n += SupportedTypes::getTypeSize(v.type, v.arraySize);
      }
    }
    if (this->solver
            .requiresJacobianToBeReinitialisedToIdentityAtEachIterations()) {
      os << "// setting jacobian to identity\n"
         << "std::fill(this->jacobian.begin(),this->jacobian.end(), "
            "NumericType(0))"
            ";\n"
         << "for(ushort mfront_idx = 0; mfront_idx != "
         << mfront::getTypeSize(d.getIntegrationVariables())
         << "; ++mfront_idx){\n"
         << "this->jacobian(mfront_idx, mfront_idx) = NumericType(1);\n"
         << "}\n";
    }
    os << "// setting f values to zeros\n"
       << "this->fzeros = this->zeros;\n";
    for (const auto& v : d.getIntegrationVariables()) {
      if (this->bd.hasAttribute(h, v.name + "_normalisation_factor")) {
        const auto& nf = this->bd.getAttribute<std::string>(
            h, v.name + "_normalisation_factor");
        os << "f" << v.name << " *= " << nf << ";\n";
      }
    }
    os << this->bd.getCodeBlock(h, BehaviourData::Integrator).code << "\n";
    for (const auto& v : d.getIntegrationVariables()) {
      if (this->bd.hasAttribute(h, 'f' + v.name + "_normalisation_factor")) {
        const auto& nf = this->bd.getAttribute<std::string>(
            h, 'f' + v.name + "_normalisation_factor");
        os << "f" << v.name << "*= NumericType(1)/(" << nf << ");\n";
      }
    }
    if ((this->solver.usesJacobian()) &&
        (!(this->solver.requiresNumericalJacobian()))) {
      for (const auto& v : d.getIntegrationVariables()) {
        for (const auto& v2 : d.getIntegrationVariables()) {
          if ((v.arraySize == 1u) && (v2.arraySize == 1u)) {
            os << "static_cast<void>(df" << v.name << "_dd" << v2.name
               << "); /* suppress potential warnings */\n";
          }
          if (jp.find("df" + v.name + "_dd" + v2.name) != jp.end()) {
            const bool bfnf = this->bd.hasAttribute(
                h, 'f' + v.name + "_normalisation_factor");
            const bool bvnf =
                this->bd.hasAttribute(h, v2.name + "_normalisation_factor");
            if (bfnf) {
              const auto& fnf = this->bd.getAttribute<std::string>(
                  h, 'f' + v.name + "_normalisation_factor");
              if (bvnf) {
                const auto& vnf = this->bd.getAttribute<std::string>(
                    h, v2.name + "_normalisation_factor");
                if ((v.arraySize != 1u) && (v2.arraySize != 1u)) {
                  os << "for(ushort mfront_idx=0; mfront_idx !=" << v.arraySize
                     << "; ++mfront_idx){\n";
                  os << "for(ushort mfront_idx2=0; mfront_idx2!="
                     << v2.arraySize << "; ++mfront_idx2){\n";
                  os << "df" << v.name << "_dd" << v2.name
                     << "(mfront_idx,mfront_idx2) *= " << vnf << "/(" << fnf
                     << ");\n";
                  os << "}\n";
                  os << "}\n";
                } else if ((v.arraySize != 1u) && (v2.arraySize == 1u)) {
                  os << "for(ushort mfront_idx=0; mfront_idx !=" << v.arraySize
                     << "; ++mfront_idx){\n";
                  os << "df" << v.name << "_dd" << v2.name
                     << "(mfront_idx) *= " << vnf << "/(" << fnf << ");\n";
                  os << "}\n";
                } else if ((v.arraySize == 1u) && (v2.arraySize != 1u)) {
                  os << "for(ushort mfront_idx=0; mfront_idx !=" << v2.arraySize
                     << "; ++mfront_idx){\n";
                  os << "df" << v.name << "_dd" << v2.name
                     << "(mfront_idx) *= " << vnf << "/(" << fnf << ");\n";
                  os << "}\n";
                } else if ((v.arraySize == 1u) && (v2.arraySize == 1u)) {
                  os << "df" << v.name << "_dd" << v2.name << " *= " << vnf
                     << "/(" << fnf << ");\n";
                }
              } else {
                if ((v.arraySize != 1u) && (v2.arraySize != 1u)) {
                  os << "for(ushort mfront_idx=0; mfront_idx !=" << v.arraySize
                     << "; ++mfront_idx){\n";
                  os << "for(ushort mfront_idx2=0; mfront_idx2!="
                     << v2.arraySize << "; ++mfront_idx2){\n";
                  os << "df" << v.name << "_dd" << v2.name
                     << "(mfront_idx,mfront_idx2) *= "
                     << "NumericType(1)/(" << fnf << ");\n";
                  os << "}\n";
                  os << "}\n";
                } else if ((v.arraySize != 1u) && (v2.arraySize == 1u)) {
                  os << "for(ushort mfront_idx=0; mfront_idx !=" << v.arraySize
                     << "; ++mfront_idx){\n";
                  os << "df" << v.name << "_dd" << v2.name << "(mfront_idx) *= "
                     << "NumericType(1)/(" << fnf << ");\n";
                  os << "}\n";
                } else if ((v.arraySize == 1u) && (v2.arraySize != 1u)) {
                  os << "for(ushort mfront_idx=0; mfront_idx !=" << v2.arraySize
                     << "; ++mfront_idx){\n";
                  os << "df" << v.name << "_dd" << v2.name << "(mfront_idx) *= "
                     << "NumericType(1)/(" << fnf << ");\n";
                  os << "}\n";
                } else if ((v.arraySize == 1u) && (v2.arraySize == 1u)) {
                  os << "df" << v.name << "_dd" << v2.name << " *= "
                     << "NumericType(1)/(" << fnf << ");\n";
                }
              }
            } else {
              if (bvnf) {
                const auto& vnf = this->bd.getAttribute<std::string>(
                    h, v2.name + "_normalisation_factor");
                if ((v.arraySize != 1u) && (v2.arraySize != 1u)) {
                  os << "for(ushort mfront_idx=0; mfront_idx !=" << v.arraySize
                     << "; ++mfront_idx){\n";
                  os << "for(ushort mfront_idx2=0; mfront_idx2!="
                     << v2.arraySize << "; ++mfront_idx2){\n";
                  os << "df" << v.name << "_dd" << v2.name
                     << "(mfront_idx,mfront_idx2) *= " << vnf << ";\n";
                  os << "}\n";
                  os << "}\n";
                } else if ((v.arraySize != 1u) && (v2.arraySize == 1u)) {
                  os << "for(ushort mfront_idx=0; mfront_idx !=" << v.arraySize
                     << "; ++mfront_idx){\n";
                  os << "df" << v.name << "_dd" << v2.name
                     << "(mfront_idx) *= " << vnf << ";\n";
                  os << "}\n";
                } else if ((v.arraySize == 1u) && (v2.arraySize != 1u)) {
                  os << "for(ushort mfront_idx=0; mfront_idx !=" << v2.arraySize
                     << "; ++mfront_idx){\n";
                  os << "df" << v.name << "_dd" << v2.name
                     << "(mfront_idx) *= " << vnf << ";\n";
                  os << "}\n";
                } else if ((v.arraySize == 1u) && (v2.arraySize == 1u)) {
                  os << "df" << v.name << "_dd" << v2.name << " *= " << vnf
                     << ";\n";
                }
              }
            }
          }
        }
      }
    }
    writeIgnoreVariablesOffsets(os, d.getIntegrationVariables());
    os << "return true;\n"
       << "}\n\n";
  }  // end of writeBehaviourIntegrator

  std::string
  ImplicitCodeGeneratorBase::getIntegrationVariablesIncrementsInitializers(
      const Hypothesis h) const {
    SupportedTypes::TypeSize n;
    std::ostringstream init;
    bool first = true;
    for (const auto& v :
         this->bd.getBehaviourData(h).getIntegrationVariables()) {
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      if (!first) {
        init << ",\n";
      }
      if (v.arraySize == 1u) {
        if (flag == SupportedTypes::SCALAR) {
          init << "d" << v.name << "(this->zeros(" << n << "))";
        } else {
          init << "d" << v.name << "(tfel::math::map<" << v.type << ", " << n
               << ">(this->zeros))";
        }
      } else {
        init << "d" << v.name << "(tfel::math::map<" << v.arraySize << ", "
             << v.type << ", " << n << ">(this->zeros))";
      }
      n += SupportedTypes::getTypeSize(v.type, v.arraySize);
      first = false;
    }
    return init.str();
  }  // end of getIntegrationVariableIncrementsInitializers

  std::string ImplicitCodeGeneratorBase::getLocalVariablesInitializers(
      const Hypothesis) const {
    auto init = std::string{};
    auto append = [&init](const std::string& s) {
      if (s.empty()) {
        return;
      }
      if (!init.empty()) {
        init += ",\n";
      }
      init += s;
    };
    if (this->bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                              false)) {
      if (this->bd.areElasticMaterialPropertiesConstantDuringTheTimeStep()) {
        append("D_tdt(D)");
      }
    }
    for (const auto& ht : this->bd.getHillTensors()) {
      if ((this->bd.getBehaviourType() !=
           BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
          (this->bd.getBehaviourType() !=
           BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)) {
        this->throwRuntimeError(
            "ImplicitCodeGeneratorBase::getLocalVariablesInitializers",
            "Hill tensors shall only be defined for finite strain "
            "or small strain behaviours");
      }
      if (this->bd.areMaterialPropertiesConstantDuringTheTimeStep(ht.c)) {
        append(ht.name + "_tdt(" + ht.name + ")");
      }
    }
    return init;
  }  // end of getLocalVariablesInitializers

  void
  ImplicitCodeGeneratorBase::writeBehaviourParserSpecificInitializeMethodPart(
      std::ostream& os, const Hypothesis h) const {
    this->solver.writeSpecificInitializeMethodPart(os, this->bd, h);
  }

  void ImplicitCodeGeneratorBase::writeBehaviourIntegrationVariablesIncrements(
      std::ostream& os, const Hypothesis h) const {
    const auto& d = this->bd.getBehaviourData(h);
    this->checkBehaviourFile(os);
    for (const auto& v : d.getIntegrationVariables()) {
      if ((!getDebugMode()) && (v.lineNumber != 0u)) {
        os << "#line " << v.lineNumber << " \"" << this->fd.fileName << "\"\n";
      }
      if (v.arraySize == 1u) {
        if (SupportedTypes::getTypeFlag(v.type) == SupportedTypes::SCALAR) {
          if (this->bd.useQt()) {
            os << "tfel::math::scalar_view<" << v.type << "> "
               << "d" << v.name << ";\n";
          } else {
            os << "real& d" << v.name << ";\n";
          }
        } else {
          os << "tfel::math::View<" << v.type << "> d" << v.name << ";\n";
        }
      } else {
        os << "tfel::math::ViewsFixedSizeVector<" << v.type << ", ushort, "
           << v.arraySize << "> d" << v.name << ";\n";
      }
    }
    os << '\n';
  }

  ImplicitCodeGeneratorBase::~ImplicitCodeGeneratorBase() = default;

}  // end of namespace mfront
