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

#include "Config/TFEL_MATH_Config.hxx"

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the fsalgorithm namespace.
#ifndef TFEL_FSALGORITHM_INLINE
#define TFEL_FSALGORITHM_INLINE    inline
#endif /* TFEL_FSALGORITHM_INLINE */

//! a usefull macro for defining levels of inlining. This is
//  mainly used in the materiallaw namespace.
#ifndef TFEL_MATERIAL_LAW_INLINE
#define TFEL_MATERIAL_LAW_INLINE   inline  
#endif /* TFEL_MATERIAL_LAW_INLINE */

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

#endif /* _LIB_TFEL_CONFIG_HXX */
