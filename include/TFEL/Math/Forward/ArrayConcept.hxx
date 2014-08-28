/*! 
 * \file  include/TFEL/Math/Forward/ArrayConcept.hxx
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

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FORWARD_ARRAYCONCEPT_H */

