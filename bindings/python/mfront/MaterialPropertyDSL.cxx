/*!
 * \file   bindings/python/mfront/MaterialPropertyDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09/08/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <set>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MaterialPropertyDSL.hxx"

void declareMaterialPropertyDSL(pybind11::module_&);

void declareMaterialPropertyDSL(pybind11::module_& m) {
  using namespace mfront;
  pybind11::class_<MaterialPropertyDSL, AbstractDSL,
                   std::shared_ptr<MaterialPropertyDSL>>(m,
                                                         "MaterialPropertyDSL")
      .def("getMaterialPropertyDescription",
           &MaterialPropertyDSL::getMaterialPropertyDescription,
           pybind11::return_value_policy::reference);
}
