/*! 
 * \file  MFrontConfig.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 janv. 2013
 */

#ifndef _LIB_MFRONT_MFRONTCONFIG_H_
#define _LIB_MFRONT_MFRONTCONFIG_H_ 

#include"TFEL/Config/TFELConfig.hxx"

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined MTestFileGenerator_EXPORTS
#    define MTESTFILEGENERATOR_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#  if not defined TFEL_STATIC_BUILD
#    define MTESTFILEGENERATOR_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  else 
#    define MTESTFILEGENERATOR_VISIBILITY_EXPORT 
#  endif
#  endif
#else
#  define MTESTFILEGENERATOR_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#endif /* _LIB_MFRONT_MFRONTCONFIG_H */

