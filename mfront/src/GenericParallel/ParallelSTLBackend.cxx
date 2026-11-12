/*!
 * \file   mfront/src/GenericParallel/ParallelSTLBackend.cxx
 * \brief  This file implements the C++ parallel algorithms backend
 * (`stlpar`) of the `generic-parallel` interfaces
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
#include "MFront/GenericParallel/BackendOptions.hxx"
#include "MFront/GenericParallel/ParallelSTLBackend.hxx"

namespace mfront::generic_parallel {

  ParallelSTLBackend::ParallelSTLBackend() = default;

  ParallelSTLBackend::ParallelSTLBackend(const tfel::utilities::DataMap& opts) {
    auto validator = tfel::utilities::DataMapValidator{};
    declareExecutionPolicyOption(validator);
    declareDeviceStackSizeOption(validator);
    validator.validate(opts);
    this->execution_policy = getExecutionPolicy(opts);
    this->device_stack_size = getDeviceStackSize(opts);
  }  // end of ParallelSTLBackend

  std::string ParallelSTLBackend::getName() const { return "stlpar"; }

  std::string ParallelSTLBackend::getSourceFileExtension() const {
    return "cxx";
  }  // end of getSourceFileExtension

  void ParallelSTLBackend::writeSpecificIncludes(std::ostream& os) const {
    os << "#include <execution>\n"
       << "#include \"MFront/GenericParallel/ParallelSTL.hxx\"\n";
  }  // end of writeSpecificIncludes

  std::string ParallelSTLBackend::getApply() const {
    if (this->device_stack_size == 0) {
      return "mfront::generic_parallel::stlpar::makeApply(" +
             this->execution_policy + ")";
    }
    return "mfront::generic_parallel::stlpar::makeApply<" +
           std::to_string(this->device_stack_size) + ">(" +
           this->execution_policy + ")";
  }  // end of getApply

  std::string ParallelSTLBackend::getMemorySpace() const {
    // the target of the parallel algorithms is selected by the compiler
    return "mfront::generic_parallel::getParallelSTLMemorySpace()";
  }  // end of getMemorySpace

  std::string ParallelSTLBackend::getExecutionPolicyType() const {
    return "::mfront::generic_parallel::ParallelSTLBackendExecutionPolicy";
  }  // end of getExecutionPolicyType

  ParallelSTLBackend::~ParallelSTLBackend() noexcept = default;

}  // end of namespace mfront::generic_parallel
