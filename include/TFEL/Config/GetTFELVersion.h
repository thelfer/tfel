/*!
 * \file   GetTFELVersion.hxx
 * \brief    
 * \author HELFER Thomas 202608
 * \date   16 janv. 2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence with linking exception 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_CONFIG_GETTFELVERSION_HXX
#define LIB_TFEL_CONFIG_GETTFELVERSION_HXX

#include"TFEL/Config/TFELConfig.hxx"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
  
  TFELCONFIG_VISIBILITY_EXPORT const char* getTFELVersion();

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* LIB_TFEL_CONFIG_GETTFELVERSION_HXX */
