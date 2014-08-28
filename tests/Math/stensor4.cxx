/*!
 * \file   tests/Math/stensor4.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   1 Fév 2010
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <iostream>
#include <cmath>
#include <cassert>
#include <cstdlib>

#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"

#include<iostream>

int main(void){

  using namespace std;
  using namespace tfel::math;

  const stensor<1>  Id  = stensor<1>::Id();
  const st2tost2<1> IxI = st2tost2<1>::IxI();

  stensor<1> a;
  a(0)=1.;
  a(1)=2.;
  a(2)=3.;

  cout << (Id^Id) << endl;
  cout << IxI << endl;
  cout << (a|(Id^Id)) << endl;

  return EXIT_SUCCESS;
}
