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

#include <boost/python.hpp>
#include "MTest/MTestFileExport.hxx"

static void mtest_write(const mtest::TestDescription& m, const std::string& f) {
  mtest::write(m, f);
}  // end of mtest_write

static void mtest_loadMTestFileContent(mtest::TestDescription& d,
                                       const std::string& f) {
  mtest::loadMTestFileContent(d, f);
}  // end of mtest_loadMTestFileContent

void declareMTestFileExport() {
  //
  boost::python::class_<mtest::TestDescription>("TestDescription")
      .def_readwrite("name", &mtest::TestDescription::name)
      .def_readwrite("scheme", &mtest::TestDescription::scheme)
      .def_readwrite("author", &mtest::TestDescription::author)
      .def_readwrite("date", &mtest::TestDescription::date)
      .def_readwrite("description", &mtest::TestDescription::description)
      .def_readwrite("behaviour", &mtest::TestDescription::behaviour)
      .def_readwrite("material", &mtest::TestDescription::material);
  //
  boost::python::def("loadMTestFileContent", mtest_loadMTestFileContent);
  boost::python::def("write", mtest_write);

}  // end of declareMTestFileExport