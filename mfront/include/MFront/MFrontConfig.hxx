/*!
 * \file  mfront/include/MFront/MFrontConfig.hxx
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

#ifndef LIB_MFRONT_MFRONTCONFIG_H_
#define LIB_MFRONT_MFRONTCONFIG_H_

#include "TFEL/Config/TFELConfig.hxx"

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined MFrontProfiling_EXPORTS
#define MFRONTPROFILING_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define MFRONTPROFILING_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define MFRONTPROFILING_VISIBILITY_EXPORT
#endif
#endif
#else
#define MFRONTPROFILING_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_MFRONT_MFRONTCONFIG_H_ */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined MFrontLogStream_EXPORTS
#define MFRONTLOGSTREAM_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define MFRONTLOGSTREAM_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define MFRONTLOGSTREAM_VISIBILITY_EXPORT
#endif
#endif
#else
#define MFRONTLOGSTREAM_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_MFRONT_MFRONTCONFIG_H_ */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELMFront_EXPORTS
#define MFRONT_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define MFRONT_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define MFRONT_VISIBILITY_EXPORT
#endif
#endif
#else
#define MFRONT_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_MFRONT_MFRONTCONFIG_H_ */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined MTestFileGenerator_EXPORTS
#define MTESTFILEGENERATOR_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define MTESTFILEGENERATOR_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define MTESTFILEGENERATOR_VISIBILITY_EXPORT
#endif
#endif
#else
#define MTESTFILEGENERATOR_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_MFRONT_MFRONTCONFIG_H_ */

#endif /* LIB_MFRONT_MFRONTCONFIG_H_ */
