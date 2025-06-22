/*!
 * \file   mfront/src/CMaterialPropertyInterface.cxx
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
#include "MFront/CMaterialPropertyInterface.hxx"

namespace mfront {

  std::string CMaterialPropertyInterface::getName() { return "c"; }

  CMaterialPropertyInterface::CMaterialPropertyInterface() = default;

  std::string CMaterialPropertyInterface::getGeneratedLibraryName(
      const std::string& l, const std::string& m) const {
    return getMaterialLawLibraryNameBase(l, m);
  }  // end of CMaterialPropertyInterface::getGeneratedLibraryName

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CMaterialPropertyInterface::treatKeyword(const std::string& k,
                                           const std::vector<std::string>& i,
                                           tokens_iterator current,
                                           const tokens_iterator) {
    if (std::find(i.begin(), i.end(), "c") != i.end()) {
      throw(
          std::runtime_error("CMaterialPropertyInterface::treatKeyword: "
                             "unsupported key '" +
                             k + "'"));
    }
    return {false, current};
  }  // end of CMaterialPropertyInterface::treatKeyword

  void CMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) {
    const auto lib = this->getGeneratedLibraryName(mpd.library, mpd.material);
    const auto name = this->getSrcFileName(mpd.material, mpd.className);
    const auto f = mpd.material.empty() ? mpd.className
                                        : mpd.material + "_" + mpd.className;
    const auto header = this->getHeaderFileName(mpd.material, mpd.className);
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(d[lib].ldflags, "-lm");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    insert_if(d[lib].sources, name + ".cxx");
    insert_if(d[lib].epts, {f, f + "_checkBounds"});
    if (!header.empty()) {
      insert_if(d.headers, header + ".hxx");
    }
  }  // end of CMaterialPropertyInterface::getTargetsDescription

  std::string CMaterialPropertyInterface::getHeaderFileName(
      const std::string& material, const std::string& className) const {
    return material.empty() ? className : material + "_" + className;
  }  // end of CMaterialPropertyInterface::getHeaderFileName

  std::string CMaterialPropertyInterface::getSrcFileName(
      const std::string& material, const std::string& className) const {
    return material.empty() ? className : material + "_" + className;
  }  // end of CMaterialPropertyInterface::getSrcFileName

  void CMaterialPropertyInterface::writeBeginHeaderNamespace() {
    this->headerFile << "#ifdef __cplusplus\n";
    this->headerFile << "extern \"C\"{\n";
    this->headerFile << "#endif /* __cplusplus */\n\n";
  }  // end of CMaterialPropertyInterface::writeBeginHeaderNamespace

  void CMaterialPropertyInterface::writeEndHeaderNamespace() {
    this->headerFile << "#ifdef __cplusplus\n";
    this->headerFile << "} /* end of extern \"C\" */\n";
    this->headerFile << "#endif /* __cplusplus */\n\n";
  }  // end of CMaterialPropertyInterface::writeEndHeaderNamespace(void)

  void CMaterialPropertyInterface::writeBeginSrcNamespace() {
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "extern \"C\"{\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
  }  // end of CMaterialPropertyInterface::writeBeginSrcNamespace

  void CMaterialPropertyInterface::writeEndSrcNamespace() {
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "} // end of extern \"C\"\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
  }  // end of CMaterialPropertyInterface::writeEndSrcNamespace(void)

  std::string CMaterialPropertyInterface::getFunctionName(
      const std::string& material, const std::string& className) const {
    return material.empty() ? className : material + "_" + className;
  }  // end of CMaterialPropertyInterface::getFunctionName

  bool CMaterialPropertyInterface::requiresCheckBoundsFunction() const {
    return false;
  }

  std::string CMaterialPropertyInterface::getCheckBoundsFunctionName(
      const std::string& material, const std::string& className) const {
    if (material.empty()) {
      return className + "_checkBounds";
    }
    return material + "_" + className + "_checkBounds";
  }  // end of CMaterialPropertyInterface::getCheckBoundsFunctionName

  CMaterialPropertyInterface::~CMaterialPropertyInterface() = default;

}  // end of namespace mfront
