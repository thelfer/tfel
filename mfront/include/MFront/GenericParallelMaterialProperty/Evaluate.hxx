/*!
 * \file   mfront/include/MFront/GenericParallelMaterialProperty/Evaluate.hxx
 * \brief  This file implements the evaluation of a material property on a
 * batch of points with the primitive of a backend (see
 * `MFront/GenericParallel/Batch.hxx`). The generated functions only provide
 * the evaluation at one point.
 * \date   09/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLELMATERIALPROPERTY_EVALUATE_HXX
#define LIB_MFRONT_GENERICPARALLELMATERIALPROPERTY_EVALUATE_HXX

#include <array>
#include <limits>
#include <string>
#include <utility>
#include "TFEL/Config/TFELConfig.hxx"
#include "MFront/GenericParallel/Batch.hxx"
#include "MFront/GenericParallelMaterialProperty/MaterialProperty.h"
#include "MFront/GenericParallelMaterialProperty/PointStatus.hxx"

namespace mfront::gpmp {

  //! \brief a simple alias
  using real = mfront_gmp_real;
  //! \brief a simple alias
  using size_type = mfront_gmp_size_type;
  //! \brief statistics of the evaluation of a batch
  using BatchStatistics = ::mfront::generic_parallel::BatchStatistics<real>;
  //! \brief execution of the evaluation of a batch
  using BatchExecution = ::mfront::generic_parallel::BatchExecution<Statistics>;
  //! \brief function reporting the statistics of an evaluation, generated
  //! for each material property (`<name>_generic_parallel_report`)
  using ReportFunction = ::mfront_gpmp_ReportPtr;
  using ::mfront::generic_parallel::report;

  //! \brief result of the evaluation of a material property at one point
  struct PointResult {
    //! \brief value of the material property
    real value;
    //! \brief status of the point
    PointStatus status;
  };

  /*!
   * \return the statistics associated with the status of a point
   * \param[in] i: index of the point
   * \param[in] s: status of the point
   */
  TFEL_HOST_DEVICE constexpr BatchStatistics getPointStatistics(
      const size_type i, const PointStatus& s) noexcept {
    const auto failure = s.isFailure();
    const auto unreliable = s.status == 1;
    const auto key = s.hasEvent()
                         ? makeKey(static_cast<atomic_size_type>(i), s.check)
                         : no_event;
    return {.number_of_failures = failure ? 1u : 0u,
            .first_failure = failure ? key : no_event,
            .number_of_unreliable_results = unreliable ? 1u : 0u,
            .first_unreliable_result = unreliable ? key : no_event};
  }  // end of getPointStatistics

  /*!
   * \brief accumulate the statistics of a batch
   * \param[in,out] s: accumulated statistics
   * \param[in] r: statistics of a batch
   */
  constexpr void accumulate(Statistics& s, const BatchStatistics& r) noexcept {
    s.number_of_failures += r.number_of_failures;
    s.first_failure =
        r.first_failure < s.first_failure ? r.first_failure : s.first_failure;
    s.number_of_unreliable_results += r.number_of_unreliable_results;
    s.first_unreliable_result =
        r.first_unreliable_result < s.first_unreliable_result
            ? r.first_unreliable_result
            : s.first_unreliable_result;
  }  // end of accumulate

  /*!
   * \brief handler of the statistics of the material properties, given to
   * the primitives of the backends (see `MFront/GenericParallel/Batch.hxx`).
   * The statistics are analysed by the report function of the material
   * property, which names the check retained at the reported point.
   */
  struct StatisticsHandler {
    //! \brief a simple alias
    using OutputStatus = ::mfront::gpmp::OutputStatus;
    //! \brief a simple alias
    using Statistics = ::mfront::gpmp::Statistics;
    //! \return the statistics of a batch without event
    constexpr BatchStatistics initial() const noexcept { return {}; }
    /*!
     * \return false if the evaluation at one point never reports an event,
     * which is the case if the runtime checks are disabled: the primitives
     * then neither combine nor report the statistics
     */
    constexpr bool hasEvents() const noexcept { return this->runtime_checks; }
    //! \brief initialize the statistics updated by the devices
    void initialize(Statistics& s) const noexcept {
      mfront_gpmp_initializeStatistics(&s);
    }
    /*!
     * \brief report the statistics updated by the devices, once the
     * computations are completed
     * \return the status
     * \param[out] s: output status
     * \param[in] r: statistics, read on the host
     */
    int report(OutputStatus& s, const Statistics& r) const noexcept {
      return this->report_function(&s, &r);
    }
    /*!
     * \brief report the statistics of a batch combined by a reduction or,
     * if the calling code gave statistics, accumulate them in those
     * statistics (see `mfront_gpmp_BatchData`)
     * \return the status
     * \param[in,out] s: output status
     * \param[in] given: statistics given by the calling code, or null
     * \param[in] r: statistics of the batch
     */
    int report(OutputStatus& s,
               Statistics* const given,
               const BatchStatistics& r) const noexcept {
      if (given != nullptr) {
        accumulate(*given, r);
        return s.status;
      }
      auto statistics = Statistics{};
      mfront_gpmp_initializeStatistics(&statistics);
      accumulate(statistics, r);
      return this->report_function(&s, &statistics);
    }
    //! \brief report function of the material property
    ReportFunction report_function;
    //! \brief if false, the runtime checks are disabled
    bool runtime_checks;
  };  // end of struct StatisticsHandler

  /*!
   * \brief function object evaluating a material property at the points of
   * a batch: the arguments are read, the evaluation at one point is called,
   * and the output and the status of the point are written.
   * \tparam Evaluation: evaluation at one point, generated by `MFront`,
   * taking the values of the arguments and the out of bounds policy, and
   * returning a `PointResult`
   * \tparam N: number of arguments
   */
  template <typename Evaluation, size_type N>
  struct PointEvaluator {
    //! \brief evaluate the material property at the `i`-th point
    TFEL_HOST_DEVICE BatchStatistics operator()(const size_type i) const {
      auto values = std::array<real, N>{};
      // `std::array<T, 0>::operator[]` is not available in device code
      if constexpr (N != 0) {
        for (size_type j = 0; j != N; ++j) {
          const auto& a = this->arguments[j];
          values[j] = a.values[a.point_stride * i];
        }
      }
      const auto r = this->evaluation(values, this->out_of_bounds_policy);
      this->output.values[this->output.point_stride * i] = r.value;
      if (this->statuses != nullptr) {
        this->statuses[i] = r.status.status;
      }
      return getPointStatistics(i, r.status);
    }
    //! \brief evaluation at one point
    Evaluation evaluation;
    //! \brief output
    Field output;
    //! \brief arguments
    std::array<ConstField, N> arguments;
    //! \brief statuses of the points, or null
    int* statuses;
    //! \brief out of bounds policy
    mfront_gmp_OutOfBoundsPolicy out_of_bounds_policy;
  };

  /*!
   * \brief initialize an output status: success, no failure, no unreliable
   * result
   * \param[out] s: output status
   */
  inline void initialize(OutputStatus& s) noexcept {
    s.status = 0;
    s.number_of_failures = 0;
    s.first_failure_index = std::numeric_limits<size_type>::max();
    s.number_of_unreliable_results = 0;
    s.first_unreliable_result_index = std::numeric_limits<size_type>::max();
    s.bounds_status = 0;
    s.msg[0] = '\0';
  }  // end of initialize

  /*!
   * \brief call the body of a generated function (see
   * `mfront::generic_parallel::handleExceptions`). The output status is
   * complete even if an exception is thrown before the batch data are
   * checked.
   * \return the status, `-5` (invalid argument) if no output status is given
   * \param[out] s: output status
   * \param[in] f: function
   */
  template <typename Function>
  int handleExceptions(OutputStatus* const s, Function&& f) noexcept {
    return ::mfront::generic_parallel::handleExceptions(
        s, [](OutputStatus& o) { initialize(o); }, std::forward<Function>(f));
  }  // end of handleExceptions

  /*!
   * \brief check the data of a batch: number of points, output, arguments
   * \return true if the data are valid, otherwise the error is reported in
   * the output status (status `-5`)
   * \param[out] s: output status
   * \param[in] d: batch data
   * \param[in] names: names of the arguments, for the error messages
   * \param[in] runtime_checks: if false, the runtime checks are disabled:
   * as with the `generic` interface, the number of arguments is not checked
   */
  template <size_type N>
  bool check(OutputStatus& s,
             const BatchData& d,
             const std::array<const char*, N>& names,
             const bool runtime_checks) {
    const auto n = d.number_of_points;
    auto error = [&s](const std::string& msg) {
      report(s, -5, msg);
      return false;
    };
    if (n > maximal_number_of_points) {
      return error("too many points (" + std::to_string(n) + " given)");
    }
    if (d.output.values == nullptr) {
      return error("no output values given");
    }
    if ((d.output.point_stride == 0) && (n != 1)) {
      return error(
          "if the output is uniform, the number of points must be equal to "
          "one (" +
          std::to_string(n) + " given)");
    }
    if ((runtime_checks) && (d.number_of_arguments != N)) {
      return error("invalid number of arguments (" +
                   std::to_string(d.number_of_arguments) + " given, " +
                   std::to_string(N) + " expected)");
    }
    if constexpr (N != 0) {
      if (d.arguments == nullptr) {
        return error("no arguments given");
      }
      for (size_type j = 0; j != N; ++j) {
        if (d.arguments[j].values == nullptr) {
          return error(std::string{"no values given for argument '"} +
                       names[j] + "'");
        }
      }
    }
    return true;
  }  // end of check

  /*!
   * \brief evaluate a material property at the points of a batch with the
   * primitive of a backend. The batch data are checked (see `check`), and
   * the statistics are reported by the report function of the material
   * property, unless the calling code gave statistics.
   *
   * \tparam Evaluation: evaluation at one point (see `PointEvaluator`)
   * \tparam N: number of arguments
   * \tparam Apply: callable object such that `apply(s, n, e, h, f)` applies
   * the function object `f` at the `n` points of a batch with the execution
   * `e` (see `BatchExecution`) and reports the statistics in the output
   * status `s` through the handler `h` (see `StatisticsHandler`), and
   * returns the status
   * \param[out] s: output status
   * \param[in] d: batch data
   * \param[in] evaluation: evaluation at one point
   * \param[in] names: names of the arguments, for the error messages
   * \param[in] runtime_checks: if false, the runtime checks are disabled:
   * the number of arguments is not checked, and the statistics are neither
   * combined nor reported, the evaluation at one point never reporting an
   * event
   * \param[in] report_function: report function of the material property
   * \param[in] apply: primitive of the backend
   * \return the status
   */
  template <typename Evaluation, size_type N, typename Apply>
  int evaluateBatch(OutputStatus& s,
                    const BatchData& d,
                    const Evaluation& evaluation,
                    const std::array<const char*, N>& names,
                    const bool runtime_checks,
                    const ReportFunction report_function,
                    const Apply& apply) {
    if (!check(s, d, names, runtime_checks)) {
      return s.status;
    }
    // the fields of the arguments are copied in the function object: the
    // array given by the calling code is generally not accessible by the
    // device
    auto arguments = std::array<ConstField, N>{};
    for (size_type j = 0; j != N; ++j) {
      arguments[j] = d.arguments[j];
    }
    return apply(
        s, d.number_of_points,
        BatchExecution{.policy = d.execution_policy,
                       .statistics = d.statistics},
        StatisticsHandler{.report_function = report_function,
                          .runtime_checks = runtime_checks},
        PointEvaluator<Evaluation, N>{evaluation, d.output, arguments,
                                      d.statuses, d.out_of_bounds_policy});
  }  // end of evaluateBatch

}  // end of namespace mfront::gpmp

#endif /* LIB_MFRONT_GENERICPARALLELMATERIALPROPERTY_EVALUATE_HXX */
