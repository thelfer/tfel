/*!
 * \file   qt2.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   18 Jul 2006
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

  using namespace std;
  using namespace tfel::math;

  typedef PowUnit_<1,2,Mass>::type SquareMass;
  
  qt<SquareMass> v1(1.5);
  qt<Mass>        v2;

  v2 = v1 * v1;

  assert(SquareMass::getName()=="[kg^1/2,m^0,s^0,A^0,K^0,cd^0,mol^0]");
  assert(std::abs(v2.getValue()-2.25)<1.e-14);

  return EXIT_SUCCESS;

}
