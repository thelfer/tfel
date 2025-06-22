/*!
 * \file  mfront/include/MFront/InstallPath.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 06/03/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_INSTALLPATH_HXX
#define LIB_MFRONT_INSTALLPATH_HXX

#include <string>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * \brief set the installation path
   * \param[in] p: installation path
   * \note an exception is thrown if the installation path has already been set.
   */
  MFRONT_VISIBILITY_EXPORT void setInstallPath(const std::string&);
  /*!
   * \return the installation path, if defined
   * \note if no installation path has been specified, one try to retrieve this
   * path from the `MFRONT_INSTALL_PREFIX` environment variable.
   */
  MFRONT_VISIBILITY_EXPORT std::string getInstallPath();

}  // namespace mfront

#endif /* LIB_MFRONT_INSTALLPATH_HXX */
