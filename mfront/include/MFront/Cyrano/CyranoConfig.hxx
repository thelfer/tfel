/*!
 * \file  mfront/include/MFront/Cyrano/CyranoConfig.hxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CYRANO_CYRANOCONFIG_H_
#define LIB_MFRONT_CYRANO_CYRANOCONFIG_H_

#include "MFront/MFrontConfig.hxx"

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined CyranoInterface_EXPORTS
#define MFRONT_CYRANO_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#if (!defined TFEL_STATIC_BUILD)
#define MFRONT_CYRANO_VISIBILITY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define MFRONT_CYRANO_VISIBILITY_EXPORT
#endif
#endif
#else
#define MFRONT_CYRANO_VISIBILITY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_MFRONT_CYRANO_CYRANOCONFIG_H_ */

#endif /* LIB_MFRONT_CYRANO_CYRANOCONFIG_H_ */
