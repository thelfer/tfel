/*!
 * \file   ndarray.cxx
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

#include <numpy/ndarrayobject.h>
#include <boost/python/numpy.hpp>
#include "TFEL/Raise.hxx"
#include "TFEL/Numpy/ndarray.hxx"

namespace tfel::numpy {

  size_t get_size(const boost::python::numpy::ndarray& a) {
    const auto nd = a.get_nd();
    if (nd != 1) {
      tfel::raise("get_size: one dimensional array expected");
    }
    return static_cast<size_t>(a.shape(0));
  }  // end of get_size

  const double* get_data(const boost::python::numpy::ndarray& a) {
    if (a.get_dtype() != boost::python::numpy::dtype::get_builtin<double>()) {
      tfel::raise("get_data: invalid data type");
    }
    return reinterpret_cast<const double*>(a.get_data());
  }  // end of get_data

  double* get_data(boost::python::numpy::ndarray& a) {
    if (a.get_dtype() != boost::python::numpy::dtype::get_builtin<double>()) {
      tfel::raise("get_data: invalid data type");
    }
    return reinterpret_cast<double*>(a.get_data());
  }  // end of get_data

}  // end of namespace tfel::numpy
