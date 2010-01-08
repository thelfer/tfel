/*!
 * \file   vector1.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   05 Jan 2007
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<iterator>

#include"Math/functions.hxx"
#include"Math/vector.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using tfel::math::vector;
  USING_TFEL_FUNCTIONS;

  const double value[]  = {0,1.32,2.12,4.33,12.23};
  const double value2[] = {1.2,3.2,2.122,5.3,6.59};

  vector<double> v(value,value+5);
  vector<double> v2(value2,value2+5);
  vector<double> v3(5);

  v3 = (exp[cos])(v+v2);
  // comparing with Octave results
  assert(abs(v3(0)-1.43671)<1.e-4);
  assert(abs(v3(1)-0.825964)<1.e-4);
  assert(abs(v3(2)-0.63557)<1.e-4);
  assert(abs(v3(3)-0.375681)<1.e-4);
  assert(abs(v3(4)-2.71709)<1.e-4);

  v3 = Cst<2>()(v+v2);
  assert(abs(v3(0)-2.)<1.e-14);
  assert(abs(v3(1)-2.)<1.e-14);
  assert(abs(v3(2)-2.)<1.e-14);
  assert(abs(v3(3)-2.)<1.e-14);
  assert(abs(v3(4)-2.)<1.e-14);

  v3 = Cst<2,3>()(v+v2);
  assert(abs(v3(0)-2./3.)<1.e-14);
  assert(abs(v3(1)-2./3.)<1.e-14);
  assert(abs(v3(2)-2./3.)<1.e-14);
  assert(abs(v3(3)-2./3.)<1.e-14);
  assert(abs(v3(4)-2./3.)<1.e-14);

  v3 = power<2>()(v+v2);
  // comparing with Octave results
  assert(abs(v3(0)-1.44)<1.e-4);
  assert(abs(v3(1)-20.4304)<1.e-4);
  assert(abs(v3(2)-17.9946)<1.e-4);
  assert(abs(v3(3)-92.7369)<1.e-4);
  assert(abs(v3(4)-354.192)<1.e-3);

  v3 = sin[power<2>()](v+v2);
  // comparing with Octave results
  assert(abs(v3(0)-0.991458)<1.e-4);
  assert(abs(v3(1)-0.99995)<1.e-4);
  assert(abs(v3(2)+0.754566)<1.e-4);
  assert(abs(v3(3)+0.998206)<1.e-4);
  assert(abs(v3(4)-0.722609)<1.e-4);

  return EXIT_SUCCESS;
}
