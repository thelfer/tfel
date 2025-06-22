/*!
 * \file   tests/Utilities/gentype2.cxx
 * \brief  This file tests some functionalities of the GenType class.
 * \author Thomas Helfer
 * \date   23 Apr 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <iostream>
#include <cstdlib>
#include <string>

#include <cassert>

#include "TFEL/Utilities/GenTypeBase.hxx"

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

struct my_function {
  using return_type = int;
  static return_type apply(const int v) { return v * 2; }
  static return_type apply(const std::string&) { return 0; }
};

struct my_function2 {
  using return_type = int;
  static return_type apply(const int v1, const int v2) { return v1 * v2; }
  template <typename T1, typename T2>
  static return_type apply(const T1&, const T2&) {
    return 0;
  }
};

struct GenTypeTest2 final : public tfel::tests::TestCase {
  GenTypeTest2()
      : tfel::tests::TestCase("TFEL/Utilities", "GenTypeTest2") {
  }  // end of GenTypeTest2
  tfel::tests::TestResult execute() final {
    using namespace std;
    using namespace tfel::utilities;

    using MyGenType = GenType<int, string>;

    MyGenType g;
    MyGenType g2;
    MyGenType g3;
    MyGenType g4;
    g = 21;
    g2 = string("s1");
    g3 = 3;

    TFEL_TESTS_CHECK_EQUAL(apply<my_function>(g), 42);
    TFEL_TESTS_CHECK_EQUAL(apply<my_function>(g2), 0);
    TFEL_TESTS_CHECK_EQUAL(apply<my_function2>(g, g2), 0);
    TFEL_TESTS_CHECK_EQUAL(apply<my_function2>(g, g3), 63);

    return this->result;
  }  // end of execute()
};

TFEL_TESTS_GENERATE_PROXY(GenTypeTest2, "GenType");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("gentype2.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
