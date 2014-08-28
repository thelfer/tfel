/*!
 * \file   tests/Math/qt3.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   04 aoû 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cassert>
#include<cmath>

#include"TFEL/Math/qt.hxx"

int main(void){

  using namespace tfel::math;
  using namespace std;

  qt<Mass,int> a(-12);
  qt<Mass,int> b(14);

  assert(a<=b);
  assert(a<b);
  assert(b>a);
  assert(b>=a);
  assert(b!=a);
  assert(!(b==a));
  assert(abs(a)==12);

  return EXIT_SUCCESS;
}
