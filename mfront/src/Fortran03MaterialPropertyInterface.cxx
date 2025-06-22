/*!
 * \file   mfront/src/Fortran03MaterialPropertyInterface.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   01 dÃƒÆ’Ã‚Â©c 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <map>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include <sys/types.h>
#include <sys/stat.h>
#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
#include <unistd.h>
#else
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h>
#define F_OK 0 /* Test for existence.  */
#ifndef S_ISREG
#define S_ISREG(mode) (((mode)&S_IFMT) == S_IFREG)
#endif
#endif

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/MFrontWarningMode.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontLock.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/Fortran03MaterialPropertyInterface.hxx"

namespace mfront {

  std::string Fortran03MaterialPropertyInterface::getName() {
    return "fortran03";
  }

  Fortran03MaterialPropertyInterface::Fortran03MaterialPropertyInterface() =
      default;

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  Fortran03MaterialPropertyInterface::treatKeyword(
      const std::string& key,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator end) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "Fortran03MaterialPropertyInterface::treatKeyword : " + m);
    };
    if (std::find(i.begin(), i.end(), "fortran03") != i.end()) {
      throw_if(key != "@Module", "unsupported key '" + key + "'");
    }
    if (key == "@Module") {
      if (i.empty()) {
        reportWarning("keyword '" + key +
                      "' is used without being restricted to the " +
                      this->getName() +
                      " interface, which could be a portability "
                      "issue. Please add [" +
                      this->getName() + "] after the keyword (i.e. replace '" +
                      key + "' by '" + key + "[" + this->getName() + "]')");
      }
      throw_if(!this->module.empty(), "module name already defined");
      throw_if(current == end, "unexpected end of file");
      const auto p = current->value;
      throw_if(!tfel::utilities::CxxTokenizer::isValidIdentifier(p, true),
               "invalid module name '" + p + "'");
      throw_if(++current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + "'");
      ++(current);
      this->module = p;
      return {true, current};
    }
    return {false, current};
  }  // end of treatKeyword

  void Fortran03MaterialPropertyInterface::getTargetsDescription(
      TargetsDescription& d, const MaterialPropertyDescription& mpd) const

  {
    auto fmname = (mpd.material.empty()) ? "mfront_mp" : mpd.material;
    const auto lib = "Fortran03" + getMaterialLawLibraryNameBase(mpd);
    const auto name = this->getSrcFileName(mpd.material, mpd.className);
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto& l = d.getLibrary(lib);
    insert_if(l.sources, name + ".cxx");
#if !((defined _WIN32) && (defined _MSC_VER))
    insert_if(l.link_libraries, "m");
#endif /* !((defined _WIN32) && (defined _MSC_VER)) */
    insert_if(l.epts, {fmname + "::" + mpd.law,
                       fmname + "::" + mpd.law + "_checkBounds"});
  }  // end of getTargetsDescription

  std::string Fortran03MaterialPropertyInterface::getHeaderFileName(
      const std::string&, const std::string&) const {
    return "";
  }  // end of getHeaderFileName

  std::string Fortran03MaterialPropertyInterface::getSrcFileName(
      const std::string& material, const std::string& className) const {
    if (material.empty()) {
      return className + "-fortran03";
    }
    return material + "_" + className + "-fortran03";
  }  // end of getSrcFileName

  void Fortran03MaterialPropertyInterface::writeInterfaceSymbol(
      std::ostream& out, const MaterialPropertyDescription& mpd) const {
    mfront::writeInterfaceSymbol(out, this->getSymbolName(mpd), "Fortran03");
  }  // end of writeInterfaceSymbol

  void Fortran03MaterialPropertyInterface::writeArgumentsList(
      std::ostream& os,
      const MaterialPropertyDescription& mpd,
      const std::string_view,
      const bool) const {
    if (!mpd.inputs.empty()) {
      for (auto p = mpd.inputs.begin(); p != mpd.inputs.end();) {
        os << "const double * const mfront_argument_" << p->name;
        if ((++p) != mpd.inputs.end()) {
          os << ",\n";
        }
      }
    } else {
      os << "void";
    }
  }  // end of writeArgumentsList

  void Fortran03MaterialPropertyInterface::writeInterfaceSpecificVariables(
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

  void Fortran03MaterialPropertyInterface::writeSrcPreprocessorDirectives(
      std::ostream& os, const MaterialPropertyDescription& mpd) const {
    writeExportDirectives(os);
    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n"
       << "MFRONT_SHAREDOBJ double " << this->getFunctionName(mpd) << "(";
    this->writeArgumentsList(os, mpd, "double", false);
    os << ");\n"
       << "MFRONT_SHAREDOBJ int " << this->getCheckBoundsFunctionName(mpd)
       << "(";
    this->writeArgumentsList(os, mpd, "double", false);
    os << ");\n\n"
       << "#ifdef __cplusplus\n"
       << "}\n"
       << "#endif /* __cplusplus */\n\n";
  }  // end of
     // Fortran03MaterialPropertyInterface::writeSrcPreprocessorDirectives

  void Fortran03MaterialPropertyInterface::writeBeginHeaderNamespace(
      std::ostream&) const {}  // end of writeBeginHeaderNamespace

  void Fortran03MaterialPropertyInterface::writeEndHeaderNamespace(
      std::ostream&) const {}  // end of writeEndHeaderNamespace()

  void Fortran03MaterialPropertyInterface::writeBeginSrcNamespace(
      std::ostream& os) const {
    os << "#ifdef __cplusplus\n"
       << "extern \"C\"{\n"
       << "#endif /* __cplusplus */\n\n";
  }  // end of writeBeginSrcNamespace

  void Fortran03MaterialPropertyInterface::writeEndSrcNamespace(
      std::ostream& os) const {
    os << "#ifdef __cplusplus\n"
       << "} // end of extern \"C\"\n"
       << "#endif /* __cplusplus */\n\n";
  }  // end of writeEndSrcNamespace()

  std::string Fortran03MaterialPropertyInterface::getFunctionName(
      const MaterialPropertyDescription& mpd) const {
    const auto material = mpd.material;
    const auto className = mpd.className;
    return ((!material.empty()) ? material + "_" : "") + className + '_';
  }  // end of getFunctionName

  bool Fortran03MaterialPropertyInterface::requiresCheckBoundsFunction() const {
    return false;
  }

  std::string Fortran03MaterialPropertyInterface::getCheckBoundsFunctionName(
      const MaterialPropertyDescription& mpd) const {
    return this->getFunctionName(mpd) + "checkBounds_";
  }  // end of getCheckBoundsFunctionName

  void Fortran03MaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "Fortran03MaterialPropertyInterface::writeOutputFiles : " + m);
    };
    CMaterialPropertyInterfaceBase::writeOutputFiles(mpd, fd);
    tfel::system::systemCall::mkdir("fortran");
    // writing fortran module
    MFrontLockGuard lock;
    struct function {
      std::string name;
      size_t n_args;
    };
    auto fcts = std::map<std::string, function>{};
    auto fmname = !this->module.empty()
                      ? this->module
                      : ((mpd.material.empty()) ? "mfront_mp" : mpd.material);
    auto fmmlst = "src/" + fmname + "-fortran03.lst";
    struct stat buffer;  // for call to stat
    if (access(fmmlst.c_str(), F_OK) == 0) {
      stat(fmmlst.c_str(), &buffer);
      throw_if(!S_ISREG(buffer.st_mode),
               "'" + fmmlst + "' is not a regular file");
      std::ifstream f(fmmlst);
      throw_if(!f, "unable to open '" + fmmlst + "'");
      auto line = std::string{};
      while (std::getline(f, line)) {
        std::istringstream iss{line};
        iss.exceptions(std::ios::badbit | std::ios::failbit);
        auto m = std::string{};  // method name
        function fct;            // function description
        iss >> m >> fct.name >> fct.n_args;
        fcts.insert({m, fct});
      }
      f.close();
    }
    if (fmname == mpd.material) {
      fcts[mpd.law] = {this->getFunctionName(mpd), mpd.inputs.size()};
    } else {
      const auto fname =
          (mpd.material.empty() ? "" : mpd.material + '_') + mpd.law;
      fcts[fname] = {this->getFunctionName(mpd), mpd.inputs.size()};
    }
    std::ofstream mf{fmmlst};
    throw_if(!mf, "unable to open '" + fmmlst + "'");
    mf.exceptions(std::ios::badbit | std::ios::failbit);
    for (const auto& m : fcts) {
      mf << m.first << " " << m.second.name << " " << m.second.n_args << "\n";
    }
    mf.close();
    std::ofstream fm{"fortran/" + fmname + ".inc"};
    throw_if(!mf, "unable to open 'fortran/" + fmname + ".inc'");
    fm.exceptions(std::ios::badbit | std::ios::failbit);
    fm << "Module " << fmname << "\n"
       << "  Interface\n";
    for (const auto& m : fcts) {
      fm << "    Function " << m.first << "(";
      for (decltype(m.second.n_args) i = 0; i != m.second.n_args; ++i) {
        if (i != 0) {
          fm << ",";
        }
        fm << "f_args" << i;
      }
      fm << ") BIND (C,NAME='" << m.second.name << "')\n"
         << "      Use iso_c_binding, only: c_double\n"
         << "      Real(kind=c_double) :: " << m.first << "\n";
      for (decltype(m.second.n_args) i = 0; i != m.second.n_args; ++i) {
        fm << "      Real(kind=c_double),intent(in) :: f_args" << i << "\n";
      }
      fm << "    End Function " << m.first << "\n";
      fm << "    Function " << m.first << "_checkBounds(";
      for (decltype(m.second.n_args) i = 0; i != m.second.n_args; ++i) {
        if (i != 0) {
          fm << ",";
        }
        fm << "f_args" << i;
      }
      fm << ") BIND (C,NAME='" << m.second.name << "_checkBounds')\n"
         << "      Use iso_c_binding, only: c_double,c_int\n"
         << "      Integer(kind=c_int) :: " << m.first << "_checkBounds\n";
      for (decltype(m.second.n_args) i = 0; i != m.second.n_args; ++i) {
        fm << "      Real(kind=c_double),intent(in) :: f_args" << i << "\n";
      }
      fm << "    End Function " << m.first << "_checkBounds\n";
    }
    fm << "  End Interface\n"
       << "End Module " << fmname << '\n';
    fm.close();
  }

  Fortran03MaterialPropertyInterface::~Fortran03MaterialPropertyInterface() =
      default;

}  // end of namespace mfront
