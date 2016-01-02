/*!
 * \file   GetInstallPath.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   21 déc. 2015
 */

#ifndef _LIB_TFEL_GETINSTALLPATH_HXX_
#define _LIB_TFEL_GETINSTALLPATH_HXX_

#include<string>
#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  TFELCONFIG_VISIBILITY_EXPORT std::string
  getInstallPath(void);
  
} // end of namespace tfel

#endif /* _LIB_TFEL_GETINSTALLPATH_HXX_ */
