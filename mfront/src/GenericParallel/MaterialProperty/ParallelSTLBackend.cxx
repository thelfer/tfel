/*!
 * \file   ParallelSTLBackend.cxx
 * \brief  This file implements the `ParallelSTLBackend` class
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
#include "MFront/GenericParallel/MaterialProperty/ParallelSTLBackend.hxx"

namespace mfront::generic_parallel::material_property {

  ParallelSTLBackend::ParallelSTLBackend()
      : execution_policy("std::execution::par_unseq") {
  }  // end of ParallelSTLBackend

  ParallelSTLBackend::ParallelSTLBackend(const tfel::utilities::DataMap& opts)
      : execution_policy("std::execution::par_unseq") {
    auto validator = tfel::utilities::DataMapValidator{};
    validator.addDataTypeValidator<std::string>("execution_policy");
    validator.validate(opts);
    if (contains(opts, "execution_policy")) {
      const auto p = get<std::string>(opts, "execution_policy");
      if ((p == "par") || (p == "parallel_policy")) {
        this->execution_policy = "std::execution::par";
      } else if ((p == "seq") || (p == "sequenced_policy")) {
        this->execution_policy = "std::execution::seq";
      } else if ((p == "unseq") || (p == "unsequenced_policy")) {
        this->execution_policy = "std::execution::unseq";
      } else if ((p == "par_unseq") || (p == "parallel_unsequenced_policy")) {
        this->execution_policy = "std::execution::par_unseq";
      } else {
        tfel::raise(
            "unsuported execution policy '" + p +
            "'. Valid execution policy are: 'sequenced_policy' (or "
            "'seq'), 'unsequenced_policy' (or 'unseq'), 'parallel_policy' (or "
            "'par') and 'parallel_unsequenced_policy' (or 'par_unseq')");
      }
    }
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
       << "#include\"TFEL/FSAlgorithm/copy.hxx\"\n\n";
  }  // end of writeSpecificIncludesInSourceFile

  void ParallelSTLBackend::writeKernelCall(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd,
      const bool treatStrides) const {
    const auto types = i.getTypesDescription();
    const auto requiresBoundsCheck = this->isBoundsCheckingRequired(mpd);
    if (requiresBoundsCheck) {
      // we do need to use heap allocations as nvc++ would automatically use
      // managed memory
      os << "auto mfront_bounds_statuses = std::vector<int>{};\n"
         << "mfront_bounds_statuses.resize(2 * (mfront_nargs + 1), 0);\n";
    }
    this->writeKernel(os, i, mpd, fd, false);
    if (treatStrides) {
      this->writeKernel(os, i, mpd, fd, true);
    }
    auto write_kernel_call = [&os, this, &types](const bool handleStrides) {
      os << "// loop over the points\n"
         << "#ifdef __ADAPTIVECPP__\n"
         << "const auto mfront_index_range = "
         << "std::views::iota(int{}, static_cast<int>(mfront_npoints));\n"
         << "#else /* __ADAPTIVECPP__ */ \n"
         << "const auto mfront_index_range = "
         << "std::views::iota(" << types.integer_type
         << "{}, mfront_npoints);\n"
         << "#endif /* __ADAPTIVECPP__ */ \n"
         << "std::for_each(" << this->execution_policy
         << ", mfront_index_range.begin(), "
         << "mfront_index_range.end(), "
         << "mfront_kernel";
      if (!handleStrides) {
        os << "_without_strides";
      }
      os << ");\n";
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

  ParallelSTLBackend::~ParallelSTLBackend() = default;

}  // end of namespace mfront::generic_parallel::material_property
