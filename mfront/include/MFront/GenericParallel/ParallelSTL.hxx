/*!
 * \file   mfront/include/MFront/GenericParallel/ParallelSTL.hxx
 * \brief  This file declares the primitive of the `stlpar` backend of the
 * `generic-parallel` interfaces, based on the parallel algorithms of the C++
 * standard library.
 * \date   05/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_PARALLELSTL_HXX
#define LIB_MFRONT_GENERICPARALLEL_PARALLELSTL_HXX

#include <cstddef>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <execution>
#include "MFront/GenericParallel/CountingIterator.hxx"
#include "MFront/GenericParallel/DeviceStackSize.hxx"
#include "MFront/GenericParallel/ParallelSTLBackendExecutionPolicy.hxx"
#include "MFront/GenericParallel/Batch.hxx"

namespace mfront::generic_parallel::stlpar {

  //! \brief a simple alias
  using CountingIterator = generic_parallel::CountingIterator<size_type>;

  static_assert(std::random_access_iterator<CountingIterator>);

  /*!
   * \brief apply a function at the points of a batch and report the
   * statistics of the batch. This function waits for the completion of the
   * computations. The statistics given by the calling code, if any,
   * are accumulated on the host. If the function never reports an event
   * (see the `hasEvents` method of the handler), the statistics are neither
   * combined nor reported.
   *
   * \tparam device_stack_size: minimal stack size, in bytes, of the threads
   * of the device when the parallel algorithms are offloaded to an NVIDIA
   * GPU by `nvc++ -stdpar=gpu` (see
   * `mfront::generic_parallel::setMinimalCUDAStackSize`). The stack size is
   * not modified if this value is null. This value is a template argument
   * so that the code setting the stack size, which looks for the CUDA
   * runtime with the functions of the dynamic loader, is only compiled if
   * required.
   * \tparam StatisticsHandler: handler of the statistics (see
   * `MFront/GenericParallel/Batch.hxx`)
   * \tparam PointFunction: function object taking the index of a point and
   * returning the statistics associated with this point
   * \tparam ExecutionPolicy: execution policy
   * \param[in,out] s: output status, which must have been initialized
   * \param[in] n: number of points
   * \param[in] e: execution of the batch. The policy is a
   * `ParallelSTLBackendExecutionPolicy`, or null for the default policy: only
   * its common options are used, the computations being synchronous.
   * \param[in] h: handler of the statistics
   * \param[in] f: function
   * \param[in] policy: execution policy
   * \return the status
   */
  template <std::size_t device_stack_size,
            typename StatisticsHandler,
            typename PointFunction,
            typename ExecutionPolicy>
  int apply(typename StatisticsHandler::OutputStatus& s,
            const size_type n,
            const BatchExecution<typename StatisticsHandler::Statistics>& e,
            const StatisticsHandler& h,
            const PointFunction& f,
            ExecutionPolicy&& policy) {
    if (n == 0) {
      return s.status;
    }
    const auto p =
        checkBackendExecutionPolicy<ParallelSTLBackendExecutionPolicy>(
            e.policy, e.statistics != nullptr);
    if (p.error != nullptr) {
      return report(s, -5, p.error);
    }
#if defined(__NVCOMPILER_STDPAR_GPU)
    if constexpr (device_stack_size != 0) {
      // the stack size is only set when the parallel algorithms are
      // offloaded to a GPU: the CUDA runtime may be loaded by another
      // component of a process whose computations run on the host
      if (!setMinimalCUDAStackSize(device_stack_size,
                                   p.common_options.preserve_stack_size)) {
        return report(s, -7, stack_size_error_message);
      }
    }
#endif /* defined(__NVCOMPILER_STDPAR_GPU) */
    if (!h.hasEvents()) {
      // the statistics are neither combined nor reported. The function is
      // captured by value: the stack of the host may not be accessible by
      // the device (`nvc++ -stdpar=gpu`)
      std::for_each(policy, CountingIterator{0}, CountingIterator{n},
                    [f](const size_type i) { static_cast<void>(f(i)); });
      return s.status;
    }
    const auto r =
        std::transform_reduce(policy, CountingIterator{0}, CountingIterator{n},
                              h.initial(), CombineStatistics{}, f);
    return h.report(s, e.statistics, r);
  }  // end of apply

  /*!
   * \return the primitive of the backend, called by the functions of the
   * interfaces (see for instance `mfront::gpmp::evaluateBatch`)
   * \tparam device_stack_size: minimal stack size of the threads of the
   * device (see `apply`)
   * \param[in] policy: execution policy
   */
  template <std::size_t device_stack_size = 0, typename ExecutionPolicy>
  auto makeApply(const ExecutionPolicy policy) {
    return [policy](auto& s, const size_type n, const auto& e, const auto& h,
                    const auto& f) {
      return stlpar::apply<device_stack_size>(s, n, e, h, f, policy);
    };
  }  // end of makeApply

}  // end of namespace mfront::generic_parallel::stlpar

#endif /* LIB_MFRONT_GENERICPARALLEL_PARALLELSTL_HXX */
