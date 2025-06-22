/*!
 * \file   bindings/python/mfront/AbstractDSL.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <set>
#include <memory>
#include "TFEL/Python/SharedPtr.hxx"
#include <boost/python.hpp>

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
                         const std::vector<std::string>& args) {
  dsl.analyseFile(f, args, {});
}

static void analyseFile3(mfront::AbstractDSL& dsl,
                         const std::string& f,
                         const std::vector<std::string>& args,
                         const std::map<std::string, std::string>& s) {
  dsl.analyseFile(f, args, s);
}

void declareAbstractDSL() {
  using namespace boost::python;
  using namespace mfront;
  enum_<AbstractDSL::DSLTarget>("DSLTarget")
      .value("MATERIALPROPERTYDSL", AbstractDSL::MATERIALPROPERTYDSL)
      .value("BEHAVIOURDSL", AbstractDSL::BEHAVIOURDSL)
      .value("MODELDSL", AbstractDSL::MODELDSL);
  class_<AbstractDSL, std::shared_ptr<AbstractDSL>, boost::noncopyable>(
      "AbstractDSL", no_init)
      .def("getTargetType", &AbstractDSL::getTargetType)
      .def("getMaterialKnowledgeDescription",
           &AbstractDSL::getMaterialKnowledgeDescription,
           return_internal_reference<>())
      .def("getFileDescription", &AbstractDSL::getFileDescription,
           return_internal_reference<>())
      .def("analyseFile", analyseFile1)
      .def("analyseFile", analyseFile2)
      .def("analyseFile", analyseFile3)
      .def("analyseString", &AbstractDSL::analyseString)
      .def("getTargetsDescription", &AbstractDSL::getTargetsDescription,
           return_internal_reference<>())
      .def("generateOutputFiles", &AbstractDSL::generateOutputFiles)
      .def("setInterfaces", &setInterfaces)
      .def("getKeywordsList", &getKeywordsList);
}
