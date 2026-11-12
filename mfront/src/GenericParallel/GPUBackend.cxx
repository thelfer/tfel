/*!
 * \file   mfront/src/GenericParallel/GPUBackend.cxx
 * \brief  This file implements the `CUDA` and `HIP` backends of the
 * `generic-parallel` interfaces
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
#include "MFront/GenericParallel/GPUBackend.hxx"

namespace mfront::generic_parallel {

  GPUBackend::GPUBackend(const tfel::utilities::DataMap& opts) {
    auto validator = tfel::utilities::DataMapValidator{};
    validator.addStrictlyPositiveIntegerCheck("number_of_threads_per_block");
    validator.validate(opts);
    if (contains(opts, "number_of_threads_per_block")) {
      this->number_of_threads_per_block =
          get<int>(opts, "number_of_threads_per_block");
    }
  }  // end of GPUBackend

  void GPUBackend::writeSpecificIncludes(std::ostream& os) const {
    os << "#include \"MFront/GenericParallel/GPU.hxx\"\n";
  }  // end of writeSpecificIncludes

  std::string GPUBackend::getApply() const {
    return "mfront::generic_parallel::gpu::makeApply(" +
           std::to_string(this->number_of_threads_per_block) + ")";
  }  // end of getApply

  std::string GPUBackend::getMemorySpace() const {
    return "mfront::generic_parallel::device_memory_space";
  }  // end of getMemorySpace

  std::string GPUBackend::getExecutionPolicyType() const {
    return this->getName() == "hip"
               ? "::mfront::generic_parallel::HIPBackendExecutionPolicy"
               : "::mfront::generic_parallel::CUDABackendExecutionPolicy";
  }  // end of getExecutionPolicyType

  GPUBackend::~GPUBackend() noexcept = default;

  std::string CUDABackend::getName() const { return "cuda"; }

  std::string CUDABackend::getSourceFileExtension() const { return "cu"; }

  CUDABackend::~CUDABackend() noexcept = default;

  std::string HIPBackend::getName() const { return "hip"; }

  std::string HIPBackend::getSourceFileExtension() const { return "hip"; }

  void HIPBackend::writeSpecificIncludes(std::ostream& os) const {
    // selects the HIP runtime, even on NVIDIA GPUs
    os << "#include \"hip/hip_runtime.h\"\n";
    GPUBackend::writeSpecificIncludes(os);
  }  // end of writeSpecificIncludes

  HIPBackend::~HIPBackend() noexcept = default;

}  // end of namespace mfront::generic_parallel
