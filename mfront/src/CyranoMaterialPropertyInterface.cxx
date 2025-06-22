/*!
 * \file   mfront/src/CyranoMaterialPropertyInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <string>
#include <set>

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"

#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontLock.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/LibraryDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyParametersHandler.hxx"
#include "MFront/CyranoMaterialPropertyInterface.hxx"

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  std::string CyranoMaterialPropertyInterface::getName() { return "cyrano"; }

  CyranoMaterialPropertyInterface::CyranoMaterialPropertyInterface() = default;

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CyranoMaterialPropertyInterface::treatKeyword(
      const std::string& k,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator) {
    tfel::raise_if((std::find(i.begin(), i.end(), "cyrano") != i.end()) ||
                       (std::find(i.begin(), i.end(), "Cyrano") != i.end()),
                   "CyranoMaterialPropertyInterface::treatKeyword: "
                   "unsupported keyword '" +
                       k + "'");
    return {false, current};
  }  // end of treatKeyword

  void CyranoMaterialPropertyInterface::getLibraryDescription(
      TargetsDescription& d,
      LibraryDescription& l,
      const MaterialPropertyDescription& mpd) const {
    const auto name = this->getCyranoFunctionName(mpd);
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(l.cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(l.include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(l.sources, this->getSourceFileName(name));
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(l.link_libraries, "m");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    insert_if(l.epts, name);
    d.headers.push_back("include/" + this->getHeaderFileName(name));
  }  // end of CyranoMaterialPropertyInterface::getLibraryDescription

  void CyranoMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const {
    const auto lib = "Cyrano" + getMaterialLawLibraryNameBase(mpd);
    this->getLibraryDescription(d, d.getLibrary(lib), mpd);
  }  // end of CyranoMaterialPropertyInterface::getTargetsDescription

  std::string CyranoMaterialPropertyInterface::getCyranoFunctionName(
      const MaterialPropertyDescription& mpd) const {
    const auto material = mpd.material;
    const auto className = mpd.className;
    if (material.empty()) {
      return className;
    }
    return material + "_" + className;
  }

  std::string CyranoMaterialPropertyInterface::getHeaderFileName(
      const std::string& name) const {
    return name + "-cyrano.hxx";
  }

  std::string CyranoMaterialPropertyInterface::getSourceFileName(
      const std::string& name) const {
    return name + "-cyrano.cxx";
  }

  static void writePhysicalBounds(std::ostream& out,
                                  const VariableDescription& v,
                                  const size_t i) {
    if (!v.hasPhysicalBounds()) {
      return;
    }
    const auto& b = v.getPhysicalBounds();
    if (b.boundsType == VariableBoundsDescription::LOWER) {
      out << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << "cyrano_report(\"" << v.name
          << " is below its physical lower bound (\" + std::to_string("
          << v.name << ")+\"<" << b.lowerBound << ").\\n\");\n"
          << "cyrano_output_status->status = -1;\n"
          << "cyrano_output_status->bounds_status = -" << i << ";\n"
          << "errno = cyrano_errno_old;\n"
          << "return nan(\"" << v.name << " is not physically valid.\");\n"
          << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      out << "if(" << v.name << " > " << b.upperBound << "){\n"
          << "cyrano_report(\"" << v.name
          << " is below its physical upper bound (\"+std::to_string(" << v.name
          << ")+\">" << b.upperBound << ").\\n\");\n"
          << "cyrano_output_status->status = -1;\n"
          << "cyrano_output_status->bounds_status = -" << i << ";\n"
          << "errno = cyrano_errno_old;\n"
          << "return nan(\"" << v.name << " is not physically valid.\");\n"
          << "}\n";
    } else {
      out << "if((" << v.name << " < " << b.lowerBound << ")||"
          << "(" << v.name << " > " << b.upperBound << ")){\n"
          << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << "cyrano_report(\"" << v.name
          << " is below its physical lower bound (\"+std::to_string(" << v.name
          << ")+\"<" << b.lowerBound << ").\\n\");\n"
          << "} else {\n"
          << "cyrano_report(\"" << v.name
          << " is over its physical upper bound (\"+std::to_string(" << v.name
          << ")+\">" << b.upperBound << ").\\n\");\n"
          << "}\n"
          << "cyrano_output_status->status = -1;\n"
          << "cyrano_output_status->bounds_status = -" << i << ";\n"
          << "errno = cyrano_errno_old;\n"
          << "return nan(\"" << v.name << " is not physically valid.\");\n"
          << "}\n";
    }
  }  // end of writePhysicalBounds

  static void writeBounds(std::ostream& out,
                          const VariableDescription& v,
                          const size_t i) {
    if (!v.hasBounds()) {
      return;
    }
    const auto& b = v.getBounds();
    if (b.boundsType == VariableBoundsDescription::LOWER) {
      out << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << "if(cyrano_out_of_bounds_policy==CYRANO_STRICT_POLICY){\n"
          << "cyrano_report(\"" << v.name << " is out of bounds.\");\n"
          << "cyrano_output_status->status = -1;\n"
          << "cyrano_output_status->bounds_status = -" << i << ";\n"
          << "errno = cyrano_errno_old;\n"
          << "return nan(\"" << v.name << " is out of bounds.\");\n"
          << "} else if (cyrano_out_of_bounds_policy==CYRANO_WARNING_POLICY){\n"
          << "cyrano_output_status->status = 1;\n"
          << "cyrano_output_status->bounds_status = " << i << ";\n"
          << "cyrano_report(\"" << v.name
          << " is below its lower bound (\" + std::to_string(" << v.name
          << ") + \"<" << b.lowerBound << ").\\n\");\n"
          << "}\n"
          << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      out << "if(" << v.name << " > " << b.upperBound << "){\n"
          << "if(cyrano_out_of_bounds_policy==CYRANO_STRICT_POLICY){\n"
          << "cyrano_report(\"" << v.name
          << " is over its upper bound (\" + std::to_string(" << v.name
          << ") + \">" << b.upperBound << ").\\n\");\n"
          << "cyrano_output_status->status = -1;\n"
          << "cyrano_output_status->bounds_status = -" << i << ";\n"
          << "return nan(\"" << v.name << " is out of bounds.\");\n"
          << "} else if (cyrano_out_of_bounds_policy==CYRANO_WARNING_POLICY){\n"
          << "cyrano_output_status->status = 1;\n"
          << "cyrano_output_status->bounds_status = " << i << ";\n"
          << "cyrano_report(\"" << v.name
          << " is over its upper bound (\" + std::to_string(" << v.name
          << ") + \">" << b.upperBound << ").\\n\");\n"
          << "}\n"
          << "}\n";
    } else {
      out << "if((" << v.name << " < " << b.lowerBound << ")||"
          << "(" << v.name << " > " << b.upperBound << ")){\n"
          << "if(cyrano_out_of_bounds_policy==CYRANO_STRICT_POLICY){\n"
          << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << "cyrano_report(\"" << v.name
          << " is below its lower bound (\" + std::to_string(" << v.name
          << ") + \"<" << b.lowerBound << ").\\n\");\n"
          << "} else {\n"
          << "cyrano_report(\"" << v.name
          << " is over its upper bound (\" + std::to_string(" << v.name
          << ") + \">" << b.upperBound << ").\\n\");\n"
          << "}\n"
          << "cyrano_output_status->status = -1;\n"
          << "cyrano_output_status->bounds_status = -" << i << ";\n"
          << "errno = cyrano_errno_old;\n"
          << "return nan(\"" << v.name << " is out of bounds.\");\n"
          << "} else if (cyrano_out_of_bounds_policy==CYRANO_WARNING_POLICY){\n"
          << "cyrano_output_status->status = 1;\n"
          << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << "cyrano_output_status->bounds_status = " << i << ";\n"
          << "cyrano_report(\"" << v.name
          << " is below its lower bound (\" + std::to_string(" << v.name
          << ") + \"<" << b.lowerBound << ").\\n\");\n"
          << "} else {\n"
          << "cyrano_output_status->bounds_status = " << i << ";\n"
          << "cyrano_report(\"" << v.name
          << " is over its upper bound (\" + std::to_string(" << v.name
          << ") + \">" << b.upperBound << ").\\n\");\n"
          << "}\n"
          << "}\n"
          << "}\n";
    }
  }  // end of writeBounds

  void CyranoMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    this->writeHeaderFile(mpd, fd);
    this->writeSrcFile(mpd, fd);
  }  // end of CyranoMaterialPropertyInterface::writeOutputFiles

  void CyranoMaterialPropertyInterface::writeHeaderFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto name = this->getCyranoFunctionName(mpd);
    const auto fn = "include/" + this->getHeaderFileName(name);
    std::ofstream out{fn};
    tfel::raise_if(!out,
                   "CyranoMaterialPropertyInterface::writeOutputFiles: "
                   "unable to open '" +
                       fn + "'");
    out.exceptions(std::ios::badbit | std::ios::failbit);
    out << "/*!\n"
        << "* \\file   " << fn << "\n"
        << "* \\brief  "
        << "this file declares the " << name << " MaterialLaw.\n"
        << "*         File generated by " << MFrontHeader::getVersionName()
        << " "
        << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!fd.authorName.empty()) {
      out << "* \\author " << fd.authorName << '\n';
    }
    if (!fd.date.empty()) {
      out << "* \\date   " << fd.date << '\n';
    }
    if (!fd.description.empty()) {
      out << fd.description << '\n';
    }
    out << " */\n\n";

    out << "#ifndef " << makeUpperCase(name) << "_CYRANO_HH\n"
        << "#define " << makeUpperCase(name) << "_CYRANO_HH\n\n";
    writeExportDirectives(out);
    out << "#include \"Cyrano/MaterialProperty.hxx\"\n\n"
        << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n"
        << "MFRONT_SHAREDOBJ CyranoRealType\n"
        << name << "(CyranoOutputStatus* const,"
        << "const CyranoRealType* const,"
        << "const CyranoIntegerType,"
        << "const CyranoOutOfBoundsPolicy);\n\n";
    if (!mpd.parameters.empty()) {
      out << "MFRONT_SHAREDOBJ int\n"
          << name << "_setParameter(const char *const,"
          << "const double"
          << ");\n\n";
    }
    out << "#ifdef __cplusplus\n"
        << "} // end of extern \"C\"\n"
        << "#endif /* __cplusplus */\n\n"
        << "#endif /* " << makeUpperCase(name) << "_CYRANO_HH */\n";
    out.close();
  }  // end of CyranoMaterialPropertyInterface::writeHeaderFile()

  void CyranoMaterialPropertyInterface::writeSrcFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto name = this->getCyranoFunctionName(mpd);
    const auto fn = "src/" + this->getSourceFileName(name);
    std::ofstream out{fn};
    tfel::raise_if(!out,
                   "CyranoMaterialPropertyInterface::writeOutputFiles: "
                   "unable to open '" +
                       fn + "'");
    out.exceptions(std::ios::badbit | std::ios::failbit);
    const auto& file = fd.fileName;
    const auto& author = fd.authorName;
    const auto& date = fd.date;
    const auto& includes = mpd.includes;
    const auto& materialLaws = mpd.materialLaws;
    const auto& staticVars = mpd.staticVars;
    const auto& params = mpd.parameters;
    const auto& function = mpd.f;
    out << "/*!\n"
        << "* \\file   " << fn << '\n'
        << "* \\brief  "
        << "this file implements the " << name << " MaterialLaw.\n"
        << "*         File generated by " << MFrontHeader::getVersionName()
        << " "
        << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!author.empty()) {
      out << "* \\author " << author << '\n';
    }
    if (!date.empty()) {
      out << "* \\date   " << date << '\n';
    }
    out << " */\n\n"
        << "#include<algorithm>\n"
        << "#include<iterator>\n"
        << "#include<iostream>\n"
        << "#include<sstream>\n"
        << "#include<fstream>\n"
        << "#include<cstring>\n"
        << "#include<cstdlib>\n"
        << "#include<cerrno>\n"
        << "#include<string>\n"
        << "#include<vector>\n"
        << "#include<cmath>\n"
        << "#include\"TFEL/Math/General/IEEE754.hxx\"\n";
    out << "#include\"" << name << "-cyrano.hxx\"\n\n";
    if (!includes.empty()) {
      out << includes << "\n\n";
    }
    writeMaterialPropertyParametersHandler(out, mpd, name, "double", "cyrano");
    out << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";

    writeVariablesNamesSymbol(out, name, mpd);
    writeVariablesBoundsSymbols(out, name, mpd);
    writeEntryPointSymbol(out, name);
    writeTFELVersionSymbol(out, name);
    writeInterfaceSymbol(out, name, "Cyrano");
    writeMaterialSymbol(out, name, mpd.material);
    writeMaterialKnowledgeTypeSymbol(out, name, MATERIALPROPERTY);
    writeParametersSymbols(out, name, mpd);

    out << "MFRONT_SHAREDOBJ const char *\n"
        << name << "_src = \""
        << tfel::utilities::tokenize(file, tfel::system::dirSeparator()).back()
        << "\";\n\n";

    if (!params.empty()) {
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      out << "MFRONT_SHAREDOBJ int\n"
          << name << "_setParameter(const char *const p,"
          << "const double v"
          << "){\n";
      for (const auto& p : params) {
        out << "if(strcmp(\"" << p.name << "\",p)==0){\n"
            << "cyrano::" << hn << "::get" << hn << "()." << p.name << " = v;\n"
            << "return 1;\n"
            << "}\n";
      }
      out << "return 0;\n"
          << "}\n\n";
    }

    out << "MFRONT_SHAREDOBJ CyranoRealType\n" << name << "(";
    if (!mpd.inputs.empty()) {
      out << "CyranoOutputStatus* const cyrano_output_status,\n"
          << "const CyranoRealType* const cyrano_params,\n"
          << "const CyranoIntegerType cyrano_nargs,\n";
      if ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs)) ||
          (hasPhysicalBounds(mpd.output)) || (hasBounds(mpd.output))) {
        out << "const CyranoOutOfBoundsPolicy cyrano_out_of_bounds_policy";
      } else {
        out << "const CyranoOutOfBoundsPolicy";
      }
    } else {
      out << "CyranoOutputStatus* const cyrano_output_status,\n"
          << "const CyranoRealType* const,\n"
          << "const CyranoIntegerType cyrano_nargs,\n";
      if ((hasBounds(mpd.output)) || (hasPhysicalBounds(mpd.output))) {
        out << "const CyranoOutOfBoundsPolicy cyrano_out_of_bounds_policy";
      } else {
        out << "const CyranoOutOfBoundsPolicy";
      }
    }
    out << ")\n{\n"
        << "using namespace std;\n"
        << "using real = CyranoRealType;\n";
    out << "auto cyrano_report = "
        << "[&cyrano_output_status](const std::string& "
        << "cyrano_error_message){\n"
        << "if(cyrano_error_message.empty()){\n"
        << "return;\n"
        << "}\n"
        << "std::strncpy(cyrano_output_status->msg,"
        << "cyrano_error_message.c_str(),511);\n"
        << "cyrano_output_status->msg[511]='\\0';\n"
        << "};\n";
    // material laws
    writeMaterialLaws(out, materialLaws);
    // static variables
    writeStaticVariables(out, staticVars, file);
    out << "const int cyrano_errno_old = errno;\n"
        << "cyrano_output_status->status = 0;\n"
        << "cyrano_output_status->bounds_status = 0;\n"
        << "cyrano_output_status->c_error_number = 0;\n"
        << "errno = 0;\n";
    // check number of arguments
    out << "if(cyrano_nargs!= " << mpd.inputs.size() << "){\n"
        << "cyrano_output_status->status = -5;\n"
        << "cyrano_report(\"invalid number of arguments "
        << "(\"+std::to_string(cyrano_nargs)+\" given, " << mpd.inputs.size()
        << " expected)\");\n"
        << "errno = cyrano_errno_old;\n"
        << "return std::nan(\"invalid number of arguments\");\n"
        << "}\n";
    // parameters
    if (!params.empty()) {
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      out << "if(!cyrano::" << hn << "::get" << hn << "().ok){\n"
          << "cyrano_output_status->status = -6;\n"
          << "cyrano_report(cyrano::" << name << "MaterialPropertyHandler::get"
          << name << "MaterialPropertyHandler().msg);\n"
          << "errno = cyrano_errno_old;\n"
          << "return std::nan(cyrano::" << name
          << "MaterialPropertyHandler::get" << name
          << "MaterialPropertyHandler().msg.c_str());\n"
          << "}\n";
      writeAssignMaterialPropertyParameters(out, mpd, name, "double", "cyrano");
    }
    if (!mpd.inputs.empty()) {
      auto p3 = mpd.inputs.begin();
      for (auto i = 0u; p3 != mpd.inputs.end(); ++p3, ++i) {
        out << "const CyranoRealType " << p3->name << " = ";
        if (i == 0) {
          out << "*(cyrano_params);\n";
        } else {
          out << "*(cyrano_params+" + std::to_string(i) + "u);\n";
        }
      }
    }
    out << "real " << mpd.output.name << ";\n";
    out << "try{\n";
    if ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs))) {
      out << "#ifndef NO_CYRANO_BOUNDS_CHECK\n";
    }
    if (hasPhysicalBounds(mpd.inputs)) {
      out << "// treating physical bounds\n";
      for (decltype(mpd.inputs.size()) i = 0; i != mpd.inputs.size(); ++i) {
        writePhysicalBounds(out, mpd.inputs[i], i + 1);
      }
    }
    if (hasBounds(mpd.inputs)) {
      out << "// treating standard bounds\n";
      for (decltype(mpd.inputs.size()) i = 0; i != mpd.inputs.size(); ++i) {
        writeBounds(out, mpd.inputs[i], i + 1);
      }
    }
    if ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs))) {
      out << "#endif /* NO_CYRANO_BOUNDS_CHECK */\n";
    }
    out << function.body;
    if ((mpd.output.hasPhysicalBounds()) || (mpd.output.hasBounds())) {
      out << "#ifndef NO_CYRANO_BOUNDS_CHECK\n";
      if (mpd.output.hasPhysicalBounds()) {
        out << "// treating physical bounds\n";
        writePhysicalBounds(out, mpd.output, mpd.inputs.size() + 1);
      }
      if (mpd.output.hasBounds()) {
        out << "// treating bounds\n";
        writeBounds(out, mpd.output, mpd.inputs.size() + 1);
      }
      out << "#endif /* NO_CYRANO_BOUNDS_CHECK */\n";
    }
    out << "} catch(std::exception& e){\n"
        << "cyrano_output_status->status = -2;\n"
        << "cyrano_report(e.what());\n"
        << "errno = cyrano_errno_old;\n"
        << "return nan(\"C++ exception\");\n"
        << "} catch(...){\n"
        << "cyrano_output_status->status = -2;\n"
        << "cyrano_report(\"unknown C++ exception\");\n"
        << "errno = cyrano_errno_old;\n"
        << "return nan(\"C++ exception\");\n"
        << "}\n"
        << "if (errno != 0) {\n"
        << "cyrano_output_status->status = -3;\n"
        << "cyrano_output_status->c_error_number = errno;\n"
        << "cyrano_report(strerror(errno));\n"
        << "}\n"
        << "errno = cyrano_errno_old;\n"
        << "if(!tfel::math::ieee754::isfinite(" << mpd.output.name << ")){\n"
        << "cyrano_output_status->status = -4;\n"
        << "}\n"
        << "return " << mpd.output.name << ";\n"
        << "} // end of " << name << "\n\n"
        << "#ifdef __cplusplus\n"
        << "} // end of extern \"C\"\n"
        << "#endif /* __cplusplus */\n\n";
    out.close();
  }  // end of CyranoMaterialPropertyInterface::writeSrcFile()

  CyranoMaterialPropertyInterface::~CyranoMaterialPropertyInterface() = default;

}  // end of namespace mfront
