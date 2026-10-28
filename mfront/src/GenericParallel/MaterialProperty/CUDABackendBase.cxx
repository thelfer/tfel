/*!
 * \file   CUDABackendBase.cxx
 * \brief  This file implements the `CUDABackendBase` class
 * \author Thomas Helfer
 * \date 07/04/2026
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
#include "MFront/GenericParallel/MaterialProperty/CUDABackendBase.hxx"

namespace mfront::generic_parallel::material_property {

  CUDABackendBase::CUDABackendBase() = default;

  CUDABackendBase::CUDABackendBase(const tfel::utilities::DataMap& opts) {
    auto validator = tfel::utilities::DataMapValidator{};
    validator.addStrictlyPositiveIntegerCheck("number_of_threads_per_block");
    validator.validate(opts);
    if (contains(opts, "number_of_threads_per_block")) {
      this->number_of_threads_per_block =
          get<int>(opts, "number_of_threads_per_block");
    }
  }  // end of CUDABackendBase

  void CUDABackendBase::writeSpecificIncludesInHeaderFile(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
  }  // end of writeSpecificIncludesInSourceFile

  void CUDABackendBase::writeSpecificIncludesInSourceFile(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
    os << "#include <algorithm>\n"
       << "#include<type_traits>\n"
       << "#include\"TFEL/FSAlgorithm/copy.hxx\"\n"
       << "#include\"MFront/GenericParallel/GPUStackSize.hxx\"\n\n";
  }  // end of writeSpecificIncludesInSourceFile

  void CUDABackendBase::writeGlobalFunctions(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd) const {
    for (const auto b : {true, false}) {
      this->writeGlobalFunctions2(os, i, mpd, fd, b);
    }
  }  // end of writeGlobalFunctions

  void CUDABackendBase::writeGlobalFunctions2(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd,
      const bool handleStrides) const {
    const auto name = i.getFunctionName(mpd);
    const auto types = i.getTypesDescription();
    const auto kernel = "mfront_" + name + "_kernel" +
                        (handleStrides ? "" : "_without_strides");
    os << "__global__ void " << kernel << "(" << types.real_type
       << "* const mfront_output";
    if (handleStrides) {
      os << ",\nconst " << types.integer_type << " mfront_output_stride";
    }
    os << ",\nint* const mfront_statuses";
    if (isBoundsCheckingRequired(mpd)) {
      os << ",\nmfront_gpmp_Statistics* const mfront_statistics"
         << ",\nconst " << types.integer_type << " mfront_out_of_bounds_policy";
    }
    if ((!areParametersTreatedAsStaticVariables(mpd)) &&
        (!mpd.parameters.empty())) {
      const auto pcn = getMaterialPropertyParametersClassName(name);
      os << ",\nconst " << i.getInterfaceInternalNamespace() << "::" << pcn
         << " mfront_parameters";
    }
    if ((handleStrides) && (!mpd.inputs.empty())) {
      os << ",\nconst std::array<" << types.integer_type << ", "
         << mpd.inputs.size() << "> mfront_strides";
    }
    for (std::size_t idx = 0; idx != mpd.inputs.size(); ++idx) {
      os << ",\nconst " << types.real_type << "* const mfront_arg" << idx;
    }
    os << ",\nconst " << types.integer_type << " mfront_npoints){\n";
    writeBeginningOfMaterialPropertyBody(os, mpd, fd, "double", true);
    os << "const auto mfront_idx = static_cast<" << types.integer_type
       << ">(blockIdx.x) * blockDim.x + threadIdx.x;\n"
       << "if(mfront_idx >= mfront_npoints){\n"
       << "return;\n"
       << "}\n";
    this->writeKernelBody(os, i, mpd, handleStrides);
    os << "} // end of " << kernel << "\n\n";
  }  // end of writeGlobalFunctions2

  void CUDABackendBase::writeCxxDeclarations(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {}

  void CUDABackendBase::writeCxxImplementations(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&,
      const FileDescription&) const {}

  void CUDABackendBase::writeCImplementations(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription&) const {
    const auto pm = this->getProgrammingModel();
    this->writeCFunctionBeginning(os, i, mpd);
    // exceptions (allocation of an error message, of a block of statistics)
    // must not cross the boundary of the C function, as with the backends
    // based on C++ programming models
    os << "try{\n";
    if (isBoundsCheckingRequired(mpd)) {
      // the statistics are held by a block of managed memory of the current
      // device
      os << "auto mfront_device = int{};\n"
         << "const auto mfront_" << pm << "_device_error = " << pm
         << "GetDevice(&mfront_device);\n"
         << "if (mfront_" << pm << "_device_error != " << pm << "Success) {\n";
      writeBatchError(os, -7,
                      pm + "GetErrorString(mfront_" + pm + "_device_error)");
      os << "}\n";
      writeStatisticsDeclaration(
          os, "int", "mfront_device",
          "mfront_gpmp_Statistics* mfront_ptr = nullptr;\n"
          "return " +
              pm +
              "MallocManaged(&mfront_ptr, sizeof(mfront_gpmp_Statistics)) == " +
              pm + "Success ? mfront_ptr : nullptr;");
    }
    this->writeKernelCall(os, i, mpd);
    this->writeResultsAnalysis(os, i, mpd);
    writeHostExceptionsHandling(os);
    writeCFunctionEnd(os, i, mpd);
  }  // end of writeCImplementations

  void CUDABackendBase::writeKernelCall(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    const auto pm = this->getProgrammingModel();
    const auto name = i.getFunctionName(mpd);
    const auto threads_per_block =
        this->number_of_threads_per_block.value_or(128);
    const auto warp_size_attribute =
        pm == "hip" ? "hipDeviceAttributeWarpSize" : "cudaDevAttrWarpSize";
    // stream of the execution policy (default stream for the default policy)
    os << "const auto mfront_stream = "
       << "(mfront_backend_policy.policy == nullptr) ? " << pm
       << "Stream_t{} : mfront_backend_policy.policy->stream;\n";
    // number of threads per block, rounded up to a multiple of the warp
    // size
    os << "auto mfront_warp_device = int{};\n"
       << "auto mfront_warp_size = int{};\n"
       << "if ((" << pm << "GetDevice(&mfront_warp_device) != " << pm
       << "Success) ||\n"
       << "(" << pm << "DeviceGetAttribute(&mfront_warp_size, "
       << warp_size_attribute << ", mfront_warp_device) != " << pm
       << "Success)) {\n";
    writeBatchError(os, -7, "\"unable to get the warp size of the device\"");
    os << "}\n"
       << "const auto mfront_threads_per_block = static_cast<unsigned int>(("
       << threads_per_block
       << " + mfront_warp_size - 1) / mfront_warp_size * mfront_warp_size);\n";
    auto write_kernel_call = [&](const std::string_view npoints,
                                 const bool handleStrides) {
      const auto kernel = "mfront_" + name + "_kernel" +
                          (handleStrides ? "" : "_without_strides");
      // the number of blocks is limited by the size of the grid (2^31 - 1
      // in the first dimension): it is checked rather than truncated
      os << "const auto mfront_number_of_blocks = (" << npoints
         << " + mfront_threads_per_block - 1) / mfront_threads_per_block;\n"
         << "if (mfront_number_of_blocks > static_cast<"
         << "decltype(mfront_number_of_blocks)>("
         << "std::numeric_limits<int>::max())) {\n";
      writeBatchError(os, -5,
                      "\"too many points for the grid of the device (\" + "
                      "std::to_string(" +
                          std::string{npoints} + ") + \" given)\"");
      os << "}\n";
      // stack size of the threads
      os << "const auto mfront_stack_size = "
         << "::mfront::generic_parallel::gpu::ensureStackSize<" << kernel
         << ">(mfront_preserve_stack_size);\n"
         << "if (!mfront_stack_size.succeeded()) {\n";
      writeBatchError(os, -7, "mfront_stack_size.getErrorMessage()");
      os << "}\n";
      os << kernel << "<<<static_cast<unsigned int>(mfront_number_of_blocks), "
         << "mfront_threads_per_block, 0, mfront_stream>>>(mfront_output, ";
      if (handleStrides) {
        os << "mfront_output_stride, ";
      }
      os << "mfront_statuses, ";
      if (isBoundsCheckingRequired(mpd)) {
        os << "mfront_statistics, mfront_out_of_bounds_policy, ";
      }
      if ((!areParametersTreatedAsStaticVariables(mpd)) &&
          (!mpd.parameters.empty())) {
        const auto hn = getMaterialPropertyParametersHandlerClassName(name);
        os << i.getInterfaceInternalNamespace() << "::"  //
           << hn << "::get" << hn << "(), ";
      }
      if ((handleStrides) && (!mpd.inputs.empty())) {
        os << "mfront_strides, ";
      }
      for (std::size_t idx = 0; idx != mpd.inputs.size(); ++idx) {
        os << "mfront_args[" << idx << "], ";
      }
      os << npoints << ");\n"
         << "const auto mfront_" << pm << "_launch_error = " << pm
         << "GetLastError();\n"
         << "if (mfront_" << pm << "_launch_error != " << pm << "Success) {\n";
      writeBatchError(os, -7,
                      pm + "GetErrorString(mfront_" + pm + "_launch_error)");
      // an asynchronous evaluation returns once the kernel is submitted
      os << "}\n"
         << "if (!mfront_asynchronous) {\n"
         << "const auto mfront_" << pm << "_kernel_error = " << pm
         << "StreamSynchronize(mfront_stream);\n"
         << "if (mfront_" << pm << "_kernel_error != " << pm << "Success) {\n";
      writeBatchError(os, -7,
                      pm + "GetErrorString(mfront_" + pm + "_kernel_error)");
      os << "}\n"
         << "}\n";
    };
    this->writeKernelSelection(os, i, mpd, write_kernel_call);
  }  // end of writeKernelCall

  bool CUDABackendBase::mergesPointStatusesInKernels() const {
    return true;
  }  // end of mergesPointStatusesInKernels

  std::string CUDABackendBase::getAtomicIncrement(
      const std::string_view c) const {
    return "atomicAdd(&(" + std::string{c} + "), 1ull)";
  }  // end of getAtomicIncrement

  std::string CUDABackendBase::getAtomicMinimum(
      const std::string_view c, const std::string_view v) const {
    return "atomicMin(&(" + std::string{c} + "), " + std::string{v} + ")";
  }  // end of getAtomicMinimum

  std::string CUDABackendBase::getMemorySpace() const {
    return "mfront::generic_parallel::device_memory_space";
  }  // end of getMemorySpace

  std::string CUDABackendBase::getExecutionPolicyType() const {
    return this->getProgrammingModel() == "hip"
               ? "::mfront::generic_parallel::HIPBackendExecutionPolicy"
               : "::mfront::generic_parallel::CUDABackendExecutionPolicy";
  }  // end of getExecutionPolicyType

  std::string CUDABackendBase::getExecutionPolicyHeader() const {
    return this->getProgrammingModel() == "hip"
               ? "MFront/GenericParallel/HIPBackendExecutionPolicy.hxx"
               : "MFront/GenericParallel/CUDABackendExecutionPolicy.hxx";
  }  // end of getExecutionPolicyHeader

  CUDABackendBase::~CUDABackendBase() = default;

}  // end of namespace mfront::generic_parallel::material_property
