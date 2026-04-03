/*!
 * \file   include/Cyrano/MaterialProperty.hxx
 * \brief
 * \author Thomas/Helfer
 * \date   14/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_CYRANO_MATERIALPROPERTY_HXX
#define LIB_CYRANO_MATERIALPROPERTY_HXX

#include "Cyrano/Types.hxx"
#include "Cyrano/OutputStatus.hxx"
#include "Cyrano/OutOfBoundsPolicy.hxx"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//! a simple alias
typedef CyranoRealType(CYRANO_ADDCALL_PTR CyranoMaterialPropertyPtr)(
    CyranoOutputStatus* const,       // output status
    const CyranoRealType* const,     // arguments
    const CyranoIntegerType,         // number of arguments
    const CyranoOutOfBoundsPolicy);  // out of bounds policy

#ifdef __cplusplus
}  // end of extern "C"
#endif /* __cplusplus */

#endif /* LIB_CYRANO_MATERIALPROPERTY_HXX */
