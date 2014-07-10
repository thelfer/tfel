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

#include "fenv.h"

#include"TFEL/Math/stensor.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  double vp1;
  double vp2;
  double vp3;
  stensor<3,double> s(0.);
  s.computeEigenValues(vp1,vp2,vp3);
  assert(abs(vp1-0)<1.e-14);
  assert(abs(vp2-0)<1.e-14);
  assert(abs(vp3-0)<1.e-14);
  return EXIT_SUCCESS;
}
