/*!
 * \file   bindings/python/include/TFEL/Numpy/Config.hxx
 * \brief
 * \author Thomas Helfer
 * \date   23/03/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_NUMPY_CONFIG_HXX
#define LIB_TFEL_NUMPY_CONFIG_HXX

#include "TFEL/Config/TFELConfig.hxx"

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#if defined TFELNumpySupport_EXPORTS
#define TFEL_NUMPY_EXPORT TFEL_VISIBILITY_EXPORT
#else
#ifndef TFEL_STATIC_BUILD
#define TFEL_NUMPY_EXPORT TFEL_VISIBILITY_IMPORT
#else
#define TFEL_NUMPY_EXPORT
#endif
#endif
#else
#define TFEL_NUMPY_EXPORT TFEL_VISIBILITY_EXPORT
#endif /* LIB_MTEST_MTEST_MTESTCONFIG_HXX */

#endif /* LIB_TFEL_NUMPY_CONFIG_HXX */
