/*!
 * \file   PipeMesh.cxx
 * \brief
 * \author Thomas Helfer
 * \date   12 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MTest/PipeMesh.hxx"

void declarePipeMesh(pybind11::module_&);

void declarePipeMesh(pybind11::module_& m) {
  pybind11::enum_<mtest::PipeMesh::ElementType>(m, "PipeMeshElementType")
      .value("DEFAULT", mtest::PipeMesh::DEFAULT)
      .value("LINEAR", mtest::PipeMesh::LINEAR)
      .value("QUADRATIC", mtest::PipeMesh::QUADRATIC)
      .value("CUBIC", mtest::PipeMesh::CUBIC);

  pybind11::class_<mtest::PipeMesh>(m, "PipeMesh")
      .def(pybind11::init<>())
      .def_readonly("inner_radius", &mtest::PipeMesh::inner_radius)
      .def_readonly("outer_radius", &mtest::PipeMesh::outer_radius)
      .def_readonly("number_of_elements", &mtest::PipeMesh::number_of_elements)
      .def_readonly("etype", &mtest::PipeMesh::etype);
}
