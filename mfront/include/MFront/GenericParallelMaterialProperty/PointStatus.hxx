/*!
 * \file   MFront/GenericParallelMaterialProperty/PointStatus.hxx
 * \brief  This file declares the status of the evaluation of a material
 * property at a point, and its merge in the statistics of an evaluation.
 * \date   08/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLELMATERIALPROPERTY_POINTSTATUS_HXX
#define LIB_MFRONT_GENERICPARALLELMATERIALPROPERTY_POINTSTATUS_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "MFront/GenericParallel/StatisticsKeys.hxx"
#include "MFront/GenericParallelMaterialProperty/MaterialProperty.h"

namespace mfront::gpmp {

  // keys of the statistics (see `MFront/GenericParallel/StatisticsKeys.hxx`)
  using ::mfront::generic_parallel::check_bits;
  using ::mfront::generic_parallel::getCheck;
  using ::mfront::generic_parallel::getPointIndex;
  using ::mfront::generic_parallel::makeKey;
  using ::mfront::generic_parallel::maximal_number_of_checks;
  using ::mfront::generic_parallel::maximal_number_of_points;
  using ::mfront::generic_parallel::no_event;

  /*!
   * \brief status of the evaluation of a material property at a point,
   * computed by the kernels. With atomic operations, it is merged in the
   * statistics of the evaluation only if an event occurred, so that the
   * cost of the reporting is null when the evaluation succeeds. The
   * reductions merge the statuses of all the points.
   *
   * The status and the retained check follow the `generic` interface: the
   * first check which makes the evaluation fail ends it, and, otherwise, the
   * last bound violated with the warning policy is retained.
   */
  struct PointStatus {
    /*!
     * \brief status of the point, with the meaning of the status of the
     * `generic` interface: `0` (success), `1` (bound violated with the
     * warning policy), `-1` (physical bound, or bound with the strict
     * policy) or `-4` (invalid result)
     */
    int status = 0;
    //! \brief identifier of the check retained at this point
    unsigned long long check = no_event;
    /*!
     * \brief report a violated bound
     * \param[in] c: identifier of the check
     * \param[in] f: true if the evaluation fails, false if the result is
     * computed but must be used with caution
     */
    TFEL_HOST_DEVICE constexpr void report(const unsigned long long c,
                                           const bool f) noexcept {
      this->status = f ? -1 : 1;
      this->check = c;
    }  // end of report
    /*!
     * \brief report an invalid result
     * \param[in] c: identifier of the check
     */
    TFEL_HOST_DEVICE constexpr void reportInvalidResult(
        const unsigned long long c) noexcept {
      this->status = -4;
      this->check = c;
    }  // end of reportInvalidResult
    //! \return true if an event occurred
    [[nodiscard]] TFEL_HOST_DEVICE constexpr bool hasEvent() const noexcept {
      return this->status != 0;
    }  // end of hasEvent
    //! \return true if the evaluation failed
    [[nodiscard]] TFEL_HOST_DEVICE constexpr bool isFailure() const noexcept {
      return this->status < 0;
    }  // end of isFailure
  };  // end of struct PointStatus

  /*!
   * \return the statistics associated with the status of a point, as merged
   * by the backends based on reductions
   * \param[in] i: index of the point
   * \param[in] s: status of the point
   */
  [[nodiscard]] TFEL_HOST_DEVICE constexpr Statistics makeStatistics(
      const unsigned long long i, const PointStatus& s) noexcept {
    const auto failure = s.isFailure();
    const auto unreliable = s.status == 1;
    return Statistics{
        .number_of_failures = failure ? 1ull : 0ull,
        .first_failure = failure ? makeKey(i, s.check) : no_event,
        .number_of_unreliable_results = unreliable ? 1ull : 0ull,
        .first_unreliable_result = unreliable ? makeKey(i, s.check) : no_event};
  }  // end of makeStatistics

  /*!
   * \return the merge of two statistics
   * \param[in] a: first statistics
   * \param[in] b: second statistics
   */
  [[nodiscard]] TFEL_HOST_DEVICE constexpr Statistics merge(
      const Statistics& a, const Statistics& b) noexcept {
    return Statistics{
        .number_of_failures = a.number_of_failures + b.number_of_failures,
        .first_failure = (a.first_failure < b.first_failure) ? a.first_failure
                                                             : b.first_failure,
        .number_of_unreliable_results =
            a.number_of_unreliable_results + b.number_of_unreliable_results,
        .first_unreliable_result =
            (a.first_unreliable_result < b.first_unreliable_result)
                ? a.first_unreliable_result
                : b.first_unreliable_result};
  }  // end of merge

}  // end of namespace mfront::gpmp

#endif /* LIB_MFRONT_GENERICPARALLELMATERIALPROPERTY_POINTSTATUS_HXX */
