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

  SYCLBackend::SYCLBackend()
      : execution_policy("std::execution::par_unseq") {
  }  // end of SYCLBackend

  SYCLBackend::SYCLBackend(const tfel::utilities::DataMap& opts)
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
  }  // end of SYCLBackend

  std::string SYCLBackend::getName() const {
    return "sycl";
  }  // end of getName

  std::string SYCLBackend::getHeaderFileExtension() const {
    return "hxx";
  }  // end of getHeaderFileExtension

  std::string SYCLBackend::getSourceFileExtension() const {
    return "cxx";
  }  // end of getSourceFileExtension

  void SYCLBackend::writeSpecificIncludesInHeaderFile(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
  }  // end of writeSpecificIncludesInSourceFile

  void SYCLBackend::writeSpecificIncludesInSourceFile(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {
    os << "#include<ranges>\n"
       << "#include<concepts>\n"
       << "#include<type_traits>\n"
       << "#include <sycl/sycl.hpp>\n"
       << "#include\"TFEL/FSAlgorithm/copy.hxx\"\n\n";
  }  // end of writeSpecificIncludesInSourceFile

  void SYCLBackend::writeGlobalFunctions(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&,
      const FileDescription&) const {}  // end of writeGlobalFunction

  void SYCLBackend::writeCxxDeclarations(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {}

  void SYCLBackend::writeCxxImplementations(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&,
      const FileDescription&) const {}

  void SYCLBackend::writeCImplementations(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd) const {
    for (const auto b : {true, false}) {
      this->writeCImplementations2(os, i, mpd, fd, b);
    }
  }  // end of writeCImplementations

  void SYCLBackend::writeCImplementations2(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd,
      const bool treatStrides) const {
    const auto name = i.getFunctionName(mpd);
    const auto nname = i.getInterfaceInternalNamespace();
    const auto types = i.getTypesDescription();
    const auto& params = mpd.parameters;
    const auto iucname = i.getInterfaceNameInUpperCase();
    const auto prefix = i.getOutOfBoundsPolicyEnumerationPrefix();
    os << "MFRONT_SHAREDOBJ void " << name;
    if (!treatStrides) {
      os << '2';
    }
    os << "(" << types.output_status_type << "* const mfront_output_status,\n"
       << types.real_type << "* const mfront_output";
    if (treatStrides) {
      os << ",\nconst " << types.integer_type << " mfront_output_stride";
    }
    os << ",\nconst " << types.real_type << "* const * const";
    if (!mpd.inputs.empty()) {
      os << " mfront_args";
    }
    if (treatStrides) {
      os << ",\nconst " << types.integer_type << "* const";
      if (!mpd.inputs.empty()) {
        os << " mfront_args_strides";
      }
    }
    os << ",\nconst " << types.integer_type << " mfront_nargs"
       << ",\nconst " << types.integer_type << " mfront_npoints"
       << ",\nconst " << types.out_of_bounds_policy_type;
    if ((hasBounds(mpd.inputs)) || (hasBounds(mpd.output))) {
      os << " mfront_out_of_bounds_policy";
    }
    os << ")\n{\n";
    os << "[[maybe_unused]] auto mfront_report = "
       << "[&mfront_output_status](const std::string& "
       << "mfront_error_message){\n"
       << "if(mfront_error_message.empty()){\n"
       << "return;\n"
       << "}\n"
       << "std::strncpy(mfront_output_status->msg,"
       << "mfront_error_message.c_str(),511);\n"
       << "mfront_output_status->msg[511]='\\0';\n"
       << "};\n";
    if (!areRuntimeChecksDisabled(mpd)) {
      os << "auto mfront_check_positive_or_null = "
         << "[]<std::integral mfront_IntegerType>"
         << "(const mfront_IntegerType mfront_integral_value){\n"
         << "if constexpr (std::is_signed_v<mfront_IntegerType>){\n"
         << "if(mfront_integral_value < 0){\n"
         << "return false;\n"
         << "}\n"
         << "}\n"
         << "return true;\n"
         << "};\n"
         << "const int mfront_errno_old = errno;\n";
    }
    os << "mfront_output_status->status = 0;\n"
       << "mfront_output_status->bounds_status = 0;\n"
       << "mfront_output_status->c_error_number = 0;\n";
    if (!areRuntimeChecksDisabled(mpd)) {
      // C-error handling
      os << "errno = 0;\n";
      // check number of points
      os << "if(!mfront_check_positive_or_null(mfront_npoints)){\n"
         << "mfront_output_status->status = -5;\n"
         << "mfront_report(\"invalid number of points "
         << "(\"+std::to_string(mfront_npoints)+\" given\");\n"
         << "errno = mfront_errno_old;\n"
         << "return;\n"
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
      if (treatStrides) {
        os << "if(!mfront_check_positive_or_null(mfront_output_stride)){\n"
           << "mfront_output_status->status = -5;\n"
           << "mfront_report(\"negative output stride "
           << "(\"+std::to_string(mfront_output_stride)+\" given\");\n"
           << "errno = mfront_errno_old;\n"
           << "return;\n"
           << "}\n";
        for (std::size_t idx = 0; idx != mpd.inputs.size(); ++idx) {
          os << "if(!mfront_check_positive_or_null(mfront_args_strides[" << idx
             << "])){\n"
             << "mfront_output_status->status = -5;\n"
             << "mfront_report(\"negative stride given for variable '"
             << mpd.inputs[idx].getExternalName()
             << "' (\"+std::to_string(mfront_args_strides[" << idx
             << "])+\" given\");\n"
             << "errno = mfront_errno_old;\n"
             << "return;\n"
             << "}\n";
        }
      }
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
    if (treatStrides) {
      os << "if((mfront_output_stride == 0) && (mfront_npoints != 1)){\n"
         << "mfront_output_status->status = -7;\n"
         << "mfront_report(\"if the output is uniform, the number of points "
         << "must be equal to one (\" + std::to_string(mfront_npoints) + \" "
         << "given)\");\n"
         << "errno = mfront_errno_old;\n"
         << "return;\n"
         << "}\n";
    }
    os << "if(mfront_npoints <= 0){\n"
       << "return;\n"
       << "}\n";
    //
    this->writeKernelCall(os, i, mpd, fd, treatStrides);
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
    os << "} // end of " << name << "\n\n";
  }  // end of writeCImplementations2

  void SYCLBackend::writeKernelCall(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd,
      const bool treatStrides) const {
    const auto types = i.getTypesDescription();
    const auto requiresBoundsCheck =
        (!areRuntimeChecksDisabled(mpd)) &&
        ((hasPhysicalBounds(mpd.inputs)) || (hasBounds(mpd.inputs)) ||
         (mpd.output.hasPhysicalBounds()) || (mpd.output.hasBounds()));
    if ((treatStrides) && (!mpd.inputs.empty())) {
      os << "auto mfront_args_strides_tmp = std::array<" << types.integer_type
         << ", " << mpd.inputs.size() << ">{};\n"
         << "tfel::fsalgo::copy<" << mpd.inputs.size() << ">::exe("
         << "mfront_args_strides, "
         << "mfront_args_strides_tmp.begin());\n";
      os << "const bool mfront_areAllArgumentsUniform = std::all_of("
         << "mfront_args_strides_tmp.begin(), "
         << "mfront_args_strides_tmp.end(), "
         << "[](const " << types.integer_type
         << " mfront_stride){return mfront_stride == 0;});\n"
         << "if((mfront_output_stride == 0) && "
         << "(!mfront_areAllArgumentsUniform)){\n"
         << "mfront_output_status->status = -7;\n"
         << "mfront_report(\"if the output is uniform, "
         << "all the arguments must be uniform "
         << "(i.e. their strides must be null)\");\n"
         << "errno = mfront_errno_old;\n"
         << "return;\n"
         << "}\n";
    }
    os << "try{\n"
       << "sycl::queue mfront_queue(sycl::gpu_selector_v);\n";
    if (requiresBoundsCheck) {
      os << "using mfront_allocator_type = "
         << "sycl::usm_allocator<int, sycl::usm::alloc::shared>;\n"
         << "auto mfront_allocator = mfront_allocator_type{Q};\n"
         << "auto mfront_bounds_statuses = "
         << "std::vector<int, front_allocator_type>{mfront_allocator}\n;"
         << "mfront_bounds_statuses.resize(2 * (mfront_nargs + 1), 0);\n";
    }
    auto write_kernel = [this, &os, &i, &mpd, &fd, &requiresBoundsCheck,
                         &types](const bool handleStrides) {
      const auto name = i.getFunctionName(mpd);
      const auto nname = i.getInterfaceInternalNamespace();
      const auto iucname = i.getInterfaceNameInUpperCase();
      const auto prefix = i.getOutOfBoundsPolicyEnumerationPrefix();
      const auto& function = mpd.f;
      const auto& params = mpd.parameters;
      // declaration of the kernel
      os << "auto mfront_kernel";
      if (!handleStrides) {
        os << "_without_strides";
      }
      os << " = [mfront_output";
      if (handleStrides) {
        os << ", mfront_output_stride";
      }
      if (!mpd.inputs.empty()) {
        os << ", mfront_args";
        if (handleStrides) {
          os << ", mfront_args_strides_tmp";
        }
      }
      if (requiresBoundsCheck) {
        os << ", mfront_bounds_statuses = mfront_bounds_statuses.data(), "
           << "mfront_out_of_bounds_policy";
      }
      if ((!areParametersTreatedAsStaticVariables(mpd)) && (!params.empty())) {
        const auto pcn = getMaterialPropertyParametersClassName(name);
        const auto hn = getMaterialPropertyParametersHandlerClassName(name);
        os << ", mfront_parameters = "                        //
           << "static_cast<" << nname << "::" << pcn << ">("  //
           << nname << "::" << hn << "::get" << hn << "())";
      }
      os << "](" << types.integer_type << " mfront_idx){\n";
      writeBeginningOfMaterialPropertyBody(os, mpd, fd, "double", true);
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
          if (handleStrides) {
            os << "const auto " << p3->name << " = " << cast_start;
            os << "(mfront_args[" << idx << "])[mfront_args_strides_tmp[" << idx
               << "] * mfront_idx]";
            os << cast_end << ";\n";
          } else {
            os << "const auto " << p3->name << " = " << cast_start;
            os << "(mfront_args[" << idx << "])[mfront_idx]";
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
      os << function.body;
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
      if (handleStrides) {
        os << "*(mfront_output + mfront_output_stride * mfront_idx) = ";
      } else {
        os << "*(mfront_output + mfront_idx) = ";
      }
      if (useQuantities(mpd)) {
        os << mpd.output.name << ".getValue();\n";
      } else {
        os << mpd.output.name << ";\n";
      }
      // end of the kernel
      os << "}; // end of mfront_kernel";
      if (!handleStrides) {
        os << "_without_strides";
      }
      os << "\n";
    };
    write_kernel(false);
    if (treatStrides) {
      write_kernel(true);
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
    os << "} catch(std::exception& mfront_cxx_exception){\n"
       << "mfront_output_status->status = -2;\n"
       << "mfront_report(mfront_cxx_exception.what());\n"
       << "return;\n"
       << "} catch(...){\n"
       << "mfront_output_status->status = -2;\n"
       << "mfront_report(\"unknown exception\");\n"
       << "return;\n"
       << "}\n";
  }  // end of writeKernelCall

  SYCLBackend::~SYCLBackend() = default;

}  // end of namespace mfront::generic_parallel::material_property
