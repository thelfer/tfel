/*!
 * \file   mfront/src/CastemModelInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   03/07/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/TargetsDescription.hxx"
// #include "MFront/CastemBehaviourSymbolsGenerator.hxx"
#include "MFront/CastemInterface.hxx"
#include "MFront/CastemSymbolsGenerator.hxx"
#include "MFront/CastemModelInterface.hxx"

namespace mfront {

  static std::string getModelName(const ModelDescription& md) {
    return md.library + md.className;
  }  // end of getModelName

  static std::string getUmatFunctionName(const ModelDescription& md) {
    return "umat" + makeLowerCase(getModelName(md));
  }  // end of getUmatFunctionName

  static bool isRealParameter(const VariableDescription& p) {
    return (p.type == "double") || (p.isScalar());
  }  // end of isRealParameter

  static bool hasRealParameters(const ModelDescription& md) {
    if (areParametersTreatedAsStaticVariables(md)) {
      return false;
    }
    for (const auto& p : md.parameters) {
      if (isRealParameter(p)) {
        return true;
      }
    }
    return false;
  }  // end of hasRealParameters

  static std::string getHeaderGuard(const ModelDescription& md) {
    auto header = std::string{"LIB_CASTEMMODEL_"};
    if (!md.library.empty()) {
      header += makeUpperCase(md.library);
      header += "_";
    }
    header += makeUpperCase(md.className);
    header += "_HXX";
    return header;
  }  // end of getHeaderGuard

  static bool useQuantities(const ModelDescription& md) {
    return md.use_qt.has_value() ? *(md.use_qt) : false;
  }  // end of useQuantities

  static void writeScalarStandardTypedefs(std::ostream& os,
                                          const ModelDescription& md) {
    const auto use_qt = useQuantities(md) ? "true" : "false";
    os << "using NumericType [[maybe_unused]] = double;\n";
    for (const auto& a : getScalarTypeAliases()) {
      os << "using " << a << " [[maybe_unused]] = "
         << "typename tfel::config::ScalarTypes<double, " << use_qt
         << ">::" << a << ";\n";
    }
  }  // end of writeScalarStandardTypedefs

  void CastemModelInterface::writeUMATFunctionArguments(std::ostream& os) {
    os << "(castem::CastemReal *const,\n"
       << " castem::CastemReal *const STATEV,\n"
       << " castem::CastemReal *const,\n"
       << " castem::CastemReal *const,\n"
       << " castem::CastemReal *const,\n"
       << " castem::CastemReal *const,\n"
       << " castem::CastemReal *const,\n"
       << " castem::CastemReal *const,\n"
       << " castem::CastemReal *const,\n"
       << " castem::CastemReal *const,\n"
       << " const castem::CastemReal *const,\n"
       << " const castem::CastemReal *const,\n"
       << " const castem::CastemReal *const,\n"
       << " const castem::CastemReal *const DTIME,\n"
       << " const castem::CastemReal *const TEMP,\n"
       << " const castem::CastemReal *const DTEMP,\n"
       << " const castem::CastemReal *const PREDEF,\n"
       << " const castem::CastemReal *const DPRED,\n"
       << " const char           *const,\n"
       << " const castem::CastemInt  *const,\n"
       << " const castem::CastemInt  *const,\n"
       << " const castem::CastemInt  *const,\n"
       << " const castem::CastemInt  *const NSTATV,\n"
       << " const castem::CastemReal *const PROPS,\n"
       << " const castem::CastemInt  *const NPROPS,\n"
       << " const castem::CastemReal *const,\n"
       << " const castem::CastemReal *const,\n"
       << "       castem::CastemReal *const PNEWDT,\n"
       << " const castem::CastemReal *const,\n"
       << " const castem::CastemReal *const,\n"
       << " const castem::CastemReal *const,\n"
       << " const castem::CastemInt  *const,\n"
       << " const castem::CastemInt  *const,\n"
       << " const castem::CastemInt  *const,\n"
       << " const castem::CastemInt  *const,\n"
       << " const castem::CastemInt  *const,\n"
       << "       castem::CastemInt  *const KINC,\n"
       << "const int)";
  }  // end of writeUMATFunctionArguments

  CastemModelInterface::CastemModelInterface() = default;

  std::string CastemModelInterface::getLibraryName(
      const ModelDescription& md) const {
    if (md.library.empty()) {
      if (!md.material.empty()) {
        return "Castem" + md.material;
      } else {
        return "CastemModel";
      }
    }
    return "Castem" + md.library;
  }  // end of getLibraryName

  std::string CastemModelInterface::getName() {
    return "castem";
  }  // end of getName

  std::pair<bool, CastemModelInterface::tokens_iterator>
  CastemModelInterface::treatKeyword(const std::string& k,
                                     const std::vector<std::string>& i,
                                     tokens_iterator current,
                                     const tokens_iterator) {
    tfel::raise_if(
        ((std::find(i.begin(), i.end(), this->getName()) != i.end()) ||
         (std::find(i.begin(), i.end(), "umat") != i.end()) ||
         (std::find(i.begin(), i.end(), "Castem") != i.end()) ||
         (std::find(i.begin(), i.end(), "Cast3M") != i.end())),
        "CastemModelInterface::treatKeyword: unsupported keyword '" + k + "'");
    return {false, current};
  }  // end of treatKeyword

  void CastemModelInterface::declareReservedNames(std::set<std::string>&) {
  }  // end of declareReservedNames

  void CastemModelInterface::writeOutputFiles(const FileDescription& fd,
                                              const ModelDescription& md) {
    this->writeHeaderFile(fd, md);
    this->writeSourceFile(fd, md);
  }  // end of writeOutputFiles

  void CastemModelInterface::writeHeaderFile(const FileDescription& fd,
                                             const ModelDescription& md) {
    using tfel::material::ModellingHypothesis;
    auto raise = [](const std::string& m) {
      tfel::raise("CastemModelInterface::writeHeaderFile: " + m);
    };
    tfel::system::systemCall::mkdir("include/MFront");
    tfel::system::systemCall::mkdir("include/MFront/CastemModel");
    const auto name = getModelName(md);
    const auto header = name + "-castem.hxx";
    std::ofstream os("include/MFront/CastemModel/" + header);
    if (!os) {
      raise("could not open file '" + header + "'");
    }
    os << "/*!\n"
       << " * \\file   " << header << '\n'
       << " * \\brief  This file declares the generic interface for the "
       << md.className << " model\n"
       << " * \\author " << fd.authorName << '\n'
       << " * \\date   " << fd.date << '\n'
       << " */\n\n";

    const auto hg = getHeaderGuard(md);
    os << "#ifndef " << hg << "\n"
       << "#define " << hg << "\n\n"
       << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
       << "#include\"MFront/Castem/Castem.hxx\"\n\n";

    writeExportDirectives(os, false);

    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n";

    if ((hasRealParameters(md)) &&
        (!areParametersTreatedAsStaticVariables(md))) {
      os << "/*!\n"
         << " * \\brief function allowing to modify the parameters of the "
         << name << " model\n"
         << " * \\param[in] n: name of the parameter\n"
         << " * \\param[in] v: value of the parameter\n"
         << " */\n"
         << "MFRONT_SHAREDOBJ int\n"
         << name << "_setParameter(const char *const, const double);\n\n";
    }

    os << "/*!\n"
       << " * \\brief function implementing the " << name << " model\n"
       << " */\n"
       << "MFRONT_SHAREDOBJ void " << getUmatFunctionName(md);
    CastemInterface::writeUMATFunctionArguments(os);
    os << ";\n\n"
       << "#ifdef __cplusplus\n"
       << "}\n"
       << "#endif /* __cplusplus */\n\n"
       << "#endif /* " << hg << " */\n";
  }  // end of writeHeaderFile

  void CastemModelInterface::writeSourceFile(const FileDescription& fd,
                                             const ModelDescription& md) {
    using namespace tfel::glossary;
    using tfel::material::ModellingHypothesis;
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto raise = [](const std::string& m) {
      tfel::raise("CastemModelInterface::writeSourceFile: " + m);
    };
    auto getVariablePosition = [&raise](const auto& variables, const auto& n,
                                        const bool skip_temperature) {
      auto temperature_found = false;
      const auto pv = std::find_if(
          variables.begin(), variables.end(),
          [&n, &raise, &temperature_found](const auto& v) {
            if (v.getTypeFlag() != SupportedTypes::SCALAR) {
              raise("unsupported type for variable '" + v.name + "'");
            }
            const auto found = v.name == n;
            if ((v.getExternalName() == Glossary::Temperature) && (!found)) {
              temperature_found = true;
            }
            return found;
          });
      if (pv == variables.end()) {
        raise("variable " + n + " not found");
      }
      if ((skip_temperature) && (temperature_found)) {
        return (pv - variables.begin()) - 1;
      }
      return pv - variables.begin();
    };  // end of getVariablePosition
    const auto name = md.library + md.className;
    const auto header = name + "-castem.hxx";
    const auto src = name + "-castem.cxx";
    const auto cn = md.className + "RealParametersInitializer";
    std::ofstream os("src/" + src);
    os.precision(14);
    if (!os) {
      raise("could not open file '" + src + "'");
    }
    os << "/*!\n"
       << " * \\file   " << src << '\n'
       << " * \\brief  This file declares the generic interface for the "
       << md.className << " model\n"
       << " * \\author " << fd.authorName << '\n'
       << " * \\date   " << fd.date << '\n'
       << " */\n\n"
       << "#include <cmath>\n"
       << "#include <locale>\n"
       << "#include <cstring>\n"
       << "#include <sstream>\n"
       << "#include \"TFEL/PhysicalConstants.hxx\"\n"
       << "#include \"TFEL/Config/TFELTypes.hxx\"\n"
       << "#include \"TFEL/Math/Array/View.hxx\"\n";
    if (useQuantities(md)) {
      os << "#include \"TFEL/Math/qt.hxx\"\n";
    }
    os << "#include \"TFEL/Material/BoundsCheck.hxx\"\n"
       << "#include \"MFront/CastemModel/" << header << "\"\n\n";
    //
    if (!md.includes.empty()) {
      os << md.includes << "\n\n";
    }
    //
    const auto mhs = ModellingHypothesis::getModellingHypotheses();
    const auto hypotheses =
        std::set<ModellingHypothesis::Hypothesis>{mhs.begin(), mhs.end()};
    const auto bd = convertToBehaviourDescription(md);
    CastemSymbolsGenerator sg;
    CastemInterface i;
    sg.generateGeneralSymbols(os, i, bd, fd, hypotheses, name);
    sg.generateSymbols(os, i, bd, fd, name, uh);
    const auto fn = getUmatFunctionName(md);
    exportUnsignedShortSymbol(os, fn + "_BehaviourType", 0u);
    exportUnsignedShortSymbol(os, fn + "_BehaviourKinematic", 0u);
    exportUnsignedShortSymbol(os, fn + "_Interface", 1u);
    exportUnsignedShortSymbol(os, fn + "_nMainVariables", 0u);
    exportUnsignedShortSymbol(os, fn + "_nGradients", 0u);
    exportArrayOfIntegersSymbol(os, fn + "_GradientsTypes", {});
    exportArrayOfStringsSymbol(os, fn + "_Gradients", {});
    exportUnsignedShortSymbol(os, fn + "_nThermodynamicForces", 0);
    exportArrayOfIntegersSymbol(os, fn + "_ThermodynamicForcesTypes", {});
    exportArrayOfStringsSymbol(os, fn + "_ThermodynamicForces", {""});
    //
    if (!md.sources.empty()) {
      os << md.sources << "\n\n";
    }
    //
    const auto has_constructor =
        (!md.constantMaterialProperties.empty()) || (hasRealParameters(md));
    //
    os << "namespace castem{\n\n";
    //
    if ((hasRealParameters(md)) &&
        (!areParametersTreatedAsStaticVariables(md))) {
      os << "struct " << cn << "{\n";
      writeScalarStandardTypedefs(os, md);
      os << '\n';
      os << "//! \\return the unique instance of the " << cn << " class\n"
         << "static " << cn << "& get(){\n"
         << "static " << cn << " parameters;\n"
         << "return parameters;\n"
         << "}\n\n";
      //
      for (const auto& p : md.parameters) {
        if (!isRealParameter(p)) {
          continue;
        }
        os << p.type << " " << p.name << " = " << p.type << "{"
           << p.getAttribute<double>(VariableDescription::defaultValue)
           << "};\n";
      }
      os << "private:\n"
         << cn << "() = default;\n"
         << "};\n\n";
    }
    //
    os << "struct " << md.className << "{\n\n";
    writeScalarStandardTypedefs(os, md);
    os << '\n';
    if (has_constructor) {
      os << md.className << "(";
      if (!md.constantMaterialProperties.empty()) {
        os << " const castem::CastemReal *const mfront_material_properties";
      }
      os << ")\n:";
      auto first = true;
      for (const auto& mp : md.constantMaterialProperties) {
        os << (first ? "" : ",\n");
        os << mp.name << "(mfront_material_properties["
           << getVariablePosition(md.constantMaterialProperties, mp.name, false)
           << "])";
        first = false;
      }
      for (const auto& p : md.parameters) {
        if (!isRealParameter(p)) {
          continue;
        }
        os << (first ? "" : ",\n");
        os << p.name << "(" << cn << "::get()." << p.name << ")";
        first = false;
      }
      os << "\n"
         << "{}\n\n";
    }
    for (const auto& f : md.functions) {
      const auto [require_temperature,
                  require_other_external_state_variable] = [&md, &f, &raise] {
        auto b1 = false;
        auto b2 = false;
        for (const auto& mv : f.usedVariables) {
          const auto [n, vdepth] = md.decomposeVariableName(mv);
          if (vdepth > 1) {
            raise("unsupported depth for variable '" + n + "'");
          }
          if (md.inputs.contains(n)) {
            const auto& v = md.inputs.getVariable(n);
            if (v.getExternalName() == Glossary::Temperature) {
              b1 = true;
            } else {
              b2 = true;
            }
          }
        }
        return std::make_pair(b1, b2);
      }();
      os << "void execute_" << f.name
         << "(castem::CastemReal *const mfront_STATEV,\n";
      if (f.useTimeIncrement) {
        os << "const castem::CastemReal *const mfront_DTIME,\n";
      } else {
        os << "const castem::CastemReal *const,\n";
      }
      if (require_temperature) {
        os << "const castem::CastemReal *const mfront_TEMP,\n "
           << "const castem::CastemReal *const mfront_DTEMP,\n ";
      } else {
        os << "const castem::CastemReal *const,\n "
           << "const castem::CastemReal *const,\n ";
      }
      if (require_other_external_state_variable) {
        os << "const castem::CastemReal *const mfront_PREDEF,\n "
           << "const castem::CastemReal *const mfront_DPRED) const {\n";
      } else {
        os << "const castem::CastemReal *const,\n "
           << "const castem::CastemReal *const) const {\n";
      }
      os << "using namespace std;\n"
         << "using namespace tfel::math;\n"
         << "using namespace tfel::material;\n";
      if (useQuantities(md)) {
        os << "using PhysicalConstants [[maybe_unused]] = "
           << "tfel::PhysicalConstants<NumericType, true>;\n";
      } else {
        os << "using PhysicalConstants [[maybe_unused]] = "
           << "tfel::PhysicalConstants<NumericType, false>;\n";
      }
      const auto policy = getDefaultOutOfBoundsPolicyAsString(md);
      os << "[[maybe_unused]] constexpr const auto policy = "
         << "tfel::material::" << policy << ";\n";
      if (f.useTimeIncrement) {
        os << "const auto dt = time{*mfront_DTIME};\n";
      }
      for (const auto& mv : f.modifiedVariables) {
        const auto& v = md.outputs.getVariable(mv);
        const auto pos = getVariablePosition(md.outputs, mv, false);
        os << "auto " << v.name << " = "  //
           << v.type << "{mfront_STATEV[" << pos << "]};\n";
      }
      for (const auto& mv : f.usedVariables) {
        const auto [n, vdepth] = md.decomposeVariableName(mv);
        if (vdepth > 1) {
          raise("unsupported depth for variable '" + n + "'");
        }
        if (md.inputs.contains(n)) {
          const auto& v = md.inputs.getVariable(n);
          const auto pos = getVariablePosition(md.inputs, n, true);
          if (v.getExternalName() == Glossary::Temperature) {
            if (vdepth == 1) {
              os << "const auto " << v.name << "_1 = " << v.type
                 << "{mfront_TEMP[" << pos << "]};\n";
            } else {
              os << "const auto " << v.name << " = " << v.type
                 << "{mfront_TEMP[" << pos << "] + "
                 << "mfront_DTEMP[" << pos << "]};\n";
            }
          } else {
            if (vdepth == 1) {
              os << "const auto " << v.name << "_1 = " << v.type
                 << "{mfront_PREDEF[" << pos << "]};\n";
            } else {
              os << "const auto " << v.name << " = " << v.type
                 << "{mfront_PREDEF[" << pos << "] + "
                 << "mfront_DPRED[" << pos << "]};\n";
            }
          }
        } else {
          const auto& v = md.outputs.getVariable(n);
          const auto pos = getVariablePosition(md.outputs, n, false);
          if (vdepth != 1) {
            raise("unsupported depth for variable '" + n + "'");
          }
          os << "const auto " << v.name << "_1 = " << v.type
             << "{mfront_STATEV[" << pos << "]};\n";
        }
      }
      if (!areRuntimeChecksDisabled(md)) {
        auto write_physical_bounds = [&os, &raise](
                                         const VariableDescription& v,
                                         const std::string& n,
                                         const unsigned short vdepth) {
          if (!v.hasPhysicalBounds()) {
            return;
          }
          if (!v.isScalar()) {
            raise("error while treating physical bounds of variable '" +
                  v.name + "' (only scalar variable are supported)");
          }
          writePhysicalBoundsChecks(os, v, n, "1u", false, false);
          if (vdepth == 1u) {
            writePhysicalBoundsChecks(os, v, v.name + "_1", "1u", false, false);
          }
        };
        for (const auto& mv : f.usedVariables) {
          const auto [n, vdepth] = md.decomposeVariableName(mv);
          if (md.outputs.contains(n)) {
            const auto& v = md.outputs.getVariable(n);
            write_physical_bounds(v, v.name, vdepth);
          }
          if (md.inputs.contains(n)) {
            const auto& v = md.inputs.getVariable(n);
            write_physical_bounds(v, v.name, vdepth);
          }
        }
        for (const auto& p : md.parameters) {
          write_physical_bounds(p, p.name, 0u);
        }
      }
      //
      if (!areRuntimeChecksDisabled(md)) {
        auto write_bounds = [&os, &raise](const VariableDescription& v,
                                          const std::string& n,
                                          const unsigned short vdepth) {
          if (!v.hasBounds()) {
            return;
          }
          if (!v.isScalar()) {
            raise("error while treating bounds of variable '" + v.name +
                  "' (only scalar variable are supported)");
          }
          writeBoundsChecks(os, v, n, "1u", "policy", false, false);
          if (vdepth == 1u) {
            writeBoundsChecks(os, v, v.name + "_1", "1u", "policy", false,
                              false);
          }
        };
        for (const auto& mv : f.usedVariables) {
          const auto [n, vdepth] = md.decomposeVariableName(mv);
          if (md.outputs.contains(n)) {
            const auto& v = md.outputs.getVariable(n);
            write_bounds(v, v.name, vdepth);
          }
          if (md.inputs.contains(n)) {
            const auto& v = md.inputs.getVariable(n);
            write_bounds(v, v.name, vdepth);
          }
        }
        for (const auto& p : md.parameters) {
          write_bounds(p, p.name, 0u);
        }
      }
      //
      os << f.body << '\n';
      for (const auto& mv : f.modifiedVariables) {
        const auto [n, vdepth] = md.decomposeVariableName(mv);
        const auto& v = md.outputs.getVariable(mv);
        const auto pos = getVariablePosition(md.outputs, mv, false);
        os << "tfel::math::map<" << v.type << ">(mfront_STATEV[" << pos
           << "]) = " << v.name << ";\n";
        static_cast<void>(vdepth);
      }
      os << "} // end of execute_" << f.name << "\n\n";
    }
    os << "private:\n";
    if (!md.members.empty()) {
      os << md.members << "\n\n";
    }
    if (!md.privateCode.empty()) {
      os << md.privateCode << "\n\n";
    }
    // material properties
    for (const auto& mp : md.constantMaterialProperties) {
      if (!getDebugMode()) {
        os << "#line " << mp.lineNumber << " \"" << fd.fileName << "\"\n";
      }
      os << "const " << mp.type << " " << mp.name << ";\n";
    }
    // parameters
    for (const auto& p : md.parameters) {
      if (!getDebugMode()) {
        os << "#line " << p.lineNumber << " \"" << fd.fileName << "\"\n";
      }
      if ((isRealParameter(p)) &&
          (!areParametersTreatedAsStaticVariables(md))) {
        os << "const " << p.type << " " << p.name << ";\n";
        continue;
      }
      if (p.type == "string") {
        os << "const std::string " << p.name << " = "
           << p.getAttribute<std::string>(VariableDescription::defaultValue);
      } else if (isRealParameter(p)) {
        os << "static constexpr auto " << p.name << " = " << p.type << "{"
           << p.getAttribute<double>(VariableDescription::defaultValue) << "}";
      } else if (p.type == "bool") {
        os << "static constexpr " << p.type << " " << p.name << " = "
           << p.getAttribute<bool>(VariableDescription::defaultValue);
      } else {
        raise("parameter type '" + p.type + "' is not supported.\n");
      }
      os << ";\n";
    }
    // static variables
    for (const auto& v : md.staticVars) {
      if (!getDebugMode()) {
        os << "#line " << v.lineNumber << " \"" << fd.fileName << "\"\n";
      }
      os << "static constexpr " << v.type << " " << v.name << " = "  //
         << v.value << ";\n";
    }
    os << "}; // end of struct " << md.className << "\n\n";
    //

    os << "static int " << name << "_implementation("
       << "       castem::CastemReal *const mfront_STATEV,\n"
       << "       castem::CastemReal *const mfront_PNEWDT,\n"
       << " const castem::CastemReal *const mfront_DTIME,\n"
       << " const castem::CastemReal *const mfront_TEMP,\n"
       << " const castem::CastemReal *const mfront_DTEMP,\n"
       << " const castem::CastemReal *const mfront_PREDEF,\n"
       << " const castem::CastemReal *const mfront_DPRED,\n";
    if (!md.constantMaterialProperties.empty()) {
      os << " const castem::CastemReal *const mfront_PROPS){\n";
    } else {
      os << " const castem::CastemReal *const){\n";
    }
    os << "try{\n";
    if (!md.constantMaterialProperties.empty()) {
      os << "const " << md.className << " m(mfront_PROPS);\n";
    } else {
      os << "const " << md.className << " m;\n";
    }
    for (const auto& f : md.functions) {
      os << "m.execute_" << f.name
         << "(mfront_STATEV, mfront_DTIME, mfront_TEMP, mfront_DTEMP, "
         << "mfront_PREDEF, mfront_DPRED);\n";
    }
    os << "} catch(...){\n"
       << "*mfront_PNEWDT = 0.1;\n"
       << "return -1;"
       << "}\n"
       << "return 1;\n"
       << "}\n\n";
    os << "} // end of namespace castem\n\n";
    //
    if ((hasRealParameters(md)) &&
        (!areParametersTreatedAsStaticVariables(md))) {
      os << "MFRONT_SHAREDOBJ int\n"
         << name << "_setParameter(const char *const n, const double v){\n";
      writeScalarStandardTypedefs(os, md);
      os << "auto& parameters = castem::" << cn << "::get();\n";
      auto first = true;
      for (const auto& p : md.parameters) {
        if (!isRealParameter(p)) {
          continue;
        }
        const auto en = p.getExternalName();
        if (en != p.name) {
          os << (first ? "if" : "} else if")  //
             << "(std::strcmp(\"" << en << "\", n) == 0){\n"
             << "parameters." << p.name << " = " << p.type << "{v};\n"
             << "return 1;\n";
          first = false;
        }
        if ((!p.symbolic_form.empty()) && (p.symbolic_form != p.name)) {
          os << (first ? "if" : "} else if")  //
             << "(std::strcmp(\"" << p.symbolic_form << "\", n) == 0){\n"
             << "parameters." << p.name << " = " << p.type << "{v};\n"
             << "return 1;\n";
          first = false;
        }
        os << (first ? "if" : "} else if")  //
           << "(std::strcmp(\"" << p.name << "\", n) == 0){\n"
           << "parameters." << p.name << " = " << p.type << "{v};\n"
           << "return 1;\n";
        first = false;
      }
      os << "}\n"
         << "return 0;\n"
         << "} // end of " << name << "_setParameter\n\n";
    }
    //
    const auto& bdata =
        bd.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    os << "void " << fn;
    CastemModelInterface::writeUMATFunctionArguments(os);
    os << "{\n";
    // checks on material properties
    const auto mps_size =
        getTypeSize(bdata.getMaterialProperties()).getValueForDimension(3);
    os << "if (" << mps_size << " != *NPROPS){\n"
       << "std::cerr << \"" << md.className
       << ": invalid number of material properties \"";
    if (mps_size == 0) {
      os << " << \"(no material property expected, ";
    } else {
      os << " << \"(" << mps_size << " material properties expected, ";
    }
    os << "\" << *NPROPS << \" given)\\n\";\n"
       << "*KINC = -1;\n"
       << "return;\n"
       << "}\n";
    // checks on internal state variables
    const auto isvs_size =
        getTypeSize(bdata.getPersistentVariables()).getValueForDimension(3);
    os << "if (" << isvs_size << " != *NSTATV){\n"
       << "std::cerr << \"" << md.className
       << ": invalid number of state variables \"";
    if (isvs_size == 0) {
      os << " << \"(no state variable expected, ";
    } else {
      os << " << \"(" << isvs_size << " state variables expected, ";
    }
    os << "\" << *NSTATV << \" given)\\n\";\n"
       << "*KINC = -1;\n"
       << "return;\n"
       << "}\n";
    // call the model
    os << "*KINC = castem::" << name
       << "_implementation(STATEV, PNEWDT, DTIME, TEMP, DTEMP, PREDEF, "
       << "DPRED, PROPS);\n"
       << "}\n\n";
  }  // end of writeSourceFile

  void CastemModelInterface::getTargetsDescription(TargetsDescription& td,
                                                   const ModelDescription& md) {
    using tfel::material::ModellingHypothesis;
    const auto lib = this->getLibraryName(md);
    const auto name = md.library + md.className;
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto& l = td.getLibrary(lib);
    insert_if(l.cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
#ifdef CASTEM_CPPFLAGS
    insert_if(l.cppflags, CASTEM_CPPFLAGS);
#endif /* CASTEM_CPPFLAGS */
#ifndef LOCAL_CASTEM_HEADER
#ifdef CASTEM_ROOT
    const auto* castem_root = ::getenv("CASTEM_ROOT");
    if (castem_root != nullptr) {
      insert_if(l.include_directories, std::string(castem_root) + "/include");
    } else {
      insert_if(l.include_directories, std::string(CASTEM_ROOT) + "/include");
    }
#else  /* CASTEM_ROOT */
    if (castem_root != 0) {
      insert_if(l.include_directories, std::string(castem_root) + "/include");
    }
#endif /* CASTEM_ROOT */
#endif /* LOCAL_CASTEM_HEADER_FILE */
    insert_if(l.include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(l.sources, name + "-castem.cxx");
    td.headers.push_back("MFront/CastemModel/" + name + "-castem.hxx");
    insert_if(l.link_directories,
              "$(shell " + tfel_config + " --library-path)");
    insert_if(l.link_libraries, "$(shell " + tfel_config +
                                    " --library-dependency "
                                    "--material --mfront-profiling)");
    insert_if(l.epts, getUmatFunctionName(md));
  }  // end of getTargetsDescription

  CastemModelInterface::~CastemModelInterface() = default;

}  // end of namespace mfront
