/*!
 * \file   mfront/tests/unit-tests/VUMATTest_dp.cxx
 * \brief
 * \author Thomas Helfer
 * \date   25 août 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "vumat-dp.cpp"

struct VUMATTest_dp final : public tfel::tests::TestCase {
  VUMATTest_dp()
      : tfel::tests::TestCase("MFront", "VUMATTest_dp") {
  }  // end of VUMATTest_dp

  tfel::tests::TestResult execute() override {
    char n[80] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
                  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
    const char* b = "VUMATBEHAVIOUR_CHABOCHE_3D_1";
    const char* b2 = "VUMATBEHAVIOUR_CHABOCHE_PSTRAIN";
    std::copy(b, b + std::strlen(b), n);
    const auto r = decompose(n);
#ifdef _WIN32
    TFEL_TESTS_ASSERT(r.first == "libVUMATBEHAVIOUR.dll");
#else
    TFEL_TESTS_ASSERT(r.first == "libVUMATBEHAVIOUR.so");
#endif /* _WIN32 */
    TFEL_TESTS_ASSERT(r.second == "CHABOCHE_3D");
    // second test
    std::fill(n, n + 80, '\0');
    std::copy(b2, b2 + std::strlen(b2), n);
    const auto r2 = decompose(n);
#ifdef _WIN32
    TFEL_TESTS_ASSERT(r2.first == "libVUMATBEHAVIOUR.dll");
#else
    TFEL_TESTS_ASSERT(r2.first == "libVUMATBEHAVIOUR.so");
#endif /* _WIN32 */
    TFEL_TESTS_ASSERT(r2.second == "CHABOCHE_PSTRAIN");
    return this->result;
  }  // end of execute()
};

TFEL_TESTS_GENERATE_PROXY(VUMATTest_dp, "VUMATTest_dp");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("VUMATTest_dp.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
