/*!
 * \file   mfront/src/MFrontMaterialPropertyInterface.cxx
 * \brief
 * \author Helfer Thomas
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
    if (std::find(i.begin(), i.end(), "mfront") != i.end()) {
      throw(
          std::runtime_error("MFrontMaterialPropertyInterface::treatKeyword: "
                             "unsupported key '" +
                             k + "'"));
    }
    return {false, current};
  }  // end of treatKeyword

  void MFrontMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) {
    const auto lib = "MFrontMaterialLaw";
    const auto name = this->getSrcFileName(mpd.material, mpd.className);
    const auto f = mpd.material.empty() ? mpd.className
                                        : mpd.material + "_" + mpd.className;
    const auto header = this->getHeaderFileName(mpd.material, mpd.className);
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(d[lib].ldflags, "-lm");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    insert_if(d[lib].sources, name + ".cxx");
    if (!header.empty()) {
      insert_if(d.headers, header + ".hxx");
    }
    insert_if(d[lib].epts, {f, f + "_checkBounds"});
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

  void MFrontMaterialPropertyInterface::writeBeginHeaderNamespace() {
    this->headerFile << "namespace mfront{\n\n";
  }  // end of MFrontMaterialPropertyInterface::writeBeginHeaderNamespace

  void MFrontMaterialPropertyInterface::writeEndHeaderNamespace() {
    this->headerFile << "} // end of namespace mfront\n\n";
  }  // end of MFrontMaterialPropertyInterface::writeEndHeaderNamespace(void)

  void MFrontMaterialPropertyInterface::writeBeginSrcNamespace() {
    this->srcFile << "namespace mfront{\n";
  }  // end of MFrontMaterialPropertyInterface::writeBeginSrcNamespace

  void MFrontMaterialPropertyInterface::writeEndSrcNamespace() {
    this->srcFile << "} // end of namespace mfront\n";
  }  // end of MFrontMaterialPropertyInterface::writeEndSrcNamespace(void)

  std::string MFrontMaterialPropertyInterface::getFunctionName(
      const std::string& material, const std::string& law) const {
    return material.empty() ? law : material + '_' + law;
  }  // end of MFrontMaterialPropertyInterface::getFunctionName

  bool MFrontMaterialPropertyInterface::requiresCheckBoundsFunction() const {
    return true;
  }

  std::string MFrontMaterialPropertyInterface::getCheckBoundsFunctionName(
      const std::string& material, const std::string& className) const {
    return this->getFunctionName(material, className) + "_checkBounds";
  }  // end of MFrontMaterialPropertyInterface::getCheckBoundsFunctionName

  MFrontMaterialPropertyInterface::~MFrontMaterialPropertyInterface() = default;

}  // end of namespace mfront
