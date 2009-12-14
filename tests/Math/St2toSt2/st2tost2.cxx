/*!
 * \file   st2tost2.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   30 jan 2007
 */

#include<iostream>
#include<cstdlib>

#include"Math/st2tost2.hxx"

int main(void)
{
  using namespace tfel::math;
  using namespace std;

  st2tost2<3> s1(1.5);
  st2tost2<3> s2(4.);

  st2tost2<3> s3;
  s3 = s1+0.5*s2;

  cout << s1 << endl;
  cout << s2 << endl;
  cout << s3 << endl;

  return EXIT_SUCCESS;
}
