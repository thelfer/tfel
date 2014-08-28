/*! 
 * \file  tests/Math/tmatrix4.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 déc. 2012
 */

#include<cmath>
#include<limits>
#include<fstream>
#include<cstdlib>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/tmatrix.hxx"

struct TMatrix4Test
  : public tfel::tests::TestCase
{
  TMatrix4Test()
    : tfel::tests::TestCase("TFEL/Math",
			    "TMatrix4Test")
  {} // end of TMatrix4Test
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::math;
    const double eps = 10*numeric_limits<double>::epsilon();
    tmatrix<3,3> J;
    J(0,0) = 0;
    J(0,1) = 1;
    J(0,2) = 0;
    J(1,0) = 3;
    J(1,1) = 2;
    J(1,2) = 3;
    J(2,0) = 4;
    J(2,1) = 5;
    J(2,2) = 6;
    TFEL_TESTS_ASSERT(abs(det(J)+6)<eps);
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(TMatrix4Test,
			  "TMatrix4Test");

int main(void)
{
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("TMatrix4.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main

