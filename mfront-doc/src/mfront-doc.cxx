/*!
 * \file  mfront-doc.cxx
 * \brief
 * \author Thomas Helfer
 * \date   22 mars 2015
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

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "MFront/InitDSLs.hxx"
#include "MFront/InitInterfaces.hxx"
#include "MFront/MFrontHeader.hxx"
#include "MFront/MFrontBase.hxx"
#include "MFront/AbstractDSL.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/MaterialPropertyDSL.hxx"
#include "MFront/DocumentationGeneratorBase.hxx"
#include "MFront/BehaviourDocumentationGenerator.hxx"
#include "MFront/MaterialPropertyDocumentationGenerator.hxx"
#include "MFront/PathSpecifier.hxx"
#include "MFront/SearchPathsHandler.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main(const int argc, const char* const* const argv) {
  using namespace mfront;
  initDSLs();
  initInterfaces();
  auto current_path_specifier = mfront::PathSpecifier{};
  auto path_specifiers = std::vector<mfront::PathSpecifier>{};
  auto untreated_arguments = std::vector<const char*>(1u, argv[0]);
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
  try {
#endif /* __CYGWIN__ */
    for (auto pa = argv + 1; pa != argv + argc; ++pa) {
      auto a = std::string(*pa);
      if (mfront::parsePathSpecifierArguments(path_specifiers,
                                              current_path_specifier, a)) {
      } else if ((a == "--help") || (a == "-h")) {
        std::cout << "Usage : " << argv[0] << " [options] [files]" << std::endl;
        std::cout << "Available options are :" << std::endl
                  << "--help,-h : print this message" << std::endl
                  << "--usage   : show how to use " << argv[0] << std::endl;
        ::exit(EXIT_SUCCESS);
      } else if ((a == "--version") || (a == "-v")) {
        std::cout << MFrontHeader::getHeader();
        exit(EXIT_SUCCESS);
      } else if (a == "--usage") {
        std::cout << "Usage : " << argv[0] << " [options] [files]" << std::endl;
        exit(EXIT_SUCCESS);
      } else {
        untreated_arguments.push_back(*pa);
      }
    }
    mfront::finalizePathSpecifierArgumentsParsing(path_specifiers,
                                                  current_path_specifier);
    const auto implementations =
        mfront::getImplementationsPaths(path_specifiers);
    SearchPathsHandler::addSearchPathsFromImplementationPaths(implementations);
    //
    auto bgens =
        std::vector<std::shared_ptr<BehaviourDocumentationGenerator>>{};
    auto mpgens =
        std::vector<std::shared_ptr<MaterialPropertyDocumentationGenerator>>{};
    for (const auto& f : implementations) {
      auto dsl = MFrontBase::getDSL(f);
      if (dsl->getTargetType() == AbstractDSL::BEHAVIOURDSL) {
        auto b = std::dynamic_pointer_cast<AbstractBehaviourDSL>(dsl);
        tfel::raise_if(!b, "mfront-doc: invalid dsl implementation");
        bgens.push_back(std::make_shared<BehaviourDocumentationGenerator>(
            untreated_arguments.size(), untreated_arguments.data(), b, f));
      } else if (dsl->getTargetType() == AbstractDSL::MATERIALPROPERTYDSL) {
        auto b = std::dynamic_pointer_cast<MaterialPropertyDSL>(dsl);
        tfel::raise_if(!b, "mfront-doc: invalid dsl implementation");
        mpgens.push_back(
            std::make_shared<MaterialPropertyDocumentationGenerator>(
                untreated_arguments.size(), untreated_arguments.data(), b, f));
      } else {
        tfel::raise("mfront-doc: unsupported dsl type");
      }
    }
    //
    for (const auto& q : bgens) {
      q->exe();
    }
    for (const auto& q : mpgens) {
      q->exe();
    }
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
  } catch (std::exception& e) {
    MessageBox(nullptr, e.what(), "mfront", 0);
    return EXIT_FAILURE;
  } catch (...) {
    return EXIT_FAILURE;
  }
#endif /* __CYGWIN__ */
  return EXIT_SUCCESS;
}
