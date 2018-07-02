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

#include <fstream>
#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/FileDescription.hxx"
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

  void GenericBehaviourInterface::endTreatment(
      const BehaviourDescription& bd, const FileDescription& fd) const {
    auto raise = [](const std::string& m) {
      tfel::raise("GenericBehaviourInterface::endTreatment: " + m);
    };
    tfel::system::systemCall::mkdir("include/MFront");
    tfel::system::systemCall::mkdir("include/MFront/GenericBehaviour");
    const auto mh = this->getModellingHypothesesToBeTreated(bd);
    const auto name = bd.getBehaviourName();
    const auto header = name + "-generic.hxx";
    std::ofstream out("include/MFront/GenericBehaviour/" + header);
    if(!out){
      raise("could not open file '" + header + "'");
    }
    out << "/*!\n"
        << "* \\file   " << header << '\n'
        << "* \\brief  This file declares the umat interface for the "
        << bd.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    const auto hg = this->getHeaderGuard(bd);
    out << "#ifndef " << hg << "\n"
        << "#define " << hg << "\n\n"
        << "#include\"TFEL/Config/TFELConfig.hxx\"\n"
        << "#include\"MFront/GenericBehaviour/BehaviourData.h\"\n\n";

    this->writeVisibilityDefines(out);
    out << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";
    this->writeSetParametersFunctionsDeclarations(out, name, bd);
    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);

    for (const auto h : this->getModellingHypothesesToBeTreated(bd)) {
      const auto f = this->getFunctionNameForHypothesis(name, h);
      out << "/*!\n"
          << " * \\param[in,out] d: material data\n"
          << " */\n"
          << "MFRONT_SHAREDOBJ void " << f << "(MFront_GB_BehaviourData* const);\n\n";
    }

    out << "#ifdef __cplusplus\n"
        << "}\n"
        << "#endif /* __cplusplus */\n\n";
    out << "#endif /* " << hg << " */\n";
    out.close();

    const auto src = name + "-generic.cxx";
    out.open("src/" + src);
    if (!out) {
      raise("could not open file '" + src + "'");
    }

    out << "/*!\n"
        << "* \\file   " << src << '\n'
        << "* \\brief  This file implements the umat interface for the " << bd.getClassName()
        << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";
    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n";
    out << "#include\"TFEL/Material/" << bd.getClassName() << ".hxx\"\n";
    if (bd.getAttribute(BehaviourData::profiling, false)) {
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n";
    }
    out << '\n';
    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);
    this->generateUMATxxGeneralSymbols(out, name, bd, fd);
    if (!bd.areAllMechanicalDataSpecialised(mh)) {
      const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      this->generateUMATxxSymbols(out, name, uh, bd, fd);
    }
    for (const auto& h : mh) {
      if (bd.hasSpecialisedMechanicalData(h)) {
        this->generateUMATxxSymbols(out, name, h, bd, fd);
      }
    }

    for (const auto h : this->getModellingHypothesesToBeTreated(bd)) {
      const auto f = this->getFunctionNameForHypothesis(name, h);
      out << "MFRONT_SHAREDOBJ void " << f
          << "(MFront_GB_BehaviourData* const d){\n"
          << "}; // end of " << f << "\n\n";
    }

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
      std::ostream& os, const std::string& name, const BehaviourDescription& bd) const {
    os << "MFRONT_SHAREDOBJ unsigned short "
        << this->getFunctionNameBasis(name) << "_BehaviourType = ";
    if (bd.getBehaviourType() == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (bd.isStrainMeasureDefined()) {
        const auto fs = bd.getStrainMeasure();
        if (fs == BehaviourDescription::LINEARISED) {
          os << "1u";
        } else if (fs == BehaviourDescription::GREENLAGRANGE) {
          os << "3u";
        } else if (fs == BehaviourDescription::HENCKY) {
          os << "3u";
        } else {
          tfel::raise(
              "GenericBehaviourInterface::writeUMATxxBehaviourTypeSymbols: "
              "unsupported strain measure");
          os << "0u";
        }
      } else {
        os << "1u";
      }
    } else if (bd.getBehaviourType() == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      os << "3u";
    } else if (bd.getBehaviourType() == BehaviourDescription::COHESIVEZONEMODEL) {
      os << "2u";
    } else {
      tfel::raise(
          "GenericBehaviourInterface::writeUMATxxBehaviourTypeSymbols: "
          "unsupported strain measure");
    }
    os << ";\n\n";
  }  // end of GenericBehaviourInterface::writeUMATxxBehaviourTypeSymbols

  void GenericBehaviourInterface::writeUMATxxBehaviourKinematicSymbols(
      std::ostream& os,
      const std::string& name,
      const BehaviourDescription& bd) const {
    os << "MFRONT_SHAREDOBJ unsigned short "
        << this->getFunctionNameBasis(name) << "_BehaviourKinematic = ";
    if (bd.getBehaviourType() == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (bd.isStrainMeasureDefined()) {
        const auto fs = bd.getStrainMeasure();
        if (fs == BehaviourDescription::LINEARISED) {
          os << "1u";
        } else if (fs == BehaviourDescription::GREENLAGRANGE) {
          os << "3u";
        } else if (fs == BehaviourDescription::HENCKY) {
          os << "3u";
        } else {
          tfel::raise(
              "GenericBehaviourInterface::writeUMATxxBehaviourTypeSymbols: "
              "unsupported strain measure");
          os << "0u";
        }
      } else {
        os << "1u";
      }
    } else if (bd.getBehaviourType() == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      os << "3u";
    } else if (bd.getBehaviourType() == BehaviourDescription::COHESIVEZONEMODEL) {
      os << "2u";
    } else {
      tfel::raise(
          "GenericBehaviourInterface::writeUMATxxBehaviourTypeSymbols: "
          "unsupported strain measure");
    }
    os << ";\n\n";
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

  GenericBehaviourInterface::~GenericBehaviourInterface() = default;

  }  // end of namespace mfront