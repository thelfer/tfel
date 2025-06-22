/*!
 * \file  mfront-query/src/mfront-query.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#ifdef small
#undef small
#endif /* small */
#endif /* __CYGWIN__ */

#include <regex>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#ifdef MFRONT_QUERY_HAVE_MADNEX
#include "Madnex/MFrontDataBase.hxx"
#include "Madnex/MFrontImplementation.hxx"
#endif /* MFRONT_QUERY_HAVE_MADNEX */

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "MFront/InitDSLs.hxx"
#include "MFront/InitInterfaces.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontBase.hxx"
#include "MFront/MaterialPropertyQuery.hxx"
#include "MFront/BehaviourQuery.hxx"
#include "MFront/ModelQuery.hxx"
#include "MFront/AbstractDSL.hxx"
#include "MFront/MaterialPropertyDSL.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/ModelDSL.hxx"
#include "MFront/PathSpecifier.hxx"
#include "MFront/SearchPathsHandler.hxx"

#ifdef MFRONT_QUERY_HAVE_MADNEX

using GetAvailableImplementationsPtr1 = std::vector<std::string> (
    madnex::MFrontDataBase::*)(const std::string&) const;
using GetAvailableImplementationsPtr2 =
    std::map<std::string, std::vector<std::string>> (
        madnex::MFrontDataBase::*)() const;

static bool checkMadnexFileExtension(const mfront::PathSpecifier& p) {
  const auto ext = [p]() -> std::string {
    const auto pos = p.file.rfind(".");
    if (pos != std::string::npos) {
      return p.file.substr(pos + 1);
    }
    return "";
  }();
  return (ext == "madnex") || (ext == "mdnx") || (ext == "edf");
}  // end of checkMadnexFileExtension

static void listImplementations1(const mfront::PathSpecifier& p,
                                 const std::string& itypes,
                                 const std::vector<std::string>& impls) {
  if (!impls.empty()) {
    if (p.material_identifier == "<none>") {
      std::cout << "- " + itypes + " associated with no material:\n";
    } else {
      std::cout << "- " + itypes + " associated with material '" +
                       p.material_identifier + "':\n";
    }
    for (const auto& i : impls) {
      std::cout << "    - " << i << '\n';
    }
  }
}

static void listImplementations2(
    const std::string& itypes,
    const std::map<std::string, std::vector<std::string>>&
        impls_sorted_by_materials) {
  for (const auto& impls : impls_sorted_by_materials) {
    if (impls.first.empty()) {
      std::cout << "- " + itypes + " associated with no material:\n";
    } else {
      std::cout << "- " + itypes + " associated with material '" + impls.first +
                       "':\n";
    }
    for (const auto& i : impls.second) {
      std::cout << "    - " << i << '\n';
    }
  }
}

static void listImplementations3(
    const std::map<std::string, std::vector<std::string>>&
        impls_sorted_by_materials) {
  for (const auto& impls : impls_sorted_by_materials) {
    bool first = true;
    for (const auto& i : impls.second) {
      if (!first) {
        std::cout << " ";
      }
      first = false;
      std::cout << i;
    }
    std::cout << '\n';
  }
}
static void listImplementations(const mfront::PathSpecifier& p,
                                const std::string& itypes,
                                const GetAvailableImplementationsPtr1 m1,
                                const GetAvailableImplementationsPtr2 m2,
                                const bool sorted_by_materials) {
  if (!p.material_property_identifier.empty()) {
    tfel::raise(
        "mfront-query: specifying a material property "
        "is not meaningful when listing " +
        itypes);
  }
  if (!p.behaviour_identifier.empty()) {
    tfel::raise(
        "mfront-query: specifying a behaviour "
        "is not meaningful when listing " +
        itypes);
  }
  if (!p.model_identifier.empty()) {
    tfel::raise(
        "mfront-query: specifying a model "
        "is not meaningful when listing " +
        itypes);
  }
  if (!checkMadnexFileExtension(p)) {
    tfel::raise("mfront-query: listing " + itypes +
                " is only supported for madnex files");
  }
  auto d = madnex::MFrontDataBase{p.file};
  if (sorted_by_materials) {
    if (!p.material_identifier.empty()) {
      const auto impls = (d.*m1)(
          p.material_identifier == "<none>" ? "" : p.material_identifier);
      listImplementations1(p, itypes, impls);
    } else {
      listImplementations2(itypes, (d.*m2)());
    }
  } else {
    listImplementations3((d.*m2)());
  }
}  // end of listImplementations

