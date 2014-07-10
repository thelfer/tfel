/*! 
 * \file  stensor_changeBasis3.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 mai 2013
 */

#include<cmath>
#include<fstream>
#include<cstdlib>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"

struct StensorChangeBasis3Test
  : public tfel::tests::TestCase
{
  StensorChangeBasis3Test()
    : tfel::tests::TestCase("TFEL/Math",
			    "StensorChangeBasis3Test")
  {} // end of StensorChangeBasis3Test

  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::math;
    typedef double real;
    const real eps = 1.e-4;
    const real pi = 4.0 * atan(1.0);  
    unsigned short i,j,k,t;
    for(t=0;t!=10;++t){
      const real psi    = 2.*pi*(rand()/((double)RAND_MAX));
      const real co = cos(psi);
      const real si = sin(psi);
      // rotation matrix
      tmatrix<3u,3u,real> r(real(0));
      r(0,0) = co;
      r(0,1) = -si;
      r(2,2) = 1.;
      r(1,0) = si;
      r(1,1) = co;
      // stresses as a symmetric matrix
      tmatrix<3u,3u,real> sm(0.);
      for(i=0;i!=3;++i){
	sm(i,i) = 100e6*(rand()/((double)RAND_MAX));
      }
      sm(0,1) = sm(1,0) = 100e6*(rand()/((double)RAND_MAX));
      // stresses as a symmetric tensor
      stensor<2u,real> st(0.);
      for(i=0;i!=3;++i){
	st(i)=sm(i,i);
      }
      st(3)=sm(0,1)*sqrt(real(2));
      // change basis
      tmatrix<3u,3u,real> sm2;
      tmatrix<3u,3u,real> tmp;
      for(i=0;i!=3;++i){
	for(j=0;j!=3;++j){
	  tmp(i,j) = 0.;
	  for(k=0;k!=3;++k){
	    tmp(i,j) += sm(i,k)*r(k,j);
	  }
	}
      }
      for(i=0;i!=3;++i){
	for(j=0;j!=3;++j){
	  sm2(i,j) = 0.;
	  for(k=0;k!=3;++k){
	    sm2(i,j) += r(k,i)*tmp(k,j);
	  }
	}
      }
      st.changeBasis(r);
      for(i=0;i!=3;++i){
	TFEL_TESTS_ASSERT(abs(st(i)-sm2(i,i))<eps);
      }
      TFEL_TESTS_ASSERT(abs(st(3)-sm2(0,1)*sqrt(real(2)))<eps);
    }
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(StensorChangeBasis3Test,
			  "StensorChangeBasis3Test");

int main(void)
{
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("StensorChangeBasis3.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main

