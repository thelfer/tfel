/*!
 * \file  tests/System/ExternalLibraryManagerTest.cxx
 * \brief This file checks the path of a library found by the dynamic loader
 * outside the current directory and `LD_LIBRARY_PATH`
 * \author Tristan Chenaille
 * \date 02/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <cstdlib>
#include <exception>
#include <iostream>
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"

struct ExternalLibraryManagerTest final : public tfel::tests::TestCase {
  ExternalLibraryManagerTest()
      : tfel::tests::TestCase("TFEL/System", "ExternalLibraryManagerTest") {
  }  // end of ExternalLibraryManagerTest
  tfel::tests::TestResult execute() override {
#if (defined(__APPLE__) && defined(__MACH__)) || defined(__GLIBC__)
    // a system library, found through the default directories
#if defined(__APPLE__) && defined(__MACH__)
    const auto l = std::string{"libc++.1.dylib"};
#else
    const auto l = std::string{"libm.so.6"};
#endif
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    auto p = std::string{};
    try {
      p = elm.getLibraryPath(l);
    } catch (std::exception& e) {
      std::cerr << e.what() << '\n';
    }
    TFEL_TESTS_ASSERT(
        (p.size() > l.size()) &&
        (p.compare(p.size() - l.size() - 1, l.size() + 1, '/' + l) == 0));
#endif
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(ExternalLibraryManagerTest,
                          "ExternalLibraryManagerTest");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ExternalLibraryManagerTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
