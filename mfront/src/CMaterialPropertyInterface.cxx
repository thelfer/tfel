/*!
 * \file   mfront/src/CMaterialPropertyInterface.cxx
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
#include "MFront/CMaterialPropertyInterface.hxx"

namespace mfront {

  std::string CMaterialPropertyInterface::getName() { return "c"; }

  CMaterialPropertyInterface::CMaterialPropertyInterface() = default;

  std::string CMaterialPropertyInterface::getGeneratedLibraryName(
      const MaterialPropertyDescription& m) const {
    return getMaterialLawLibraryNameBase(m);
  }  // end of CMaterialPropertyInterface::getGeneratedLibraryName

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  CMaterialPropertyInterface::treatKeyword(const std::string& k,
                                           const std::vector<std::string>& i,
                                           tokens_iterator current,
                                           const tokens_iterator) {
    tfel::raise_if(std::find(i.begin(), i.end(), "c") != i.end(),
                   "CMaterialPropertyInterface::treatKeyword: "
                   "unsupported key '" +
                       k + "'");
    return {false, current};
  }  // end of CMaterialPropertyInterface::treatKeyword

  void CMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const {
    const auto lib = this->getGeneratedLibraryName(mpd);
    const auto name = this->getSrcFileName(mpd.material, mpd.className);
    const auto f = mpd.material.empty() ? mpd.className
                                        : mpd.material + "_" + mpd.className;
    const auto header = this->getHeaderFileName(mpd.material, mpd.className);
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(d[lib].cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(d[lib].include_directories,
              "$(shell " + tfel_config + " --include-path)");
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(d[lib].link_libraries, "m");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    insert_if(d[lib].sources, name + ".cxx");
    insert_if(d[lib].epts, {f, f + "_checkBounds"});
    if (!header.empty()) {
      insert_if(d.headers, header + ".hxx");
    }
  }  // end of CMaterialPropertyInterface::getTargetsDescription

  void CMaterialPropertyInterface::writeInterfaceSymbol(
      std::ostream& out, const MaterialPropertyDescription& mpd) const {
    mfront::writeInterfaceSymbol(out, this->getFunctionName(mpd), "C");
  }  // end of CMaterialPropertyInterface

  std::string CMaterialPropertyInterface::getHeaderFileName(
      const std::string& material, const std::string& className) const {
    return material.empty() ? className : material + "_" + className;
  }  // end of CMaterialPropertyInterface::getHeaderFileName

  std::string CMaterialPropertyInterface::getSrcFileName(
      const std::string& material, const std::string& className) const {
    return material.empty() ? className : material + "_" + className;
  }  // end of CMaterialPropertyInterface::getSrcFileName

  void CMaterialPropertyInterface::writeBeginHeaderNamespace(
      std::ostream& os) const {
    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n";
  }  // end of CMaterialPropertyInterface::writeBeginHeaderNamespace

  void CMaterialPropertyInterface::writeEndHeaderNamespace(
      std::ostream& os) const {
    os << "#ifdef __cplusplus\n"
       << "} /* end of extern \"C\" */\n"
       << "#endif /* __cplusplus */\n\n";
  }  // end of CMaterialPropertyInterface::writeEndHeaderNamespace()

  void CMaterialPropertyInterface::writeBeginSrcNamespace(
      std::ostream& os) const {
    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n";
  }  // end of CMaterialPropertyInterface::writeBeginSrcNamespace

  void CMaterialPropertyInterface::writeEndSrcNamespace(
      std::ostream& os) const {
    os << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n";
  }  // end of CMaterialPropertyInterface::writeEndSrcNamespace()

  std::string CMaterialPropertyInterface::getFunctionName(
      const MaterialPropertyDescription& mpd) const {
    const auto material = mpd.material;
    const auto className = mpd.className;
    return material.empty() ? className : material + "_" + className;
  }  // end of CMaterialPropertyInterface::getFunctionName

  bool CMaterialPropertyInterface::requiresCheckBoundsFunction() const {
    return false;
  }

  std::string CMaterialPropertyInterface::getCheckBoundsFunctionName(
      const MaterialPropertyDescription& mpd) const {
    const auto material = mpd.material;
    const auto className = mpd.className;
    if (material.empty()) {
      return className + "_checkBounds";
    }
    return material + "_" + className + "_checkBounds";
  }  // end of CMaterialPropertyInterface::getCheckBoundsFunctionName

  CMaterialPropertyInterface::~CMaterialPropertyInterface() = default;

}  // end of namespace mfront
