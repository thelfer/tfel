/*!
 * \file   tests/Math/ExpressionTemplates/test_expr1.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Oct 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cassert>
#include<cstdlib>

#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Math/tvector.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::utilities;

  tvector<3,tvector<3,double> > v1(tvector<3,double>(1.4));
  tvector<3,tvector<3,double> > v2(tvector<3,double>(3.));
  tvector<3,tvector<3,double> > v3(tvector<3,double>(1.2));

  assert(abs(v1(0)(0)-1.4)<1.e-14);  
  assert(abs(v2(0)(0)-3.)<1.e-14);  
  assert(abs(v3(0)(0)-1.2)<1.e-14);  
  
  assert(name((v1+v2)(0))==
	 "VectorExpr<tvector<3,double>,MathObjectMathObjectExpr<tvector<3,double>,tvector<3,double>,+>>");
  assert(abs(((v1+v2)(0))(1)-4.4)<1.e-14);
  assert(abs((v1(0)+v2(0))(1)-4.4)<1.e-14);

  assert(name((v1+v2+v3)(0))==
	 "VectorExpr<tvector<3,double>,MathObjectMathObjectExpr<VectorExpr<tvector<3,double>,MathObjectMathObjectExpr<tvector<3,double>,tvector<3,double>,+>>,tvector<3,double>,+>>");
  assert(abs(((v1+v2+v3)(0))(1)-5.6)<1.e-14);
  assert(abs((v1(0)+v2(0)+v3(0))(1)-5.6)<1.e-14);

  return EXIT_SUCCESS;
}
