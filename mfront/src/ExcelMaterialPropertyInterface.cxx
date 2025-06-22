/*!
 * \file   mfront/src/ExcelMaterialPropertyInterface.cxx
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

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <string>
#include <set>

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
#include "TFEL/System/System.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontLock.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/CMaterialPropertyInterface.hxx"
#include "MFront/ExcelMaterialPropertyInterface.hxx"

namespace mfront {

  struct ExcelInterface {
    ExcelInterface() = default;
    ExcelInterface(ExcelInterface&&) = default;
    ExcelInterface(const ExcelInterface&) = default;
    ExcelInterface& operator=(ExcelInterface&&) = default;
    ExcelInterface& operator=(const ExcelInterface&) = default;
    ~ExcelInterface() = default;
    std::vector<std::string> variables;
    std::string function;
    std::string library;
  };  // end of ExcelInterface

  static void readExcelInterface(std::vector<ExcelInterface>& interfaces,
                                 const std::string& line) {
    if (!line.empty()) {
      std::istringstream tokenizer(line);
      auto tokens = std::vector<std::string>{};
      copy(std::istream_iterator<std::string>(tokenizer),
           std::istream_iterator<std::string>(), back_inserter(tokens));
      tfel::raise_if(tokens.size() < 2,
                     "readExcelInterface: "
                     "invalid line '" +
                         line + "'");
      ExcelInterface i;
      i.function = tokens[0];
      i.library = tokens[1];
      i.variables.insert(i.variables.end(), tokens.cbegin() + 2, tokens.cend());
      interfaces.push_back(i);
    }
  }  // end of readExcelInterface

  std::string ExcelMaterialPropertyInterface::getName() { return "excel"; }

  ExcelMaterialPropertyInterface::~ExcelMaterialPropertyInterface() = default;

  std::pair<bool, ExcelMaterialPropertyInterface::tokens_iterator>
  ExcelMaterialPropertyInterface::treatKeyword(
      const std::string& k,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator) {
    tfel::raise_if(std::find(i.begin(), i.end(), "excel") != i.end(),
                   "ExcelMaterialPropertyInterface::treatKeyword: "
                   "unsupported key '" +
                       k + "'");
    return {false, current};
  }  // end of treatKeyword

  void ExcelMaterialPropertyInterface::getTargetsDescription(
      TargetsDescription&, const MaterialPropertyDescription&) const {
  }  // end of ExcelMaterialPropertyInterface::getTargetsDescription

  void ExcelMaterialPropertyInterface::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription&) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "ExcelMaterialPropertyInterface::writeOutputFiles: " + m);
    };
    // writing excel interface
    MFrontLockGuard lock;
    auto interfaces = std::vector<ExcelInterface>{};
    auto interfaces2 = std::map<std::string, std::vector<ExcelInterface>>{};
    const auto fname = std::string{"src/excel.lst"};
    const auto name =
        (mpd.material.empty() ? "" : mpd.material + "_") + mpd.className;
    struct stat buffer;  // for call to stat
    if (access(fname.c_str(), F_OK) == 0) {
      throw_if(stat(fname.c_str(), &buffer) == -1,
               "can't stat file '" + fname + "'");
      throw_if(!S_ISREG(buffer.st_mode),
               "'" + fname + "' is not a regular file");
      std::fstream iwrapper(fname);
      throw_if(!iwrapper, "unable to open '" + fname + "'");
      while (!iwrapper.eof()) {
        throw_if(!iwrapper.good(), "error while reading file '" + fname + "'");
        auto line = std::string{};
        getline(iwrapper, line);
        readExcelInterface(interfaces, line);
      }
      iwrapper.close();
    }
    const auto lib = "Excel" + getMaterialLawLibraryNameBase(mpd);
    auto p2 = interfaces.end();
    for (auto p = interfaces.begin(); p != interfaces.end(); ++p) {
      if ((p->function == name) && (p->library == lib)) {
        p2 = p;
      }
    }
    if (p2 == interfaces.end()) {
      interfaces.resize(interfaces.size() + 1);
      p2 = interfaces.end();
      --p2;
    }
    p2->function = name;
    p2->library = lib;
    p2->variables.clear();
    for (const auto& v : mpd.inputs) {
      p2->variables.push_back(v.name);
    }
    std::ofstream wrapper(fname);
    throw_if(!wrapper, "unable to open '" + fname + "'");
    for (const auto& i : interfaces) {
      wrapper << i.function << " " << i.library << " ";
      copy(i.variables.begin(), i.variables.end(),
           std::ostream_iterator<std::string>(wrapper, " "));
      interfaces2[i.library].push_back(i);
      wrapper << '\n';
    }
    for (const auto& i : interfaces2) {
      const auto& libName = i.first;
      std::ofstream file("src/" + libName + ".bas");
      throw_if(!file, "unable to open file 'src/" + libName + ".bas'");
      for (const auto& f : i.second) {
        file << "Declare Function " << f.function << " Lib \"" << libName
             << ".dll\" (";
        for (auto p6 = f.variables.cbegin(); p6 != f.variables.cend();) {
          file << "ByVal " << *p6 << " As Double";
          if (++p6 != f.variables.end()) {
            file << ",";
          }
        }
        file << ") As Double\n";
      }
    }
    wrapper.close();
  }  // end of ExcelMaterialPropertyInterface::writeSrcFile()

}  // end of namespace mfront
