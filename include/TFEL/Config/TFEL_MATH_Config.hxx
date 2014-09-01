/*!
 * \file   include/TFEL/Config/TFEL_MATH_Config.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   03 Jul 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef HAVE_FENV
#include "fenv.h"
#endif /* HAVE_FENV */

#ifndef _LIB_TFEL_MATH_CONFIG_HXX_
#define _LIB_TFEL_MATH_CONFIG_HXX_ 

#ifdef TFEL_MATH_USE_HARD_INLINING
#ifdef __GNUC__
#ifndef __INTEL_COMPILER
// Gnu Compiler
#define TFEL_MATH_INLINE __attribute__ ((__always_inline__))
#else
// Intel Compiler
#define TFEL_MATH_INLINE __forceinline
#endif
#else
// Unknown Compiler
#define TFEL_MATH_INLINE inline  
#endif
#else
// TFEL_MATH_USE_HARD_INLINING is not defined
#define TFEL_MATH_INLINE inline  
#endif

#define TFEL_MATH_INLINE2 inline  
#define TFEL_MATH_INLINE3

#endif /* _LIB_TFEL_MATH_CONFIG_HXX */

