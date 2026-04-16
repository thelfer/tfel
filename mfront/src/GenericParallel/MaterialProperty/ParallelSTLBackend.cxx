/*!
 * \file   ParallelSTLBackend.cxx
 * \brief  This file implements the `ParallelSTLBackend` class
 * \author Thomas Helfer
 * \date   07/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "MFront/VariableDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyParametersHandler.hxx"
#include "MFront/GenericParallelMaterialPropertyInterface.hxx"
#include "MFront/GenericParallel/MaterialProperty/ParallelSTLBackend.hxx"

namespace mfront::generic_parallel::material_property {

  ParallelSTLBackend::ParallelSTLBackend() = default;

  ParallelSTLBackend::ParallelSTLBackend(const tfel::utilities::DataMap& opts)
      : execution_policy("std::execution::par_unseq") {
    auto validator = tfel::utilities::DataMapValidator{};
    validator.addDataTypeValidator<std::string>("execution_policy");
    validator.validate(opts);
    if (contains(opts, "execution_policy")) {
      const auto p = get<std::string>(opts, "execution_policy");
      if ((p == "par") || (p == "parallel_policy")) {
        this->execution_policy = "std::execution::par";
      } else if ((p == "seq") || (p == "sequenced_policy")) {
        this->execution_policy = "std::execution::seq";
      } else if ((p == "unseq") || (p == "unsequenced_policy")) {
        this->execution_policy = "std::execution::unseq";
      } else if ((p == "par_unseq") || (p == "parallel_unsequenced_policy")) {
        this->execution_policy = "std::execution::par_unseq";
      } else {
        tfel::raise(
            "unsuported execution policy '" + p +
            "'. Valid execution policy are: 'sequenced_policy' (or "
            "'seq'), 'unsequenced_policy' (or 'unseq'), 'parallel_policy' (or "
            "'par') and 'parallel_unsequenced_policy' (or 'par_unseq')");
      }
    }
  }  // end of ParallelSTLBackend

  void ParallelSTLBackend::writeSpecificIncludesInHeaderFile(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
  }  // end of writeSpecificIncludesInSourceFile

  void ParallelSTLBackend::writeSpecificIncludesInSourceFile(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
    os << "#include<ranges>\n";
    os << "#include<execution>\n";
  }  // end of writeSpecificIncludesInSourceFile

  void ParallelSTLBackend::writeGlobalFunction(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
  }  // end of writeGlobalFunction

  void ParallelSTLBackend::writeCxxDeclaration(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {}

  void ParallelSTLBackend::writeCDeclaration(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    const auto name = i.getFunctionName(mpd);
    const auto types = i.getTypesDescription();
    os << "/*!\n"
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
  }  // end of writeDeclaration

  void ParallelSTLBackend::writeCxxImplementation(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&,
      const FileDescription&) const {}

  void ParallelSTLBackend::writeCImplementation(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd) const {
    const auto name = i.getFunctionName(mpd);
    const auto nname = i.getInterfaceInternalNamespace();
    const auto types = i.getTypesDescription();
    const auto& params = mpd.parameters;
    const auto iucname = i.getInterfaceNameInUpperCase();
    const auto prefix = i.getOutOfBoundsPolicyEnumerationPrefix();
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
    //
    this->writeKernelCall(os, i, mpd);
    //
    if (!areRuntimeChecksDisabled(mpd)) {
      os << "if (errno != 0) {\n"
         << "mfront_output_status->status = -3;\n"
         << "mfront_output_status->c_error_number = errno;\n"
         << "mfront_report(strerror(errno));\n"
         << "}\n"
         << "errno = mfront_errno_old;\n";
      this->writeBoundsStatusesAnalysis(os, i, mpd);
    }
    os << "} // end of " << name << "\n\n";
  }  // end of writeImplementation

  void ParallelSTLBackend::writeKernelCall(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    const auto name = i.getFunctionName(mpd);
    const auto types = i.getTypesDescription();
    const auto nname = i.getInterfaceInternalNamespace();
    const auto iucname = i.getInterfaceNameInUpperCase();
    const auto prefix = i.getOutOfBoundsPolicyEnumerationPrefix();
    const auto& function = mpd.f;
    const auto& params = mpd.parameters;
    const auto requiresBoundsCheck =
        (!areRuntimeChecksDisabled(mpd)) &&
        ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs)) ||
         (mpd.output.hasPhysicalBounds()) || (mpd.output.hasBounds()));
    if (requiresBoundsCheck) {
      os << "auto mfront_bounds_statuses = std::vector<int>{};\n"
         << "mfront_bounds_statuses.resize(2 * (mfront_nargs + 1));\n";
    }
    // declaration of the kernel
    os << "auto mfront_kernel = [";
    if (requiresBoundsCheck) {
      os << "mfront_bounds_statuses = mfront_bounds_statuses.data(), "
         << "mfront_out_of_bounds_policy, ";
    }
    if ((!areParametersTreatedAsStaticVariables(mpd)) && (!params.empty())) {
      os << "mfront_parameters = " << nname << "::" << name
         << "MaterialPropertyHandler::get" << name
         << "MaterialPropertyHandler(), ";
    }
    os << "mfront_output, mfront_args](" << types.integer_type
       << " mfront_idx){\n";
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
      for (auto idx = 0u; p3 != mpd.inputs.end(); ++p3, ++idx) {
        auto cast_start = useQuantities(mpd) ? p3->type + "(" : "";
        auto cast_end = useQuantities(mpd) ? ")" : "";
        os << "const auto " << p3->name << " = " << cast_start;
        if (idx == 0) {
          os << "*(*(mfront_args) + mfront_idx)";
        } else {
          os << "*(*(mfront_args + " + std::to_string(idx) + "u) + mfront_idx)";
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
        for (decltype(mpd.inputs.size()) idx = 0; idx != mpd.inputs.size();
             ++idx) {
          this->writePhysicalBounds(os, mpd.inputs[idx], idx,
                                    useQuantities(mpd));
        }
      }
      if (hasBounds(mpd.inputs)) {
        os << "// treating standard bounds\n";
        for (decltype(mpd.inputs.size()) idx = 0; idx != mpd.inputs.size();
             ++idx) {
          this->writeBounds(os, prefix, mpd.inputs[idx], idx,
                            useQuantities(mpd));
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
          this->writePhysicalBounds(os, mpd.output, mpd.inputs.size() + 1,
                                    useQuantities(mpd));
        }
        if (mpd.output.hasBounds()) {
          os << "// treating bounds\n";
          this->writeBounds(os, prefix, mpd.output, mpd.inputs.size() + 1,
                            useQuantities(mpd));
        }
        os << "#endif /* NO_" << iucname << "_BOUNDS_CHECK */\n";
      }
    }
    //     if (!areRuntimeChecksDisabled(mpd)) {
    //       os << "if(!tfel::math::ieee754::isfinite(" << mpd.output.name <<
    //       ")){\n"
    //          << "mfront_output_status->status = -4;\n"
    //          << "}\n";
    //     }
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
       << "std::for_each(" << this->execution_policy
       << ", mfront_index_range.begin(), "
       << "mfront_index_range.end(),\n"
       << "[mfront_kernel](const " << types.integer_type << " mfront_idx){\n"
       << "mfront_kernel(mfront_idx);\n"
       << "});\n";
  }  // end of writeKernelCall

  ParallelSTLBackend::~ParallelSTLBackend() = default;

}  // end of namespace mfront::generic_parallel::material_property
