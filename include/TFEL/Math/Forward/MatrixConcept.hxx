/*! 
 * \file  MatrixConcept.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_FORWARD_MATRIXCONCEPT_H_
#define _LIB_TFEL_MATH_FORWARD_MATRIXCONCEPT_H_ 

namespace tfel
{

  namespace math
  {

    template<class T>
    struct MatrixConcept;

    template<typename T_type, typename Expr>
    class MatrixExpr;

    template<typename A>
    class MatrixNegExpr;

    template<typename A>
    struct MatrixNegExprWithoutConstIterator;

    template<typename A,typename Func>
    class FctMatrixExpr;

    template<typename A, typename Func>
    struct FctMatrixExprWithoutConstIterator;

    template<typename A, typename B, class Op>
    class MatrixScalarExpr;

    template<typename A, typename B, class Op>
    class MatrixScalarExprWithoutConstIterator;

    template<typename A, typename B, class Op>
    class ScalarMatrixExpr;

    template<typename A, typename B, class Op>
    class MatrixScalarExprWithoutConstIterator;

    template<typename A, typename B, class Op>
    class ScalarMatrixExprWithoutConstIterator;

    template<typename A, typename B, class Op>
    class ScalarMatrixExprWithoutConstIterator;

    template<typename A, typename B,typename Op>
    struct MatrixMatrixExpr;

    template<typename A, typename B,typename Op>
    struct MatrixMatrixExprWithoutConstIterator;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_MATRIXCONCEPT_H */