static void listMaterialProperties(const mfront::PathSpecifier& p,
                                   const bool sorted_by_materials) {
  GetAvailableImplementationsPtr1 m1 =
      &madnex::MFrontDataBase::getAvailableMaterialProperties;
  GetAvailableImplementationsPtr2 m2 =
      &madnex::MFrontDataBase::getAvailableMaterialProperties;
  listImplementations(p, "material properties", m1, m2, sorted_by_materials);
}  // end of listMaterialProperties

static void listBehaviours(const mfront::PathSpecifier& p,
                           const bool sorted_by_materials) {
  GetAvailableImplementationsPtr1 m1 =
      &madnex::MFrontDataBase::getAvailableBehaviours;
  GetAvailableImplementationsPtr2 m2 =
      &madnex::MFrontDataBase::getAvailableBehaviours;
  listImplementations(p, "behaviours", m1, m2, sorted_by_materials);
}  // end of listBehaviours

static void listModels(const mfront::PathSpecifier& p,
                       const bool sorted_by_materials) {
  GetAvailableImplementationsPtr1 m1 =
      &madnex::MFrontDataBase::getAvailableModels;
  GetAvailableImplementationsPtr2 m2 =
      &madnex::MFrontDataBase::getAvailableModels;
  listImplementations(p, "models", m1, m2, sorted_by_materials);
}  // end of listModels

static void listMaterials(const mfront::PathSpecifier& p) {
  if (!checkMadnexFileExtension(p)) {
    tfel::raise(
        "mfront-query: listing materials is only supported for madnex files");
  }
  auto d = madnex::MFrontDataBase{p.file};
  auto first = true;
  auto display = [&first](const std::string& m) {
    if (!first) {
      std::cout << " ";
    }
    first = false;
    std::cout << m;
  };
  for (const auto& m : d.getMaterialsList()) {
    display(m);
  }
  // looking for material knowledge associated with no materials.
  auto none = false;
  auto check =
      [&none](const std::map<std::string, std::vector<std::string>>& impls) {
        if (none) {
          return;
        }
        for (const auto& kv : impls) {
          if (kv.first.empty()) {
            none = true;
            return;
          }
        }
      };
  check(d.getAvailableMaterialProperties());
  check(d.getAvailableBehaviours());
  check(d.getAvailableModels());
  if (none) {
    display("<none>");
  }
  std::cout << '\n';
}  // end of listMaterials

static void listImplementationPaths(const mfront::PathSpecifier& p,
                                    const bool sorted_by_files) {
  if (!checkMadnexFileExtension(p)) {
    tfel::raise(
        "mfront-query: listing implementation paths is only supported for "
        "madnex files");
  }
  auto d = madnex::MFrontDataBase{p.file};
  auto first = true;
  auto display = [&first](const std::string& path) {
    if (!first) {
      std::cout << " ";
    }
    first = false;
    std::cout << path;
  };
  const auto paths = getImplementationsPaths(p);
  if (sorted_by_files) {
    std::cout << "- " << p.file << ": ";
  }
  for (const auto& path : paths) {
    display(path);
  }
  std::cout << '\n';
}  // end of listMaterials

static std::vector<std::string> filter(const std::vector<std::string>& values,
                                       const std::string& rvalue) {
  std::regex r(rvalue);
  auto results = std::vector<std::string>{};
  std::copy_if(values.begin(), values.end(), std::back_inserter(results),
               [&r](const std::string& n) { return std::regex_match(n, r); });
  return results;
}  // end of filter

template <std::vector<std::string> (madnex::DataBase::*getAvailableTests)(
    const std::string&, const std::string&) const>
