/*!
 * \file  bindings/python/tfel/TestResult.cxx
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
#include "TFEL/Tests/TestResult.hxx"

void declareTestResult(pybind11::module_&);

void declareTestResult(pybind11::module_& m) {
  using namespace tfel::tests;
  pybind11::class_<TestResult>(m, "TestResult")
      .def(pybind11::init<>())
      .def("success", &TestResult::success)
      .def("duration", &TestResult::duration);

}  // end of declareExternalLibraryManager
