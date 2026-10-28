/*!
 * \file   mfront/src/GenericMaterialPropertyInterfaceBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date 04/05/2022
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

  void GenericMaterialPropertyInterfaceBase::setOptions(const DataMap& opts) {
    if (!opts.empty()) {
      tfel::raise("no options expected for interface '" +
                  this->getInterfaceName() + "'");
    }
  }  // end of setOptions

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

  unsigned long long
  GenericMaterialPropertyInterfaceBase::getPhysicalBoundCheck(
      const std::size_t i, const bool upper) {
    return 1 + 2 * i + (upper ? 0 : 1);
  }  // end of getPhysicalBoundCheck

  unsigned long long GenericMaterialPropertyInterfaceBase::getBoundCheck(
      const MaterialPropertyDescription& mpd,
      const std::size_t i,
      const bool upper) {
    return 1 + 2 * (mpd.inputs.size() + 1) + 2 * i + (upper ? 0 : 1);
  }  // end of getBoundCheck

  void GenericMaterialPropertyInterfaceBase::writePointEvaluation(
      std::ostream& os,
      const MaterialPropertyDescription& mpd,
      const PointEvaluationOptions& opts) {
    const auto checks = !areRuntimeChecksDisabled(mpd);
    auto has_lower_bound = [](const VariableBoundsDescription& b) {
      return (b.boundsType == VariableBoundsDescription::LOWER) ||
             (b.boundsType == VariableBoundsDescription::LOWERANDUPPER);
    };
    auto has_upper_bound = [](const VariableBoundsDescription& b) {
      return (b.boundsType == VariableBoundsDescription::UPPER) ||
             (b.boundsType == VariableBoundsDescription::LOWERANDUPPER);
    };
    // the physical bounds always make the evaluation fail
    auto write_physical_bounds =
        [&os, &opts, &has_lower_bound, &has_upper_bound](
            const VariableDescription& v, const std::size_t idx) {
          if (!v.hasPhysicalBounds()) {
            return;
          }
          const auto& b = v.getPhysicalBounds();
          auto write = [&os, &opts, &v, idx](const char* const op,
                                             const long double value,
                                             const bool upper) {
            os << "if(" << v.name << " " << op << " " << v.type << "(" << value
               << ")){\n";
            opts.failure(os, BoundCheck{.variable = v,
                                        .rank = idx + 1,
                                        .physical = true,
                                        .upper = upper,
                                        .bound = value,
                                        .identifier =
                                            getPhysicalBoundCheck(idx, upper)});
            os << "}\n";
          };
          if (has_lower_bound(b)) {
            write("<", b.lowerBound, false);
          }
          if (has_upper_bound(b)) {
            write(">", b.upperBound, true);
          }
        };
    // the bounds make the evaluation fail with the strict policy, and are
    // reported with the warning policy
    auto write_bounds = [&os, &opts, &mpd, &has_lower_bound, &has_upper_bound](
                            const VariableDescription& v,
                            const std::size_t idx) {
      if (!v.hasBounds()) {
        return;
      }
      const auto& b = v.getBounds();
      const auto& prefix = opts.out_of_bounds_policy_prefix;
      auto write = [&os, &opts, &mpd, &v, &prefix, idx](const char* const op,
                                                        const long double value,
                                                        const bool upper) {
        const auto c = BoundCheck{.variable = v,
                                  .rank = idx + 1,
                                  .physical = false,
                                  .upper = upper,
                                  .bound = value,
                                  .identifier = getBoundCheck(mpd, idx, upper)};
        os << "if(" << v.name << " " << op << " " << v.type << "(" << value
           << ")){\n"
           << "if(mfront_out_of_bounds_policy==" << prefix
           << "_STRICT_POLICY){\n";
        opts.failure(os, c);
        os << "}\n"
           << "if(mfront_out_of_bounds_policy==" << prefix
           << "_WARNING_POLICY){\n";
        opts.warning(os, c);
        os << "}\n"
           << "}\n";
      };
      if (has_lower_bound(b)) {
        write("<", b.lowerBound, false);
      }
      if (has_upper_bound(b)) {
        write(">", b.upperBound, true);
      }
    };
    const auto& inputs = mpd.inputs;
    if ((checks) && ((hasPhysicalBounds(inputs)) || (hasBounds(inputs)))) {
      os << "#ifndef " << opts.bounds_check_macro << "\n";
      for (std::size_t idx = 0; idx != inputs.size(); ++idx) {
        write_physical_bounds(inputs[idx], idx);
      }
      for (std::size_t idx = 0; idx != inputs.size(); ++idx) {
        write_bounds(inputs[idx], idx);
      }
      os << "#endif /* " << opts.bounds_check_macro << " */\n";
    }
    os << mpd.f.body;
    if ((checks) &&
        ((mpd.output.hasPhysicalBounds()) || (mpd.output.hasBounds()))) {
      os << "#ifndef " << opts.bounds_check_macro << "\n";
      write_physical_bounds(mpd.output, inputs.size());
      write_bounds(mpd.output, inputs.size());
      os << "#endif /* " << opts.bounds_check_macro << " */\n";
    }
  }  // end of writePointEvaluation

  /*!
   * \brief write the report of a violated bound by the `generic` and
   * `cyrano` interfaces: error message giving the value of the variable,
   * status and bounds status. A failure ends the evaluation: the output is
   * `NaN`.
   * \param[out] os: output stream
   * \param[in] c: check
   * \param[in] qt: true if quantities are used
   * \param[in] failure: true if the evaluation fails
   */
  static void writeGenericBoundReport(
      std::ostream& os,
      const GenericMaterialPropertyInterfaceBase::BoundCheck& c,
      const bool qt,
      const bool failure) {
    const auto& v = c.variable;
    const auto to_string = qt ? "std::to_string(" + v.name + ".getValue())"
                              : "std::to_string(" + v.name + ")";
    const auto kind = c.physical ? std::string{"physical "} : std::string{};
    const auto msg =
        "mfront_report(\"" + v.name +
        (c.upper ? " is over its " + kind + "upper bound (\" + "
                 : " is below its " + kind + "lower bound (\" + ") +
        to_string + " + \"" + (c.upper ? ">" : "<");
    if (failure) {
      os << msg << c.bound << ").\\n\");\n"
         << "mfront_output_status->status = -1;\n"
         << "mfront_output_status->bounds_status = -" << c.rank << ";\n"
         << "errno = mfront_errno_old;\n"
         << "return std::nan(\"\");\n";
    } else {
      os << "mfront_output_status->status = 1;\n"
         << "mfront_output_status->bounds_status = " << c.rank << ";\n"
         << msg << c.bound << ").\\n\");\n";
    }
  }  // end of writeGenericBoundReport

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
    os.precision(14);
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
      os << "* \\date " << fd.date << '\n';
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

    writeParametersSetterFunctionDeclaration(
        os, mpd,
        {.material_property_name = name,
         .floating_point_type = "double",
         .interface_namespace = this->getInterfaceName()});

    os << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n"
       << "#endif /* " << makeUpperCase(name) << "_" << iucname << "_HH */\n";
    os.close();
  }  // end of writeHeaderFile()

  void GenericMaterialPropertyInterfaceBase::writePointwiseFunction(
      std::ostream& os,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd,
      const std::string& name,
      const PointwiseFunctionDescription& d) {
    const auto& types = d.types;
    const auto& iname = d.interface_namespace;
    const auto& params = mpd.parameters;
    os << "MFRONT_SHAREDOBJ " << types.real_type << "\n" << name << "(";
    os << types.output_status_type << "* const mfront_output_status,\n"
       << "const " << types.real_type << "* const";
    if (!mpd.inputs.empty()) {
      os << " mfront_params";
    }
    os << ",\nconst " << types.integer_type << " mfront_nargs,\n"
       << "const " << types.out_of_bounds_policy_type;
    if ((hasBounds(mpd.inputs)) || (hasBounds(mpd.output))) {
      os << " mfront_out_of_bounds_policy";
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
           << "mfront_report(" << iname << "::" << hn << "::get" << hn
           << "().msg);\n"
           << "errno = mfront_errno_old;\n"
           << "return std::nan(" << iname << "::" << hn << "::get" << hn
           << "().msg.c_str());\n"
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
    const auto qt = useQuantities(mpd);
    writePointEvaluation(
        os, mpd,
        PointEvaluationOptions{
            .bounds_check_macro = d.bounds_check_macro,
            .out_of_bounds_policy_prefix = d.out_of_bounds_policy_prefix,
            .failure =
                [qt](std::ostream& out, const BoundCheck& c) {
                  writeGenericBoundReport(out, c, qt, true);
                },
            .warning =
                [qt](std::ostream& out, const BoundCheck& c) {
                  writeGenericBoundReport(out, c, qt, false);
                }});
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
    os << "} // end of " << name << "\n\n";
  }  // end of writePointwiseFunction

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
    os.precision(14);
    const auto& file = fd.fileName;
    const auto& author = fd.authorName;
    const auto& date = fd.date;
    const auto& includes = mpd.includes;
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
      os << "* \\date " << date << '\n';
    }
    os << " */\n\n";
    os << "#include<algorithm>\n"
       << "#include<iterator>\n"
       << "#include<iostream>\n"
       << "#include<sstream>\n"
       << "#include<fstream>\n"
       << "#include<cstring>\n"
       << "#include<cstdlib>\n"
       << "#include<cerrno>\n"
       << "#include<string>\n"
       << "#include<vector>\n"
       << "#include<locale>\n"
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

    writeParametersSetterFunctionImplementation(
        os, mpd,
        {.material_property_name = name,
         .floating_point_type = "double",
         .interface_namespace = iname});

    writePointwiseFunction(
        os, mpd, fd, name,
        PointwiseFunctionDescription{
            .types = types,
            .interface_namespace = iname,
            .bounds_check_macro = "NO_" + iucname + "_BOUNDS_CHECK",
            .out_of_bounds_policy_prefix = prefix});
    os << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n";
    os.close();
  }  // end of writeSrcFile()

  GenericMaterialPropertyInterfaceBase::
      ~GenericMaterialPropertyInterfaceBase() = default;

}  // end of namespace mfront
