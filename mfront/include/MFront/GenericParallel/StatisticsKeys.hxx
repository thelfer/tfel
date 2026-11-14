/*!
 * \file   mfront/include/MFront/GenericParallel/StatisticsKeys.hxx
 * \brief  This file declares the opaque keys of the statistics of the
 * computations on a batch of points, used by the `generic-parallel`
 * interface for material properties.
 * \date   21/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_STATISTICSKEYS_HXX
#define LIB_MFRONT_GENERICPARALLEL_STATISTICSKEYS_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "MFront/GenericParallel/Types.h"

namespace mfront::generic_parallel {

  //! \brief a simple alias
  using atomic_size_type = ::mfront_gp_atomic_size_type;

  //! \brief value of a key meaning that no event occurred
  inline constexpr atomic_size_type no_event = ~atomic_size_type{0};
  /*!
   * \brief number of bits of the identifier of a check in a key. The other
   * bits hold the index of the point.
   */
  inline constexpr unsigned int check_bits = 16;
  //! \brief maximal number of checks
  inline constexpr atomic_size_type maximal_number_of_checks =
      (atomic_size_type{1} << check_bits) - 1;
  //! \brief maximal number of points of a batch
  inline constexpr atomic_size_type maximal_number_of_points =
      (atomic_size_type{1} << (64 - check_bits)) - 1;

  /*!
   * \return the key of an event: the minimum of the keys gives the first
   * point concerned, whatever the order of the computations
   * \param[in] i: index of the point
   * \param[in] c: identifier of the check retained at this point
   */
  [[nodiscard]] TFEL_HOST_DEVICE constexpr atomic_size_type makeKey(
      const atomic_size_type i, const atomic_size_type c) noexcept {
    return (i << check_bits) | c;
  }  // end of makeKey

  //! \return the index of the point of a key
  [[nodiscard]] TFEL_HOST_DEVICE constexpr atomic_size_type getPointIndex(
      const atomic_size_type k) noexcept {
    return k >> check_bits;
  }  // end of getPointIndex

  //! \return the identifier of the check of a key
  [[nodiscard]] TFEL_HOST_DEVICE constexpr atomic_size_type getCheck(
      const atomic_size_type k) noexcept {
    return k & maximal_number_of_checks;
  }  // end of getCheck

}  // end of namespace mfront::generic_parallel

#endif /* LIB_MFRONT_GENERICPARALLEL_STATISTICSKEYS_HXX */
