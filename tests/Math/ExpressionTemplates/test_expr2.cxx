/*!
 * \file   tests/Math/ExpressionTemplates/test_expr2.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Oct 2006
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
