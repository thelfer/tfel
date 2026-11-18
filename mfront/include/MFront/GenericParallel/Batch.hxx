/*!
 * \file   mfront/include/MFront/GenericParallel/Batch.hxx
 * \brief  This file declares the statistics, the execution and the report
 * of the computations on a batch of points, shared by the primitives of the
 * backends of the `generic-parallel` interfaces.
 * \date   09/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_BATCH_HXX
#define LIB_MFRONT_GENERICPARALLEL_BATCH_HXX

#include <bit>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <exception>
#include <string_view>
#include "TFEL/Config/TFELConfig.hxx"
#include "MFront/GenericParallel/StatisticsKeys.hxx"
#include "MFront/GenericParallel/BackendExecutionPolicy.h"

/*!
 * The primitives of the backends (`apply`, see for instance
 * `MFront/GenericParallel/ParallelSTL.hxx`) apply a function at the points
 * of a batch. This function returns the statistics of a point
 * (`BatchStatistics`). The primitives combine them, by atomic operations or
 * by reductions, and report the result through a handler of the statistics
 * given by the interface (`mfront::gpmp::StatisticsHandler` for the
 * material properties), which provides:
 *
 * - the type `OutputStatus` of the output status and the type `Statistics`
 *   of the statistics updated by the devices and given by the calling code
 *   (C structures of the interface);
 * - `initial()`: the statistics of a batch without event, neutral element of
 *   the reductions;
 * - `hasEvents()`: false if the function applied at the points never reports
 *   an event, in which case the statistics are neither combined nor
 *   reported;
 * - `initialize(Statistics&)`: initialization of the statistics updated by
 *   the devices;
 * - `report(OutputStatus&, const Statistics&)`: report of the statistics
 *   updated by the devices, once the computations are completed;
 * - `report(OutputStatus&, Statistics*, const BatchStatistics&)`: report of
 *   the statistics combined by a reduction, or accumulation of those
 *   statistics in the statistics given by the calling code, if any.
 */

namespace mfront::generic_parallel {

  //! \brief type of the indices and of the numbers of points
  using size_type = std::size_t;

  /*!
   * \brief statistics of the computations at one point or at a group of
   * points of a batch, combined by parallel reductions. The keys are built
   * by `makeKey`.
   *
   * The time step scaling factor is a statistics of the computations which
   * may reduce the time step (see `hasTimeStepScalingFactor`): the material
   * properties leave it to its neutral value.
   *
   * \tparam real: numeric type of the time step scaling factor
   */
  template <typename real>
  struct BatchStatistics {
    //! \brief combine two statistics
    TFEL_HOST_DEVICE friend constexpr BatchStatistics combine(
        const BatchStatistics& a, const BatchStatistics& b) noexcept {
      return {.number_of_failures = a.number_of_failures + b.number_of_failures,
              .first_failure = a.first_failure < b.first_failure
                                   ? a.first_failure
                                   : b.first_failure,
              .number_of_unreliable_results = a.number_of_unreliable_results +
                                              b.number_of_unreliable_results,
              .first_unreliable_result =
                  a.first_unreliable_result < b.first_unreliable_result
                      ? a.first_unreliable_result
                      : b.first_unreliable_result,
              .rdt = a.rdt < b.rdt ? a.rdt : b.rdt};
    }
    //! \brief number of points where the computations failed
    size_type number_of_failures = 0;
    //! \brief key of the first failure (see `makeKey`)
    atomic_size_type first_failure = no_event;
    //! \brief number of points where the result is unreliable
    size_type number_of_unreliable_results = 0;
    //! \brief key of the first unreliable result (see `makeKey`)
    atomic_size_type first_unreliable_result = no_event;
    //! \brief time step scaling factor
    real rdt = 1;
  };

  //! \brief function object combining two statistics
  struct CombineStatistics {
    //! \return the combination of two statistics (see `combine`)
    template <typename real>
    TFEL_HOST_DEVICE constexpr BatchStatistics<real> operator()(
        const BatchStatistics<real>& a,
        const BatchStatistics<real>& b) const noexcept {
      return combine(a, b);
    }
  };

  /*!
   * \return an unsigned integer whose ordering is the same as the ordering
   * of the given floating point values. This allows to compute the minimum
   * of floating point values with an integer atomic operation, which is how
   * the devices update the time step scaling factor.
   */
  TFEL_HOST_DEVICE constexpr atomic_size_type toOrderedKey(
      const double v) noexcept {
    const auto b = std::bit_cast<atomic_size_type>(v);
    constexpr auto sign = atomic_size_type{1} << 63;
    return (b & sign) ? ~b : (b | sign);
  }  // end of toOrderedKey

