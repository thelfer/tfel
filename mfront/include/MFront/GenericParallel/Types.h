/*!
 * \file   mfront/include/MFront/GenericParallel/Types.h
 * \brief  This file declares the types shared by the `C` structures of the
 * `generic-parallel` interfaces.
 * \date   10/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_TYPES_H
#define LIB_MFRONT_GENERICPARALLEL_TYPES_H

// NOLINTBEGIN(modernize-use-using)
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * \brief unsigned integer type of the values updated by atomic operations
 * in the kernels. `CUDA` only provides the 64 bits `atomicAdd` and
 * `atomicMin` functions for `unsigned long long`: `uint64_t` and `size_t`,
 * which are `unsigned long` on `Linux`, are refused.
 */
typedef unsigned long long mfront_gp_atomic_size_type;

#ifdef __cplusplus
}  // end of extern "C"
#endif /* __cplusplus */
// NOLINTEND(modernize-use-using)

#endif /* LIB_MFRONT_GENERICPARALLEL_TYPES_H */
