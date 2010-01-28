/*!
 * \file   solve.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   01 Aug 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<cassert>
#include<iterator>

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/TinyMatrixSolve.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;

  double a[] = {0.132, 0.60, 0.57,
		0.324, 0.25, 0.99,
		0.26 , 0.78, 0.29};
  double b[] = {0.132, 0.60, 0.,
		0.324, 0.25, 0.,
		0.26 , 0.78, 0.};
  double c[] = {1.0, 2.0, 3.0};

  tmatrix<3,3> m(a);
  tmatrix<3,3> m2(b);
  tvector<3>   v(c);

  try{
    TinyMatrixSolve<3,double>::exe(m,v);
    assert(std::abs(v(0)-10.6118682566723)<=1.e-13);
    assert(std::abs(v(1)-0.93696763202725)<=1.e-13);
    assert(std::abs(v(2)+1.68938103350369)<=1.e-13);
  } 
  catch(LUException&){
    assert(false);
  }   

  try{
    TinyMatrixSolve<3,double>::exe(m2,v);
    assert(false);
  } 
  catch(LUException&){
    assert(true);
  }   

  return EXIT_SUCCESS;
}
