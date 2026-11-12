/*!
 * \file   mfront/include/MFront/GenericParallelMaterialProperty/SYCL.hxx
 * \brief  This file declares the functions of the `sycl` backend of the
 * `generic-parallel` material property interface which are specific to the
 * material properties: transfer of the fields of the batches. The primitive
 * of the backend is declared in `MFront/GenericParallel/SYCL.hxx`.
 * \date   09/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLELMATERIALPROPERTY_SYCL_HXX
#define LIB_MFRONT_GENERICPARALLELMATERIALPROPERTY_SYCL_HXX

#include <array>
#include <sycl/sycl.hpp>
#include "MFront/GenericParallel/SYCL.hxx"
#include "MFront/GenericParallel/SYCLDeviceCopies.hxx"
#include "MFront/GenericParallelMaterialProperty/Evaluate.hxx"

namespace mfront::gpmp::sycl {

  //! \brief policy for the transfer of the fields of a batch
  using mfront::generic_parallel::sycl::DataTransferPolicy;

  /*!
   * \brief evaluate a material property at the points of a batch (see
   * `mfront::gpmp::evaluateBatch`).
   *
   * The execution policy must be a `SyclBackendExecutionPolicy`. With the
   * `NONE` data transfer policy, the fields must be accessible by the device
   * associated with its queue (shared, device or host USM allocations, or
   * any memory if the device is the host). With the `AUTOMATIC` policy, the
   * arguments, the output, the statuses of the points and the statistics
   * which are not accessible are copied in device memory. The output is
   * copied in both directions, so that the values between two values of a
   * strided output are kept. The evaluation is asynchronous if the policy
   * requests it and if no field is copied.
   *
   * The batch, the execution policy and the queue are checked before any
   * transfer, and checked again, at no cost, by the functions called after
   * the transfers.
   *
   * \param[in] transfers: policy for the transfer of the fields
   * \param[in] apply: primitive of the backend (see
   * `mfront::generic_parallel::sycl::makeApply`)
   */
  template <typename Evaluation, size_type N, typename Apply>
  int evaluateBatch(OutputStatus& s,
                    const BatchData& d,
                    const Evaluation& evaluation,
                    const std::array<const char*, N>& names,
                    const bool runtime_checks,
                    const ReportFunction report_function,
                    const DataTransferPolicy transfers,
                    const Apply& apply) {
    using ::mfront::generic_parallel::sycl::DeviceCopies;
    using ::mfront::generic_parallel::sycl::StridedArray;
    // the batch is checked before any transfer
    if (!mfront::gpmp::check(s, d, names, runtime_checks)) {
      return s.status;
    }
    const auto p = ::mfront::generic_parallel::sycl::checkExecutionPolicy(
        d.execution_policy, d.statistics != nullptr);
    if (p.error != nullptr) {
      return report(s, -5, p.error);
    }
    if (const auto* const error =
            ::mfront::generic_parallel::sycl::checkQueue(p);
        error != nullptr) {
      return report(s, -5, error);
    }
    auto q = p.policy->queue;
    // with an out-of-order queue, the computations already submitted, which
    // may compute the fields, must be completed before any transfer
    if (!q.is_in_order()) {
      q.wait_and_throw();
    }
    auto copies = DeviceCopies(q, d.number_of_points, transfers);
    auto dd = d;
    // fields of the arguments
    auto arguments = std::array<ConstField, N>{};
    for (size_type j = 0; j != N; ++j) {
      auto a = StridedArray<const real>{d.arguments[j].values,
                                        d.arguments[j].point_stride, 1};
      copies.transfer(a, 1, false);
      arguments[j] = ConstField{a.values, a.point_stride};
    }
    dd.arguments = arguments.data();
    auto output = StridedArray<real>{d.output.values, d.output.point_stride, 1};
    copies.transfer(output, 1, true);
    dd.output.values = output.values;
    copies.transfer(dd.statuses, d.number_of_points, true);
    if (runtime_checks) {
      // the statistics are not accessed if the runtime checks are disabled
      copies.transfer(dd.statistics, 1, true);
    }
    const auto r = mfront::gpmp::evaluateBatch(
        s, dd, evaluation, names, runtime_checks, report_function, apply);
    copies.copyOutputs();
    return r;
  }  // end of evaluateBatch

}  // end of namespace mfront::gpmp::sycl

#endif /* LIB_MFRONT_GENERICPARALLELMATERIALPROPERTY_SYCL_HXX */
