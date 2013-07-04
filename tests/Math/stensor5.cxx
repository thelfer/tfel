#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cmath>

#include<cassert>

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/stensor.hxx"

int main(void){
  
  tfel::math::stensor<1,int>    v1;
  tfel::math::stensor<1,double>  v2;
  tfel::math::stensor<1,double> v3(0.);

  assert(std::abs(v3(0))<1.e-14);
  assert(std::abs(v3(1))<1.e-14);
  assert(std::abs(v3(2))<1.e-14);

  v1(0)=12;
  v1(1)=13;
  v1(2)=14;

  v2 = v1;
  
  assert(std::abs(v2(0)-12.)<1.e-14);
  assert(std::abs(v2(1)-13.)<1.e-14);
  assert(std::abs(v2(2)-14.)<1.e-14);

  v2(0)=7.f;
  v2(1)=4.23f;

  v3 = 0.5*v1 + v2;

  assert(std::abs(v3(0)-13.)<1.e-7);
  assert(std::abs(v3(1)-10.73)<1.e-7);
  assert(std::abs(v3(2)-21.)<1.e-7);
    
  return EXIT_SUCCESS;

}
