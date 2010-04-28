/*!
 * \file   test12.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   16 Oct 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cstdlib>

#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Math/tvector.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::utilities;

  tvector<3,double> v1(1.);
  tvector<3,double> v2(1.5);
  tvector<3,double> v3(1.76);

  assert(abs((v1+v2+v3)(0)-4.26)<1.e-14);
  assert(abs((v1+v2+v3)(1)-4.26)<1.e-14);
  assert(abs((v1+v2+v3)(2)-4.26)<1.e-14);

  return EXIT_SUCCESS;
}
