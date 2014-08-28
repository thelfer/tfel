/*! 
 * \file   tests/Math/tvector5.cxx
 * \brief
 * \author Helfer Thomas
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cmath>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/tvector.hxx"

struct TVectorTest5
  : public tfel::tests::TestCase
{

  TVectorTest5()
    : tfel::tests::TestCase("TFEL/Math",
			    "TVectorTesT5")
  {} // end of TVectorTest

  tfel::tests::TestResult
  execute(void)
  {
    using namespace std;
    using namespace tfel::math;
    const double v1_values[3u] = {1.2,-4.2,-0.3};
    tvector<3u,double> v1(v1_values);
    cout << "v1      : " << v1      << endl;
    cout << "abs(v1) : " << abs(v1) << endl;
    return this->result;
  } // end of execute

};

TFEL_TESTS_GENERATE_PROXY(TVectorTest5,
			  "TVectorTest5");

int main(void)
{
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("tvector5.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main
