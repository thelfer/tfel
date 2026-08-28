/*!
 * \file   include/TFEL/Config/Substitutions.hxx
 * \brief
 * \author Thomas Helfer
 * \date   12/06/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_CONFIG_SUBSTITUTIONS_HXX
#define LIB_TFEL_CONFIG_SUBSTITUTIONS_HXX

#include <map>
#include <string>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::config {

  /*!
   * \brief return the default TFEL's substitutions
   *
   * Those substitutions are used by `MFront` when reading configuration files
   * and by `tfel-check` when analysing `.check` files.
   */
  TFELCONFIG_VISIBILITY_EXPORT std::map<std::string, std::string>
  getTFELDefaultSubstitutions();

}  // end of namespace tfel::config

#endif /* LIB_TFEL_CONFIG_SUBSTITUTIONS_HXX */
