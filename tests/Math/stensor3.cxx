/*!
 * \file   tests/Math/stensor3.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   07 Aug 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cmath>
#include <cassert>
#include <cstdlib>

#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/Stensor/StensorExternalTabStorage.hxx"
#include"TFEL/Math/Stensor/StensorExternalVoigtStorage.hxx"

#include<iostream>

int main(void){

  using namespace std;
  using namespace tfel::math;

  double  a[] ={1.,1.3,0.5,1.};
  double  b[] ={0.2,0.3,-0.4,3.4*std::sqrt(2.)};

  stensor<2,double,StensorExternalVoigt> sig(b);
  assert(abs(sig(0)-0.2)<1.e-14);
  assert(abs(sig(1)-0.3)<1.e-14);
  assert(abs(sig(2)+0.4)<1.e-14);
  assert(abs(sig(3)-3.4)<1.e-14);
  sig*=2u;
  assert(abs(sig(0)-0.4)<1.e-14);
  assert(abs(sig(1)-0.6)<1.e-14);
  assert(abs(sig(2)+0.8)<1.e-14);
  assert(abs(sig(3)-6.8)<1.e-14);
  
  stensor<2,double,StensorExternalTab> sig2(a);
  assert(abs(sig2(0)-1.)<1.e-13);
  assert(abs(sig2(1)-1.3)<1.e-14);
  assert(abs(sig2(2)-0.5)<1.e-14);
  assert(abs(sig2(3)-sqrt(2.))<1.e-14);
  sig2 *= 2u;
  assert(abs(sig2(0)-2.)<1.e-14);
  assert(abs(sig2(1)-2.6)<1.e-14);
  assert(abs(sig2(2)-1.)<1.e-14);
  assert(abs(sig2(3)-2*sqrt(2.))<1.e-14);

  return EXIT_SUCCESS;
}
