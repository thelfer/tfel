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

namespace mfront {

  /*!
   * \brief convert the given string token to an integer
   * \return the number on success
   * \param[in] v: string
   */
  [[nodiscard]] static std::size_t convertToInteger(const std::string& v) {
    // std::from_chars does not accept an explicit plus sign
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

  void check(const CompatibilityMode::Version& v) {
    if (v < CompatibilityMode::v5_2_0) {
      tfel::raise("version must be greater than 5.2.0");
    }
  }

  [[nodiscard]] static CompatibilityMode::Version getClosetVersion(
      const CompatibilityMode::Version& v) {
    if (v == CompatibilityMode::future) {
      return v;
    }
    check(v);
    const auto versions = std::set<CompatibilityMode::Version>{
        CompatibilityMode::known_compatibility_modes.begin(),
        CompatibilityMode::known_compatibility_modes.end()};
    const auto p = versions.lower_bound(v);
    if (p == versions.end()) {
      return *(versions.rbegin());
    }
    return *p;
  }  // end of getCompatibilityMode

  [[nodiscard]] static CompatibilityMode::Version& getCompatibilityMode() {
    static CompatibilityMode::Version v = getClosetVersion(
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
    return getClosetVersion(
        CompatibilityMode::Version{.major = convertToInteger(numbers.at(0)),
                                   .minor = convertToInteger(numbers.at(1)),
                                   .patch = convertToInteger(numbers.at(2))});
  } // end of fromString


  void setDefaultCompatibilityMode(const CompatibilityMode::Version& v) {
    getCompatibilityMode() = getClosetVersion(v);
  }  // end of setDefaultCompatibilityMode

  void setDefaultCompatibilityMode(std::string_view v) {
    const auto version = CompatibilityMode::fromString(v);
    setDefaultCompatibilityMode(version);
  }  // end of setDefaultCompatibilityMode

  CompatibilityMode::Version getDefaultCompatibilityMode() noexcept {
    return getCompatibilityMode();
  }  // end of getDefaultCompatibilityMode

}  // end of namespace mfront