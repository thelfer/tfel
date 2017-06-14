/*!
 * \file   NUMODIS/Config.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   08 juin 2017
 */

#ifndef LIB_NUMODIS_CONFIG_HXX
#define LIB_NUMODIS_CONFIG_HXX

#include"TFEL/Config/TFELConfig.hxx"

/*
 * Defining macros by librairies using cmake conventions.
 * When compiling libfoo, cmake defines the macro foo_EXPORTS.
 * This allows to distinguish import/export definitions of symbols
 */

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined TFELNUMODIS_EXPORTS
#    define TFELNUMODIS_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#  ifndef TFEL_STATIC_BUILD
#    define TFELNUMODIS_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  else 
#    define TFELNUMODIS_VISIBILITY_EXPORT 
#  endif
#  endif
#else
#  define TFELNUMODIS_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_NUMODIS_HXX */

#endif /* LIB_NUMODIS_CONFIG_HXX */
