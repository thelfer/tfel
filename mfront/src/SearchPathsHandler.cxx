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
#include <algorithm>
// #include <filesystem>
#ifdef MFRONT_HAVE_MADNEX
#include "Madnex/MFrontDataBase.hxx"
#endif /* MFRONT_HAVE_MADNEX */

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/SearchPathsHandler.hxx"

namespace mfront {

#ifdef MFRONT_HAVE_MADNEX

  static std::string getFileExtension(const std::string& f) {
    const auto pos = f.find(".");
    if (pos != std::string::npos) {
      return f.substr(pos + 1);
    }
    return "";
  }  // end of getFileExtension

#endif /* MFRONT_HAVE_MADNEX */

  static bool fileExistsAndIsReadable(const std::string& f) {
    if (::access(f.c_str(), F_OK) == 0) {
      tfel::raise_if(::access(f.c_str(), R_OK) != 0,
                     "SearchPathsHandler::search : "
                     "'" +
                         f + "' is not readable");
      return true;
    }
    return false;
  }  // end of fileExistsAndIsReadable

  static bool isDirectory(const std::string&) {
    // for the moment (g++8) std::filesystem is not usable without adding
    // adding explicit link to a library. This is too cumbersome to handle,
    // let's fix this later.
    //    return std::filesystem::is_directory(p);
    return true;
  }  // end of isDirectory

  std::string SearchPathsHandler::searchMadnexFile(const std::string& f) {
    using namespace tfel::system;
    auto& msf = SearchPathsHandler::getSearchPathsHandler();
    if (fileExistsAndIsReadable(f)) {
      return f;
    }
    for (const auto& p : msf.paths) {
      if (!p.is<std::string>()) {
        continue;
      }
      const auto file = p.get<std::string>() + dirSeparator() + f;
      if (fileExistsAndIsReadable(file)) {
        return file;
      }
    }
    tfel::raise(
        "SearchPathsHandler::search : "
        "file '" +
        f + "' has not been found.");
  }  // namespace mfront

#ifdef MFRONT_HAVE_MADNEX
  template <std::vector<std::string> (madnex::DataBase::*get)(
      const std::string&) const>
  static std::string getMatchingMaterialKnoweldge(const madnex::DataBase& d,
                                                  const std::string& f,
                                                  const std::string& m,
                                                  const std::string& n,
                                                  const std::string mkt) {
    const auto mks = (d.*get)(m);
    if (std::find(mks.begin(), mks.end(), n) != mks.end()) {
      return "madnex:" + f + ":" + mkt + ":" + m + ":" + n;
    }
    return {};
  }    // end of getMatchingMaterialKnoweldge
#endif /* MFRONT_HAVE_MADNEX */

  std::string SearchPathsHandler::search(const std::string& f) {
    using namespace tfel::system;
    auto& msf = SearchPathsHandler::getSearchPathsHandler();
    if (fileExistsAndIsReadable(f)) {
      return f;
    }
    for (const auto& p : msf.paths) {
      if (p.is<MadnexPath>()) {
#ifdef MFRONT_HAVE_MADNEX
        using ptr = std::vector<std::string>  //
            (madnex::DataBase::*)(const std::string&) const;
        const auto& madnex_path = p.get<MadnexPath>();
        const auto name = [&f] {
          const auto ext = getFileExtension(f);
          if (ext == "mfront") {
            return f.substr(0, f.size() - 7);
          }
          return f;
        }();
        auto db = madnex::DataBase(madnex_path.file_path);
        auto mpath = std::string{};
        if ((madnex_path.mkt == MadnexPath::MADNEX_ALL) ||
            (madnex_path.mkt == MadnexPath::MADNEX_MATERIAL_PROPERTY)) {
          mpath = getMatchingMaterialKnoweldge<static_cast<ptr>(
              &madnex::DataBase::getAvailableMaterialProperties)>(
              db, madnex_path.file_path, madnex_path.material, name,
              "MaterialProperty");
        }
        if ((mpath.empty()) &&
            ((madnex_path.mkt == MadnexPath::MADNEX_ALL) ||
             (madnex_path.mkt == MadnexPath::MADNEX_BEHAVIOUR))) {
          mpath = getMatchingMaterialKnoweldge<static_cast<ptr>(
              &madnex::DataBase::getAvailableMaterialProperties)>(
              db, madnex_path.file_path, madnex_path.material, name,
              "Behaviour");
        }
        if ((mpath.empty()) &&
            ((madnex_path.mkt == MadnexPath::MADNEX_ALL) ||
             (madnex_path.mkt == MadnexPath::MADNEX_MODEL))) {
          mpath = getMatchingMaterialKnoweldge<static_cast<ptr>(
              &madnex::DataBase::getAvailableMaterialProperties)>(
              db, madnex_path.file_path, madnex_path.material, name, "Model");
        }
        if (!mpath.empty()) {
          return mpath;
        }
#else  /* MFRONT_HAVE_MADNEX */
        tfel::raise(
            "SearchPathsHandler::search: madnex support is not enabled");
#endif /* MFRONT_HAVE_MADNEX */
      } else {
        const auto file = p.get<std::string>() + dirSeparator() + f;
        if (fileExistsAndIsReadable(file)) {
          return file;
        }
      }
    }
    tfel::raise(
        "SearchPathsHandler::search : "
        "file '" +
        f + "' has not been found.");
  }

