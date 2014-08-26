/*! 
 * \file  TensorProductLeftDerivativeExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 04 juin 2014
 */

#ifndef _LIB_TFEL_MATH_TENSORPRODUCTLEFTDERIVATIVEEXPR_H_
#define _LIB_TFEL_MATH_TENSORPRODUCTLEFTDERIVATIVEEXPR_H_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Math/T2toT2/T2toT2Expr.hxx"

namespace tfel{

  namespace math{

    /*!
     * Empty structure allowing partial specialisation
     */
    template<unsigned short N>
    struct TensorProductLeftDerivativeExpr
    {}; // end of struct TensorProductLeftDerivativeExpr

    /*!
     * Partial specialisation for 1D tensor
     */
    template<typename T2toT2Type>
    struct T2toT2Expr<T2toT2Type,TensorProductLeftDerivativeExpr<1u> >
      : public T2toT2Concept<T2toT2Expr<T2toT2Type,TensorProductLeftDerivativeExpr<1u> > >,
	public t2tot2_base<T2toT2Expr<T2toT2Type,TensorProductLeftDerivativeExpr<1u> > >,
	public fsarray<9u,typename T2toT2Traits<T2toT2Type>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toT2Traits<T2toT2Type>::dime==1u);
      //! a simple alias
      typedef typename T2toT2Traits<T2toT2Type>::NumType value_type;
      /*!
       * \param[in] B : second tensor of the product
       */
      template<typename TensorType>
      T2toT2Expr(const TensorType& B)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2Type>::dime==TensorTraits<TensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,
							     typename T2toT2Traits<T2toT2Type>::NumType>::cond));
	this->v[0] = B[0];
	this->v[4] = B[1];
	this->v[8] = B[2];
	this->v[1] = this->v[2] = value_type(0);
	this->v[3] = this->v[5] = value_type(0);
	this->v[6] = this->v[7] = value_type(0);
      } // end of T2toT2Expr
    }; // end of struct T2toT2Expr<T2toT2Type,TensorProductLeftDerivativeExpr<1u> >

  } // end of namespace tfel

} // end of namespace math

#endif /* _LIB_TFEL_MATH_TENSORPRODUCTLEFTDERIVATIVEEXPR_H */

