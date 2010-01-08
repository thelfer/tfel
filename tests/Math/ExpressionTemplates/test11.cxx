/*!
 * \file   test11.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   14 oct 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<iostream>
#include<cstdlib>

#include"Utilities/Name.hxx"
#include"Math/functions.hxx"
#include"Math/tvector.hxx"

int main(void)
{
  using namespace tfel::math;
  using namespace tfel::utilities;
  using std::cout;
  using std::endl;
  USING_TFEL_FUNCTIONS;

  tvector<3,double> v1(1);
  tvector<3,tvector<3,double> > v2(tvector<3,double>(1));

  cout << name(v1+sin(v1)) << endl;
  cout << (v1+sin(v1))(0) << endl;
  cout << sin(1.) << endl;

  cout << "v2(0) : " << &v2(0) << endl;

  cout << name(sin(v2)) << endl;
  cout << sin(v2)(0)(0) << endl;
  cout << sin(1.) << endl;

  cout << "*****************************************************" << &v2(0) << endl;

  cout << "v2(0) : " << &v2(0) << endl;

  cout << name(v2+sin(v2)) << endl;
  cout << (v2+sin(v2))(0)(0) << endl;
  cout << 1.+sin(1.) << endl;

  cout << name(sin(v2)+v2) << endl;
  cout << (v2+sin(v2))(0)(0) << endl;
  cout << 1.+sin(1.) << endl;

  return EXIT_SUCCESS;
}
