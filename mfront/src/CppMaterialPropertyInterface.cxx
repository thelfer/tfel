/*!
 * \file   mfront/src/CppMaterialPropertyInterface.cxx
 * \brief
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/CppMaterialPropertyInterface.hxx"

namespace mfront {

  std::string CppMaterialPropertyInterface::getName() { return "c++"; }

  CppMaterialPropertyInterface::CppMaterialPropertyInterface() {}

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CppMaterialPropertyInterface::treatKeyword(const std::string& k,
                                             const std::vector<std::string>& i,
                                             tokens_iterator current,
                                             const tokens_iterator) {
    if ((std::find(i.begin(), i.end(), "c++") != i.end()) ||
        (std::find(i.begin(), i.end(), "C++") != i.end()) ||
        (std::find(i.begin(), i.end(), "cxx") != i.end()) ||
        (std::find(i.begin(), i.end(), "Cxx") != i.end()) ||
        (std::find(i.begin(), i.end(), "cpp") != i.end()) ||
        (std::find(i.begin(), i.end(), "Cpp") != i.end())) {
      throw(
          std::runtime_error("CastemMaterialPropertyInterface::treatKeyword: "
                             "unsupported keyword '" +
                             k + "'"));
    }
    return {false, current};
  }  // end of treatKeyword

  void CppMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) {
    const auto lib =
        "Cpp" + getMaterialLawLibraryNameBase(mpd.library, mpd.material);
    const auto name = mpd.material.empty() ? mpd.className
                                           : mpd.material + "_" + mpd.className;
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(d[lib].ldflags, "-lm");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    insert_if(d[lib].sources, name + "-cxx.cxx");
    insert_if(d.headers, this->headerFileName);
    auto cn = std::string{};
#pragma message("handle namespace")
    // for(const auto& ns : this->namespaces){
    //   cc += ns + "::"
    // }
    cn += name;
    insert_if(d[lib].epts, cn);
  }  // end of CMaterialPropertyInterface::getTargetsDescription

  void CppMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) {
    using namespace std;
    using namespace tfel::system;
    const auto& file = fd.fileName;
    const auto& author = fd.authorName;
    const auto& description = fd.description;
    const auto& date = fd.date;
    const auto& material = mpd.material;
    const auto& className = mpd.className;
    const auto& includes = mpd.includes;
    const auto& output = mpd.output;
    const auto& inputs = mpd.inputs;
    const auto& materialLaws = mpd.materialLaws;
    const auto& staticVars = mpd.staticVars;
    const auto& params = mpd.parameters;
    const auto& paramValues = mpd.parametersValues;
    const auto& function = mpd.f;
    const auto& bounds = mpd.bounds;
    const auto& physicalBounds = mpd.physicalBounds;
    const auto name =
        (!material.empty()) ? material + "_" + className : className;
    this->writeHeaderFile(name, author, date, description, includes, inputs,
                          params, bounds, physicalBounds);
    this->writeSrcFile(file, name, author, date, output, inputs, materialLaws,
                       staticVars, params, paramValues, function, bounds,
                       physicalBounds);
  }  // end of CppMaterialPropertyInterface::writeOutputFiles

  void CppMaterialPropertyInterface::writeHeaderFile(
      const std::string& name,
      const std::string& author,
      const std::string& date,
      const std::string& description,
      const std::string& includes,
      const VariableDescriptionContainer& inputs,
      const std::vector<std::string>& params,
      const std::vector<VariableBoundsDescription>& bounds,
      const std::vector<VariableBoundsDescription>& physicalBounds) {
    using namespace std;
    this->headerFileName = "include/" + name + "-cxx.hxx";
    std::ofstream header(this->headerFileName);
    if (!header) {
      string msg("MaterialPropertyDSL::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->headerFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    header.exceptions(ios::badbit | ios::failbit);
    vector<string>::const_iterator p3;
    VariableDescriptionContainer::const_iterator p4;
    header << "/*!" << endl;
    header << "* \\file   " << this->headerFileName << endl;
    header << "* \\brief  "
           << "this file declares the " << name << " MaterialLaw.\n";
    header << "*         File generated by ";
    header << MFrontHeader::getVersionName() << " ";
    header << "version " << MFrontHeader::getVersionNumber();
    header << endl;
    if (!author.empty()) {
      header << "* \\author " << author << endl;
    }
    if (!date.empty()) {
      header << "* \\date   " << date << endl;
    }
    if (!description.empty()) {
      header << description << endl;
    }
    header << " */\n\n";

    header << "#ifndef LIB_MFRONT_" << makeUpperCase(name) << "_HXX\n";
    header << "#define LIB_MFRONT_" << makeUpperCase(name) << "_HXX\n\n";
    header << "#include<ostream>\n";
    header << "#include<cmath>\n";
    header << "#include<algorithm>\n";
    header << "#include<stdexcept>\n\n";
    header << "#include<functional>\n\n";
    if (!includes.empty()) {
      header << includes << endl;
    }
    writeExportDirectives(header);
    header << "namespace mfront\n{\n\n";
    header << "struct MFRONT_SHAREDOBJ " << name << endl;
    if (!inputs.empty()) {
      if (inputs.size() == 1) {
        header << ": std::unary_function<double,double>\n";
      } else if (inputs.size() == 2) {
        header << ": std::binary_function<double,double,double>\n";
      }
    }
    header << "{\n\n";
    if (inputs.empty()) {
      header << "//! nested typedef to make " << name
             << " model an adaptable generator (STL compliance)\n\n";
      header << "typedef double result_type;\n\n";
    }
    header << "//! default constructor\n";
    header << name << "() noexcept;\n\n";
    header << "//! move constructor\n";
    header << name << "(" << name << "&&) noexcept = default;\n";
    header << "//! copy constructor\n";
    header << name << "(const " << name << "&) noexcept = default;\n";
    header << "//! move assignement operator\n";
    header << name << "& operator=(" << name << "&&) noexcept = default;\n";
    header << "//! assignement operator\n";
    header << name << "& operator=(const " << name
           << "&) noexcept = default;\n\n";
    header << "double\noperator()(";
    for (p4 = inputs.begin(); p4 != inputs.end();) {
      header << "const double";
      if ((++p4) != inputs.end()) {
        header << ",";
      }
    }
    header << ") const;\n\n";
    if ((!bounds.empty()) || (!physicalBounds.empty())) {
      header << "static void\ncheckBounds(";
      for (p4 = inputs.begin(); p4 != inputs.end();) {
        header << "const double";
        if ((++p4) != inputs.end()) {
          header << ",";
        }
      }
      header << ");\n\n";
    }
    for (p3 = params.begin(); p3 != params.end(); ++p3) {
      header << "const double& get" << *p3 << "(void) const;\n";
    }
    for (p3 = params.begin(); p3 != params.end(); ++p3) {
      header << "double& get" << *p3 << "(void);\n";
    }
    for (p3 = params.begin(); p3 != params.end(); ++p3) {
      header << "void set" << *p3 << "(const double);\n";
    }
    if (!params.empty()) {
      header << "private:\n";
      for (p3 = params.begin(); p3 != params.end(); ++p3) {
        header << "double " << *p3 << ";\n";
      }
    }
    header << "}; // end of class " << name << endl << endl;
    header << "std::ostream&\n";
    header << "operator<<(std::ostream&,const " << name;
    header << "&);\n\n";
    header << "} // end of namespace mfront\n\n";
    header << "#endif /* LIB_MFRONT_" << makeUpperCase(name) << "_HXX */\n";
    header.close();
  }  // end of CppMaterialPropertyInterface::writeHeaderFile(void)

  void CppMaterialPropertyInterface::writeSrcFile(
      const std::string& file,
      const std::string& name,
      const std::string& author,
      const std::string& date,
      const std::string& output,
      const VariableDescriptionContainer& inputs,
      const std::vector<std::string>& materialLaws,
      const StaticVariableDescriptionContainer& staticVars,
      const std::vector<std::string>& params,
      const std::map<std::string, double>& paramValues,
      const LawFunction& function,
      const std::vector<VariableBoundsDescription>& bounds,
      const std::vector<VariableBoundsDescription>& physicalBounds) {
    using namespace std;
    const auto src_name = "src/" + name + "-cxx.cxx";
    std::ofstream src(src_name);
    if (!src) {
      throw(
          runtime_error("MaterialPropertyDSL::writeOutputFiles: "
                        "unable to open '" +
                        src_name + "'"));
    }
    src.exceptions(ios::badbit | ios::failbit);
    vector<string>::const_iterator p;
    VariableDescriptionContainer::const_iterator p3;
    map<string, double>::const_iterator p7;
    src << "/*!" << endl;
    src << "* \\file   " << src_name << endl;
    src << "* \\brief  "
        << "this file implements the " << name << " MaterialLaw.\n";
    src << "*         File generated by ";
    src << MFrontHeader::getVersionName() << " ";
    src << "version " << MFrontHeader::getVersionNumber();
    src << endl;
    if (!author.empty()) {
      src << "* \\author " << author << endl;
    }
    if (!date.empty()) {
      src << "* \\date   " << date << endl;
    }
    src << " */\n\n"
        << "#include<stdexcept>\n"
        << "#include<iostream>\n"
        << "#include<cstdlib>\n"
        << "#include<sstream>\n"
        << "#include<cstring>\n"
        << "#include<string>\n"
        << "#include\"" << name << "-cxx.hxx\"\n\n"
        << "namespace mfront\n{\n\n"
        << name << "::" << name << "() noexcept\n";
    if (!params.empty()) {
      src << ": ";
    }
    for (p = params.begin(); p != params.end();) {
      p7 = paramValues.find(*p);
      if (p7 == paramValues.end()) {
        string msg("MaterialPropertyDSL::writeCppSrcFile : ");
        msg += "internal error (can't find value of parameter '" + *p + "')";
        throw(runtime_error(msg));
      }
      src << *p << "(" << p7->second << ")";
      if (++p != params.end()) {
        src << ",";
      }
      src << "\n";
    }
    src << "{} // end of " << name << "::" << name << "\n\n";
    for (p = params.begin(); p != params.end(); ++p) {
      src << "const double& ";
      src << name;
      src << "::get" << *p << "(void) const{\n"
          << "return this->" << *p << ";\n"
          << "} // end of " << name << "::get\n\n";
    }
    for (p = params.begin(); p != params.end(); ++p) {
      src << "double& ";
      src << name;
      src << "::get" << *p << "(void){\n"
          << "return " << *p << ";\n"
          << "} // end of " << name << "::get\n\n";
    }
    for (p = params.begin(); p != params.end(); ++p) {
      src << "void " << name;
      src << "::set" << *p;
      src << "(const double " << name << "_value_)";
      src << "{\n"
          << "this->" << *p << " = " << name << "_value_;\n"
          << "} // end of " << name << "::set\n\n";
    }
    src << "double\n";
    src << name;
    src << "::operator()(";
    for (p3 = inputs.begin(); p3 != inputs.end();) {
      src << "const double " << p3->name;
      if (++p3 != inputs.end()) {
        src << ",";
      }
    }
    src << ") const\n{\n";
    src << "using namespace std;" << endl;
    src << "using real = double;" << endl;
    // material laws
    writeMaterialLaws("CMaterialPropertyInterfaceBase::writeSrcFile", src,
                      materialLaws);
    // static variables
    writeStaticVariables("MaterialPropertyDSL::writeSrcFile", src, staticVars,
                         file);
    // body
    src << "real " << output << ";\n";
    if ((!bounds.empty()) || (!physicalBounds.empty())) {
      src << "#ifndef MFRONT_NO_BOUNDS_CHECK\n";
      src << name << "::checkBounds(";
      for (p3 = inputs.begin(); p3 != inputs.end();) {
        src << p3->name;
        if ((++p3) != inputs.end()) {
          src << ",";
        }
      }
      src << ");\n";
      src << "#endif /* MFRONT_NO_BOUNDS_CHECK */\n";
    }
    if (!inputs.empty()) {
      src << "#ifndef MFRONT_NOERRNO_HANDLING\n"
          << "const auto mfront_errno_old = errno;\n"
          << "errno=0;\n"
          << "#endif /* MFRONT_NOERRNO_HANDLING */\n";
    }
    src << function.body;
    if (!inputs.empty()) {
      src << "#ifndef MFRONT_NOERRNO_HANDLING\n"
          // can't use std::swap here as errno might be a macro
          << "const auto mfront_errno = errno;\n"
          << "errno = mfront_errno_old;\n"
          << "if(mfront_errno!=0){\n"
          << "throw(runtime_error(\"" << name << ": errno has been set \"\n"
          << "                    "
             "\"(\"+std::string(::strerror(errno))+\")\"));\n"
          << "}\n"
          << "#endif /* MFRONT_NOERRNO_HANDLING */\n";
    }
    src << "return " << output << ";\n";
    src << "} // end of " << name << "::operator()\n\n";
    if ((!bounds.empty()) || (!physicalBounds.empty())) {
      src << "void\n";
      src << name;
      src << "::checkBounds(";
      if (!inputs.empty()) {
        for (p3 = inputs.begin(); p3 != inputs.end();) {
          src << "const double " << p3->name;
          if ((++p3) != inputs.end()) {
            src << ",";
          }
        }
      } else {
        src << "void";
      }
      src << ")\n{\n";
      if (!physicalBounds.empty()) {
        src << "using namespace std;\n";
        src << "// treating physical bounds\n";
        for (const auto& b : physicalBounds) {
          if (b.boundsType == VariableBoundsDescription::Lower) {
            src << "if(" << b.varName << " < " << b.lowerBound << "){\n";
            src << "ostringstream msg;\n";
            src << "msg << \"" << name << " : " << b.varName
                << " is below its physical lower bound \";\n";
            src << "msg << \"(\" << " << b.varName << " << \" < "
                << b.lowerBound << ")\";\n";
            src << "throw(range_error(msg.str()));\n";
            src << "}\n";
          } else if (b.boundsType == VariableBoundsDescription::Upper) {
            src << "if(" << b.varName << " > " << b.upperBound << "){\n";
            src << "ostringstream msg;\n";
            src << "msg << \"" << name << " : " << b.varName
                << " is beyond its physical upper bound \";\n";
            src << "msg << \"(\" << " << b.varName << " << \" > "
                << b.upperBound << ")\";\n";
            src << "throw(range_error(msg.str()));\n";
            src << "}\n";
          } else {
            src << "if(" << b.varName << " < " << b.lowerBound << "){\n";
            src << "ostringstream msg;\n";
            src << "msg << \"" << name << " : " << b.varName
                << " is below its physical lower bound \";\n";
            src << "msg << \"(\" << " << b.varName << " << \" < "
                << b.lowerBound << ")\";\n";
            src << "throw(range_error(msg.str()));\n";
            src << "}\n";
            src << "if(" << b.varName << " > " << b.upperBound << "){\n";
            src << "ostringstream msg;\n";
            src << "msg << \"" << name << " : " << b.varName
                << " is beyond its physical upper bound \";\n";
            src << "msg << \"(\" << " << b.varName << " << \" > "
                << b.upperBound << ")\";\n";
            src << "throw(range_error(msg.str()));\n";
            src << "}\n";
          }
        }
      }
      if (!bounds.empty()) {
        src << "// treating standard bounds\n";
        for (const auto& b : bounds) {
          if (b.boundsType == VariableBoundsDescription::Lower) {
            src << "if(" << b.varName << " < " << b.lowerBound << "){\n";
            src << "const char * const policy = "
                << "::getenv(\"OUT_OF_BOUNDS_POLICY\");\n";
            src << "if(policy!=nullptr){\n";
            src << "if(strcmp(policy,\"STRICT\")==0){\n";
            src << "ostringstream msg;\n";
            src << "msg << \"" << name << " : " << b.varName
                << " is below its lower bound \";\n";
            src << "msg << \"(\" << " << b.varName << " << \" < "
                << b.lowerBound << ")\";\n";
            src << "throw(range_error(msg.str()));\n";
            src << "} else if(strcmp(policy,\"WARNING\")==0){\n";
            src << "cerr << \"" << b.varName
                << " is below its lower bound \";\n";
            src << "cerr << \"(\" << " << b.varName << " << \" < "
                << b.lowerBound << ")\\n\";\n";
            src << "}\n";
            src << "}\n";
            src << "}\n";
          } else if (b.boundsType == VariableBoundsDescription::Upper) {
            src << "if(" << b.varName << " > " << b.upperBound << "){\n";
            src << "const char * const policy = "
                << "::getenv(\"OUT_OF_BOUNDS_POLICY\");\n";
            src << "if(policy!=nullptr){\n";
            src << "if(strcmp(policy,\"STRICT\")==0){\n";
            src << "ostringstream msg;\n";
            src << "msg << \"" << name << " : " << b.varName
                << " is beyond its upper bound \";\n";
            src << "msg << \"(\" << " << b.varName << " << \" > "
                << b.upperBound << ")\";\n";
            src << "throw(range_error(msg.str()));\n";
            src << "} else if(strcmp(policy,\"WARNING\")==0){\n";
            src << "cerr << \"" << b.varName
                << " is beyond its upper bound \";\n";
            src << "cerr << \"(\" << " << b.varName << " << \" > "
                << b.upperBound << ")\\n\";\n";
            src << "}\n";
            src << "}\n";
            src << "}\n";
          } else {
            src << "if(" << b.varName << " < " << b.lowerBound << "){\n";
            src << "const char * const policy = "
                << "::getenv(\"OUT_OF_BOUNDS_POLICY\");\n";
            src << "if(policy!=nullptr){\n";
            src << "if(strcmp(policy,\"STRICT\")==0){\n";
            src << "ostringstream msg;\n";
            src << "msg << \"" << name << " : " << b.varName
                << " is below its lower bound \";\n";
            src << "msg << \"(\" << " << b.varName << " << \" < "
                << b.lowerBound << ")\";\n";
            src << "throw(range_error(msg.str()));\n";
            src << "} else if(strcmp(policy,\"WARNING\")==0){\n";
            src << "cerr << \"" << b.varName
                << " is below its lower bound \";\n";
            src << "cerr << \"(\" << " << b.varName << " << \" < "
                << b.lowerBound << ")\\n\";\n";
            src << "}\n";
            src << "}\n";
            src << "}\n";
            src << "if(" << b.varName << " > " << b.upperBound << "){\n";
            src << "const char * const policy = "
                << "::getenv(\"OUT_OF_BOUNDS_POLICY\");\n";
            src << "if(policy!=nullptr){\n";
            src << "if(strcmp(policy,\"STRICT\")==0){\n";
            src << "ostringstream msg;\n";
            src << "msg << \"" << name << " : " << b.varName
                << " is beyond its upper bound \";\n";
            src << "msg << \"(\" << " << b.varName << " << \" > "
                << b.upperBound << ")\";\n";
            src << "throw(range_error(msg.str()));\n";
            src << "} else if(strcmp(policy,\"WARNING\")==0){\n";
            src << "cerr << \"" << b.varName
                << " is beyond its upper bound \";\n";
            src << "cerr << \"(\" << " << b.varName << " << \" > "
                << b.upperBound << ")\\n\";\n";
            src << "}\n";
            src << "}\n";
            src << "}\n";
          }
        }
      }
      src << "} // end of " << name << "::checkBounds\n\n";
    }
    src << "std::ostream&\n";
    src << "operator<<(std::ostream& os,const " << name;
    if (!params.empty()) {
      src << "& src){\n";
    } else {
      src << "&){\n";
    }
    for (p = params.begin(); p != params.end(); ++p) {
      src << "os << \"" << *p << " : \" << src.get" << *p
          << "() << std::endl;\n";
    }
    src << "return os;\n}// end of operator(std::ostream& os," << name
        << "\n\n";
    src << "} // end of namespace mfront\n\n";
    src.close();
  }  // end of CppMaterialPropertyInterface::writeSrcFile(void)

  CppMaterialPropertyInterface::~CppMaterialPropertyInterface() = default;

}  // end of namespace mfront