static void listBehaviourTests(const mfront::PathSpecifier& p,
                               const bool sorted_by_behaviours,
                               const std::string& test_specifier) {
  if (!p.material_property_identifier.empty()) {
    tfel::raise(
        "mfront-query: specifying a material property "
        "is not meaningful when listing mtest tests");
  }
  if (!p.model_identifier.empty()) {
    tfel::raise(
        "mfront-query: specifying a model "
        "is not meaningful when listing mtest tests");
  }
  if (!checkMadnexFileExtension(p)) {
    tfel::raise(
        "mfront-query: listing mtest tests "
        "is only supported for madnex files");
  }
  auto d = madnex::DataBase{p.file};
  const auto materials = [&p, &d] {
    if ((!p.material_identifier.empty()) &&
        (p.material_identifier != "<none>")) {
      return filter(d.getMaterialsList(), p.material_identifier);
    }
    return std::vector<std::string>{p.material_identifier};
  }();
  for (const auto& m : materials) {
    const auto behaviours = [&p, &m, &d] {
      if ((!m.empty()) && (m != "<none>")) {
        if (p.behaviour_identifier.empty()) {
          return d.getAvailableBehaviours(m);
        }
        return filter(d.getAvailableBehaviours(m), p.behaviour_identifier);
      }
      return d.getAvailableBehaviours("");
    }();
    for (const auto& b : behaviours) {
      const auto tests = [&test_specifier, &d, &m, &b] {
        if (test_specifier.empty()) {
          return ((d.*getAvailableTests)(m, b));
        }
        return filter(((d.*getAvailableTests)(m, b)), test_specifier);
      }();
      if (tests.empty()) {
        continue;
      }
      if (sorted_by_behaviours) {
        std::cout << "- tests associated with behaviour " << b;
        if ((!m.empty()) && (m != "<none>")) {
          std::cout << " associated with material " << m;
        }
        std::cout << '\n';
        for (const auto& t : tests) {
          std::cout << "- " << t << '\n';
        }
      } else {
        for (const auto& t : tests) {
          std::cout << t << " ";
        }
      }
    }
  }
  if (!sorted_by_behaviours) {
    std::cout << '\n';
  }
}  // end of listBehaviourTests

static void listBehaviourMTestTests(const mfront::PathSpecifier& p,
                                    const bool sorted_by_behaviours,
                                    const std::string& test_specifier) {
#ifdef MADNEX_MTEST_TEST_SUPPORT
  using MemberType = std::vector<std::string> (madnex::DataBase::*)(
      const std::string&, const std::string&) const;
  listBehaviourTests<static_cast<MemberType>(
      &madnex::DataBase::getAvailableMTestTests)>(p, sorted_by_behaviours,
                                                  test_specifier);
#else  /* MADNEX_MTEST_TEST_SUPPORT */
  tfel::raise(
      "mfront-query: mtest tests are not supported by the madnex library");
#endif /* MADNEX_MTEST_TEST_SUPPORT */
}  // end of listBehaviourMTestTests

static void listBehaviourMFMTestGeneratorTests(
    const mfront::PathSpecifier& p,
    const bool sorted_by_behaviours,
    const std::string& test_specifier) {
#ifdef MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT
  using MemberType = std::vector<std::string> (madnex::DataBase::*)(
      const std::string&, const std::string&) const;
  listBehaviourTests<static_cast<MemberType>(
      &madnex::DataBase::getAvailableMFMTestGeneratorTests)>(
      p, sorted_by_behaviours, test_specifier);
#else  /* MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT */
  tfel::raise(
      "mfront-query: mfm-test-generator tests are not supported by the madnex "
      "library");
#endif /* MADNEX_MFM_TEST_GENERATOR_TEST_SUPPORT */
}  // end of listBehaviourMFMTestGeneratorTests

#endif /* MFRONT_QUERY_HAVE_MADNEX */

static void treatHasQuery(
    const tfel::utilities::ArgumentParser::CallBacksContainer& callBacks,
    const std::string& arg,
    const std::string& cmd) {
  if (!tfel::utilities::starts_with(arg, cmd)) {
    tfel::raise("treatHasQuery: invalid call (internal error)");
  }
  const auto l = cmd.size();
  if (arg.size() < l + 2) {
    tfel::raise("treatHasQuery: invalid argument ('" + std::string{arg} + "')");
  }
  if (arg[l] != '=') {
    tfel::raise("treatHasQuery: invalid argument ('" + std::string{arg} + "')");
  }
  const auto c = "--" + std::string{arg.substr(l + 1)};
  if (tfel::utilities::starts_with(c, "--")) {
    if (callBacks.find(c) != callBacks.end()) {
      std::cout << "true" << std::endl;
      return;
    }
  }
  std::cout << "false" << std::endl;
}  // end of treatHasQuery

