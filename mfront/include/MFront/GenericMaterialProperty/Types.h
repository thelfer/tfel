/*!
 * \file   include/MFront/GenericMaterialProperty/Types.h
 * \brief
 * \author Thomas Helfer
 * \date   03/05/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with linking exception
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICMATERIALPROPERTY_TYPES_H
#define LIB_MFRONT_GENERICMATERIALPROPERTY_TYPES_H

#ifdef __cplusplus
#include <cstddef>
#else  /* __cplusplus */
#include <stddef.h>
#endif /* __cplusplus */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \brief numerical type used by the interface
 */
typedef double mfront_gmp_real;
/*!
 * \brief numerical type used by the interface
 */
typedef size_t mfront_gmp_size_type;

#ifdef __cplusplus
} // end of extern "C"
#endif /* __cplusplus */

#endif /* LIB_MFRONT_GENERICMATERIALPROPERTY_TYPES_H */
