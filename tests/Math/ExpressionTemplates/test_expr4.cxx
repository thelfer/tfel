/*!
 * \file   tests/Math/ExpressionTemplates/test_expr4.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   13 oct 2006
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

#include<cmath>
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

  tvector<3,double> v1(1);
  tvector<3,tvector<3,double> > v2(tvector<3,double>(1));

  assert(name(sin(v1))=="VectorExpr<tvector<3,double>,FctMathObjectExpr<tvector<3,double>,FctSin>>");
  assert(abs(sin(v1)(0)-sin(1.))<1.e-14);
  assert(abs(sin(v1)(1)-sin(1.))<1.e-14);
  assert(abs(sin(v1)(2)-sin(1.))<1.e-14);

  assert(name(sin(v2))=="VectorExpr<tvector<3,tvector<3,double>>,FctMathObjectExpr<tvector<3,tvector<3,double>>,FctSin>>");
  assert(abs((sin(v2))(0)(0)-sin(1.))<1.e-14);
  assert(abs((sin(v2))(0)(1)-sin(1.))<1.e-14);
  assert(abs((sin(v2))(0)(2)-sin(1.))<1.e-14);
  assert(abs((sin(v2))(1)(0)-sin(1.))<1.e-14);
  assert(abs((sin(v2))(1)(1)-sin(1.))<1.e-14);
  assert(abs((sin(v2))(1)(2)-sin(1.))<1.e-14);
  assert(abs((sin(v2))(2)(0)-sin(1.))<1.e-14);
  assert(abs((sin(v2))(2)(1)-sin(1.))<1.e-14);
  assert(abs((sin(v2))(2)(2)-sin(1.))<1.e-14);

  return EXIT_SUCCESS;
}