  /*!
   * \brief true if the statistics updated by the devices hold a time step
   * scaling factor, false otherwise, which is the case of the statistics of
   * the material properties
   */
  template <typename Statistics>
  inline constexpr bool hasTimeStepScalingFactor =
      requires(Statistics& s) { s.rdt; };

  /*!
   * \brief update the statistics shared by the threads with the statistics
   * of a point or of a group of points, by atomic operations. The shared
   * statistics are only accessed if an event occurred: a failure, an
   * unreliable result, or a time step scaling factor lower than the initial
   * one.
   * \tparam Atomics: atomic operations of the backend, given by the static
   * functions `add` and `min` on values of type `atomic_size_type`
   * \param[in,out] s: shared statistics
   * \param[in] r: statistics of a point or of a group of points
   * \param[in] rdt0: ordered key of the initial time step scaling factor
   * (see `toOrderedKey`)
   */
  template <typename Atomics, typename Statistics, typename real>
  TFEL_HOST_DEVICE void updateStatistics(Statistics& s,
                                         const BatchStatistics<real>& r,
                                         const atomic_size_type rdt0) {
    if (r.number_of_failures != 0) {
      Atomics::add(&(s.number_of_failures),
                   static_cast<atomic_size_type>(r.number_of_failures));
      Atomics::min(&(s.first_failure), r.first_failure);
    }
    if (r.number_of_unreliable_results != 0) {
      Atomics::add(
          &(s.number_of_unreliable_results),
          static_cast<atomic_size_type>(r.number_of_unreliable_results));
      Atomics::min(&(s.first_unreliable_result), r.first_unreliable_result);
    }
    if constexpr (hasTimeStepScalingFactor<Statistics>) {
      const auto rdt = toOrderedKey(r.rdt);
      if (rdt < rdt0) {
        Atomics::min(&(s.rdt), rdt);
      }
    }
  }  // end of updateStatistics

  /*!
   * \brief execution of the computations on a batch, as given by the
   * calling code: the policy, checked by the backend against its own policy
   * (see `mfront::generic_parallel::checkBackendExecutionPolicy`), and the
   * statistics
   * \tparam Statistics: statistics updated by the devices
   */
  template <typename Statistics>
  struct BatchExecution {
    //! \brief execution policy, or null for the default policy
    const mfront_gp_AbstractBackendExecutionPolicy* policy;
    //! \brief statistics, or null
    Statistics* statistics;
  };  // end of struct BatchExecution

  /*!
   * \brief set the status and the error message of an output status
   * \param[out] s: output status
   * \param[in] status: status
   * \param[in] msg: error message
   * \return the status
   */
  template <typename OutputStatus>
  int report(OutputStatus& s,
             const int status,
             const std::string_view msg) noexcept {
    constexpr auto msize = sizeof(s.msg) - 1;
    const auto n = msg.size() < msize ? msg.size() : msize;
    s.status = status;
    std::memcpy(s.msg, msg.data(), n);
    s.msg[n] = '\0';
    return status;
  }  // end of report

  /*!
   * \brief call the body of a generated function of batches, so that no
   * exception goes through its C interface: an exception is reported as an
   * error of the backend (status `-7`). The output status is initialized
   * before the call, and `errno` is restored on return: it may be modified
   * by the runtimes of the backends and by the computations on the host.
   * \return the status
   * \param[out] s: output status
   * \param[in] initialize: function initializing the output status
   * \param[in] f: body of the function, returning the status
   */
  template <typename OutputStatus, typename Initialize, typename Function>
  int handleExceptions(OutputStatus* const s,
                       const Initialize& initialize,
                       Function&& f) noexcept {
    if (s == nullptr) {
      return -5;
    }
    struct ErrnoGuard {
      const int value = errno;
      ~ErrnoGuard() { errno = this->value; }
    };
    [[maybe_unused]] const auto errno_guard = ErrnoGuard{};
    initialize(*s);
    try {
      return f();
    } catch (std::exception& e) {
      return report(*s, -7, e.what());
    } catch (...) {
      return report(*s, -7, "unknown exception");
    }
  }  // end of handleExceptions

}  // end of namespace mfront::generic_parallel

#endif /* LIB_MFRONT_GENERICPARALLEL_BATCH_HXX */
