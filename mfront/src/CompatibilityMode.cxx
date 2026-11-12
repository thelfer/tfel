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
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/MFrontWarningMode.hxx"
#include "MFront/CompatibilityMode.hxx"

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

  namespace internals {

    [[nodiscard]] static std::string to_string(
        const CompatibilityMode::Version& v) {
      if (v == CompatibilityMode::future) {
        return "future";
      }
      return std::to_string(v.major) + "." + std::to_string(v.minor) + "." +
             std::to_string(v.patch);
    }  // end of to_string

  }  // namespace internals

  void check(const CompatibilityMode::Version& v) {
    if (v < CompatibilityMode::v5_2_0) {
      tfel::raise("version '" + internals::to_string(v) +
                  "' is invalid: it must be greater than or equal to 5.2.0");
    }
  }

  std::string to_string(const CompatibilityMode::Version& v) {
    check(v);
    return internals::to_string(v);
  }  // end of to_string

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
      tfel::raise("version '" + internals::to_string(v) +
                  "' is invalid: it must be greater than or equal to 5.2.0");
    }
    const auto selected_version = *(std::prev(p));
    if (v != selected_version) {
      reportWarning("clamping version '" + internals::to_string(v) +
                    "' to closest compatibility mode '" +
                    internals::to_string(selected_version) + "'");
    }
    return selected_version;
  }  // end of getClosestVersion

  [[nodiscard]] static std::optional<CompatibilityMode::Version>&
  getCompatibilityMode() noexcept {
    static auto v = std::optional<CompatibilityMode::Version>{};
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
  }  // end of fromString

  void setDefaultCompatibilityMode(const CompatibilityMode::Version& v) {
    getCompatibilityMode() = getClosestVersion(v);
  }  // end of setDefaultCompatibilityMode

  void setDefaultCompatibilityMode(std::string_view v) {
    const auto version = CompatibilityMode::fromString(v);
    setDefaultCompatibilityMode(version);
  }  // end of setDefaultCompatibilityMode

  CompatibilityMode::Version getGreatestCompatibilityMode() noexcept {
    const auto pm =
        std::max_element(CompatibilityMode::known_compatibility_modes.begin(),
                         CompatibilityMode::known_compatibility_modes.end());
    return *pm;
  }  // end of getGreatestCompatibilityMode

  std::optional<CompatibilityMode::Version>
  getDefaultCompatibilityMode() noexcept {
    return getCompatibilityMode();
  }  // end of getDefaultCompatibilityMode

}  // end of namespace mfront