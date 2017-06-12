/*!
 * \file   NUMODIS/Config.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   08 juin 2017
 */

#ifndef LIB_NUMODIS_CONFIG_HXX
#define LIB_NUMODIS_CONFIG_HXX

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#define TFEL_VISIBILITY_IMPORT __declspec(dllimport)
#define TFEL_VISIBILITY_EXPORT __declspec(dllexport)
#define TFEL_VISIBILITY_LOCAL
#else /* defined _WIN32 || defined __CYGWIN__ */
#if (defined __GNUC__) && (! defined __INTEL_COMPILER)
#if __GNUC__ >= 4
#define TFEL_VISIBILITY_IMPORT __attribute__((visibility("default")))
#define TFEL_VISIBILITY_EXPORT __attribute__((visibility("default")))
#define TFEL_VISIBILITY_LOCAL  __attribute__((visibility("hidden")))
#else /*__GNUC__ >= 4 */
#define TFEL_VISIBILITY_IMPORT
#define TFEL_VISIBILITY_EXPORT
#define TFEL_VISIBILITY_LOCAL
#endif /* LIB_TFEL_CONFIG_HXX */
#elif defined __INTEL_COMPILER
#define TFEL_VISIBILITY_IMPORT __attribute__((visibility("default")))
#define TFEL_VISIBILITY_EXPORT __attribute__((visibility("default")))
#define TFEL_VISIBILITY_LOCAL  __attribute__((visibility("hidden")))
#else 
#define TFEL_VISIBILITY_IMPORT
#define TFEL_VISIBILITY_EXPORT
#define TFEL_VISIBILITY_LOCAL
#endif /* LIB_TFEL_CONFIG_HXX */
#endif /* LIB_TFEL_CONFIG_HXX */

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
