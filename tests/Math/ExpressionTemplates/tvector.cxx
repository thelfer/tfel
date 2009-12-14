/*!
 * \file   tvector.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   16 Oct 2006
 */

#include"tvector-header.hxx"

void test2(void)
{
  using namespace std;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;
  cout << "test2()" << endl;

  tvector<2,tvector<3,unsigned short> >    v;
  tvector<2,tvector<3,float> >  v2;
  tvector<2,tvector<3,double> > v3;

  v(0)(0) = 2;
  v(0)(1) = 7;
  v(0)(2) = 9;
  v(1)(0) = 2;
  v(1)(1) = 7;
  v(1)(2) = 9;

  v2(0)(0) = 2.34f;
  v2(0)(1) = 54.2f;
  v2(0)(2) = 412.232f;
  v2(1)(0) = 2.34f;
  v2(1)(1) = 54.2f;
  v2(1)(2) = 412.232f;

  v3 = 2.*v+v2;

  cout << v3(0) << endl;
  cout << v3(1) << endl;
   
  //  v3 = exp(sin(2.f*v+3.5f*cos(v2))); 

//   cout << v3(0) << endl;
//   cout << v3(1) << endl;
}

void test3(void)
{
  using namespace std;
  using namespace tfel::math;

  cout << "test3()" << endl;

  tvector<2,tvector<3,float> >    v;
  tvector<2,tvector<3,float> >  v2;
  tvector<2,tvector<3,double> > v3;
  tvector<2,tvector<3,double> > v4;

  v(0)(0) = 2.f;
  v(0)(1) = 7.f;
  v(0)(2) = 9.f;
  v(1)(0) = 2.f;
  v(1)(1) = 7.f;
  v(1)(2) = 9.f;

  v2(0)(0) = 2.34f;
  v2(0)(1) = 54.2f;
  v2(0)(2) = 412.232f;
  v2(1)(0) = 2.34f;
  v2(1)(1) = 54.2f;
  v2(1)(2) = 412.232f;

  v3(0)(0) = 2.34f;
  v3(0)(1) = 54.2f;
  v3(0)(2) = 412.232f;
  v3(1)(0) = 2.34f;
  v3(1)(1) = 54.2f;
  v3(1)(2) = 412.232f;

  v4 = v+v2+v3;
  cout << v4(0) << endl;
  cout << v4(1) << endl;

}

int main(void)
{
  test1<float>();
  test1<double>();
  test1<long double>();

  test2();

  test3();

  return EXIT_SUCCESS;
}
