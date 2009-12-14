/*!
 * \file   st2tost2-2.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   19 mar 2008
 */

#include<iostream>
#include<cstdlib>

#include"Math/st2tost2.hxx"
#include"Math/stensor.hxx"

int
main(void)
{
  using namespace tfel::math;
  using namespace std;

  st2tost2<2> D(1.);
  stensor<2>  s(4.);
  stensor<2>  s1;

  s(0) = 1.;
  s(1) = 2.;
  s(2) = 3.;
  s(3) = 4.;

  s1 = D*s;

  cout << s1 << endl;

  return EXIT_SUCCESS;
}

