/*! 
 * \file  CyranoConfig.hxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_MFRONT_CYRANO_CYRANOCONFIG_H_
#define _LIB_MFRONT_CYRANO_CYRANOCONFIG_H_ 

#include"TFEL/Config/TFELConfig.hxx"

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined CyranoInterface_EXPORTS
#    define MFRONT_CYRANO_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#  if not defined TFEL_STATIC_BUILD
#    define MFRONT_CYRANO_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  else 
#    define MFRONT_CYRANO_VISIBILITY_EXPORT 
#  endif
#  endif
#else
#  define MFRONT_CYRANO_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#endif /* _LIB_MFRONT_CYRANO_CYRANOCONFIG_H */

