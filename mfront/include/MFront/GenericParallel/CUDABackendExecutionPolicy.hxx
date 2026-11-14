/*!
 * \file   mfront/include/MFront/GenericParallel/CUDABackendExecutionPolicy.hxx
 * \brief  This file declares the execution policy of the `cuda` backend
 * of the `generic-parallel` interfaces.
 * \date   22/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_CUDABACKENDEXECUTIONPOLICY_HXX
#define LIB_MFRONT_GENERICPARALLEL_CUDABACKENDEXECUTIONPOLICY_HXX

#include <cuda_runtime_api.h>
#include "MFront/GenericParallel/BackendExecutionPolicyBase.hxx"

namespace mfront::generic_parallel {

  //! \brief options of the execution policy of the `cuda` backend
  struct CUDABackendExecutionOptions {
    //! \brief common options
    BackendExecutionCommonPolicy common_options;
    //! \brief stream on which the kernels are launched (default stream if null)
    cudaStream_t stream = nullptr;
  };  // end of struct CUDABackendExecutionOptions

  //! \brief execution policy of the `cuda` backend
  struct CUDABackendExecutionPolicy final : BackendExecutionPolicyBase {
    //! \return the identifier of the policy
    static constexpr const char* identifier() noexcept { return "cuda"; }
    /*!
     * \brief constructor
     * \param[in] opts: options
     */
    explicit CUDABackendExecutionPolicy(
        const CUDABackendExecutionOptions& opts = {}) noexcept
        : BackendExecutionPolicyBase(opts.common_options),
          stream(opts.stream) {}
    const char* getIdentifier() const noexcept override { return identifier(); }
    //! \brief stream on which the kernels are launched (default stream if null)
    cudaStream_t stream = nullptr;
  };  // end of struct CUDABackendExecutionPolicy

  /*!
   * \return an execution policy of the `cuda` backend
   * \param[in] opts: options
   */
  inline CUDABackendExecutionPolicy makeCUDABackendExecutionPolicy(
      const CUDABackendExecutionOptions& opts = {}) noexcept {
    return CUDABackendExecutionPolicy{opts};
  }  // end of makeCUDABackendExecutionPolicy

}  // end of namespace mfront::generic_parallel

#endif /* LIB_MFRONT_GENERICPARALLEL_CUDABACKENDEXECUTIONPOLICY_HXX */
