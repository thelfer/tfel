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
#include "MFront/BehaviourDocumentationGenerator.hxx"
#include "MFront/AbstractDSL.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main(const int argc, const char* const* const argv) {
  using namespace mfront;
  using BGen = BehaviourDocumentationGenerator;
  initDSLs();
  initInterfaces();
  std::vector<std::shared_ptr<BGen>> bgens;
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
  try {
#endif /* __CYGWIN__ */
    for (auto a = argv + 1; a != argv + argc; ++a) {
      if ((*a)[0] != '-') {
        const std::string f{*a};
        auto dsl = MFrontBase::getDSL(f);
        if (dsl->getTargetType() == AbstractDSL::BEHAVIOURDSL) {
          auto b = std::dynamic_pointer_cast<AbstractBehaviourDSL>(dsl);
          tfel::raise_if(!b, "mfront-doc: invalid dsl implementation");
          bgens.push_back(std::make_shared<BGen>(argc, argv, b, f));
        } else {
          tfel::raise("mfront-doc: unsupported dsl type");
        }
      } else if ((strcmp(*a, "--help") == 0) || (strcmp(*a, "-h") == 0)) {
        std::cout << "Usage : " << argv[0] << " [options] [files]" << std::endl;
        std::cout << "Available options are :" << std::endl
                  << "--help,-h : print this message" << std::endl
                  << "--usage   : show how to use " << argv[0] << std::endl;
        ::exit(EXIT_SUCCESS);
      } else if ((strcmp(*a, "--version") == 0) || (strcmp(*a, "-v") == 0)) {
        std::cout << MFrontHeader::getHeader();
        exit(EXIT_SUCCESS);
      } else if (strcmp(*a, "--usage") == 0) {
        std::cout << "Usage : " << argv[0] << " [options] [files]" << std::endl;
        exit(EXIT_SUCCESS);
      }
    }
    for (const auto& q : bgens) {
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
