/*
 * \brief test_Column.cpp
 *
 * \file test_Column.cpp
 *
 * \date 11 juin 2013
 * \author rp238441
 *
 * Copyright © 2013
 */

#include<cmath>
#include<string>
#include<memory>
#include<fstream>
#include<stdexcept>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFELCheck/Column.hxx"

struct test_Column final
  : public tfel::tests::TestCase
{
  test_Column()
    : tfel::tests::TestCase("TFELCheck","Column")
  {}

  virtual tfel::tests::TestResult
  execute() override
  {
    const double eps = 1.e-14;
    const std::string f = "test_Column.res";
    tfel_check::Column c("B");
    std::ofstream file{f};
    if (!file) {
      throw std::runtime_error("test_Column::execute: "
			       "could not open file '"+f+"'");
    }
    file << "tps B C \n";
    file << "1. 2. 2.\n";
    file << "2. 3. 1.\n";
    file.close();
    c.setFilename(f);
    TFEL_TESTS_ASSERT(std::abs(3.-c.getValues().at(1))<eps);
    TFEL_TESTS_ASSERT(c.getName()=="B");
    return this->result;
  }
};

TFEL_TESTS_GENERATE_PROXY(test_Column,"Column");

