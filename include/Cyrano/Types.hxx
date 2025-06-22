/*!
 * \file   Types.hxx
 * \brief
 * \author Thomas Helfer
 * \date   14/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_CYRANO_TYPES_HXX
#define LIB_CYRANO_TYPES_HXX

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#define CYRANO_ADDCALL_PTR __cdecl*
#else
#define CYRANO_ADDCALL_PTR *
#endif

#ifndef CYRANO_ARCH
#if defined TFEL_ARCH64
#define CYRANO_ARCH 64
#elif defined TFEL_ARCH32
#define CYRANO_ARCH 32
#else
#error \
    "unable to define an architecture for Cyrano, neither CYRANO_ARCH nor TFEL_ARCH32 or TFEL_ARCH64 are defined"
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if CYRANO_ARCH == 64
#ifdef _WIN64
typedef long long CyranoIntegerType;
#else
typedef long CyranoIntegerType;
#endif /* LIB_TFEL_CYRANO_MFRONT_CYRANO_HXX */
#elif CYRANO_ARCH == 32
typedef int CyranoIntegerType;
#else
#error "cyrano : unsupported machine type"
#endif /* LIB_TFEL_CYRANO_MFRONT_CYRANO_HXX */

//! a simple alias
typedef double CyranoRealType;

#ifdef __cplusplus
}  // end of extern "C"
#endif /* __cplusplus */

#endif /* LIB_CYRANO_TYPES_HXX */
