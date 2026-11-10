/*!
 * \file   mfront/src/CompatibilityMode.cxx
 * \brief  This file implements the function associated with the
 * CompatibilityMode strcture
 * \author Thomas Helfer
 * \date   08/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <set>
#include <cstdint>
#include <charconv>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/CompatibilityMode.hxx"

#ifndef TFEL_VERSION_MAJOR
#error "TFEL_VERSION_MAJOR must be defined"
#endif

#ifndef TFEL_VERSION_MINOR
#error "TFEL_VERSION_MINOR must be defined"
#endif

#ifndef TFEL_VERSION_PATCH
#error "TFEL_VERSION_PATCH must be defined"
#endif

namespace mfront {

  /*!
   * \brief convert the given string token to an integer
   * \return the number on success
   * \param[in] v: string
   */
  [[nodiscard]] static std::size_t convertToInteger(const std::string& v) {
    const auto b = v.data();
    const auto e = v.data() + v.size();
    auto i = static_cast<std::uint64_t>(0);
    const auto [pi, ec] = std::from_chars(b, e, i);
    if ((ec != std::errc{}) || (!std::in_range<std::size_t>(i))) {
      tfel::raise("can't convert '" + v + "' to an integer");
    }
    if (pi != e) {
      tfel::raise("can't convert '" + v + "' to an integer");
    }
    return static_cast<std::size_t>(i);
  }  // end of convertToInteger

  [[nodiscard]] static std::string to_string(const CompatibilityMode::Version& v) {
    return std::to_string(v.major) + "." + std::to_string(v.minor) + "." +
           std::to_string(v.patch);
  }  // end of to_string

  void check(const CompatibilityMode::Version& v) {
    if (v < CompatibilityMode::v5_2_0) {
      tfel::raise("version '" + to_string(v) +
                  "' is invalid: it must be greater or egal than 5.2.0");
    }
  }

  [[nodiscard]] static CompatibilityMode::Version getClosestVersion(
      const CompatibilityMode::Version& v) {
    if (v == CompatibilityMode::future) {
      return v;
    }
    check(v);
    const auto versions = std::set<CompatibilityMode::Version>{
        CompatibilityMode::known_compatibility_modes.begin(),
        CompatibilityMode::known_compatibility_modes.end()};
    const auto p = versions.upper_bound(v);
    if (p == versions.begin()) {
      // this shall have been catched by check, just a paranoiac check, that
      // does not hurt
      tfel::raise("version '" + to_string(v) +
                  "' is invalid: it must be greater or egal than 5.2.0");
    }
    return *(std::prev(p));
  }  // end of getCompatibilityMode

  [[nodiscard]] static CompatibilityMode::Version& getCompatibilityMode() {
    static CompatibilityMode::Version v = getClosestVersion(
        CompatibilityMode::Version{.major = TFEL_VERSION_MAJOR,
                                   .minor = TFEL_VERSION_MINOR,
                                   .patch = TFEL_VERSION_PATCH});
    return v;
  }  // end of getCompatibilityMode

  CompatibilityMode::Version CompatibilityMode::fromString(std::string_view s) {
    if (s == "future") {
      return CompatibilityMode::future;
    }
    const auto numbers = tfel::utilities::tokenize(s, ".");
    if (numbers.size() != 3) {
      tfel::raise("can't convert '" + std::string{s} + "' to a version");
    }
    return getClosestVersion(
        CompatibilityMode::Version{.major = convertToInteger(numbers.at(0)),
                                   .minor = convertToInteger(numbers.at(1)),
                                   .patch = convertToInteger(numbers.at(2))});
  } // end of fromString


  void setDefaultCompatibilityMode(const CompatibilityMode::Version& v) {
    getCompatibilityMode() = getClosestVersion(v);
  }  // end of setDefaultCompatibilityMode

  void setDefaultCompatibilityMode(std::string_view v) {
    const auto version = CompatibilityMode::fromString(v);
    setDefaultCompatibilityMode(version);
  }  // end of setDefaultCompatibilityMode

  // can't be noexcept since getCompatibilityMode is not
  CompatibilityMode::Version getDefaultCompatibilityMode() {
    return getCompatibilityMode();
  }  // end of getDefaultCompatibilityMode

}  // end of namespace mfront