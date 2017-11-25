/*!
 * \file   include/TFEL/Config/GetInstallPath.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   21 déc. 2015
 */

#ifndef LIB_TFEL_GETINSTALLPATH_HXX
#define LIB_TFEL_GETINSTALLPATH_HXX

#include<string>
#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  TFELCONFIG_VISIBILITY_EXPORT std::string
  getInstallPath();

  TFELCONFIG_VISIBILITY_EXPORT std::string
  getTFELConfigExecutableName();

  TFELCONFIG_VISIBILITY_EXPORT std::string
  getLibraryInstallName(const std::string&);
  
} // end of namespace tfel

#endif /* LIB_TFEL_GETINSTALLPATH_HXX */
