/*!
 * \file   mfront/src/FortranMaterialPropertyInterface.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   01 déc 2008
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

#include "TFEL/Config/GetInstallPath.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/FortranMaterialPropertyInterface.hxx"

namespace mfront {

  std::string FortranMaterialPropertyInterface::getName() { return "fortran"; }

  FortranMaterialPropertyInterface::FortranMaterialPropertyInterface()
      : CMaterialPropertyInterfaceBase() {}

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  FortranMaterialPropertyInterface::treatKeyword(
      const std::string& key,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator) {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error(
            "FortranMaterialPropertyInterface::treatKeyword : " + m));
      }
    };
    if (std::find(i.begin(), i.end(), "fortran") != i.end()) {
      throw_if(key != "@Module", "unsupported key '" + key + "'");
    }
    return {false, current};
  }

  void FortranMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd)

  {
    const auto lib =
        "Fortran" + getMaterialLawLibraryNameBase(mpd.library, mpd.material);
    const auto name = this->getSrcFileName(mpd.material, mpd.className);
    const auto f = makeLowerCase(mpd.material.empty()
                                     ? mpd.className
                                     : mpd.material + "_" + mpd.className);
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(d[lib].ldflags, "-lm");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    insert_if(d[lib].sources, name + ".cxx");
    insert_if(d[lib].epts, {f, f + "_checkBounds"});
  }  // end of FortranMaterialPropertyInterface::getTargetsDescription

  std::string FortranMaterialPropertyInterface::getHeaderFileName(
      const std::string&, const std::string&) const {
    return "";
  }  // end of FortranMaterialPropertyInterface::getHeaderFileName

  std::string FortranMaterialPropertyInterface::getSrcFileName(
      const std::string& material, const std::string& className) const {
    if (material.empty()) {
      return className + "-fortran";
    }
    return material + "_" + className + "-fortran";
  }  // end of FortranMaterialPropertyInterface::getSrcFileName

  void FortranMaterialPropertyInterface::writeInterfaceSpecificVariables(
      const VariableDescriptionContainer& inputs) {
    for (const auto& i : inputs) {
      this->srcFile << "const mfront_fortran_real8 " << i.name
                    << " =  *(_mfront_var_" << i.name << ");\n";
    }
  }  // end of FortranMaterialPropertyInterface::writeInterfaceSpecificVariables

  void FortranMaterialPropertyInterface::writeParameterList(
      std::ostream& file, const VariableDescriptionContainer& inputs) {
    if (!inputs.empty()) {
      for (auto p = inputs.begin(); p != inputs.end();) {
        file << "const mfront_fortran_real8 * const _mfront_var_" << p->name;
        if ((++p) != inputs.end()) {
          file << ",\n";
        }
      }
    } else {
      file << "void";
    }
  }  // end of FortranMaterialPropertyInterface::writeParameterList

  void FortranMaterialPropertyInterface::writeSrcPreprocessorDirectives(
      const MaterialPropertyDescription& mpd) {
    const auto name = (!mpd.material.empty())
                          ? mpd.material + "_" + mpd.className
                          : mpd.className;
    const auto f77_func =
        (name.find('_') != std::string::npos) ? "F77_FUNC_" : "F77_FUNC";
    writeExportDirectives(this->srcFile);
    writeF77FUNCMacros(this->srcFile);
    this->srcFile << "#define "
                  << this->getFunctionName(mpd.material, mpd.className) << " "
                  << f77_func << "(" << makeLowerCase(name) << ","
                  << makeUpperCase(name) << ")\n"
                  << "#define "
                  << this->getCheckBoundsFunctionName(mpd.material,
                                                      mpd.className)
                  << " F77_FUNC_(" << makeLowerCase(name) << "_checkbounds,"
                  << makeUpperCase(name) << "_CHECKBOUNDS)\n\n"
                  << "#ifdef __cplusplus\n"
                  << "extern \"C\"{\n"
                  << "#endif /* __cplusplus */\n\n"
                  << "typedef double mfront_fortran_real8;\n\n"
                  << "\nMFRONT_SHAREDOBJ double "
                  << this->getFunctionName(mpd.material, mpd.className) << "(";
    this->writeParameterList(this->srcFile, mpd.inputs);
    this->srcFile << ");\n"
                  << "MFRONT_SHAREDOBJ int "
                  << this->getCheckBoundsFunctionName(mpd.material,
                                                      mpd.className)
                  << "(";
    this->writeParameterList(this->srcFile, mpd.inputs);
    this->srcFile << ");\n\n"
                  << "#ifdef __cplusplus\n"
                  << "}\n"
                  << "#endif /* __cplusplus */\n\n";
  }  // end of FortranMaterialPropertyInterface::writeSrcPreprocessorDirectives

  void FortranMaterialPropertyInterface::writeBeginHeaderNamespace() {
  }  // end of FortranMaterialPropertyInterface::writeBeginHeaderNamespace

  void FortranMaterialPropertyInterface::writeEndHeaderNamespace() {
  }  // end of FortranMaterialPropertyInterface::writeEndHeaderNamespace(void)

  void FortranMaterialPropertyInterface::writeBeginSrcNamespace() {
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "extern \"C\"{\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
  }  // end of FortranMaterialPropertyInterface::writeBeginSrcNamespace

  void FortranMaterialPropertyInterface::writeEndSrcNamespace() {
    this->srcFile << "#ifdef __cplusplus\n";
    this->srcFile << "} // end of extern \"C\"\n";
    this->srcFile << "#endif /* __cplusplus */\n\n";
  }  // end of FortranMaterialPropertyInterface::writeEndSrcNamespace(void)

  std::string FortranMaterialPropertyInterface::getFunctionName(
      const std::string& material, const std::string& className) const {
    return (material.empty() ? makeUpperCase(className)
                             : makeUpperCase(material + "_" + className)) +
           "_F77";
  }  // end of FortranMaterialPropertyInterface::getFunctionName

  bool FortranMaterialPropertyInterface::requiresCheckBoundsFunction() const {
    return false;
  }

  std::string FortranMaterialPropertyInterface::getCheckBoundsFunctionName(
      const std::string& material, const std::string& className) const {
    return (material.empty() ? makeUpperCase(className)
                             : makeUpperCase(material + "_" + className)) +
           "_CHECKBOUNDS_F77";
  }  // end of FortranMaterialPropertyInterface::getCheckBoundsFunctionName

  FortranMaterialPropertyInterface::~FortranMaterialPropertyInterface() =
      default;

}  // end of namespace mfront
