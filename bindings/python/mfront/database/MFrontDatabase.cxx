/*!
 * \file   MFrontDatabase.cxx
 * \brief
 * \author th202608
 * \date   13/11/2025
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontDatabase.hxx"

#define MFRONT_DATABASE_MODULE_NAME \
  TFEL_PP_JOIN(_mfront_database, TFEL_SUFFIX_FOR_PYTHON_MODULES)

static std::vector<mfront::MFrontDatabase::EntryPoint>
MFrontDatabase_getEntryPoints(const mfront::MFrontDatabase& db,
                              const std::optional<std::string>& type,
                              const std::optional<std::string>& name,
                              const std::optional<std::string>& material,
                              const std::optional<std::string>& interface,
                              const std::optional<std::string>& syntax) {
  const auto s = [&syntax]() -> std::regex_constants::syntax_option_type {
    if (!syntax.has_value()) {
      return std::regex_constants::ECMAScript;
    }
    if (*syntax == "ECMAScript") {
      return std::regex_constants::ECMAScript;
    } else if (*syntax == "basic") {
      return std::regex_constants::basic;
    } else if (*syntax == "extended") {
      return std::regex_constants::extended;
    } else if (*syntax == "awk") {
      return std::regex_constants::awk;
    } else if (*syntax == "grep") {
      return std::regex_constants::grep;
    } else if (*syntax != "egrep") {
      tfel::raise(
          "MFrontDatabase::getEntryPoints: unsupported regular "
          "expression syntax '" +
          *syntax + "'");
    }
    return std::regex_constants::egrep;
  }();
  const auto t =
      [&type]() -> std::optional<
                    mfront::MFrontDatabase::EntryPoint::MaterialKnowledgeType> {
    if (!type.has_value()) {
      return {};
    }
    if ((*type == "material_property") || (*type == "MaterialProperty")) {
      return mfront::MFrontDatabase::EntryPoint::MATERIALPROPERTY;
    }
    if ((*type == "behaviour") || (*type == "Behaviour")) {
      return mfront::MFrontDatabase::EntryPoint::BEHAVIOUR;
    }
    if ((*type != "model") && (*type != "Model")) {
      tfel::raise(
          "MFrontDatabase:getEntryPoints: invalid material knowledge type '" +
          *type + "'");
    }
    return mfront::MFrontDatabase::EntryPoint::MODEL;
  }();
  const auto query =
      mfront::MFrontDatabase::Query{.type = t,
                                    .name_filter = name,
                                    .material_filter = material,
                                    .interface_filter = interface,
                                    .regular_expression_syntax = s};
  return db.getEntryPoints(query);
}  // end of mfront::MFrontDatabase_getEntryPoints

static mfront::MFrontDatabase::AnalyseDirectoryResults
MFrontDatabase_analyseDirectory(
    mfront::MFrontDatabase& db,
    const std::string& d,
    const bool ignore_errors,
    const std::optional<std::string> library_prefix,
    const std::optional<std::string> library_suffix) {
  const auto opts = mfront::MFrontDatabase::AnalyseDirectoryOptions{
      .ignore_errors = ignore_errors,
      .library_prefix = library_prefix,
      .library_suffix = library_suffix};
  return db.analyseDirectory(d, opts);
}  // end of MFrontDatabase_analyseDirectory

static mfront::MFrontDatabase::AnalyseDirectoryResults
MFrontDatabase_analyseDirectories(
    mfront::MFrontDatabase& db,
    const std::vector<std::string>& directories,
    const bool ignore_errors,
    const std::optional<std::string> library_prefix,
    const std::optional<std::string> library_suffix) {
  const auto opts = mfront::MFrontDatabase::AnalyseDirectoryOptions{
      .ignore_errors = ignore_errors,
      .library_prefix = library_prefix,
      .library_suffix = library_suffix};
  return db.analyseDirectories(directories, opts);
}  // end of MFrontDatabase_analyseDirectories

static mfront::MFrontDatabase::AnalyseDirectoryResults
MFrontDatabase_analyseDirectoriesListedInEnvironmentVariable(
    mfront::MFrontDatabase& db,
    const std::string& e,
    const bool ignore_errors,
    const std::optional<std::string> library_prefix,
    const std::optional<std::string> library_suffix) {
  const auto opts = mfront::MFrontDatabase::AnalyseDirectoryOptions{
      .ignore_errors = ignore_errors,
      .library_prefix = library_prefix,
      .library_suffix = library_suffix};
  return db.analyseDirectoriesListedInEnvironmentVariable(e, opts);
}  // end of MFrontDatabase_analyseDirectoriesListedInEnvironmentVariable

#ifdef TFEL_SUFFIX_FOR_PYTHON_MODULES
PYBIND11_MODULE(MFRONT_DATABASE_MODULE_NAME, m) {
#else
PYBIND11_MODULE(_mfront_database, m) {
#endif
  using namespace pybind11::literals;
  using mfront::MFrontDatabase;
  pybind11::enum_<MFrontDatabase::EntryPoint::MaterialKnowledgeType>(
      m, "MFrontDatabaseEntryPointMaterialKnowledgeType")
      .value("MATERIALPROPERTY", MFrontDatabase::EntryPoint::MATERIALPROPERTY)
      .value("BEHAVIOUR", MFrontDatabase::EntryPoint::BEHAVIOUR)
      .value("MODEL", MFrontDatabase::EntryPoint::MODEL);

  pybind11::class_<MFrontDatabase::EntryPoint>(m, "MFrontDatabaseEntryPoint")
      .def_readonly("library", &MFrontDatabase::EntryPoint::library)
      .def_readonly("name", &MFrontDatabase::EntryPoint::name)
      .def_readonly("source", &MFrontDatabase::EntryPoint::source)
      .def_readonly("source", &MFrontDatabase::EntryPoint::type);

  pybind11::class_<
      MFrontDatabase::AnalyseDirectoryResults::LibraryAnalysisFailureReport>(
      m, "MFrontDatabaseAnalyseDirectoryResultsLibraryAnalysisFailureReport")
      .def_readonly("library", &MFrontDatabase::AnalyseDirectoryResults::
                                   LibraryAnalysisFailureReport::library)
      .def_readonly("error_message",
                    &MFrontDatabase::AnalyseDirectoryResults::
                        LibraryAnalysisFailureReport::error_message);

  pybind11::class_<MFrontDatabase::AnalyseDirectoryResults>(
      m, "MFrontDatabaseAnalyseDirectoryResults")
      .def_readonly(
          "library_analysis_failures",
          &MFrontDatabase::AnalyseDirectoryResults::library_analysis_failures)
      .def_readonly(
          "invalid_directories",
          &MFrontDatabase::AnalyseDirectoryResults::invalid_directories);

  pybind11::class_<MFrontDatabase>(m, "MFrontDatabase")
      .def(pybind11::init<>())
      .def("analyseLibrary", &MFrontDatabase::analyseLibrary)
      .def("analyseLibraries", &MFrontDatabase::analyseLibraries)
      .def("analyseDirectory", pybind11::overload_cast<const std::string&>(
                                   &MFrontDatabase::analyseDirectory))
      .def("analyseDirectory", MFrontDatabase_analyseDirectory, "directory"_a,
           "ignore_errors"_a = false,
           "library_prefix"_a = std::optional<std::string>{},
           "library_suffix"_a = std::optional<std::string>{})
      .def("analyseDirectories", MFrontDatabase_analyseDirectories,
           "directories"_a, "ignore_errors"_a = false,
           "library_prefix"_a = std::optional<std::string>{},
           "library_suffix"_a = std::optional<std::string>{})
      .def("analyseDirectories",
           pybind11::overload_cast<const std::vector<std::string>&>(
               &MFrontDatabase::analyseDirectories))
      .def("analyseDirectoriesListedInEnvironmentVariable",
           MFrontDatabase_analyseDirectoriesListedInEnvironmentVariable,
           "directories"_a, "ignore_errors"_a = false,
           "library_prefix"_a = std::optional<std::string>{},
           "library_suffix"_a = std::optional<std::string>{})
      .def("analyseDirectoriesListedInEnvironmentVariable",
           pybind11::overload_cast<const std::string&>(
               &MFrontDatabase::analyseDirectoriesListedInEnvironmentVariable))
      .def("getEntryPoints", MFrontDatabase_getEntryPoints,
           "type"_a = std::optional<std::string>{},
           "name"_a = std::optional<std::string>{},
           "material"_a = std::optional<std::string>{},
           "interface"_a = std::optional<std::string>{},
           "regular_expression_syntax"_a = std::optional<std::string>{})
      .def("getEntryPoints",
           static_cast<const std::vector<MFrontDatabase::EntryPoint>& (
               MFrontDatabase::*)() const>(&MFrontDatabase::getEntryPoints));
}
