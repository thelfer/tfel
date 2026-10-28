/*!
 * \file   SYCLBackend.cxx
 * \brief  This file implements the `SYCLBackend` class
 * \author Thomas Helfer
 * \date 18/05/2026
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
#include "MFront/GenericParallel/MaterialProperty/SYCLBackend.hxx"

namespace mfront::generic_parallel::material_property {

  SYCLBackend::SYCLBackend() {}  // end of SYCLBackend

  SYCLBackend::SYCLBackend(const tfel::utilities::DataMap& opts) {
    auto validator = tfel::utilities::DataMapValidator{};
    declareDataTransferPolicyOption(validator);
    declareDeviceStackSizeOption(validator);
    validator.validate(opts);
    this->data_transfer_policy = areDataTransfersAutomatic(opts)
                                     ? DataTransferPolicy::AUTOMATIC
                                     : DataTransferPolicy::NONE;
    this->device_stack_size = getDeviceStackSize(opts);
  }  // end of SYCLBackend

  std::string SYCLBackend::getName() const { return "sycl"; }  // end of getName

  void SYCLBackend::writeSpecificIncludesInSourceFile(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    CxxProgrammingModelBackendBase::writeSpecificIncludesInSourceFile(os, i,
                                                                      mpd);
    os << "#include <sycl/sycl.hpp>\n";
    if (this->device_stack_size != 0) {
      os << "#include\"MFront/GenericParallel/DeviceStackSize.hxx\"\n";
    }

    if (this->handlesDataTransfer()) {
      os << "#include\"MFront/GenericParallel/SYCLDeviceCopies.hxx\"\n";
    }
    os << "\n";
  }  // end of writeSpecificIncludesInSourceFile

  void SYCLBackend::writeExecutionPreparation(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
    // SYCL has no default queue: the queue of the execution policy gives
    // the device and the context of the unified shared memory of the calling
    // code
    os << "if (mfront_backend_policy.policy == nullptr) {\n";
    writeBatchError(os, -5,
                    "\"no execution policy given: the sycl backend requires "
                    "a queue\"");
    os << "}\n"
       << "auto mfront_sycl_queue = mfront_backend_policy.policy->queue;\n"
       << "auto* const mfront_queue = &mfront_sycl_queue;\n"
       // the kernel must be executed after the computations already
       // submitted to the queue, which may compute the arguments. This is
       // guaranteed by an in-order queue. With an out-of-order queue, the
       // evaluation is synchronous and the queue is synchronized before the
       // kernel.
       << "if (!mfront_queue->is_in_order()) {\n"
       << "if (mfront_asynchronous) {\n";
    writeBatchError(os, -5,
                    "\"asynchronous evaluations require an in-order SYCL "
                    "queue\"");
    os << "}\n"
       << "mfront_queue->wait_and_throw();\n"
       << "}\n";
  }  // end of writeExecutionPreparation

  void SYCLBackend::writeKernelCall(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd) const {
    const auto requiresBoundsCheck = this->isBoundsCheckingRequired(mpd);
    writeDeviceStackSizeSetting(os, this->device_stack_size);
    if (requiresBoundsCheck) {
      // the statistics are held by a block of shared memory, identified by
      // the device and the context of the queue
      writeStatisticsDeclaration(
          os, "std::pair<sycl::device, sycl::context>",
          "{mfront_queue->get_device(), mfront_queue->get_context()}",
          "return sycl::malloc_shared<mfront_gpmp_Statistics>(1, "
          "*mfront_queue);");
      if (this->handlesDataTransfer()) {
        // statistics given by the calling code which are not accessible by
        // the device, as the arguments and the output
        os << "if (mfront_deferred_report) {\n"
           << "mfront_copies.transfer(mfront_statistics, 1, true);\n"
           << "}\n";
      }
    }
    this->writeKernels(os, i, mpd, fd);
    auto write_kernel_call = [&os, this](const std::string_view npoints,
                                         const bool handleStrides) {
      os << "// loop over the points\n";
      os << "auto mfront_sycl_kernel_event = ";
      os << "mfront_queue->parallel_for(" << npoints << ", mfront_kernel";
      if (!handleStrides) {
        os << "_without_strides";
      }
      os << ");\n";
      if (this->handlesDataTransfer()) {
        // the copies must be completed before they are destroyed: the
        // evaluation is then synchronous
        os << "if (mfront_copies.holdsCopies()) {\n"
           << "mfront_copies.copyOutputs();\n"
           << "} else if (!mfront_asynchronous) {\n"
           << "mfront_sycl_kernel_event.wait_and_throw();\n"
           << "}\n";
      } else {
        // an asynchronous evaluation returns once the kernel is submitted
        os << "if (!mfront_asynchronous) {\n"
           << "mfront_sycl_kernel_event.wait_and_throw();\n"
           << "}\n";
      }
    };
    this->writeKernelSelection(os, i, mpd, write_kernel_call);
  }  // end of writeKernelCall

  bool SYCLBackend::mergesPointStatusesInKernels() const {
    return true;
  }  // end of mergesPointStatusesInKernels

  /*!
   * \return the declaration of an atomic reference to a counter of the
   * statistics, held by unified shared memory
   * \param[in] c: counter
   */
  static std::string getSYCLAtomicReference(const std::string_view c) {
    return "::sycl::atomic_ref<unsigned long long, "
           "::sycl::memory_order::relaxed, ::sycl::memory_scope::device, "
           "::sycl::access::address_space::global_space>(" +
           std::string{c} + ")";
  }  // end of getSYCLAtomicReference

  std::string SYCLBackend::getAtomicIncrement(const std::string_view c) const {
    return getSYCLAtomicReference(c) + ".fetch_add(1ull)";
  }  // end of getAtomicIncrement

  std::string SYCLBackend::getAtomicMinimum(const std::string_view c,
                                            const std::string_view v) const {
    return getSYCLAtomicReference(c) + ".fetch_min(" + std::string{v} + ")";
  }  // end of getAtomicMinimum

  bool SYCLBackend::handlesDataTransfer() const {
    return this->data_transfer_policy == DataTransferPolicy::AUTOMATIC;
  }  // end of handlesDataTransfer

  void SYCLBackend::writeDataTransfersToDevice(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    if (!this->handlesDataTransfer()) {
      return;
    }
    // the arrays which are not accessible by the device are copied in
    // device memory. The output is copied in both
    // directions, so that the values between two values of a strided output
    // are kept
    const auto types = i.getTypesDescription();
    const auto ns = std::string{"::mfront::generic_parallel::sycl::"};
    os << "auto mfront_copies = " << ns << "DeviceCopies{*mfront_queue, "
       << "mfront_npoints, " << ns << "DataTransferPolicy::AUTOMATIC};\n";
    if (!mpd.inputs.empty()) {
      os << "auto mfront_args_storage = std::array<const " << types.real_type
         << "*, " << mpd.inputs.size() << ">{};\n";
      for (std::size_t idx = 0; idx != mpd.inputs.size(); ++idx) {
        os << "{\n"
           << "auto mfront_values = " << ns << "StridedArray<const "
           << types.real_type << ">{mfront_args_host[" << idx << "], "
           << "mfront_strides[" << idx << "], 1};\n"
           << "mfront_copies.transfer(mfront_values, 1, false);\n"
           << "mfront_args_storage[" << idx << "] = mfront_values.values;\n"
           << "}\n";
      }
      os << "const auto* const mfront_args = mfront_args_storage.data();\n";
    }
    os << "auto mfront_output_values = " << ns << "StridedArray<"
       << types.real_type << ">{mfront_output_host, mfront_output_stride, "
       << "1};\n"
       << "mfront_copies.transfer(mfront_output_values, 1, true);\n"
       << "auto* const mfront_output = mfront_output_values.values;\n"
       // statuses of the points, if requested
       << "mfront_copies.transfer(mfront_statuses, mfront_npoints, true);\n";
  }  // end of writeDataTransfersToDevice

  void SYCLBackend::writeDataTransfersToHost(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {}

  std::string SYCLBackend::getMemorySpace() const {
    // with automatic transfers, the arguments and the output which are not
    // accessible by the device are copied
    if (this->handlesDataTransfer()) {
      return "mfront::generic_parallel::any_memory_space";
    }
    return "mfront::generic_parallel::device_memory_space";
  }  // end of getMemorySpace

  std::string SYCLBackend::getExecutionPolicyType() const {
    return "::mfront::generic_parallel::SyclBackendExecutionPolicy";
  }  // end of getExecutionPolicyType

  std::string SYCLBackend::getExecutionPolicyHeader() const {
    return "MFront/GenericParallel/SyclBackendExecutionPolicy.hxx";
  }  // end of getExecutionPolicyHeader

  SYCLBackend::~SYCLBackend() = default;

}  // end of namespace mfront::generic_parallel::material_property
