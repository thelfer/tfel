/*!
 * \file   Log.cxx
 * \brief
 * \author Thomas Helfer
 * \date   26/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "MFront/MFrontLogStream.hxx"
#include "MFMTestGenerator/Log.hxx"

namespace mfmtg {

  void debug(const std::string& msg) {
    if (mfront::getVerboseMode() >= mfront::VERBOSE_DEBUG) {
      mfront::getLogStream() << msg << std::endl;
    }
  }  // end of debug

  void message(const std::string& msg) {
    if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
      mfront::getLogStream() << msg << '\n';
    }
  }  // end of message

}  // end of namespace mfmtg
