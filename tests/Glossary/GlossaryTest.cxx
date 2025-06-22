/*!
 * \file   GlossaryTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08/07/2022
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

#include <string>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"

struct GlossaryTest final : public tfel::tests::TestCase {
  GlossaryTest()
      : tfel::tests::TestCase("TFEL/Glossary", "GlossaryTest") {
  }  // end of MyTest
  tfel::tests::TestResult execute() override {
    this->test1();
    this->test2();
    this->test3();
    return this->result;
  }  // end of execute
 private:
  void test1() {
    using Glossary = tfel::glossary::Glossary;
    auto& g = Glossary::getGlossary();
    const auto& e = g.getGlossaryEntry(Glossary::YoungModulus);
    TFEL_TESTS_ASSERT(e.getUnit("SI") == "Pa");
  }
  void test2() {
    using Glossary = tfel::glossary::Glossary;
    auto& g = Glossary::getGlossary();
    const auto& f = g.getGlossaryEntry(Glossary::Porosity);
    TFEL_TESTS_ASSERT(f.hasLowerPhysicalBound("SI"));
    TFEL_TESTS_ASSERT(f.getLowerPhysicalBound("SI") == "0");
    TFEL_TESTS_ASSERT(f.hasUpperPhysicalBound("SI"));
    TFEL_TESTS_ASSERT(f.getUpperPhysicalBound("SI") == "1");
  }
  void test3() {
    using Glossary = tfel::glossary::Glossary;
    auto& g = Glossary::getGlossary();
    const auto& T = g.getGlossaryEntry(Glossary::Temperature);
    TFEL_TESTS_ASSERT(T.hasLowerPhysicalBound("SI"));
    TFEL_TESTS_ASSERT(T.getLowerPhysicalBound("SI") == "0");
    TFEL_TESTS_ASSERT(!T.hasUpperPhysicalBound("SI"));
  }
};

TFEL_TESTS_GENERATE_PROXY(GlossaryTest, "Glossary");

int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("Glossary.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
