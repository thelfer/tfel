/*!
 * \file   mfront/include/MFront/GenericParallel/DeviceStackSize.hxx
 * \brief  This file declares a function setting the stack size of the
 * threads of NVIDIA GPUs for the parallel programming models which do not
 * give access to this setting.
 * \date   05/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_DEVICESTACKSIZE_HXX
#define LIB_MFRONT_GENERICPARALLEL_DEVICESTACKSIZE_HXX

#include <mutex>
#include <cstddef>
#if !defined(_WIN32)
#include <dlfcn.h>
#endif

namespace mfront::generic_parallel {

  /*!
   * \return a mutex protecting the modifications of the stack size of the
   * threads of the device. The stack size is a property of the context of
   * the device, shared by all the threads of the process: concurrent
   * modifications by other libraries are not protected.
   */
  inline std::mutex& getStackSizeMutex() noexcept {
    static std::mutex m;
    return m;
  }  // end of getStackSizeMutex

  //! \brief error message associated with `setMinimalCUDAStackSize`
  inline constexpr const char* stack_size_error_message =
      "unable to set the stack size of the threads of the device, or the "
      "stack size is too small and must be preserved";

  /*!
   * \brief increase the stack size of the threads of the current CUDA
   * device, if the CUDA runtime is loaded by the process. Otherwise, the
   * computations are not performed on an NVIDIA GPU through the CUDA runtime
   * and nothing is done.
   *
   * Some parallel programming models execute the computations on NVIDIA GPUs
   * through the CUDA runtime but give no access to the stack size of the
   * threads: the C++ standard parallelism with `nvc++ -stdpar=gpu` and SYCL
   * with AdaptiveCpp. The default stack size (1024 bytes) and the automatic
   * increase performed by the CUDA driver at kernel launch, which does not
   * take into account the internal routines of the compilers (such as the
   * division of double precision numbers), are not sufficient for some
   * kernels.
   *
   * The CUDA runtime is looked up among the loaded libraries, so that this
   * function does not require the CUDA headers nor the CUDA runtime.
   *
   * \return true on success, false if the stack size could not be set or
   * is too small and must be preserved
   * \param[in] stack_size: minimal stack size, in bytes
   * \param[in] preserve: if true, the stack size is only checked
   */
  inline bool setMinimalCUDAStackSize(const std::size_t stack_size,
                                      const bool preserve = false) noexcept {
#if defined(_WIN32)
    static_cast<void>(stack_size);
    static_cast<void>(preserve);
    return true;
#else
    // `cudaLimitStackSize` in `cudaLimit` and `cudaSuccess` in `cudaError_t`
    constexpr int stack_size_limit = 0;
    constexpr int success = 0;
    using GetLimit = int (*)(std::size_t*, int);
    using SetLimit = int (*)(int, std::size_t);
    if (stack_size == 0) {
      return true;
    }
    for (const auto* const library :
         {"libcudart.so.13", "libcudart.so.12", "libcudart.so"}) {
      auto* const h = ::dlopen(library, RTLD_LAZY | RTLD_NOLOAD);
      if (h == nullptr) {
        continue;
      }
      const auto get_limit =
          reinterpret_cast<GetLimit>(::dlsym(h, "cudaDeviceGetLimit"));
      const auto set_limit =
          reinterpret_cast<SetLimit>(::dlsym(h, "cudaDeviceSetLimit"));
      auto r = (get_limit != nullptr) && (set_limit != nullptr);
      if (r) {
        auto lock = std::lock_guard<std::mutex>{getStackSizeMutex()};
        auto current = std::size_t{};
        r = get_limit(&current, stack_size_limit) == success;
        if ((r) && (current < stack_size)) {
          r = (!preserve) &&
              (set_limit(stack_size_limit, stack_size) == success);
        }
      }
      ::dlclose(h);
      return r;
    }
    return true;
#endif
  }  // end of setMinimalCUDAStackSize

}  // end of namespace mfront::generic_parallel

#endif /* LIB_MFRONT_GENERICPARALLEL_DEVICESTACKSIZE_HXX */
