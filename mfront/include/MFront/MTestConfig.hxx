/*! 
 * \file  mfront/include/MFront/MTestConfig.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 25 juin 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MTEST_MTESTCONFIG_H_
#define _LIB_MFRONT_MTEST_MTESTCONFIG_H_ 

#include"TFEL/Config/TFELConfig.hxx"

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined TFELMTest_EXPORTS
#    define MFRONT_MTEST_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#  if not defined TFEL_STATIC_BUILD
#    define MFRONT_MTEST_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  else 
#    define MFRONT_MTEST_VISIBILITY_EXPORT
#  endif
#  endif
#else
#  define MFRONT_MTEST_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#endif /* _LIB_MFRONT_MTEST_MTESTCONFIG_H */

