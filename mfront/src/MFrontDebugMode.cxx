/*!
 * \file  mfront/src/MFrontDebugMode.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 avril 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/MFrontDebugMode.hxx"

namespace mfront {

  static bool& getMFrontDebugMode() {
    static bool b = false;
    return b;
  }

  bool getDebugMode() { return getMFrontDebugMode(); }

  void setDebugMode(const bool b) { getMFrontDebugMode() = b; }

}  // end of namespace mfront
