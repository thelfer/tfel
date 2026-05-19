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
    //     auto validator = tfel::utilities::DataMapValidator{};
    //     validator.addDataTypeValidator<std::string>("execution_policy");
    //     validator.validate(opts);
    //     if (contains(opts, "execution_policy")) {
    //       const auto p = get<std::string>(opts, "execution_policy");
    //       if ((p == "par") || (p == "parallel_policy")) {
    //         this->execution_policy = "std::execution::par";
    //       } else if ((p == "seq") || (p == "sequenced_policy")) {
    //         this->execution_policy = "std::execution::seq";
    //       } else if ((p == "unseq") || (p == "unsequenced_policy")) {
    //         this->execution_policy = "std::execution::unseq";
    //       } else if ((p == "par_unseq") || (p ==
    //       "parallel_unsequenced_policy")) {
    //         this->execution_policy = "std::execution::par_unseq";
    //       } else {
    //         tfel::raise(
    //             "unsuported execution policy '" + p +
    //             "'. Valid execution policy are: 'sequenced_policy' (or "
    //             "'seq'), 'unsequenced_policy' (or 'unseq'), 'parallel_policy'
    //             (or "
    //             "'par') and 'parallel_unsequenced_policy' (or 'par_unseq')");
    //       }
    //     }
  }  // end of SYCLBackend

  std::string SYCLBackend::getName() const {
    return "sycl";
  }  // end of getName

  void SYCLBackend::writeSpecificIncludesInSourceFile(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
    os << "#include<concepts>\n"
       << "#include<type_traits>\n"
       << "#include <sycl/sycl.hpp>\n"
       << "#include\"TFEL/FSAlgorithm/copy.hxx\"\n\n";
  }  // end of writeSpecificIncludesInSourceFile

  void SYCLBackend::writeKernelCall(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd,
      const bool treatStrides) const {
    const auto types = i.getTypesDescription();
    const auto requiresBoundsCheck = this->isBoundsCheckingRequired(mpd);
    os << "sycl::queue mfront_queue(sycl::gpu_selector_v);\n";
    if (requiresBoundsCheck) {
      os << "using mfront_allocator_type = "
         << "sycl::usm_allocator<int, sycl::usm::alloc::shared>;\n"
         << "auto mfront_allocator = mfront_allocator_type{mfront_queue};\n"
         << "auto mfront_bounds_statuses = "
         << "std::vector<int, mfront_allocator_type>{mfront_allocator}\n;"
         << "mfront_bounds_statuses.resize(2 * (mfront_nargs + 1), 0);\n";
    }
    this->writeKernel(os, i, mpd, fd, false);
    if (treatStrides) {
      this->writeKernel(os, i, mpd, fd, true);
    }
    auto write_kernel_call = [&os, this, &types](const bool handleStrides) {
      os << "// loop over the points\n"
         << "mfront_queue.submit([&](sycl::handler &mfront_handler) {"
         << "mfront_handler.parallel_for(mfront_npoints, mfront_kernel";
      if (!handleStrides) {
        os << "_without_strides";
      }
      os << ");});\n";
    };
    if (treatStrides) {
      if (mpd.inputs.empty()) {
        os << "if(mfront_output_stride == 0){\n"
           << "mfront_kernel(0);\n"
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
           << "if(mfront_areAllArgumentsStrideOne && (mfront_output_stride == "
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

  SYCLBackend::~SYCLBackend() = default;

}  // end of namespace mfront::generic_parallel::material_property
