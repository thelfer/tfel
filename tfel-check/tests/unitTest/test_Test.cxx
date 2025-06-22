/*
 * \file test_Test.cpp
 * \brief test_Test.cpp
 *
 * tests the Test class
 * \date 11 juin 2013
 * \author Rémy Petkantchin
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFELCheck/NoInterpolation.hxx"
#include "TFELCheck/Interpolation.hxx"
#include "TFELCheck/Test.hxx"
#include "TFELCheck/AbsoluteComparison.hxx"

struct test_Test final : public tfel::tests::TestCase {
  test_Test() : tfel::tests::TestCase("TFELCheck", "Test") {}
  virtual tfel::tests::TestResult execute() override {
    std::ofstream file{"test_Test.res"};
    if (!file) {
      throw(
          std::runtime_error("test_Test::execute: "
                             "could not open file 'test_Test.res'"));
    }
    file << "tps B C \n";
    file << "1. 2. 2.\n";
    file << "2. 3. 1.\n";
    file.close();
    file.open("test_TestErr.res");
    if (!file) {
      throw(
          std::runtime_error("test_Test::execute: "
                             "could not open file 'test_TestErr.res'"));
    }
    file << "tps B C \n";
    file << "1. 2. 2.\n";
    file << "2. 3. 1.\n";
    file << "3. 3. 1.\n";
    file.close();
    // beginning of the tests
    tfel_check::Test t;
    t.setPrec(0.);
    t.setPrecision2(0.);
    t.setInterpolation(std::shared_ptr<tfel_check::Interpolation>(
        new tfel_check::NoInterpolation()));
    if (t.getInterpolation()->isConform()) {
      t.setColInterpolated(std::make_shared<tfel_check::Column>("tps"));
    }
    t.setAllowLessResults(false);
    t.setComparison(std::shared_ptr<tfel_check::Comparison>(
        new tfel_check::AbsoluteComparison()));
    t.setColA(std::make_shared<tfel_check::Column>("B"));
    t.setColB(std::make_shared<tfel_check::Column>("B"));
    t.setFileA("test_Test.res");
    t.setFileB("test_Test.res");
    {
      bool hasThrown = false;
      try {
        t.applyInterpolation();
      } catch (...) {
        hasThrown = true;
      }
      this->registerResult("check no throw : 't.applyInterpolation()'",
                           !hasThrown);
    }
    t.compare();
    TFEL_TESTS_ASSERT(t.hasSucceed());
    // a file with non matching column length
    t.setFileA("test_TestErr.res");
    TFEL_TESTS_CHECK_THROW(t.applyInterpolation(), std::runtime_error);
    // back to working file
    t.setFileA("test_Test.res");
    t.applyInterpolation();
    return this->result;
  }
};

TFEL_TESTS_GENERATE_PROXY(test_Test, "Test");
