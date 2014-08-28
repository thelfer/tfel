/*!
 * \file   tests/Math/ExpressionTemplates/test_expr2.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Oct 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cassert>
#include<cstdlib>

#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Math/tvector.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::utilities;

  tvector<3,tvector<3,short> > v1(tvector<3,short>(1));

  assert(name(2.*v1)=="VectorExpr<tvector<3,tvector<3,double>>,ScalarMathObjectExpr<double,tvector<3,tvector<3,short>>,*>>");
  assert(abs((2.*v1)(0)(0)-2.)<1.e-14);
  assert(abs((2.*v1)(0)(1)-2.)<1.e-14);
  assert(abs((2.*v1)(0)(2)-2.)<1.e-14);
  assert(abs((2.*v1)(1)(0)-2.)<1.e-14);
  assert(abs((2.*v1)(1)(1)-2.)<1.e-14);
  assert(abs((2.*v1)(1)(2)-2.)<1.e-14);

  return EXIT_SUCCESS;
}
