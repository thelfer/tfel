/*!
 * \file   test.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Oct 2006
 */

#include<iostream>
#include<cstdlib>

#include"Utilities/Name.hxx"

#include"Math/tvector.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::utilities;

  tvector<3,tvector<2,double> > v1(tvector<2,double>(1.4));
  tvector<3,tvector<2,double> > v2(tvector<2,double>(3.));
  tvector<3,tvector<2,double> > v3(tvector<2,double>(1.2));

  cout << v1(0)(0) << endl;  
  cout << v2(0)(0) << endl;  
  cout << v3(0)(0) << endl;  
  
  cout << name((v1+v2)(0)) << endl;
  cout << ((v1+v2)(0))(1)  << endl;
  cout << (v1(0)+v2(0))(1) << endl;

  cout << name((v1+v2+v3)(0)) << endl;
  cout << ((v1+v2+v3)(0))(0)  << endl;
  cout << (v1(0)+v2(0)+v3(0))(0) << endl;

  return EXIT_SUCCESS;
}
