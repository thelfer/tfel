/*!
 * \file   VectorResultType.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   09 Oct 2006
 */

#include<iostream>
#include<cstdlib>

#include"Utilities/Name.hxx"
#include"Math/vector.hxx"
#include"Math/tvector.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::utilities;
  using tfel::math::vector;

  vector<tvector<3> > v(3,tvector<3>(4.));
  vector<tvector<3> > v2(3,tvector<3>(2.));

  cout << name((v+v2)(2)) << endl;

  return EXIT_SUCCESS;
}
