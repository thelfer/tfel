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
    validator.addStrictlyPositiveIntegerCheck("number_of_threads_per_block");
    validator.validate(opts);
    if (contains(opts, "number_of_threads_per_block")) {
      this->number_of_threads_per_block =
          get<int>(opts, "number_of_threads_per_block");
    }
  }  // end of CUDABackend

  std::string CUDABackend::getName() const { return "cuda"; }  // end of getName

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
    os << "#include <algorithm>\n"
       << "#include<type_traits>\n"
       << "#include\"TFEL/FSAlgorithm/copy.hxx\"\n"
       << "#include\"TFEL/FSAlgorithm/fill.hxx\"\n\n";
  }  // end of writeSpecificIncludesInSourceFile

  void CUDABackend::writeGlobalFunctions(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd) const {
    for (const auto b : {true, false}) {
      this->writeGlobalFunctions2(os, i, mpd, fd, b);
    }
  }  // end of writeGlobalFunctions

  void CUDABackend::writeGlobalFunctions2(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd,
      const bool treatUniformArguments) const {
    // declaration of the kernel
    const auto name = i.getFunctionName(mpd);
    const auto types = i.getTypesDescription();
    const auto iucname = i.getInterfaceNameInUpperCase();
    const auto& params = mpd.parameters;
    const auto prefix = i.getOutOfBoundsPolicyEnumerationPrefix();
    os << "__global__ void mfront_" << name << "_kernel";
    if (treatUniformArguments) {
      os << "2";
    }
    os << "(" << types.real_type << "* const mfront_output";
    if (requiresBoundsCheck(mpd)) {
      os << ",\nint* const mfront_bounds_statuses"
         << ",\nconst " << types.integer_type << " mfront_out_of_bounds_policy";
    }
    if ((!areParametersTreatedAsStaticVariables(mpd)) && (!params.empty())) {
      const auto pcn = getMaterialPropertyParametersClassName(name);
      os << ",\nconst " << i.getInterfaceInternalNamespace() << "::" << pcn
         << " mfront_parameters";
    }
    if ((treatUniformArguments) && (!mpd.inputs.empty())) {
      os << ",\nconst std::array<bool, " << mpd.inputs.size()
         << "> mfront_uniform_markers";
    }
    for (const auto& v : mpd.inputs) {
      os << ",\nconst " << types.real_type << "* const "  //
         << "mfront_" << v.name << "_values";
    }
    os << ",\nconst int mfront_npoints){\n";
    writeBeginningOfMaterialPropertyBody(os, mpd, fd, "double", true);
    //
    os << "auto mfront_idx = blockIdx.x * blockDim.x + threadIdx.x;\n"
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
        if (treatUniformArguments) {
          os << "const auto " << p3->name << " = " << cast_start;
          os << "mfront_uniform_markers[" << idx << "] ? "
             << "mfront_" << p3->name << "_values[0] "
             << ": mfront_" << p3->name << "_values[mfront_idx]";
          os << cast_end << ";\n";
        } else {
          os << "const auto " << p3->name << " = " << cast_start;
          os << "mfront_" << p3->name << "_values[mfront_idx]";
          os << cast_end << ";\n";
        }
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
    os << "} // end of mfront_" << name << "_kernel";
    if (treatUniformArguments) {
      os << "2";
    }
    os << "\n\n";
  }  // end of writeGlobalFunction2

  void CUDABackend::writeCxxDeclarations(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {}

  void CUDABackend::writeCDeclarations(
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
       << " * \\param[in] mfront_args_uniform_markers: array of integers\n"
       << " *             stating if the variables are uniform or not.\n"
       << " *             If 0, the associated variable is not uniform\n"
       << " * \\param[in] mfront_npoints: number of points in which the "
       << "material property is computed.\n"
       << " * \\param[in] mfront_out_of_bounds_policy: out of bounds policy\n"
       << " *\n"
       << " * \\note `mfront_npoints` is also the size of the arrays giving\n"
       << " *   the output and the nputs\n"
       << " */\n"
       << "MFRONT_SHAREDOBJ void " << name << "(\n"
       << types.output_status_type << "* const,\n"
       << types.real_type << "* const,\n"
       << "const " << types.real_type << "* const* const,\n"
       << "const " << types.integer_type << ",\n"
       << "const " << types.integer_type << ",\n"
       << "const " << types.out_of_bounds_policy_type << ");\n\n";
    os << "/*!\n"
       << " * \\brief compute the value of '" << mpd.className
       << "' material property on a set of points\n"
       << " *\n"
       << " * \\param[in] mfront_output_status: output status\n"
       << " * \\param[in] mfront_args: array of pointers to the state "
       << "variables's values\n"
       << " * \\param[in] mfront_nargs: number of state variables'\n";
    os << " * \\param[in] mfront_uniform_output_option: ";
    if (!mpd.inputs.empty()) {
      os << "integer stating what shall be done if\n"
         << " * all inputs are uniform\n"
         << " * If not null, only the first element of the output "
         << " * is computed\n"
         << " *\n";
    } else {
      os << "integer stating what shall be done.\n"
         << " * If not null, only the first\n "
         << " * element of the output is computed. \n"
         << " *\n";
    }
    os << " * \\param[in] mfront_npoints: number of points in which the "
       << "material property is computed.\n"
       << " * \\param[in] mfront_out_of_bounds_policy: out of bounds policy\n"
       << " *\n"
       << " * \\note `mfront_npoints` is also the size of the arrays giving\n"
       << " *   the output and the nputs\n"
       << " */\n"
       << "MFRONT_SHAREDOBJ void " << name << "2(\n"
       << types.output_status_type << "* const,\n"
       << types.real_type << "* const,\n"
       << "const " << types.real_type << "* const* const,\n"
       << "const " << types.integer_type << "*,\n"
       << "const " << types.integer_type << ",\n"
       << "const " << types.integer_type << ",\n"
       << "const " << types.integer_type << ",\n"
       << "const " << types.out_of_bounds_policy_type << ");\n\n";
  }  // end of writeDeclaration

  void CUDABackend::writeCxxImplementations(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&,
      const FileDescription&) const {}

  void CUDABackend::writeCImplementations(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd) const {
    for (const auto b : {true, false}) {
      this->writeCImplementations2(os, i, mpd, fd, b);
    }
  }  // end of writeCImplementations

  void CUDABackend::writeCImplementations2(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription&,
      const bool treatUniformArguments) const {
    const auto name = i.getFunctionName(mpd);
    const auto nname = i.getInterfaceInternalNamespace();
    const auto types = i.getTypesDescription();
    const auto& params = mpd.parameters;
    const auto prefix = i.getOutOfBoundsPolicyEnumerationPrefix();
    os << "MFRONT_SHAREDOBJ void " << name;
    if (treatUniformArguments) {
      os << "2";
    }
    os << "(";
    os << types.output_status_type << "* const mfront_output_status,\n"
       << types.real_type << "* const mfront_output,\n"
       << "const " << types.real_type << "* const * const";
    if (!mpd.inputs.empty()) {
      os << " mfront_args";
    }
    if (treatUniformArguments) {
      os << ",\nconst " << types.integer_type << "* const";
      if (!mpd.inputs.empty()) {
        os << " mfront_args_uniform_markers";
      }
    }
    os << ",\nconst " << types.integer_type << " mfront_nargs"
       << ",\nconst " << types.integer_type << " mfront_npoints";
    if (treatUniformArguments) {
      os << ",\nconst " << types.integer_type
         << " mfront_uniform_output_option";
    }
    os << ",\nconst " << types.out_of_bounds_policy_type;
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
      os << "if constexpr (std::is_signed_v<" << types.integer_type << ">){\n"
         << "if(mfront_npoints < 0){\n"
         << "mfront_output_status->status = -5;\n"
         << "mfront_report(\"invalid number of points "
         << "(\"+std::to_string(mfront_npoints)+\" given\");\n"
         << "errno = mfront_errno_old;\n"
         << "return;\n"
         << "}\n"
         << "}\n";
      // check number of arguments
      os << "if(mfront_nargs != " << mpd.inputs.size() << "){\n"
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
           << "mfront_report(" << nname << "::" << hn << "::get" << hn
           << "().msg);\n"
           << "errno = mfront_errno_old;\n"
           << "return;\n"
           << "}\n";
      }
    }
    os << "if(mfront_npoints == 0){\n"
       << "return;\n"
       << "}\n";
    if (requiresBoundsCheck(mpd)) {
      os << "int *mfront_bounds_statuses;\n"
         << "cudaMallocManaged(&mfront_bounds_statuses,\n"
         << "(2 * (mfront_nargs + 1)) * sizeof(int));\n"
         << "const auto mfront_cuda_alloc_managed__error = "
         << "cudaGetLastError();\n"
         << "if (mfront_cuda_alloc_managed__error != cudaSuccess) {\n"
         << "mfront_output_status->status = -6;\n"
         << "mfront_report(cudaGetErrorString(mfront_cuda_alloc_managed__error)"
         << ");\n"
         << "return;\n"
         << "}\n"
         << "tfel::fsalgo::fill<" << 2 * (mpd.inputs.size() + 1)
         << ">::exe(mfront_bounds_statuses, 0);\n";
    }
    //
    this->writeKernelCall(os, i, mpd, treatUniformArguments);
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
      os << "cudaFree(mfront_bounds_statuses);\n"
         << "const auto mfront_cuda_free_error = cudaGetLastError();\n"
         << "if (mfront_cuda_free_error != cudaSuccess) {\n"
         << "mfront_output_status->status = -6;\n"
         << "mfront_report(cudaGetErrorString(mfront_cuda_free_error));\n"
         << "return;\n"
         << "}\n";
    }
    os << "} // end of " << name << "\n\n";
  }  // end of writeImplementation2

  void CUDABackend::writeKernelCall(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const bool treatUniformArguments) const {
    if ((treatUniformArguments) && (!mpd.inputs.empty())) {
      os << "auto mfront_uniform_markers_tmp = std::array<bool, "
         << mpd.inputs.size() << ">{};\n"
         << "tfel::fsalgo::copy<" << mpd.inputs.size() << ">::exe("
         << "mfront_args_uniform_markers, "
         << "mfront_uniform_markers_tmp.begin());\n";
    }
    const auto name = i.getFunctionName(mpd);
    auto write_kernel_call = [this, &os, &i, &mpd, &treatUniformArguments,
                              &name](const bool handleUniformArguments,
                                     const bool uniform) {
      os << "// loop over the points\n"
         << "mfront_" << name << "_kernel";
      if (handleUniformArguments) {
        os << "2";
      }
      const auto block_size = this->number_of_threads_per_block.value_or(64);
      if (uniform) {
        os << "<<<1, 1>>>(mfront_output, ";
      } else {
        os << "<<<((mfront_npoints + " << block_size + 1 << ") / ( "
           << block_size << ")), " << block_size << ">>>(mfront_output, ";
      }
      if (requiresBoundsCheck(mpd)) {
        os << "mfront_bounds_statuses, "
           << "mfront_out_of_bounds_policy, ";
      }
      if ((!areParametersTreatedAsStaticVariables(mpd)) &&
          (!mpd.parameters.empty())) {
        const auto hn = getMaterialPropertyParametersHandlerClassName(name);
        os << i.getInterfaceInternalNamespace() << "::"  //
           << hn << "::get" << hn << "(), ";
      }
      if ((handleUniformArguments) && (!mpd.inputs.empty())) {
        os << "mfront_uniform_markers_tmp, ";
      }
      for (std::size_t idx = 0; idx != mpd.inputs.size(); ++idx) {
        os << "mfront_args[" << idx << "], ";
      }
      if (uniform) {
        os << "1";
      } else {
        os << "static_cast<int>(mfront_npoints)";
      }
      os << ");\n";
      os << "const auto mfront_cuda_kernel_error = cudaGetLastError();\n"
         << "if (mfront_cuda_kernel_error != cudaSuccess) {\n"
         << "mfront_output_status->status = -6;\n"
         << "mfront_report(cudaGetErrorString(mfront_cuda_kernel_error));\n"
         << "return;\n"
         << "}\n";
    };
    if (treatUniformArguments) {
      if (mpd.inputs.empty()) {
        os << "if (mfront_uniform_output_option != 0) {\n";
        write_kernel_call(treatUniformArguments, true);
        os << "} else {\n";
        write_kernel_call(treatUniformArguments, false);
        os << "}\n";
      } else {
        os << "const bool mfront_areAllArgumentsUniform = std::all_of("
           << "mfront_uniform_markers_tmp.begin(), "
           << "mfront_uniform_markers_tmp.end(), "
           << "[](const bool mfront_boolean){return mfront_boolean != 0;});\n"
           << "if((mfront_areAllArgumentsUniform) && "
              "(mfront_uniform_output_option != 0)){\n";
        write_kernel_call(treatUniformArguments, true);
        os << "} else {\n"
           << "const bool mfront_areAllArgumentsNonUniform = std::all_of("
           << "mfront_uniform_markers_tmp.begin(), "
           << "mfront_uniform_markers_tmp.end(), "
           << "[](const bool mfront_boolean){return mfront_boolean == 0;});\n"
           << "if (mfront_areAllArgumentsNonUniform){\n";
        write_kernel_call(false, false);
        os << "} else {\n";
        write_kernel_call(treatUniformArguments, false);
        os << "}\n"
           << "}\n";
      }
    } else {
      write_kernel_call(false, false);
    }
  }  // end of writeKernelCall

  CUDABackend::~CUDABackend() = default;

}  // end of namespace mfront::generic_parallel::material_property
