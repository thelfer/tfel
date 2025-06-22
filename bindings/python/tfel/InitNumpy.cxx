/*!
 * \file   InitNumpy.cxx
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

#include <boost/python/numpy.hpp>
#include "TFEL/Numpy/InitNumpy.hxx"

namespace tfel::numpy {

  void initializeNumPy() {
    Py_Initialize();
    boost::python::numpy::initialize();
  }  // end of initializeNumPy

}  // end of namespace tfel::numpy
