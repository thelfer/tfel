/*!
 * \file   PipeMesh.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   12 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "MTest/PipeMesh.hxx"

void declarePipeMesh();

void declarePipeMesh() {
  boost::python::enum_<mtest::PipeMesh::ElementType>("PipeMeshElementType")
      .value("DEFAULT", mtest::PipeMesh::DEFAULT)
      .value("LINEAR", mtest::PipeMesh::LINEAR)
      .value("QUADRATIC", mtest::PipeMesh::QUADRATIC)
      .value("CUBIC", mtest::PipeMesh::CUBIC);

  boost::python::class_<mtest::PipeMesh, boost::noncopyable>("PipeMesh")
      .def_readonly("inner_radius", &mtest::PipeMesh::inner_radius)
      .def_readonly("outer_radius", &mtest::PipeMesh::outer_radius)
      .def_readonly("number_of_elements", &mtest::PipeMesh::number_of_elements)
      .def_readonly("etype", &mtest::PipeMesh::etype);
}
