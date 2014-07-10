/*!
 * \file   tvector3.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   08 avr 2008
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"

int
main(void)
{
  using namespace std;
  using namespace tfel::math;
  tvector<2> v1;
  tvector<2> v2;
  tmatrix<2,2> m;
  v1(0)=0.5;
  v1(1)=0.25;
  v2(0)=4.;
  v2(1)=2.;
  m = v1^v2;
  return EXIT_SUCCESS;
}
