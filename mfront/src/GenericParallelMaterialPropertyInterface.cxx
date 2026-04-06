/*!
 * \file   mfront/src/GenericParallelMaterialPropertyInterface.cxx
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
#include "MFront/GenericParallelMaterialPropertyInterface.hxx"

namespace mfront {

  std::string GenericParallelMaterialPropertyInterface::getName() {
    return "generic-parallel";
  }

  GenericParallelMaterialPropertyInterface::
      GenericParallelMaterialPropertyInterface() = default;

  GenericParallelMaterialPropertyInterface::TypesDescription
  GenericParallelMaterialPropertyInterface::getTypesDescription() const {
    auto types = TypesDescription{};
    types.real_type = "mfront_gmp_real";
    types.integer_type = "mfront_gmp_size_type";
    types.out_of_bounds_policy_type = "mfront_gmp_OutOfBoundsPolicy";
    types.output_status_type = "mfront_gmp_OutputStatus";
    return types;
  }  // end of getTypesDescription

  std::vector<std::string>
  GenericParallelMaterialPropertyInterface::getInterfaceHeaderFiles() const {
    return {"MFront/GenericMaterialProperty/Types.h",
            "MFront/GenericMaterialProperty/MaterialProperty.h"};
  }  // end of getInterfaceHeaderFiles

  std::string GenericParallelMaterialPropertyInterface::getInterfaceName()
      const {
    return GenericParallelMaterialPropertyInterface::getName();
  }  // end of getInterfaceName

  std::string
  GenericParallelMaterialPropertyInterface::getInterfaceInternalNamespace()
      const {
    return "generic_parallel";
  }  // end of getInterfaceInternalNamespace

  std::string
  GenericParallelMaterialPropertyInterface::getInterfaceNameInCamelCase()
      const {
    return "GenericParallel";
  }  // end of getInterfaceNameInCamelCase

  std::string
  GenericParallelMaterialPropertyInterface::getInterfaceNameInUpperCase()
      const {
    return "GENERIC_PARALLEL";
  }  // end of getInterfaceNameInUpperCase

  std::string GenericParallelMaterialPropertyInterface::
      getOutOfBoundsPolicyEnumerationPrefix() const {
    return "GENERIC_MATERIALPROPERTY";
  }  // end of getOutOfBoundsPolicyEnumerationPrefix

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  GenericParallelMaterialPropertyInterface::treatKeyword(
      const std::string& k,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator) {
    const auto iname = this->getInterfaceName();
    const auto iccname = this->getInterfaceNameInCamelCase();
    tfel::raise_if((std::find(i.begin(), i.end(), iname) != i.end()) ||
                       (std::find(i.begin(), i.end(), iccname) != i.end()),
                   "GenericParallelMaterialPropertyInterface::treatKeyword: "
                   "unsupported keyword '" +
                       k + "'");
    return {false, current};
  }  // end of treatKeyword

  void GenericParallelMaterialPropertyInterface::getLibraryDescription(
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

  void GenericParallelMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const {
    const auto iccname = this->getInterfaceNameInCamelCase();
    const auto lib = iccname + getMaterialLawLibraryNameBase(mpd);
    this->getLibraryDescription(d, d.getLibrary(lib), mpd);
  }  // end of getTargetsDescription

  std::string GenericParallelMaterialPropertyInterface::getFunctionName(
      const MaterialPropertyDescription& mpd) const {
    const auto& material = mpd.material;
    const auto& className = mpd.className;
    if (material.empty()) {
      return className;
    }
    return material + "_" + className;
  }

  std::string GenericParallelMaterialPropertyInterface::getHeaderFileName(
      const std::string& name) const {
    const auto i = this->getInterfaceName();
    return name + "-" + i + ".hxx";
  }

  std::string GenericParallelMaterialPropertyInterface::getSourceFileName(
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
    if ((b.boundsType == VariableBoundsDescription::LOWER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      os << "if(" << v.name << " < " << v.type << "(" << b.lowerBound << ")){\n"
         << "mfront_bounds_statuses[" << 2 * i << "] = -1;\n"
         << "return;\n"
         << "}\n";
    }
    if ((b.boundsType == VariableBoundsDescription::UPPER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      os << "if(" << v.name << " > " << v.type << "(" << b.upperBound << ")){\n"
         << "mfront_bounds_statuses[" << 2 * i << "] = 1;\n"
         << "return;\n"
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
    if ((b.boundsType == VariableBoundsDescription::LOWER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      os << "if(" << v.name << " < " << v.type << "(" << b.lowerBound << ")){\n"
         << "mfront_bounds_statuses[" << 2 * i + 1 << "] = -1;\n"
         << "if (mfront_out_of_bounds_policy==" << iucname
         << "_STRICT_POLICY){\n"
         << "return;\n"
         << "}\n"
         << "}\n";
    }
    if ((b.boundsType == VariableBoundsDescription::UPPER) ||
        (b.boundsType == VariableBoundsDescription::LOWERANDUPPER)) {
      os << "if(" << v.name << " > " << v.type << "(" << b.lowerBound << ")){\n"
         << "mfront_bounds_statuses[" << 2 * i + 1 << "] = 1;\n"
         << "if (mfront_out_of_bounds_policy==" << iucname
         << "_STRICT_POLICY){\n"
         << "return;\n"
         << "}\n"
         << "}\n";
    }
  }  // end of writeBounds

  void GenericParallelMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    this->writeHeaderFile(mpd, fd);
    this->writeSrcFile(mpd, fd);
  }  // end of writeOutputFiles

  void GenericParallelMaterialPropertyInterface::writeHeaderFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto iucname = this->getInterfaceNameInUpperCase();
    const auto types = this->getTypesDescription();
    const auto name = this->getFunctionName(mpd);
    const auto fn = "include/" + this->getHeaderFileName(name);
    std::ofstream os{fn};
    tfel::raise_if(
        !os,
        "GenericParallelMaterialPropertyInterface::writeOutputFiles: "
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
       << "/*!\n"
       << " * \\brief compute the value of '" << mpd.className
       << "' material property on a set of points\n"
       << " *\n"
       << " * \\param[in] mfront_output_status: output status\n"
       << " * \\param[in] mfront_args: array of pointers to the state "
       << "variables's values\n"
       << " * \\param[in] mfront_nargs: number of state variables'\n"
       << " * \\param[in] mfront_npoints: number of points in which the "
       << "material property is computed. This is also the size of the arrays "
       << "givent the state variables\n"
       << " * \\param[in] mfront_out_of_bounds_policy: out of bounds policy\n"
       << " */\n"
       << "MFRONT_SHAREDOBJ void " << name << "(\n"
       << types.output_status_type << "* const,\n"
       << types.real_type << "* const,\n"
       << "const " << types.real_type << "* const* const,\n"
       << "const " << types.integer_type << ",\n"
       << "const " << types.integer_type << ",\n"
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

  void GenericParallelMaterialPropertyInterface::writeSrcFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    const auto types = this->getTypesDescription();
    const auto iname = this->getInterfaceName();
    const auto nname = this->getInterfaceInternalNamespace();
    const auto name = this->getFunctionName(mpd);
    const auto fn = "src/" + this->getSourceFileName(name);
    std::ofstream os{fn};
    tfel::raise_if(
        !os,
        "GenericParallelMaterialPropertyInterface::writeOutputFiles: "
        "unable to open '" +
            fn + "'");
    os.exceptions(std::ios::badbit | std::ios::failbit);
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
       << "#include<locale>\n"
       << "#include<cmath>\n";
    if (this->backend == "stlpar") {
      os << "#include<ranges>\n";
      os << "#include<execution>\n";
    }
    os << "#include\"TFEL/Config/TFELTypes.hxx\"\n"
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

    writeMaterialPropertyParametersHandler(os, mpd,
                                           {.material_property_name = name,
                                            .floating_point_type = "double",
                                            .interface_namespace = nname,
                                            .allow_copy_constructor = true});

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
         .interface_namespace = nname});

    if (this->backend == "stlpar") {
      this->writeParallelSTLImplementation(os, mpd, fd);
    } else {
      tfel::raise("unsupported backend '" + this->backend + "'");
    }
    os.close();
  }  // end of GenericParallelMaterialPropertyInterface::writeSrcFile

  void GenericParallelMaterialPropertyInterface::writeParallelSTLImplementation(
      std::ostream& os,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd) const {
    const auto name = this->getFunctionName(mpd);
    const auto nname = this->getInterfaceInternalNamespace();
    const auto types = this->getTypesDescription();
    const auto& function = mpd.f;
    const auto& params = mpd.parameters;
    const auto iucname = this->getInterfaceNameInUpperCase();
    const auto prefix = this->getOutOfBoundsPolicyEnumerationPrefix();
    os << "MFRONT_SHAREDOBJ void " << name << "(";
    os << types.output_status_type << "* const mfront_output_status,\n"
       << types.real_type << "* const mfront_output,\n"
       << "const " << types.real_type << "* const * const";
    if (!mpd.inputs.empty()) {
      os << " mfront_args";
    }
    os << ",\nconst " << types.integer_type << " mfront_nargs"
       << ",\nconst " << types.integer_type << " mfront_npoints"
       << ",\nconst " << types.out_of_bounds_policy_type;
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
         //         << "return std::nan(\"invalid number of arguments\");\n"
         << "return;\n"
         << "}\n";
      // parameters
      if ((!areParametersTreatedAsStaticVariables(mpd)) && (!params.empty())) {
        const auto hn = getMaterialPropertyParametersHandlerClassName(name);
        os << "if(!" << nname << "::" << hn << "::get" << hn << "().ok){\n"
           << "mfront_output_status->status = -6;\n"
           << "mfront_report(" << nname << "::" << name
           << "MaterialPropertyHandler::get" << name
           << "MaterialPropertyHandler().msg);\n"
           << "errno = mfront_errno_old;\n"
           //            << "return std::nan(" << nname << "::" << name
           //            << "MaterialPropertyHandler::get" << name
           //            << "MaterialPropertyHandler().msg.c_str());\n"
           << "return;\n"
           << "}\n";
      }
    }
    // array of errors for bounds checking
    os << "auto mfront_bounds_statuses = std::vector<int>{};\n"
       << "mfront_bounds_statuses.resize(2 * (mfront_nargs + 1));\n";
    // declaration of the kernel
    if ((!areParametersTreatedAsStaticVariables(mpd)) && (!params.empty())) {
      os << "auto mfront_kernel = [mfront_output_status, "
         << "mfront_bounds_statuses = mfront_bounds_statuses.data(), "
         << "mfront_out_of_bounds_policy, "
         << "mfront_parameters = " << nname << "::" << name
         << "MaterialPropertyHandler::get" << name
         << "MaterialPropertyHandler(), mfront_output, mfront_args]("
         << types.integer_type << " mfront_idx){\n";
    } else {
      os << "auto mfront_kernel = [mfront_output_status, "
         << "mfront_bounds_statuses = mfront_bounds_statuses.data(), "
         << "mfront_out_of_bounds_policy, "
         << "mfront_output, mfront_args](" << types.integer_type
         << " mfront_idx){\n";
    }
    // declaration of the parameters
    if (areParametersTreatedAsStaticVariables(mpd)) {
      for (const auto& p : mpd.parameters) {
        const auto pv =
            p.getAttribute<double>(VariableDescription::defaultValue);
        os << "constexpr auto " << p.name << " = ";
        if (useQuantities(mpd)) {
          os << p.type << "(" << pv << ");\n";
        } else {
          os << "double(" << pv << ");\n";
        }
      }
    } else {
      if (useQuantities(mpd)) {
        for (const auto& p : mpd.parameters) {
          os << "const auto " << p.name << " = " << p.type
             << "(mfront_parameters." << p.name << ");\n";
        }
      } else {
        for (const auto& p : mpd.parameters) {
          os << "const auto " << p.name << " = real(mfront_parameters."
             << p.name << ");\n";
        }
      }
    }
    // declaration of inputs
    if (!mpd.inputs.empty()) {
      auto p3 = mpd.inputs.begin();
      for (auto i = 0u; p3 != mpd.inputs.end(); ++p3, ++i) {
        auto cast_start = useQuantities(mpd) ? p3->type + "(" : "";
        auto cast_end = useQuantities(mpd) ? ")" : "";
        os << "const auto " << p3->name << " = " << cast_start;
        if (i == 0) {
          os << "*(*(mfront_args) + mfront_idx)";
        } else {
          os << "*(*(mfront_args + " + std::to_string(i) + "u) + mfront_idx)";
        }
        os << cast_end << ";\n";
      }
    }
    // declaration of the output
    os << "auto " << mpd.output.name << " = " << mpd.output.type << "{};\n";
    if (!areRuntimeChecksDisabled(mpd)) {
      if ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs))) {
        os << "#ifndef NO_" << iucname << "_BOUNDS_CHECK\n";
      }
      if (hasPhysicalBounds(mpd.inputs)) {
        os << "// treating physical bounds\n";
        for (decltype(mpd.inputs.size()) i = 0; i != mpd.inputs.size(); ++i) {
          writePhysicalBounds(os, mpd.inputs[i], i, useQuantities(mpd));
        }
      }
      if (hasBounds(mpd.inputs)) {
        os << "// treating standard bounds\n";
        for (decltype(mpd.inputs.size()) i = 0; i != mpd.inputs.size(); ++i) {
          writeBounds(os, prefix, mpd.inputs[i], i, useQuantities(mpd));
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
    if (!areRuntimeChecksDisabled(mpd)) {
      os << "if(!tfel::math::ieee754::isfinite(" << mpd.output.name << ")){\n"
         << "mfront_output_status->status = -4;\n"
         << "}\n";
    }
    os << "*(mfront_output + mfront_idx) = ";
    if (useQuantities(mpd)) {
      os << mpd.output.name << ".getValue();\n";
    } else {
      os << mpd.output.name << ";\n";
    }
    // end of the kernel
    os << "}; // end of mfront_kernel\n";
    os << "// loop over the points\n"
       << "const auto mfront_index_range = "
       << "std::views::iota(" << types.integer_type << "{}, mfront_npoints);\n"
       << "std::for_each(mfront_index_range.begin(), "
       << "mfront_index_range.end(),\n"
       << "[mfront_kernel](const " << types.integer_type << " mfront_idx){\n"
       << "mfront_kernel(mfront_idx);\n"
       << "});\n";
    if (!areRuntimeChecksDisabled(mpd)) {
      os << "if (errno != 0) {\n"
         << "mfront_output_status->status = -3;\n"
         << "mfront_output_status->c_error_number = errno;\n"
         << "mfront_report(strerror(errno));\n"
         << "}\n"
         << "errno = mfront_errno_old;\n";
      this->writeBoundsStatusesAnalysis(os, mpd);
    }
    os << "} // end of " << name << "\n\n"
       << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n";
  }  // end of writeParallelSTLImplementation

  void GenericParallelMaterialPropertyInterface::writeBoundsStatusesAnalysis(
      std::ostream& os, const MaterialPropertyDescription& mpd) const {
    if (!((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs)) ||
          (mpd.output.hasPhysicalBounds()) || (mpd.output.hasBounds()))) {
      return;
    }
    const auto iucname = this->getInterfaceNameInUpperCase();
    const auto prefix = this->getOutOfBoundsPolicyEnumerationPrefix();
    os << "#ifndef NO_" << iucname << "_BOUNDS_CHECK\n";
    auto write_physical_bounds_violation = [&os](const VariableDescription& v,
                                                 std::size_t idx) {
      os << "if(mfront_bounds_statuses[" << 2 * idx << "] > 0){\n"
         << "mfront_report(\"" << v.name
         << " is over its physical upper bound at at least one "
         << "point\");\n"
         << "mfront_output_status->status = -1;\n"
         << "mfront_output_status->bounds_status = -" << idx + 1 << ";\n"
         << "return;\n"
         << "}\n";
      os << "if(mfront_bounds_statuses[" << 2 * idx << "] < 0){\n"
         << "mfront_report(\"" << v.name
         << " is below its physical lower bound at at least one "
         << "point\");\n"
         << "mfront_output_status->status = -1;\n"
         << "mfront_output_status->bounds_status = -" << idx + 1 << ";\n"
         << "return;\n"
         << "}\n";
    };
    for (decltype(mpd.inputs.size()) i = 0; i != mpd.inputs.size(); ++i) {
      const auto& v = mpd.inputs[i];
      if (!v.hasPhysicalBounds()) {
        continue;
      }
      write_physical_bounds_violation(v, i);
    }
    if (mpd.output.hasPhysicalBounds()) {
      write_physical_bounds_violation(mpd.output, mpd.inputs.size());
    }
    if ((hasBounds(mpd.inputs)) || (mpd.output.hasBounds())) {
      auto write_bounds_violation = [&os, &prefix](const VariableDescription& v,
                                                   std::size_t idx) {
        os << "if(mfront_bounds_statuses[" << 2 * idx + 1 << "] > 0){\n"
           << "mfront_report(\"" << v.name
           << " is over its upper bound at at least one point\");\n"
           << "if(mfront_out_of_bounds_policy==" << prefix
           << "_STRICT_POLICY){\n"
           << "mfront_output_status->status = -1;\n"
           << "mfront_output_status->bounds_status = -" << idx + 1 << ";\n"
           << "return;\n"
           << "}\n"
           << "}\n";
        os << "if(mfront_bounds_statuses[" << 2 * idx + 1 << "] < 0){\n"
           << "mfront_report(\"" << v.name
           << " is below its lower bound at at least one point\");\n"
           << "if(mfront_out_of_bounds_policy==" << prefix
           << "_STRICT_POLICY){\n"
           << "mfront_output_status->status = -1;\n"
           << "mfront_output_status->bounds_status = -" << idx + 1 << ";\n"
           << "return;\n"
           << "}\n"
           << "}\n";
      };
      os << "if((mfront_out_of_bounds_policy==" << prefix
         << "_STRICT_POLICY)||\n"
         << "(mfront_out_of_bounds_policy==" << prefix
         << "_WARNING_POLICY)){\n";
      for (decltype(mpd.inputs.size()) i = 0; i != mpd.inputs.size(); ++i) {
        const auto& v = mpd.inputs[i];
        if (!v.hasBounds()) {
          continue;
        }
        write_bounds_violation(v, i);
      }
      if (mpd.output.hasBounds()) {
        write_bounds_violation(mpd.output, mpd.inputs.size());
      }
      os << "}\n";
    }
    os << "#endif /* NO_" << iucname << "_BOUNDS_CHECK */\n";
  }

  GenericParallelMaterialPropertyInterface::
      ~GenericParallelMaterialPropertyInterface() = default;

}  // end of namespace mfront
