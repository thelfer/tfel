/*!
 * \file   test-suite/material-properties/generic-parallel/runtime.hxx
 * \brief  This file selects the runtime of the tests of the
 * `generic-parallel` interface for material properties (memory holding the
 * data, synchronization, execution policy), shared by the comparison to the
 * `generic` interface (`compare.cxx`) and by the test of the runtime checks
 * (`test-no-runtime-checks.cxx`).
 *
 * The data are allocated in managed memory if this file is compiled by a
 * CUDA compiler or by a HIP compiler. The HIP runtime is used if the HIP
 * language is enabled or if the `MFRONT_GPMP_COMPARE_USE_HIP` macro is
 * defined (required on NVIDIA GPUs). If the `MFRONT_GPMP_COMPARE_USE_SYCL`
 * macro is defined, the data are allocated in shared USM memory (or in host
 * memory if the `MFRONT_GPMP_COMPARE_USE_HOST_MEMORY` macro is defined) and
 * the computations are performed on the device selected by the default SYCL
 * selector. Otherwise, the data are allocated in host memory.
 *
 * \date   20/10/2026
 */

#ifndef LIB_MFRONT_GENERICPARALLELMATERIALPROPERTY_TESTS_RUNTIME_HXX
#define LIB_MFRONT_GENERICPARALLELMATERIALPROPERTY_TESTS_RUNTIME_HXX

#include <dlfcn.h>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <algorithm>
#if defined(MFRONT_GPMP_COMPARE_USE_SYCL)
#include <sycl/sycl.hpp>
#include "MFront/GenericParallel/SyclBackendExecutionPolicy.hxx"
#elif defined(__HIP__) || defined(MFRONT_GPMP_COMPARE_USE_HIP)
#include <hip/hip_runtime.h>
#include "MFront/GenericParallel/HIPBackendExecutionPolicy.hxx"
#define GPU_API(f) hip##f
#elif defined(__CUDACC__)
#include <cuda_runtime.h>
#include "MFront/GenericParallel/CUDABackendExecutionPolicy.hxx"
#define GPU_API(f) cuda##f
#endif
#include "MFront/GenericParallel/BackendExecutionPolicy.h"
#include "MFront/GenericParallel/ParallelSTLBackendExecutionPolicy.hxx"

[[noreturn]] static void fail(const std::string& msg) {
  std::fprintf(stderr, "%s\n", msg.c_str());
  std::exit(EXIT_FAILURE);
}

#if defined(MFRONT_GPMP_COMPARE_USE_SYCL)

//! \brief nothing to initialize
struct Runtime {};

/*!
 * \return the SYCL queue used by all the computations. The asynchronous
 * computations require an in-order queue.
 */
static ::sycl::queue& getQueue() {
  static auto q = ::sycl::queue{::sycl::property::queue::in_order{}};
  return q;
}

#if defined(MFRONT_GPMP_COMPARE_USE_HOST_MEMORY)

template <typename T>
static T* allocate(const std::size_t n) {
  auto* const p =
      static_cast<T*>(std::malloc(std::max<std::size_t>(n, 1) * sizeof(T)));
  if (p == nullptr) {
    fail("allocation failed");
  }
  return p;
}

static void deallocate(void* const p) { std::free(p); }

#else /* defined(MFRONT_GPMP_COMPARE_USE_HOST_MEMORY) */

template <typename T>
static T* allocate(const std::size_t n) {
  auto* const p =
      ::sycl::malloc_shared<T>(std::max<std::size_t>(n, 1), getQueue());
  if (p == nullptr) {
    fail("allocation failed");
  }
  return p;
}

static void deallocate(void* const p) { ::sycl::free(p, getQueue()); }

#endif /* defined(MFRONT_GPMP_COMPARE_USE_HOST_MEMORY) */

static void synchronize() { getQueue().wait_and_throw(); }

/*!
 * \return the execution policy of the synchronous computations, on the
 * queue of `getQueue` (the `sycl` backend has no default policy)
 */
