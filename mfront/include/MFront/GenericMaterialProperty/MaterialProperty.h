/*!
 * \file   include/GenericMaterialProperty/MaterialProperty.h
 * \brief
 * \author Thomas/Helfer
 * \date   03/05/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with linking exception
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_GENERIC_MATERIALPROPERTY_MATERIALPROPERTY_H
#define LIB_GENERIC_MATERIALPROPERTY_MATERIALPROPERTY_H

#include "MFront/GenericMaterialProperty/Types.h"
#include "MFront/GenericMaterialProperty/OutputStatus.h"
#include "MFront/GenericMaterialProperty/OutOfBoundsPolicy.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#define GENERIC_MATERIALPROPERTY_ADDCALL_PTR __cdecl*
#else
#define GENERIC_MATERIALPROPERTY_ADDCALL_PTR *
#endif

//! \brief a simple alias
typedef mfront_gmp_real(
    GENERIC_MATERIALPROPERTY_ADDCALL_PTR mfront_gmp_MaterialPropertyPtr)(
    mfront_gmp_OutputStatus* const,       // output status
    const mfront_gmp_real* const,         // arguments
    const mfront_gmp_size_type,           // number of arguments
    const mfront_gmp_OutOfBoundsPolicy);  // out of bounds policy

#ifdef __cplusplus
}  // end of extern "C"
#endif /* __cplusplus */

#endif /* LIB_GENERIC_MATERIALPROPERTY_MATERIALPROPERTY_H */
