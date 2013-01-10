/*!
 * \file   TFELConfig.hxx
 * \brief  This file declares some useful typedef to control the level
 * of inlining used by the different parts of TFEL.
 * \author Helfer Thomas
 * \date   31 mai 2006
 */

#ifndef _LIB_TFEL_CONFIG_HXX_
#define _LIB_TFEL_CONFIG_HXX_ 

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the utilities namespace.
#ifndef TFEL_INLINE
#define TFEL_INLINE  inline  
#endif /* TFEL_INLINE */

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the utilities namespace.
#ifndef TFEL_INLINE2
#define TFEL_INLINE2 inline  
#endif /* TFEL_INLINEZ */

#include"TFEL/Config/TFEL_MATH_Config.hxx"

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the fsalgorithm namespace.
#ifndef TFEL_FSALGORITHM_INLINE
#define TFEL_FSALGORITHM_INLINE    inline
#endif /* TFEL_FSALGORITHM_INLINE */

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the material namespace.
#ifndef TFEL_MATERIAL_INLINE
#define TFEL_MATERIAL_INLINE   inline  
#endif /* TFEL_MATERIAL_INLINE */

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the materiallaw namespace.
#ifndef TFEL_MATERIAL_LAW_INLINE2
#define TFEL_MATERIAL_LAW_INLINE2  inline  
#endif /* TFEL_MATERIAL_LAW_INLINE2 */

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the fe namespace.
#ifndef TFEL_FE_INLINE
#define TFEL_FE_INLINE             inline  
#endif /* TFEL_FE_INLINE */

/*!
 * Macro extracted from :
 * "Why is the new C++ visibility support so useful?"
 * from http://gcc.gnu.org/wiki/Visibility
 */
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
#endif /* __GNUC__ >= 4 */
#elif defined __INTEL_COMPILER
#define TFEL_VISIBILITY_IMPORT __attribute__((visibility("default")))
#define TFEL_VISIBILITY_EXPORT __attribute__((visibility("default")))
#define TFEL_VISIBILITY_LOCAL  __attribute__((visibility("hidden")))
#else 
#define TFEL_VISIBILITY_IMPORT
#define TFEL_VISIBILITY_EXPORT
#define TFEL_VISIBILITY_LOCAL
#endif 
#endif /* defined _WIN32 || _WIN64 || defined __CYGWIN__ */

/*!
 * defining macros by librairies using cmake conventions.
 * When compiling libfoo, cmake defines the macro foo_EXPORTS.
 * This allows to distinguish import/export definitions of symbols
 */
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined TFELException_EXPORTS
#    define TFELEXCEPTION_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#    define TFELEXCEPTION_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  endif
#else
#  define TFELEXCEPTION_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined TFELTests_EXPORTS
#    define TFELTESTS_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#    define TFELTESTS_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  endif
#else
#  define TFELTESTS_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined TFELUtilities_EXPORTS
#    define TFELUTILITIES_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#    define TFELUTILITIES_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  endif
#else
#  define TFELUTILITIES_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined TFELSystem_EXPORTS
#    define TFELSYSTEM_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#    define TFELSYSTEM_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  endif
#else
#  define TFELSYSTEM_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined TFELMath_EXPORTS
#    define TFELMATH_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#    define TFELMATH_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  endif
#else
#  define TFELMATH_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined TFELMathCubicSpline_EXPORTS
#    define TFELMATHCUBICSPLINE_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#    define TFELMATHCUBICSPLINE_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  endif
#else
#  define TFELMATHCUBICSPLINE_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined TFELMathKriging_EXPORTS
#    define TFELMATHKRIGING_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#    define TFELMATHKRIGING_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  endif
#else
#  define TFELMATHKRIGING_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined TFELMathParser_EXPORTS
#    define TFELMATHPARSER_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#    define TFELMATHPARSER_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  endif
#else
#  define TFELMATHPARSER_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#  if defined TFELMaterial_EXPORTS
#    define TFELMATERIAL_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#  else
#    define TFELMATERIAL_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#  endif
#else
#  define TFELMATERIAL_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif

#endif /* _LIB_TFEL_CONFIG_HXX */
