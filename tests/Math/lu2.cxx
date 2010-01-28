/*! 
 * \file  test.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 avr 2009
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>

#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/matrix.hxx"
#include"TFEL/Math/LUSolve.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using tfel::math::vector;
  matrix<double> m(3,3);
  vector<double> b(3);

  m(0,0)=3.;
  m(0,1)=-1.;
  m(0,2)=2.;
  m(1,0)=1.;
  m(1,1)=2.;
  m(1,2)=3.;
  m(2,0)=2.;
  m(2,1)=-2.;
  m(2,2)=-1.;

  b(0) = 12;
  b(1) = 11;
  b(2) = 2;

  LUSolve::exe(m,b);

  assert(abs(b(0)-3)<1.e-14);
  assert(abs(b(1)-1)<1.e-14);
  assert(abs(b(2)-2)<1.e-14);

} // end of main
