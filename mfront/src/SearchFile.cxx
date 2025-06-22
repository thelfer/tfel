/*!
 * \file  mfront/src/SearchFile.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 21 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#if !(defined _WIN32 || defined _WIN64 || defined __CYGWIN__)
#include <unistd.h>
#else
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <io.h>
#define R_OK 4 /* Test for read permission.  */
#define F_OK 0 /* Test for existence.  */
#endif
#include <cstdlib>

#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/SearchFile.hxx"

namespace mfront {

  std::string SearchFile::search(const std::string& f) {
    using namespace tfel::system;
    auto& msf = SearchFile::getSearchFile();
    if (::access(f.c_str(), F_OK) == 0) {
      if (::access(f.c_str(), R_OK) != 0) {
        throw(std::runtime_error("SearchFile::search : '" + f +
                                 "' is not readable"));
      }
      return f;
    }
    for (const auto& p : msf.paths) {
      const auto file = p + dirSeparator() + f;
      if (::access(file.c_str(), F_OK) == 0) {
        if (::access(file.c_str(), R_OK) != 0) {
          throw(std::runtime_error("SearchFile::search : '" + file +
                                   "' is not readable"));
        }
        return file;
      }
    }
    throw(std::runtime_error("SearchFile::search : '" + f +
                             "' has not been found."));
  }

  void SearchFile::addSearchPaths(const std::string& p) {
    using namespace tfel::utilities;
    auto& msf = SearchFile::getSearchFile();
#if defined _WIN32 || defined _WIN64
    const auto npaths = tokenize(p, ';');
#else
    const auto npaths = tokenize(p, ':');
#endif
    msf.paths.insert(msf.paths.begin(), npaths.begin(), npaths.end());
  }  // end of SearchFile::addSearchPaths

  const std::vector<std::string>& SearchFile::getSearchPaths() {
    return SearchFile::getSearchFile().paths;
  }  // end of SearchFile::getSearchPaths

  SearchFile::SearchFile() {
    using namespace tfel::utilities;
    const char* const p = ::getenv("MFRONT_INCLUDE_PATH");
    if (p != nullptr) {
#if defined _WIN32 || defined _WIN64
      const auto npaths = tokenize(p, ';');
#else
      const auto npaths = tokenize(p, ':');
#endif
      this->paths.insert(this->paths.begin(), npaths.begin(), npaths.end());
    }
  }

  SearchFile& SearchFile::getSearchFile() {
    static SearchFile msf;
    return msf;
  }  // end of SearchFile::getSearchFile

}  // end of namespace mfront
