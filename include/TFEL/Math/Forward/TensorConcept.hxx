/*! 
 * \file  include/TFEL/Math/Forward/TensorConcept.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  3 dec. 2013
 */

#ifndef _LIB_TFEL_MATH_FORWARD_TENSORCONCEPT_H_
#define _LIB_TFEL_MATH_FORWARD_TENSORCONCEPT_H_ 

namespace tfel
{

  namespace math
  {

    template<class T>
    class TensorConcept;

    template<typename T_type, typename Expr>
    struct TensorExpr;

    template<typename T_type>
    struct TensorTraits;

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_TENSORCONCEPT_H */

