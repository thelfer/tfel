/*!
 * \file  mtest/include/MTest/Config.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 25 juin 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTEST_MTESTCONFIG_H_
#define LIB_MTEST_MTEST_MTESTCONFIG_H_

#include "TFEL/Config/TFELConfig.hxx"

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELMTest_EXPORTS
#define MTEST_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define MTEST_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define MTEST_VISIBILITY_EXPORT
#endif
#endif
#else
#define MTEST_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_MTEST_MTEST_MTESTCONFIG_H_ */

#endif /* LIB_MTEST_MTEST_MTESTCONFIG_H_ */