  void SearchPathsHandler::addMadnexSearchPath(const std::string& p) {
#ifdef MFRONT_HAVE_MADNEX
    if (p.empty()) {
      tfel::raise(
          "SearchPathsHandler::addMadnexSearchPaths: "
          "invalid empty path");
    }
    const auto details = tfel::utilities::tokenize(p, ':', true);
    if ((details.empty()) || (details.size() > 3)) {
      tfel::raise(
          "SearchPathsHandler::addMadnexSearchPaths: "
          "invalid path '" +
          p + "'");
    }
    // check if the madnex file exists
    auto& msf = SearchPathsHandler::getSearchPathsHandler();
    const auto file_path = msf.searchMadnexFile(details[0]);
    const auto ext = getFileExtension(file_path);
    if (!((ext == "madnex") || (ext == "mdnx") || (ext == "edf"))) {
      tfel::raise(
          "SearchPathsHandler::addMadnexSearchPaths: "
          "invalid path '" +
          p + "' (invalid file extension for file '" + details[1] + "')");
    }
    if (details.size() > 1) {
      const auto mkt = [&p, &details] {
        if (details[1].empty()) {
          return MadnexPath::MADNEX_ALL;
        } else if ((details[1] == "material_property") ||
                   (details[1] == "MaterialProperty")) {
          return MadnexPath::MADNEX_MATERIAL_PROPERTY;
        } else if ((details[1] == "behaviour") || (details[1] == "Behaviour")) {
          return MadnexPath::MADNEX_BEHAVIOUR;
        } else if ((details[1] == "model") || (details[1] == "Model")) {
          return MadnexPath::MADNEX_MODEL;
        }
        tfel::raise(
            "SearchPathsHandler::addMadnexSearchPaths: "
            "invalid path '" +
            p + "' (invalid file extension for file '" + details[1] + "')");
      }();
      const auto& material_id = [&details] {
        if (details.size() == 3) {
          return details[2];
        }
        return std::string{};
      }();
      if ((material_id.empty()) || (material_id == "<none>")) {
        MadnexPath path;
        path.file_path = file_path;
        path.mkt = mkt;
        msf.paths.push_back(path);
      } else {
        const auto db = madnex::MFrontDataBase{file_path};
        const auto materials = madnex::getMatchingMaterials(db, material_id);
        for (const auto& material : materials) {
          MadnexPath path;
          path.file_path = file_path;
          path.material = material;
          msf.paths.push_back(path);
        }
      }
    } else {
      MadnexPath path;
      path.file_path = file_path;
      msf.paths.push_back(path);
    }
#else  /* MFRONT_HAVE_MADNEX */
    static_cast<void>(p);
    tfel::raise(
        "SearchPathsHandler::addMadnexSearchPaths: "
        "madnex support is not enabled");
#endif /* MFRONT_HAVE_MADNEX */
  }    // end of addMadnexSearchPath

  void SearchPathsHandler::addSearchPaths(const std::string& p) {
    auto& msf = SearchPathsHandler::getSearchPathsHandler();
#if defined _WIN32 || defined _WIN64
    const auto npaths = tfel::utilities::tokenize(p, ';');
#else
    const auto npaths = tfel::utilities::tokenize(p, ':');
#endif
    for (const auto& path : npaths) {
      if (!isDirectory(path)) {
        tfel::raise(
            "SearchPathsHandler::addSearchPaths: "
            "path '" +
            path + "' is not a directory");
      }
    }
    msf.paths.insert(msf.paths.begin(), npaths.begin(), npaths.end());
  }  // end of addSearchPaths

  void SearchPathsHandler::addSearchPathsFromImplementationPaths(
      const std::vector<std::string>& paths) {
    for (const auto& path : paths) {
      if (tfel::utilities::starts_with(path, "madnex:")) {
#ifdef MFRONT_HAVE_MADNEX
        const auto f =
            std::get<0>(decomposeImplementationPathInMadnexFile(path));
        SearchPathsHandler::addMadnexSearchPath(f);
#else  /* MFRONT_HAVE_MADNEX */
        tfel::raise("MFrontBase::addInputPaths: madnex support is not enabled");
#endif /* MFRONT_HAVE_MADNEX */
      }
    }
  }  // end of addSearchPathsFromImplementationPaths

  std::vector<std::string> SearchPathsHandler::getSearchPaths() {
    auto& msf = SearchPathsHandler::getSearchPathsHandler();
    auto directories = std::vector<std::string>{};
    for (const auto& path : msf.paths) {
      if (path.is<std::string>()) {
        directories.push_back(path.get<std::string>());
      }
    }
    return directories;
  }  // end of getSearchPaths

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
  }  // end of SearchPathsHandler

  SearchPathsHandler& SearchPathsHandler::getSearchPathsHandler() {
    static SearchPathsHandler msf;
    return msf;
  }  // end of getSearchPathsHandler

}  // end of namespace mfront
