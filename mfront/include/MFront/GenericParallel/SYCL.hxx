/*!
 * \file   mfront/include/MFront/GenericParallel/SYCL.hxx
 * \brief  This file declares the primitive of the `sycl` backend of the
 * `generic-parallel` interfaces.
 * \date   05/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_SYCL_HXX
#define LIB_MFRONT_GENERICPARALLEL_SYCL_HXX

#include <new>
#include <cstddef>
#include <utility>
#include <cstdlib>
#include <sycl/sycl.hpp>
#include "MFront/GenericParallel/Batch.hxx"
#include "MFront/GenericParallel/DeviceStackSize.hxx"
#include "MFront/GenericParallel/StatusBlockPool.hxx"
#include "MFront/GenericParallel/SyclBackendExecutionPolicy.hxx"

namespace mfront::generic_parallel::sycl {

  namespace internals {

    //! \brief atomic operations of the statistics
    struct Atomics {
      //! \brief a simple alias
      using AtomicCounter =
          ::sycl::atomic_ref<atomic_size_type,
                             ::sycl::memory_order::relaxed,
                             ::sycl::memory_scope::device,
                             ::sycl::access::address_space::global_space>;
      //! \brief add a value to a counter
      static void add(atomic_size_type* const p, const atomic_size_type v) {
        AtomicCounter{*p}.fetch_add(v);
      }
      //! \brief replace a counter by the minimum of its value and a value
      static void min(atomic_size_type* const p, const atomic_size_type v) {
        AtomicCounter{*p}.fetch_min(v);
      }
      /*!
       * \brief set a flag (see `signalEvents`), by an addition: contrary to
       * the exchanges, the additions of the threads of a warp of an NVIDIA
       * device are merged in a single atomic operation by its compiler
       * \return true if the flag was already set
       */
      static bool testAndSet(atomic_size_type* const p) {
        return AtomicCounter{*p}.fetch_add(1) != 0;
      }
    };

    /*!
     * \brief kernel applying a function at one point and updating the
     * statistics of the batch.
     *
     * The statistics are updated by atomic operations, which are only
     * performed if an event occurs at this point (see `updateStatistics`).
     * Parallel reductions are not used since their synchronisation barriers
     * make the compilation of large kernels fail with AdaptiveCpp on CPUs.
     *
     * \tparam Statistics: statistics updated by the devices
     * \tparam PointFunction: function object applied at one point
     */
    template <typename Statistics, typename PointFunction>
    struct Kernel {
      //! \brief apply the function at the `i`-th point
      void operator()(const ::sycl::id<1> i) const {
        const auto r = this->f(i[0]);
        if (this->statistics != nullptr) {
          updateStatistics<Atomics>(*(this->statistics), r, this->rdt0,
                                    this->signalled, this->events);
        }
      }
      //! \brief function object applied at one point
      PointFunction f;
      //! \brief statistics, accessible by the device, or null if the
      //! function never reports an event
      Statistics* statistics;
      //! \brief flag set by the threads which signal an event, if the
      //! statistics are the ones of a block (see `StatisticsBlock`)
      atomic_size_type* signalled;
      //! \brief flag set by the first thread which signals an event (see
      //! `signalEvents`), or null
      int* events;
      //! \brief ordered key of the initial time step scaling factor
      atomic_size_type rdt0;
    };

    /*!
     * \return a block of statistics (see `StatisticsBlock`), the statistics
     * being allocated in the memory of the device of the queue, or a null
     * pointer if an allocation failed
     * \tparam Statistics: statistics updated by the devices
     * \param[in] q: queue
     * \param[in] shared: if true, the block is allocated in host memory
     * accessible by the device (host allocation in the sense of USM)
     */
    template <typename Statistics>
    StatisticsBlock<Statistics>* allocateStatisticsBlock(
        const ::sycl::queue& q, const bool shared) noexcept {
      using Block = StatisticsBlock<Statistics>;
      try {
        auto* const device =
            ::sycl::malloc_device<DeviceStatistics<Statistics>>(1, q);
        if (device == nullptr) {
          return nullptr;
        }
        auto* const block =
            shared ? ::sycl::malloc_host<Block>(1, q)
                   : static_cast<Block*>(std::malloc(sizeof(Block)));
        if (block == nullptr) {
          ::sycl::free(device, q);
          return nullptr;
        }
        return ::new (block) Block{
            .device = device, .neutral = false, .events = 0, .statistics = {}};
      } catch (...) {
      }
      return nullptr;
    }  // end of allocateStatisticsBlock

    /*!
     * \return the pool of blocks holding the statistics of the synchronous
     * computations of a function of the generated library (see
     * `StatisticsBlock`). The blocks are identified by the device and the
     * context of the queue.
     * \tparam Statistics: statistics updated by the devices
     * \tparam PointFunction: function applied at the points, which is
     * specific to a generated function, so that each generated library has
     * its own pool (see `mfront::generic_parallel::gpu::internals`)
     */
    template <typename Statistics, typename PointFunction>
    auto& getStatisticsBlocks() noexcept {
      using Pool = StatusBlockPool<StatisticsBlock<Statistics>,
                                   std::pair<::sycl::device, ::sycl::context>>;
      static auto pool = Pool{};
      return pool;
    }  // end of getStatisticsBlocks

  }  // end of namespace internals

  /*!
   * \return the policy given by the calling code, checked against the
   * policy of the backend (see
   * `mfront::generic_parallel::checkBackendExecutionPolicy`). The `sycl`
   * backend has no default policy: a null policy is refused.
   * \param[in] p: policy given by the calling code
   * \param[in] has_statistics: true if the calling code gave statistics
   */
  inline CheckedBackendExecutionPolicy<SyclBackendExecutionPolicy>
  checkExecutionPolicy(const mfront_gp_AbstractBackendExecutionPolicy* const p,
                       const bool has_statistics) noexcept {
    auto r = checkBackendExecutionPolicy<SyclBackendExecutionPolicy>(
        p, has_statistics);
    if ((r.error == nullptr) && (r.policy == nullptr)) {
      r.error = "no execution policy given: the sycl backend requires a queue";
    }
    return r;
  }  // end of checkExecutionPolicy

  /*!
   * \return an error message if the computations can't be submitted to the
   * queue of the given policy, a null pointer otherwise
   *
   * The computations must be executed after the computations already
   * submitted to the queue, which may compute the inputs. This is guaranteed
   * by an in-order queue. With an out-of-order queue, the computations must
   * be synchronous, and the queue is synchronized before them (see `apply`).
   *
   * \param[in] p: policy given by the calling code, checked by
   * `checkExecutionPolicy`
   */
  inline const char* checkQueue(
      const CheckedBackendExecutionPolicy<SyclBackendExecutionPolicy>& p) {
    if ((!p.policy->queue.is_in_order()) && (p.common_options.asynchronous)) {
      return "asynchronous evaluations require an in-order SYCL queue";
    }
    return nullptr;
  }  // end of checkQueue

  /*!
   * \brief apply a function at the points of a batch and report the
   * statistics of the batch.
   *
   * If the computations are asynchronous, this function returns once they
   * are submitted to the queue. Otherwise, it waits for their completion.
   *
   * Without statistics given by the calling code, the statistics are held by
   * a block of a pool (see `StatisticsBlock` and `StatusBlockPool`): no
   * memory is allocated per call. The device only accesses the statistics,
   * in device memory, if an event occurs at a point, and then signals it to
   * the host: a call without event submits a single command, the kernel.
   * The statistics are only copied back after an event, and reset by the
   * next call using the block. A device which can't access the memory of
   * the host (no host allocation in the sense of USM) can't signal events:
   * the statistics are then reset and copied back at each call.
   *
   * If the function never reports an event (see the `hasEvents` method of
   * the handler), the statistics are neither combined nor reported, and no
   * block of the pool is used.
   *
   * \tparam device_stack_size: minimal stack size, in bytes, of the threads
   * of the device when the computations are performed on an NVIDIA GPU
   * through the CUDA runtime (see `setMinimalCUDAStackSize`). The stack size
   * is not modified if this value is null. This value is a template argument
   * so that the code setting the stack size, which looks for the CUDA
   * runtime with the functions of the dynamic loader, is only compiled if
   * required.
   * \tparam StatisticsHandler: handler of the statistics (see
   * `MFront/GenericParallel/Batch.hxx`)
   * \tparam PointFunction: function object taking the index of a point and
   * returning the statistics associated with this point
   * \param[in,out] s: output status, which must have been initialized
   * \param[in] n: number of points
   * \param[in] e: execution of the batch. The policy must be a
   * `SyclBackendExecutionPolicy`.
   * \param[in] h: handler of the statistics
   * \param[in] f: function
   * \return the status
   */
  template <std::size_t device_stack_size,
            typename StatisticsHandler,
            typename PointFunction>
  int apply(typename StatisticsHandler::OutputStatus& s,
            const size_type n,
            const BatchExecution<typename StatisticsHandler::Statistics>& e,
            const StatisticsHandler& h,
            const PointFunction& f) {
    using Statistics = typename StatisticsHandler::Statistics;
    using Kernel = internals::Kernel<Statistics, PointFunction>;
    if (n == 0) {
      return s.status;
    }
    const auto p = checkExecutionPolicy(e.policy, e.statistics != nullptr);
    if (p.error != nullptr) {
      return report(s, -5, p.error);
    }
    if (const auto* const error = checkQueue(p); error != nullptr) {
      return report(s, -5, error);
    }
    auto q = p.policy->queue;
    if (!q.is_in_order()) {
      q.wait_and_throw();
    }
    if constexpr (device_stack_size != 0) {
      // the stack size is only set when the queue runs on a GPU: the CUDA
      // runtime may be loaded by another component of a process whose
      // computations run on the host
      if ((q.get_device().is_gpu()) &&
          (!setMinimalCUDAStackSize(device_stack_size,
                                    p.common_options.preserve_stack_size))) {
        return report(s, -7, stack_size_error_message);
      }
    }
    const auto range = ::sycl::range<1>{n};
    const auto rdt0 = toOrderedKey(h.initial().rdt);
    if ((e.statistics != nullptr) || (!h.hasEvents())) {
      // statistics given by the calling code: the result is given by the
      // report function of the library. Without event, the statistics, if
      // any, are not accessed by the kernel
      auto* const statistics = h.hasEvents() ? e.statistics : nullptr;
      auto event =
          q.parallel_for(range, Kernel{f, statistics, nullptr, nullptr, rdt0});
      if (!p.common_options.asynchronous) {
        event.wait_and_throw();
      }
      return s.status;
    }
    // synchronous computations: block of statistics of the pool (see
    // `StatisticsBlock`). A device which can't access the memory of the host
    // can't signal events: the flag of the block is then left set
    const auto shared =
        q.get_device().has(::sycl::aspect::usm_host_allocations);
    const auto block =
        internals::getStatisticsBlocks<Statistics, PointFunction>().acquire(
            {q.get_device(), q.get_context()}, [&q, shared] {
              return internals::allocateStatisticsBlock<Statistics>(q, shared);
            });
    auto* const b = block.get();
    if (b == nullptr) {
      return report(s, -7,
                    "unable to allocate the statistics of the evaluation");
    }
    auto statistics = Statistics{};
    h.initialize(statistics);
    if (!b->neutral) {
      // reset, after an event or at the first use
      const auto neutral = getNeutralStatistics(statistics);
      q.copy(&neutral, b->device, 1).wait_and_throw();
    }
    b->neutral = false;
    b->events = shared ? 0 : 1;
    q.parallel_for(range,
                   Kernel{f, &(b->device->statistics), &(b->device->signalled),
                          shared ? &(b->events) : nullptr, rdt0})
        .wait_and_throw();
    if (b->events == 0) {
      b->neutral = true;
      return h.report(s, statistics);
    }
    q.copy(&(b->device->statistics), &(b->statistics), 1).wait_and_throw();
    return h.report(s, mergeBlockStatistics(statistics, b->statistics));
  }  // end of apply

  /*!
   * \return the primitive of the backend, called by the functions of the
   * interfaces (see for instance `mfront::gpmp::evaluateBatch`)
   * \tparam device_stack_size: minimal stack size of the threads of the
   * device (see `apply`)
   */
  template <std::size_t device_stack_size = 0>
  auto makeApply() {
    return [](auto& s, const size_type n, const auto& e, const auto& h,
              const auto& f) {
      return sycl::apply<device_stack_size>(s, n, e, h, f);
    };
  }  // end of makeApply

}  // end of namespace mfront::generic_parallel::sycl

#endif /* LIB_MFRONT_GENERICPARALLEL_SYCL_HXX */
