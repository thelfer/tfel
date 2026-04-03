/*!
 * \file   bindings/python/mfront/DSLFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   03 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/DSLFactory.hxx"

static auto createNewDSL(mfront::DSLFactory& f, const std::string& n) {
  return f.createNewDSL(n, {});
}

void declareDSLFactory(pybind11::module_&);

void declareDSLFactory(pybind11::module_& m) {
  using namespace mfront;
  pybind11::class_<DSLFactory>(m, "DSLFactory")
      .def_static("getDSLFactory", DSLFactory::getDSLFactory,
                  pybind11::return_value_policy::reference)
      .def("createNewParser", &createNewDSL)
      .def("createNewDSL", &createNewDSL)
      .def("createNewParser", &DSLFactory::createNewDSL)
      .def("createNewDSL", &DSLFactory::createNewDSL);
}
