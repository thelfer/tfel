/*! 
 * \file  StensorConcept.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_FORWARD_STENSORCONCEPT_H_
#define _LIB_TFEL_MATH_FORWARD_STENSORCONCEPT_H_ 

namespace tfel
{

  namespace math
  {

    template<class T>
    struct StensorConcept;

    template<typename T_type, typename Expr>
    class StensorExpr;

    template<typename A>
    class StensorNegExpr;

    template<typename A>
    struct StensorNegExprWithoutConstIterator;

    template<typename A, typename B, class Op>
    class StensorScalarExpr;

    template<typename A, typename B, class Op>
    class StensorScalarExprWithoutConstIterator;

    template<typename A, typename B, class Op>
    class ScalarStensorExpr;

    template<typename A, typename B, class Op>
    class ScalarStensorExprWithoutConstIterator;

    template<typename A, typename B,typename Op>
    struct StensorStensorExpr;

    template<typename A, typename B,typename Op>
    struct StensorStensorExprWithoutConstIterator;

    template<typename A,
	     typename B>
    struct StensorStensorDiadicProductExpr;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_STENSORCONCEPT_H */

