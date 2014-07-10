/*!
 * \file   Function.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   05 nov 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cstdlib>
#include<cassert>

#include"TFEL/Math/functions.hxx"

int main(void){

  using namespace std;
  USING_TFEL_FUNCTIONS;

  assert(abs(cos(12.)-std::cos(12.))<1.e-14);
  assert(abs(sin(12.)-std::sin(12.))<1.e-14);
  assert(abs((cos+sin)(12.)-std::cos(12.)-std::sin(12.))<1.e-14);
  assert(abs((exp[sin])(12.)-std::exp(std::sin(12.)))<1.e-14);
  assert(abs((2.321*sin)(12.)-2.321*std::sin(12.))<1.e-14);
  assert(abs((sin*3.421)(12.)-3.421*std::sin(12.))<1.e-14);

  return EXIT_SUCCESS;
}
