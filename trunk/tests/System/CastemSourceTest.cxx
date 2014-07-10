/*! 
 * \file  CastemSourceTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 déc. 2012
 */


#include<iostream>
#include<cstdlib>
#include<cmath>

#include<TFEL/System/ExternalLibraryManager.hxx>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

struct CastemSourceTest
  : public tfel::tests::TestCase
{
  CastemSourceTest()
    : tfel::tests::TestCase("TFEL/System",
			    "CastemSourceTest")
  {} // end of CastemSourceTest

  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::system;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    const string f = elm.getSource(".libs/libCastemMaterialLaw.so","Test");
    TFEL_TESTS_ASSERT(f=="Test.mfront");
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(CastemSourceTest,
			  "CastemSourceTest");

int main(void)
{
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("CastemSource.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main

