/*!
 * \file   BackendBase.cxx
 * \brief  This file implements the `BackendBase` class
 * \author Thomas Helfer
 * \date 07/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <ostream>
#include "TFEL/Raise.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyParametersHandler.hxx"
#include "MFront/GenericMaterialPropertyInterfaceBase.hxx"
#include "MFront/GenericParallelMaterialPropertyInterface.hxx"
#include "MFront/GenericParallelMaterialProperty/PointStatus.hxx"
#include "MFront/GenericParallel/MaterialProperty/BackendBase.hxx"

namespace mfront::generic_parallel::material_property {

  /*
   * The identifiers of the checks performed by the kernels, stored in the
   * keys of the statistics (see `mfront::gpmp::makeKey`), are given by the
   * `generic` interface, which writes the code evaluating a material
   * property at a point (see
   * `GenericMaterialPropertyInterfaceBase::writePointEvaluation`). The
   * physical bounds and the bounds are numbered variable by variable, the
   * output being after the arguments, the upper bound before the lower
   * bound.
   */
  using GenericBase = ::mfront::GenericMaterialPropertyInterfaceBase;

  //! \brief identifier of the check of the finiteness of the result
  static constexpr unsigned long long invalid_result_check =
      GenericBase::invalid_result_check;

  //! \return the identifier of the check of a physical bound
  static unsigned long long getPhysicalBoundCheck(const std::size_t i,
                                                  const bool upper) {
    return GenericBase::getPhysicalBoundCheck(i, upper);
  }  // end of getPhysicalBoundCheck

  //! \return the identifier of the check of a bound
  static unsigned long long getBoundCheck(
      const MaterialPropertyDescription& mpd,
      const std::size_t i,
      const bool upper) {
    return GenericBase::getBoundCheck(mpd, i, upper);
  }  // end of getBoundCheck

  /*!
   * \return the output value at the current point
   * \param[in] handleStrides: if true, the output is strided
   */
  static std::string getOutputValue(const bool handleStrides) {
    return handleStrides ? "mfront_output[mfront_output_stride * mfront_idx]"
                         : "mfront_output[mfront_idx]";
  }  // end of getOutputValue

  /*!
   * \return the code ending the evaluation at the current point when it
   * fails: as with the `generic` interface, the output is `NaN`
   * \param[in] i: interface
   * \param[in] handleStrides: if true, the output is strided
   */
  static std::string getFailureEnd(
      const GenericParallelMaterialPropertyInterface& i,
      const bool handleStrides) {
    const auto types = i.getTypesDescription();
    return getOutputValue(handleStrides) + " = std::numeric_limits<" +
           types.real_type + ">::quiet_NaN();\n" + "return mfront_status;\n";
  }  // end of getFailureEnd

  /*!
   * \brief write the code reporting an invalid result (not finite) in a
   * kernel: the evaluation fails at this point, and, as with the `generic`
   * interface, the output is the computed value
   * \param[in] os: output stream
   * \param[in] mpd: material property description
   */
  static void writeInvalidResultCheck(std::ostream& os,
                                      const MaterialPropertyDescription& mpd) {
    const auto v =
        useQuantities(mpd) ? mpd.output.name + ".getValue()" : mpd.output.name;
    os << "if(!tfel::math::ieee754::isfinite(" << v << ")){\n"
       << "mfront_status.reportInvalidResult(" << invalid_result_check
       << "ull);\n"
       << "}\n";
  }  // end of writeInvalidResultCheck

  /*!
   * \brief write the code analysing the statistics `mfront_statistics` in
   * the function reporting the result. The reported point is the first point
   * where the evaluation failed, otherwise the first point where the result
   * is unreliable: the status, the bounds status and the message describe
   * the check retained at this point.
   * \param[in] os: output stream
   * \param[in] i: interface
   * \param[in] mpd: material property description
   */
  static void writeStatisticsAnalysis(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) {
    const auto types = i.getTypesDescription();
    const auto ns = std::string{"::mfront::gpmp::"};
    os << "const auto mfront_number_of_failures = static_cast<"
       << types.integer_type << ">(mfront_statistics->number_of_failures);\n"
       << "const auto mfront_number_of_unreliable_results = static_cast<"
       << types.integer_type
       << ">(mfront_statistics->number_of_unreliable_results);\n"
       << "mfront_output_status->number_of_failures = "
       << "mfront_number_of_failures;\n"
       << "mfront_output_status->number_of_unreliable_results = "
       << "mfront_number_of_unreliable_results;\n"
       << "if (mfront_statistics->first_failure != " << ns << "no_event) {\n"
       << "mfront_output_status->first_failure_index = static_cast<"
       << types.integer_type << ">(" << ns
       << "getPointIndex(mfront_statistics->first_failure));\n"
       << "}\n"
       << "if (mfront_statistics->first_unreliable_result != " << ns
       << "no_event) {\n"
       << "mfront_output_status->first_unreliable_result_index = static_cast<"
       << types.integer_type << ">(" << ns
       << "getPointIndex(mfront_statistics->first_unreliable_result));\n"
       << "}\n"
       // reported point: first failure, otherwise first unreliable result
       << "const bool mfront_failure = "
       << "mfront_statistics->first_failure != " << ns << "no_event;\n"
       << "const auto mfront_key = mfront_failure ? "
       << "mfront_statistics->first_failure : "
       << "mfront_statistics->first_unreliable_result;\n"
       << "if (mfront_key == " << ns << "no_event) {\n"
       << "return 0;\n"
       << "}\n"
       << "const auto mfront_check = " << ns << "getCheck(mfront_key);\n"
       << "const auto mfront_points = \" at point \" + std::to_string(" << ns
       << "getPointIndex(mfront_key)) + \" (\" + "
       << "std::to_string(mfront_number_of_failures) + \" failure(s), \" + "
       << "std::to_string(mfront_number_of_unreliable_results) + "
       << "\" unreliable result(s))\";\n"
       << "if (mfront_check == " << invalid_result_check << "ull) {\n"
       << "mfront_output_status->status = -4;\n"
       << "mfront_report(\"invalid result\" + mfront_points);\n"
       << "return -4;\n"
       << "}\n";
    // as with the `generic` interface, the status and the bounds status are
    // negative if the evaluation failed (physical bound, or bound with the
    // strict policy), positive otherwise (bound with the warning policy)
    auto write_check = [&os](const unsigned long long c, const std::string& msg,
                             const std::size_t rank) {
      os << "if (mfront_check == " << c << "ull) {\n"
         << "const auto mfront_status = mfront_failure ? -1 : 1;\n"
         << "mfront_output_status->status = mfront_status;\n"
         << "mfront_output_status->bounds_status = mfront_status * " << rank
         << ";\n"
         << "mfront_report(\"" << msg << "\" + mfront_points);\n"
         << "return mfront_status;\n"
         << "}\n";
    };
    auto for_each_variable = [&mpd](const auto& f) {
      for (std::size_t idx = 0; idx != mpd.inputs.size(); ++idx) {
        f(mpd.inputs[idx], idx);
      }
      f(mpd.output, mpd.inputs.size());
    };
    auto has_upper_bound = [](const VariableBoundsDescription& b) {
      return (b.boundsType == VariableBoundsDescription::UPPER) ||
             (b.boundsType == VariableBoundsDescription::LOWERANDUPPER);
    };
    auto has_lower_bound = [](const VariableBoundsDescription& b) {
      return (b.boundsType == VariableBoundsDescription::LOWER) ||
             (b.boundsType == VariableBoundsDescription::LOWERANDUPPER);
    };
    for_each_variable([&write_check, &has_upper_bound, &has_lower_bound](
                          const VariableDescription& v, const std::size_t idx) {
      if (!v.hasPhysicalBounds()) {
        return;
      }
      const auto& b = v.getPhysicalBounds();
      if (has_upper_bound(b)) {
        write_check(getPhysicalBoundCheck(idx, true),
                    v.name + " is over its physical upper bound", idx + 1);
      }
      if (has_lower_bound(b)) {
        write_check(getPhysicalBoundCheck(idx, false),
                    v.name + " is below its physical lower bound", idx + 1);
      }
    });
    for_each_variable([&write_check, &has_upper_bound, &has_lower_bound, &mpd](
                          const VariableDescription& v, const std::size_t idx) {
      if (!v.hasBounds()) {
        return;
      }
      const auto& b = v.getBounds();
      if (has_upper_bound(b)) {
        write_check(getBoundCheck(mpd, idx, true),
                    v.name + " is over its upper bound", idx + 1);
      }
      if (has_lower_bound(b)) {
        write_check(getBoundCheck(mpd, idx, false),
                    v.name + " is below its lower bound", idx + 1);
      }
    });
    // the keys are only built by the kernels of this material property
    os << "mfront_output_status->status = -5;\n"
       << "mfront_report(\"invalid statistics (unknown check)\");\n"
       << "return -5;\n";
  }  // end of writeStatisticsAnalysis

  /*!
   * \brief write the initialization of the output status: success, no
   * failure, no unreliable result, empty message
   * \param[in] os: output stream
   * \param[in] i: interface
   */
  static void writeOutputStatusInitialization(
      std::ostream& os, const GenericParallelMaterialPropertyInterface& i) {
    const auto types = i.getTypesDescription();
    const auto max =
        "std::numeric_limits<" + types.integer_type + ">::max();\n";
    os << "mfront_output_status->status = 0;\n"
       << "mfront_output_status->number_of_failures = 0;\n"
       << "mfront_output_status->first_failure_index = " << max
       << "mfront_output_status->number_of_unreliable_results = 0;\n"
       << "mfront_output_status->first_unreliable_result_index = " << max
       << "mfront_output_status->bounds_status = 0;\n"
       << "mfront_output_status->msg[0] = '\\0';\n";
  }  // end of writeOutputStatusInitialization

  /*!
   * \brief write the end of the `try` block which encloses the body of a
   * generated function, so that no exception goes through the C interface.
   * Nothing is allocated: the message is a literal.
   * \param[in] os: output stream
   */
  static void writeUnexpectedExceptionHandling(std::ostream& os) {
    os << "} catch (...) {\n"
       << "mfront_output_status->status = -7;\n"
       << "std::strncpy(mfront_output_status->msg, "
       << "\"unexpected exception\", 511);\n"
       << "mfront_output_status->msg[511] = '\\0';\n"
       << "return -7;\n"
       << "}\n";
  }  // end of writeUnexpectedExceptionHandling

  BackendBase::BackendBase() = default;

  bool BackendBase::handlesDataTransfer() const {
    return false;
  }  // end of handlesDataTransfer

  bool BackendBase::mergesPointStatusesInKernels() const {
    return false;
  }  // end of mergesPointStatusesInKernels

  std::string BackendBase::getAtomicIncrement(const std::string_view) const {
    tfel::raise(
        "BackendBase::getAtomicIncrement: "
        "the statuses of the points are merged by a reduction");
  }  // end of getAtomicIncrement

  std::string BackendBase::getAtomicMinimum(const std::string_view,
                                            const std::string_view) const {
    tfel::raise(
        "BackendBase::getAtomicMinimum: "
        "the statuses of the points are merged by a reduction");
  }  // end of getAtomicMinimum

  void BackendBase::writeBatchError(std::ostream& os,
                                    const int status,
                                    const std::string_view message) {
    os << "mfront_output_status->status = " << status << ";\n"
       << "mfront_report(" << message << ");\n"
       << "return " << status << ";\n";
  }  // end of writeBatchError

  void BackendBase::writeStatisticsDeclaration(
      std::ostream& os,
      const std::string_view device_type,
      const std::string_view device,
      const std::string_view allocation) {
    const auto pool =
        "mfront::generic_parallel::StatusBlockPool<"
        "mfront_gpmp_Statistics, " +
        std::string{device_type} + ">";
    // statistics given by the calling code (deferred report), or block of
    // the pool, initialized here
    os << "static auto mfront_statistics_blocks = " << pool << "{};\n"
       << "auto mfront_statistics_block = std::optional<" << pool
       << "::Block>{};\n"
       << "auto* mfront_statistics = mfront_given_statistics;\n"
       << "if (mfront_statistics == nullptr) {\n"
       << "mfront_statistics_block.emplace("
       << "mfront_statistics_blocks.acquire(" << device << ", [&]{\n"
       << allocation << "\n"
       << "}));\n"
       << "mfront_statistics = mfront_statistics_block->get();\n"
       << "if (mfront_statistics == nullptr) {\n";
    writeBatchError(os, -7,
                    "\"unable to allocate the statistics of the evaluation\"");
    os << "}\n"
       << "mfront_gpmp_initializeStatistics(mfront_statistics);\n"
       << "}\n";
  }  // end of writeStatisticsDeclaration

  void BackendBase::writeLocalStatisticsDeclaration(std::ostream& os) {
    os << "auto mfront_local_statistics = mfront_gpmp_Statistics{};\n"
       << "mfront_gpmp_initializeStatistics(&mfront_local_statistics);\n"
       << "auto* const mfront_statistics = mfront_deferred_report ? "
       << "mfront_given_statistics : &mfront_local_statistics;\n";
  }  // end of writeLocalStatisticsDeclaration

  void BackendBase::writeErrorReporting(std::ostream& os) {
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
  }  // end of writeErrorReporting

  void BackendBase::writeReportFunction(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    const auto name = i.getFunctionName(mpd);
    const auto types = i.getTypesDescription();
    os << "MFRONT_SHAREDOBJ int " << name << "_generic_parallel_report("
       << types.output_status_type << "* const mfront_output_status,\n"
       << "const mfront_gpmp_Statistics* const mfront_statistics){\n"
       << "if (mfront_output_status == nullptr) {\n"
       << "return -5;\n"
       << "}\n";
    writeErrorReporting(os);
    writeOutputStatusInitialization(os, i);
    // the messages are built in a `try` block
    os << "try {\n"
       << "if (mfront_statistics == nullptr) {\n";
    writeBatchError(os, -5, "\"no statistics given\"");
    os << "}\n";
    if (!areRuntimeChecksDisabled(mpd)) {
      writeStatisticsAnalysis(os, i, mpd);
    } else {
      os << "return 0;\n";
    }
    writeUnexpectedExceptionHandling(os);
    os << "} // end of " << name << "_generic_parallel_report\n\n";
  }  // end of writeReportFunction

  void BackendBase::writeDeviceStackSizeSetting(std::ostream& os,
                                                const std::size_t stack_size) {
    if (stack_size == 0) {
      return;
    }
    os << "if (!mfront::generic_parallel::setMinimalCUDAStackSize("
       << stack_size << ", mfront_preserve_stack_size)) {\n";
    writeBatchError(os, -7,
                    "mfront::generic_parallel::stack_size_error_message");
    os << "}\n";
  }  // end of writeDeviceStackSizeSetting

  void BackendBase::writeCDeclarations(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    const auto name = i.getFunctionName(mpd);
    const auto types = i.getTypesDescription();
    os << "/*!\n"
       << " * \\brief compute the value of '" << mpd.className
       << "' material property on a batch of points (see "
       << "`mfront_gpmp_BatchData`)\n"
       << " *\n"
       << " * \\param[out] mfront_output_status: output status\n"
       << " * \\param[in] mfront_batch: data of the batch\n"
       << " * \\return the status, also given by the output status\n"
       << " */\n"
       << "MFRONT_SHAREDOBJ int " << name << "_generic_parallel(\n"
       << types.output_status_type << "* const,\n"
       << "const mfront_gpmp_BatchData* const);\n\n"
       << "/*!\n"
       << " * \\brief analyse the statistics of an evaluation of '"
       << mpd.className << "' material property, once the computations are "
       << "completed, and report the result as a synchronous evaluation "
       << "does\n"
       << " *\n"
       << " * \\param[out] mfront_output_status: output status\n"
       << " * \\param[in] mfront_statistics: statistics given by the "
       << "calling code, read on the host\n"
       << " * \\return the status, also given by the output status\n"
       << " */\n"
       << "MFRONT_SHAREDOBJ int " << name << "_generic_parallel_report(\n"
       << types.output_status_type << "* const,\n"
       << "const mfront_gpmp_Statistics* const);\n\n";
  }  // end of writeDeclarations

  void BackendBase::writeProxyObjects(
      std::ostream&,
      const GenericParallelMaterialPropertyInterface&,
      const MaterialPropertyDescription&) const {}  // end of writeProxyObject

  bool BackendBase::isBoundsCheckingRequired(
      const MaterialPropertyDescription& mpd) {
    // the statistics also report the invalid results
    return !areRuntimeChecksDisabled(mpd);
  }  // end of isBoundsCheckingRequired

  void BackendBase::writeCFunctionBeginning(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    const auto name = i.getFunctionName(mpd);
    const auto nname = i.getInterfaceInternalNamespace();
    const auto types = i.getTypesDescription();
    const auto& params = mpd.parameters;
    const auto nargs = mpd.inputs.size();
    // the identifier of a check is stored in 16 bits of the keys of the
    // statistics
    if (getBoundCheck(mpd, nargs, false) >
        ::mfront::gpmp::maximal_number_of_checks) {
      tfel::raise(
          "BackendBase::writeCFunctionBeginning: "
          "too many arguments");
    }
    const auto output =
        this->handlesDataTransfer() ? "mfront_output_host" : "mfront_output";
    const auto args =
        this->handlesDataTransfer() ? "mfront_args_host" : "mfront_args";
    // signature
    os << "MFRONT_SHAREDOBJ int " << name << "_generic_parallel("
       << types.output_status_type << "* const mfront_output_status,\n"
       << "const mfront_gpmp_BatchData* const mfront_batch)\n{\n"
       << "if (mfront_output_status == nullptr) {\n"
       << "return -5;\n"
       << "}\n";
    writeErrorReporting(os);
    // errno is restored on every return. It is not used to detect the
    // errors: it is thread-local, does not exist on GPUs, and may be modified
    // by the runtimes (for instance at the initialisation of TBB). Invalid
    // results are detected by the kernels.
    os << "struct mfront_ErrnoGuard {\n"
       << "const int value = errno;\n"
       << "~mfront_ErrnoGuard() { errno = this->value; }\n"
       << "};\n"
       << "[[maybe_unused]] const auto mfront_errno_guard = "
       << "mfront_ErrnoGuard{};\n";
    writeOutputStatusInitialization(os, i);
    // the checks build messages, which may throw: the body of the function
    // is enclosed in a `try` block, closed by writeCFunctionEnd
    os << "try {\n"
       << "if (mfront_batch == nullptr) {\n";
    writeBatchError(os, -5, "\"no batch data given\"");
    os << "}\n"
       << "const auto mfront_npoints = mfront_batch->number_of_points;\n"
       // an empty batch is accepted without any check: nothing is read,
       // written or submitted
       << "if (mfront_npoints == 0) {\n"
       << "return 0;\n"
       << "}\n"
       << "if (mfront_npoints > ::mfront::gpmp::maximal_number_of_points) {\n";
    writeBatchError(os, -5,
                    "\"too many points (\" + std::to_string(mfront_npoints) + "
                    "\" given)\"");
    os << "}\n"
       // statistics given by the calling code: the output status then only
       // reports the errors of the batch (see `writeResultsAnalysis`)
       << "[[maybe_unused]] auto* const mfront_given_statistics = "
       << "mfront_batch->statistics;\n"
       << "[[maybe_unused]] const bool mfront_deferred_report = "
       << "mfront_given_statistics != nullptr;\n"
       // execution policy given by the calling code, checked against the
       // policy of the backend
       << "const auto mfront_backend_policy = "
       << "::mfront::generic_parallel::checkBackendExecutionPolicy<"
       << this->getExecutionPolicyType() << ">("
       << "mfront_batch->execution_policy, mfront_deferred_report);\n"
       << "if (mfront_backend_policy.error != nullptr) {\n";
    writeBatchError(os, -5, "mfront_backend_policy.error");
    os << "}\n"
       << "[[maybe_unused]] const bool mfront_asynchronous = "
       << "mfront_backend_policy.common_options.asynchronous;\n"
       << "[[maybe_unused]] const bool mfront_preserve_stack_size = "
       << "mfront_backend_policy.common_options.preserve_stack_size;\n"
       << "[[maybe_unused]] const auto mfront_out_of_bounds_policy = "
       << "mfront_batch->out_of_bounds_policy;\n"
       << "[[maybe_unused]] auto* mfront_statuses = "
       << "mfront_batch->statuses;\n"
       << types.real_type << "* const " << output
       << " = mfront_batch->output.values;\n"
       << "const auto mfront_output_stride = "
       << "mfront_batch->output.point_stride;\n"
       << "if (" << output << " == nullptr) {\n";
    writeBatchError(os, -5, "\"no output values given\"");
    os << "}\n"
       << "if ((mfront_output_stride == 0) && (mfront_npoints != 1)) {\n";
    writeBatchError(os, -5,
                    "\"if the output is uniform, the number of points must be "
                    "equal to one (\" + std::to_string(mfront_npoints) + \" "
                    "given)\"");
    os << "}\n";
    // as with the `generic` interface, the number of arguments is not
    // checked if the runtime checks are disabled
    if (!areRuntimeChecksDisabled(mpd)) {
      os << "if (mfront_batch->number_of_arguments != " << nargs << ") {\n";
      writeBatchError(
          os, -5,
          "\"invalid number of arguments (\" + "
          "std::to_string(mfront_batch->number_of_arguments) + \" given, " +
              std::to_string(nargs) + " expected)\"");
      os << "}\n";
    }
    // pointers to the values and strides of the arguments
    if (nargs != 0) {
      os << "if (mfront_batch->arguments == nullptr) {\n";
      writeBatchError(os, -5, "\"no arguments given\"");
      os << "}\n"
         << "auto " << args << " = std::array<const " << types.real_type
         << "*, " << nargs << ">{};\n"
         << "auto mfront_strides = std::array<" << types.integer_type << ", "
         << nargs << ">{};\n";
      for (std::size_t idx = 0; idx != nargs; ++idx) {
        const auto a = "mfront_batch->arguments[" + std::to_string(idx) + "]";
        os << args << "[" << idx << "] = " << a << ".values;\n"
           << "mfront_strides[" << idx << "] = " << a << ".point_stride;\n"
           << "if (" << args << "[" << idx << "] == nullptr) {\n";
        writeBatchError(os, -5,
                        "\"no values given for argument '" +
                            mpd.inputs[idx].getExternalName() + "'\"");
        os << "}\n";
      }
    }
    if ((!areRuntimeChecksDisabled(mpd)) &&
        (!areParametersTreatedAsStaticVariables(mpd)) && (!params.empty())) {
      const auto hn = getMaterialPropertyParametersHandlerClassName(name);
      const auto handler = nname + "::" + hn + "::get" + hn + "()";
      os << "if(!" << handler << ".ok){\n";
      writeBatchError(os, -6, handler + ".msg");
      os << "}\n";
    }
  }  // end of writeCFunctionBeginning

  void BackendBase::writeCFunctionEnd(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) {
    os << "return mfront_output_status->status;\n";
    writeUnexpectedExceptionHandling(os);
    os << "} // end of " << i.getFunctionName(mpd) << "_generic_parallel\n\n";
  }  // end of writeCFunctionEnd

  void BackendBase::writeHostExceptionsHandling(std::ostream& os) {
    os << "} catch(std::exception& mfront_cxx_exception){\n";
    writeBatchError(os, -7, "mfront_cxx_exception.what()");
    os << "} catch(...){\n";
    writeBatchError(os, -7, "\"unknown exception\"");
    os << "}\n";
  }  // end of writeHostExceptionsHandling

  void BackendBase::writeKernelBody(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const bool handleStrides) const {
    const auto iucname = i.getInterfaceNameInUpperCase();
    const auto prefix = i.getOutOfBoundsPolicyEnumerationPrefix();
    const auto qt = useQuantities(mpd);
    const auto checks = isBoundsCheckingRequired(mpd);
    const auto failure = getFailureEnd(i, handleStrides);
    // parameters
    for (const auto& p : mpd.parameters) {
      if (areParametersTreatedAsStaticVariables(mpd)) {
        const auto pv =
            p.getAttribute<double>(VariableDescription::defaultValue);
        os << "constexpr auto " << p.name << " = "
           << (qt ? p.type : std::string{"double"}) << "(" << pv << ");\n";
      } else {
        os << "const auto " << p.name << " = "
           << (qt ? p.type : std::string{"real"}) << "(mfront_parameters."
           << p.name << ");\n";
      }
    }
    // arguments
    for (std::size_t idx = 0; idx != mpd.inputs.size(); ++idx) {
      const auto& v = mpd.inputs[idx];
      const auto arg = "mfront_arg" + std::to_string(idx);
      const auto value = handleStrides
                             ? arg + "[mfront_strides[" + std::to_string(idx) +
                                   "] * mfront_idx]"
                             : arg + "[mfront_idx]";
      os << "const auto " << v.name << " = "
         << (qt ? v.type + "(" + value + ")" : value) << ";\n";
    }
    // with the runtime checks, the evaluation is written in a lambda
    // function returning the status of the point, so that a failed check
    // ends the evaluation at this point
    if (checks) {
      os << "const auto mfront_point_status = [=]() -> "
         << "::mfront::gpmp::PointStatus {\n"
         << "auto mfront_status = ::mfront::gpmp::PointStatus{};\n";
    }
    // output
    os << "auto " << mpd.output.name << " = " << mpd.output.type << "{};\n";
    // evaluation at the point, written as for the `generic` interface: a
    // failed check ends the evaluation at this point (the output is `NaN`)
    GenericBase::writePointEvaluation(
        os, mpd,
        GenericBase::PointEvaluationOptions{
            .bounds_check_macro = "NO_" + iucname + "_BOUNDS_CHECK",
            .out_of_bounds_policy_prefix = prefix,
            .failure =
                [&failure](std::ostream& out,
                           const GenericBase::BoundCheck& c) {
                  out << "mfront_status.report(" << c.identifier
                      << "ull, true);\n"
                      << failure;
                },
            .warning =
                [](std::ostream& out, const GenericBase::BoundCheck& c) {
                  out << "mfront_status.report(" << c.identifier
                      << "ull, false);\n";
                }});
    if (checks) {
      writeInvalidResultCheck(os, mpd);
    }
    os << getOutputValue(handleStrides) << " = " << mpd.output.name
       << (qt ? ".getValue();\n" : ";\n");
    if (!checks) {
      // without runtime checks, the evaluation always succeeds
      os << "if (mfront_statuses != nullptr) {\n"
         << "mfront_statuses[mfront_idx] = 0;\n"
         << "}\n";
      return;
    }
    os << "return mfront_status;\n"
       << "}();\n"
       << "if (mfront_statuses != nullptr) {\n"
       << "mfront_statuses[mfront_idx] = mfront_point_status.status;\n"
       << "}\n";
    if (!this->mergesPointStatusesInKernels()) {
      // merged by a reduction
      os << "return mfront_point_status;\n";
      return;
    }
    // the statistics are only accessed if an event occurred
    const auto increment = [this](const std::string_view c) {
      return this->getAtomicIncrement("mfront_statistics->" + std::string{c});
    };
    const auto minimum = [this](const std::string_view c,
                                const std::string_view v) {
      return this->getAtomicMinimum("mfront_statistics->" + std::string{c}, v);
    };
    os << "if (mfront_point_status.hasEvent()) {\n"
       << "const auto mfront_key = ::mfront::gpmp::makeKey("
       << "static_cast<unsigned long long>(mfront_idx), "
       << "mfront_point_status.check);\n"
       << "if (mfront_point_status.isFailure()) {\n"
       << increment("number_of_failures") << ";\n"
       << minimum("first_failure", "mfront_key") << ";\n"
       << "} else {\n"
       << increment("number_of_unreliable_results") << ";\n"
       << minimum("first_unreliable_result", "mfront_key") << ";\n"
       << "}\n"
       << "}\n";
  }  // end of writeKernelBody

  void BackendBase::writeKernelSelection(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd,
      const std::function<void(std::string_view, bool)>& write_call) const {
    // a single point is evaluated by the kernel without strides, whatever
    // the strides: only the first values are read and written
    os << "if (mfront_npoints == 1) {\n";
    write_call("1", false);
    os << "} else {\n";
    if (mpd.inputs.empty()) {
      os << "if (mfront_output_stride == 1) {\n";
    } else {
      const auto types = i.getTypesDescription();
      os << "const bool mfront_areAllArgumentsStrideOne = std::all_of("
         << "mfront_strides.begin(), mfront_strides.end(), "
         << "[](const " << types.integer_type << " mfront_stride){"
         << "return mfront_stride == 1;});\n"
         << "if (mfront_areAllArgumentsStrideOne && "
         << "(mfront_output_stride == 1)) {\n";
    }
    write_call("mfront_npoints", false);
    os << "} else {\n";
    write_call("mfront_npoints", true);
    os << "}\n"
       << "}\n";
  }  // end of writeKernelSelection

  void BackendBase::writeResultsAnalysis(
      std::ostream& os,
      const GenericParallelMaterialPropertyInterface& i,
      const MaterialPropertyDescription& mpd) const {
    if (areRuntimeChecksDisabled(mpd)) {
      return;
    }
    // with statistics given by the calling code, the output status only
    // reports the errors of the batch: the result is given by the
    // `<name>_generic_parallel_report` function
    os << "if (!mfront_deferred_report) {\n"
       << i.getFunctionName(mpd) << "_generic_parallel_report("
       << "mfront_output_status, mfront_statistics);\n"
       << "}\n";
  }  // end of writeResultsAnalysis

  BackendBase::~BackendBase() noexcept = default;

}  // end of namespace mfront::generic_parallel::material_property
