/*!
 * \file   stensor2.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   09 Jun 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

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

//   cout << s2 << endl;

  s(0) = Complex<float>(12.f);
  s(1) = Complex<float>(-3.);
  s(2) = Complex<float>(-1.);
  s2(0) = Complex<float>(2.f);
  s2(1) = Complex<float>(1.f);
  s2(2) = Complex<float>(4.f);

//   cout << trace(s+0.5f*s2) << endl;

//   cout << 2.f*Complex<float>(4.f) << endl;

  return EXIT_SUCCESS;
}
