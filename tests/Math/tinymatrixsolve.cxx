/*! 
 * \file  test.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 avr 2009
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>

#include"Math/tvector.hxx"
#include"Math/tmatrix.hxx"
#include"Math/TinyMatrixSolve.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  tmatrix<3,3> m;
  tvector<3> b;

  m(0,0)=0.;
  m(0,1)=2.;
  m(0,2)=1.;
  m(1,0)=1.;
  m(1,1)=0.;
  m(1,2)=0.;
  m(2,0)=3.;
  m(2,1)=0.;
  m(2,2)=1.;

  b(0) = 5;
  b(1) = -1;
  b(2) = -2;

//   cout << "********" << endl;
//   cout << m << endl;
  TinyMatrixSolve<3,double>::exe(m,b);
//   cout << m << endl;
//   cout << "b : " << b << endl;
//   cout << endl;
} // end of main
