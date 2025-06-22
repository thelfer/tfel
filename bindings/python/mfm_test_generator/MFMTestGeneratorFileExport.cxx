/*!
 * \file  bindings/python/mfm_test_generator/MFMTestGeneratorFileExport.cxx
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
#include "MFMTestGenerator/MFMTestGeneratorFileExport.hxx"

static void mfmtg_write(const mfmtg::TestDescription& m, const std::string& f) {
  mfmtg::write(m, f);
}  // end of mfmtg_write

static void mfmtg_loadMFMTestGeneratorFileContent(mfmtg::TestDescription& d,
                                                  const std::string& f) {
  mfmtg::loadMFMTestGeneratorFileContent(d, f);
}  // end of mfmtg_loadMFMTestGeneratorFileContent

void declareMFMTestGeneratorFileExport() {
  //
  boost::python::class_<mfmtg::TestDescription>("TestDescription")
      .def_readwrite("name", &mfmtg::TestDescription::name)
      .def_readwrite("author", &mfmtg::TestDescription::author)
      .def_readwrite("date", &mfmtg::TestDescription::date)
      .def_readwrite("description", &mfmtg::TestDescription::description)
      .def_readwrite("behaviour", &mfmtg::TestDescription::behaviour)
      .def_readwrite("material", &mfmtg::TestDescription::material);
  //
  boost::python::def("loadMFMTestGeneratorFileContent",
                     mfmtg_loadMFMTestGeneratorFileContent);
  boost::python::def("write", mfmtg_write);

}  // end of declareMFMTestGeneratorFileExport