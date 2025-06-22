/*!
 * \file   mfront/src/GenericMaterialPropertyInterfaceBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04/05/2022
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
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/LibraryDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyParametersHandler.hxx"
#include "MFront/GenericMaterialPropertyInterfaceBase.hxx"

namespace mfront {

  GenericMaterialPropertyInterfaceBase::GenericMaterialPropertyInterfaceBase() =
      default;

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  GenericMaterialPropertyInterfaceBase::treatKeyword(
      const std::string& k,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator) {
    const auto iname = this->getInterfaceName();
    const auto iccname = this->getInterfaceNameInCamelCase();
    tfel::raise_if((std::find(i.begin(), i.end(), iname) != i.end()) ||
                       (std::find(i.begin(), i.end(), iccname) != i.end()),
                   "GenericMaterialPropertyInterfaceBase::treatKeyword: "
                   "unsupported keyword '" +
                       k + "'");
    return {false, current};
  }  // end of treatKeyword

  void GenericMaterialPropertyInterfaceBase::getLibraryDescription(
      TargetsDescription& d,
      LibraryDescription& l,
      const MaterialPropertyDescription& mpd) const {
    const auto name = this->getFunctionName(mpd);
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(l.sources, this->getSourceFileName(name));
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(l.link_libraries, "m");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    insert_if(l.epts, name);
    d.headers.push_back(this->getHeaderFileName(name));
  }  // end of getLibraryDescription

  void GenericMaterialPropertyInterfaceBase::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const {
    const auto iccname = this->getInterfaceNameInCamelCase();
    const auto lib = iccname + getMaterialLawLibraryNameBase(mpd);
    this->getLibraryDescription(d, d.getLibrary(lib), mpd);
  }  // end of getTargetsDescription

  std::string GenericMaterialPropertyInterfaceBase::getFunctionName(
      const MaterialPropertyDescription& mpd) const {
    const auto& material = mpd.material;
    const auto& className = mpd.className;
    if (material.empty()) {
      return className;
    }
    return material + "_" + className;
  }

  std::string GenericMaterialPropertyInterfaceBase::getHeaderFileName(
      const std::string& name) const {
    const auto i = this->getInterfaceName();
    return name + "-" + i + ".hxx";
  }

  std::string GenericMaterialPropertyInterfaceBase::getSourceFileName(
      const std::string& name) const {
    const auto i = this->getInterfaceName();
    return name + "-" + i + ".cxx";
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
         << "mfront_report(\"" << v.name
         << " is below its physical lower bound (\" + " << to_string << " + \"<"
         << b.lowerBound << ").\\n\");\n"
         << "mfront_output_status->status = -1;\n"
         << "mfront_output_status->bounds_status = -" << i << ";\n"
         << "errno = mfront_errno_old;\n"
         << "return std::nan(\"\");\n"
         << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      os << "if(" << v.name << " > " << v.type << "(" << b.upperBound << ")){\n"
         << "mfront_report(\"" << v.name
         << " is below its physical upper bound (\" + " << to_string << " + \">"
         << b.upperBound << ").\\n\");\n"
         << "mfront_output_status->status = -1;\n"
         << "mfront_output_status->bounds_status = -" << i << ";\n"
         << "errno = mfront_errno_old;\n"
         << "return std::nan(\"\");\n"
         << "}\n";
    } else {
      os << "if((" << v.name << " < " << v.type << "(" << b.lowerBound << "))||"
         << "(" << v.name << " > " << v.type << "(" << b.upperBound << "))){\n"
         << "if(" << v.name << " < " << v.type << "(" << b.lowerBound << ")){\n"
         << "mfront_report(\"" << v.name
         << " is below its physical lower bound (\" + " << to_string << " + \"<"
         << b.lowerBound << ").\\n\");\n"
         << "} else {\n"
         << "mfront_report(\"" << v.name
         << " is over its physical upper bound (\" + " << to_string << " + \">"
         << b.upperBound << ").\\n\");\n"
         << "}\n"
         << "mfront_output_status->status = -1;\n"
         << "mfront_output_status->bounds_status = -" << i << ";\n"
         << "errno = mfront_errno_old;\n"
         << "return std::nan(\"\");\n"
         << "}\n";
    }
  }  // end of writePhysicalBounds

  static void writeBounds(std::ostream& os,
                          const std::string& iucname,
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
         << "if(mfront_out_of_bounds_policy==" << iucname
         << "_STRICT_POLICY){\n"
         << "mfront_report(\"" << v.name << " is out of bounds.\");\n"
         << "mfront_output_status->status = -1;\n"
         << "mfront_output_status->bounds_status = -" << i << ";\n"
         << "errno = mfront_errno_old;\n"
         << "return std::nan(\"\");\n"
         << "} else if (mfront_out_of_bounds_policy==" << iucname
         << "_WARNING_POLICY){\n"
         << "mfront_output_status->status = 1;\n"
         << "mfront_output_status->bounds_status = " << i << ";\n"
         << "mfront_report(\"" << v.name << " is below its lower bound (\" + "
         << to_string << " + \"<" << b.lowerBound << ").\\n\");\n"
         << "}\n"
         << "}\n";
    } else if (b.boundsType == VariableBoundsDescription::UPPER) {
      os << "if(" << v.name << " > " << v.type << "(" << b.upperBound << ")){\n"
         << "if(mfront_out_of_bounds_policy==" << iucname
         << "_STRICT_POLICY){\n"
         << "mfront_report(\"" << v.name << " is over its upper bound (\" + "
         << to_string << " + \">" << b.upperBound << ").\\n\");\n"
         << "mfront_output_status->status = -1;\n"
         << "mfront_output_status->bounds_status = -" << i << ";\n"
         << "return std::nan(\"\");\n"
         << "} else if (mfront_out_of_bounds_policy==" << iucname
         << "_WARNING_POLICY){\n"
         << "mfront_output_status->status = 1;\n"
         << "mfront_output_status->bounds_status = " << i << ";\n"
         << "mfront_report(\"" << v.name << " is over its upper bound (\" + "
         << to_string << " + \">" << b.upperBound << ").\\n\");\n"
         << "}\n"
         << "}\n";
    } else {
      os << "if((" << v.name << " < " << v.type << "(" << b.lowerBound << "))||"
         << "(" << v.name << " > " << v.type << "(" << b.upperBound << "))){\n"
         << "if(mfront_out_of_bounds_policy==" << iucname
         << "_STRICT_POLICY){\n"
         << "if(" << v.name << " < " << v.type << "(" << b.lowerBound << ")){\n"
         << "mfront_report(\"" << v.name << " is below its lower bound (\" + "
         << to_string << " + \"<" << b.lowerBound << ").\\n\");\n"
         << "} else {\n"
         << "mfront_report(\"" << v.name << " is over its upper bound (\" + "
         << to_string << " + \">" << b.upperBound << ").\\n\");\n"
         << "}\n"
         << "mfront_output_status->status = -1;\n"
         << "mfront_output_status->bounds_status = -" << i << ";\n"
         << "errno = mfront_errno_old;\n"
         << "return std::nan(\"\");\n"
         << "} else if (mfront_out_of_bounds_policy==" << iucname
         << "_WARNING_POLICY){\n"
         << "mfront_output_status->status = 1;\n"
         << "if(" << v.name << " < " << v.type << "(" << b.lowerBound << ")){\n"
         << "mfront_output_status->bounds_status = " << i << ";\n"
         << "mfront_report(\"" << v.name << " is below its lower bound (\" + "
         << to_string << " + \"<" << b.lowerBound << ").\\n\");\n"
         << "} else {\n"
         << "mfront_output_status->bounds_status = " << i << ";\n"
         << "mfront_report(\"" << v.name << " is over its upper bound (\" + "
         << to_string << " + \">" << b.upperBound << ").\\n\");\n"
         << "}\n"
         << "}\n"
         << "}\n";
    }
  }  // end of writeBounds

  void GenericMaterialPropertyInterfaceBase::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    this->writeHeaderFile(mpd, fd);
    this->writeSrcFile(mpd, fd);
  }  // end of writeOutputFiles

  void GenericMaterialPropertyInterfaceBase::writeHeaderFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto iucname = this->getInterfaceNameInUpperCase();
    const auto types = this->getTypesDescription();
    const auto name = this->getFunctionName(mpd);
    const auto fn = "include/" + this->getHeaderFileName(name);
    std::ofstream os{fn};
    tfel::raise_if(!os,
                   "GenericMaterialPropertyInterfaceBase::writeOutputFiles: "
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

    os << "#ifndef " << makeUpperCase(name) << "_" << iucname << "_HH\n"
       << "#define " << makeUpperCase(name) << "_" << iucname << "_HH\n\n";
    writeExportDirectives(os);

    for (const auto& header : this->getInterfaceHeaderFiles()) {
      os << "#include \"" << header << "\"\n\n";
    }
    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n"
       << "MFRONT_SHAREDOBJ " << types.real_type << "\n"
       << name << "(" << types.output_status_type << "* const,"
       << "const " << types.real_type << "* const,"
       << "const " << types.integer_type << ","
       << "const " << types.out_of_bounds_policy_type << ");\n\n";
    if (!mpd.parameters.empty()) {
      os << "MFRONT_SHAREDOBJ int\n"
         << name << "_setParameter(const char *const,"
         << "const double);\n\n";
    }
    os << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n"
       << "#endif /* " << makeUpperCase(name) << "_" << iucname << "_HH */\n";
    os.close();
  }  // end of writeHeaderFile()

  void GenericMaterialPropertyInterfaceBase::writeSrcFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto types = this->getTypesDescription();
    const auto iucname = this->getInterfaceNameInUpperCase();
    const auto iname = this->getInterfaceName();
    const auto prefix = this->getOutOfBoundsPolicyEnumerationPrefix();
    const auto name = this->getFunctionName(mpd);
    const auto fn = "src/" + this->getSourceFileName(name);
    std::ofstream os{fn};
    tfel::raise_if(!os,
                   "GenericMaterialPropertyInterfaceBase::writeOutputFiles: "
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
       << "#include\"TFEL/PhysicalConstants.hxx\"\n"
       << "#include\"TFEL/Math/General/IEEE754.hxx\"\n"
       << "#include\"TFEL/Math/General/DerivativeType.hxx\"\n";
    if (useQuantities(mpd)) {
      os << "#include\"TFEL/Math/qt.hxx\"\n"
         << "#include\"TFEL/Math/Quantity/qtIO.hxx\"\n";
    }
    os << "#include\"" << name << "-" << iname << ".hxx\"\n\n";
    if (!includes.empty()) {
      os << includes << "\n\n";
    }

    writeMaterialPropertyParametersHandler(os, mpd, name, "double", iname);

    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n";

    writeFileDescriptionSymbols(os, name, fd);
    writeVariablesNamesSymbol(os, name, mpd);
    writeVariablesBoundsSymbols(os, name, mpd);
    writeValidatorSymbol(os, name, mpd);
    writeBuildIdentifierSymbol(os, name, mpd);
    writeEntryPointSymbol(os, name);
    writeTFELVersionSymbol(os, name);
    writeUnitSystemSymbol(os, name, mpd);
    writeInterfaceSymbol(os, name, this->getInterfaceNameInCamelCase());
    writeLawSymbol(os, name, mpd.law);
    writeMaterialSymbol(os, name, mpd.material);
    writeMaterialKnowledgeTypeSymbol(os, name,
                                     MaterialKnowledgeType::MATERIALPROPERTY);
    writeParametersSymbols(os, name, mpd);
    exportStringSymbol(
        os, name + "_src",
        tfel::utilities::tokenize(file, tfel::system::dirSeparator()).back());

    if ((!areParametersTreatedAsStaticVariables(mpd)) && (!params.empty())) {
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      os << "MFRONT_SHAREDOBJ int\n"
         << name << "_setParameter(const char *const p,"
         << "const double v"
         << "){\n";
      for (const auto& p : params) {
        os << "if(strcmp(\"" << p.name << "\",p) == 0){\n"
           << iname << "::" << hn << "::get" << hn << "()." << p.name
           << " = v;\n"
           << "return 1;\n"
           << "}\n";
      }
      os << "return 0;\n"
         << "}\n\n";
    }

    os << "MFRONT_SHAREDOBJ " << types.real_type << "\n" << name << "(";
    if (!mpd.inputs.empty()) {
      os << "" << types.output_status_type << "* const mfront_output_status,\n"
         << "const " << types.real_type << "* const mfront_params,\n"
         << "const " << types.integer_type << " mfront_nargs,\n";
      if ((hasBounds(mpd.inputs)) || (hasBounds(mpd.output))) {
        os << "const " << types.out_of_bounds_policy_type
           << " mfront_out_of_bounds_policy";
      } else {
        os << "const " << types.out_of_bounds_policy_type << "";
      }
    } else {
      os << "" << types.output_status_type << "* const mfront_output_status,\n"
         << "const " << types.real_type << "* const,\n"
         << "const " << types.integer_type << " mfront_nargs,\n";
      if ((hasBounds(mpd.output)) || (hasPhysicalBounds(mpd.output))) {
        os << "const " << types.out_of_bounds_policy_type
           << " mfront_out_of_bounds_policy";
      } else {
        os << "const " << types.out_of_bounds_policy_type << "";
      }
    }
    os << ")\n{\n";
    writeBeginningOfMaterialPropertyBody(os, mpd, fd, "double", true);
    os << "auto mfront_report = "
       << "[&mfront_output_status](const std::string& "
       << "mfront_error_message){\n"
       << "if(mfront_error_message.empty()){\n"
       << "return;\n"
       << "}\n"
       << "std::strncpy(mfront_output_status->msg,"
       << "mfront_error_message.c_str(),511);\n"
       << "mfront_output_status->msg[511]='\\0';\n"
       << "};\n";
    if (!areRuntimeChecksDisabled(mpd)) {
      os << "const int mfront_errno_old = errno;\n";
    }
    os << "mfront_output_status->status = 0;\n"
       << "mfront_output_status->bounds_status = 0;\n"
       << "mfront_output_status->c_error_number = 0;\n";
    if (!areRuntimeChecksDisabled(mpd)) {
      // C-error handling
      os << "errno = 0;\n";
      // check number of arguments
      os << "if(mfront_nargs!= " << mpd.inputs.size() << "){\n"
         << "mfront_output_status->status = -5;\n"
         << "mfront_report(\"invalid number of arguments "
         << "(\"+std::to_string(mfront_nargs)+\" given, " << mpd.inputs.size()
         << " expected)\");\n"
         << "errno = mfront_errno_old;\n"
         << "return std::nan(\"invalid number of arguments\");\n"
         << "}\n";
      // parameters
      if ((!areParametersTreatedAsStaticVariables(mpd)) && (!params.empty())) {
        const auto hn = getMaterialPropertyParametersHandlerClassName(name);
        os << "if(!" << iname << "::" << hn << "::get" << hn << "().ok){\n"
           << "mfront_output_status->status = -6;\n"
           << "mfront_report(" << iname << "::" << name
           << "MaterialPropertyHandler::get" << name
           << "MaterialPropertyHandler().msg);\n"
           << "errno = mfront_errno_old;\n"
           << "return std::nan(" << iname << "::" << name
           << "MaterialPropertyHandler::get" << name
           << "MaterialPropertyHandler().msg.c_str());\n"
           << "}\n";
      }
    }
    writeAssignMaterialPropertyParameters(os, mpd, name, "real", iname);
    //
    if (!mpd.inputs.empty()) {
      auto p3 = mpd.inputs.begin();
      for (auto i = 0u; p3 != mpd.inputs.end(); ++p3, ++i) {
        auto cast_start = useQuantities(mpd) ? p3->type + "(" : "";
        auto cast_end = useQuantities(mpd) ? ")" : "";
        os << "const auto " << p3->name << " = " << cast_start;
        if (i == 0) {
          os << "*(mfront_params)";
        } else {
          os << "*(mfront_params+" + std::to_string(i) + "u)";
        }
        os << cast_end << ";\n";
      }
    }
    os << "auto " << mpd.output.name << " = " << mpd.output.type << "{};\n";
    os << "try{\n";
    if (!areRuntimeChecksDisabled(mpd)) {
      if ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs))) {
        os << "#ifndef NO_" << iucname << "_BOUNDS_CHECK\n";
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
          writeBounds(os, prefix, mpd.inputs[i], i + 1, useQuantities(mpd));
        }
      }
      if ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs))) {
        os << "#endif /* NO_" << iucname << "_BOUNDS_CHECK */\n";
      }
    }
    os << function.body;
    if (!areRuntimeChecksDisabled(mpd)) {
      if ((mpd.output.hasPhysicalBounds()) || (mpd.output.hasBounds())) {
        os << "#ifndef NO_" << iucname << "_BOUNDS_CHECK\n";
        if (mpd.output.hasPhysicalBounds()) {
          os << "// treating physical bounds\n";
          writePhysicalBounds(os, mpd.output, mpd.inputs.size() + 1,
                              useQuantities(mpd));
        }
        if (mpd.output.hasBounds()) {
          os << "// treating bounds\n";
          writeBounds(os, prefix, mpd.output, mpd.inputs.size() + 1,
                      useQuantities(mpd));
        }
        os << "#endif /* NO_" << iucname << "_BOUNDS_CHECK */\n";
      }
    }
    os << "} catch(std::exception& e){\n"
       << "mfront_output_status->status = -2;\n"
       << "mfront_report(e.what());\n";
    if (!areRuntimeChecksDisabled(mpd)) {
      os << "errno = mfront_errno_old;\n";
    }
    os << "return std::nan(\"\");\n"
       << "} catch(...){\n"
       << "mfront_output_status->status = -2;\n"
       << "mfront_report(\"unknown C++ exception\");\n";
    if (!areRuntimeChecksDisabled(mpd)) {
      os << "errno = mfront_errno_old;\n";
    }
    os << "return nan(\"\");\n"
       << "}\n";
    if (!areRuntimeChecksDisabled(mpd)) {
      os << "if (errno != 0) {\n"
         << "mfront_output_status->status = -3;\n"
         << "mfront_output_status->c_error_number = errno;\n"
         << "mfront_report(strerror(errno));\n"
         << "}\n"
         << "errno = mfront_errno_old;\n"
         << "if(!tfel::math::ieee754::isfinite(" << mpd.output.name << ")){\n"
         << "mfront_output_status->status = -4;\n"
         << "}\n";
    }
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
  }  // end of writeSrcFile()

  GenericMaterialPropertyInterfaceBase::
      ~GenericMaterialPropertyInterfaceBase() = default;

}  // end of namespace mfront
