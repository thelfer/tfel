/*!
 * \file   mfront/src/CastemMaterialPropertyInterface.cxx
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

#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyParametersHandler.hxx"
#include "MFront/CastemMaterialPropertyInterface.hxx"

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

  std::string CastemMaterialPropertyInterface::getName() { return "castem"; }

  CastemMaterialPropertyInterface::CastemMaterialPropertyInterface() = default;

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CastemMaterialPropertyInterface::treatKeyword(
      const std::string& k,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator) {
    tfel::raise_if((std::find(i.begin(), i.end(), "castem") != i.end()) ||
                       (std::find(i.begin(), i.end(), "Castem") != i.end()) ||
                       (std::find(i.begin(), i.end(), "Cast3m") != i.end()),
                   "CastemMaterialPropertyInterface::treatKeyword: "
                   "unsupported keyword '" +
                       k + "'");
    return {false, current};
  }  // end of treatKeyword

  CastemMaterialPropertyInterface::~CastemMaterialPropertyInterface() = default;

  void CastemMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const {
    const auto lib = "Castem" + getMaterialLawLibraryNameBase(mpd);
    const auto name = this->getCastemFunctionName(mpd);
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(d[lib].cppflags, CASTEM_CPPFLAGS);
    insert_if(d[lib].cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(d[lib].sources, this->getSourceFileName(name));
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(d[lib].link_libraries, "m");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    d.headers.push_back("include/" + this->getHeaderFileName(name));
    insert_if(d[lib].epts, name);
  }  // end of CastemMaterialPropertyInterface::getTargetsDescription

  std::string CastemMaterialPropertyInterface::getCastemFunctionName(
      const MaterialPropertyDescription& mpd) const {
    const auto material = mpd.material;
    const auto className = mpd.className;
    if (material.empty()) {
      return className;
    }
    return material + "_" + className;
  }

  std::string CastemMaterialPropertyInterface::getHeaderFileName(
      const std::string& name) const {
    return name + "-castem.hxx";
  }

  std::string CastemMaterialPropertyInterface::getSourceFileName(
      const std::string& name) const {
    return name + "-castem.cxx";
  }

  static void writePhysicalBounds(std::ostream& out,
                                  const std::string& name,
                                  const VariableDescription& v) {
    if (!v.hasPhysicalBounds()) {
      return;
    }
    const auto& b = v.getPhysicalBounds();
    if (b.boundsType == VariableBoundsDescription::LOWER) {
      out << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << "cerr << \"" << name << ": " << v.name
          << " is below its physical lower bound (\"\n << " << v.name
          << " << \"<" << b.lowerBound << ").\\n\";\n"
          << "return nan(\"" << name << ": " << v.name
          << " is not physically valid.\");\n"
          << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      out << "if(" << v.name << " > " << b.upperBound << "){\n"
          << "cerr << \"" << name << ": " << v.name
          << " is below its physical upper bound (\"\n << " << v.name
          << " << \">" << b.upperBound << ").\\n\";\n"
          << "return nan(\"" << name << ": " << v.name
          << " is not physically valid.\");\n"
          << "}\n";
    } else {
      out << "if((" << v.name << " < " << b.lowerBound << ")||"
          << "(" << v.name << " > " << b.upperBound << ")){\n"
          << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << "cerr << \"" << name << ": " << v.name
          << " is below its physical lower bound (\"\n << " << v.name
          << " << \"<" << b.lowerBound << ").\\n\";\n"
          << "} else {\n"
          << "cerr << \"" << name << ": " << v.name
          << " is over its physical upper bound (\"\n << " << v.name
          << " << \">" << b.upperBound << ").\\n\";\n"
          << "}\n"
          << "return nan(\"" << name << ": " << v.name
          << " is not physically valid.\");\n"
          << "}\n";
    }
  }  // end of writePhysicalBounds

  static void writeBounds(std::ostream& out,
                          const std::string& name,
                          const VariableDescription& v) {
    if (!v.hasBounds()) {
      return;
    }
    const auto& b = v.getBounds();
    if (b.boundsType == VariableBoundsDescription::LOWER) {
      out << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << "const char * const policy = "
          << "::getenv(\"CASTEM_OUT_OF_BOUNDS_POLICY\");\n"
          << "if(policy!=nullptr){\n"
          << "if(strcmp(policy,\"STRICT\")==0){\n"
          << "return nan(\"" << name << ": " << v.name
          << " is out of bounds.\");\n"
          << "} else if (strcmp(policy,\"WARNING\")==0){\n"
          << "cerr << \"" << name << ": " << v.name
          << " is below its lower bound (\"\n << " << v.name << " << \"<"
          << b.lowerBound << ").\\n\";\n"
          << "}\n"
          << "}\n"
          << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      out << "if(" << v.name << " > " << b.upperBound << "){\n"
          << "const char * const policy = "
          << "::getenv(\"CASTEM_OUT_OF_BOUNDS_POLICY\");\n"
          << "if(policy!=nullptr){\n"
          << "if(strcmp(policy,\"STRICT\")==0){\n"
          << "cerr << \"" << name << ": " << v.name
          << " is over its upper bound (\"\n << " << v.name << " << \">"
          << b.upperBound << ").\\n\";\n"
          << "return nan(\"" << name << ": " << v.name
          << " is out of bounds.\");\n"
          << "} else if (strcmp(policy,\"WARNING\")==0){\n"
          << "cerr << \"" << name << ": " << v.name
          << " is over its upper bound (\"\n << " << v.name << " << \">"
          << b.upperBound << ").\\n\";\n"
          << "}\n"
          << "}\n"
          << "}\n";
    } else {
      out << "if((" << v.name << " < " << b.lowerBound << ")||"
          << "(" << v.name << " > " << b.upperBound << ")){\n"
          << "const char * const policy = "
          << "::getenv(\"CASTEM_OUT_OF_BOUNDS_POLICY\");\n"
          << "if(policy!=nullptr){\n"
          << "if(strcmp(policy,\"STRICT\")==0){\n"
          << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << "cerr << \"" << name << ": " << v.name
          << " is below its lower bound (\"\n << " << v.name << " << \"<"
          << b.lowerBound << ").\\n\";\n"
          << "} else {\n"
          << "cerr << \"" << name << ": " << v.name
          << " is over its upper bound (\"\n << " << v.name << " << \">"
          << b.upperBound << ").\\n\";\n"
          << "}\n"
          << "return nan(\"" << name << ": " << v.name
          << " is out of bounds.\");\n"
          << "} else if (strcmp(policy,\"WARNING\")==0){\n"
          << "if(" << v.name << " < " << b.lowerBound << "){\n"
          << "cerr << \"" << name << ": " << v.name
          << " is below its lower bound (\"\n << " << v.name << " << \"<"
          << b.lowerBound << ").\\n\";\n"
          << "} else {\n"
          << "cerr << \"" << name << ": " << v.name
          << " is over its upper bound (\"\n << " << v.name << " << \">"
          << b.upperBound << ").\\n\";\n"
          << "}\n"
          << "}\n"
          << "}\n"
          << "}\n";
    }
  }  // end of writeBounds

  void CastemMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    this->writeHeaderFile(mpd, fd);
    this->writeSrcFile(mpd, fd);
  }  // end of CastemMaterialPropertyInterface::writeOutputFiles

  void CastemMaterialPropertyInterface::writeHeaderFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto name = this->getCastemFunctionName(mpd);
    const auto fn = "include/" + this->getHeaderFileName(name);
    std::ofstream out{fn};
    tfel::raise_if(!out,
                   "CastemMaterialPropertyInterface::writeOutputFiles: "
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

    out << "#ifndef " << makeUpperCase(name) << "_CASTEM_HH\n"
        << "#define " << makeUpperCase(name) << "_CASTEM_HH\n\n";
    writeExportDirectives(out);
    out << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n"
        << "MFRONT_SHAREDOBJ double\n"
        << name << "("
        << "const double * const"
        << ");\n\n";
    if (!mpd.parameters.empty()) {
      out << "MFRONT_SHAREDOBJ int\n"
          << name << "_setParameter(const char *const,"
          << "const double"
          << ");\n\n";
    }
    out << "#ifdef __cplusplus\n"
        << "} // end of extern \"C\"\n"
        << "#endif /* __cplusplus */\n\n"
        << "#endif /* " << makeUpperCase(name) << "_CASTEM_HH */\n";
    out.close();
  }  // end of CastemMaterialPropertyInterface::writeHeaderFile()

  void CastemMaterialPropertyInterface::writeSrcFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto name = this->getCastemFunctionName(mpd);
    const auto fn = "src/" + this->getSourceFileName(name);
    std::ofstream out{fn};
    tfel::raise_if(!out,
                   "CastemMaterialPropertyInterface::writeOutputFiles: "
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
        << "#include<string>\n"
        << "#include<vector>\n"
        << "#include<cmath>\n";
    if (!includes.empty()) {
      out << includes << "\n\n";
    }
    out << "#include\"" << name << "-castem.hxx\"\n\n";
    writeMaterialPropertyParametersHandler(out, mpd, name, "double", "castem");
    out << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";

    out << "MFRONT_SHAREDOBJ const char *\n"
        << name << "_src = \""
        << tfel::utilities::tokenize(file, tfel::system::dirSeparator()).back()
        << "\";\n\n";
    if (!mpd.inputs.empty()) {
      out << "MFRONT_SHAREDOBJ const char *\n"
          << name << "_args[" << mpd.inputs.size() << "] = {";
      for (auto p3 = mpd.inputs.begin(); p3 != mpd.inputs.end();) {
        const auto iname = '\"' + p3->getExternalName() + '\"';
        out << iname;
        if (++p3 != mpd.inputs.end()) {
          out << ",";
        }
      }
      out << "};\n\n";
    }

    out << "MFRONT_SHAREDOBJ unsigned short\n"
        << name << "_nargs = " << mpd.inputs.size() << "u;\n\n";

    if (!params.empty()) {
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      out << "MFRONT_SHAREDOBJ int\n"
          << name << "_setParameter(const char *const p,"
          << "const double v"
          << "){\n";
      for (const auto& p : params) {
        out << "if(strcmp(\"" << p.name << "\",p)==0){\n"
            << "castem::" << hn << "::get" << hn << "()." << p.name << " = v;\n"
            << "return 1;\n"
            << "}\n";
      }
      out << "return 0;\n"
          << "}\n\n";
    }
    writeEntryPointSymbol(out, name);
    writeTFELVersionSymbol(out, name);
    writeInterfaceSymbol(out, name, "Castem");
    writeMaterialSymbol(out, name, mpd.material);
    writeMaterialKnowledgeTypeSymbol(out, name, MATERIALPROPERTY);
    out << "MFRONT_SHAREDOBJ double\n" << name << "(";
    if (!mpd.inputs.empty()) {
      out << "const double * const castem_params";
    } else {
      out << "const double * const";
    }
    out << ")\n{\n"
        << "using namespace std;\n"
        << "typedef double real;\n";
    // material laws
    writeMaterialLaws(out, materialLaws);
    // static variables
    writeStaticVariables(out, staticVars, file);
    // parameters
    if (!params.empty()) {
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      out << "if(!castem::" << hn << "::get" << hn << "().ok){\n"
          << "return std::nan(castem::" << name
          << "MaterialPropertyHandler::get" << name
          << "MaterialPropertyHandler().msg.c_str());\n"
          << "}\n";
      writeAssignMaterialPropertyParameters(out, mpd, name, "double", "castem");
    }
    if (!mpd.inputs.empty()) {
      auto p3 = mpd.inputs.begin();
      for (auto i = 0u; p3 != mpd.inputs.end(); ++p3, ++i) {
        out << "const double " << p3->name << " = ";
        if (i == 0) {
          out << "*(castem_params);\n";
        } else {
          out << "*(castem_params+" + std::to_string(i) + "u);\n";
        }
      }
    }
    out << "real " << mpd.output.name << ";\n";
    if ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs))) {
      out << "#ifndef NO_CASTEM_BOUNDS_CHECK\n";
    }
    if (hasPhysicalBounds(mpd.inputs)) {
      out << "// treating physical bounds\n";
      for (const auto& i : mpd.inputs) {
        writePhysicalBounds(out, name, i);
      }
    }
    if (hasBounds(mpd.inputs)) {
      out << "// treating standard bounds\n";
      for (const auto& i : mpd.inputs) {
        writeBounds(out, name, i);
      }
    }
    if ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs))) {
      out << "#endif /* NO_CASTEM_BOUNDS_CHECK */\n";
    }
    out << function.body;
    if ((mpd.output.hasPhysicalBounds()) || (mpd.output.hasBounds())) {
      out << "#ifndef NO_CASTEM_BOUNDS_CHECK\n";
      if (mpd.output.hasPhysicalBounds()) {
        out << "// treating physical bounds\n";
        writePhysicalBounds(out, name, mpd.output);
      }
      if (mpd.output.hasBounds()) {
        out << "// treating bounds\n";
        writeBounds(out, name, mpd.output);
      }
      out << "#endif /* NO_CASTEM_BOUNDS_CHECK */\n";
    }
    out << "return " << mpd.output.name << ";\n"
        << "} // end of " << name << "\n\n"
        << "#ifdef __cplusplus\n"
        << "} // end of extern \"C\"\n"
        << "#endif /* __cplusplus */\n\n";
    out.close();
  }  // end of CastemMaterialPropertyInterface::writeSrcFile()

}  // end of namespace mfront
