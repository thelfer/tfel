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
#if defined _WIN32 || defined __CYGWIN__
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
#endif /* defined _WIN32 || defined __CYGWIN__ */

#endif /* _LIB_TFEL_CONFIG_HXX */
