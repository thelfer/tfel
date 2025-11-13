/*!
 * \file   MFrontDatabase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   12/11/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <regex>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <functional>
#include <filesystem>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/MFrontDatabase.hxx"

namespace mfront {

  void MFrontDatabase::analyseLibrary(const std::string& l) {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    const auto enames = elm.getEntryPoints(l);
    for (const auto& en : enames) {
      auto ep = EntryPoint{};
      ep.library = l;
      ep.name = en;
      ep.source = elm.getSource(l, en);
      const auto et = elm.getMaterialKnowledgeType(l, en);
      if (et == 0u) {
        ep.type = EntryPoint::MATERIALPROPERTY;
      } else if (et == 1u) {
        ep.type = EntryPoint::BEHAVIOUR;
      } else {
        if (et != 2u) {
          tfel::raise(
              "internal error "
              "(invalid material knowledge type)");
        }
        ep.type = EntryPoint::MODEL;
      }
      this->epts.push_back(std::move(ep));
    }
  }  // end of analyseLibrary

  void MFrontDatabase::analyseLibraries(
      const std::vector<std::string>& libraries) {
    for (const auto& l : libraries) {
      this->analyseLibrary(l);
    }
  }  // end of analyseLibraries

  void MFrontDatabase::analyseDirectory(AnalyseDirectoryResults& r,
                                        const std::string& d,
                                        const AnalyseDirectoryOptions& opts) {
    std::filesystem::path directory(d);
    if (!std::filesystem::exists(d)) {
      if (!opts.ignore_errors) {
        tfel::raise(
            "MFrontDatabase::analyseDirectory: "
            "directory '" +
            d + "' does not exist");
      }
      r.invalid_directories.push_back(d);
      return;
    }
    if (!std::filesystem::is_directory(directory)) {
      if (!opts.ignore_errors) {
        tfel::raise(
            "MFrontDatabase::analyseDirectory: "
            "'" +
            d + "' is not a directory");
      }
      r.invalid_directories.push_back(d);
      return;
    }
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
      if (!entry.is_regular_file()) {
        continue;
      }
      const auto prefix = [&opts]() -> std::string {
        if (opts.library_prefix.has_value()) {
          return *(opts.library_prefix);
        }
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
        return "cyg";
#elif defined _MSC_VER
        return "";
#else
        return "lib";
#endif
      }();
      const auto suffix = [&opts]() -> std::string {
        if (opts.library_suffix.has_value()) {
          return *(opts.library_suffix);
        }
#if defined __APPLE__
        return ".dylib";
#elif defined __CYGWIN__
        return ".cyg";
#elif (defined _WIN32 || defined _WIN64)
        return ".dll";
#else
        return ".so";
#endif
      }();
      const auto p = entry.path();
      const auto l = p.filename().string();
      if ((tfel::utilities::starts_with(l, prefix)) &&
          (tfel::utilities::ends_with(l, suffix))) {
        try {
          this->analyseLibrary(p.string());
        } catch (std::exception& error) {
          if (opts.ignore_errors) {
            r.library_analysis_failures.push_back(
                {.library = l, .error_message = error.what()});
          } else {
            throw;
          }
        }
      }
    }
  }  // end of analyseLibraries

  MFrontDatabase::AnalyseDirectoryResults MFrontDatabase::analyseDirectory(
      const std::string& d) {
    const auto opts = AnalyseDirectoryOptions{};
    return this->analyseDirectory(d, opts);
  }  // end of analyseDirectory

  MFrontDatabase::AnalyseDirectoryResults MFrontDatabase::analyseDirectory(
      const std::string& d, const AnalyseDirectoryOptions& opts) {
    auto r = AnalyseDirectoryResults{};
    this->analyseDirectory(r, d, opts);
    return r;
  }  // end of analyseDirectory

  MFrontDatabase::AnalyseDirectoryResults MFrontDatabase::analyseDirectories(
      const std::vector<std::string>& directories) {
    const auto opts = AnalyseDirectoryOptions{};
    return this->analyseDirectories(directories, opts);
  }  // end of analyseDirectories

  MFrontDatabase::AnalyseDirectoryResults MFrontDatabase::analyseDirectories(
      const std::vector<std::string>& directories,
      const AnalyseDirectoryOptions& opts) {
    auto r = AnalyseDirectoryResults{};
    for (const auto& d : directories) {
      this->analyseDirectory(d, opts);
    }
    return r;
  }  // end of analyseDirectories

  MFrontDatabase::AnalyseDirectoryResults
  MFrontDatabase::analyseDirectoriesListedInEnvironmentVariable(
      const std::string& e) {
    const auto opts = AnalyseDirectoryOptions{};
    return this->analyseDirectoriesListedInEnvironmentVariable(e, opts);
  }  // end of analyseDirectoriesListedInEnvironmentVariable

  MFrontDatabase::AnalyseDirectoryResults
  MFrontDatabase::analyseDirectoriesListedInEnvironmentVariable(
      const std::string& e, const AnalyseDirectoryOptions& opts) {
    auto r = AnalyseDirectoryResults{};
    const char* const p = std::getenv(e.c_str());
    if (p == nullptr) {
      return r;
    }
#if defined _WIN32 || defined _WIN64
    const auto directories = tfel::utilities::tokenize(p, ';');
#else
    const auto directories = tfel::utilities::tokenize(p, ':');
#endif
    return this->analyseDirectories(directories, opts);
  }  // end of analyseDirectoriesListedInEnvironmentVariable

  void MFrontDatabase::clear() { this->epts.clear(); }  // end of clear

  std::vector<MFrontDatabase::EntryPoint> MFrontDatabase::getEntryPoints(
      const Query& q) const {
    using Filter = std::function<bool(const EntryPoint&)>;
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    auto filters = std::vector<Filter>{};
    if (q.type.has_value()) {
      filters.push_back([t = *(q.type)](const EntryPoint& e) noexcept {
        return t == e.type;
      });
    }
    if (q.interface_filter.has_value()) {
      auto expr =
          std::regex{*(q.interface_filter),
                     q.regular_expression_syntax | std::regex_constants::icase};
      filters.push_back([&elm, &q, r = expr](const EntryPoint& e) noexcept {
        return std::regex_match(elm.getInterface(e.library, e.name), r);
      });
    }
    if (q.material_filter.has_value()) {
      auto expr =
          std::regex{*(q.material_filter),
                     q.regular_expression_syntax | std::regex_constants::icase};
      filters.push_back([&elm, &q, r = expr](const EntryPoint& e) noexcept {
        return std::regex_match(elm.getMaterial(e.library, e.name), r);
      });
    }
    if (q.name_filter.has_value()) {
      auto expr = std::regex{*(q.name_filter), q.regular_expression_syntax |
                                                   std::regex_constants::icase};
      filters.push_back([&q, r = expr](const EntryPoint& e) noexcept {
        return std::regex_match(e.name, r);
      });
    }
    // applying filters
    auto r = this->epts;
    for (const auto& f : filters) {
      auto p = std::remove_if(r.begin(), r.end(),
                              [&f](const EntryPoint& e) { return !f(e); });
      r.erase(p, r.end());
    }
    return r;
  }  // end of getEntryPoints

  const std::vector<MFrontDatabase::EntryPoint>&
  MFrontDatabase::getEntryPoints() const {
    return this->epts;
  }  // end of getEntryPoints

}  // end of namespace mfront
