/*! 
 * \file  CastemParameterTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 déc. 2012
 */

#include<iostream>
#include<cstdlib>
#include<cmath>

#include<TFEL/System/ExternalLibraryManager.hxx>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

struct CastemParameterTest
  : public tfel::tests::TestCase
{
  CastemParameterTest()
    : tfel::tests::TestCase("TFEL/System",
			    "CastemParameterTest")
  {} // end of CastemParameterTest

  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::system;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    CastemFunctionPtr f;
    const double eps = 1.e-13;
    f = elm.getCastemFunction(".libs/libCastemMaterialLaw.so","Test");
    TFEL_TESTS_ASSERT(abs(f(static_cast<double *>(0))-3.)<eps);
    elm.setParameter(".libs/libCastemMaterialLaw.so","Test",
		     "kmin",2.7);
    TFEL_TESTS_ASSERT(abs(f(static_cast<double *>(0))-2.7)<eps);
    
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(CastemParameterTest,
			  "CastemParameterTest");

int main(void)
{
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("CastemParameter.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main
