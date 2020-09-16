/*!
 * \file  mfront-query.cxx
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

struct MFrontPathSpecifier {
  std::string file;
  std::string material_identifier;
  std::string material_property_identifier;
  std::string behaviour_identifier;
  std::string model_identifier;
};  // end of struct MFrontPathSpecifier

#ifdef MFRONT_QUERY_HAVE_MADNEX

using GetAvailableImplementationsPtr1 = std::vector<std::string> (
    madnex::MFrontDataBase::*)(const std::string&) const;
using GetAvailableImplementationsPtr2 =
    std::map<std::string, std::vector<std::string>> (
        madnex::MFrontDataBase::*)() const;

static void listImplementations(const MFrontPathSpecifier& p,
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
  if ((ext != "madnex") && (ext != "edf")) {
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

static void listMaterialProperties(const MFrontPathSpecifier& p) {
  GetAvailableImplementationsPtr1 m1 =
      &madnex::MFrontDataBase::getAvailableMaterialProperties;
  GetAvailableImplementationsPtr2 m2 =
      &madnex::MFrontDataBase::getAvailableMaterialProperties;
  listImplementations(p, "material properties", m1, m2);
}  // end of listMaterialProperties

static void listBehaviours(const MFrontPathSpecifier& p) {
  GetAvailableImplementationsPtr1 m1 =
      &madnex::MFrontDataBase::getAvailableBehaviours;
  GetAvailableImplementationsPtr2 m2 =
      &madnex::MFrontDataBase::getAvailableBehaviours;
  listImplementations(p, "behaviours", m1, m2);
}  // end of listBehaviours

static void listModels(const MFrontPathSpecifier& p) {
  GetAvailableImplementationsPtr1 m1 =
      &madnex::MFrontDataBase::getAvailableModels;
  GetAvailableImplementationsPtr2 m2 =
      &madnex::MFrontDataBase::getAvailableModels;
  listImplementations(p, "models", m1, m2);
}  // end of listModels

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
    auto current_path_specifier = MFrontPathSpecifier{};
    auto path_specifiers = std::vector<MFrontPathSpecifier>{};
    auto queries_arguments = std::vector<const char*>{};
#ifdef MFRONT_QUERY_HAVE_MADNEX
    auto list_material_properties = false;
    auto list_behaviours = false;
    auto list_models = false;
#endif /* MFRONT_QUERY_HAVE_MADNEX */
    queries_arguments.push_back(argv[0]);
    for (auto arg = argv + 1; arg != argv + argc; ++arg) {
      const auto a = std::string{*arg};
      if (a[0] != '-') {
        current_path_specifier.file = a;
        path_specifiers.push_back(current_path_specifier);
        current_path_specifier.file.clear();
        current_path_specifier.material_identifier.clear();
        current_path_specifier.material_property_identifier.clear();
        current_path_specifier.behaviour_identifier.clear();
        current_path_specifier.model_identifier.clear();
      } else if (tfel::utilities::starts_with(a, "--material=")) {
        tfel::raise_if(!current_path_specifier.material_identifier.empty(),
                       "mfront-query: material multiply defined");
        current_path_specifier.material_identifier =
            a.substr(std::strlen("--material="));
        tfel::raise_if(current_path_specifier.material_identifier.empty(),
                       "mfront-query: empty material name specified");
      } else if (tfel::utilities::starts_with(a, "--material-property=")) {
        tfel::raise_if(
            !current_path_specifier.material_property_identifier.empty(),
            "mfront-query: material property multiply defined");
        current_path_specifier.material_property_identifier =
            a.substr(std::strlen("--material-property="));
        tfel::raise_if(
            current_path_specifier.material_property_identifier.empty(),
            "mfront-query: empty material property specified");
      } else if (tfel::utilities::starts_with(a, "--behaviour=")) {
        tfel::raise_if(!current_path_specifier.behaviour_identifier.empty(),
                       "mfront-query: behaviour multiply defined");
        current_path_specifier.behaviour_identifier =
            a.substr(std::strlen("--behaviour="));
        tfel::raise_if(current_path_specifier.behaviour_identifier.empty(),
                       "mfront-query: empty behaviour specified");
      } else if (tfel::utilities::starts_with(a, "--model=")) {
        tfel::raise_if(!current_path_specifier.model_identifier.empty(),
                       "mfront-query: model multiply defined");
        current_path_specifier.model_identifier =
            a.substr(std::strlen("--model="));
        tfel::raise_if(current_path_specifier.model_identifier.empty(),
                       "mfront-query: empty model specified");
#if (defined _WIN32) || (defined _WIN64)
      } else if (tfel::utilities::starts_with(a, "/material=")) {
        material = a.substr(std::strlen("/material="));
        tfel::raise_if(material.empty(),
                       "mfront-query: empty material name specified");
      } else if (tfel::utilities::starts_with(a, "/material-property=")) {
        material_property_identifier =
            a.substr(std::strlen("/material-property="));
        tfel::raise_if(material_property_identifier.empty(),
                       "mfront-query: empty material property specified");
      } else if (tfel::utilities::starts_with(a, "/behaviour=")) {
        behaviour_identifier = a.substr(std::strlen("/behaviour="));
        tfel::raise_if(behaviour_identifier.empty(),
                       "mfront-query: empty behaviour specified");
      } else if (tfel::utilities::starts_with(a, "/model=")) {
        model_identifier = a.substr(std::strlen("/model="));
        tfel::raise_if(model_identifier.empty(),
                       "mfront-query: empty model specified");
#endif /* (defined _WIN32) || (defined _WIN64)*/
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
      } else if (a == "--help-material-property-queries-list") {
        const char* args[2] = {argv[0], "--help"};
        std::shared_ptr<MaterialPropertyDSL> b;
        auto bq = std::make_shared<MaterialPropertyQuery>(2, args, b, "");
        std::exit(EXIT_SUCCESS);
      } else if (a == "--help-behaviour-queries-list") {
        const char* args[2] = {argv[0], "--help"};
        std::shared_ptr<AbstractBehaviourDSL> b;
        auto bq = std::make_shared<BehaviourQuery>(2, args, b, "");
        std::exit(EXIT_SUCCESS);
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
    //
    if (!current_path_specifier.file.empty()) {
      path_specifiers.push_back(current_path_specifier);
    } else {
      tfel::raise_if(!current_path_specifier.material_identifier.empty(),
                     "mfront-query: material name specified "
                     "after the last input file");
      tfel::raise_if(
          !current_path_specifier.material_property_identifier.empty(),
          "mfront-query: material property specified "
          "after the last input file");
      tfel::raise_if(!current_path_specifier.behaviour_identifier.empty(),
                     "mfront-query: behaviour specified "
                     "after the last input file");
      tfel::raise_if(!current_path_specifier.model_identifier.empty(),
                     "mfront-query: model specified "
                     "after the last input file");
    }
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
    auto implementations = std::vector<std::string>{};
    for (const auto& p : path_specifiers) {
      const auto paths = mfront::getMFrontImplementationsPaths(
          p.file, p.material_identifier, p.material_property_identifier,
          p.behaviour_identifier, p.model_identifier);
      implementations.insert(implementations.end(), paths.begin(), paths.end());
    }
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
  } catch (exception& e) {
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
