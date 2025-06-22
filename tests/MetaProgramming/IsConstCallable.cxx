/*!
 * \file  IsConstCallable.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15 févr. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <iostream>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "TFEL/Metaprogramming/IsConstCallable.hxx"
#include "TFEL/Metaprogramming/ResultOf.hxx"

struct ConstCallableType {
  void operator()(double) const;
};

struct IsConstCallableTest final : public tfel::tests::TestCase {
  IsConstCallableTest()
      : tfel::tests::TestCase("TFEL/Math", "IsConstCallableTest") {
  }  // end of IsConstCallableTest
  tfel::tests::TestResult execute() override {
    using namespace tfel::meta;
    TFEL_TESTS_STATIC_ASSERT((!IsConstCallable<std::string, double>::cond));
    TFEL_TESTS_STATIC_ASSERT(
        (IsConstCallable<ConstCallableType, double>::cond));
    TFEL_TESTS_STATIC_ASSERT((IsConstCallable<ConstCallableType, int>::cond));
    TFEL_TESTS_STATIC_ASSERT(
        (!IsConstCallable<ConstCallableType, std::string>::cond));
    TFEL_TESTS_STATIC_ASSERT((
        std::is_same<ResultOf<std::string, double>::type, InvalidType>::value));
    TFEL_TESTS_STATIC_ASSERT(
        (std::is_same<ResultOf<ConstCallableType, double>::type, void>::value));
    return this->result;
  }  // end of execute
};

TFEL_TESTS_GENERATE_PROXY(IsConstCallableTest, "IsConstCallableTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("ResultOf.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
