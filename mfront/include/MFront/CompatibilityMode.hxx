/*!
 * \file   MFront/CompatibilityMode.hxx
 * \brief  This file declare the CompatibilityMode
 * structure and associated functions
 * \author Thomas Helfer
 * \date   08/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_COMPATIBILITYMODE_HXX
#define LIB_MFRONT_COMPATIBILITYMODE_HXX

#include <limits>
#include <array>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * \brief struct describing the various compatibility modes available
   *
   * Compability modes are made for introducing breaking changes in the results
   * or in the behaviour of MFront. Two examples motivated the introduction of
   * this concept:
   *
   * - the modification of the values defined in the PhysicalConstants library
   *   to be more accurate and up-to-date.
   * - the evolution of the PowellDogLeg algorithms to a more robust and
   *   classical version.
   *
   * Those evolutions do not correct bugs, but changes the default MFront's
   * behaviour and the results of the generated code for the better.
   * Compatibility modes are menat to restore the behaviour of previous versions
   * when needed for backward compatibility.
   *
   * Note that such evolutions of the default MFront's behaviour shall only be
   * tolerated on major versions. To be able to use such evolutions in the
   * current version for testing and/or anticipating changes, the `future`
   * compatibility mode allows changes that will be the default in the next
   * major version.
   *
   * Compatiblity modes can be defined globally on the command line using
   * `--compatibility-mode` or in a file using the @CompatiblityMode keyword.
   */
  struct CompatibilityMode {
    /*!
     * \brief simple data structure describing a version with three numbers
     * named major, minor and patch
     */
    struct Version {
      std::size_t major;
      std::size_t minor;
      std::size_t patch;
      constexpr auto operator<=>(const Version&) const noexcept = default;
    };  // end of struct version
    /*!
     * \brief compatibility mode with the next (unpublished) major version
     */
    static inline constexpr auto future =
        Version{.major = std::numeric_limits<std::size_t>::max(),
                .minor = std::numeric_limits<std::size_t>::max(),
                .patch = std::numeric_limits<std::size_t>::max()};
    // \brief first version in which compatiblity modes were introduced
    static inline constexpr auto v_5_2_0 =
        Version{.major = 5, .minor = 2, .patch = 0};
    //! \brief list of compatibility modes
    static inline auto known_compatibility_modes = std::array{v_5_2_0};
  };  // end of struct CompatibilityMode

}  // end of namespace mfront

#endif /* LIB_MFRONT_COMPATIBILITYMODE_HXX */
