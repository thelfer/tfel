/*! 
 * \file  include/TFEL/Math/Forward/Function.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_FORWARD_FUNCTION_H_
#define _LIB_TFEL_MATH_FORWARD_FUNCTION_H_ 

namespace tfel{

  namespace math{

    template<typename Func>
    struct FunctionConcept;

    template<typename Expr>
    class FunctionExpr;

    template<short N,unsigned short D>
    struct Cst;

    template<typename Func1,typename Func2, typename Op>
    class FunctionFunctionExpr;

    template<typename Func1,typename Func2, typename Op>
    class FunctionFunctionExpr;

    template<typename F>
    class FunctionNegExpr;

    template<typename Func,typename Scal, typename Op>
    class FunctionScalarExpr;

    template<typename Func1,typename Func2>
    class FunctionsCompositionExpr;

    template<typename Scal,typename Func, typename Op>
    class ScalarFunctionExpr;

  } // end of namespace typetraits
    
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_FUNCTION_H */