static const mfront_gp_AbstractBackendExecutionPolicy* executionPolicy() {
  static const auto p =
      mfront::generic_parallel::makeSyclBackendExecutionPolicy(getQueue());
  return &p;
}

#elif defined(GPU_API)

//! \brief nothing to initialize
struct Runtime {};

static void check(const GPU_API(Error_t) e) {
  if (e != GPU_API(Success)) {
    fail(std::string{"GPU error: "} + GPU_API(GetErrorString)(e));
  }
}

template <typename T>
static T* allocate(const std::size_t n) {
  T* p = nullptr;
  check(GPU_API(MallocManaged)(&p, std::max<std::size_t>(n, 1) * sizeof(T)));
  return p;
}

static void deallocate(void* const p) { check(GPU_API(Free)(p)); }

static void synchronize() { check(GPU_API(DeviceSynchronize)()); }

/*!
 * \return the execution policy of the synchronous computations: the default
 * policy (default stream)
 */
static const mfront_gp_AbstractBackendExecutionPolicy* executionPolicy() {
  return nullptr;
}

#else

//! \brief nothing to initialize
struct Runtime {};

template <typename T>
static T* allocate(const std::size_t n) {
  auto* const p =
      static_cast<T*>(std::malloc(std::max<std::size_t>(n, 1) * sizeof(T)));
  if (p == nullptr) {
    fail("allocation failed");
  }
  return p;
}

static void deallocate(void* const p) { std::free(p); }

static void synchronize() {}

/*!
 * \return the execution policy of the synchronous computations: the default
 * policy
 */
static const mfront_gp_AbstractBackendExecutionPolicy* executionPolicy() {
  return nullptr;
}

#endif

/*!
 * \return an execution policy whose identifier is the one expected by the
 * tested library (`<name>_generic_parallel_execution_policy` symbol), with
 * the given common options, on the queue of the tests with SYCL and on the
 * default queue otherwise. The program fails if the identifier is
 * not the one of a policy available in this program.
 * \param[in] identifier: identifier of the policy
 * \param[in] o: common options
 */
static std::unique_ptr<mfront::generic_parallel::AbstractBackendExecutionPolicy>
makeExecutionPolicy(
    const std::string& identifier,
    const mfront::generic_parallel::BackendExecutionCommonPolicy& o) {
  namespace mgp = mfront::generic_parallel;
  if (identifier == mgp::ParallelSTLBackendExecutionPolicy::identifier()) {
    return std::make_unique<mgp::ParallelSTLBackendExecutionPolicy>(o);
  }
#if defined(MFRONT_GPMP_COMPARE_USE_SYCL)
  if (identifier == mgp::SyclBackendExecutionPolicy::identifier()) {
    return std::make_unique<mgp::SyclBackendExecutionPolicy>(getQueue(), o);
  }
#elif defined(__HIP__) || defined(MFRONT_GPMP_COMPARE_USE_HIP)
  if (identifier == mgp::HIPBackendExecutionPolicy::identifier()) {
    return std::make_unique<mgp::HIPBackendExecutionPolicy>(
        mgp::HIPBackendExecutionOptions{.common_options = o});
  }
#elif defined(__CUDACC__)
  if (identifier == mgp::CUDABackendExecutionPolicy::identifier()) {
    return std::make_unique<mgp::CUDABackendExecutionPolicy>(
        mgp::CUDABackendExecutionOptions{.common_options = o});
  }
#endif
  fail("no execution policy '" + identifier + "' in this program");
}

static void* load(const std::string& lib, const std::string& f) {
  auto* const l = dlopen(lib.c_str(), RTLD_NOW | RTLD_LOCAL);
  if (l == nullptr) {
    fail(dlerror());
  }
  auto* const s = dlsym(l, f.c_str());
  if (s == nullptr) {
    fail(dlerror());
  }
  return s;
}

#endif /* LIB_MFRONT_GENERICPARALLELMATERIALPROPERTY_TESTS_RUNTIME_HXX */
