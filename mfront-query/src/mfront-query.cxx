/*!
 * \file  mfront-query/src/mfront-query.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#ifdef MFRONT_HAVE_MADNEX
#include "Madnex/MFrontDataBase.hxx"
#include "Madnex/MFrontImplementation.hxx"
#endif /* MFRONT_HAVE_MADNEX */

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

static void listImplementations(const mfront::PathSpecifier& p,
                                const std::string& itypes,
                                const GetAvailableImplementationsPtr1 m1,
                                const GetAvailableImplementationsPtr2 m2) {
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
  const auto ext = [p]() -> std::string {
    const auto pos = p.file.find(".");
    if (pos != std::string::npos) {
      return p.file.substr(pos + 1);
    }
    return "";
  }();
  if ((ext != "madnex") && (ext != "edf") && (ext != "mdnx")) {
    tfel::raise("mfront-query: listing " + itypes +
                " is only supported for madnex files");
  }
  auto d = madnex::MFrontDataBase{p.file};
  if (!p.material_identifier.empty()) {
    const auto impls =
        (d.*m1)(p.material_identifier == "<none>" ? "" : p.material_identifier);
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
  } else {
    for (const auto& impls : (d.*m2)()) {
      if (impls.first.empty()) {
        std::cout << "- " + itypes + " associated with no material:\n";
      } else {
        std::cout << "- " + itypes + " associated with material '" +
                         impls.first + "':\n";
      }
      for (const auto& i : impls.second) {
        std::cout << "    - " << i << '\n';
      }
    }
  }
}  // end of listImplementations

static void listMaterialProperties(const mfront::PathSpecifier& p) {
  GetAvailableImplementationsPtr1 m1 =
      &madnex::MFrontDataBase::getAvailableMaterialProperties;
  GetAvailableImplementationsPtr2 m2 =
      &madnex::MFrontDataBase::getAvailableMaterialProperties;
  listImplementations(p, "material properties", m1, m2);
}  // end of listMaterialProperties

static void listBehaviours(const mfront::PathSpecifier& p) {
  GetAvailableImplementationsPtr1 m1 =
      &madnex::MFrontDataBase::getAvailableBehaviours;
  GetAvailableImplementationsPtr2 m2 =
      &madnex::MFrontDataBase::getAvailableBehaviours;
  listImplementations(p, "behaviours", m1, m2);
}  // end of listBehaviours

static void listModels(const mfront::PathSpecifier& p) {
  GetAvailableImplementationsPtr1 m1 =
      &madnex::MFrontDataBase::getAvailableModels;
  GetAvailableImplementationsPtr2 m2 =
      &madnex::MFrontDataBase::getAvailableModels;
  listImplementations(p, "models", m1, m2);
}  // end of listModels

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
    auto list_material_properties = false;
    auto list_behaviours = false;
    auto list_models = false;
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
                  << "--behaviour                           : "
                  << "specify a behaviour (regular expression)\n"
                  << "--model                           : "
                  << "specify a model (regular expression)\n";
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
      } else if (a == "--list-material-properties") {
        list_material_properties = true;
      } else if (a == "--list-behaviours") {
        list_behaviours = true;
      } else if (a == "--list-models") {
        list_models = true;
#if (defined _WIN32) || (defined _WIN64)
      } else if (a == "/list-material-properties") {
        list_material_properties = true;
      } else if (a == "/list-behaviours") {
        list_behaviours = true;
      } else if (a == "/list-models") {
        list_models = true;
#endif /* (defined _WIN32) || (defined _WIN64)*/
#endif /* MFRONT_QUERY_HAVE_MADNEX */
      } else {
        queries_arguments.push_back(*arg);
      }
    }
    mfront::finalizePathSpecifierArgumentsParsing(path_specifiers,
                                                  current_path_specifier);
#ifdef MFRONT_QUERY_HAVE_MADNEX
    if (list_material_properties || list_behaviours || list_models) {
      tfel::raise_if(
          queries_arguments.size() != 1u,
          "specifying queries with the --list-material-properties, "
          "--list-behaviour or --list-models argument is not supported");
    }
    for (const auto& p : path_specifiers) {
      if (list_material_properties) {
        listMaterialProperties(p);
      }
      if (list_behaviours) {
        listBehaviours(p);
      }
      if (list_models) {
        listModels(p);
      }
    }
    if (list_material_properties || list_behaviours || list_models) {
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
