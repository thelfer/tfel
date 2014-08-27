/*!
 * \file   tmatrix2.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   12 avr 2008
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"

int
main(void){
  
  using namespace std;
  using namespace tfel::math;

  tmatrix<2,2,double> m;
  tvector<2,double> v;
  tvector<2,double> v2;

  m(0,0)=0.;
  m(0,1)=1.;
  m(1,0)=2.;
  m(1,1)=3.;
  
  v(0) = 0.5;
  v(1) = 0.25;

  v2 = v*m;

  return EXIT_SUCCESS;

} // end of main

