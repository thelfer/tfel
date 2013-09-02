/*! 
 * \file   tensor3.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  1 sept. 2013
 */

#include<cmath>
#include<limits>
#include<fstream>
#include<cstdlib>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/tensor.hxx"

struct Tensor3Test
  : public tfel::tests::TestCase
{
  Tensor3Test()
    : tfel::tests::TestCase("TFEL/Math",
			    "Tensor3Test")
  {} // end of Tensor3Test
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::math;
    const double eps = 10*numeric_limits<double>::epsilon();
    tensor<3,double> t;
    t(0) = 0;
    t(3) = 1;
    t(5) = 0;
    t(4) = 3;
    t(1) = 2;
    t(7) = 3;
    t(6) = 4;
    t(8) = 5;
    t(2) = 6;
    TFEL_TESTS_ASSERT(abs(det(t)+6)<eps);
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(Tensor3Test,
			  "Tensor3Test");

int main(void)
{
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("Tensor3.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main

