/*!
 * \file   mfront/include/MFront/GenericParallel/BackendExecutionPolicy.h
 * \brief  This file declares the type of the execution policies seen by the
 * `C` structures of the `generic-parallel` interfaces: the base class of the
 * policies in `C++` (see
 * `MFront/GenericParallel/AbstractBackendExecutionPolicy.hxx`), an opaque
 * structure in `C`, which can only be given as a null pointer (default
 * policy of the backend).
 * \date   22/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICPARALLEL_BACKENDEXECUTIONPOLICY_H
#define LIB_MFRONT_GENERICPARALLEL_BACKENDEXECUTIONPOLICY_H

// NOLINTBEGIN(modernize-use-using)
#ifdef __cplusplus

#include "MFront/GenericParallel/AbstractBackendExecutionPolicy.hxx"

//! \brief a simple alias
typedef mfront::generic_parallel::AbstractBackendExecutionPolicy
    mfront_gp_AbstractBackendExecutionPolicy;

#else /* __cplusplus */

/*!
 * \brief opaque structure: the execution policies are created by `C++`
 * codes only
 */
typedef struct mfront_gp_AbstractBackendExecutionPolicy
    mfront_gp_AbstractBackendExecutionPolicy;

#endif /* __cplusplus */
// NOLINTEND(modernize-use-using)

#endif /* LIB_MFRONT_GENERICPARALLEL_BACKENDEXECUTIONPOLICY_H */
