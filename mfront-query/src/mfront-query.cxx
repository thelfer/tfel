/*!
 * \file  mfront-query.cxx
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
#endif /* __CYGWIN__ */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
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

/* coverity [UNCAUGHT_EXCEPT]*/
int main(const int argc, const char* const* const argv) {
  using namespace std;
  using namespace mfront;
  initDSLs();
  initInterfaces();
  auto mpqueries = vector<shared_ptr<MaterialPropertyQuery>>{};
  auto bqueries = vector<shared_ptr<BehaviourQuery>>{};
  auto mqueries = vector<shared_ptr<ModelQuery>>{};
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
    for (auto a = argv + 1; a != argv + argc; ++a) {
      if ((*a)[0] != '-') {
        const string f{*a};
        auto dsl = MFrontBase::getDSL(f);
        if (dsl->getTargetType() == AbstractDSL::MATERIALPROPERTYDSL) {
          auto b = dynamic_pointer_cast<MaterialPropertyDSL>(dsl);
          tfel::raise_if(!b, "mfront-query: invalid dsl implementation");
          mpqueries.push_back(
              make_shared<MaterialPropertyQuery>(argc, argv, b, f));
        } else if (dsl->getTargetType() == AbstractDSL::BEHAVIOURDSL) {
          auto b = dynamic_pointer_cast<AbstractBehaviourDSL>(dsl);
          tfel::raise_if(!b, "mfront-query: invalid dsl implementation");
          bqueries.push_back(make_shared<BehaviourQuery>(argc, argv, b, f));
        } else if (dsl->getTargetType() == AbstractDSL::MODELDSL) {
          auto b = dynamic_pointer_cast<ModelDSL>(dsl);
          tfel::raise_if(!b, "mfront-query: invalid dsl implementation");
          mqueries.push_back(make_shared<ModelQuery>(argc, argv, b, f));
        } else {
          tfel::raise("mfront-query: unsupported dsl type");
        }
      } else if ((strcmp(*a, "--help") == 0) || (strcmp(*a, "-h") == 0)) {
        cout << "Usage : " << argv[0] << " [options] [files]\n"
             << "Available options are:\n"
             << "--help,-h                             : "
             << "print this message\n"
             << "--usage                               : "
             << "show how to use " << argv[0] << endl
             << "--help-material-property-queries-list : "
             << "list all queries avaiable for mfront behaviour files\n"
             << "--help-behaviour-queries-list         : "
             << "list all queries avaiable for mfront behaviour files\n"
             << "--help-model-queries-list             : "
             << "list all queries avaiable for mfront model files\n";
        exit(EXIT_SUCCESS);
      } else if ((strcmp(*a, "--version") == 0) || (strcmp(*a, "-v") == 0)) {
        cout << MFrontHeader::getHeader();
        std::exit(EXIT_SUCCESS);
      } else if (strcmp(*a, "--usage") == 0) {
        cout << "Usage : " << argv[0] << " [options] [files]\n";
        std::exit(EXIT_SUCCESS);
      } else if (strcmp(*a, "--help-material-property-queries-list") == 0) {
        const char* args[2] = {argv[0], "--help"};
        shared_ptr<MaterialPropertyDSL> b;
        auto bq = make_shared<MaterialPropertyQuery>(2, args, b, "");
        std::exit(EXIT_SUCCESS);
      } else if (strcmp(*a, "--help-behaviour-queries-list") == 0) {
        const char* args[2] = {argv[0], "--help"};
        shared_ptr<AbstractBehaviourDSL> b;
        auto bq = make_shared<BehaviourQuery>(2, args, b, "");
        std::exit(EXIT_SUCCESS);
      } else if (strcmp(*a, "--help-model-queries-list") == 0) {
        const char* args[2] = {argv[0], "--help"};
        shared_ptr<ModelDSL> b;
        auto bq = make_shared<ModelQuery>(2, args, b, "");
        std::exit(EXIT_SUCCESS);
      }
    }
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
