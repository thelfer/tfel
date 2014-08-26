#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cmath>

#include<cassert>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/stensor.hxx"

struct STensorTest7
  : public tfel::tests::TestCase
{
  STensorTest7()
    : tfel::tests::TestCase("TFEL/Math",
			    "STensorTest7")
  {} // end of STensorTest7
  tfel::tests::TestResult
  execute()
  {
    using namespace std;  
    using namespace tfel::math;
    const double sqrt2 = sqrt(2.);
    const double v[6] = {2.3,4.7,3.1,7.2*sqrt2,0.9*sqrt2,1.32*sqrt2};
    stensor<3u,double> s(v);
    tvector<3u,double> vp;
    tmatrix<3u,3u,double> m;
    s.computeEigenVectors(vp,m);
    // cout << m(0,0) << " " << m(1,0) << " " << m(2,0) << endl;
    // cout << s(0)*m(0,0)+s(3)*m(1,0)/sqrt2+s(4)*m(2,0)/sqrt2 << endl;
    // cout << s(1)*m(1,0)+s(3)*m(0,0)/sqrt2+s(5)*m(2,0)/sqrt2 << endl;
    // cout << s(2)*m(2,0)+s(4)*m(0,0)/sqrt2+s(5)*m(1,0)/sqrt2 << endl;
    // cout << "vp : " << vp << endl;
    return this->result;
  } // end of execute
};


TFEL_TESTS_GENERATE_PROXY(STensorTest7,"STensorTest7");

int main(void){
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("stensor7.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
