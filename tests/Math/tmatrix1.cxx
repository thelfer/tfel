/*!
 * \file   tmatrix1.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 avr 2008
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cstdlib>
#include<cassert>

#include "Math/tmatrix.hxx"
#include "Math/TinyMatrixSolve.hxx"

int main(void){
  
  using namespace std;
  using namespace tfel::math;

  tmatrix<2,2,double> m;
  tmatrix<2,2,double> m2;

  m(0,0)=0.;
  m(0,1)=1.;
  m(1,0)=2.;
  m(1,1)=3.;
  
  m2 = m;

  assert(abs(m(0,0)-0.)<1.e-14);
  assert(abs(m(0,1)-1.)<1.e-14);
  assert(abs(m(1,0)-2.)<1.e-14);
  assert(abs(m(1,1)-3.)<1.e-14);
  assert(abs(m2(0,0)-0.)<1.e-14);
  assert(abs(m2(0,1)-1.)<1.e-14);
  assert(abs(m2(1,0)-2.)<1.e-14);
  assert(abs(m2(1,1)-3.)<1.e-14);

  m2 = 2.*m;

  assert(abs(m(0,0)-0.)<1.e-14);
  assert(abs(m(0,1)-1.)<1.e-14);
  assert(abs(m(1,0)-2.)<1.e-14);
  assert(abs(m(1,1)-3.)<1.e-14);
  assert(abs(m2(0,0)-0.)<1.e-14);
  assert(abs(m2(0,1)-2.)<1.e-14);
  assert(abs(m2(1,0)-4.)<1.e-14);
  assert(abs(m2(1,1)-6.)<1.e-14);

  return EXIT_SUCCESS;

}

