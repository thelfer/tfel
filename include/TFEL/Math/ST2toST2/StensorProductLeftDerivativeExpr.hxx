/*! 
 * \file  StensorProductLeftDerivativeExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 04 juin 2014
 */

#ifndef _LIB_TFEL_MATH_STENSORPRODUCTLEFTDERIVATIVEEXPR_H_
#define _LIB_TFEL_MATH_STENSORPRODUCTLEFTDERIVATIVEEXPR_H_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2Expr.hxx"

namespace tfel{

  namespace math{

    /*!
     * Empty structure allowing partial specialisation
     */
    template<unsigned short N>
    struct StensorProductLeftDerivativeExpr
    {}; // end of struct StensorProductLeftDerivativeExpr

    /*!
     * Partial specialisation for 1D tensor
     */
    template<typename ST2toST2Type>
    struct ST2toST2Expr<ST2toST2Type,StensorProductLeftDerivativeExpr<1u> >
      : public ST2toST2Concept<ST2toST2Expr<ST2toST2Type,StensorProductLeftDerivativeExpr<1u> > >,
	public st2tost2_base<ST2toST2Expr<ST2toST2Type,StensorProductLeftDerivativeExpr<1u> > >,
	public fsarray<9u,typename ST2toST2Traits<ST2toST2Type>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2Type>::dime==1u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2Type>::NumType value_type;
      /*!
       * \param[in] B : second tensor of the product
       */
      template<typename StensorType>
      ST2toST2Expr(const StensorType& B)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2Type>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
							     typename ST2toST2Traits<ST2toST2Type>::NumType>::cond));
	const value_type zero = value_type(0);
	this->v[0] = B[0];
	this->v[4] = B[1];
	this->v[8] = B[2];
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
    }; // end of struct ST2toST2Expr<ST2toST2Type,StensorProductLeftDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 2D tensor
     */
    template<typename ST2toST2Type>
    struct ST2toST2Expr<ST2toST2Type,StensorProductLeftDerivativeExpr<2u> >
      : public ST2toST2Concept<ST2toST2Expr<ST2toST2Type,StensorProductLeftDerivativeExpr<2u> > >,
	public st2tost2_base<ST2toST2Expr<ST2toST2Type,StensorProductLeftDerivativeExpr<2u> > >,
	public fsarray<16u,typename ST2toST2Traits<ST2toST2Type>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2Type>::dime==2u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2Type>::NumType value_type;
      /*!
       * \param[in] B : second tensor of the product
       */
      template<typename StensorType>
      ST2toST2Expr(const StensorType& B)
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
	this->v[0]  = B[0];
	this->v[1]  = zero;
	this->v[2]  = zero;
	this->v[3]  = B[3]*one_half;
	this->v[4]  = zero;
	this->v[5]  = B[1];
	this->v[6]  = zero;
	this->v[7]  = B[3]*one_half;
	this->v[8]  = zero;
	this->v[9]  = zero;
	this->v[10] = B[2];
	this->v[11] = zero;
	this->v[12] = B[3];
	this->v[13] = zero;
	this->v[14] = zero;
	this->v[15] = B[1];
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
    }; // end of struct ST2toST2Expr<ST2toST2Type,StensorProductLeftDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 3D tensor
     */
    template<typename ST2toST2Type>
    struct ST2toST2Expr<ST2toST2Type,StensorProductLeftDerivativeExpr<3u> >
      : public ST2toST2Concept<ST2toST2Expr<ST2toST2Type,StensorProductLeftDerivativeExpr<3u> > >,
	public st2tost2_base<ST2toST2Expr<ST2toST2Type,StensorProductLeftDerivativeExpr<3u> > >,
	public fsarray<36u,typename ST2toST2Traits<ST2toST2Type>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2Type>::dime==3u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2Type>::NumType value_type;
      /*!
       * \param[in] B : second tensor of the product
       */
      template<typename StensorType>
      ST2toST2Expr(const StensorType& B)
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
	const real cste  = real(1)/std::sqrt(real(2));
	this->v[0]  = B[0];
	this->v[1]  = this->v[2]  = zero;
	this->v[3]  = B[3]*one_half;
	this->v[4]  = B[4]*one_half;
	this->v[5]  = this->v[6]  = zero;
	this->v[7]  = B[1];
	this->v[8]  = zero;
	this->v[9]  = B[3]*one_half;
	this->v[10] = zero;
	this->v[11] = B[5]*one_half;
	this->v[12] = this->v[13] = zero;
	this->v[14] = B[2];
	this->v[15] = zero;
	this->v[16] = B[4]*one_half;
	this->v[17] = B[5]*one_half;	
	this->v[18] = B[3];
	this->v[19] = this->v[20] = zero;
	this->v[21] = B[1];
	this->v[22] = B[5]*cste;
	this->v[23] = zero;
	this->v[24] = B[4];
	this->v[25] = this->v[26] = zero;
	this->v[27] = B[5]*cste;
	this->v[28] = B[2];
	this->v[29] = zero;
	this->v[30] = this->v[31] = zero;
	this->v[32] = B[5];
	this->v[33] = zero;
	this->v[34] = B[3]*cste;
	this->v[35] = B[1];
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
    }; // end of struct ST2toST2Expr<ST2toST2Type,StensorProductLeftDerivativeExpr<1u> >

  } // end of namespace tfel

} // end of namespace math

#endif /* _LIB_TFEL_MATH_STENSORPRODUCTLEFTDERIVATIVEEXPR_H */

