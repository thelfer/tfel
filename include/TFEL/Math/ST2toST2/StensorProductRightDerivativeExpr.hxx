/*! 
 * \file  StensorProductRightDerivativeExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 04 juin 2014
 */

#ifndef _LIB_TFEL_MATH_STENSORPRODUCTRIGHTDERIVATIVEEXPR_H_
#define _LIB_TFEL_MATH_STENSORPRODUCTRIGHTDERIVATIVEEXPR_H_ 

#include<iostream>

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2Expr.hxx"

namespace tfel{

  namespace math{

    /*!
     * Empty structure allowing partial specialisation
     */
    template<unsigned short N>
    struct StensorProductRightDerivativeExpr
    {}; // end of struct StensorProductRightDerivativeExpr

    /*!
     * Partial specialisation for 1D stensor
     */
    template<typename ST2toST2Type>
    struct ST2toST2Expr<ST2toST2Type,StensorProductRightDerivativeExpr<1u> >
      : public ST2toST2Concept<ST2toST2Expr<ST2toST2Type,StensorProductRightDerivativeExpr<1u> > >,
	public st2tost2_base<ST2toST2Expr<ST2toST2Type,StensorProductRightDerivativeExpr<1u> > >,
	public fsarray<9u,typename ST2toST2Traits<ST2toST2Type>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2Type>::dime==1u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2Type>::NumType value_type;
      /*!
       * \param[in] A : second stensor of the product
       */
      template<typename StensorType>
      ST2toST2Expr(const StensorType& A)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2Type>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
							     typename ST2toST2Traits<ST2toST2Type>::NumType>::cond));
	const value_type zero = value_type(0);
	this->v[0] = A[0];
	this->v[4] = A[1];
	this->v[8] = A[2];
	this->v[1] = this->v[2] = zero;
	this->v[3] = this->v[5] = zero;
	this->v[6] = this->v[7] = zero;
      } // end of ST2toST2Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      const value_type&
      operator()(const unsigned short i,
		 const unsigned short j) const
      {
	return this->v[i*3+j];
      } // end of operator()
    }; // end of struct ST2toST2Expr<ST2toST2Type,StensorProductRightDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 2D stensor
     */
    template<typename ST2toST2Type>
    struct ST2toST2Expr<ST2toST2Type,StensorProductRightDerivativeExpr<2u> >
      : public ST2toST2Concept<ST2toST2Expr<ST2toST2Type,StensorProductRightDerivativeExpr<2u> > >,
	public st2tost2_base<ST2toST2Expr<ST2toST2Type,StensorProductRightDerivativeExpr<2u> > >,
	public fsarray<16u,typename ST2toST2Traits<ST2toST2Type>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2Type>::dime==2u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2Type>::NumType value_type;
      /*!
       * \param[in] A : second stensor of the product
       */
      template<typename StensorType>
      ST2toST2Expr(const StensorType& A)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2Type>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
							     typename ST2toST2Traits<ST2toST2Type>::NumType>::cond));
	using tfel::typetraits::BaseType;
	typedef typename BaseType<value_type>::type real;
	const value_type zero = value_type(0);
	const real one_half   = real(1)/2;
	this->v[0]  = A[0];
	this->v[1]  = zero;
	this->v[2]  = zero;
	this->v[3]  = A[3]*one_half;
	this->v[4]  = zero;
	this->v[5]  = A[1];
	this->v[6]  = zero;
	this->v[7]  = A[3]*one_half;
	this->v[8]  = zero;
	this->v[9]  = zero;
	this->v[10] = A[2];
	this->v[11] = zero;
	this->v[12] = zero;
	this->v[13] = A[3];
	this->v[14] = zero;
	this->v[15] = A[0];
      } // end of ST2toST2Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      const value_type&
      operator()(const unsigned short i,
		 const unsigned short j) const
      {
	return this->v[i*4+j];
      } // end of operator()
    }; // end of struct ST2toST2Expr<ST2toST2Type,StensorProductRightDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 3D stensor
     */
    template<typename ST2toST2Type>
    struct ST2toST2Expr<ST2toST2Type,StensorProductRightDerivativeExpr<3u> >
      : public ST2toST2Concept<ST2toST2Expr<ST2toST2Type,StensorProductRightDerivativeExpr<3u> > >,
	public st2tost2_base<ST2toST2Expr<ST2toST2Type,StensorProductRightDerivativeExpr<3u> > >,
	public fsarray<36u,typename ST2toST2Traits<ST2toST2Type>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2Type>::dime==3u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2Type>::NumType value_type;
      /*!
       * \param[in] A : second stensor of the product
       */
      template<typename StensorType>
      ST2toST2Expr(const StensorType& A)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2Type>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
							     typename ST2toST2Traits<ST2toST2Type>::NumType>::cond));
	using tfel::typetraits::BaseType;
	typedef typename BaseType<value_type>::type real;
	const value_type zero = value_type(0);
	const real one_half   = real(1)/2;
	const real cste       = real(1)/std::sqrt(real(2));
	this->v[0]  = A[0];
	this->v[1]  = this->v[2]  = zero;
	this->v[3]  = A[3]*one_half;
	this->v[4]  = A[4]*one_half;
	this->v[5]  = this->v[6]  = zero;
	this->v[7]  = A[1];
	this->v[8]  = zero;
	this->v[9]  = A[3]*one_half;
	this->v[10] = zero;
	this->v[11] = A[5]*one_half;
	this->v[12] = this->v[13] = zero;
	this->v[14] = A[2];
	this->v[15] = zero;
	this->v[16] = A[4]*one_half;
	this->v[17] = A[5]*one_half;
	this->v[18] = zero;
	this->v[19] = A[3];
	this->v[20] = zero;
	this->v[21] = A[0];
	this->v[22] = zero;
	this->v[23] = A[4]*cste;
	this->v[24] = this->v[25] =zero;
	this->v[26] = A[4];
	this->v[27] = zero;
	this->v[28] = A[0];
	this->v[29] = A[3]*cste;
	this->v[30] = zero;
	this->v[31] = A[5];
	this->v[32] = zero;
	this->v[33] = A[4]*cste;
	this->v[34] = zero;
	this->v[35] = A[2];
      } // end of ST2toST2Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      const value_type&
      operator()(const unsigned short i,
		 const unsigned short j) const
      {
	return this->v[i*6+j];
      } // end of operator()
    }; // end of struct ST2toST2Expr<ST2toST2Type,StensorProductRightDerivativeExpr<1u> >

  } // end of namespace tfel

} // end of namespace math

#endif /* _LIB_TFEL_MATH_STENSORPRODUCTRIGHTDERIVATIVEEXPR_H */

