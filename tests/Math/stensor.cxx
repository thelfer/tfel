
#include<iostream>
#include<cstdlib>

#include "Math/stensor.hxx"
#include "function_tests.hxx"

int main(void){

  using namespace tfel::math;  

  typedef stensor<1> stensor1;
  typedef stensor<1,unsigned short> stensor2;

  stensor1 v1;
  stensor2 v2;

  v2(0)=2;
  v2(1)=1;
  v2(2)=5;

  v1(0)=4.;
  v1(1)=1.;
  v1(2)=25.;

  std::cout << stensor<1>::getName() << std::endl;
  std::cout << stensor<1,unsigned short>::getName() << std::endl;

  function(v1);
  function(2*v1);
  function(v1*2);
  function(v2);
  function(v1+v2);
  function(v1-v2);
  function(v1+v2+v1);
  function(v1+v2-v1);
  function(2.*v1+v2);
  function(v1+2.*(v1+v2));
  
  std::cout << "success" << std::endl;

  return EXIT_SUCCESS;

}
