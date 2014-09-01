/*!
 * \file   tests/Math/ExpressionTemplates/test_expr3.cxx
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

#include"TFEL/Math/General/UnaryResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/tvector.hxx"

int main(void){
  
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::math;

  tvector<3,unsigned short> v1(1);
  tvector<3,tvector<3,unsigned short> > v2(tvector<3,unsigned short>(1));

  assert((Name<ComputeUnaryResult<tvector<3,unsigned short>,OpNeg>::Result>::getName()=="tvector<3,int>"));
  assert((Name<ComputeUnaryResult<tvector<3,unsigned short>,OpNeg>::Handle>::getName()=="VectorExpr<tvector<3,int>,MathObjectNegExpr<tvector<3,unsigned short>>>"));
  assert(name(-v1)=="VectorExpr<tvector<3,int>,MathObjectNegExpr<tvector<3,unsigned short>>>");
  assert(name(-v2)=="VectorExpr<tvector<3,tvector<3,int>>,MathObjectNegExpr<tvector<3,tvector<3,unsigned short>>>>");
  
  return EXIT_SUCCESS;
}
