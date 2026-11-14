/*!
 * \file   mfront/include/MFront/GenericParallel/GPUStackSize.hxx
 * \brief  This file declares a function adapting the stack size of the
 * threads of the device to a CUDA or HIP kernel, used by the
 * `generic-parallel` interface for material properties. This file must be
 * compiled by a CUDA or a HIP compiler. The HIP runtime is used if the
 * `hip/hip_runtime.h` header has been included before this file or if the HIP
 * language is enabled, the CUDA runtime otherwise.
 * \date   08/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_GPUSTACKSIZE_HXX
#define LIB_MFRONT_GENERICPARALLEL_GPUSTACKSIZE_HXX

#include <mutex>
#include <string>
#include <cstddef>
// the `MFRONT_GENERIC_PARALLEL_GPU_API` macro gives the name of a function
// or of a type of the runtime used
#if defined(__HIPCC__) || defined(__HIP__)
#include <hip/hip_runtime.h>
#define MFRONT_GENERIC_PARALLEL_GPU_API(f) hip##f
#else
#include <cuda_runtime.h>
#define MFRONT_GENERIC_PARALLEL_GPU_API(f) cuda##f
#endif
#include "MFront/GenericParallel/DeviceStackSize.hxx"

namespace mfront::generic_parallel::gpu {

  //! \brief error code of the runtime
  using Error = MFRONT_GENERIC_PARALLEL_GPU_API(Error_t);

  //! \brief result of `ensureStackSize`
  struct StackSizeCheck {
    //! \brief error of the runtime
    Error error = MFRONT_GENERIC_PARALLEL_GPU_API(Success);
    /*!
     * \brief stack size required by the kernel if the current stack size is
     * too small and must be preserved, zero otherwise
     */
    std::size_t required_stack_size = 0;
    //! \return true if the kernel can be launched
    [[nodiscard]] bool succeeded() const noexcept {
      return (this->error == MFRONT_GENERIC_PARALLEL_GPU_API(Success)) &&
             (this->required_stack_size == 0);
    }
    //! \return the error message
    [[nodiscard]] std::string getErrorMessage() const {
      if (this->error != MFRONT_GENERIC_PARALLEL_GPU_API(Success)) {
        return MFRONT_GENERIC_PARALLEL_GPU_API(GetErrorString)(this->error);
      }
      return "the stack size of the threads of the device must be at "
             "least " +
             std::to_string(this->required_stack_size) +
             " bytes, but it is preserved by the execution policy";
    }
  };

  /*!
   * \brief increase the stack size of the threads of the current device if
   * the given kernel requires it. The default stack size (1024 bytes with
   * CUDA) is too small for some kernels, and the increase performed by
   * the runtime at kernel launch does not account for the internal routines
   * of the compilers (see `setMinimalCUDAStackSize`), hence the margin
   * added below to the static estimate of the local memory of the kernel.
   *
   * The kernel is a template argument, so that this function is instantiated
   * in the library defining the kernel. The CUDA runtime is linked
   * statically by default: each library has its own instance of the
   * runtime, which only knows the kernels of this library. A function
   * shared by several libraries, for instance a function taking the kernel
   * as argument, would be resolved to the instance of one library by the
   * dynamic linker.
   *
   * \tparam kernel: kernel
   * \param[in] preserve: if true, the stack size is not modified, and the
   * required stack size is returned if the current one is too small
   */
  template <auto kernel>
  StackSizeCheck ensureStackSize(const bool preserve) noexcept {
    auto r = StackSizeCheck{};
    auto a = MFRONT_GENERIC_PARALLEL_GPU_API(FuncAttributes){};
    // the HIP runtime only provides the untyped overload
    r.error = MFRONT_GENERIC_PARALLEL_GPU_API(FuncGetAttributes)(
        &a, reinterpret_cast<const void*>(kernel));
    if (r.error != MFRONT_GENERIC_PARALLEL_GPU_API(Success)) {
      return r;
    }
    // the static estimate of the local memory of the kernel may be slightly
    // lower than the actual stack usage (by a few tens of bytes for a kernel
    // calling the pow function of libdevice), so a margin is added
    constexpr auto margin = std::size_t{512};
    constexpr auto granularity = std::size_t{1024};
    const auto required =
        ((a.localSizeBytes + margin + granularity - 1) / granularity) *
        granularity;
    auto lock = std::lock_guard<std::mutex>{getStackSizeMutex()};
    auto l = std::size_t{};
    r.error = MFRONT_GENERIC_PARALLEL_GPU_API(DeviceGetLimit)(
        &l, MFRONT_GENERIC_PARALLEL_GPU_API(LimitStackSize));
    if ((r.error != MFRONT_GENERIC_PARALLEL_GPU_API(Success)) ||
        (required <= l)) {
      return r;
    }
    if (preserve) {
      r.required_stack_size = required;
      return r;
    }
    r.error = MFRONT_GENERIC_PARALLEL_GPU_API(DeviceSetLimit)(
        MFRONT_GENERIC_PARALLEL_GPU_API(LimitStackSize), required);
    return r;
  }  // end of ensureStackSize

}  // end of namespace mfront::generic_parallel::gpu

#endif /* LIB_MFRONT_GENERICPARALLEL_GPUSTACKSIZE_HXX */
