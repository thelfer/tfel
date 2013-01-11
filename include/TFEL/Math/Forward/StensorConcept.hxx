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
    class StensorConcept;

    template<typename T_type, typename Expr>
    struct StensorExpr;

    template<typename A>
    class StensorNegExpr;

    template<typename A>
    class StensorNegExprWithoutConstIterator;

    template<typename A, typename B, class Op>
    class StensorScalarExpr;

    template<typename A, typename B, class Op>
    class StensorScalarExprWithoutConstIterator;

    template<typename A, typename B, class Op>
    class ScalarStensorExpr;

    template<typename A, typename B, class Op>
    class ScalarStensorExprWithoutConstIterator;

    template<typename A, typename B,typename Op>
    class StensorStensorExpr;

    template<typename A, typename B,typename Op>
    class StensorStensorExprWithoutConstIterator;

    template<typename A,
	     typename B>
    class StensorStensorDiadicProductExpr;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_STENSORCONCEPT_H */

