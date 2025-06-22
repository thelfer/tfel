/*!
 * \file  mfront/src/SearchPathsHandler.cxx
 * \brief
 * \author Thomas Helfer
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

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/SearchPathsHandler.hxx"

namespace mfront {

  std::string SearchPathsHandler::search(const std::string& f) {
    using namespace tfel::system;
    auto& msf = SearchPathsHandler::getSearchPathsHandler();
    if (::access(f.c_str(), F_OK) == 0) {
      tfel::raise_if(::access(f.c_str(), R_OK) != 0,
                     "SearchPathsHandler::search : "
                     "'" +
                         f + "' is not readable");
      return f;
    }
    for (const auto& p : msf.paths) {
      const auto file = p + dirSeparator() + f;
      if (::access(file.c_str(), F_OK) == 0) {
        tfel::raise_if(::access(file.c_str(), R_OK) != 0,
                       "SearchPathsHandler::search : "
                       "'" +
                           file + "' is not readable");
        return file;
      }
    }
    tfel::raise(
        "SearchPathsHandler::search : "
        "'" +
        f + "' has not been found.");
  }

  void SearchPathsHandler::addSearchPaths(const std::string& p) {
    auto& msf = SearchPathsHandler::getSearchPathsHandler();
#if defined _WIN32 || defined _WIN64
    const auto npaths = tfel::utilities::tokenize(p, ';');
#else
    const auto npaths = tfel::utilities::tokenize(p, ':');
#endif
    msf.paths.insert(msf.paths.begin(), npaths.begin(), npaths.end());
  }  // end of SearchPathsHandler::addSearchPaths

  const std::vector<std::string>& SearchPathsHandler::getSearchPaths() {
    return SearchPathsHandler::getSearchPathsHandler().paths;
  }  // end of SearchPathsHandler::getSearchPaths

  SearchPathsHandler::SearchPathsHandler() {
    const char* const p = ::getenv("MFRONT_INCLUDE_PATH");
    if (p != nullptr) {
#if defined _WIN32 || defined _WIN64
      const auto npaths = tfel::utilities::tokenize(p, ';');
#else
      const auto npaths = tfel::utilities::tokenize(p, ':');
#endif
      this->paths.insert(this->paths.begin(), npaths.begin(), npaths.end());
    }
  }

  SearchPathsHandler& SearchPathsHandler::getSearchPathsHandler() {
    static SearchPathsHandler msf;
    return msf;
  }  // end of SearchPathsHandler::getSearchPathsHandler

}  // end of namespace mfront