#ifdef MFRONT_QUERY_HAVE_MADNEX

static bool treatListMaterialKnowledge(bool& b,
                                       bool& sorted,
                                       const char* const opt,
                                       const std::string& arg) {
  if (tfel::utilities::starts_with(arg, opt)) {
    if (b) {
      tfel::raise("mfront-query: " + std::string{opt} + " multiply defined");
    }
    const auto o = arg.substr(std::strlen(opt));
    if (!o.empty()) {
      if (o == "=sorted-by-materials") {
        sorted = true;
      } else if (o == "=unsorted") {
        sorted = false;
      } else {
        tfel::raise("invalid command line argument '" + arg + "'");
      }
    } else {
      sorted = true;
    }
    b = true;
    return true;
  }
  return false;
}  // end of treatListMaterialKnowledge

static bool treatListImplementationPathsOption(bool& b,
                                               bool& sorted,
                                               const char* const opt,
                                               const std::string& arg) {
  if (tfel::utilities::starts_with(arg, opt)) {
    if (b) {
      tfel::raise("mfront-query: " + std::string{opt} + " multiply defined");
    }
    const auto o = arg.substr(std::strlen(opt));
    if (!o.empty()) {
      if (o == "=sorted-by-files") {
        sorted = true;
      } else if (o == "=unsorted") {
        sorted = false;
      } else {
        tfel::raise("invalid command line argument '" + arg + "'");
      }
    } else {
      sorted = true;
    }
    b = true;
    return true;
  }
  return false;
}  // end of treatListImplementationPathsOption

static bool treatListBehaviourTestsOptions(bool& b,
                                           bool& sorted,
                                           const char* const opt,
                                           const std::string& arg) {
  if (tfel::utilities::starts_with(arg, opt)) {
    if (b) {
      tfel::raise("mfront-query: " + std::string{opt} + " multiply defined");
    }
    const auto o = arg.substr(std::strlen(opt));
    if (!o.empty()) {
      if (o == "=sorted-by-behaviours") {
        sorted = true;
      } else if (o == "=unsorted") {
        sorted = false;
      } else {
        tfel::raise("invalid command line argument '" + arg + "'");
      }
    } else {
      sorted = true;
    }
    b = true;
    return true;
  }
  return false;
}  // end of treatListBehaviourTestsOptions

#endif /* MFRONT_QUERY_HAVE_MADNEX */

