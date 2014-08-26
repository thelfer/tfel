/*! 
 * \file   TinyVectorOfTinyVectorFromTinyVectorView.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  08 févr. 2013
 */

#include<cmath>
#include<cassert>
#include<cstdlib>
#include<limits>

#include"TFEL/Tests/Test.hxx"
#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/Vector/TinyVectorOfTinyVectorFromTinyVectorView.hxx"

struct TinyVectorOfTinyVectorFromTinyVectorViewTest
  : public tfel::tests::TestCase
{
  
  TinyVectorOfTinyVectorFromTinyVectorViewTest()
    : tfel::tests::TestCase("TFEL/Math",
			    "TinyVectorOfTinyVectorFromTinyVectorView")
  {} // end of TinyVectorOfTinyVectorFromTinyVectorViewTest
  
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::math;
    const double eps = 100*std::numeric_limits<double>::epsilon();
    
    double values[10] = {1.1,10.1,
			 2.9,9.2,3.8,8.3,
			 4.7,7.4,5.6,6.5};
    tvector<10,double> v(values);
    TinyVectorOfTinyVectorFromTinyVectorView<2,10,2,2,double>::type view(v);
    const tvector<2,double>& s1 = view(0);
    tvector<2,double>& s2 = view(1);
    tvector<2,double>::size_type i;
    for(i=0;i!=s1.size();++i){
      unsigned short idx;
      idx = static_cast<unsigned short>(i+2u);
      TFEL_TESTS_ASSERT(abs(s1(i)-v(idx))<eps);
      idx = static_cast<unsigned short>(i+4u);
      TFEL_TESTS_ASSERT(abs(s2(i)-v(idx))<eps);
    }
    s2 += tvector<2,double>(1.5);
    for(i=0;i!=s1.size();++i){
      unsigned short idx;
      idx = static_cast<unsigned short>(i+4u);
      TFEL_TESTS_ASSERT(abs(s2(i)-values[idx]-1.5)<eps);
      TFEL_TESTS_ASSERT(abs(v(idx)-values[idx]-1.5)<eps);
    }
    return this->result;

  } // end of execute

}; // end of TinyVectorOfTinyVectorFromTinyVectorViewTest

TFEL_TESTS_GENERATE_PROXY(TinyVectorOfTinyVectorFromTinyVectorViewTest,"TinyVectorOfTinyVectorFromTinyVectorViewTest");


int main(void)
{
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("TinyVectorOfTinyVectorFromTinyVectorView.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main

