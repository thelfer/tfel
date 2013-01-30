/*! 
 * \file  AsterConfig.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 janv. 2013
 */

#ifndef _LIB_MFRONT_ASTER_ASTERCONFIG_H_
#define _LIB_MFRONT_ASTER_ASTERCONFIG_H_ 

#include"TFEL/Config/TFELConfig.hxx"

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined ASTERInterface_EXPORTS
#    define MFRONT_ASTER_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#    define MFRONT_ASTER_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  endif
#else
#  define MFRONT_ASTER_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#endif /* _LIB_MFRONT_ASTER_ASTERCONFIG_H */

