/*!
 * \file   MakefileGenerator.cxx
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
#include "MFront/MakefileGenerator.hxx"

struct MakefileGenerator {
  void exe0(const mfront::TargetsDescription&);
  void exe1(const mfront::TargetsDescription&, const mfront::GeneratorOptions&);
  void exe2(const mfront::TargetsDescription&, const std::string&);
  void exe3(const mfront::TargetsDescription&,
             const mfront::GeneratorOptions&,
             const std::string&);
  void generate0(const mfront::TargetsDescription&);
  void generate1(const mfront::TargetsDescription&,
                  const mfront::GeneratorOptions&);
  void callMake0();
  void callMake1(const std::string&);
};

void MakefileGenerator::exe0(const mfront::TargetsDescription& t) {
  this->exe3(t, mfront::GeneratorOptions(), "all");
}

void MakefileGenerator::exe1(const mfront::TargetsDescription& t,
                              const mfront::GeneratorOptions& o) {
  this->exe3(t, o, "all");
}

void MakefileGenerator::exe2(const mfront::TargetsDescription& t,
                              const std::string& tn) {
  this->exe3(t, mfront::GeneratorOptions(), tn);
}

void MakefileGenerator::exe3(const mfront::TargetsDescription& t,
                              const mfront::GeneratorOptions& o,
                              const std::string& tn) {
  mfront::generateMakeFile(t, o);
  mfront::callMake(tn);
}

void MakefileGenerator::generate0(const mfront::TargetsDescription& t) {
  this->generate1(t, mfront::GeneratorOptions());
}

void MakefileGenerator::generate1(const mfront::TargetsDescription& t,
                                   const mfront::GeneratorOptions& o) {
  mfront::generateMakeFile(t, o);
}

void MakefileGenerator::callMake0() { this->callMake1("all"); }

void MakefileGenerator::callMake1(const std::string& tn) {
  mfront::callMake(tn);
}

void declareMakefileGenerator(pybind11::module_&);

void declareMakefileGenerator(pybind11::module_& m) {
  using namespace mfront;
  pybind11::class_<MakefileGenerator>(m, "MakefileGenerator")
      .def(pybind11::init<>())
      .def("generate", &MakefileGenerator::generate0, pybind11::arg("t"))
      .def("generate", &MakefileGenerator::generate1,
          pybind11::arg("t"), pybind11::arg("o"))
      .def("callMake", &MakefileGenerator::callMake0)
      .def("callMake", &MakefileGenerator::callMake1, pybind11::arg("tn"))
      .def("exe", &MakefileGenerator::exe0, pybind11::arg("t"))
      .def("exe", &MakefileGenerator::exe1,
          pybind11::arg("t"), pybind11::arg("o"))
      .def("exe", &MakefileGenerator::exe2,
          pybind11::arg("t"), pybind11::arg("tn"))
      .def("exe", &MakefileGenerator::exe3,
          pybind11::arg("t"), pybind11::arg("o"), pybind11::arg("tn"));

}
