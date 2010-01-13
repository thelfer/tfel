/*!
 * \file   st2tost2-3.cxx
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
  using namespace std;
  using namespace tfel::math;

  st2tost2<2> Id  = st2tost2<2>::Id();
  st2tost2<2> IxI = st2tost2<2>::IxI();
  st2tost2<2> J   = st2tost2<2>::J();
  st2tost2<2> K   = st2tost2<2>::K();

  assert(abs(Id(0,0)-1.)<1.e-14);
  assert(abs(Id(0,1))<1.e-14);
  assert(abs(Id(0,2))<1.e-14);
  assert(abs(Id(0,3))<1.e-14);

  assert(abs(Id(1,0))<1.e-14);
  assert(abs(Id(1,1)-1.)<1.e-14);
  assert(abs(Id(1,2))<1.e-14);
  assert(abs(Id(1,3))<1.e-14);

  assert(abs(Id(2,0))<1.e-14);
  assert(abs(Id(2,1))<1.e-14);
  assert(abs(Id(2,2)-1.)<1.e-14);
  assert(abs(Id(2,3))<1.e-14);

  assert(abs(Id(3,0))<1.e-14);
  assert(abs(Id(3,1))<1.e-14);
  assert(abs(Id(3,2))<1.e-14);
  assert(abs(Id(3,3)-1.)<1.e-14);

  assert(abs(IxI(0,0)-1.)<1.e-14);
  assert(abs(IxI(0,1)-1.)<1.e-14);
  assert(abs(IxI(0,2)-1.)<1.e-14);
  assert(abs(IxI(0,3))<1.e-14);

  assert(abs(IxI(1,0)-1.)<1.e-14);
  assert(abs(IxI(1,1)-1.)<1.e-14);
  assert(abs(IxI(1,2)-1.)<1.e-14);
  assert(abs(IxI(1,3))<1.e-14);

  assert(abs(IxI(2,0)-1.)<1.e-14);
  assert(abs(IxI(2,1)-1.)<1.e-14);
  assert(abs(IxI(2,2)-1.)<1.e-14);
  assert(abs(IxI(2,3))<1.e-14);

  assert(abs(IxI(3,0))<1.e-14);
  assert(abs(IxI(3,1))<1.e-14);
  assert(abs(IxI(3,2))<1.e-14);
  assert(abs(IxI(3,3))<1.e-14);

  assert(abs(J(0,0)-1./3.)<1.e-14);
  assert(abs(J(0,1)-1./3.)<1.e-14);
  assert(abs(J(0,2)-1./3.)<1.e-14);
  assert(abs(J(0,3))<1.e-14);

  assert(abs(J(1,0)-1./3.)<1.e-14);
  assert(abs(J(1,1)-1./3.)<1.e-14);
  assert(abs(J(1,2)-1./3.)<1.e-14);
  assert(abs(J(1,3))<1.e-14);

  assert(abs(J(2,0)-1./3.)<1.e-14);
  assert(abs(J(2,1)-1./3.)<1.e-14);
  assert(abs(J(2,2)-1./3.)<1.e-14);
  assert(abs(J(2,3))<1.e-14);

  assert(abs(J(3,0))<1.e-14);
  assert(abs(J(3,1))<1.e-14);
  assert(abs(J(3,2))<1.e-14);
  assert(abs(J(3,3))<1.e-14);

  assert(abs(K(0,0)-2./3.)<1.e-13);
  assert(abs(K(0,1)+1./3.)<1.e-14);
  assert(abs(K(0,2)+1./3.)<1.e-14);
  assert(abs(K(0,3))<1.e-14);

  assert(abs(K(1,0)+1./3.)<1.e-14);
  assert(abs(K(1,1)-2./3.)<1.e-14);
  assert(abs(K(1,2)+1./3.)<1.e-14);
  assert(abs(K(1,3))<1.e-14);

  assert(abs(K(2,0)+1./3.)<1.e-14);
  assert(abs(K(2,1)+1./3.)<1.e-14);
  assert(abs(K(2,2)-2./3.)<1.e-14);
  assert(abs(K(2,3))<1.e-14);

  assert(abs(K(3,0))<1.e-14);
  assert(abs(K(3,1))<1.e-14);
  assert(abs(K(3,2))<1.e-14);
  assert(abs(K(3,3)-1.)<1.e-14);

  return EXIT_SUCCESS;
}

