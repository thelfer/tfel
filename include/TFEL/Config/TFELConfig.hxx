/*!
 * \file   include/TFEL/Config/TFELConfig.hxx
 * \brief  This file declares some useful typedef to control the level
 * of inlining used by the different parts of TFEL.
 * \author Thomas Helfer
 * \date   31 mai 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_CONFIG_HXX
#define LIB_TFEL_CONFIG_HXX

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the utilities namespace.
#ifndef TFEL_INLINE
#define TFEL_INLINE inline
#endif /* LIB_TFEL_CONFIG_HXX */

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the utilities namespace.
#ifndef TFEL_INLINE2
#define TFEL_INLINE2 inline
#endif /* LIB_TFEL_CONFIG_HXX */

#include "TFEL/Config/TFEL_MATH_Config.hxx"

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the fsalgorithm namespace.
#ifndef TFEL_FSALGORITHM_INLINE
#define TFEL_FSALGORITHM_INLINE inline
#endif /* LIB_TFEL_CONFIG_HXX */

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the material namespace.
#ifndef TFEL_MATERIAL_INLINE
#define TFEL_MATERIAL_INLINE inline
#endif /* LIB_TFEL_CONFIG_HXX */

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the materiallaw namespace.
#ifndef TFEL_MATERIAL_LAW_INLINE2
#define TFEL_MATERIAL_LAW_INLINE2 inline
#endif /* LIB_TFEL_CONFIG_HXX */

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the fe namespace.
#ifndef TFEL_FE_INLINE
#define TFEL_FE_INLINE inline
#endif /* LIB_TFEL_CONFIG_HXX */

#if (defined __GNUC__) && (!defined __INTEL_COMPILER)
#if __GNUC__ >= 4
#define TFEL_MAY_ALIAS_ATTRIBUTE __attribute__((__may_alias__))
#else /* __GNUC__ >= 4 */
#define TFEL_MAY_ALIAS_ATTRIBUTE
#endif /* LIB_TFEL_CONFIG_HXX */
#else  /* (defined __GNUC__) && (! defined __INTEL_COMPILER) */
#define TFEL_MAY_ALIAS_ATTRIBUTE
#endif /* LIB_TFEL_CONFIG_HXX */

#if (defined __GNUC__) && (!defined __INTEL_COMPILER)
#if __GNUC__ >= 4
#define TFEL_UNUSED_ATTRIBUTE __attribute__((__unused__))
#else /* __GNUC__ >= 4 */
#define TFEL_UNUSED_ATTRIBUTE
#endif /* LIB_TFEL_CONFIG_HXX */
#else  /* (defined __GNUC__) && (! defined __INTEL_COMPILER) */
#define TFEL_UNUSED_ATTRIBUTE
#endif /* LIB_TFEL_CONFIG_HXX */

/*!
 * \def TFEL_VISIBILITY_IMPORT
 *
 * On windows, this macro is used to declare a function or a class as
 * imported in the dynamic loadable library (dll) sense.
 *
 * On other plateforms, this macros affects the default visibility
 * attribute to a function or a class.
 *
 * Macro extracted from :
 * "Why is the new C++ visibility support so useful?"
 * from http://gcc.gnu.org/wiki/Visibility
 */
/*!
 * \def TFEL_VISIBILITY_EXPORT
 *
 * On Windows, this macro is used to declare a function or a class as
 * exported in the dynamic loadable library (dll) sense.
 *
 * On other plateforms, this macros affects the default visibility
 * attribute to a function or a class.
 *
 * Macro extracted from :
 * "Why is the new C++ visibility support so useful?"
 * from http://gcc.gnu.org/wiki/Visibility
 */
/*!
 * \def TFEL_VISIBILITY_LOCAL
 *
 * On Windows, this macro has no effect.
 *
 * On other plateforms, this macros affects the hidden visibility
 * attribute to a function or a class.
 *
 * Macro extracted from :
 * "Why is the new C++ visibility support so useful?"
 * from http://gcc.gnu.org/wiki/Visibility
 */

