/*!
 * \file  bindings/python/tfel/XMLTestOutput.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 20 sept. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Tests/XMLTestOutput.hxx"

void declareXMLTestOutput(pybind11::module_&);

void declareXMLTestOutput(pybind11::module_& m) {
  using namespace tfel::tests;
  pybind11::class_<XMLTestOutput>(m, "XMLTestOutput")
      .def(pybind11::init<std::string>())
      .def("beginTestSuite", &XMLTestOutput::beginTestSuite)
      .def("endTestSuite", &XMLTestOutput::endTestSuite)
      .def("addTest", &XMLTestOutput::addTest);

}  // end of declareExternalLibraryManager
