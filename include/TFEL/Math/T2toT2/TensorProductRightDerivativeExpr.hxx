/*! 
 * \file  TensorProductRightDerivativeExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 04 juin 2014
 */

#ifndef _LIB_TFEL_MATH_TENSORPRODUCTRIGHTDERIVATIVEEXPR_H_
#define _LIB_TFEL_MATH_TENSORPRODUCTRIGHTDERIVATIVEEXPR_H_ 

#include<iostream>

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Math/T2toT2/T2toT2Expr.hxx"

namespace tfel{

  namespace math{

    /*!
     * Empty structure allowing partial specialisation
     */
    template<unsigned short N>
    struct TensorProductRightDerivativeExpr
    {}; // end of struct TensorProductRightDerivativeExpr

    /*!
     * Partial specialisation for 1D tensor
     */
    template<typename T2toT2Type>
    struct T2toT2Expr<T2toT2Type,TensorProductRightDerivativeExpr<1u> >
      : public T2toT2Concept<T2toT2Expr<T2toT2Type,TensorProductRightDerivativeExpr<1u> > >,
	public t2tot2_base<T2toT2Expr<T2toT2Type,TensorProductRightDerivativeExpr<1u> > >,
	public fsarray<9u,typename T2toT2Traits<T2toT2Type>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toT2Traits<T2toT2Type>::dime==1u);
      //! a simple alias
      typedef typename T2toT2Traits<T2toT2Type>::NumType value_type;
      /*!
       * \param[in] A : second tensor of the product
       */
      template<typename TensorType>
      T2toT2Expr(const TensorType& A)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2Type>::dime==TensorTraits<TensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,
							     typename T2toT2Traits<T2toT2Type>::NumType>::cond));
	this->v[0] = A[0];
	this->v[4] = A[1];
	this->v[8] = A[2];
	this->v[1] = this->v[2] = value_type(0);
	this->v[3] = this->v[5] = value_type(0);
	this->v[6] = this->v[7] = value_type(0);
      } // end of T2toT2Expr
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
    }; // end of struct T2toT2Expr<T2toT2Type,TensorProductRightDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 2D tensor
     */
    template<typename T2toT2Type>
    struct T2toT2Expr<T2toT2Type,TensorProductRightDerivativeExpr<2u> >
      : public T2toT2Concept<T2toT2Expr<T2toT2Type,TensorProductRightDerivativeExpr<2u> > >,
	public t2tot2_base<T2toT2Expr<T2toT2Type,TensorProductRightDerivativeExpr<2u> > >,
	public fsarray<25u,typename T2toT2Traits<T2toT2Type>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toT2Traits<T2toT2Type>::dime==2u);
      //! a simple alias
      typedef typename T2toT2Traits<T2toT2Type>::NumType value_type;
      /*!
       * \param[in] A : second tensor of the product
       */
      template<typename TensorType>
      T2toT2Expr(const TensorType& A)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2Type>::dime==TensorTraits<TensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,
							     typename T2toT2Traits<T2toT2Type>::NumType>::cond));
	this->v[0]  = A[0]; this->v[4]  = A[3];
	this->v[1]  = this->v[2]  = this->v[3]  = value_type(0);
	this->v[6]  = A[1]; this->v[8]  = A[4];
	this->v[5]  = this->v[7]  = this->v[9]  = value_type(0);
	this->v[12] = A[2];
	this->v[10] = this->v[11] = this->v[13] = this->v[14] = value_type(0);
	this->v[16] = A[3]; this->v[18]  = A[0];
	this->v[15] = this->v[17] = this->v[19]  = value_type(0);
	this->v[20] = A[4]; this->v[24]  = A[1];
	this->v[21] = this->v[22] = this->v[23]  = value_type(0);
      } // end of T2toT2Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      const value_type&
      operator()(const unsigned short i,
		 const unsigned short j) const
      {
	return this->v[i*5+j];
      } // end of operator()
    }; // end of struct T2toT2Expr<T2toT2Type,TensorProductRightDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 3D tensor
     */
    template<typename T2toT2Type>
    struct T2toT2Expr<T2toT2Type,TensorProductRightDerivativeExpr<3u> >
      : public T2toT2Concept<T2toT2Expr<T2toT2Type,TensorProductRightDerivativeExpr<3u> > >,
	public t2tot2_base<T2toT2Expr<T2toT2Type,TensorProductRightDerivativeExpr<3u> > >,
	public fsarray<81u,typename T2toT2Traits<T2toT2Type>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(T2toT2Traits<T2toT2Type>::dime==3u);
      //! a simple alias
      typedef typename T2toT2Traits<T2toT2Type>::NumType value_type;
      /*!
       * \param[in] A : second tensor of the product
       */
      template<typename TensorType>
      T2toT2Expr(const TensorType& A)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2Type>::dime==TensorTraits<TensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,
							     typename T2toT2Traits<T2toT2Type>::NumType>::cond));
	this->v[0]  = A[0]; this->v[4]  = A[3]; this->v[6] = A[5];
	this->v[1]  = this->v[2] = this->v[3] = this->v[5] = this->v[7] = this->v[8] =value_type(0);
	this->v[10] = A[1]; this->v[12] = A[4]; this->v[17] = A[7];
	this->v[9]  = this->v[11] = this->v[13] = this->v[14] = this->v[15] = this->v[16] = value_type(0);
	this->v[20] = A[2]; this->v[23] = A[6]; this->v[25]=A[8];
	this->v[18] = this->v[19] = this->v[21] = this->v[22] = this->v[24] = this->v[26] = value_type(0);
	this->v[28] = A[3]; this->v[30] = A[0]; this->v[35]=A[5];
	this->v[27] = this->v[29] = this->v[31] = this->v[32] = this->v[33] = this->v[34] = value_type(0);
	this->v[36] = A[4]; this->v[40] = A[1]; this->v[42]=A[7];
	this->v[37] = this->v[38] = this->v[39] = this->v[41] = this->v[43] = this->v[44] = value_type(0);
	this->v[47] = A[5]; this->v[50] = A[0]; this->v[52]=A[3];
	this->v[45] = this->v[46] = this->v[48] = this->v[49] = this->v[51] = this->v[53] = value_type(0);
	this->v[54] = A[6]; this->v[58] = A[8]; this->v[60]=A[2];
	this->v[55] = this->v[56] = this->v[57] = this->v[59] = this->v[61] = this->v[62] = value_type(0);
	this->v[65] = A[7]; this->v[68] = A[4]; this->v[70]=A[1];
	this->v[63] = this->v[64] = this->v[66] = this->v[67] = this->v[69] = this->v[71] = value_type(0);
	this->v[73] = A[8]; this->v[75] = A[6]; this->v[80]=A[2];
	this->v[72] = this->v[74] = this->v[76] = this->v[77] = this->v[78] = this->v[79] = value_type(0);
      } // end of T2toT2Expr
      /*!
       * \brief access operator
       * \param[in] i : line   index
       * \param[in] j : column index
       */
      const value_type&
      operator()(const unsigned short i,
		 const unsigned short j) const
      {
	return this->v[i*9+j];
      } // end of operator()
    }; // end of struct T2toT2Expr<T2toT2Type,TensorProductRightDerivativeExpr<1u> >

  } // end of namespace tfel

} // end of namespace math

#endif /* _LIB_TFEL_MATH_TENSORPRODUCTRIGHTDERIVATIVEEXPR_H */

