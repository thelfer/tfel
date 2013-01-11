/*! 
 * \file  ArrayConcept.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_FORWARD_ARRAYCONCEPT_H_
#define _LIB_TFEL_MATH_FORWARD_ARRAYCONCEPT_H_ 

namespace tfel
{

  namespace math
  {

    template<typename T,
	     typename Ordering,
	     typename TAllocator>
    class ArrayStorage_;

    template<unsigned short N,
	     typename T,
	     typename Ordering,
	     typename TAllocator>
    struct ArrayStorage;

    template<class T>
    struct ArrayConcept;

    template<typename T_type, typename Expr>
    class ArrayExpr;

    template<typename A>
    class ArrayNegExpr;

    template<typename A>
    class ArrayNegExprWithoutConstIterator;

    template<typename A,typename Func>
    class FctArrayExpr;

    template<typename A, typename Func>
    class FctArrayExprWithoutConstIterator;

    template<typename A, typename B, class Op>
    class ArrayScalarExpr;

    template<typename A, typename B, class Op>
    class ArrayScalarExprWithoutConstIterator;

    template<typename A, typename B, class Op>
    class ScalarArrayExpr;

    template<typename A, typename B, class Op>
    class ArrayScalarExprWithoutConstIterator;

    template<typename A, typename B, class Op>
    class ScalarArrayExprWithoutConstIterator;

    template<typename A, typename B, class Op>
    class ScalarArrayExprWithoutConstIterator;

    template<typename A, typename B,typename Op>
    class ArrayArrayExpr;

    template<typename A, typename B,typename Op>
    class ArrayArrayExprWithoutConstIterator;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_ARRAYCONCEPT_H */

