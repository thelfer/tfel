/*!
 * \file   array.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 jui 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<cassert>
#include<algorithm>

#include"TFEL/Math/Array/ArrayRunTimeProperties.hxx"
#include"TFEL/Math/array.hxx"
#include"TFEL/Math/functions.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;

  array<1u>::RunTimeProperties RT(4);
  array<1u> a(RT,2.);
  array<1u> b(RT,4.);
  array<1u> c;
  array<1u> d(RT,4.);

  c = d;
  std::fill(d.begin(),d.end(),12);
  c = sin(2*a + b);
  c*=2;
  c/=3;
  c*=(b+a);
  a=-c;

  return EXIT_SUCCESS;
}
