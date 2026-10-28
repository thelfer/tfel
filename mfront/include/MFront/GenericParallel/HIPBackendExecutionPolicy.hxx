/*!
 * \file   mfront/include/MFront/GenericParallel/HIPBackendExecutionPolicy.hxx
 * \brief  This file declares the execution policy of the `hip` backend
 * of the `generic-parallel` interfaces.
 * \date   22/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_HIPBACKENDEXECUTIONPOLICY_HXX
#define LIB_MFRONT_GENERICPARALLEL_HIPBACKENDEXECUTIONPOLICY_HXX

#include <hip/hip_runtime_api.h>
#include "MFront/GenericParallel/BackendExecutionPolicyBase.hxx"

namespace mfront::generic_parallel {

  //! \brief options of the execution policy of the `hip` backend
  struct HIPBackendExecutionOptions {
    //! \brief common options
    BackendExecutionCommonPolicy common_options;
    //! \brief stream on which the kernels are launched (default stream if null)
    hipStream_t stream = nullptr;
  };  // end of struct HIPBackendExecutionOptions

  //! \brief execution policy of the `hip` backend
  struct HIPBackendExecutionPolicy final : BackendExecutionPolicyBase {
    //! \return the identifier of the policy
    static constexpr const char* identifier() noexcept { return "hip"; }
    /*!
     * \brief constructor
     * \param[in] opts: options
     */
    explicit HIPBackendExecutionPolicy(
        const HIPBackendExecutionOptions& opts = {}) noexcept
        : BackendExecutionPolicyBase(opts.common_options),
          stream(opts.stream) {}
    const char* getIdentifier() const noexcept override { return identifier(); }
    //! \brief stream on which the kernels are launched (default stream if null)
    hipStream_t stream = nullptr;
  };  // end of struct HIPBackendExecutionPolicy

  /*!
   * \return an execution policy of the `hip` backend
   * \param[in] opts: options
   */
  inline HIPBackendExecutionPolicy makeHIPBackendExecutionPolicy(
      const HIPBackendExecutionOptions& opts = {}) noexcept {
    return HIPBackendExecutionPolicy{opts};
  }  // end of makeHIPBackendExecutionPolicy

}  // end of namespace mfront::generic_parallel

#endif /* LIB_MFRONT_GENERICPARALLEL_HIPBACKENDEXECUTIONPOLICY_HXX */
