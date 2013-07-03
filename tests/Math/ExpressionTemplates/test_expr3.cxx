/*!
 * \file   test3.cxx
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
