/*!
 * \file   test2.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Oct 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<iostream>
#include<cstdlib>

#include"Utilities/Name.hxx"

#include"Math/tvector.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::utilities;

  tvector<3,tvector<3,short> > v1(tvector<3,short>(1));

  cout << name(2.*v1) << endl;
  cout << name((2.*v1)(0)) << endl;
  cout << (2.*v1)(0)(1) << endl;

  return EXIT_SUCCESS;
}
