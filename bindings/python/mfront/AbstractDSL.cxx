
/*!
 * \file   bindings/python/mfront/AbstractDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date 04/05/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <set>
#include <memory>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/MaterialKnowledgeDescription.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/AbstractDSL.hxx"

static void setInterfaces(mfront::AbstractDSL& dsl,
                          const std::vector<std::string>& i) {
  std::set<std::string> inames(i.begin(), i.end());
  dsl.setInterfaces(inames);
}

static std::vector<std::string> getKeywordsList(mfront::AbstractDSL& dsl) {
  std::vector<std::string> keys;
  dsl.getKeywordsList(keys);
  return keys;
}

static void analyseFile1(mfront::AbstractDSL& dsl, const std::string& f) {
  dsl.analyseFile(f, {}, {});
}

static void analyseFile2(mfront::AbstractDSL& dsl,
                         const std::string& f,
                         const std::vector<std::string>& ecmds) {
  dsl.analyseFile(f, ecmds, {});
}

static void analyseFile3(mfront::AbstractDSL& dsl,
                         const std::string& f,
                         const std::vector<std::string>& ecmds,
                         const std::map<std::string, std::string>& s) {
  dsl.analyseFile(f, ecmds, s);
}

void declareAbstractDSL(pybind11::module_&);

void declareAbstractDSL(pybind11::module_& m) {
  using namespace mfront;
  pybind11::enum_<AbstractDSL::DSLTarget>(m, "DSLTarget")
      .value("MATERIALPROPERTYDSL", AbstractDSL::MATERIALPROPERTYDSL)
      .value("BEHAVIOURDSL", AbstractDSL::BEHAVIOURDSL)
      .value("MODELDSL", AbstractDSL::MODELDSL);
  pybind11::class_<AbstractDSL, std::shared_ptr<AbstractDSL>>(
      m, "AbstractDSL",
      "Interface class for all domain specific languages")
      .def("getTargetType", &AbstractDSL::getTargetType,
           "Return the target of the dsl")
      .def("getMaterialKnowledgeDescription",
           &AbstractDSL::getMaterialKnowledgeDescription,
           pybind11::return_value_policy::reference,
           "Return the description of the considered material knowledge")
      .def("getFileDescription", &AbstractDSL::getFileDescription,
           pybind11::return_value_policy::reference,
           "Return the file description associated with the treated file")
      .def("analyseFile", analyseFile1,
           pybind11::arg("f"),
           "Analyse a file")
      .def("analyseFile", analyseFile2,
           pybind11::arg("f"), pybind11::arg("ecmds"),
           "Analyse a file")
      .def("analyseFile", analyseFile3,
           pybind11::arg("f"), pybind11::arg("ecmds"), pybind11::arg("s"),
           "Analyse a file")
      .def("analyseString", &AbstractDSL::analyseString,
           pybind11::arg("s"),
           "Analyse the specified string")
      .def("getTargetsDescription", &AbstractDSL::getTargetsDescription,
           pybind11::return_value_policy::reference)
      .def("generateOutputFiles", &AbstractDSL::generateOutputFiles)
      .def("setInterfaces", &setInterfaces, pybind11::arg("i"))
      .def("getKeywordsList", &getKeywordsList);
}
