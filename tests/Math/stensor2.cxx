/*!
 * \file   stensor2.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   09 Jun 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cmath>
#include <cassert>
#include <cstdlib>

#include "Math/General/Complex.hxx"
#include "Math/stensor.hxx"

int main(void){

  using namespace tfel::math;
  using namespace std;

  stensor<1,Complex<float> > s;
  stensor<1,Complex<float> > s2;

  s(0) = Complex<float>(2.f);
  s(1) = Complex<float>(3.1415929f);
  s(2) = Complex<float>(4231.421f);

  s2 = 0.5f*s;

  assert((abs(real(s2(0)-Complex<float>(1.f)))<1.e-3));
  assert(abs(imag(s2(0)))<1.e-3);

  assert((abs(real(s2(1)-Complex<float>(1.5708f)))<1.e-3));
  assert(abs(imag(s2(1)))<1.e-3);

  assert((abs(real(s2(2)-Complex<float>(2115.7)))<1.));
  assert(abs(imag(s2(2)))<1.e-3);

  return EXIT_SUCCESS;
}
