/*!
 * \file   DataTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   13/09/2016
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

#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Utilities/Data.hxx"

struct DataTest final : public tfel::tests::TestCase {
  DataTest()
      : tfel::tests::TestCase("TFEL/Utilities", "DataTest") {}  // end of MyTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    return this->result;
  }

 private:
  void test1() {
    using namespace tfel::utilities;
    CxxTokenizer t;
    // C++ style comments
    t.parseString(
        "{"
        "\"value\"  : \"a string\","
        "\"value2\" : 12.e5,"
        "\"value3\" : -45,"
        "\"value4\" : {14,\"test\",-3.14159,"
        "              {\"key1\":4,\"key2\":-5}}"
        "}");
    auto c = t.begin();
    const auto d = Data::read(c, t.end());
    const auto m = d.get<DataMap>();
    TFEL_TESTS_ASSERT(m.size() == 4u);
    TFEL_TESTS_ASSERT(m.count("value") == 1u);
    TFEL_TESTS_ASSERT(m.count("value2") == 1u);
    TFEL_TESTS_ASSERT(m.count("value3") == 1u);
    TFEL_TESTS_ASSERT(m.count("value4") == 1u);
    const auto d1 = m.at("value");
    TFEL_TESTS_ASSERT(d1.is<std::string>());
    TFEL_TESTS_ASSERT(d1.get<std::string>() == "a string");
    const auto d2 = m.at("value2");
    TFEL_TESTS_ASSERT(d2.is<double>());
    TFEL_TESTS_ASSERT(std::abs(d2.get<double>() - 12.e5) < 1);
    const auto d3 = m.at("value3");
    TFEL_TESTS_ASSERT(d3.is<int>());
    TFEL_TESTS_ASSERT(d3.get<int>() == -45);
    const auto d4 = m.at("value4");
    TFEL_TESTS_ASSERT(d4.is<std::vector<Data>>());
    const auto vd = d4.get<std::vector<Data>>();
    TFEL_TESTS_ASSERT(vd.size() == 4u);
    TFEL_TESTS_ASSERT(vd.at(0).is<int>());
    TFEL_TESTS_ASSERT(vd.at(0).get<int>() == 14);
    TFEL_TESTS_ASSERT(vd.at(1).is<std::string>());
    TFEL_TESTS_ASSERT(vd.at(1).get<std::string>() == "test");
    TFEL_TESTS_ASSERT(vd.at(2).is<double>());
    TFEL_TESTS_ASSERT(std::abs(vd.at(2).get<double>() + 3.14159) < 1.e-14);
    TFEL_TESTS_ASSERT((vd.at(3).is<DataMap>()));
    const auto& md = vd.at(3).get<DataMap>();
    TFEL_TESTS_ASSERT(md.size() == 2u);
    TFEL_TESTS_ASSERT(md.count("key1") == 1u);
    TFEL_TESTS_ASSERT(md.count("key2") == 1u);
    TFEL_TESTS_ASSERT(md.at("key1").is<int>());
    TFEL_TESTS_ASSERT(md.at("key1").get<int>() == 4);
    TFEL_TESTS_ASSERT(md.at("key2").is<int>());
    TFEL_TESTS_ASSERT(md.at("key2").get<int>() == -5);
  }  // end of test1

  void test2() {
    using namespace tfel::utilities;
    CxxTokenizer t;
    t.parseString(
        "{\n"
        " criterion: \"Mises\",\n"
        " isotropic_hardening : \"Linear\" {R0 : 150e6, H : 2e9},\n"
        " isotropic_hardening : \"Voce\" {R0 : 260e6, Rinf : 280e6, b : "
        "31.51},\n"
        " kinematic_hardening : \"Chaboche 2012\" {C : 250e7, D : 100, m : 2, "
        "w : 0.6},\n"
        " kinematic_hardening : \"Chaboche 2012\" {C :  3e11, D : 500, m : 1, "
        "w : 0.0}\n"
        "}");
    DataParsingOptions o;
    o.allowMultipleKeysInMap = true;
    auto p = t.begin();
    const auto m = Data::read(p, t.end(), o).get<DataMap>();
    TFEL_TESTS_ASSERT(m.count("criterion") == 1u);
    TFEL_TESTS_ASSERT(m.count("isotropic_hardening") == 1u);
    TFEL_TESTS_ASSERT(m.at("isotropic_hardening").is<std::vector<Data>>());
    TFEL_TESTS_ASSERT(m.at("kinematic_hardening").is<std::vector<Data>>());
  }  // end of test2
};

TFEL_TESTS_GENERATE_PROXY(DataTest, "CxxTokenizer");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("Data.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
