/*!
 * \file   qt.cxx
 * \brief  
 * 
 * \author Helfer thomas
 * \date   06 Jun 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cmath>
#include<cassert>

#include"TFEL/Math/qt.hxx"

int main(void){

  using namespace tfel::math; 

  qt<Mass> m1(100.);
  qt<Mass> m2(100.);
  qt<Mass> m3;
  qt<Acceleration,unsigned short> a(2);
  qt<Force> f;
  
  m3 = m1+0.5*m2;

  f  = m1*a;

  assert((qt<Mass>::getName()=="qt<[kg],double>"));
  assert((qt<Acceleration,unsigned short>::getName()=="qt<[Acceleration],unsigned short>"));
  assert((qt<Force>::getName()=="qt<[Force],double>"));

  assert(std::abs(m3.getValue()-150.)<1.e-14);
  assert(std::abs(f.getValue()-200.)<1.e-14);
  
  assert((std::abs(std::cos(qt<NoUnit>(12.))-std::cos(12.))<1.e-14));

  return EXIT_SUCCESS;

}
