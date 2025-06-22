/*!
 * \file   InitNumpy.hxx
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

#ifndef LIB_TFEL_NUMPY_INITNUMPY_HXX
#define LIB_TFEL_NUMPY_INITNUMPY_HXX

#include "TFEL/Numpy/Config.hxx"

namespace tfel::numpy {

  /*!
   * \brief function to be called in modules using numpy
   */
  TFEL_NUMPY_EXPORT void initializeNumPy();

}  // end of namespace tfel::numpy

#endif /* LIB_TFEL_NUMPY_INITNUMPY_HXX */
