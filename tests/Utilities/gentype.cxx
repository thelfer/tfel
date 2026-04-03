/*!
 * \file   tests/Utilities/gentype.cxx
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

struct GenTypeTest final : public tfel::tests::TestCase {
  GenTypeTest()
      : tfel::tests::TestCase("TFEL/Utilities", "GenTypeTest") {
  }  // end of MyTest
  tfel::tests::TestResult execute() override {
    using namespace std;
    using namespace tfel::meta;
    using namespace tfel::utilities;

    typedef GenerateTypeList<int, string>::type HoldedTypes;
    using MyGenType = GenTypeBase<HoldedTypes>;

    MyGenType test;
    MyGenType test2(string("string1"));
    MyGenType test3(test2);

    test = string("string2");
    TFEL_TESTS_ASSERT(test.is<string>());
    TFEL_TESTS_CHECK_EQUAL(test.get<string>(), "string2");
    TFEL_TESTS_CHECK_EQUAL(static_cast<const string&>(test), "string2");
    TFEL_TESTS_CHECK_EQUAL(test.getString(), "string2");

    test2 = string("string1");
    TFEL_TESTS_ASSERT(test2.is<string>());
    TFEL_TESTS_CHECK_EQUAL(test2.get<string>(), "string1");
    TFEL_TESTS_CHECK_EQUAL(static_cast<const string&>(test2), "string1");
    TFEL_TESTS_CHECK_EQUAL(test2.getString(), "string1");

    test3 = string("string1");
    TFEL_TESTS_ASSERT(test3.is<string>());
    TFEL_TESTS_CHECK_EQUAL(test3.get<string>(), "string1");
    TFEL_TESTS_CHECK_EQUAL(static_cast<const string&>(test3), "string1");
    TFEL_TESTS_CHECK_EQUAL(test3.getString(), "string1");

    test.set(12);
    TFEL_TESTS_ASSERT(test.is<int>());
    TFEL_TESTS_CHECK_EQUAL(test.get<int>(), 12);
    TFEL_TESTS_CHECK_EQUAL(static_cast<const int&>(test.get<int>()), 12);
    TFEL_TESTS_CHECK_EQUAL(test.getInt(), 12);

    test2 = test;
    TFEL_TESTS_ASSERT(test2.is<int>());
    TFEL_TESTS_CHECK_EQUAL(test2.get<int>(), 12);
    TFEL_TESTS_CHECK_EQUAL(static_cast<const int&>(test2.get<int>()), 12);
    TFEL_TESTS_CHECK_EQUAL(test2.getInt(), 12);

    return this->result;
  }  // end of execute()
};

TFEL_TESTS_GENERATE_PROXY(GenTypeTest, "GenType");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("gentype.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
