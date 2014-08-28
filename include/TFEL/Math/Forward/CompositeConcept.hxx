/*! 
 * \file  include/TFEL/Math/Forward/CompositeConcept.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_FORWARD_COMPOSITECONCEPT_H_
#define _LIB_TFEL_MATH_FORWARD_COMPOSITECONCEPT_H_ 

namespace tfel
{

  namespace math
  {

    template<class T>
    struct CompositeConcept;

    template<typename T_type, typename Expr>
    struct CompositeExpr;

    template<typename A,typename Func>
    class FctCompositeExpr;

    template<typename A>
    class CompositeNegExpr;

    template<typename A, typename B, class Op>
    class CompositeScalarExpr;

    template<typename A, typename B, class Op>
    class ScalarCompositeExpr;

    template<typename A, typename B,typename Op>
    class CompositeCompositeExpr;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_COMPOSITECONCEPT_H */

