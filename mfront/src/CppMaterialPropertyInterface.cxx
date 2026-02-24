/*!
 * \file   mfront/src/CppMaterialPropertyInterface.cxx
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

#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/CppMaterialPropertyInterface.hxx"

namespace mfront {

  static std::string getHeaderFileName(const std::string& n) {
    return n + "-cxx.hxx";
  }

  static void writeBoundsChecks(std::ostream& src,
                                const MaterialPropertyDescription& mpd,
                                const VariableDescription& v,
                                const std::string& name) {
    if (!v.hasBounds()) {
      return;
    }
    const auto& b = v.getBounds();
    const auto get_policy = [&mpd] {
      const auto default_policy =
          getDefaultOutOfBoundsPolicyAsUpperCaseString(mpd);
      if (allowRuntimeModificationOfTheOutOfBoundsPolicy(mpd)) {
        return "const char * const mfront_policy = "
               "[]{\n"
               " const auto* const p= "
               " ::getenv(\"OUT_OF_BOUNDS_POLICY\");\n"
               " if(p == nullptr){\n"
               " return \"" +
               default_policy +
               "\";\n"
               " }\n"
               " return p;\n"
               "}();\n";
      }
      return "const char * const mfront_policy = \"" +  //
             default_policy + "\";\n";
    }();
    if (b.boundsType == VariableBoundsDescription::LOWER) {
      src << "if(" << v.name << " < " << v.type << '{' << b.lowerBound
          << "}){\n"
          << get_policy  //
          << "if(::strcmp(mfront_policy,\"STRICT\")==0){\n"
          << "std::ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is below its lower bound \";\n"
          << "msg << \"(\" << " << v.name << " << \" < " << b.lowerBound
          << ")\";\n"
          << "tfel::raise<std::range_error>(msg.str());\n"
          << "} else if(::strcmp(mfront_policy,\"WARNING\")==0){\n"
          << "std::cerr << \"" << v.name << " is below its lower bound \";\n"
          << "std::cerr << \"(\" << " << v.name << " << \" < " << b.lowerBound
          << ")\\n\";\n"
          << "}\n"
          << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      src << "if(" << v.name << " > " << v.type << '{' << b.upperBound
          << "}){\n"
          << get_policy  //
          << "if(::strcmp(mfront_policy,\"STRICT\")==0){\n"
          << "std::ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is beyond its upper bound \";\n"
          << "msg << \"(\" << " << v.name << " << \" > " << b.upperBound
          << ")\";\n"
          << "tfel::raise<std::range_error>(msg.str());\n"
          << "} else if(::strcmp(mfront_policy,\"WARNING\")==0){\n"
          << "std::cerr << \"" << v.name << " is beyond its upper bound \";\n"
          << "std::cerr << \"(\" << " << v.name << " << \" > " << b.upperBound
          << ")\\n\";\n"
          << "}\n"
          << "}\n";
    } else {
      src << "if(" << v.name << " < " << v.type << '{' << b.lowerBound
          << "}){\n"
          << get_policy  //
          << "if(::strcmp(mfront_policy,\"STRICT\")==0){\n"
          << "std::ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is below its lower bound \";\n"
          << "msg << \"(\" << " << v.name << " << \" < " << b.lowerBound
          << ")\";\n"
          << "tfel::raise<std::range_error>(msg.str());\n"
          << "} else if(::strcmp(mfront_policy,\"WARNING\")==0){\n"
          << "std::cerr << \"" << v.name << " is below its lower bound \";\n"
          << "std::cerr << \"(\" << " << v.name << " << \" < " << b.lowerBound
          << ")\\n\";\n"
          << "}\n"
          << "}\n"
          << "if(" << v.name << " > " << v.type << '{' << b.upperBound
          << "}){\n"
          << get_policy  //
          << "if(::strcmp(mfront_policy,\"STRICT\")==0){\n"
          << "std::ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is beyond its upper bound \";\n"
          << "msg << \"(\" << " << v.name << " << \" > " << b.upperBound
          << ")\";\n"
          << "tfel::raise<std::range_error>(msg.str());\n"
          << "} else if(::strcmp(mfront_policy,\"WARNING\")==0){\n"
          << "std::cerr << \"" << v.name << " is beyond its upper bound \";\n"
          << "std::cerr << \"(\" << " << v.name << " << \" > " << b.upperBound
          << ")\\n\";\n"
          << "}\n"
          << "}\n";
    }
  }

  static void writePhysicalBoundsChecks(std::ostream& src,
                                        const VariableDescription& v,
                                        const std::string& name) {
    if (!v.hasPhysicalBounds()) {
      return;
    }
    const auto& b = v.getPhysicalBounds();
    if (b.boundsType == VariableBoundsDescription::LOWER) {
      src << "if(" << v.name << " < " << v.type << '{' << b.lowerBound
          << "}){\n"
          << "std::ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is below its physical lower bound \";\n"
          << "msg << \"(\" << " << v.name << " << \" < " << b.lowerBound
          << ")\";\n"
          << "tfel::raise<std::range_error>(msg.str());\n"
          << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      src << "if(" << v.name << " > " << v.type << '{' << b.upperBound
          << "}){\n"
          << "std::ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is beyond its physical upper bound \";\n"
          << "msg << \"(\" << " << v.name << " << \" > " << b.upperBound
          << ")\";\n"
          << "tfel::raise<std::range_error>(msg.str());\n"
          << "}\n";
    } else {
      src << "if(" << v.name << " < " << v.type << '{' << b.lowerBound
          << "}){\n"
          << "std::ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is below its physical lower bound \";\n"
          << "msg << \"(\" << " << v.name << " << \" < " << b.lowerBound
          << ")\";\n"
          << "tfel::raise<std::range_error>(msg.str());\n"
          << "}\n"
          << "if(" << v.name << " > " << v.type << '{' << b.upperBound
          << "}){\n"
          << "std::ostringstream msg;\n"
          << "msg << \"" << name << " : " << v.name
          << " is beyond its physical upper bound \";\n"
          << "msg << \"(\" << " << v.name << " << \" > " << b.upperBound
          << ")\";\n"
          << "tfel::raise<std::range_error>(msg.str());\n"
          << "}\n";
    }
  }

  std::string CppMaterialPropertyInterface::getName() { return "c++"; }

  CppMaterialPropertyInterface::CppMaterialPropertyInterface() = default;

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CppMaterialPropertyInterface::treatKeyword(const std::string& k,
                                             const std::vector<std::string>& i,
                                             tokens_iterator current,
                                             const tokens_iterator) {
    tfel::raise_if((std::find(i.begin(), i.end(), "c++") != i.end()) ||
                       (std::find(i.begin(), i.end(), "C++") != i.end()) ||
                       (std::find(i.begin(), i.end(), "cxx") != i.end()) ||
                       (std::find(i.begin(), i.end(), "Cxx") != i.end()) ||
                       (std::find(i.begin(), i.end(), "cpp") != i.end()) ||
                       (std::find(i.begin(), i.end(), "Cpp") != i.end()),
                   "CppMaterialPropertyInterface::treatKeyword: "
                   "unsupported keyword '" +
                       k + "'");
    return {false, current};
  }  // end of treatKeyword

  void CppMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const {
    const auto lib = "Cpp" + getMaterialLawLibraryNameBase(mpd);
    const auto name = mpd.material.empty() ? mpd.className
                                           : mpd.material + "_" + mpd.className;
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto& l = d.getLibrary(lib);
    insert_if(l.sources, name + "-cxx.cxx");
    insert_if(d.headers, getHeaderFileName(name));
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(l.link_libraries, "m");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    auto cn = std::string{};
#pragma message("handle namespace")
    // for(const auto& ns : this->namespaces){
    //   cc += ns + "::"
    // }
    cn += name;
    insert_if(l.epts, cn);
  }  // end of getTargetsDescription

  void CppMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    this->writeHeaderFile(mpd, fd);
    this->writeSrcFile(mpd, fd);
  }  // end of writeOutputFiles

  void CppMaterialPropertyInterface::writeHeaderFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto name = mpd.material.empty() ? mpd.className
                                           : mpd.material + "_" + mpd.className;
    std::ofstream header("include/" + getHeaderFileName(name));
    tfel::raise_if(!header,
                   "CppMaterialPropertyInterface::writeHeaderFile: "
                   "unable to open '" +
                       getHeaderFileName(name) +
                       "' "
                       "for writing output file.");
    header.exceptions(std::ios::badbit | std::ios::failbit);
    header << "/*!\n"
           << "* \\file   " << getHeaderFileName(name) << '\n'
           << "* \\brief  "
           << "this file declares the " << name << " MaterialLaw.\n"
           << "*         File generated by " << MFrontHeader::getVersionName()
           << " "
           << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!fd.authorName.empty()) {
      header << "* \\author " << fd.authorName << '\n';
    }
    if (!fd.date.empty()) {
      header << "* \\date   " << fd.date << '\n';
    }
    if (!fd.description.empty()) {
      header << fd.description << '\n';
    }
    header << " */\n\n";

    header << "#ifndef LIB_MFRONT_" << makeUpperCase(name) << "_HXX\n"
           << "#define LIB_MFRONT_" << makeUpperCase(name) << "_HXX\n\n"
           << "#include<ostream>\n"
           << "#include<cmath>\n"
           << "#include<algorithm>\n"
           << "#include<stdexcept>\n"
           << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
           << "#include\"TFEL/PhysicalConstants.hxx\"\n"
           << "#include\"TFEL/Math/General/IEEE754.hxx\"\n\n"
           << "#include\"TFEL/Math/General/DerivativeType.hxx\"\n";
    if (useQuantities(mpd)) {
      header << "#include\"TFEL/Math/qt.hxx\"\n"
             << "#include\"TFEL/Math/Quantity/qtIO.hxx\"\n";
    }
    if (!mpd.includes.empty()) {
      header << mpd.includes << '\n';
    }
    writeExportDirectives(header);
    header << "namespace mfront\n{\n\n"
           << "struct MFRONT_SHAREDOBJ " << name << '\n';
    header << "{\n\n";
    if (mpd.inputs.empty()) {
      header << "//! nested typedef to make " << name
             << " model an adaptable generator (STL compliance)\n\n";
      header << "using result_type =  double;\n\n";
    }
    const auto use_qt = useQuantities(mpd) ? "true" : "false";
    header << "[[maybe_unused]] static constexpr auto use_qt = "  //
           << use_qt << ";\n";
    writeScalarStandardTypedefs(header, mpd, "double", true);
    header << "//! \\brief default constructor\n"
           << name << "() noexcept;\n\n"
           << "//! \\brief move constructor\n"
           << name << "(" << name << "&&) noexcept = default;\n"
           << "//! \\brief copy constructor\n"
           << name << "(const " << name << "&) noexcept = default;\n"
           << "//! \\brief move assignement operator\n"
           << name << "& operator=(" << name << "&&) noexcept = default;\n"
           << "//! \\brief assignement operator\n"
           << name << "& operator=(const " << name
           << "&) noexcept = default;\n\n"
           << "double\noperator()(";
    for (auto p4 = mpd.inputs.begin(); p4 != mpd.inputs.end();) {
      header << "const double";
      if ((++p4) != mpd.inputs.end()) {
        header << ",";
      }
    }
    header << ") const;\n\n";
    if ((hasBounds(mpd.inputs)) || (hasPhysicalBounds(mpd.inputs))) {
      header << "static void\ncheckBounds(";
      for (auto p4 = mpd.inputs.begin(); p4 != mpd.inputs.end();) {
        header << "const double";
        if ((++p4) != mpd.inputs.end()) {
          header << ",";
        }
      }
      header << ");\n\n";
    }
    for (const auto& p : mpd.parameters) {
      const auto t = useQuantities(mpd) ? p.type : "double";
      header << "auto get" << p.name << "() const -> const " << t << "&;\n";
      header << "auto get" << p.name << "() -> " << t << "&;\n";
      header << "void set" << p.name << "(const double);\n";
    }
    if (!mpd.parameters.empty()) {
      header << "private:\n";
      for (const auto& p : mpd.parameters) {
        const auto t = useQuantities(mpd) ? p.type : "double";
        header << t << " " << p.name << ";\n";
      }
    }
    header << "}; // end of class " << name << "\n\n"
           << "std::ostream& operator<<(std::ostream&,const " << name
           << "&);\n\n"
           << "} // end of namespace mfront\n\n"
           << "#endif /* LIB_MFRONT_" << makeUpperCase(name) << "_HXX */\n";
    header.close();
  }  // end of writeHeaderFile()

  void CppMaterialPropertyInterface::writeSrcFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "CppMaterialPropertyInterface::writeSrcFile: " + m);
    };
    const auto name = mpd.material.empty() ? mpd.className
                                           : mpd.material + "_" + mpd.className;
    const auto src_name = "src/" + name + "-cxx.cxx";
    std::ofstream src(src_name);
    throw_if(!src, "unable to open '" + src_name + "'");
    src.exceptions(std::ios::badbit | std::ios::failbit);
    src << "/*!\n"
        << "* \\file   " << src_name << '\n'
        << "* \\brief  "
        << "this file implements the " << name << " MaterialLaw.\n"
        << "*         File generated by " << MFrontHeader::getVersionName()
        << " "
        << "version " << MFrontHeader::getVersionNumber() << '\n';
    if (!fd.authorName.empty()) {
      src << "* \\author " << fd.authorName << '\n';
    }
    if (!fd.date.empty()) {
      src << "* \\date   " << fd.date << '\n';
    }
    src << " */\n\n"
        << "#include<stdexcept>\n"
        << "#include<iostream>\n"
        << "#include<cstdlib>\n"
        << "#include<sstream>\n"
        << "#include<cstring>\n"
        << "#include<vector>\n"
        << "#include<string>\n"
        << "#include\"TFEL/Raise.hxx\"\n"
        << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
        << "#include\"TFEL/Math/General/IEEE754.hxx\"\n"
        << "#include\"" << name << "-cxx.hxx\"\n\n";
    writeExportDirectives(src);
    src << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";
    // mfront metadata
    writeFileDescriptionSymbols(src, name, fd);
    writeVariablesNamesSymbol(src, name, mpd);
    writeVariablesBoundsSymbols(src, name, mpd);
    writeEntryPointSymbol(src, name);
    writeLawSymbol(src, name, mpd.law);
    writeTFELVersionSymbol(src, name);
    writeUnitSystemSymbol(src, name, mpd);
    writeInterfaceSymbol(src, name, "C++");
    writeMaterialSymbol(src, name, mpd.material);
    writeMaterialKnowledgeTypeSymbol(src, name,
                                     MaterialKnowledgeType::MATERIALPROPERTY);
    src << "#ifdef __cplusplus\n"
        << "} // end of extern \"C\"\n"
        << "#endif /* __cplusplus */\n\n";
    src << "namespace mfront\n{\n\n" << name << "::" << name << "() noexcept\n";
    if (!mpd.parameters.empty()) {
      src << ": ";
    }
    for (auto p = mpd.parameters.begin(); p != mpd.parameters.end();) {
      throw_if(!p->hasAttribute(VariableDescription::defaultValue),
               "internal error (can't find value of "
               "parameter '" +
                   p->name + "')");
      src << p->name << "("
          << p->getAttribute<double>(VariableDescription::defaultValue) << ")";
      if (++p != mpd.parameters.end()) {
        src << ",";
      }
      src << "\n";
    }
    src << "{} // end of " << name << "::" << name << "\n\n";
    for (const auto& p : mpd.parameters) {
      const auto t = useQuantities(mpd) ? p.type : "double";
      src << "auto " << name << "::get" << p.name << "() const "  //
          << "-> const " << t << "& {\n"
          << "return this->" << p.name << ";\n"
          << "} // end of " << name << "::get" << p.name << "\n\n";
      src << "auto " << name << "::get" << p.name << "() -> " << t << "& {\n"
          << "return " << p.name << ";\n"
          << "} // end of " << name << "::get\n\n";
      src << "void " << name << "::set" << p.name  //
          << "(const double mfront_" << name << "){\n";
      if (useQuantities(mpd)) {
        src << "this->" << p.name << " = "  //
            << p.type << "(mfront_" << name << ");\n";
      } else {
        src << "this->" << p.name << " = mfront_" << name << ";\n";
      }
      src << "} // end of " << name << "::set" << p.name << "\n\n";
    }
    src << "double " << name << "::operator()(";
    for (auto pi = mpd.inputs.begin(); pi != mpd.inputs.end();) {
      if (useQuantities(mpd)) {
        src << "const double mfront_" << pi->name;
      } else {
        src << "const double " << pi->name;
      }
      if (++pi != mpd.inputs.end()) {
        src << ",";
      }
    }
    src << ") const\n{\n"
        << "using namespace std;\n"
        << "using tfel::math::invert_type;\n"
        << "using tfel::math::result_type;\n"
        << "using tfel::math::derivative_type;\n";
    if (useQuantities(mpd)) {
      src << "using PhysicalConstants [[maybe_unused]] = "
          << "tfel::PhysicalConstants<double, true>;\n";
    } else {
      src << "using PhysicalConstants [[maybe_unused]] = "
          << "tfel::PhysicalConstants<double, false>;\n";
    }
    src << "[[maybe_unused]] auto min = [](const auto a, const auto b) "
        << "{ return a < b ? a : b; };\n"
        << "[[maybe_unused]] auto max = [](const auto a, const auto b) "
        << "{ return a > b ? a : b; };\n";
    writeMaterialLaws(src, mpd.materialLaws);
    writeStaticVariables(src, mpd.staticVars, fd.fileName);
    for (const auto& i : mpd.inputs) {
      if (useQuantities(mpd)) {
        src << "const auto " << i.name << " = "  //
            << i.type << "(mfront_" << i.name << ");\n";
      }
    }
    // body
    if (useQuantities(mpd)) {
      src << "auto " << mpd.output.name << " = " << mpd.output.type << "{};\n";
    } else {
      src << "auto " << mpd.output.name << " = real{};\n";
    }
    if (!areRuntimeChecksDisabled(mpd)) {
      if ((hasBounds(mpd.inputs)) || (hasPhysicalBounds(mpd.inputs))) {
        src << "#ifndef MFRONT_NO_BOUNDS_CHECK\n";
        src << name << "::checkBounds(";
        for (auto pi = mpd.inputs.begin(); pi != mpd.inputs.end();) {
          src << pi->name;
          if (useQuantities(mpd)) {
            src << ".getValue()";
          }
          if ((++pi) != mpd.inputs.end()) {
            src << ",";
          }
        }
        src << ");\n";
        src << "#endif /* MFRONT_NO_BOUNDS_CHECK */\n";
      }
      if (!mpd.inputs.empty()) {
        src << "#ifndef MFRONT_NOERRNO_HANDLING\n"
            << "const auto mfront_errno_old = errno;\n"
            << "errno=0;\n"
            << "#endif /* MFRONT_NOERRNO_HANDLING */\n";
      }
    }
    src << mpd.f.body;
    if (!areRuntimeChecksDisabled(mpd)) {
      if (!mpd.inputs.empty()) {
        src << "#ifndef MFRONT_NOERRNO_HANDLING\n"
            // can't use std::swap here as errno might be a macro
            << "const auto mfront_errno = errno;\n"
            << "errno = mfront_errno_old;\n"
            << "tfel::raise_if((mfront_errno!=0)||"
            << "(!tfel::math::ieee754::isfinite(" << mpd.output.name << ")),\n"
            << "\"" << name << ": errno has been set \"\n"
            << "\"(\"+std::string(::strerror(errno))+\")\");\n"
            << "#endif /* MFRONT_NOERRNO_HANDLING */\n";
      }
      //
      writePhysicalBoundsChecks(src, mpd.output, name);
      writeBoundsChecks(src, mpd, mpd.output, name);
    }
    //
    if (useQuantities(mpd)) {
      src << "return " << mpd.output.name << ".getValue();\n";
    } else {
      src << "return " << mpd.output.name << ";\n";
    }
    src << "} // end of " << name << "::operator()\n\n";
    if ((hasBounds(mpd.inputs)) || (hasPhysicalBounds(mpd.inputs))) {
      src << "void\n";
      src << name;
      src << "::checkBounds(";
      if (!mpd.inputs.empty()) {
        for (auto pi = mpd.inputs.begin(); pi != mpd.inputs.end();) {
          if (useQuantities(mpd)) {
            src << "[[maybe_unused]] const double mfront_" << pi->name;
          } else {
            src << "const double " << pi->name;
          }
          if ((++pi) != mpd.inputs.end()) {
            src << ",";
          }
        }
      } else {
        src << "void";
      }
      src << ")\n{\n"
          << "using namespace std;\n";
      for (const auto& i : mpd.inputs) {
        if (useQuantities(mpd)) {
          src << "[[maybe_unused]] const auto " << i.name << " = "  //
              << i.type << "(mfront_" << i.name << ");\n";
        }
      }
      if (!areRuntimeChecksDisabled(mpd)) {
        if (hasPhysicalBounds(mpd.inputs)) {
          src << "// treating physical bounds\n";
          for (const auto& i : mpd.inputs) {
            writePhysicalBoundsChecks(src, i, name);
          }
        }
        if (hasBounds(mpd.inputs)) {
          src << "// treating standard bounds\n";
          if (!((!allowRuntimeModificationOfTheOutOfBoundsPolicy(mpd)) &&
                (getDefaultOutOfBoundsPolicy(mpd) == tfel::material::None))) {
            for (const auto& i : mpd.inputs) {
              writeBoundsChecks(src, mpd, i, name);
            }
          }
        }
      }
      src << "} // end of " << name << "::checkBounds\n\n";
    }
    src << "std::ostream& operator<<(std::ostream& os,const " << name;
    if (!mpd.parameters.empty()) {
      src << "& src){\n";
    } else {
      src << "&){\n";
    }
    for (const auto& p : mpd.parameters) {
      src << "os << \"" << p.name << " : \" << src.get" << p.name
          << "() << std::endl;\n";
    }
    src << "return os;\n}// end of operator(std::ostream& os," << name << "\n\n"
        << "} // end of namespace mfront\n\n";
    src.close();
  }  // end of writeSrcFile

  CppMaterialPropertyInterface::~CppMaterialPropertyInterface() = default;

}  // end of namespace mfront
