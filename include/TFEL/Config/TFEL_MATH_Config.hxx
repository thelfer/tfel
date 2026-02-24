/*!
 * \file   include/TFEL/Config/TFEL_MATH_Config.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   03 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef HAVE_FENV
#ifdef __cplusplus
#include <cfenv>
#endif /* __cplusplus */
#endif /* LIB_TFEL_MATH_CONFIG_HXX */

#ifndef LIB_TFEL_MATH_CONFIG_HXX
#define LIB_TFEL_MATH_CONFIG_HXX

#ifdef TFEL_MATH_USE_HARD_INLINING
#ifdef __GNUC__
#ifndef __INTEL_COMPILER
// Gnu Compiler
#define TFEL_MATH_INLINE __attribute__((__always_inline__))
#else
// Intel Compiler
#define TFEL_MATH_INLINE __forceinline
#endif /* LIB_TFEL_MATH_CONFIG_HXX */
#else
// Unknown Compiler
#define TFEL_MATH_INLINE inline
#endif /* LIB_TFEL_MATH_CONFIG_HXX */
#else
// TFEL_MATH_USE_HARD_INLINING is not defined
#define TFEL_MATH_INLINE inline
#endif /* LIB_TFEL_MATH_CONFIG_HXX */

#define TFEL_MATH_INLINE2 inline
#define TFEL_MATH_INLINE3

#endif /* LIB_TFEL_MATH_CONFIG_HXX */
