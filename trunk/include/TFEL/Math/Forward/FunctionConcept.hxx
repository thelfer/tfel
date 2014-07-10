/*! 
 * \file  FunctionConcept.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_FORWARD_FUNCTIONCONCEPT_H_
#define _LIB_TFEL_MATH_FORWARD_FUNCTIONCONCEPT_H_ 

namespace tfel
{

  namespace math
  {

    template<class T>
    struct FunctionConcept;

    template<typename Expr>
    class FunctionExpr;

    template<typename A>
    class FunctionNegExpr;

    template<typename A,typename Func>
    class FctFunctionExpr;

    template<typename A, typename B, class Op>
    class FunctionScalarExpr;

    template<typename A, typename B, class Op>
    class ScalarFunctionExpr;

    template<typename A, typename B,typename Op>
    class FunctionFunctionExpr;

    template<typename Func1,typename Func2>
    class FunctionsCompositionExpr;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_FUNCTIONCONCEPT_H */

