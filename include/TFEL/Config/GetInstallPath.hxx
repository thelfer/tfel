/*!
 * \file   include/TFEL/Config/GetInstallPath.hxx
 * \brief
 * \author Thomas Helfer
 * \date   21 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_GETINSTALLPATH_HXX
#define LIB_TFEL_GETINSTALLPATH_HXX

#include <string>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  TFELCONFIG_VISIBILITY_EXPORT std::string getInstallPath();

  /*!
   * \return the name of an executable generated by the `TFEL` project, taking
   * into account the operating system and an eventual suffix (as defined by the
   * `TFEL_VERSION_FLAVOUR` and the `TFEL_APPEND_VERSION` options that can be
   * passed to `cmake` for details).
   * \param[in] n: base name of the executable (for example, `tfel-config`,
   * `mfront`).
   */
  TFELCONFIG_VISIBILITY_EXPORT std::string getTFELExecutableName(
      const std::string&);

  TFELCONFIG_VISIBILITY_EXPORT std::string getTFELConfigExecutableName();

  TFELCONFIG_VISIBILITY_EXPORT std::string getLibraryInstallName(
      const std::string&);

}  // end of namespace tfel

#endif /* LIB_TFEL_GETINSTALLPATH_HXX */
