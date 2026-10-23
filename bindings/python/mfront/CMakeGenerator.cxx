/*!
 * \file   CMakeGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \date 11/07/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/TargetsDescription.hxx"
#include "MFront/GeneratorOptions.hxx"
#include "MFront/CMakeGenerator.hxx"

struct CMakeGenerator {
  void exe0(const mfront::TargetsDescription&);
  void exe1(const mfront::TargetsDescription&, const mfront::GeneratorOptions&);
  void exe2(const mfront::TargetsDescription&, const std::string&);
  void exe3(const mfront::TargetsDescription&,
             const mfront::GeneratorOptions&,
             const std::string&);
  void generate0(const mfront::TargetsDescription&);
  void generate1(const mfront::TargetsDescription&,
                  const mfront::GeneratorOptions&);
  void callCMake0();
  void callCMake1(const std::string&);
};

void CMakeGenerator::exe0(const mfront::TargetsDescription& t) {
  this->exe3(t, mfront::GeneratorOptions(), "all");
}

void CMakeGenerator::exe1(const mfront::TargetsDescription& t,
                           const mfront::GeneratorOptions& o) {
  this->exe3(t, o, "all");
}

void CMakeGenerator::exe2(const mfront::TargetsDescription& t,
                           const std::string& tn) {
  this->exe3(t, mfront::GeneratorOptions(), tn);
}

void CMakeGenerator::exe3(const mfront::TargetsDescription& t,
                           const mfront::GeneratorOptions& o,
                           const std::string& tn) {
  mfront::generateCMakeListsFile(t, o);
  mfront::callCMake(tn);
}

void CMakeGenerator::generate0(const mfront::TargetsDescription& t) {
  this->generate1(t, mfront::GeneratorOptions());
}

void CMakeGenerator::generate1(const mfront::TargetsDescription& t,
                                const mfront::GeneratorOptions& o) {
  mfront::generateCMakeListsFile(t, o);
}

void CMakeGenerator::callCMake0() { this->callCMake1("all"); }

void CMakeGenerator::callCMake1(const std::string& tn) {
  mfront::callCMake(tn);
}

void declareCMakeGenerator(pybind11::module_&);

void declareCMakeGenerator(pybind11::module_& m) {
  using namespace mfront;
  pybind11::class_<CMakeGenerator>(m, "CMakeGenerator")
      .def("generate", &CMakeGenerator::generate0, pybind11::arg("t"))
      .def("generate", &CMakeGenerator::generate1,
          pybind11::arg("t"), pybind11::arg("o"))
      .def("callCMake", &CMakeGenerator::callCMake0)
      .def("callCMake", &CMakeGenerator::callCMake1, pybind11::arg("tn"))
      .def("exe", &CMakeGenerator::exe0, pybind11::arg("t"))
      .def("exe", &CMakeGenerator::exe1,
          pybind11::arg("t"), pybind11::arg("o"))
      .def("exe", &CMakeGenerator::exe2,
          pybind11::arg("t"), pybind11::arg("tn"))
      .def("exe", &CMakeGenerator::exe3,
          pybind11::arg("t"), pybind11::arg("o"), pybind11::arg("tn"));

}
