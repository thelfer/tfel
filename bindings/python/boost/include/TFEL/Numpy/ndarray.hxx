/*!
 * \file   bindings/python/include/TFEL/Numpy/ndarray.hxx
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

#ifndef LIB_TFEL_NUMPY_NDARRAY_HXX
#define LIB_TFEL_NUMPY_NDARRAY_HXX

#include <boost/python/numpy.hpp>
#include "TFEL/Numpy/Config.hxx"

namespace tfel::numpy {

  TFEL_NUMPY_EXPORT size_t get_size(const boost::python::numpy::ndarray&);

  TFEL_NUMPY_EXPORT const double* get_data(
      const boost::python::numpy::ndarray&);

  TFEL_NUMPY_EXPORT double* get_data(boost::python::numpy::ndarray&);

}  // end of namespace tfel::numpy

#endif /* LIB_TFEL_NUMPY_NDARRAY_HXX */
