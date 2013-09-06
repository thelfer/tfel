/*!
 * \file   StensorFromTinyMatrixColumnView.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>

#include"TFEL/Math/Stensor/StensorFromTinyMatrixColumnView.hxx"
#include"TFEL/Math/Stensor/StensorFromTinyMatrixRowView.hxx"

int
main(void)
{
  using namespace std;
  using namespace tfel::math;
  tmatrix<3,2> m(0.);
  StensorFromTinyMatrixColumnView<1,3,2,0,0>::type ms(m);
  StensorFromTinyMatrixColumnView<1,3,2,0,1>::type ms2(m);
  stensor<1> s(1.);
  ms(0) = 1.;
  ms(1) = 2.;
  ms(2) = 3.;
  s += ms;
  ms2 = ms + 2. * s;
} // end of main
