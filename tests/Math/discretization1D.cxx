/*! 
 * \file  discretization1D.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 déc. 2011
 */

#include<cmath>
#include<vector>
#include<cstdlib>

#include"TFEL/Math/Discretization1D.hxx"

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"


struct Discretization1DTest
  : public tfel::tests::TestCase
{
  Discretization1DTest()
    : tfel::tests::TestCase("TFEL/Math",
			    "Discretization1DTest")
  {} // end of Discretization1DTest

  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::math;
    vector<double> v;
    const double eps = 1.e-8;
    geometricProgression(v,2., 17.,
			 0.1, 5.,10u);
    TFEL_TESTS_ASSERT(v.size()==11u);
    TFEL_TESTS_ASSERT(abs(v[0]-2.)<eps);
    TFEL_TESTS_ASSERT(abs(v[1]-2.2320018)<eps);
    TFEL_TESTS_ASSERT(abs(v[2]-2.55317371)<eps);
    TFEL_TESTS_ASSERT(abs(v[3]-2.99778834)<eps);
    TFEL_TESTS_ASSERT(abs(v[4]-3.613291)<eps);
    TFEL_TESTS_ASSERT(abs(v[5]-4.46536267)<eps);
    TFEL_TESTS_ASSERT(abs(v[6]-5.64492887)<eps);
    TFEL_TESTS_ASSERT(abs(v[7]-7.27786241)<eps);
    TFEL_TESTS_ASSERT(abs(v[8]-9.53841545)<eps);
    TFEL_TESTS_ASSERT(abs(v[9]-12.6678141)<eps);
    TFEL_TESTS_ASSERT(abs(v[10]-17)<eps);
    return this->result;
  } // end of execute

}; // end of struct Discretization1DTest

TFEL_TESTS_GENERATE_PROXY(Discretization1DTest,
			  "Discretization1DTest");

int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("Discretization1D.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main
