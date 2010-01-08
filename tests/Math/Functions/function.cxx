/*!
 * \file   function.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   04 Jan 2007
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cassert>
#include<cstdlib>

#include "Math/functions.hxx"
#include "Utilities/Name.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::utilities;

  USING_TFEL_FUNCTIONS;
  
  assert(name(sin+cos)=="FunctionExpr<FunctionFunctionExpr<FctSin,FctCos,+>>");
  assert(name(exp[sin])==
	 "FunctionExpr<FunctionsCompositionExpr<FctExp,FctSin>>");
  assert(name(exp[sin+cos])==
	 "FunctionExpr<FunctionsCompositionExpr<FctExp,FunctionExpr<FunctionFunctionExpr<FctSin,FctCos,+>>>>");
  // Comparison with Octave results
  assert(abs((sin+cos)(3.4312)+ 1.2439)<1.e-4);
  assert(abs((-(sin+cos))(3.4312)-1.2439)<1.e-4);
  assert(abs((exp[sin])(3.4312)-0.75158)<1.e-4);
  assert(abs((exp[sin+cos])(3.4312)-0.28825)<1.e-4);
  return EXIT_SUCCESS;

}