/* coverity [UNCAUGHT_EXCEPT]*/
int main(const int argc, const char* const* const argv) {
  using namespace mfront;
  initDSLs();
  initInterfaces();
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
  const auto bg = [argv, argc] {
    for (auto a = argv; a != argv + argc; ++a) {
      if (::strcmp(*a, "--no-gui") == 0) {
        return false;
      }
    }
    return true;
  }();
#endif
#if not defined(__GLIBCXX__)
  try {
#endif /* not defined(__GLIBCXX__) */
    auto current_path_specifier = mfront::PathSpecifier{};
    auto path_specifiers = std::vector<mfront::PathSpecifier>{};
    auto queries_arguments = std::vector<const char*>{};
#ifdef MFRONT_QUERY_HAVE_MADNEX
    auto list_materials = false;
    auto list_implementation_paths = false;
    auto sort_implementation_paths_list = true;
    auto list_material_properties = false;
    auto sort_material_properties_list = true;
    auto list_behaviours = false;
    auto sort_behaviours_list = true;
    auto list_behaviour_mtest_tests = false;
    auto sort_behaviour_mtest_tests_list = true;
    auto list_behaviour_mfm_test_generator_tests = false;
    auto sort_behaviour_mfm_test_generator_tests_list = true;
    auto list_models = false;
    auto sort_models_list = true;
    std::string test;
    auto treatTest = [&test](const std::string& a) {
      if (a[0] != '=') {
        return false;
      }
      if (!test.empty()) {
        tfel::raise("test specifier already set");
      }
      if (a.size() == 1) {
        tfel::raise("empty test specifier");
      }
      test = a.substr(1);
      return true;
    };
#endif /* MFRONT_QUERY_HAVE_MADNEX */
    queries_arguments.push_back(argv[0]);
    for (auto arg = argv + 1; arg != argv + argc; ++arg) {
      const auto a = std::string{*arg};
      if (mfront::parsePathSpecifierArguments(path_specifiers,
                                              current_path_specifier, a)) {
      } else if ((a == "--help") || (a == "-h")) {
        std::cout << "Usage : " << argv[0] << " [options] [files]\n"
                  << "Available options are:\n"
                  << "--help,-h                             : "
                  << "print this message\n"
                  << "--usage                               : "
                  << "show how to use " << argv[0] << '\n'
                  << "--help-material-property-queries-list : "
                  << "list all queries avaiable for mfront behaviour files\n"
                  << "--help-behaviour-queries-list         : "
                  << "list all queries avaiable for mfront behaviour files\n"
                  << "--help-model-queries-list             : "
                  << "list all queries avaiable for mfront model files\n";
#ifdef MFRONT_QUERY_HAVE_MADNEX
        std::cout << "--material                            : "
                  << "specify a material (regular expression)\n"
                  << "--material-property                   : "
                  << "specify a material property (regular expression)\n"
                  << "--all-material-properties             : "
                  << "select all material properties (equivalent to "
                  << "--material-property='.+')\n"
                  << "--list-implementation-paths           : "
                  << "list all implementation paths associated with a set of "
                  << "material properties, behaviours or models in a "
                  << "madnex file.\n"
                  << "--list-materials                      : "
                  << "list all materials in a madnex file\n"
                  << "--list-material-properties            : "
                  << "list all material propertiess in a madnex `file\n"
                  << "--list-behaviours                     : "
                  << "list all behaviours in a madnex file\n"
                  << "--list-behaviour-mtest-tests          : "
                  << "list all the mtest tests associated with a set of "
                  << "behaviours in a madnex file\n"
                  << "--list-behaviour-mfm-test-genartor-tests : "
                  << "list all the mtest tests associated with a set of "
                  << "behaviours in a madnex file\n"
                  << "--list-models                         : "
                  << "list all models in a madnex file\n"
                  << "--behaviour                           : "
                  << "specify a behaviour (regular expression)\n"
                  << "--test: "
                  << "specify a test (regular expression)\n"
                  << "--all-behaviours                      : "
                  << "select all behaviours (equivalent to --behaviour='.+')\n"
                  << "--model                           : "
                  << "specify a model (regular expression)\n"
                  << "--all-models                      : "
                  << "select all models (equivalent to --model='.+')\n";
#endif /* MFRONT_QUERY_HAVE_MADNEX */
        std::exit(EXIT_SUCCESS);
      } else if ((a == "--version") || (a == "-v")) {
        std::cout << MFrontHeader::getHeader();
        std::exit(EXIT_SUCCESS);
      } else if (a == "--usage") {
        std::cout << "Usage : " << argv[0] << " [options] [files]\n";
        std::exit(EXIT_SUCCESS);
      } else if (tfel::utilities::starts_with(
                     a, "--has-material-property-query")) {
        const char* args[1] = {argv[0]};
        std::shared_ptr<MaterialPropertyDSL> b;
        auto bq = std::make_shared<MaterialPropertyQuery>(1, args, b, "");
        treatHasQuery(bq->getRegistredCallBacks(), a,
                      "--has-material-property-query");
      } else if (a == "--help-material-property-queries-list") {
        const char* args[2] = {argv[0], "--help"};
        std::shared_ptr<MaterialPropertyDSL> b;
        auto bq = std::make_shared<MaterialPropertyQuery>(2, args, b, "");
        std::exit(EXIT_SUCCESS);
      } else if (tfel::utilities::starts_with(a, "--has-behaviour-query")) {
        const char* args[1] = {argv[0]};
        std::shared_ptr<AbstractBehaviourDSL> b;
        auto bq = std::make_shared<BehaviourQuery>(1, args, b, "");
        treatHasQuery(bq->getRegistredCallBacks(), a, "--has-behaviour-query");
      } else if (a == "--help-behaviour-queries-list") {
        const char* args[2] = {argv[0], "--help"};
        std::shared_ptr<AbstractBehaviourDSL> b;
        auto bq = std::make_shared<BehaviourQuery>(2, args, b, "");
        std::exit(EXIT_SUCCESS);
      } else if (tfel::utilities::starts_with(a, "--has-model-query")) {
        const char* args[1] = {argv[0]};
        std::shared_ptr<ModelDSL> b;
        auto bq = std::make_shared<ModelQuery>(1, args, b, "");
        treatHasQuery(bq->getRegistredCallBacks(), a, "--has-model-query");
      } else if (a == "--help-model-queries-list") {
        const char* args[2] = {argv[0], "--help"};
        std::shared_ptr<ModelDSL> b;
        auto bq = std::make_shared<ModelQuery>(2, args, b, "");
        std::exit(EXIT_SUCCESS);
#ifdef MFRONT_QUERY_HAVE_MADNEX
      } else if (treatListImplementationPathsOption(
                     list_implementation_paths, sort_implementation_paths_list,
                     "--list-implementation-paths", a)) {
      } else if (a == "--list-materials") {
        list_materials = true;
      } else if (treatListMaterialKnowledge(list_material_properties,
                                            sort_material_properties_list,
                                            "--list-material-properties", a)) {
      } else if (treatListMaterialKnowledge(list_behaviours,
                                            sort_behaviours_list,
                                            "--list-behaviours", a)) {
      } else if (treatListBehaviourTestsOptions(list_behaviour_mtest_tests,
                                                sort_behaviour_mtest_tests_list,
                                                "--list-behaviour-mtest-tests",
                                                a)) {
      } else if (treatListBehaviourTestsOptions(
                     list_behaviour_mfm_test_generator_tests,
                     sort_behaviour_mfm_test_generator_tests_list,
                     "--list-behaviour-mfm-test-generator-tests", a)) {
      } else if ((tfel::utilities::starts_with(a, "--test")) &&
                 (treatTest(a.substr(strlen("--test"))))) {
      } else if (treatListMaterialKnowledge(list_models, sort_models_list,
                                            "--list-models", a)) {
#if (defined _WIN32) || (defined _WIN64)
      } else if (a == "/list-materials") {
        list_materials = true;
      } else if (treatListImplementationPathsOption(
                     list_implementation_paths, sort_implementation_paths_list,
                     "/list-implementation-paths", a)) {
      } else if (treatListMaterialKnowledge(list_material_properties,
                                            sort_material_properties_list,
                                            "/list-material-properties", a)) {
      } else if (treatListMaterialKnowledge(list_behaviours,
                                            sort_behaviours_list,
                                            "/list-behaviours", a)) {
      } else if ((tfel::utilities::starts_with(a, "/test")) &&
                 (treatTest(a.substr(strlen("/test"))))) {
        treatTest(a.substr(strlen("/test")));
      } else if (treatListBehaviourTestsOptions(list_behaviour_mtest_tests,
                                                sort_behaviour_mtest_tests_list,
                                                "/list-behaviour-mtest-tests",
                                                a)) {
      } else if (treatListBehaviourTestsOptions(
                     list_behaviour_mfm_test_generator_tests,
                     sort_behaviour_mfm_test_generator_tests_list,
                     "/list-behaviour-mfm-test-generator-tests", a)) {
      } else if (treatListMaterialKnowledge(list_models, sort_models_list,
                                            "/list-models", a)) {
#endif /* (defined _WIN32) || (defined _WIN64)*/
#endif /* MFRONT_QUERY_HAVE_MADNEX */
      } else {
        queries_arguments.push_back(*arg);
      }
    }
    mfront::finalizePathSpecifierArgumentsParsing(path_specifiers,
                                                  current_path_specifier);
#ifdef MFRONT_QUERY_HAVE_MADNEX
    if ((list_materials || list_material_properties ||  //
         list_behaviours || list_models || list_behaviour_mtest_tests ||
         list_behaviour_mfm_test_generator_tests) &&
        (list_implementation_paths)) {
      tfel::raise(
          "specifying --list-implementation-paths can't be combined "
          "with --list-materials, --list-material-properties, "
          "--list-behaviour, --list-behaviour-mtest-tests or --list-models");
    }
    if (!test.empty()) {
      if ((!list_behaviour_mtest_tests) &&
          (!list_behaviour_mfm_test_generator_tests)) {
        tfel::raise(
            "using --test is only meaningful with "
            "--list_behaviour_mtest_tests or "
            "--list_behaviour_mfm_test_generator_tests "
            "command line arguments");
      }
    }
    for (const auto& p : path_specifiers) {
      if (list_materials) {
        listMaterials(p);
      }
      if (list_material_properties) {
        listMaterialProperties(p, sort_material_properties_list);
      }
      if (list_behaviours) {
        listBehaviours(p, sort_behaviours_list);
      }
      if (list_models) {
        listModels(p, sort_models_list);
      }
      if (list_implementation_paths) {
        listImplementationPaths(p, sort_implementation_paths_list);
      }
      if (list_behaviour_mtest_tests) {
        listBehaviourMTestTests(p, sort_behaviour_mtest_tests_list, test);
      }
      if (list_behaviour_mfm_test_generator_tests) {
        listBehaviourMFMTestGeneratorTests(
            p, sort_behaviour_mfm_test_generator_tests_list, test);
      }
      //
      if (list_materials || list_material_properties ||     //
          list_behaviours || list_behaviour_mtest_tests ||  //
          list_behaviour_mfm_test_generator_tests ||        //
          list_models || list_implementation_paths) {
        std::exit(EXIT_SUCCESS);
      }
    }
#endif /* MFRONT_QUERY_HAVE_MADNEX */
    //
    const auto implementations =
        mfront::getImplementationsPaths(path_specifiers);
    SearchPathsHandler::addSearchPathsFromImplementationPaths(implementations);
    //
    auto mpqueries = std::vector<std::shared_ptr<MaterialPropertyQuery>>{};
    auto bqueries = std::vector<std::shared_ptr<BehaviourQuery>>{};
    auto mqueries = std::vector<std::shared_ptr<ModelQuery>>{};
    for (const auto& i : implementations) {
      auto dsl = MFrontBase::getDSL(i);
      if (dsl->getTargetType() == AbstractDSL::MATERIALPROPERTYDSL) {
        auto b = std::dynamic_pointer_cast<MaterialPropertyDSL>(dsl);
        tfel::raise_if(!b, "mfront-query: invalid dsl implementation");
        mpqueries.push_back(std::make_shared<MaterialPropertyQuery>(
            queries_arguments.size(), queries_arguments.data(), b, i));
      } else if (dsl->getTargetType() == AbstractDSL::BEHAVIOURDSL) {
        auto b = std::dynamic_pointer_cast<AbstractBehaviourDSL>(dsl);
        tfel::raise_if(!b, "mfront-query: invalid dsl implementation");
        bqueries.push_back(std::make_shared<BehaviourQuery>(
            queries_arguments.size(), queries_arguments.data(), b, i));
      } else if (dsl->getTargetType() == AbstractDSL::MODELDSL) {
        auto b = std::dynamic_pointer_cast<ModelDSL>(dsl);
        tfel::raise_if(!b, "mfront-query: invalid dsl implementation");
        mqueries.push_back(std::make_shared<ModelQuery>(
            queries_arguments.size(), queries_arguments.data(), b, i));
      } else {
        tfel::raise("mfront-query: unsupported dsl type");
      }
    }
    // treating all queries
    for (const auto& q : mpqueries) {
      q->exe();
    }
    for (const auto& q : bqueries) {
      q->exe();
    }
    for (const auto& q : mqueries) {
      q->exe();
    }
#if not defined(__GLIBCXX__)
  } catch (std::exception& e) {
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    if (bg) {
      MessageBox(nullptr, e.what(), "mfront", 0);
    } else {
      std::cerr << e.what() << std::endl;
    }
#else  /* defined _WIN32 || defined _WIN64 ||defined __CYGWIN__ */
    std::cerr << e.what() << std::endl;
#endif /* defined _WIN32 || defined _WIN64 ||defined __CYGWIN__ */
    return EXIT_FAILURE;
  }
#endif /* not defined(__GLIBCXX__) */
  return EXIT_SUCCESS;
}
