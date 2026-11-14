/*!
 * \file   bindings/python/tfel/Config.cxx
 * \brief
 * \author Thomas Helfer
 * \date 02/04/2023
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of theses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Config/GetTFELVersion.h"

void declareConfig(pybind11::module_&);

void declareConfig(pybind11::module_& m) {
  m.def("getTFELVersion", ::getTFELVersion);
}  // end of declareConfig
