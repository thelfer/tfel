/*!
 * \file   tests/Math/ExpressionTemplates/test_expr5.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   14 oct 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cassert>
#include<cstdlib>

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Math/functions.hxx"
#include"TFEL/Math/tvector.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::utilities;
  USING_TFEL_FUNCTIONS;


  tvector<3,float> v1(1);
  tvector<3,float> v2(2);
  tvector<3,tvector<3,float> > v3(tvector<3,float>(1));
  tvector<3,tvector<3,float> > v4(tvector<3,float>(1));

  assert(name(2.*v1)=="VectorExpr<tvector<3,double>,ScalarMathObjectExpr<double,tvector<3,float>,*>>");
  assert(abs((2.*v1)(0)-2.)<1.e-14);
  assert(abs((2.*v1)(1)-2.)<1.e-14);
  assert(abs((2.*v1)(2)-2.)<1.e-14);

  assert(name(2.*(v1+v2))=="VectorExpr<tvector<3,double>,ScalarMathObjectExpr<double,VectorExpr<tvector<3,float>,MathObjectMathObjectExpr<tvector<3,float>,tvector<3,float>,+>>,*>>");
  assert(abs((2.*(v1+v2))(0)-6.)<1.e-14);
  assert(abs((2.*(v1+v2))(1)-6.)<1.e-14);
  assert(abs((2.*(v1+v2))(2)-6.)<1.e-14);
  
  assert(name(v1+2.*v2)=="VectorExpr<tvector<3,double>,MathObjectMathObjectExpr<tvector<3,float>,VectorExpr<tvector<3,double>,ScalarMathObjectExpr<double,tvector<3,float>,*>>,+>>");
  assert(abs((v1+2.*v2)(0)-5.)<1.e-14);
  assert(abs((v1+2.*v2)(1)-5.)<1.e-14);
  assert(abs((v1+2.*v2)(2)-5.)<1.e-14);

  assert(name(v1+v2*2.)=="VectorExpr<tvector<3,double>,MathObjectMathObjectExpr<tvector<3,float>,VectorExpr<tvector<3,double>,MathObjectScalarExpr<tvector<3,float>,double,*>>,+>>");
  assert(abs((v1+v2*2.)(0)-5.)<1.e-14);
  assert(abs((v1+v2*2.)(1)-5.)<1.e-14);
  assert(abs((v1+v2*2.)(2)-5.)<1.e-14);

  return EXIT_SUCCESS;
}
