/*!
 * \file   mfront/src/MFrontMaterialPropertyInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MFrontMaterialPropertyInterface.hxx"

namespace mfront {

  std::string MFrontMaterialPropertyInterface::getName() { return "mfront"; }

  MFrontMaterialPropertyInterface::MFrontMaterialPropertyInterface() = default;

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontMaterialPropertyInterface::treatKeyword(
      const std::string& k,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator) {
    tfel::raise_if(std::find(i.begin(), i.end(), "mfront") != i.end(),
                   "MFrontMaterialPropertyInterface::treatKeyword: "
                   "unsupported key '" +
                       k + "'");
    return {false, current};
  }  // end of treatKeyword

  void MFrontMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const {
    const auto lib = "MFrontMaterialLaw";
    const auto name = this->getSrcFileName(mpd.material, mpd.className);
    const auto f = mpd.material.empty() ? mpd.className
                                        : mpd.material + "_" + mpd.className;
    const auto header = this->getHeaderFileName(mpd.material, mpd.className);
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto& l = d.getLibrary(lib);
    insert_if(l.cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(l.include_directories,
              "$(shell " + tfel_config + " --include-path)");
    if (!header.empty()) {
      insert_if(d.headers, header + ".hxx");
    }
    insert_if(l.sources, name + ".cxx");
    insert_if(l.link_libraries, "m");
    insert_if(l.epts, {f, f + "_checkBounds"});
  }  // end of MFrontMaterialPropertyInterface::getTargetsDescription

  std::string MFrontMaterialPropertyInterface::getHeaderFileName(
      const std::string& material, const std::string& className) const {
    if (material.empty()) {
      return className + "-mfront";
    }
    return material + "_" + className + "-mfront";
  }  // end of MFrontMaterialPropertyInterface::getHeaderFileName

  std::string MFrontMaterialPropertyInterface::getSrcFileName(
      const std::string& material, const std::string& className) const {
    if (material.empty()) {
      return className + "-mfront";
    }
    return material + "_" + className + "-mfront";
  }  // end of MFrontMaterialPropertyInterface::getSrcFileName

  void MFrontMaterialPropertyInterface::writeBeginHeaderNamespace(
      std::ostream& os) const {
    os << "namespace mfront{\n\n";
  }  // end of MFrontMaterialPropertyInterface::writeBeginHeaderNamespace

  void MFrontMaterialPropertyInterface::writeEndHeaderNamespace(
      std::ostream& os) const {
    os << "} // end of namespace mfront\n\n";
  }  // end of MFrontMaterialPropertyInterface::writeEndHeaderNamespace()

  void MFrontMaterialPropertyInterface::writeBeginSrcNamespace(
      std::ostream& os) const {
    os << "namespace mfront{\n";
  }  // end of MFrontMaterialPropertyInterface::writeBeginSrcNamespace

  void MFrontMaterialPropertyInterface::writeEndSrcNamespace(
      std::ostream& os) const {
    os << "} // end of namespace mfront\n";
  }  // end of MFrontMaterialPropertyInterface::writeEndSrcNamespace()

  std::string MFrontMaterialPropertyInterface::getFunctionName(
      const MaterialPropertyDescription& mpd) const {
    const auto material = mpd.material;
    const auto law = mpd.className;
    return material.empty() ? law : material + '_' + law;
  }  // end of MFrontMaterialPropertyInterface::getFunctionName

  bool MFrontMaterialPropertyInterface::requiresCheckBoundsFunction() const {
    return true;
  }

  std::string MFrontMaterialPropertyInterface::getCheckBoundsFunctionName(
      const MaterialPropertyDescription& mpd) const {
    return this->getFunctionName(mpd) + "_checkBounds";
  }  // end of MFrontMaterialPropertyInterface::getCheckBoundsFunctionName

  void MFrontMaterialPropertyInterface::writeEntryPointSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of MFrontMaterialPropertyInterface::writeEntryPointSymbol

  void MFrontMaterialPropertyInterface::writeTFELVersionSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of MFrontMaterialPropertyInterface::writeTFELVersionSymbol

  void MFrontMaterialPropertyInterface::writeMaterialSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of MFrontMaterialPropertyInterface::writeMaterialSymbol

  void MFrontMaterialPropertyInterface::writeInterfaceSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of MFrontMaterialPropertyInterface::writeInterfaceSymbol

  void MFrontMaterialPropertyInterface::writeMaterialKnowledgeTypeSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of MFrontMaterialPropertyInterface::writeEntryPointSymbol

  MFrontMaterialPropertyInterface::~MFrontMaterialPropertyInterface() = default;

}  // end of namespace mfront
