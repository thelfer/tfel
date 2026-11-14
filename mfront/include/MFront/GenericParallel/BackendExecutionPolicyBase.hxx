/*!
 * \file   mfront/include/MFront/GenericParallel/BackendExecutionPolicyBase.hxx
 * \brief  This file declares the options common to the execution policies
 * of most backends, the base class of those policies, and the functions
 * used by the generated functions to check the policy they receive.
 * \date   22/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_BACKENDEXECUTIONPOLICYBASE_HXX
#define LIB_MFRONT_GENERICPARALLEL_BACKENDEXECUTIONPOLICYBASE_HXX

#include <cstring>
#include "MFront/GenericParallel/AbstractBackendExecutionPolicy.hxx"

namespace mfront::generic_parallel {

  //! \brief options common to the execution policies of most backends
  struct BackendExecutionCommonPolicy {
    /*!
     * \brief if true, the stack size of the threads of the device is never
     * modified and an error is reported if it is too small. Otherwise, the
     * stack size is increased if the kernel requires it. The stack size is a
     * property of the context of the device, shared by the whole process
     * (`CUDA` and `HIP` runtimes), and costs memory of the device: a code
     * sharing the device with other libraries may set this option.
     */
    bool preserve_stack_size = false;
    /*!
     * \brief if true, the generated function may return before the end of
     * the computations, once they are submitted to the queue of the policy.
     * The calling code then synchronizes the queue before reading the
     * outputs and the statistics. An asynchronous computation requires
     * statistics (invalid arguments otherwise). The `stlpar` backend, even
     * when it computes on a device, and the `sycl` backend when it transfers
     * data, compute synchronously.
     */
    bool asynchronous = false;
  };  // end of struct BackendExecutionCommonPolicy

  /*!
   * \brief base class of the execution policies of the backends using the
   * common options
   */
  struct BackendExecutionPolicyBase : AbstractBackendExecutionPolicy,
                                      BackendExecutionCommonPolicy {
    /*!
     * \brief constructor
     * \param[in] opts: common options
     */
    explicit BackendExecutionPolicyBase(
        const BackendExecutionCommonPolicy& opts) noexcept
        : BackendExecutionCommonPolicy(opts) {}
  };  // end of struct BackendExecutionPolicyBase

  /*!
   * \return the policy of the given type, or a null pointer if the
   * identifier of the policy is not the one of this type
   * \tparam Policy: expected policy, whose static member function
   * `identifier` gives the identifier
   * \param[in] p: policy
   */
  template <typename Policy>
  const Policy* getBackendExecutionPolicy(
      const AbstractBackendExecutionPolicy& p) noexcept {
    if (std::strcmp(p.getIdentifier(), Policy::identifier()) != 0) {
      return nullptr;
    }
    // valid: the base classes of the policies are not virtual
    return static_cast<const Policy*>(&p);
  }  // end of getBackendExecutionPolicy

  /*!
   * \brief policy given to a generated function, once checked (see
   * `checkBackendExecutionPolicy`)
   */
  template <typename Policy>
  struct CheckedBackendExecutionPolicy {
    /*!
     * \brief policy given by the caller, or a null pointer for the default
     * policy of the backend
     */
    const Policy* policy = nullptr;
    //! \brief common options, the default ones for the default policy
    BackendExecutionCommonPolicy common_options;
    //! \brief reason why the policy is refused, or a null pointer
    const char* error = nullptr;
  };  // end of struct CheckedBackendExecutionPolicy

  /*!
   * \return the policy given to a generated function, checked against the
   * policy of its backend: a policy of another backend, or an asynchronous
   * computation without statistics, are refused. The generated function
   * copies what it needs from the policy, which does not have to outlive the
   * call.
   * \tparam Policy: policy of the backend
   * \param[in] p: policy given by the caller, or a null pointer for the
   * default policy
   * \param[in] has_statistics: true if the caller gave statistics
   */
  template <typename Policy>
  CheckedBackendExecutionPolicy<Policy> checkBackendExecutionPolicy(
      const AbstractBackendExecutionPolicy* const p,
      const bool has_statistics) noexcept {
    auto r = CheckedBackendExecutionPolicy<Policy>{};
    if (p == nullptr) {
      return r;
    }
    r.policy = getBackendExecutionPolicy<Policy>(*p);
    if (r.policy == nullptr) {
      r.error = "the execution policy is not the one of the backend";
      return r;
    }
    r.common_options =
        static_cast<const BackendExecutionCommonPolicy&>(*(r.policy));
    if (r.common_options.asynchronous && !has_statistics) {
      r.error = "an asynchronous computation requires statistics";
    }
    return r;
  }  // end of checkBackendExecutionPolicy

}  // end of namespace mfront::generic_parallel

#endif /* LIB_MFRONT_GENERICPARALLEL_BACKENDEXECUTIONPOLICYBASE_HXX */