/*!
 * Macro extracted from :
 * "Why is the new C++ visibility support so useful?"
 * from http://gcc.gnu.org/wiki/Visibility
 */
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#define TFEL_VISIBILITY_IMPORT __declspec(dllimport)
#define TFEL_VISIBILITY_EXPORT __declspec(dllexport)
#define TFEL_VISIBILITY_LOCAL
#else /* defined _WIN32 || defined __CYGWIN__ */
#if (defined __GNUC__) && (!defined __INTEL_COMPILER)
#if __GNUC__ >= 4
#define TFEL_VISIBILITY_IMPORT __attribute__((visibility("default")))
#define TFEL_VISIBILITY_EXPORT __attribute__((visibility("default")))
#define TFEL_VISIBILITY_LOCAL __attribute__((visibility("hidden")))
#else /*__GNUC__ >= 4 */
#define TFEL_VISIBILITY_IMPORT
#define TFEL_VISIBILITY_EXPORT
#define TFEL_VISIBILITY_LOCAL
#endif /* LIB_TFEL_CONFIG_HXX */
#elif defined __INTEL_COMPILER
#define TFEL_VISIBILITY_IMPORT __attribute__((visibility("default")))
#define TFEL_VISIBILITY_EXPORT __attribute__((visibility("default")))
#define TFEL_VISIBILITY_LOCAL __attribute__((visibility("hidden")))
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

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELConfig_EXPORTS
#define TFELCONFIG_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFELCONFIG_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFELCONFIG_VISIBILITY_EXPORT
#endif
#endif
#else
#define TFELCONFIG_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_CONFIG_HXX */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELPhysicalConstants_EXPORTS
#define TFELPHYSICALCONSTANTS_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFELPHYSICALCONSTANTS_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFELPHYSICALCONSTANTS_VISIBILITY_EXPORT
#endif
#endif
#else
#define TFELPHYSICALCONSTANTS_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_PHYSICALCONSTANTS_HXX */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELException_EXPORTS
#define TFELEXCEPTION_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFELEXCEPTION_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFELEXCEPTION_VISIBILITY_EXPORT
#endif
#endif
#else
#define TFELEXCEPTION_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_CONFIG_HXX */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELTests_EXPORTS
#define TFELTESTS_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFELTESTS_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFELTESTS_VISIBILITY_EXPORT
#endif
#endif
#else
#define TFELTESTS_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_CONFIG_HXX */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELUtilities_EXPORTS
#define TFELUTILITIES_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFELUTILITIES_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFELUTILITIES_VISIBILITY_EXPORT
#endif
#endif
#else
#define TFELUTILITIES_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_CONFIG_HXX */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELUnicodeSupport_EXPORTS
#define TFELUNICODESUPPORT_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFELUNICODESUPPORT_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFELUNICODESUPPORT_VISIBILITY_EXPORT
#endif
#endif
#else
#define TFELUNICODESUPPORT_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_CONFIG_HXX */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELGlossary_EXPORTS
#define TFELGLOSSARY_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFELGLOSSARY_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFELGLOSSARY_VISIBILITY_EXPORT
#endif
#endif
#else
#define TFELGLOSSARY_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_CONFIG_HXX */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELSystem_EXPORTS
#define TFELSYSTEM_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFELSYSTEM_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFELSYSTEM_VISIBILITY_EXPORT
#endif
#endif
#else
#define TFELSYSTEM_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_CONFIG_HXX */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELMath_EXPORTS
#define TFELMATH_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFELMATH_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFELMATH_VISIBILITY_EXPORT
#endif
#endif
#else
#define TFELMATH_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_CONFIG_HXX */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELMathCubicSpline_EXPORTS
#define TFELMATHCUBICSPLINE_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFELMATHCUBICSPLINE_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFELMATHCUBICSPLINE_VISIBILITY_EXPORT
#endif
#endif
#else
#define TFELMATHCUBICSPLINE_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_CONFIG_HXX */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELMathKriging_EXPORTS
#define TFELMATHKRIGING_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFELMATHKRIGING_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFELMATHKRIGING_VISIBILITY_EXPORT
#endif
#endif
#else
#define TFELMATHKRIGING_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_CONFIG_HXX */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELMathParser_EXPORTS
#define TFELMATHPARSER_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFELMATHPARSER_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFELMATHPARSER_VISIBILITY_EXPORT
#endif
#endif
#else
#define TFELMATHPARSER_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_CONFIG_HXX */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELMaterial_EXPORTS
#define TFELMATERIAL_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFELMATERIAL_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFELMATERIAL_VISIBILITY_EXPORT
#endif
#endif
#else
#define TFELMATERIAL_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_TFEL_CONFIG_HXX */

#ifdef TFEL_HAVE_NORETURN_ATTRIBUTE
#define TFEL_NORETURN [[noreturn]]
#else
#define TFEL_NORETURN
#endif /* TFEL_HAVE_NORETURN_ATTRIBUTE */

// mitigate infamous windows macros effects
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#if defined(__GNUG__) && (!defined(__CLANG__)) && (!defined(__INTEL_COMPILER))
#if ((__GNUC__ > 4) ||                            \
     ((__GNUC__ == 4) && ((__GNUC_MINOR__ > 9) || \
                          (__GNUC_MINOR__ == 9 && __GNUC_PATCHLEVEL__ > 0))))
#define TFEL_CONSTEXPR constexpr
#else
#define TFEL_CONSTEXPR
#endif
#elif defined(_MSC_VER)
#define TFEL_CONSTEXPR
#else
#define TFEL_CONSTEXPR constexpr
#endif

#endif /* LIB_TFEL_CONFIG_HXX */
