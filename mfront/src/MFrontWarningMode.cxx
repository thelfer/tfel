/*!
 * \file  mfront/src/MFrontWarningMode.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 avril 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
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

  static bool& getMFrontWarningMode() {
    static bool b = true;
    return b;
  }

  bool getWarningMode() { return getMFrontWarningMode(); }

  void setWarningMode(const bool b) { getMFrontWarningMode() = b; }

  static bool& getMFrontWarningErrorMode() {
    static bool b = false;
    return b;
  }

  bool getWarningErrorMode() { return getMFrontWarningErrorMode(); }

  void setWarningErrorMode(const bool b) { getMFrontWarningErrorMode() = b; }

  void reportWarning(std::string_view msg){
    if ((!getWarningMode()) || (msg.empty())) {
      return;
    }
    if (getWarningErrorMode()) {
      tfel::raise("[warning]: " + std::string(msg));
    } else {
      getLogStream() << "[warning]: " << msg << '\n';
    }
  } // end of reportWarning

  MFRONT_VISIBILITY_EXPORT void reportWarning(const std::vector<std::string>& warnings) {
    if ((!getWarningMode()) || (warnings.empty())) {
      return;
    }
    auto p = warnings.begin();
    auto pe = warnings.end();
    auto msg = "[warning]: " + *p;
    for (; p != pe; ++p) {
      msg += "\n[warning]: " + *p;
    }
    if (getWarningErrorMode()) {
      tfel::raise(msg);
    } else {
      getLogStream() << msg << '\n';
    }
  }

}  // end of namespace mfront
