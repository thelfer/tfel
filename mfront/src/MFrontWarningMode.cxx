/*!
 * \file  mfront/src/MFrontWarningMode.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 avril 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontWarningMode.hxx"

namespace mfront {

  static bool& getMFrontWarningMode() noexcept {
    static bool b = true;
    return b;
  }

  bool getWarningMode() noexcept { return getMFrontWarningMode(); }

  void setWarningMode(const bool b) noexcept { getMFrontWarningMode() = b; }

  static bool& getIgnoreSafeOption() noexcept {
    static bool b = false;
    return b;
  }

  bool ignoreSafeOptionForWarnings() noexcept { return getIgnoreSafeOption(); }

  void setIgnoreSafeOptionForWarnings(const bool b) noexcept {
    getIgnoreSafeOption() = b;
  }

  static bool& getMFrontWarningErrorMode() noexcept {
    static bool b = false;
    return b;
  }

  bool getWarningErrorMode() noexcept { return getMFrontWarningErrorMode(); }

  void setWarningErrorMode(const bool b) noexcept {
    getMFrontWarningErrorMode() = b;
  }

  void reportWarning(std::string_view msg) {
    if ((!getWarningMode()) || (msg.empty())) {
      return;
    }
    if (getWarningErrorMode()) {
      tfel::raise("[warning]: " + std::string(msg));
    } else {
      getLogStream() << "[warning]: " << msg << '\n';
    }
  }  // end of reportWarning

  void reportWarning(const std::vector<std::string>& warnings) {
    if ((!getWarningMode()) || (warnings.empty())) {
      return;
    }
    auto p = warnings.begin();
    auto pe = warnings.end();
    auto msg = "[warning]: " + *p;
    ++p;
    for (; p != pe; ++p) {
      msg += "\n[warning]: " + *p;
    }
    if (getWarningErrorMode()) {
      tfel::raise(msg);
    } else {
      getLogStream() << msg << '\n';
    }
  }  // end of reportWarning

}  // end of namespace mfront
