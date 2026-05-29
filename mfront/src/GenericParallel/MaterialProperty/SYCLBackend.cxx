/*!
 * \file   SYCLBackend.cxx
 * \brief  This file implements the `SYCLBackend` class
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
#include "MFront/VariableDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyParametersHandler.hxx"
#include "MFront/GenericParallelMaterialPropertyInterface.hxx"
#include "MFront/GenericParallel/MaterialProperty/SYCLBackend.hxx"

namespace mfront::generic_parallel::material_property {

  SYCLBackend::SYCLBackend() {}  // end of SYCLBackend

  SYCLBackend::SYCLBackend(const tfel::utilities::DataMap& opts) {
    auto validator = tfel::utilities::DataMapValidator{};
    validator.addDataTypeValidator<std::string>("data_location");
    validator.validate(opts);
    if (!contains(opts, "data_location")) {
      tfel::raise("the 'data_location' option is not provided");
    }
    const auto dl = get<std::string>(opts, "data_location");
    if (dl != "host") {
      tfel::raise("unsuported data_location '" + dl +
                  "'. The only valid value is: 'host'");
    }
  }  // end of SYCLBackend

  std::string SYCLBackend::getName() const { return "sycl"; }  // end of getName

  void SYCLBackend::writeSpecificIncludesInHeaderFile(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    CxxProgrammingModelBackendBase::writeSpecificIncludesInHeaderFile(os, i,
                                                                      mpd);
    os << "#ifdef __cplusplus\n"
       << "#include <sycl/sycl.hpp>\n"
       << "#endif\n\n"
       << "#ifdef __cplusplus\n"
       << "using mfront_gpmp_sycl_queue = sycl::queue;\n"
       << "#else\n"
       << "typedef struct mfront_gpmp_sycl_queue mfront_gpmp_sycl_queue;\n"
       << "#endif\n\n";
  }  // end of writeSpecificIncludesInSourceFile

  void SYCLBackend::writeSpecificIncludesInSourceFile(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    CxxProgrammingModelBackendBase::writeSpecificIncludesInSourceFile(os, i,
                                                                      mpd);
    os << "#include<concepts>\n"
       << "#include<type_traits>\n"
       << "#include\"TFEL/FSAlgorithm/copy.hxx\"\n\n"
       << "namespace mfront::gpmp::sycl{\n\n"
       << "template<typename T>\n"
       << "struct [[nodiscard]] DeviceDataPtr{\n"
       << "DeviceDataPtr(::sycl::queue& q)\n"
       << " : queue(q)\n"
       << "{}\n"
       << "DeviceDataPtr(DeviceDataPtr&&) = delete;\n"
       << "DeviceDataPtr(const DeviceDataPtr&) = delete;\n"
       << "DeviceDataPtr& operator=(DeviceDataPtr&&) = delete;\n"
       << "DeviceDataPtr& operator=(const DeviceDataPtr&) = delete;\n"
       << "void allocate(std::size_t s){\n"
       << "this->ptr = ::sycl::malloc_device<T>(s, this->queue);\n"
       << "}\n"
       << "[[nodiscard]] T* data() noexcept{\n"
       << "return this->ptr;\n"
       << "}\n"
       << "~DeviceDataPtr() noexcept{\n"
       << "if(this->ptr != nullptr) {\n"
       << "::sycl::free(this->ptr, this->queue);\n"
       << "}\n"
       << "}\n"
       << "private:\n"
       << "::sycl::queue& queue;\n"
       << "T* ptr = nullptr;\n"
       << "};\n\n"
       << "} // end of namespace mfront::gpmp::sycl\n";
  }  // end of writeSpecificIncludesInSourceFile

  std::vector<BackendBase::ExtraArgumentOfCFunctions>
  SYCLBackend::getExtraArgumentsOfCFunctions() const {
    return {{.type = "mfront_gpmp_sycl_queue",
             .name = "mfront_queue",
             .description = "SYCL queue",
             .is_pointer = true,
             .is_mutable = true}};
  }  // end of getExtraArgumentsOfCFunctions

  void SYCLBackend::writeKernelCall(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd,
      const bool treatStrides) const {
    const auto types = i.getTypesDescription();
    const auto requiresBoundsCheck = this->isBoundsCheckingRequired(mpd);
    if (requiresBoundsCheck) {
      os << "using mfront_shared_allocator_type = "
         << "sycl::usm_allocator<int, sycl::usm::alloc::shared>;\n"
         << "auto mfront_shared_allocator = "
            "mfront_shared_allocator_type{*mfront_queue};\n"
         << "auto mfront_bounds_statuses = "
         << "std::vector<int, "
            "mfront_shared_allocator_type>{mfront_shared_allocator};\n"
         << "mfront_bounds_statuses.resize(2 * (mfront_nargs + 1), 0);\n";
    }
    this->writeKernel(os, i, mpd, fd, false);
    if (treatStrides) {
      this->writeKernel(os, i, mpd, fd, true);
    }
    auto write_kernel_call = [&os, this, &types](const bool handleStrides) {
      os << "// loop over the points\n";
      os << "auto mfront_sycl_kernel_event = ";
      os << "mfront_queue->parallel_for(mfront_npoints";
      if (this->handlesDataTransfer()) {
        os << ", mfront_sycl_memcpy_events";
      }
      os << ", mfront_kernel";
      if (!handleStrides) {
        os << "_without_strides";
      }
      os << ");\n";
      if (this->handlesDataTransfer()) {
        os << "mfront_queue->memcpy(mfront_output_host, "
           << "mfront_output_ptr.data(), mfront_npoints, "
           << "mfront_sycl_kernel_event).wait();\n";
      } else {
        os << "mfront_sycl_kernel_event.wait();\n";
      }
    };
    if (treatStrides) {
      if (mpd.inputs.empty()) {
        os << "if(mfront_output_stride == 0){\n"
           << "mfront_kernel_without_strides(0);\n"
           << "mfront_queue->wait();\n"
           << "} else if(mfront_output_stride == 1){\n";
        write_kernel_call(false);
        os << "} else {\n";
        write_kernel_call(true);
        os << "}\n";
      } else {
        os << "if(mfront_areAllArgumentsUniform && "
           << "(mfront_output_stride == 0)){\n"
           << "mfront_kernel(0);\n"
           << "} else {\n";
        os << "const bool mfront_areAllArgumentsStrideOne = std::all_of("
           << "mfront_args_strides_tmp.begin(), "
           << "mfront_args_strides_tmp.end(), "
           << "[](const " << types.integer_type << " mfront_stride){"
           << "return mfront_stride == 1;});\n"
           << "if(mfront_areAllArgumentsStrideOne && "
              "(mfront_output_stride == "
              "1)){\n";
        write_kernel_call(false);
        os << "} else {\n";
        write_kernel_call(true);
        os << "}\n"
           << "}\n";
      }
    } else {
      write_kernel_call(false);
    }
  }  // end of writeKernelCall

  bool SYCLBackend::handlesDataTransfer() const {
    return false;  // this->data_location == "host";
  }                // end of handlesDataTransfer

  void SYCLBackend::writeDataTransfersToDevice(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const bool treatStrides) const {
    const auto types = i.getTypesDescription();
    if (!this->handlesDataTransfer()) {
      return;
    }
    os << "auto mfront_sycl_memcpy_events = std::vector<::sycl::event>{};\n";
    if (mpd.inputs.empty()) {
      os << "const " << types.real_type << "* const mfront_args = nullptr;";
      os << "mfront_sycl_memcpy_events.reserve(1);\n";
    } else {
      os << "mfront_sycl_memcpy_events.reserve(" << mpd.inputs.size() + 1
         << ");\n"
         << "auto mfront_args_storage = std::array<const " << types.real_type
         << "*, " << mpd.inputs.size() << ">{};\n";
      for (std::size_t idx = 0; idx != mpd.inputs.size(); ++idx) {
        os << "auto mfront_arg" << idx << "_ptr = "
           << "::mfront::gpmp::sycl::DeviceDataPtr<" << types.real_type
           << ">{*mfront_queue};\n";
        if (treatStrides) {
          os << "if(mfront_args_strides[" << idx << "] ==0){\n"
             << "mfront_arg" << idx << "_ptr.allocate(1);\n"
             << "mfront_sycl_memcpy_events.push_back("
             << "mfront_queue->memcpy(mfront_arg" << idx
             << "_ptr.data(), mfront_args_host[" << idx << "], 1));\n"
             << "} else {\n"
             << "mfront_arg" << idx << "_ptr.allocate("
             << "mfront_npoints * mfront_args_strides[" << idx << "]);\n"
             << "mfront_sycl_memcpy_events.push_back("
             << "mfront_queue->memcpy(mfront_arg" << idx
             << "_ptr.data(), mfront_args_host[" << idx << "], "
             << "mfront_npoints * mfront_args_strides[" << idx << "]));\n"
             << "}\n";
        } else {
          os << "mfront_arg" << idx << "_ptr.allocate(mfront_npoints);\n"
             << "mfront_sycl_memcpy_events.push_back("
             << "mfront_queue->memcpy(mfront_arg" << idx
             << "_ptr.data(), mfront_args_host[" << idx
             << "], mfront_npoints));\n";
        }
        os << "mfront_args_storage[" << idx << "] = "  //
           << "mfront_arg" << idx << "_ptr.data();\n";
      }
      os << "const auto* const mfront_args = mfront_args_storage.data();\n";
    }
    os << "auto mfront_output_ptr = "
       << "::mfront::gpmp::sycl::DeviceDataPtr<" << types.real_type
       << ">{*mfront_queue};\n"
       << "mfront_output_ptr.allocate(mfront_npoints);\n"
       << "mfront_sycl_memcpy_events.push_back("
       << "mfront_queue->memcpy(mfront_output_ptr.data(), mfront_output_host, "
       << "mfront_npoints));\n"
       << "auto *mfront_output = mfront_output_ptr.data();\n";
  }

  void SYCLBackend::writeDataTransfersToHost(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&,
      const bool) const {}

  SYCLBackend::~SYCLBackend() = default;

}  // end of namespace mfront::generic_parallel::material_property
