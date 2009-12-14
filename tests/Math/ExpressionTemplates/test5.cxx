/*!
 * \file   test5.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 oct 2006
 */

#include<iostream>
#include<cstdlib>

#include"Utilities/Name.hxx"
#include"Math/functions.hxx"
#include"Math/tvector.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::utilities;
  USING_TFEL_FUNCTIONS;


  tvector<3,float> v1(1);
  tvector<3,float> v2(2);
  tvector<3,tvector<3,float> > v3(tvector<3,float>(1));
  tvector<3,tvector<3,float> > v4(tvector<3,float>(1));

  cout << name(2.*v1) << endl;
  cout << (2.*v1)(2) << endl;
  cout << name(2.*(v1+v2)) << endl;
  cout << (2.*(v1+v2))(2) << endl;
  cout << name(v1+2.*v2) << endl;
  cout << (v1+2.*v2)(2) << endl;
  cout << name(v1+v2*2.) << endl;
  cout << (v1+v2*2.)(2) << endl;
  cout << name(v2*2.+v1) << endl;
  cout << (v2*2.+v1)(2) << endl;
  cout << name(v2*2.+sin(v1)) << endl;
  cout << (v2*2.+sin(v1))(2) << endl;

  cout << name(2.*v3) << endl;
  cout << (2.*v3)(2)(0) << endl;
  cout << name(2.*(v3+v4)) << endl;
  cout << (2.*(v3+v4))(2)(0) << endl;
  cout << name(v3+2.*v4) << endl;
  cout << (v3+2.*v4)(2)(0) << endl;
  cout << name(v3+v4*2.) << endl;
  cout << (v3+v4*2.)(2)(0) << endl;
  cout << name(v4*2.+v3) << endl;
  cout << (v4*2.+v3)(2)(0) << endl;
  cout << name(v4*2.+sin(v3)) << endl;
  cout << (v4*2.+sin(v3))(2)(0) << endl;

}
