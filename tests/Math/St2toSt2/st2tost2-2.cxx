/*!
 * \file   st2tost2-2.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   19 mar 2008
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cstdlib>
#include<cassert>

#include"Math/st2tost2.hxx"
#include"Math/stensor.hxx"

int
main(void)
{
  using namespace tfel::math;
  using namespace std;

  st2tost2<2> D(1.);
  stensor<2>  s;
  stensor<2>  s1;

  s(0) = 1.;
  s(1) = 2.;
  s(2) = 3.;
  s(3) = 4.;

  s1 = D*s;

  assert(abs(s1(0)-10.)<1.e-14);
  assert(abs(s1(1)-10.)<1.e-14);
  assert(abs(s1(2)-10.)<1.e-14);
  assert(abs(s1(3)-10.)<1.e-14);

  return EXIT_SUCCESS;
}

