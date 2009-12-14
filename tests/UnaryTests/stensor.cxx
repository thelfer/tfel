#include<iostream>
#include<cstdlib>
#include<cmath>

#include<cassert>

#include "Math/Vector/VectorUtilities.hxx"
#include "Math/stensor.hxx"

template<class T>
void function(const tfel::math::StensorConcept<T>& x){
  std::cout << x(0) << std::endl;
  std::cout << x(1) << std::endl;
  std::cout << x(2) << std::endl;
  std::cout << "-------" << std::endl;
}

int main(void){
  
  tfel::math::stensor<1,int>    v1;
  tfel::math::stensor<1,double>  v2;
  tfel::math::stensor<1,double> v3(0.);

  assert(std::fabs(v3(0))<1.e-14);
  assert(std::fabs(v3(1))<1.e-14);
  assert(std::fabs(v3(2))<1.e-14);

  v1(0)=12;
  v1(1)=13;
  v1(2)=14;

  v2 = v1;
  
  assert(std::fabs(v2(0)-12.)<1.e-14);
  assert(std::fabs(v2(1)-13.)<1.e-14);
  assert(std::fabs(v2(2)-14.)<1.e-14);

  v2(0)=7.f;
  v2(1)=4.23f;

  v3 = 0.5*v1 + v2;

  assert(std::fabs(v3(0)-13.)<1.e-7);
  assert(std::fabs(v3(1)-10.73)<1.e-7);
  assert(std::fabs(v3(2)-21.)<1.e-7);
    
  std::cout << "success" << std::endl;

  return EXIT_SUCCESS;

}
