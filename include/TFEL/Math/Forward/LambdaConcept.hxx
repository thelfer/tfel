/*! 
 * \file  LambdaConcept.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_FORWARD_LAMBDACONCEPT_H_
#define _LIB_TFEL_MATH_FORWARD_LAMBDACONCEPT_H_ 

namespace tfel
{

  namespace math
  {

    template<class T>
    struct LambdaConcept;

    template<typename T_type, typename Expr>
    class LambdaExpr;

    template<typename A,typename Func>
    class FctLambdaExpr;

    template<typename A>
    class LambdaNegExpr;

    template<typename A, typename B, class Op>
    class LambdaScalarExpr;

    template<typename A, typename B, class Op>
    class ScalarLambdaExpr;

    template<typename A, typename B,typename Op>
    class LambdaLambdaExpr;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_LAMBDACONCEPT_H */

