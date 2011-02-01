/*!
 * \file   stensor_eigenvalues.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   03 jui 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cassert>
#include<cstdlib>

#include"TFEL/Math/stensor.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;

  double vp1;
  double vp2;
  double vp3;
  stensor<1,double> s(0.);
  s(0)=1.;
  s(1)=2.5;
  s(2)=0.234;

  s.computeEigenValues(vp1,vp2,vp3);

  assert(abs(vp1-1.)<1.e-14);
  assert(abs(vp2-2.5)<1.e-14);
  assert(abs(vp3-0.234)<1.e-14);

  return EXIT_SUCCESS;
}
