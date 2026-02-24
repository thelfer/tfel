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
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/MFrontMaterialPropertyInterface.hxx"

namespace mfront {

  std::string MFrontMaterialPropertyInterface::getName() { return "mfront"; }

  MFrontMaterialPropertyInterface::MFrontMaterialPropertyInterface() = default;

  std::vector<std::string>
  MFrontMaterialPropertyInterface::getSupportedFloatingPointTypes() const {
    return {"float", "double", "long double"};
  }  // end of getSupportedFloatingPointTypes

  bool MFrontMaterialPropertyInterface::
      shallGenerateOverloadedFunctionForQuantities() const {
    return true;
  }  // end of shallGenerateOverloadedFunctionForQuantities

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
    if (!header.empty()) {
      insert_if(d.headers, header + ".hxx");
    }
    insert_if(l.sources, name + ".cxx");
    insert_if(l.link_libraries, "m");
    insert_if(l.epts, {f, f + "_checkBounds"});
  }  // end of getTargetsDescription

  std::string MFrontMaterialPropertyInterface::getHeaderFileName(
      const std::string& material, const std::string& className) const {
    if (material.empty()) {
      return className + "-mfront";
    }
    return material + "_" + className + "-mfront";
  }  // end of getHeaderFileName

  std::string MFrontMaterialPropertyInterface::getSrcFileName(
      const std::string& material, const std::string& className) const {
    if (material.empty()) {
      return className + "-mfront";
    }
    return material + "_" + className + "-mfront";
  }  // end of getSrcFileName

  void MFrontMaterialPropertyInterface::writeBeginHeaderNamespace(
      std::ostream& os) const {
    os << "namespace mfront{\n\n";
  }  // end of writeBeginHeaderNamespace

  void MFrontMaterialPropertyInterface::writeEndHeaderNamespace(
      std::ostream& os) const {
    os << "} // end of namespace mfront\n\n";
  }  // end of writeEndHeaderNamespace()

  void MFrontMaterialPropertyInterface::writeBeginSrcNamespace(
      std::ostream& os) const {
    os << "namespace mfront{\n\n";
  }  // end of writeBeginSrcNamespace

  void MFrontMaterialPropertyInterface::writeEndSrcNamespace(
      std::ostream& os) const {
    os << "} // end of namespace mfront\n";
  }  // end of writeEndSrcNamespace()

  std::string MFrontMaterialPropertyInterface::getFunctionName(
      const MaterialPropertyDescription& mpd) const {
    const auto material = mpd.material;
    const auto law = mpd.className;
    return material.empty() ? law : material + '_' + law;
  }  // end of getFunctionName

  bool MFrontMaterialPropertyInterface::requiresCheckBoundsFunction() const {
    return true;
  }

  std::string MFrontMaterialPropertyInterface::getCheckBoundsFunctionName(
      const MaterialPropertyDescription& mpd) const {
    return this->getFunctionName(mpd) + "_checkBounds";
  }  // end of getCheckBoundsFunctionName

  void MFrontMaterialPropertyInterface::writeFileDescriptionSymbols(
      std::ostream&,
      const MaterialPropertyDescription&,
      const FileDescription&) const {}  // end of writeFileDescriptionSymbols

  void MFrontMaterialPropertyInterface::writeValidatorSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of writeValidatorSymbol

  void MFrontMaterialPropertyInterface::writeBuildIdentifierSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of writeBuildIdentifierSymbol

  void MFrontMaterialPropertyInterface::writeEntryPointSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of writeEntryPointSymbol

  void MFrontMaterialPropertyInterface::writeTFELVersionSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of writeTFELVersionSymbol

  void MFrontMaterialPropertyInterface::writeUnitSystemSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of writeUnitSystemSymbol

  void MFrontMaterialPropertyInterface::writeLawSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of writeMaterialSymbol

  void MFrontMaterialPropertyInterface::writeMaterialSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of writeMaterialSymbol

  void MFrontMaterialPropertyInterface::writeInterfaceSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of writeInterfaceSymbol

  void MFrontMaterialPropertyInterface::writeMaterialKnowledgeTypeSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of writeEntryPointSymbol

  void MFrontMaterialPropertyInterface::writeVariablesNamesSymbol(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of writeVariablesNamesSymbol

  void MFrontMaterialPropertyInterface::writeVariablesBoundsSymbols(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of writeVariablesBoundsSymbols

  MFrontMaterialPropertyInterface::~MFrontMaterialPropertyInterface() = default;

}  // end of namespace mfront
