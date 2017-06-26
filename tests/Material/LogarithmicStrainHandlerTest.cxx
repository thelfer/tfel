/*!
 * \file   LogarithmicStrainHandlerTest.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   25 juin 2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<utility>
#include<iostream>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"
#include"TFEL/Material/LogarithmicStrainHandler.hxx"

struct LogarithmicStrainHandlerTest final
  : public tfel::tests::TestCase
{
  LogarithmicStrainHandlerTest()
    : tfel::tests::TestCase("TFEL/Material",
			    "LogarithmicStrainHandlerTest")
  {} // end of LogarithmicStrainHandlerTest
  virtual tfel::tests::TestResult
  execute() override
  {
    return this->result;
  }
};

TFEL_TESTS_GENERATE_PROXY(LogarithmicStrainHandlerTest,
			  "LogarithmicStrainHandlerTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main()
{
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("LogarithmicStrainHandlerTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
} // end of main

