/*!
 * \file   array.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   01 jui 2006
 */

#include<iostream>
#include<cstdlib>
#include<cassert>
#include<algorithm>

#include"Math/Array/ArrayRunTimeProperties.hxx"
#include"Math/array.hxx"
#include"Math/functions.hxx"

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

  cout << a   << endl;
  cout << b   << endl;
  cout << c   << endl;

  c = sin(2*a + b);

  cout << d   << endl;

  c*=2;

  cout << d   << endl;

  c/=3;

  cout << d   << endl;

  c*=(b+a);

  cout << d   << endl;

  a=-c;

  cout << a   << endl;

  return EXIT_SUCCESS;
}
