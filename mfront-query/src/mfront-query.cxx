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

#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string_view>

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
  auto display = [&first](std::string_view m) {
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
  auto display = [&first](std::string_view path) {
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

#endif /* MFRONT_QUERY_HAVE_MADNEX */

static void treatHasQuery(
    const tfel::utilities::ArgumentParser::CallBacksContainer& callBacks,
    const std::string_view arg,
    const std::string_view cmd) {
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
    const auto o = std::string_view(arg).substr(std::strlen(opt));
    if (!o.empty()) {
      if (o == "=sorted-by-materials") {
        sorted = true;
      } else if (o == "=unsorted") {
        sorted = false;
      } else {
        tfel::raise(
            "mfront-query: "
            "invalid command line argument '" +
            arg + "'");
      }
    } else {
      sorted = true;
    }
    b = true;
  }
  return b;
}  // end of treatListMaterialKnowledge

static bool treatListImplementationPathsOption(bool& b,
                                               bool& sorted,
                                               const char* const opt,
                                               const std::string& arg) {
  if (tfel::utilities::starts_with(arg, opt)) {
    if (b) {
      tfel::raise("mfront-query: " + std::string{opt} + " multiply defined");
    }
    const auto o = std::string_view(arg).substr(std::strlen(opt));
    if (!o.empty()) {
      if (o == "=sorted-by-files") {
        sorted = true;
      } else if (o == "=unsorted") {
        sorted = false;
      } else {
        tfel::raise(
            "mfront-query: "
            "invalid command line argument '" +
            arg + "'");
      }
    } else {
      sorted = true;
    }
    b = true;
  }
  return b;
}  // end of treatListImplementationPathsOption

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
    auto list_models = false;
    auto sort_models_list = true;
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
                     "--material-property='.+')\n"
                  << "--behaviour                           : "
                  << "specify a behaviour (regular expression)\n"
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
         list_behaviours || list_models) &&
        (list_implementation_paths)) {
      tfel::raise(
          "specifying --list-paths can be combined with --list-materials, "
          "--list-material-properties, "
          "--list-behaviour or --list-models");
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
    }
    if (list_materials || list_material_properties ||  //
        list_behaviours || list_models || list_implementation_paths) {
      std::exit(EXIT_SUCCESS);
    }
#endif /* MFRONT_QUERY_HAVE_MADNEX */
    //
    const auto implementations =
        mfront::getImplementationsPaths(path_specifiers);
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
