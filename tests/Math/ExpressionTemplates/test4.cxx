/*!
 * \file   test4.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   13 oct 2006
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

  tvector<3,double> v1(1);
  tvector<3,tvector<3,double> > v2(tvector<3,double>(1));

  cout << name(sin(v1)) << endl;
  cout << sin(v1)(0) << endl;
  cout << sin(1.) << endl;

  cout << name(sin(v2)) << endl;
  cout << ((sin(v2))(0))(0) << endl;
  cout << sin(1.) << endl;

}
