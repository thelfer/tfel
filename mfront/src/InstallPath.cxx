/*!
 * \file   mfront/src/InstallPath.cxx
 * \brief
 * \author Thomas Helfer
 * \date   06/03/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include "TFEL/Raise.hxx"
#include "MFront/InstallPath.hxx"

namespace mfront {

  namespace internals {

    static std::string& getInstallPath() {
      static std::string p;
      return p;
    }  // end of getInstallPath

  }  // end of namespace internals

  void setInstallPath(const std::string& p) {
    auto& path = mfront::internals::getInstallPath();
    tfel::raise_if(!path.empty(),
                   "mfront::setInstallPath: install path already set");
    path = p;
  }  // end of setInstallPath

  std::string getInstallPath() {
    auto& path = mfront::internals::getInstallPath();
    if (!path.empty()) {
      return path;
    }
    auto* epath = std::getenv("MFRONT_INSTALL_PREFIX");
    if (epath != nullptr) {
      return epath;
    }
    return {};
  }  // end of getInstallPath

}  // namespace mfront
