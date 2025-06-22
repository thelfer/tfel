/*!
 * \file  mfront/include/MFront/Castem/CastemConfig.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 janv. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEM_CASTEMCONFIG_H_
#define LIB_MFRONT_CASTEM_CASTEMCONFIG_H_

#include "MFront/MFrontConfig.hxx"

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined CastemTimer_EXPORTS
#define MFRONT_CASTEMTIMER_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define MFRONT_CASTEMTIMER_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define MFRONT_CASTEMTIMER_VISIBILITY_EXPORT
#endif
#endif
#else
#define MFRONT_CASTEMTIMER_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_MFRONT_CASTEM_CASTEMCONFIG_H_ */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined CastemInterface_EXPORTS
#define MFRONT_CASTEM_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define MFRONT_CASTEM_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define MFRONT_CASTEM_VISIBILITY_EXPORT
#endif
#endif
#else
#define MFRONT_CASTEM_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_MFRONT_CASTEM_CASTEMCONFIG_H_ */

#endif /* LIB_MFRONT_CASTEM_CASTEMCONFIG_H_ */
