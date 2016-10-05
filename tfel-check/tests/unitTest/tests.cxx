/*!
 * \file   tests.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   22 janv. 2016
 */

#include<iostream>
#include<cstdlib>
#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestManager.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main()
{
  auto& manager = tfel::tests::TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("tfel-check.xml");
  const auto r = manager.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
} // end of main
