/*
 * \file test_Column.cpp
 * \author rp238441
 * \date 11 juin 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <string>
#include <memory>
#include <fstream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Check/Column.hxx"

struct test_Column final : public tfel::tests::TestCase {
  test_Column() : tfel::tests::TestCase("TFEL/Check", "Column") {}

  tfel::tests::TestResult execute() override {
    const double eps = 1.e-14;
    const std::string f = "test_Column.res";
    tfel::check::Column c("B");
    std::ofstream file{f};
    tfel::raise_if(!file,
                   "test_Column::execute: "
                   "could not open file '" +
                       f + "'");
    file << "tps B C \n";
    file << "1. 2. 2.\n";
    file << "2. 3. 1.\n";
    file.close();
    c.setFilename(f);
    TFEL_TESTS_ASSERT(std::abs(3. - c.getValues().at(1)) < eps);
    TFEL_TESTS_ASSERT(c.getName() == "B");
    return this->result;
  }
};

TFEL_TESTS_GENERATE_PROXY(test_Column, "Column");
