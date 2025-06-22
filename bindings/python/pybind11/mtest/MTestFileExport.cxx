/*!
 * \file  bindings/python/mtest/MTestFileExport.cxx
 * \brief
 * \author Thomas Helfer
 * \date   01/03/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MTest/MTestFileExport.hxx"

static void mtest_write(const mtest::TestDescription& m, const std::string& f) {
  mtest::write(m, f);
}  // end of mtest_write

static void mtest_loadMTestFileContent(mtest::TestDescription& d,
                                       const std::string& f) {
  mtest::loadMTestFileContent(d, f);
}  // end of mtest_loadMTestFileContent

void declareMTestFileExport(pybind11::module_&);

void declareMTestFileExport(pybind11::module_& m) {
  //
  pybind11::class_<mtest::TestDescription>(m, "TestDescription")
      .def(pybind11::init<>())
      .def_readwrite("name", &mtest::TestDescription::name)
      .def_readwrite("scheme", &mtest::TestDescription::scheme)
      .def_readwrite("author", &mtest::TestDescription::author)
      .def_readwrite("date", &mtest::TestDescription::date)
      .def_readwrite("description", &mtest::TestDescription::description)
      .def_readwrite("behaviour", &mtest::TestDescription::behaviour)
      .def_readwrite("material", &mtest::TestDescription::material);
  //
  m.def("loadMTestFileContent", mtest_loadMTestFileContent);
  m.def("write", mtest_write);

}  // end of declareMTestFileExport
