/*!
 * \file   bindings/python/tfel/Config.cxx
 * \brief
 * \author Thomas Helfer
 * \date   02/04/2023
 */

#include <string>
#include <pybind11/pybind11.h>
#include "TFEL/Config/GetTFELVersion.h"

void declareConfig(pybind11::module_&);

void declareConfig(pybind11::module_& m) {
  m.def("getTFELVersion", ::getTFELVersion);
}  // end of declareConfig
