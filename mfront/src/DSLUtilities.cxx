/*!
 * \file   mfront/src/DSLUtilities.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   23 oct 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include <fstream>
#include <sstream>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/DSLUtilities.hxx"

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  void writeMaterialLaws(const std::string&,
                         std::ostream& srcFile,
                         const std::vector<std::string>& materialLaws) {
    for (const auto& l : materialLaws) {
      srcFile << "using mfront::" << l << ";\n"
              << "using mfront::" << l << "_checkBounds;\n";
    }
  }  // end of writeMaterialLaws

  void writeStaticVariables(
      const std::string& method,
      std::ostream& srcFile,
      const StaticVariableDescriptionContainer& staticVars,
      const std::string& file) {
    if (staticVars.empty()) {
      return;
    }
    for (const auto& v : staticVars) {
      if (getDebugMode()) {
        srcFile << "#line " << v.lineNumber << " \"" << file << "\"\n";
      }
      if (v.type == "short") {
        srcFile << "static " << constexpr_c << "  short " << v.name << " = "
                << static_cast<short>(v.value) << ";\n";
      } else if (v.type == "ushort") {
        srcFile << "static " << constexpr_c << "  unsigned short " << v.name
                << " = " << static_cast<unsigned short>(v.value) << ";\n";
      } else if (v.type == "int") {
        srcFile << "static " << constexpr_c << "  int " << v.name << " = "
                << static_cast<int>(v.value) << ";\n";
      } else if (v.type == "uint") {
        srcFile << "static " << constexpr_c << "  unsigned int " << v.name
                << " = " << static_cast<unsigned int>(v.value) << ";\n";
      } else if (v.type == "long") {
        srcFile << "static " << constexpr_c << "  long " << v.name << " = "
                << static_cast<long>(v.value) << ";\n";
      } else if (v.type == "ulong") {
        srcFile << "static " << constexpr_c << "  unsigned long " << v.name
                << " = " << static_cast<unsigned long>(v.value) << ";\n";
      } else if (v.type == "float") {
        srcFile << "static " << constexpr_c << "  float " << v.name << " = "
                << static_cast<float>(v.value) << ";\n";
      } else if ((v.type == "double") || (v.type == "real")) {
        srcFile << "static " << constexpr_c << "  double " << v.name << " = "
                << static_cast<double>(v.value) << ";\n";
      } else if (v.type == "ldouble") {
        srcFile << "static " << constexpr_c << "  long double " << v.name
                << " = " << static_cast<long double>(v.value) << ";\n";
      } else {
        std::ostringstream msg;
        msg << method << "type '" + v.type
            << "' is not a supported type for a static variable."
            << "Supported types are short, ushort, int, uint, long, ulong,"
            << "float, double and ldouble.\n"
            << "Error at line " << v.lineNumber << ".";
        throw(std::runtime_error(msg.str()));
      }
    }
    srcFile << '\n';
  }  // end of writeStaticVariables

  std::string getMaterialLawLibraryNameBase(const std::string& library,
                                            const std::string& material) {
    if (library.empty()) {
      if (material.empty()) {
        return "MaterialLaw";
      }
      return material;
    }
    return library;
  }  // end of getLibraryNameBase

  void writeF77FUNCMacros(std::ostream& f) {
    auto def = [&f]() {
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

  void writeExportDirectives(std::ostream& file) {
    file
        << "#ifdef _WIN32\n"
        << "#ifndef NOMINMAX\n"
        << "#define NOMINMAX\n"
        << "#endif /* NOMINMAX */\n"
        << "#include <windows.h>\n"
        << "#ifdef small\n"
        << "#undef small\n"
        << "#endif /* small */\n"
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
        << "#define MFRONT_SHAREDOBJ __attribute__((visibility(\"default\")))\n"
        << "#else\n"
        << "#define MFRONT_SHAREDOBJ\n"
        << "#endif /* __GNUC__ */\n"
        << "#endif /* MFRONT_SHAREDOBJ */\n"
        << "#endif /* _WIN32 */\n\n";
  }  // end of writeExportDirectives

  std::string makeUpperCase(const std::string& n) {
    std::string s(n);
    std::string::const_iterator p;
    std::string::iterator p2;
    for (p = n.begin(), p2 = s.begin(); p != n.end(); ++p, ++p2) {
      *p2 = static_cast<char>(toupper(*p));
    }
    return s;
  }  // end of makeUpperCase

  std::string makeLowerCase(const std::string& n) {
    std::string s(n);
    std::string::const_iterator p;
    std::string::iterator p2;
    for (p = n.begin(), p2 = s.begin(); p != n.end(); ++p, ++p2) {
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

}  // end of namespace mfront
