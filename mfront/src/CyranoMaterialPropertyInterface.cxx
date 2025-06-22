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

  static void writePhysicalBounds(std::ostream& os,
                                  const VariableDescription& v,
                                  const size_t i,
                                  const bool useQuantities) {
    if (!v.hasPhysicalBounds()) {
      return;
    }
    const auto& b = v.getPhysicalBounds();
    const auto to_string = useQuantities
                               ? "std::to_string(" + v.name + ".getValue())"
                               : "std::to_string(" + v.name + ")";
    if (b.boundsType == VariableBoundsDescription::LOWER) {
      os << "if(" << v.name << " < " << v.type << "(" << b.lowerBound << ")){\n"
         << "cyrano_report(\"" << v.name
         << " is below its physical lower bound (\" + " << to_string << " + \"<"
         << b.lowerBound << ").\\n\");\n"
         << "cyrano_output_status->status = -1;\n"
         << "cyrano_output_status->bounds_status = -" << i << ";\n"
         << "errno = cyrano_errno_old;\n"
         << "return nan(\"" << v.name << " is not physically valid.\");\n"
         << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      os << "if(" << v.name << " > " << v.type << "(" << b.upperBound << ")){\n"
         << "cyrano_report(\"" << v.name
         << " is below its physical upper bound (\" + " << to_string << " + \">"
         << b.upperBound << ").\\n\");\n"
         << "cyrano_output_status->status = -1;\n"
         << "cyrano_output_status->bounds_status = -" << i << ";\n"
         << "errno = cyrano_errno_old;\n"
         << "return nan(\"" << v.name << " is not physically valid.\");\n"
         << "}\n";
    } else {
      os << "if((" << v.name << " < " << v.type << "(" << b.lowerBound << "))||"
         << "(" << v.name << " > " << v.type << "(" << b.upperBound << "))){\n"
         << "if(" << v.name << " < " << v.type << "(" << b.lowerBound << ")){\n"
         << "cyrano_report(\"" << v.name
         << " is below its physical lower bound (\" + " << to_string << " + \"<"
         << b.lowerBound << ").\\n\");\n"
         << "} else {\n"
         << "cyrano_report(\"" << v.name
         << " is over its physical upper bound (\" + " << to_string << " + \">"
         << b.upperBound << ").\\n\");\n"
         << "}\n"
         << "cyrano_output_status->status = -1;\n"
         << "cyrano_output_status->bounds_status = -" << i << ";\n"
         << "errno = cyrano_errno_old;\n"
         << "return nan(\"" << v.name << " is not physically valid.\");\n"
         << "}\n";
    }
  }  // end of writePhysicalBounds

  static void writeBounds(std::ostream& os,
                          const VariableDescription& v,
                          const size_t i,
                          const bool useQuantities) {
    if (!v.hasBounds()) {
      return;
    }
    const auto to_string = useQuantities
                               ? "std::to_string(" + v.name + ".getValue())"
                               : "std::to_string(" + v.name + ")";
    const auto& b = v.getBounds();
    if (b.boundsType == VariableBoundsDescription::LOWER) {
      os << "if(" << v.name << " < " << v.type << "(" << b.lowerBound << ")){\n"
         << "if(cyrano_os_of_bounds_policy==CYRANO_STRICT_POLICY){\n"
         << "cyrano_report(\"" << v.name << " is os of bounds.\");\n"
         << "cyrano_output_status->status = -1;\n"
         << "cyrano_output_status->bounds_status = -" << i << ";\n"
         << "errno = cyrano_errno_old;\n"
         << "return nan(\"" << v.name << " is os of bounds.\");\n"
         << "} else if (cyrano_os_of_bounds_policy==CYRANO_WARNING_POLICY){\n"
         << "cyrano_output_status->status = 1;\n"
         << "cyrano_output_status->bounds_status = " << i << ";\n"
         << "cyrano_report(\"" << v.name << " is below its lower bound (\" + "
         << to_string << " + \"<" << b.lowerBound << ").\\n\");\n"
         << "}\n"
         << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      os << "if(" << v.name << " > " << v.type << "(" << b.upperBound << ")){\n"
         << "if(cyrano_os_of_bounds_policy==CYRANO_STRICT_POLICY){\n"
         << "cyrano_report(\"" << v.name << " is over its upper bound (\" + "
         << to_string << " + \">" << b.upperBound << ").\\n\");\n"
         << "cyrano_output_status->status = -1;\n"
         << "cyrano_output_status->bounds_status = -" << i << ";\n"
         << "return nan(\"" << v.name << " is os of bounds.\");\n"
         << "} else if (cyrano_os_of_bounds_policy==CYRANO_WARNING_POLICY){\n"
         << "cyrano_output_status->status = 1;\n"
         << "cyrano_output_status->bounds_status = " << i << ";\n"
         << "cyrano_report(\"" << v.name << " is over its upper bound (\" + "
         << to_string << " + \">" << b.upperBound << ").\\n\");\n"
         << "}\n"
         << "}\n";
    } else {
      os << "if((" << v.name << " < " << v.type << "(" << b.lowerBound << "))||"
         << "(" << v.name << " > " << v.type << "(" << b.upperBound << "))){\n"
         << "if(cyrano_os_of_bounds_policy==CYRANO_STRICT_POLICY){\n"
         << "if(" << v.name << " < " << v.type << "(" << b.lowerBound << ")){\n"
         << "cyrano_report(\"" << v.name << " is below its lower bound (\" + "
         << to_string << " + \"<" << b.lowerBound << ").\\n\");\n"
         << "} else {\n"
         << "cyrano_report(\"" << v.name << " is over its upper bound (\" + "
         << to_string << " + \">" << b.upperBound << ").\\n\");\n"
         << "}\n"
         << "cyrano_output_status->status = -1;\n"
         << "cyrano_output_status->bounds_status = -" << i << ";\n"
         << "errno = cyrano_errno_old;\n"
         << "return nan(\"" << v.name << " is os of bounds.\");\n"
         << "} else if (cyrano_os_of_bounds_policy==CYRANO_WARNING_POLICY){\n"
         << "cyrano_output_status->status = 1;\n"
         << "if(" << v.name << " < " << v.type << "(" << b.lowerBound << ")){\n"
         << "cyrano_output_status->bounds_status = " << i << ";\n"
         << "cyrano_report(\"" << v.name << " is below its lower bound (\" + "
         << to_string << " + \"<" << b.lowerBound << ").\\n\");\n"
         << "} else {\n"
         << "cyrano_output_status->bounds_status = " << i << ";\n"
         << "cyrano_report(\"" << v.name << " is over its upper bound (\" + "
         << to_string << " + \">" << b.upperBound << ").\\n\");\n"
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
    std::ofstream os{fn};
    tfel::raise_if(!os,
                   "CyranoMaterialPropertyInterface::writeOutputFiles: "
                   "unable to open '" +
                       fn + "'");
    os.exceptions(std::ios::badbit | std::ios::failbit);
    os << "/*!\n"
       << "* \\file   " << fn << "\n"
       << "* \\brief  "
       << "this file declares the " << name << " MaterialLaw.\n"
       << "*         File generated by " << MFrontHeader::getVersionName()
       << " "
       << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!fd.authorName.empty()) {
      os << "* \\author " << fd.authorName << '\n';
    }
    if (!fd.date.empty()) {
      os << "* \\date   " << fd.date << '\n';
    }
    if (!fd.description.empty()) {
      os << fd.description << '\n';
    }
    os << " */\n\n";

    os << "#ifndef " << makeUpperCase(name) << "_CYRANO_HH\n"
       << "#define " << makeUpperCase(name) << "_CYRANO_HH\n\n";
    writeExportDirectives(os);
    os << "#include \"Cyrano/MaterialProperty.hxx\"\n\n"
       << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n"
       << "MFRONT_SHAREDOBJ CyranoRealType\n"
       << name << "(CyranoOutputStatus* const,"
       << "const CyranoRealType* const,"
       << "const CyranoIntegerType,"
       << "const CyranoOutOfBoundsPolicy);\n\n";
    if (!mpd.parameters.empty()) {
      os << "MFRONT_SHAREDOBJ int\n"
         << name << "_setParameter(const char *const,"
         << "const double"
         << ");\n\n";
    }
    os << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n"
       << "#endif /* " << makeUpperCase(name) << "_CYRANO_HH */\n";
    os.close();
  }  // end of CyranoMaterialPropertyInterface::writeHeaderFile()

  void CyranoMaterialPropertyInterface::writeSrcFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto name = this->getCyranoFunctionName(mpd);
    const auto fn = "src/" + this->getSourceFileName(name);
    std::ofstream os{fn};
    tfel::raise_if(!os,
                   "CyranoMaterialPropertyInterface::writeOutputFiles: "
                   "unable to open '" +
                       fn + "'");
    os.exceptions(std::ios::badbit | std::ios::failbit);
    const auto& file = fd.fileName;
    const auto& author = fd.authorName;
    const auto& date = fd.date;
    const auto& includes = mpd.includes;
    const auto& params = mpd.parameters;
    const auto& function = mpd.f;
    os << "/*!\n"
       << "* \\file   " << fn << '\n'
       << "* \\brief  "
       << "this file implements the " << name << " MaterialLaw.\n"
       << "*         File generated by " << MFrontHeader::getVersionName()
       << " "
       << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!author.empty()) {
      os << "* \\author " << author << '\n';
    }
    if (!date.empty()) {
      os << "* \\date   " << date << '\n';
    }
    os << " */\n\n"
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
       << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
       << "#include\"TFEL/Math/General/IEEE754.hxx\"\n";
    if (useQuantities(mpd)) {
      os << "#include\"TFEL/Math/qt.hxx\"\n"
         << "#include\"TFEL/Math/Quantity/qtIO.hxx\"\n";
    }
    os << "#include\"" << name << "-cyrano.hxx\"\n\n";
    if (!includes.empty()) {
      os << includes << "\n\n";
    }
    writeMaterialPropertyParametersHandler(os, mpd, name, "double", "cyrano");
    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n";

    writeVariablesNamesSymbol(os, name, mpd);
    writeVariablesBoundsSymbols(os, name, mpd);
    writeEntryPointSymbol(os, name);
    writeTFELVersionSymbol(os, name);
    writeInterfaceSymbol(os, name, "Cyrano");
    writeMaterialSymbol(os, name, mpd.material);
    writeMaterialKnowledgeTypeSymbol(os, name, MATERIALPROPERTY);
    writeParametersSymbols(os, name, mpd);

    os << "MFRONT_SHAREDOBJ const char *\n"
       << name << "_src = \""
       << tfel::utilities::tokenize(file, tfel::system::dirSeparator()).back()
       << "\";\n\n";

    if (!params.empty()) {
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      os << "MFRONT_SHAREDOBJ int\n"
         << name << "_setParameter(const char *const p,"
         << "const double v"
         << "){\n";
      for (const auto& p : params) {
        os << "if(strcmp(\"" << p.name << "\",p)==0){\n"
           << "cyrano::" << hn << "::get" << hn << "()." << p.name << " = v;\n"
           << "return 1;\n"
           << "}\n";
      }
      os << "return 0;\n"
         << "}\n\n";
    }

    os << "MFRONT_SHAREDOBJ CyranoRealType\n" << name << "(";
    if (!mpd.inputs.empty()) {
      os << "CyranoOutputStatus* const cyrano_output_status,\n"
         << "const CyranoRealType* const cyrano_params,\n"
         << "const CyranoIntegerType cyrano_nargs,\n";
      if ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs)) ||
          (hasPhysicalBounds(mpd.output)) || (hasBounds(mpd.output))) {
        os << "const CyranoOutOfBoundsPolicy cyrano_os_of_bounds_policy";
      } else {
        os << "const CyranoOutOfBoundsPolicy";
      }
    } else {
      os << "CyranoOutputStatus* const cyrano_output_status,\n"
         << "const CyranoRealType* const,\n"
         << "const CyranoIntegerType cyrano_nargs,\n";
      if ((hasBounds(mpd.output)) || (hasPhysicalBounds(mpd.output))) {
        os << "const CyranoOutOfBoundsPolicy cyrano_os_of_bounds_policy";
      } else {
        os << "const CyranoOutOfBoundsPolicy";
      }
    }
    os << ")\n{\n";
    writeBeginningOfMaterialPropertyBody(os, mpd, fd, "double", true);
    os << "auto cyrano_report = "
       << "[&cyrano_output_status](const std::string& "
       << "cyrano_error_message){\n"
       << "if(cyrano_error_message.empty()){\n"
       << "return;\n"
       << "}\n"
       << "std::strncpy(cyrano_output_status->msg,"
       << "cyrano_error_message.c_str(),511);\n"
       << "cyrano_output_status->msg[511]='\\0';\n"
       << "};\n";
    os << "const int cyrano_errno_old = errno;\n"
       << "cyrano_output_status->status = 0;\n"
       << "cyrano_output_status->bounds_status = 0;\n"
       << "cyrano_output_status->c_error_number = 0;\n"
       << "errno = 0;\n";
    // check number of arguments
    os << "if(cyrano_nargs!= " << mpd.inputs.size() << "){\n"
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
      os << "if(!cyrano::" << hn << "::get" << hn << "().ok){\n"
         << "cyrano_output_status->status = -6;\n"
         << "cyrano_report(cyrano::" << name << "MaterialPropertyHandler::get"
         << name << "MaterialPropertyHandler().msg);\n"
         << "errno = cyrano_errno_old;\n"
         << "return std::nan(cyrano::" << name << "MaterialPropertyHandler::get"
         << name << "MaterialPropertyHandler().msg.c_str());\n"
         << "}\n";
      writeAssignMaterialPropertyParameters(os, mpd, name, "double", "cyrano");
    }
    if (!mpd.inputs.empty()) {
      auto p3 = mpd.inputs.begin();
      for (auto i = 0u; p3 != mpd.inputs.end(); ++p3, ++i) {
        auto cast_start = useQuantities(mpd) ? p3->type + "(" : "";
        auto cast_end = useQuantities(mpd) ? ")" : "";
        os << "const auto " << p3->name << " = " << cast_start;
        if (i == 0) {
          os << "*(cyrano_params)";
        } else {
          os << "*(cyrano_params+" + std::to_string(i) + "u)";
        }
        os << cast_end << ";\n";
      }
    }
    os << "auto " << mpd.output.name << " = " << mpd.output.type << "{};\n";
    os << "try{\n";
    if ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs))) {
      os << "#ifndef NO_CYRANO_BOUNDS_CHECK\n";
    }
    if (hasPhysicalBounds(mpd.inputs)) {
      os << "// treating physical bounds\n";
      for (decltype(mpd.inputs.size()) i = 0; i != mpd.inputs.size(); ++i) {
        writePhysicalBounds(os, mpd.inputs[i], i + 1, useQuantities(mpd));
      }
    }
    if (hasBounds(mpd.inputs)) {
      os << "// treating standard bounds\n";
      for (decltype(mpd.inputs.size()) i = 0; i != mpd.inputs.size(); ++i) {
        writeBounds(os, mpd.inputs[i], i + 1, useQuantities(mpd));
      }
    }
    if ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs))) {
      os << "#endif /* NO_CYRANO_BOUNDS_CHECK */\n";
    }
    os << function.body;
    if ((mpd.output.hasPhysicalBounds()) || (mpd.output.hasBounds())) {
      os << "#ifndef NO_CYRANO_BOUNDS_CHECK\n";
      if (mpd.output.hasPhysicalBounds()) {
        os << "// treating physical bounds\n";
        writePhysicalBounds(os, mpd.output, mpd.inputs.size() + 1,
                            useQuantities(mpd));
      }
      if (mpd.output.hasBounds()) {
        os << "// treating bounds\n";
        writeBounds(os, mpd.output, mpd.inputs.size() + 1, useQuantities(mpd));
      }
      os << "#endif /* NO_CYRANO_BOUNDS_CHECK */\n";
    }
    os << "} catch(std::exception& e){\n"
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
       << "}\n";
    if (useQuantities(mpd)) {
      os << "return " << mpd.output.name << ".getValue();\n";
    } else {
      os << "return " << mpd.output.name << ";\n";
    }
    os << "} // end of " << name << "\n\n"
       << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n";
    os.close();
  }  // end of CyranoMaterialPropertyInterface::writeSrcFile()

  CyranoMaterialPropertyInterface::~CyranoMaterialPropertyInterface() = default;

}  // end of namespace mfront
