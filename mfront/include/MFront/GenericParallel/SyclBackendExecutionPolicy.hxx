/*!
 * \file   mfront/include/MFront/GenericParallel/SyclBackendExecutionPolicy.hxx
 * \brief  This file declares the execution policy of the `sycl` backend of
 * the `generic-parallel` interfaces, which holds a queue. Its identifier
 * includes the implementation of SYCL, so that a policy created with one
 * implementation is refused by a library compiled with another one.
 * \date   22/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_SYCLBACKENDEXECUTIONPOLICY_HXX
#define LIB_MFRONT_GENERICPARALLEL_SYCLBACKENDEXECUTIONPOLICY_HXX

#include <sycl/sycl.hpp>
#include "MFront/GenericParallel/BackendExecutionPolicyBase.hxx"

namespace mfront::generic_parallel {

  /*!
   * \brief execution policy of the `sycl` backend. The `sycl` backend has no
   * default policy: the arrays given by the caller belong to the context of
   * a queue, so the queue is required.
   */
  struct SyclBackendExecutionPolicy final : BackendExecutionPolicyBase {
    //! \return the identifier of the policy
    static constexpr const char* identifier() noexcept {
#if defined(SYCL_IMPLEMENTATION_ONEAPI)
      return "sycl:oneapi";
#elif defined(SYCL_IMPLEMENTATION_ACPP) || defined(SYCL_IMPLEMENTATION_HIPSYCL)
      return "sycl:acpp";
#else
#error "unsupported implementation of SYCL"
#endif
    }
    /*!
     * \brief constructor
     * \param[in] q: queue on which the kernels are submitted. Asynchronous
     * computations require an in-order queue.
     * \param[in] opts: common options
     */
    SyclBackendExecutionPolicy(const ::sycl::queue& q,
                               const BackendExecutionCommonPolicy& opts = {})
        : BackendExecutionPolicyBase(opts), queue(q) {}
    const char* getIdentifier() const noexcept override { return identifier(); }
    //! \brief queue on which the kernels are submitted
    ::sycl::queue queue;
  };  // end of struct SyclBackendExecutionPolicy

  /*!
   * \return an execution policy of the `sycl` backend
   * \param[in] q: queue on which the kernels are submitted (required)
   * \param[in] opts: common options
   */
  inline SyclBackendExecutionPolicy makeSyclBackendExecutionPolicy(
      const ::sycl::queue& q, const BackendExecutionCommonPolicy& opts = {}) {
    return SyclBackendExecutionPolicy{q, opts};
  }  // end of makeSyclBackendExecutionPolicy

}  // end of namespace mfront::generic_parallel

#endif /* LIB_MFRONT_GENERICPARALLEL_SYCLBACKENDEXECUTIONPOLICY_HXX */
