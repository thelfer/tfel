/*!
 * \file   mfront/include/MFront/GenericParallel/GPU.hxx
 * \brief  This file declares the primitive of the `cuda` and `hip` backends
 * of the `generic-parallel` interfaces. This file must be compiled by a CUDA
 * or a HIP compiler. The HIP runtime is used if the `hip/hip_runtime.h`
 * header has been included before this file or if the HIP language is
 * enabled, the CUDA runtime otherwise. Including the HIP runtime header
 * first is required to use HIP on NVIDIA GPUs, since `hipcc` then calls
 * `nvcc`.
 * \date   05/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_GPU_HXX
#define LIB_MFRONT_GENERICPARALLEL_GPU_HXX

#include <limits>
#include <string>
#include <cstdint>
#include "MFront/GenericParallel/GPUStackSize.hxx"
#include "MFront/GenericParallel/Batch.hxx"
#include "MFront/GenericParallel/StatusBlockPool.hxx"
#if defined(__HIPCC__) || defined(__HIP__)
#include "MFront/GenericParallel/HIPBackendExecutionPolicy.hxx"
#else
#include "MFront/GenericParallel/CUDABackendExecutionPolicy.hxx"
#endif

namespace mfront::generic_parallel::gpu {

  namespace internals {

#if defined(__HIPCC__) || defined(__HIP__)
    //! \brief device attribute giving the warp size
    inline constexpr auto warp_size_attribute = hipDeviceAttributeWarpSize;
    //! \brief execution policy of the backend
    using ExecutionPolicy = HIPBackendExecutionPolicy;
#else
    //! \brief device attribute giving the warp size
    inline constexpr auto warp_size_attribute = cudaDevAttrWarpSize;
    //! \brief execution policy of the backend
    using ExecutionPolicy = CUDABackendExecutionPolicy;
#endif

    /*!
     * \brief report an error of the runtime. Only the first error is
     * reported.
     * \return true if the given code denotes an error
     * \param[in,out] s: output status
     * \param[in] e: error code
     */
    template <typename OutputStatus>
    bool failed(OutputStatus& s, const Error e) noexcept {
      if (e == MFRONT_GENERIC_PARALLEL_GPU_API(Success)) {
        return false;
      }
      if (s.status != -7) {
        report(s, -7, MFRONT_GENERIC_PARALLEL_GPU_API(GetErrorString)(e));
      }
      return true;
    }  // end of failed

    /*!
     * \brief increase the stack size of the threads if the given kernel
     * requires it (see `mfront::generic_parallel::gpu::ensureStackSize`).
     * \return true on success
     * \tparam kernel: kernel
     * \param[in,out] s: output status
     * \param[in] preserve: if true, the stack size is not modified and an
     * error is reported if it is too small
     */
    template <auto kernel, typename OutputStatus>
    bool ensureStackSize(OutputStatus& s, const bool preserve) {
      const auto r = gpu::ensureStackSize<kernel>(preserve);
      if (failed(s, r.error)) {
        return false;
      }
      if (r.required_stack_size != 0) {
        report(s, -7, r.getErrorMessage());
        return false;
      }
      return true;
    }  // end of ensureStackSize

    //! \brief sum of the values of a warp, in the first thread of the warp
    template <typename T>
    __device__ inline T warpSum(T v) {
      for (int o = warpSize / 2; o > 0; o /= 2) {
#if defined(__HIP_PLATFORM_AMD__)
        v += __shfl_down(v, o);
#else
        v += __shfl_down_sync(0xffffffffu, v, o);
#endif
      }
      return v;
    }  // end of warpSum

    //! \return true if the predicate is true for a thread of the warp
    __device__ inline bool warpAny(const bool p) {
#if defined(__HIP_PLATFORM_AMD__)
      return __any(p);
#else
      return __any_sync(0xffffffffu, p);
#endif
    }  // end of warpAny

    //! \brief minimum of the values of a warp, in the first thread of the warp
    __device__ inline atomic_size_type warpMin(atomic_size_type v) {
      for (int o = warpSize / 2; o > 0; o /= 2) {
#if defined(__HIP_PLATFORM_AMD__)
        const auto w = __shfl_down(v, o);
#else
        const auto w = __shfl_down_sync(0xffffffffu, v, o);
#endif
        v = w < v ? w : v;
      }
      return v;
    }  // end of warpMin

    /*!
     * \brief apply a function at the points of a batch, one thread per
     * point, and update the statistics of the batch. The number of threads
     * per block must be a multiple of the warp size.
     *
     * \param[in] f: function
     * \param[in] n: number of points
     * \param[out] s: statistics, or null if the function never reports an
     * event (see the `hasEvents` method of the handlers of the statistics)
     * \param[in] rdt0: ordered key of the initial time step scaling factor
     */
    template <typename Statistics, typename PointFunction>
    __global__ void apply(const PointFunction f,
                          const size_type n,
                          Statistics* const s,
                          const atomic_size_type rdt0) {
      const auto i = static_cast<size_type>(blockIdx.x) * blockDim.x +
                     static_cast<size_type>(threadIdx.x);
      auto failures = 0u;
      auto first_failure = no_event;
      auto unreliable_results = 0u;
      auto first_unreliable_result = no_event;
      auto rdt = rdt0;
      if (i < n) {
        const auto r = f(i);
        failures = static_cast<unsigned int>(r.number_of_failures);
        first_failure = r.first_failure;
        unreliable_results =
            static_cast<unsigned int>(r.number_of_unreliable_results);
        first_unreliable_result = r.first_unreliable_result;
        rdt = toOrderedKey(r.rdt);
      }
      // the statistics are only reduced and updated if a point of the warp
      // failed, gave an unreliable result or decreased the time step
      // scaling factor: otherwise, the statistics are not accessed
      if (!warpAny((failures != 0) || (unreliable_results != 0) ||
                   (rdt < rdt0))) {
        return;
      }
      failures = warpSum(failures);
      first_failure = warpMin(first_failure);
      unreliable_results = warpSum(unreliable_results);
      first_unreliable_result = warpMin(first_unreliable_result);
      rdt = warpMin(rdt);
      if (threadIdx.x % warpSize == 0) {
        if (failures != 0) {
          atomicAdd(&(s->number_of_failures),
                    static_cast<atomic_size_type>(failures));
          atomicMin(&(s->first_failure), first_failure);
        }
        if (unreliable_results != 0) {
          atomicAdd(&(s->number_of_unreliable_results),
                    static_cast<atomic_size_type>(unreliable_results));
          atomicMin(&(s->first_unreliable_result), first_unreliable_result);
        }
        if constexpr (hasTimeStepScalingFactor<Statistics>) {
          if (rdt < rdt0) {
            atomicMin(&(s->rdt), rdt);
          }
        }
      }
    }  // end of apply

    /*!
     * \return the pool of blocks of device memory holding the statistics of
     * the synchronous computations of a function of the generated library
     * \tparam Statistics: statistics updated by the devices
     * \tparam PointFunction: function applied at the points, which is
     * specific to a generated function
     *
     * The pool depends on the function applied at the points, whose type is
     * declared in an anonymous namespace by the generated code: each
     * generated function, and thus each generated library, has its own pool.
     * A pool shared by several libraries through the dynamic linker would be
     * destroyed when the library which initialized it is unloaded, even if
     * another library still uses it.
     */
    template <typename Statistics, typename PointFunction>
    auto& getStatisticsBlocks() noexcept {
      using Pool = StatusBlockPool<Statistics, int>;
      static auto pool = Pool{};
      return pool;
    }  // end of getStatisticsBlocks

  }  // end of namespace internals

  /*!
   * \brief apply a function at the points of a batch, one thread per point,
   * and report the statistics of the batch.
   *
   * The kernel is launched on the stream given by the execution policy. If
   * the computations are asynchronous, this function returns once the
   * kernel is launched. Otherwise, it waits for its completion.
   *
   * Without statistics given by the calling code, the statistics are held by
   * a block of device memory of a pool of blocks (see `StatusBlockPool`): no
   * memory is allocated per call. Local statistics are initialized on the
   * host, copied to the block just before the launch and copied back once
   * the computations are completed: the host never accesses the block,
   * which the device may not share with the host while kernels run (devices
   * without concurrent access to managed memory). The device only accesses
   * the block if an event occurs at a point.
   *
   * If the function never reports an event (see the `hasEvents` method of
   * the handler), the statistics are neither combined nor reported, and no
   * block of the pool is used.
   *
   * \tparam StatisticsHandler: handler of the statistics (see
   * `MFront/GenericParallel/Batch.hxx`)
   * \tparam PointFunction: function object taking the index of a point and
   * returning the statistics associated with this point
   * \param[in,out] s: output status, which must have been initialized
   * \param[in] n: number of points
   * \param[in] e: execution of the batch. The policy is a
   * `CUDABackendExecutionPolicy` (`HIPBackendExecutionPolicy` with HIP), or
   * null for the default stream.
   * \param[in] h: handler of the statistics
   * \param[in] f: function
   * \param[in] threads_per_block: number of threads per block, rounded up to
   * a multiple of the warp size
   * \return the status
   */
  template <typename StatisticsHandler, typename PointFunction>
  int apply(typename StatisticsHandler::OutputStatus& s,
            const size_type n,
            const BatchExecution<typename StatisticsHandler::Statistics>& e,
            const StatisticsHandler& h,
            const PointFunction& f,
            const unsigned int threads_per_block) {
    using Statistics = typename StatisticsHandler::Statistics;
    using Stream = MFRONT_GENERIC_PARALLEL_GPU_API(Stream_t);
    using internals::failed;
    if (n == 0) {
      return s.status;
    }
    const auto p = checkBackendExecutionPolicy<internals::ExecutionPolicy>(
        e.policy, e.statistics != nullptr);
    if (p.error != nullptr) {
      return report(s, -5, p.error);
    }
    auto device = 0;
    auto warp_size = 0;
    if (failed(s, MFRONT_GENERIC_PARALLEL_GPU_API(GetDevice)(&device))) {
      return s.status;
    }
    if (MFRONT_GENERIC_PARALLEL_GPU_API(DeviceGetAttribute)(
            &warp_size, internals::warp_size_attribute, device) !=
        MFRONT_GENERIC_PARALLEL_GPU_API(Success)) {
      return report(s, -7, "unable to get the warp size of the device");
    }
    const auto ws = static_cast<unsigned int>(warp_size);
    const auto nthreads = ((threads_per_block + ws - 1) / ws) * ws;
    const auto nblocks = (n + nthreads - 1) / nthreads;
    // the number of blocks must fit in the grid of the device
    if (nblocks >
        static_cast<decltype(nblocks)>(std::numeric_limits<int>::max())) {
      return report(s, -5,
                    "too many points for the grid of the device (" +
                        std::to_string(n) + " given)");
    }
#if defined(__HIP_PLATFORM_AMD__)
    // the AMD devices also limit the number of work-items of a launch to
    // 2^32 - 1
    if (nblocks * nthreads > std::numeric_limits<std::uint32_t>::max()) {
      return report(s, -5,
                    "too many points for the grid of the device (" +
                        std::to_string(n) + " given)");
    }
#endif /* defined(__HIP_PLATFORM_AMD__) */
    constexpr auto k = internals::apply<Statistics, PointFunction>;
    if (!internals::ensureStackSize<k>(s,
                                       p.common_options.preserve_stack_size)) {
      return s.status;
    }
    const auto stream = p.policy == nullptr ? Stream{} : p.policy->stream;
    const auto rdt0 = toOrderedKey(h.initial().rdt);
    // launch the kernel and return the error of the launch
    auto launch = [&k, &f, n, nblocks, nthreads, stream,
                   rdt0](Statistics* const statistics) {
      // an error left by a previous call of the calling code must not be
      // taken for an error of the launch
      static_cast<void>(MFRONT_GENERIC_PARALLEL_GPU_API(GetLastError)());
      k<<<static_cast<unsigned int>(nblocks), nthreads, 0, stream>>>(
          f, n, statistics, rdt0);
      return MFRONT_GENERIC_PARALLEL_GPU_API(GetLastError)();
    };
    if ((e.statistics != nullptr) || (!h.hasEvents())) {
      // statistics given by the calling code: the result is given by the
      // report function of the library. Without event, the statistics, if
      // any, are not accessed by the kernel
      if (failed(s, launch(e.statistics)) || p.common_options.asynchronous) {
        return s.status;
      }
      failed(s, MFRONT_GENERIC_PARALLEL_GPU_API(StreamSynchronize)(stream));
      return s.status;
    }
    // synchronous computations: the statistics are held by a block of the
    // pool, initialized after the checks: a copy still in progress when the
    // block is given back to the pool would overwrite the statistics of the
    // next call using it
    const auto block =
        internals::getStatisticsBlocks<Statistics, PointFunction>().acquire(
            device, [] {
              Statistics* ptr = nullptr;
              const auto r = MFRONT_GENERIC_PARALLEL_GPU_API(Malloc)(
                  reinterpret_cast<void**>(&ptr), sizeof(Statistics));
              return r == MFRONT_GENERIC_PARALLEL_GPU_API(Success) ? ptr
                                                                   : nullptr;
            });
    auto* const statistics = block.get();
    if (statistics == nullptr) {
      return report(s, -7,
                    "unable to allocate the statistics of the evaluation");
    }
    auto host_statistics = Statistics{};
    h.initialize(host_statistics);
    if (MFRONT_GENERIC_PARALLEL_GPU_API(MemcpyAsync)(
            statistics, &host_statistics, sizeof(Statistics),
            MFRONT_GENERIC_PARALLEL_GPU_API(MemcpyHostToDevice),
            stream) != MFRONT_GENERIC_PARALLEL_GPU_API(Success)) {
      return report(s, -7,
                    "unable to initialize the statistics of the evaluation");
    }
    if (const auto le = launch(statistics);
        le != MFRONT_GENERIC_PARALLEL_GPU_API(Success)) {
      // the copy of the statistics must be completed before the block is
      // given back to the pool
      static_cast<void>(
          MFRONT_GENERIC_PARALLEL_GPU_API(StreamSynchronize)(stream));
      failed(s, le);
      return s.status;
    }
    if (failed(s, MFRONT_GENERIC_PARALLEL_GPU_API(StreamSynchronize)(stream))) {
      return s.status;
    }
    if ((MFRONT_GENERIC_PARALLEL_GPU_API(MemcpyAsync)(
             &host_statistics, statistics, sizeof(Statistics),
             MFRONT_GENERIC_PARALLEL_GPU_API(MemcpyDeviceToHost),
             stream) != MFRONT_GENERIC_PARALLEL_GPU_API(Success)) ||
        (MFRONT_GENERIC_PARALLEL_GPU_API(StreamSynchronize)(stream) !=
         MFRONT_GENERIC_PARALLEL_GPU_API(Success))) {
      return report(s, -7, "unable to copy the statistics of the evaluation");
    }
    return h.report(s, host_statistics);
  }  // end of apply

  /*!
   * \return the primitive of the backend, called by the functions of the
   * interfaces (see for instance `mfront::gpmp::evaluateBatch`)
   * \param[in] threads_per_block: number of threads per block
   */
  inline auto makeApply(const unsigned int threads_per_block) {
    return [threads_per_block](auto& s, const size_type n, const auto& e,
                               const auto& h, const auto& f) {
      return gpu::apply(s, n, e, h, f, threads_per_block);
    };
  }  // end of makeApply

}  // end of namespace mfront::generic_parallel::gpu

#endif /* LIB_MFRONT_GENERICPARALLEL_GPU_HXX */
