/*!
 * \file   mfront/src/GenericParallel/SYCLBackend.cxx
 * \brief  This file implements the SYCL backend of the `generic-parallel`
 * interfaces
 * \author Thomas Helfer
 * \date   18/05/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "MFront/GenericParallel/BackendOptions.hxx"
#include "MFront/GenericParallel/SYCLBackend.hxx"

namespace mfront::generic_parallel {

  SYCLBackend::SYCLBackend(const tfel::utilities::DataMap& opts) {
    auto validator = tfel::utilities::DataMapValidator{};
    declareDataTransferPolicyOption(validator);
    declareDeviceStackSizeOption(validator);
    validator.validate(opts);
    this->automatic_data_transfers = areDataTransfersAutomatic(opts);
    this->device_stack_size = getDeviceStackSize(opts);
  }  // end of SYCLBackend

  std::string SYCLBackend::getName() const { return "sycl"; }

  std::string SYCLBackend::getSourceFileExtension() const { return "cxx"; }

  void SYCLBackend::writeSpecificIncludes(std::ostream& os) const {
    // primitive of the backend and functions transferring the fields of the
    // batches
    os << "#include \"MFront/GenericParallel/SYCL.hxx\"\n"
       << "#include \"MFront/GenericParallelMaterialProperty/SYCL.hxx\"\n";
  }  // end of writeSpecificIncludes

  std::string SYCLBackend::getApply() const {
    if (this->device_stack_size == 0) {
      return "mfront::generic_parallel::sycl::makeApply()";
    }
    return "mfront::generic_parallel::sycl::makeApply<" +
           std::to_string(this->device_stack_size) + ">()";
  }  // end of getApply

  std::string SYCLBackend::getMemorySpace() const {
    // with automatic transfers, the fields which are not accessible by the
    // device are copied
    if (this->automatic_data_transfers) {
      return "mfront::generic_parallel::any_memory_space";
    }
    return "mfront::generic_parallel::device_memory_space";
  }  // end of getMemorySpace

  std::string SYCLBackend::getExecutionPolicyType() const {
    return "::mfront::generic_parallel::SyclBackendExecutionPolicy";
  }  // end of getExecutionPolicyType

  void SYCLBackend::writeCall(std::ostream& os,
                              const std::string_view f,
                              const std::string_view status,
                              const std::string_view arguments) const {
    // the functions of the SYCL backend transfer the fields which are not
    // accessible by the device before calling the generic ones
    const auto ns = std::string{"mfront::gpmp::sycl::"};
    os << "return " << ns << f << "(" << status << ", " << arguments << ", "
       << ns << "DataTransferPolicy::"
       << (this->automatic_data_transfers ? "AUTOMATIC" : "NONE") << ", "
       << this->getApply() << ");\n";
  }  // end of writeCall

  SYCLBackend::~SYCLBackend() noexcept = default;

}  // end of namespace mfront::generic_parallel
