/*!
 * \file   CUDABackend.cxx
 * \brief  This file implements the `CUDABackend` class
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
#include "MFront/GenericParallel/MaterialProperty/CUDABackend.hxx"

namespace mfront::generic_parallel::material_property {

  [[nodiscard]] static bool requiresBoundsCheck(
      const MaterialPropertyDescription& mpd) {
    return (!areRuntimeChecksDisabled(mpd)) &&
           ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs)) ||
            (mpd.output.hasPhysicalBounds()) || (mpd.output.hasBounds()));
  }  // end of requiresBoundsCheck

  CUDABackend::CUDABackend() = default;

  CUDABackend::CUDABackend(const tfel::utilities::DataMap& opts) {
    auto validator = tfel::utilities::DataMapValidator{};
    validator.addStrictlyPositiveIntegerCheck("number_of_blocks");
    validator.addStrictlyPositiveIntegerCheck("number_of_threads_per_block");
    validator.validate(opts);
    if (contains(opts, "number_of_blocks")) {
      this->number_of_blocks = get<int>(opts, "number_of_blocks");
    }
    if (contains(opts, "number_of_threads_per_block")) {
      this->number_of_threads_per_block =
          get<int>(opts, "number_of_threads_per_block");
    }
  }  // end of CUDABackend

  std::string CUDABackend::getHeaderFileExtension() const {
    return "hxx";
  }  // end of getHeaderFileExtension

  std::string CUDABackend::getSourceFileExtension() const {
    return "cu";
  }  // end of getSourceFileExtension

  void CUDABackend::writeSpecificIncludesInHeaderFile(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
  }  // end of writeSpecificIncludesInSourceFile

  void CUDABackend::writeSpecificIncludesInSourceFile(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
    os << "#include <algorithm>\n";
  }  // end of writeSpecificIncludesInSourceFile

  void CUDABackend::writeGlobalFunction(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd) const {
    // declaration of the kernel
    const auto name = i.getFunctionName(mpd);
    const auto types = i.getTypesDescription();
    const auto iucname = i.getInterfaceNameInUpperCase();
    const auto& params = mpd.parameters;
    const auto prefix = i.getOutOfBoundsPolicyEnumerationPrefix();
    os << "__global__ void mfront_" << name << "_kernel(" << types.real_type
       << "* const mfront_output";
    if (requiresBoundsCheck(mpd)) {
      os << ",\nint* const mfront_bounds_statuses"
         << ",\nconst " << types.integer_type << " mfront_out_of_bounds_policy";
    }
    if ((!areParametersTreatedAsStaticVariables(mpd)) && (!params.empty())) {
      os << ",\nconst " << i.getInterfaceInternalNamespace() << "::" << name
         << "MaterialPropertyHandler mfront_parameters";
    }
    for (const auto& v : mpd.inputs) {
      os << ",\nconst " << types.real_type << "* const "  //
         << "mfront_" << v.name << "_values";
    }
    os << ",\nconst int mfront_npoints){\n";
    writeBeginningOfMaterialPropertyBody(os, mpd, fd, "double", true);
    //
    os << "auto mfront_idx = threadIdx.x;\n"
       << "if(mfront_idx >= mfront_npoints){\n"
       << "return;\n"
       << "}\n";
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
        os << "mfront_" << p3->name << "_values[mfront_idx]";
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
    os << mpd.f.body;
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
    os << "mfront_output[mfront_idx] = ";
    if (useQuantities(mpd)) {
      os << mpd.output.name << ".getValue();\n";
    } else {
      os << mpd.output.name << ";\n";
    }
    // end of the kernel
    os << "} // end of mfront_" << name << "_kernel\n\n";
  }  // end of writeGlobalFunction

  void CUDABackend::writeCxxDeclaration(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {}

  void CUDABackend::writeCDeclaration(
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

  void CUDABackend::writeCxxImplementation(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&,
      const FileDescription&) const {}

  void CUDABackend::writeCImplementation(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription&) const {
    const auto name = i.getFunctionName(mpd);
    const auto nname = i.getInterfaceInternalNamespace();
    const auto types = i.getTypesDescription();
    const auto& params = mpd.parameters;
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
    if (!areRuntimeChecksDisabled(mpd)) {
      os << "auto mfront_report = "
         << "[&mfront_output_status](const std::string& "
         << "mfront_error_message){\n"
         << "if(mfront_error_message.empty()){\n"
         << "return;\n"
         << "}\n"
         << "std::strncpy(mfront_output_status->msg,"
         << "mfront_error_message.c_str(),511);\n"
         << "mfront_output_status->msg[511]='\\0';\n"
         << "};\n"
         << "const int mfront_errno_old = errno;\n";
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
    if (requiresBoundsCheck(mpd)) {
      os << "int *mfront_bounds_statuses;\n"
         << "cudaMallocManaged(&mfront_bounds_statuses,\n"
         << "(2 * (mfront_nargs + 1)) * sizeof(int));\n"
         << "std::fill(mfront_bounds_statuses,\n"
         << "mfront_bounds_statuses + 2 * (mfront_nargs + 1), 0);\n";
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
    if (requiresBoundsCheck(mpd)) {
      os << "cudaFree(mfront_bounds_statuses);\n";
    }
    os << "} // end of " << name << "\n\n";
  }  // end of writeImplementation

  void CUDABackend::writeKernelCall(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    const auto name = i.getFunctionName(mpd);
    os << "// loop over the points\n"
       << "mfront_" << name << "_kernel<<<"
       << this->number_of_blocks.value_or(1) << ", ";
    if (this->number_of_threads_per_block.has_value()) {
      os << *(this->number_of_threads_per_block);
    } else {
      os << "mfront_npoints";
    }
    os << ">>>(mfront_output, ";
    if (requiresBoundsCheck(mpd)) {
      os << "mfront_bounds_statuses, "
         << "mfront_out_of_bounds_policy, ";
    }
    if ((!areParametersTreatedAsStaticVariables(mpd)) &&
        (!mpd.parameters.empty())) {
      os << i.getInterfaceInternalNamespace() << "::" << name
         << "MaterialPropertyHandler::get" << name
         << "MaterialPropertyHandler(), ";
    }
    for (std::size_t idx = 0; idx != mpd.inputs.size(); ++idx) {
      os << "mfront_args[" << idx << "], ";
    }
    os << "static_cast<int>(mfront_npoints));\n";
  }  // end of writeKernelCall

  CUDABackend::~CUDABackend() = default;

}  // end of namespace mfront::generic_parallel::material_property
