/*!
 * \file   test7.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   16 Oct 2006
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

  tvector<3,double> v1(1.);
  tvector<3,double> v2(1.5);
  tvector<3,double> v3(1.76);

  std::cout << (v1+v2+v3)(0) << std::endl;

  return EXIT_SUCCESS;
}
