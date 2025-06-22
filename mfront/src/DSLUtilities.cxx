/*!
 * \file   mfront/src/DSLUtilities.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   23 oct 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <utility>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Config/GetTFELVersion.h"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/MaterialKnowledgeDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyParametersHandler.hxx"
#include "MFront/SymbolsGenerator.hxx"

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  std::vector<std::string> getScalarTypeAliases() {
    return {"numeric_type",
            "real",
            "time",
            "length",
            "frequency",
            "speed",
            "stress",
            "strain",
            "strainrate",
            "stressrate",
            "temperature",
            "thermalexpansion",
            "thermalconductivity",
            "massdensity",
            "energydensity"};
  }  // end of getScalarTypeAliases

  std::vector<std::string> getTinyVectorTypeAliases() {
    return {"TVector", "DisplacementTVector", "ForceTVector", "HeatFlux",
            "TemperatureGradient"};
  }  // end of getTinyVectorTypeAliases

  std::vector<std::string> getStensorTypeAliases() {
    return {"Stensor",       "StressStensor",     "StressRateStensor",
            "StrainStensor", "StrainRateStensor", "FrequencyStensor"};
  }

  std::vector<std::string> getTensorTypeAliases() {
    return {"Tensor", "DeformationGradientTensor",
            "DeformationGradientRateTensor", "StressTensor"};
  }

  std::vector<std::string> getST2toST2TypeAliases() {
    return {"StiffnessTensor", "Stensor4"};
  }

  std::vector<std::string> getTypeAliases() {
    auto aliases = getScalarTypeAliases();
    auto append = [&aliases](const std::vector<std::string>& others) {
      aliases.insert(aliases.end(), others.begin(), others.end());
    };
    append(getTinyVectorTypeAliases());
    append(getStensorTypeAliases());
    append(getTensorTypeAliases());
    append(getST2toST2TypeAliases());
    return aliases;
  }  // end of getTypeAliases

  void exportStringSymbol(std::ostream& os,
                          const std::string_view n,
                          const std::string_view v) {
    exportSymbol(os, "const char*", n, '"' + std::string{v} + '"');
  }  // end of exportStringSymbol

  void exportUnsignedShortSymbol(std::ostream& os,
                                 const std::string_view n,
                                 const unsigned short v) {
    std::ostringstream value;
    value << v;
    exportSymbol(os, "unsigned short", n, value.str() + "u");
  }  // end of exportUnsignedShortSymbol

  void writeParametersSymbols(std::ostream& os,
                              const std::string_view n,
                              const MaterialPropertyDescription& mpd) {
    if (areParametersTreatedAsStaticVariables(mpd)) {
      writeParametersDeclarationSymbols(os, n, {});
      return;
    }
    writeParametersDeclarationSymbols(os, n, mpd.parameters);
    writeParametersDefaultValuesSymbols(os, n, mpd.parameters);
    writeBoundsSymbols(os, n, mpd.parameters);
    writePhysicalBoundsSymbols(os, n, mpd.parameters);
  }  // end of writeParametersSymbols

  void writeParametersDeclarationSymbols(
      std::ostream& os,
      const std::string_view n,
      const VariableDescriptionContainer& parameters) {
    exportUnsignedShortSymbol(os, std::string{n} + "_nParameters",
                              parameters.getNumberOfVariables());
    exportArrayOfStringsSymbol(os, std::string{n} + "_Parameters",
                               parameters.getExternalNames());
    if (parameters.empty()) {
      exportSymbol(os, "const int *", std::string{n} + "_ParametersTypes",
                   "nullptr");
      return;
    }
    const auto size = [&parameters] {
      auto s = std::size_t{};
      for (const auto& p : parameters) {
        s += p.arraySize;
      }
      return s;
    }();
    os << "MFRONT_EXPORT_ARRAY_OF_SYMBOLS(int, " << n << "_ParametersTypes, "
       << size << ", MFRONT_EXPORT_ARRAY_ARGUMENTS(";
    for (auto p = parameters.begin(); p != parameters.end();) {
      for (unsigned short is = 0; is != p->arraySize;) {
        if (p->type == "int") {
          os << "1";
        } else if (p->type == "ushort") {
          os << "2";
        } else {
          const auto f = SupportedTypes::getTypeFlag(p->type);
          tfel::raise_if(f != SupportedTypes::SCALAR,
                         "SymbolsGenerator::writeParametersSymbols: "
                         "internal error, unsupported type "
                         "for parameter '" +
                             p->name + "'");
          os << "0";
        }
        if (++is != p->arraySize) {
          os << ",";
        }
      }
      if (++p != parameters.end()) {
        os << ",";
      }
    }
    os << "));\n\n";
  }  // end of writeParametersDeclarationSymbols

  void writeParametersDefaultValuesSymbols(
      std::ostream& os,
      const std::string_view n,
      const VariableDescriptionContainer& parameters) {
    const auto prec = os.precision();
    for (const auto& p : parameters) {
      const auto f = SupportedTypes::getTypeFlag(p.type);
      if (f != SupportedTypes::SCALAR) {
        tfel::raise(
            "writeParameterDefaultValueSymbols: "
            "unsupported paramaeter type '" +
            p.type + "'");
      }
      os.precision(14);
      if (p.arraySize != 1u) {
        tfel::raise(
            "writeParameterDefaultValueSymbols: "
            "array of parameters is not supported");
      }
      exportSymbol(
          os, "double",
          std::string{n} + "_" + p.getExternalName() + "_ParameterDefaultValue",
          p.getAttribute<double>(VariableDescription::defaultValue));
      os.precision(prec);
    }
  }  // end of writeParametersDefaultValuesSymbols

  void exportArrayOfIntegersSymbol(std::ostream& os,
                                   const std::string_view s,
                                   const std::vector<int>& values) {
    if (values.empty()) {
      exportSymbol(os, "const int *", s, "nullptr");
      return;
    }
    auto i = decltype(values.size()){};
    auto p = values.begin();
    os << "MFRONT_EXPORT_ARRAY_OF_SYMBOLS(int, " << s << ", " << values.size()
       << ", MFRONT_EXPORT_ARRAY_ARGUMENTS(";
    while (p != values.end()) {
      os << *p;
      if (++p != values.end()) {
        if (i % 5 == 0) {
          os << ",\n";
        } else {
          os << ",";
        }
      }
      ++i;
    }
    os << "));\n\n";
  }  // end of exportArrayOfIntegersSymbol

  void exportArrayOfStringsSymbol(std::ostream& os,
                                  const std::string_view s,
                                  const std::vector<std::string>& values) {
    if (values.empty()) {
      exportSymbol(os, "const char * const *", s, "nullptr");
      return;
    }
    auto i = decltype(values.size()){};
    auto p = values.begin();
    os << "MFRONT_EXPORT_ARRAY_OF_SYMBOLS(const char *, " << s << ", "
       << values.size() << ", MFRONT_EXPORT_ARRAY_ARGUMENTS(";
    while (p != values.end()) {
      os << '"' << *p << '"';
      if (++p != values.end()) {
        if (i % 5 == 0) {
          os << ",\n";
        } else {
          os << ",";
        }
      }
      ++i;
    }
    os << "));\n\n";
  }  // end of exportArrayOfStringsSymbol

  void writeVariablesNamesSymbol(
      std::ostream& out,
      const std::string_view name,
      const mfront::MaterialPropertyDescription& mpd) {
    exportStringSymbol(out, std::string{name} + "_output",
                       mpd.output.getExternalName());
    exportUnsignedShortSymbol(out, std::string{name} + "_nargs",
                              mpd.inputs.size());
    if (!mpd.inputs.empty()) {
      out << "MFRONT_EXPORT_ARRAY_OF_SYMBOLS(const char *, " << name
          << "_args, " << mpd.inputs.size()
          << ", MFRONT_EXPORT_ARRAY_ARGUMENTS(";
      for (auto p3 = mpd.inputs.begin(); p3 != mpd.inputs.end();) {
        const auto iname = '\"' + p3->getExternalName() + '\"';
        out << iname;
        if (++p3 != mpd.inputs.end()) {
          out << ",";
        }
      }
      out << "));\n\n";
    }
  }  // end of writeVariableNamesSymbol

  void writeBoundsSymbol(std::ostream& out,
                         const std::string_view n,
                         const std::string_view vn,
                         const std::string_view bt,
                         const VariableBoundsDescription& b) {
    if ((b.boundsType == VariableBoundsDescription::LOWER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      out << "MFRONT_EXPORT_SYMBOL(long double, "  //
          << n << "_" << vn << "_"
          << "Lower" << bt << "Bound, "  //
          << "static_cast<long double>(" << b.lowerBound << "));\n\n";
    }
    if ((b.boundsType == VariableBoundsDescription::UPPER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      out << "MFRONT_EXPORT_SYMBOL(long double, "  //
          << n << "_" << vn << "_"
          << "Upper" << bt << "Bound, "  //
          << "static_cast<long double>(" << b.upperBound << "));\n\n";
    }
  }  // end of writeBoundsSymbol

  void writeBoundsSymbols(std::ostream& os,
                          const std::string_view n,
                          const VariableDescriptionContainer& vc) {
    const auto prec = os.precision();
    os.precision(14);
    for (const auto& v : vc) {
      if (v.arraySize == 1u) {
        if (!v.hasBounds()) {
          continue;
        }
        mfront::writeBoundsSymbol(os, n, v.getExternalName(), "",
                                  v.getBounds());
      } else {
        for (auto idx = 0; idx != v.arraySize; ++idx) {
          if (!v.hasBounds(idx)) {
            continue;
          }
          mfront::writeBoundsSymbol(os, n,
                                    v.getExternalName() + "_mfront_index_" +
                                        std::to_string(idx) + "_",
                                    "", v.getBounds(idx));
        }
      }
    }
    os.precision(prec);
  }  // end of writeBoundsSymbols

  void writePhysicalBoundsSymbols(std::ostream& os,
                                  const std::string_view n,
                                  const VariableDescriptionContainer& vc) {
    const auto prec = os.precision();
    os.precision(14);
    for (const auto& v : vc) {
      if (v.arraySize == 1u) {
        if (!v.hasPhysicalBounds()) {
          continue;
        }
        mfront::writeBoundsSymbol(os, n, v.getExternalName(), "Physical",
                                  v.getPhysicalBounds());
      } else {
        for (auto idx = 0; idx != v.arraySize; ++idx) {
          if (!v.hasPhysicalBounds(idx)) {
            continue;
          }
          mfront::writeBoundsSymbol(os, n,
                                    v.getExternalName() + "_mfront_index_" +
                                        std::to_string(idx) + "_",
                                    "Physical", v.getPhysicalBounds(idx));
        }
      }
    }
    os.precision(prec);
  }  // end of writePhysicalBoundsSymbols

  void writeVariablesBoundsSymbols(
      std::ostream& out,
      const std::string_view name,
      const mfront::MaterialPropertyDescription& mpd) {
    const auto prec = out.precision();
    out.precision(14);
    for (const auto& v : mpd.inputs) {
      if (v.arraySize == 1u) {
        if (!v.hasBounds()) {
          continue;
        }
        writeBoundsSymbol(out, name, v.getExternalName(), "", v.getBounds());
        if (!v.hasPhysicalBounds()) {
          continue;
        }
        writeBoundsSymbol(out, name, v.getExternalName(), "Physical",
                          v.getPhysicalBounds());
      } else {
        for (auto idx = 0; idx != v.arraySize; ++idx) {
          if (!v.hasBounds(idx)) {
            continue;
          }
          writeBoundsSymbol(out, name,
                            v.getExternalName() + "_mfront_index_" +
                                std::to_string(idx) + "_",
                            "", v.getBounds(idx));
          if (!v.hasPhysicalBounds(idx)) {
            continue;
          }
          writeBoundsSymbol(out, name,
                            v.getExternalName() + "_mfront_index_" +
                                std::to_string(idx) + "_",
                            "Physical", v.getPhysicalBounds(idx));
        }
      }
    }
    out.precision(prec);
  }  // end of writeVariablesBoundsSymbols

  void writeFileDescriptionSymbols(std::ostream& os,
                                   const std::string_view n,
                                   const FileDescription& fd) {
    auto description = [&fd] {
      auto rtrim = [](const std::string_view s) {
        auto end = s.find_last_not_of(" \n\r\t\f\v");
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
      };
      auto transform = [rtrim](const std::string_view s) {
        return tfel::utilities::replace_all(rtrim(s), "\\", "\\\\");
      };
      auto r = std::string{};
      auto lines = std::vector<std::string>{};
      std::istringstream d(fd.description);
      for (std::string line; std::getline(d, line);) {
        if (!r.empty()) {
          r += "\\n";
        }
        if (tfel::utilities::starts_with(line, "* ")) {
          r += transform(line.substr(2));
        } else {
          r += transform(rtrim(line));
        }
      }
      return r;
    }();
    exportStringSymbol(os, std::string{n} + "_author", fd.authorName);
    exportStringSymbol(os, std::string{n} + "_date", fd.date);
    exportStringSymbol(os, std::string{n} + "_description", description);
  }  // end of writeFileDescriptionSymbols

  void writeBuildIdentifierSymbol(std::ostream& os,
                                  const std::string_view n,
                                  const MaterialKnowledgeDescription& d) {
    const auto build_id = [&d]() -> std::string {
      const auto* const bid = std::getenv("TFEL_BUILD_ID");
      if (bid != nullptr) {
        return bid;
      }
      return d.getAttribute<std::string>(
          MaterialKnowledgeDescription::buildIdentifier, "");
    }();
    exportStringSymbol(os, std::string{n} + "_build_id", build_id);
  }  // end of writeBuildIdentifierSymbols

  void writeEntryPointSymbol(std::ostream& out, const std::string_view n) {
    writeEntryPointSymbol(out, n, n);
  }  // end of writeEntryPointSymbols

  void writeEntryPointSymbol(std::ostream& out,
                             const std::string_view n,
                             const std::string_view n2) {
    exportStringSymbol(out, std::string{n} + "_mfront_ept", n2);
  }  // end of writeEntryPointSymbols

  void writeTFELVersionSymbol(std::ostream& out, const std::string_view n) {
    exportStringSymbol(out, std::string{n} + "_tfel_version",
                       ::getTFELVersion());
  }  // end of writeTFELVersionSymbol

  void writeUnitSystemSymbol(std::ostream& out,
                             const std::string_view n,
                             const MaterialKnowledgeDescription& d) {
    if (d.hasUnitSystem()) {
      exportStringSymbol(out, std::string{n} + "_unit_system",
                         d.getUnitSystem());
    } else {
      exportStringSymbol(out, std::string{n} + "_unit_system", "");
    }
  }  // end of writeUnitSystemSymbol

  void writeInterfaceSymbol(std::ostream& out,
                            const std::string_view n,
                            const std::string_view i) {
    exportStringSymbol(out, std::string{n} + "_mfront_interface", i);
  }  // end of writeInterfaceSymbol

  void writeLawSymbol(std::ostream& out,
                      const std::string_view n,
                      const std::string_view l) {
    exportStringSymbol(out, std::string{n} + "_mfront_law", l);
  }  // end of writeLawSymbol

  void writeMaterialSymbol(std::ostream& out,
                           const std::string_view n,
                           const std::string_view m) {
    if (!m.empty()) {
      exportStringSymbol(out, std::string{n} + "_mfront_material", m);
    }
  }  // end of writeMaterialSymbol

  void writeMaterialKnowledgeTypeSymbol(std::ostream& out,
                                        const std::string_view n,
                                        const MaterialKnowledgeType& t) {
    const auto s = [&t] {
      if (t == MATERIALPROPERTY) {
        return 0;
      } else if (t == BEHAVIOUR) {
        return 1;
      } else if (t == MODEL) {
        return 2;
      } else {
        tfel::raise(
            "writeMaterialKnowledgeTypeSymbol: "
            "internal error, (unsupported material knowledge type)");
      }
    }();
    exportUnsignedShortSymbol(out, std::string{n} + "_mfront_mkt", s);
  }  // end of writeMaterialKnowledgeTypeSymbol

  void writeMaterialLaws(std::ostream& os,
                         const std::vector<std::string>& materialLaws) {
    for (const auto& l : materialLaws) {
      os << "using mfront::" << l << ";\n"
         << "using mfront::" << l << "_checkBounds;\n";
    }
  }  // end of writeMaterialLaws

  void writeStaticVariables(std::ostream& os,
                            const StaticVariableDescriptionContainer& vc,
                            const std::string_view f) {
    if (vc.empty()) {
      return;
    }
    for (const auto& v : vc) {
      if (getDebugMode()) {
        os << "#line " << v.lineNumber << " \"" << f << "\"\n";
      }
      if (v.type == "short") {
        os << "static " << constexpr_c << "  short " << v.name << " = "
           << static_cast<short>(v.value) << ";\n";
      } else if (v.type == "ushort") {
        os << "static " << constexpr_c << "  unsigned short " << v.name << " = "
           << static_cast<unsigned short>(v.value) << ";\n";
      } else if (v.type == "int") {
        os << "static " << constexpr_c << "  int " << v.name << " = "
           << static_cast<int>(v.value) << ";\n";
      } else if (v.type == "uint") {
        os << "static " << constexpr_c << "  unsigned int " << v.name << " = "
           << static_cast<unsigned int>(v.value) << ";\n";
      } else if (v.type == "long") {
        os << "static " << constexpr_c << "  long " << v.name << " = "
           << static_cast<long>(v.value) << ";\n";
      } else if (v.type == "ulong") {
        os << "static " << constexpr_c << "  unsigned long " << v.name << " = "
           << static_cast<unsigned long>(v.value) << ";\n";
      } else if (v.type == "float") {
        os << "static " << constexpr_c << "  float " << v.name << " = "
           << static_cast<float>(v.value) << ";\n";
      } else if ((v.type == "double") || (v.type == "real")) {
        os << "static " << constexpr_c << "  double " << v.name << " = "
           << static_cast<double>(v.value) << ";\n";
      } else if (v.type == "ldouble") {
        os << "static " << constexpr_c << "  long double " << v.name << " = "
           << static_cast<long double>(v.value) << ";\n";
      } else {
        std::ostringstream msg;
        msg << "writeStaticVariables: "
            << "type '" + v.type << "' of variable '" << v.name
            << "' defined at line '" << v.lineNumber << "' "
            << "is not a supported type for a static variable."
            << "Supported types are short, ushort, int, uint, long, ulong,"
            << "float, double and ldouble.";
        tfel::raise(msg.str());
      }
    }
    os << '\n';
  }  // end of writeStaticVariables

  std::string getMaterialLawLibraryNameBase(
      const MaterialPropertyDescription& mpd) {
    const auto material = mpd.material;
    const auto library = mpd.library;
    if (library.empty()) {
      if (material.empty()) {
        return "MaterialLaw";
      }
      return material;
    }
    return library;
  }  // end of getMaterialLawLibraryNameBase

  void writeF77FUNCMacros(std::ostream& f) {
    auto def = [&f] {
      f << "#ifndef F77_FUNC\n"
        << "#define F77_FUNC(X,Y) X##_\n"
        << "#endif\n"
        << "#ifndef F77_FUNC_\n"
        << "#define F77_FUNC_(X,Y) X##_\n"
        << "#endif\n";
    };
    f << "#if (defined GNU_FORTRAN_COMPILER)\n";
    def();
    f << "#elif (defined INTEL_FORTRAN_COMPILER)\n"
      << "#ifdef _WIN32\n"
      << "#ifndef F77_FUNC\n"
      << "#define F77_FUNC(X,Y) Y\n"
      << "#endif\n"
      << "#ifndef F77_FUNC_\n"
      << "#define F77_FUNC_(X,Y) Y\n"
      << "#endif\n"
      << "#else\n";
    def();
    f << "#endif /* _WIN32 */\n"
      << "#else\n";
    def();
    f << "#endif\n\n";
  }  // end of writeF77FuncMacros

  void writeExportDirectives(std::ostream& os, const bool b) {
    os << "#ifdef _WIN32\n"
       << "#ifndef NOMINMAX\n"
       << "#define NOMINMAX\n"
       << "#endif /* NOMINMAX */\n"
       << "#include <windows.h>\n"
       << "#ifdef small\n"
       << "#undef small\n"
       << "#endif /* small */\n"
       << "#endif /* _WIN32 */\n\n";
    if (b) {
      os << "#ifdef _WIN32\n"
         << "#ifndef MFRONT_SHAREDOBJ\n"
         << "#ifdef  MFRONT_COMPILING\n"
         << "#define MFRONT_SHAREDOBJ __declspec(dllexport)\n"
         << "#else /* MFRONT_COMPILING */\n"
         << "#define MFRONT_SHAREDOBJ __declspec(dllimport)\n"
         << "#endif /* MFRONT_COMPILING */\n"
         << "#endif /* MFRONT_SHAREDOBJ */\n"
         << "#else\n"
         << "#ifndef MFRONT_SHAREDOBJ\n"
         << "#ifdef __GNUC__\n"
         << "#define MFRONT_SHAREDOBJ "
         << "__attribute__((visibility(\"default\")))\n"
         << "#else\n"
         << "#define MFRONT_SHAREDOBJ\n"
         << "#endif /* __GNUC__ */\n"
         << "#endif /* MFRONT_SHAREDOBJ */\n"
         << "#endif /* _WIN32 */\n\n";
    } else {
      os << "#ifndef MFRONT_SHAREDOBJ\n"
         << "#define MFRONT_SHAREDOBJ TFEL_VISIBILITY_EXPORT\n"
         << "#endif /* MFRONT_SHAREDOBJ */\n\n";
    }
    os << "#ifndef MFRONT_EXPORT_SYMBOL\n"
       << "#define MFRONT_EXPORT_SYMBOL(TYPE, NAME, VALUE) \\\n"
       << "  MFRONT_SHAREDOBJ extern TYPE NAME;            \\\n"
       << "  MFRONT_SHAREDOBJ TYPE NAME = VALUE\n"
       << "#endif /* MFRONT_EXPORT_SYMBOL*/\n\n"
       << "#ifndef MFRONT_EXPORT_ARRAY_ARGUMENTS\n"
       << "#define MFRONT_EXPORT_ARRAY_ARGUMENTS(...) __VA_ARGS__\n"
       << "#endif /* MFRONT_EXPORT_ARRAY_ARGUMENTS */\n\n"
       << "#ifndef MFRONT_EXPORT_ARRAY_OF_SYMBOLS\n"
       << "#define MFRONT_EXPORT_ARRAY_OF_SYMBOLS(TYPE, NAME, SIZE, VALUE) \\\n"
       << "  MFRONT_SHAREDOBJ extern TYPE NAME[SIZE];                      \\\n"
       << "  MFRONT_SHAREDOBJ TYPE NAME[SIZE] = {VALUE}\n"
       << "#endif /* MFRONT_EXPORT_ARRAY_OF_SYMBOLS*/\n\n";
  }  // end of writeExportDirectives

  std::string makeUpperCase(const std::string_view n) {
    std::string s(n);
    auto p = n.begin();
    auto p2 = s.begin();
    for (; p != n.end(); ++p, ++p2) {
      *p2 = static_cast<char>(toupper(*p));
    }
    return s;
  }  // end of makeUpperCase

  std::string makeLowerCase(const std::string_view n) {
    std::string s(n);
    auto p = n.begin();
    auto p2 = s.begin();
    for (; p != n.end(); ++p, ++p2) {
      *p2 = static_cast<char>(tolower(*p));
    }
    return s;
  }  // end of makeLowerCase

  void displayGlossaryEntryCompleteDescription(
      std::ostream& os, const tfel::glossary::GlossaryEntry& e) {
    const auto& k = e.getKey();
    const auto& n = e.getNames();
    const auto& sd = e.getShortDescription();
    const auto& d = e.getDescription();
    if ((!sd.empty()) || (!d.empty())) {
      os << '\n'
         << "For your information, the description of the glossary entry '" << k
         << "' is:\n";
      if (!n.empty()) {
        os << k << ":";
        for (const auto& elem : n) {
          os << " '" << elem << "'";
        }
        os << '\n';
      }
      if (!sd.empty()) {
        os << sd << '\n';
      }
      if (!d.empty()) {
        for (const auto& elem : d) {
          os << elem << '\n';
        }
      }
    }
  }  // end of displayGlossaryEntryCompleteDescription

  void addSymbol(std::map<std::string, std::string>& symbols,
                 const std::string_view s,
                 const std::string_view r) {
    const auto sname = std::string{s};
    if (symbols.find(sname) != symbols.end()) {
      if (symbols.at(sname) != r) {
        tfel::raise("addSymbol: symbol '" + sname +
                    "' has multiple replacement strings ('" +
                    symbols.at(sname) + "' and '" + std::string{r} + "')");
      }
    }
    symbols.insert({sname, std::string{r}});
  }  // end of addSymbol

  static void writeBoundsChecks(std::ostream& os,
                                const VariableDescription& v,
                                const std::string_view n,
                                const std::string_view space_dimension,
                                const std::string_view policy,
                                const bool addThis,
                                const bool checkEndOfTimeStepValue,
                                const bool physicalBounds) {
    if (physicalBounds) {
      if (!v.hasPhysicalBounds()) {
        return;
      }
    } else {
      if (!v.hasBounds()) {
        return;
      }
    }
    const auto& bounds = physicalBounds ? v.getPhysicalBounds() : v.getBounds();
    const auto numeric_type = [&v] {
      if (v.isScalar()) {
        return v.type;
      }
      return "tfel::math::numeric_type<" + v.type + ">";
    }();
    const auto this_pointer = addThis ? "this->" : "";
    const auto policy_argument = [&physicalBounds, &policy]() -> std::string {
      if (!physicalBounds) {
        return ", " + std::string{policy};
      }
      return "";
    }();
    if (bounds.boundsType == VariableBoundsDescription::LOWER) {
      os << "tfel::material::BoundsCheck<" << space_dimension
         << ">::lowerBoundCheck(\"" << n << "\", " << this_pointer << n << ","
         << "static_cast<" << numeric_type << ">(" << bounds.lowerBound << ")"
         << policy_argument << ");\n";
      if (checkEndOfTimeStepValue) {
        os << "tfel::material::BoundsCheck<" << space_dimension
           << ">::lowerBoundCheck(\"" << n << "+d" << n << "\", "
           << this_pointer << n << "+this->d" << n << ","
           << "static_cast<" << numeric_type << ">(" << bounds.lowerBound << ")"
           << policy_argument << ");\n";
      }
    } else if (bounds.boundsType == VariableBoundsDescription::UPPER) {
      os << "tfel::material::BoundsCheck<" << space_dimension
         << ">::upperBoundCheck(\"" << n << "\", " << this_pointer << n << ","
         << "static_cast<" << numeric_type << ">(" << bounds.upperBound << ")"
         << policy_argument << ");\n";
      if (checkEndOfTimeStepValue) {
        os << "tfel::material::BoundsCheck<" << space_dimension
           << ">::upperBoundCheck(\"" << n << "+d" << n << "\", "
           << this_pointer << n << "+this->d" << n << ","
           << "static_cast<" << numeric_type << ">(" << bounds.upperBound << ")"
           << policy_argument << ");\n";
      }
    } else if (bounds.boundsType == VariableBoundsDescription::LOWERANDUPPER) {
      os << "tfel::material::BoundsCheck<" << space_dimension
         << ">::lowerAndUpperBoundsChecks(\"" << n << "\", " << this_pointer
         << n << ","
         << "static_cast<" << numeric_type << ">(" << bounds.lowerBound << "),"
         << "static_cast<" << numeric_type << ">(" << bounds.upperBound << ")"
         << policy_argument << ");\n";
      if (checkEndOfTimeStepValue) {
        os << "tfel::material::BoundsCheck<" << space_dimension
           << ">::lowerAndUpperBoundsChecks(\"" << n << "+d" << n << "\", "
           << this_pointer << n << "+this->d" << n << ","
           << "static_cast<" << numeric_type << ">(" << bounds.lowerBound
           << "),"
           << "static_cast<" << numeric_type << ">(" << bounds.upperBound << ")"
           << policy_argument << ");\n";
      }
    } else {
      tfel::raise(
          "writeBoundsChecks: internal error (unsupported bounds type)");
    }
  }

  void writeBoundsChecks(std::ostream& os,
                         const VariableDescription& v,
                         const std::string_view n,
                         const std::string_view space_dimension,
                         const std::string_view policy,
                         const bool addThis,
                         const bool checkEndOfTimeStepValue) {
    writeBoundsChecks(os, v, n, space_dimension, policy, addThis,
                      checkEndOfTimeStepValue, false);
  }  // end of writeBoundsChecks

  void writePhysicalBoundsChecks(std::ostream& os,
                                 const VariableDescription& v,
                                 const std::string_view n,
                                 const std::string_view space_dimension,
                                 const bool addThis,
                                 const bool checkEndOfTimeStepValue) {
    writeBoundsChecks(os, v, n, space_dimension, "", addThis,
                      checkEndOfTimeStepValue, true);
  }  // end of writePhysicalBoundsChecks

#ifdef MFRONT_HAVE_MADNEX

  std::tuple<std::string, std::string, std::string, std::string>
  decomposeImplementationPathInMadnexFile(const std::string_view p) {
    using result_type =
        std::tuple<std::string, std::string, std::string, std::string>;
    const auto details = tfel::utilities::tokenize(p, ':');
    auto raise_if = [&p](const bool b) {
      if (b) {
        tfel::raise("decomposeImplementationPathInMadnexFile: invalid path '" +
                    std::string{p} + "'");
      }
    };
    raise_if((details.size() != 5) && (details.size() != 4));
    if (details.size() == 4) {
      return result_type{std::move(details[1]), std::move(details[2]), "",
                         std::move(details[3])};
    }
    const auto mid = details[3] == "<none>" ? "" : details[3];
    return result_type{std::move(details[1]), std::move(details[2]),
                       std::move(mid), std::move(details[4])};
  }  // end of decomposeImplementationPathInMadnexFile

#endif /* MFRONT_HAVE_MADNEX */

}  // end of namespace mfront
