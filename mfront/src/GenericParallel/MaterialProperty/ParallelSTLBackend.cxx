/*!
 * \file   ParallelSTLBackend.cxx
 * \brief  This file implements the `ParallelSTLBackend` class
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
#include "MFront/GenericParallel/BackendOptions.hxx"
#include "MFront/GenericParallel/MaterialProperty/ParallelSTLBackend.hxx"

namespace mfront::generic_parallel::material_property {

  ParallelSTLBackend::ParallelSTLBackend()
      : execution_policy("std::execution::par_unseq") {
  }  // end of ParallelSTLBackend

  ParallelSTLBackend::ParallelSTLBackend(const tfel::utilities::DataMap& opts) {
    auto validator = tfel::utilities::DataMapValidator{};
    declareExecutionPolicyOption(validator);
    declareDeviceStackSizeOption(validator);
    validator.validate(opts);
    this->execution_policy = getExecutionPolicy(opts);
    this->device_stack_size = getDeviceStackSize(opts);
  }  // end of ParallelSTLBackend

  std::string ParallelSTLBackend::getName() const {
    return "stlpar";
  }  // end of getName

  void ParallelSTLBackend::writeSpecificIncludesInSourceFile(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
    os << "#include<ranges>\n"
       << "#include<concepts>\n"
       << "#include<execution>\n"
       << "#include<type_traits>\n"
       << "#include\"TFEL/FSAlgorithm/copy.hxx\"\n"
       << "#include\"MFront/GenericParallel/CountingIterator.hxx\"\n";
    if (this->device_stack_size != 0) {
      os << "#include\"MFront/GenericParallel/DeviceStackSize.hxx\"\n";
    }
    os << "\n";
  }  // end of writeSpecificIncludesInSourceFile

  void ParallelSTLBackend::writeKernelCall(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd) const {
    const auto types = i.getTypesDescription();
    const auto requiresBoundsCheck = this->isBoundsCheckingRequired(mpd);
    // only the common options of the execution policy are used: the C++
    // parallel algorithms have no queue
    writeDeviceStackSizeSetting(os, this->device_stack_size);
    if (requiresBoundsCheck) {
      // the statuses of the points are merged by a reduction
      writeLocalStatisticsDeclaration(os);
    }
    this->writeKernels(os, i, mpd, fd);
    auto write_kernel_call = [&os, this, &types, requiresBoundsCheck](
                                 const std::string_view npoints,
                                 const bool handleStrides) {
      const auto kernel = std::string{
          handleStrides ? "mfront_kernel" : "mfront_kernel_without_strides"};
      // the iterators of std::views::iota can't be used: with libstdc++,
      // the parallel algorithms run sequentially with them (see the
      // documentation of mfront::generic_parallel::CountingIterator)
      os << "// loop over the points\n"
         << "using mfront_index_type = " << types.integer_type << ";\n"
         << "using mfront_index_iterator = "
         << "mfront::generic_parallel::CountingIterator<mfront_index_type>;\n";
      const auto range =
          "mfront_index_iterator{0}, mfront_index_iterator{"
          "static_cast<mfront_index_type>(" +
          std::string{npoints} + ")}";
      if (!requiresBoundsCheck) {
        os << "std::for_each(" << this->execution_policy << ", " << range
           << ", " << kernel << ");\n";
        return;
      }
      os << "const auto mfront_reduced_statistics = std::transform_reduce("
         << this->execution_policy << ", " << range << ", "
         << "mfront_gpmp_Statistics{.number_of_failures = 0, "
         << ".first_failure = ::mfront::gpmp::no_event, "
         << ".number_of_unreliable_results = 0, "
         << ".first_unreliable_result = ::mfront::gpmp::no_event}, "
         << "[](const mfront_gpmp_Statistics& mfront_a, "
         << "const mfront_gpmp_Statistics& mfront_b) {\n"
         << "return ::mfront::gpmp::merge(mfront_a, mfront_b);\n"
         << "}, "
         << "[" << kernel << "](const mfront_index_type mfront_i) {\n"
         << "return ::mfront::gpmp::makeStatistics("
         << "static_cast<unsigned long long>(mfront_i), " << kernel
         << "(mfront_i));\n"
         << "});\n"
         << "*mfront_statistics = ::mfront::gpmp::merge(*mfront_statistics, "
         << "mfront_reduced_statistics);\n";
    };
    this->writeKernelSelection(os, i, mpd, write_kernel_call);
  }  // end of writeKernelCall

  std::string ParallelSTLBackend::getMemorySpace() const {
    // the target of the parallel algorithms is selected by the compiler
    return "mfront::generic_parallel::getParallelSTLMemorySpace()";
  }  // end of getMemorySpace

  std::string ParallelSTLBackend::getExecutionPolicyType() const {
    return "::mfront::generic_parallel::ParallelSTLBackendExecutionPolicy";
  }  // end of getExecutionPolicyType

  std::string ParallelSTLBackend::getExecutionPolicyHeader() const {
    return "MFront/GenericParallel/ParallelSTLBackendExecutionPolicy.hxx";
  }  // end of getExecutionPolicyHeader

  ParallelSTLBackend::~ParallelSTLBackend() = default;

}  // end of namespace mfront::generic_parallel::material_property
