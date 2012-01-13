/*! 
 * \file  ST2toST2Concept.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_FORWARD_ST2TOST2CONCEPT_H_
#define _LIB_TFEL_MATH_FORWARD_ST2TOST2CONCEPT_H_ 

namespace tfel
{

  namespace math
  {

    template<class T>
    struct ST2toST2Concept;

    template<typename T_type, typename Expr>
    class ST2toST2Expr;

    template<typename A>
    class ST2toST2NegExpr;

    template<typename A, typename B, class Op>
    class ST2toST2ScalarExpr;

    template<typename A, typename B, class Op>
    class ScalarST2toST2Expr;

    template<typename A, typename B,typename Op>
    struct ST2toST2ST2toST2Expr;

    template<typename A, typename B,typename Op>
    struct ST2toST2ST2toST2ExprWithoutConstIterator;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_ST2TOST2CONCEPT_H */

