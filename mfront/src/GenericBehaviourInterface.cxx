/*!
 * \file   mfront/src/GenericBehaviourInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   30/06/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "MFront/LibraryDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/GenericBehaviourInterface.hxx"

namespace mfront {

  std::string GenericBehaviourInterface::getName() {
    return "generic-behaviour";
  }  // end of getName

  std::string GenericBehaviourInterface::getInterfaceName() const {
    return "Generic";
  }  // end of GenericBehaviourInterface::getInterfaceName

  std::pair<bool, GenericBehaviourInterface::tokens_iterator>
  GenericBehaviourInterface::treatKeyword(BehaviourDescription&,
                                          const std::string&,
                                          const std::vector<std::string>&,
                                          tokens_iterator p,
                                          const tokens_iterator) {
    return {false, p};
  }  // end of GenericBehaviourInterface::treatKeyword

  std::set<GenericBehaviourInterface::Hypothesis>
  GenericBehaviourInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& bd) const {
    return bd.getModellingHypotheses();
  }  // end of GenericBehaviourInterface::getModellingHypothesesToBeTreated

  void GenericBehaviourInterface::writeInterfaceSpecificIncludes(
      std::ostream&, const BehaviourDescription&) const {
  }  // end of GenericBehaviourInterface::writeInterfaceSpecificIncludes

  void GenericBehaviourInterface::getTargetsDescription(
      TargetsDescription& d, const BehaviourDescription& bd) {
    const auto lib = this->getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(d[lib].cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(d[lib].include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(d[lib].sources, name + "-generic.cxx");
    d.headers.push_back("MFront/GenericBehaviour/" + name + "-generic.hxx");
    insert_if(d[lib].link_directories,
              "$(shell " + tfel_config + " --library-path)");
    if (this->shallGenerateMTestFileOnFailure(bd)) {
      insert_if(d[lib].link_libraries,
                tfel::getLibraryInstallName("MTestFileGenerator"));
    }
#if __cplusplus >= 201703L
    insert_if(d[lib].link_libraries, "$(shell " + tfel_config +
                                         " --library-dependency "
                                         "--material --mfront-profiling)");
#else  /* __cplusplus < 201703L */
    insert_if(d[lib].link_libraries,
              "$(shell " + tfel_config +
                  " --library-dependency "
                  "--material --mfront-profiling --physical-constants)");
#endif /* __cplusplus < 201703L */
    for (const auto h : this->getModellingHypothesesToBeTreated(bd)) {
      insert_if(d[lib].epts, this->getFunctionNameForHypothesis(name, h));
    }
  }  // end of GenericBehaviourInterface::getTargetsDescription

  void GenericBehaviourInterface::endTreatment(const BehaviourDescription&,
                                               const FileDescription&) const {
  }  // end of GenericBehaviourInterface::endTreatment

  std::string GenericBehaviourInterface::getLibraryName(
      const BehaviourDescription& bd) const {
    if (bd.getLibrary().empty()) {
      if (!bd.getMaterialName().empty()) {
        return bd.getMaterialName() + "-generic";
      } else {
        return "Behaviour";
      }
    }
    return bd.getLibrary() + "-generic";
  }  // end of GenericBehaviourInterface::getLibraryName

  std::string GenericBehaviourInterface::getFunctionNameBasis(
      const std::string& n) const {
    return n;
  }  // end of GenericBehaviourInterface::getFunctionName

  std::string GenericBehaviourInterface::getFunctionNameForHypothesis(
      const std::string& n, const Hypothesis h) const {
    return n + "_" + ModellingHypothesis::toString(h);
  }  // end of GenericBehaviourInterface::getFunctionNameForHypothesis

  void GenericBehaviourInterface::writeUMATxxBehaviourTypeSymbols(
      std::ostream&, const std::string&, const BehaviourDescription&) const {
  }  // end of GenericBehaviourInterface::writeUMATxxBehaviourTypeSymbols

  void GenericBehaviourInterface::writeUMATxxBehaviourKinematicSymbols(
      std::ostream&, const std::string&, const BehaviourDescription&) const {
  }  // end of
     // GenericBehaviourInterface::writeUMATxxBehaviourKinematicSymbols

  void GenericBehaviourInterface::writeUMATxxAdditionalSymbols(
      std::ostream&,
      const std::string&,
      Hypothesis,
      const BehaviourDescription&,
      const FileDescription&) const {
  }  // end of GenericBehaviourInterface::writeUMATxxAdditionalSymbols

  void GenericBehaviourInterface::writeMTestFileGeneratorSetModellingHypothesis(
      std::ostream& out) const {
    out << "mg.setModellingHypothesis(h);\n";
  }  // end of
  // GenericBehaviourInterface::writeMTestFileGeneratorSetModellingHypothesis

  std::string GenericBehaviourInterface::getModellingHypothesisTest(
      const Hypothesis) const {
  }  // end of GenericBehaviourInterface::getModellingHypothesisTest

  GenericBehaviourInterface::~GenericBehaviourInterface() = default;

  }  // end of namespace mfront