/*!
 * \file   mfront/src/FortranMaterialPropertyInterface.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   01/08/2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/FortranMaterialPropertyInterface.hxx"

namespace mfront {

  std::string FortranMaterialPropertyInterface::getName() { return "fortran"; }

  FortranMaterialPropertyInterface::FortranMaterialPropertyInterface() =
      default;

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  FortranMaterialPropertyInterface::treatKeyword(
      const std::string& key,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator) {
    if (std::find(i.begin(), i.end(), "fortran") != i.end()) {
      tfel::raise_if(key != "@Module",
                     "FortranMaterialPropertyInterface::treatKeyword: "
                     "unsupported key '" +
                         key + "'");
    }
    return {false, current};
  }

  void FortranMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const

  {
    const auto lib = "Fortran" + getMaterialLawLibraryNameBase(mpd);
    const auto name = this->getSrcFileName(mpd.material, mpd.className);
    const auto f = makeLowerCase(mpd.material.empty()
                                     ? mpd.className
                                     : mpd.material + "_" + mpd.className);
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto& l = d.getLibrary(lib);
    insert_if(l.sources, name + ".cxx");
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(l.link_libraries, "m");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    insert_if(l.epts, {f, f + "_checkBounds"});
  }  // end of getTargetsDescription

  std::string FortranMaterialPropertyInterface::getHeaderFileName(
      const std::string&, const std::string&) const {
    return "";
  }  // end of getHeaderFileName

  std::string FortranMaterialPropertyInterface::getSrcFileName(
      const std::string& material, const std::string& className) const {
    if (material.empty()) {
      return className + "-fortran";
    }
    return material + "_" + className + "-fortran";
  }  // end of getSrcFileName

  void FortranMaterialPropertyInterface::writeInterfaceSymbol(
      std::ostream& out, const MaterialPropertyDescription& mpd) const {
    mfront::writeInterfaceSymbol(out, this->getSymbolName(mpd), "Fortran");
  }  // end of FortranMaterialPropertyInterface

  void FortranMaterialPropertyInterface::writeArgumentsList(
      std::ostream& file,
      const MaterialPropertyDescription& mpd,
      const std::string_view,
      const bool) const {
    if (!mpd.inputs.empty()) {
      for (auto p = mpd.inputs.begin(); p != mpd.inputs.end();) {
        file << "const mfront_fortran_real8 * const mfront_argument_"
             << p->name;
        if ((++p) != mpd.inputs.end()) {
          file << ",\n";
        }
      }
    } else {
      file << "void";
    }
  }  // end of writeArgumentsList

  void FortranMaterialPropertyInterface::writeInterfaceSpecificVariables(
      std::ostream& os, const MaterialPropertyDescription& mpd) const {
    const auto use_qt = useQuantities(mpd);
    for (const auto& i : mpd.inputs) {
      os << "const auto " << i.name << " =  ";
      if (use_qt) {
        os << i.type << "(*(mfront_argument_" << i.name << "));\n";
      } else {
        os << "*(mfront_argument_" << i.name << ");\n";
      }
    }
  }  // end of writeInterfaceSpecificVariables

  void FortranMaterialPropertyInterface::writeSrcPreprocessorDirectives(
      std::ostream& os, const MaterialPropertyDescription& mpd) const {
    const auto name = (!mpd.material.empty())
                          ? mpd.material + "_" + mpd.className
                          : mpd.className;
    const auto f77_func =
        (name.find('_') != std::string::npos) ? "F77_FUNC_" : "F77_FUNC";
    writeExportDirectives(os);
    writeF77FUNCMacros(os);
    os << "#define " << this->getFunctionName(mpd) << " " << f77_func << "("
       << makeLowerCase(name) << "," << makeUpperCase(name) << ")\n"
       << "#define " << this->getCheckBoundsFunctionName(mpd) << " F77_FUNC_("
       << makeLowerCase(name) << "_checkbounds," << makeUpperCase(name)
       << "_CHECKBOUNDS)\n\n"
       << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n"
       << "typedef double mfront_fortran_real8;\n\n"
       << "\nMFRONT_SHAREDOBJ double " << this->getFunctionName(mpd) << "(";
    this->writeArgumentsList(os, mpd, "double", false);
    os << ");\n"
       << "MFRONT_SHAREDOBJ int " << this->getCheckBoundsFunctionName(mpd)
       << "(";
    this->writeArgumentsList(os, mpd, "double", false);
    os << ");\n\n"
       << "#ifdef __cplusplus\n"
       << "}\n"
       << "#endif /* __cplusplus */\n\n";
  }  // end of writeSrcPreprocessorDirectives

  void FortranMaterialPropertyInterface::writeBeginHeaderNamespace(
      std::ostream&) const {}  // end of writeBeginHeaderNamespace

  void FortranMaterialPropertyInterface::writeEndHeaderNamespace(
      std::ostream&) const {}  // end of writeEndHeaderNamespace

  void FortranMaterialPropertyInterface::writeBeginSrcNamespace(
      std::ostream& os) const {
    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n";
  }  // end of writeBeginSrcNamespace

  void FortranMaterialPropertyInterface::writeEndSrcNamespace(
      std::ostream& os) const {
    os << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n";
  }  // end of writeEndSrcNamespace

  std::string FortranMaterialPropertyInterface::getFunctionName(
      const MaterialPropertyDescription& mpd) const {
    const auto material = mpd.material;
    const auto className = mpd.className;
    return (material.empty() ? makeUpperCase(className)
                             : makeUpperCase(material + "_" + className)) +
           "_F77";
  }  // end of getFunctionName

  bool FortranMaterialPropertyInterface::requiresCheckBoundsFunction() const {
    return false;
  }  // end of requiresCheckBoundsFunction

  std::string FortranMaterialPropertyInterface::getCheckBoundsFunctionName(
      const MaterialPropertyDescription& mpd) const {
    const auto material = mpd.material;
    const auto className = mpd.className;
    return (material.empty() ? makeUpperCase(className)
                             : makeUpperCase(material + "_" + className)) +
           "_CHECKBOUNDS_F77";
  }  // end of getCheckBoundsFunctionName

  FortranMaterialPropertyInterface::~FortranMaterialPropertyInterface() =
      default;

}  // end of namespace mfront
