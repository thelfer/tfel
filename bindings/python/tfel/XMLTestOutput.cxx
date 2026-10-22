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
  pybind11::class_<XMLTestOutput>(
      m, "XMLTestOutput",
      "Instances of this class reports tests' outputs in an\n"
      "XML file in the JUnit format")
      .def(pybind11::init<std::string>(),
           pybind11::arg("o"),
           "Constructor\n"
           "o: output file name")
      .def("beginTestSuite", &XMLTestOutput::beginTestSuite,
           pybind11::arg("n"),
           "Begin a new test suite\n"
           "n: name of the test suite")
      .def("endTestSuite", &XMLTestOutput::endTestSuite,
           pybind11::arg("r"),
           "End a test suite\n"
           "r: (global) result of the test suite")
      .def("addTest", &XMLTestOutput::addTest,
           pybind11::arg("g"), pybind11::arg("n"), pybind11::arg("r"),
           "Add a new test\n"
           "g: group of the test\n"
           "n: name of the test\n"
           "r: result of the test");

}  // end of declareXMLTestOutput
