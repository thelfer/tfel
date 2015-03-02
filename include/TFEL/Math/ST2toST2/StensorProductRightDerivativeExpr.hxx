/*! 
 * \file  include/TFEL/Math/ST2toST2/StensorProductRightDerivativeExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 04 juin 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_STENSORPRODUCTRIGHTDERIVATIVEEXPR_H_
#define LIB_TFEL_MATH_STENSORPRODUCTRIGHTDERIVATIVEEXPR_H_ 

#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"

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
    template<typename ST2toST2ResultType>
    struct Expr<ST2toST2ResultType,StensorProductRightDerivativeExpr<1u> >
      : public ST2toST2Concept<Expr<ST2toST2ResultType,StensorProductRightDerivativeExpr<1u> > >,
	public fsarray<9u,typename ST2toST2Traits<ST2toST2ResultType>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2ResultType>::dime==1u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] A : second stensor of the product
       */
      template<typename StensorType>
      Expr(const StensorType& A)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2ResultType>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
							     typename ST2toST2Traits<ST2toST2ResultType>::NumType>::cond));
	const value_type zero = value_type(0);
	this->v[0] = A[0];
	this->v[4] = A[1];
	this->v[8] = A[2];
	this->v[1] = this->v[2] = zero;
	this->v[3] = this->v[5] = zero;
	this->v[6] = this->v[7] = zero;
      } // end of Expr
      /*!
       * \param[in] A : first tensor of the product
       * \param[in] C : derivative of the second tensor
       */
      template<typename StensorType,
	       typename ST2toST2Type>
      Expr(const StensorType& A,
		   const ST2toST2Type& C)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2ResultType>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2Type>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename StensorTraits<StensorType>::NumType,
											  typename ST2toST2Traits<ST2toST2Type>::NumType,
											  OpMult>::Result,
							     typename ST2toST2Traits<ST2toST2ResultType>::NumType>::cond));
	this->v[0]=C(0,1)*A[0];
	this->v[1]=C(0,1)*A[0];
	this->v[2]=C(0,2)*A[0];
	this->v[3]=C(1,0)*A[1];
	this->v[4]=C(1,1)*A[1];
	this->v[5]=C(1,2)*A[1];
	this->v[6]=C(2,0)*A[2];
	this->v[7]=C(2,1)*A[2];
	this->v[8]=C(2,2)*A[2];
      } // end of Expr
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
      /*!
       * \return the runtime properties of this object
       * In this case, the number of lines and columns
       * are deduced from the template parameter
       */
      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }
    }; // end of struct Expr<ST2toST2ResultType,StensorProductRightDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 2D stensor
     */
    template<typename ST2toST2ResultType>
    struct Expr<ST2toST2ResultType,StensorProductRightDerivativeExpr<2u> >
      : public ST2toST2Concept<Expr<ST2toST2ResultType,StensorProductRightDerivativeExpr<2u> > >,
	public fsarray<16u,typename ST2toST2Traits<ST2toST2ResultType>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2ResultType>::dime==2u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] A : second stensor of the product
       */
      template<typename StensorType>
      Expr(const StensorType& A)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2ResultType>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
							     typename ST2toST2Traits<ST2toST2ResultType>::NumType>::cond));
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
      } // end of Expr
      /*!
       * \param[in] A : first tensor of the product
       * \param[in] C : derivative of the second tensor
       */
      template<typename StensorType,
	       typename ST2toST2Type>
      Expr(const StensorType& A,
		   const ST2toST2Type& C)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2ResultType>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2Type>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename StensorTraits<StensorType>::NumType,
											  typename ST2toST2Traits<ST2toST2Type>::NumType,
											  OpMult>::Result,
							     typename ST2toST2Traits<ST2toST2ResultType>::NumType>::cond));
	using tfel::typetraits::BaseType;
	typedef typename BaseType<value_type>::type real;
	const real one_half   = real(1)/2;
	this->v[0]=C(3,0)*A[3]*one_half+C(0,0)*A[0];
	this->v[1]=C(3,1)*A[3]*one_half+C(0,1)*A[0];
	this->v[2]=C(3,2)*A[3]*one_half+C(0,2)*A[0];
	this->v[3]=C(3,3)*A[3]*one_half+C(0,3)*A[0];
	this->v[4]=C(3,0)*A[3]*one_half+C(1,0)*A[1];
	this->v[5]=C(3,1)*A[3]*one_half+C(1,1)*A[1];
	this->v[6]=C(3,2)*A[3]*one_half+C(1,2)*A[1];
	this->v[7]=C(3,3)*A[3]*one_half+C(1,3)*A[1];
	this->v[8]=C(2,0)*A[2];
	this->v[9]=C(2,1)*A[2];
	this->v[10]=C(2,2)*A[2];
	this->v[11]=C(2,3)*A[2];
	this->v[12]=C(1,0)*A[3]+C(3,0)*A[0];
	this->v[13]=C(1,1)*A[3]+C(3,1)*A[0];
	this->v[14]=C(1,2)*A[3]+C(3,2)*A[0];
	this->v[15]=C(1,3)*A[3]+C(3,3)*A[0];
      } // end of Expr
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
      /*!
       * \return the runtime properties of this object
       * In this case, the number of lines and columns
       * are deduced from the template parameter
       */
      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }
    }; // end of struct Expr<ST2toST2ResultType,StensorProductRightDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 3D stensor
     */
    template<typename ST2toST2ResultType>
    struct Expr<ST2toST2ResultType,StensorProductRightDerivativeExpr<3u> >
      : public ST2toST2Concept<Expr<ST2toST2ResultType,StensorProductRightDerivativeExpr<3u> > >,
	public fsarray<36u,typename ST2toST2Traits<ST2toST2ResultType>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2ResultType>::dime==3u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] A : second stensor of the product
       */
      template<typename StensorType>
      Expr(const StensorType& A)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2ResultType>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
							     typename ST2toST2Traits<ST2toST2ResultType>::NumType>::cond));
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
      } // end of Expr
      /*!
       * \param[in] A : second stensor of the product
       * \param[in] C : derivative of the second tensor
       */
      template<typename StensorType,
	       typename ST2toST2Type>
      Expr(const StensorType& A,
		   const ST2toST2Type& C)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2ResultType>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2Type>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename StensorTraits<StensorType>::NumType,
											  typename ST2toST2Traits<ST2toST2Type>::NumType,
											  OpMult>::Result,
							     typename ST2toST2Traits<ST2toST2ResultType>::NumType>::cond));
	using tfel::typetraits::BaseType;
	typedef typename BaseType<value_type>::type real;
	const value_type zero = value_type(0);
	const real one_half   = real(1)/2;
	const real cste       = real(1)/std::sqrt(real(2));
	this->v[0]=C(4,0)*A[4]*one_half+C(3,0)*A[3]*one_half+C(0,0)*A[0];
	this->v[1]=C(4,1)*A[4]*one_half+C(3,1)*A[3]*one_half+C(0,1)*A[0];
	this->v[2]=C(4,2)*A[4]*one_half+C(3,2)*A[3]*one_half+C(0,2)*A[0];
	this->v[3]=C(4,3)*A[4]*one_half+C(3,3)*A[3]*one_half+C(0,3)*A[0];
	this->v[4]=C(4,4)*A[4]*one_half+C(3,4)*A[3]*one_half+C(0,4)*A[0];
	this->v[5]=C(4,5)*A[4]*one_half+C(3,5)*A[3]*one_half+C(0,5)*A[0];
	this->v[6]=C(5,0)*A[5]*one_half+C(3,0)*A[3]*one_half+C(1,0)*A[1];
	this->v[7]=C(5,1)*A[5]*one_half+C(3,1)*A[3]*one_half+C(1,1)*A[1];
	this->v[8]=C(5,2)*A[5]*one_half+C(3,2)*A[3]*one_half+C(1,2)*A[1];
	this->v[9]=C(5,3)*A[5]*one_half+C(3,3)*A[3]*one_half+C(1,3)*A[1];
	this->v[10]=C(5,4)*A[5]*one_half+C(3,4)*A[3]*one_half+C(1,4)*A[1];
	this->v[11]=C(5,5)*A[5]*one_half+C(3,5)*A[3]*one_half+C(1,5)*A[1];
	this->v[12]=C(5,0)*A[5]*one_half+C(4,0)*A[4]*one_half+C(2,0)*A[2];
	this->v[13]=C(5,1)*A[5]*one_half+C(4,1)*A[4]*one_half+C(2,1)*A[2];
	this->v[14]=C(5,2)*A[5]*one_half+C(4,2)*A[4]*one_half+C(2,2)*A[2];
	this->v[15]=C(5,3)*A[5]*one_half+C(4,3)*A[4]*one_half+C(2,3)*A[2];
	this->v[16]=C(5,4)*A[5]*one_half+C(4,4)*A[4]*one_half+C(2,4)*A[2];
	this->v[17]=C(5,5)*A[5]*one_half+C(4,5)*A[4]*one_half+C(2,5)*A[2];
	this->v[18]=C(5,0)*A[4]*cste+C(1,0)*A[3]+C(3,0)*A[0];
	this->v[19]=C(5,1)*A[4]*cste+C(1,1)*A[3]+C(3,1)*A[0];
	this->v[20]=C(5,2)*A[4]*cste+C(1,2)*A[3]+C(3,2)*A[0];
	this->v[21]=C(5,3)*A[4]*cste+C(1,3)*A[3]+C(3,3)*A[0];
	this->v[22]=C(5,4)*A[4]*cste+C(1,4)*A[3]+C(3,4)*A[0];
	this->v[23]=C(5,5)*A[4]*cste+C(1,5)*A[3]+C(3,5)*A[0];
	this->v[24]=C(2,0)*A[4]+C(5,0)*A[3]*cste+C(4,0)*A[0];
	this->v[25]=C(2,1)*A[4]+C(5,1)*A[3]*cste+C(4,1)*A[0];
	this->v[26]=C(2,2)*A[4]+C(5,2)*A[3]*cste+C(4,2)*A[0];
	this->v[27]=C(2,3)*A[4]+C(5,3)*A[3]*cste+C(4,3)*A[0];
	this->v[28]=C(2,4)*A[4]+C(5,4)*A[3]*cste+C(4,4)*A[0];
	this->v[29]=C(2,5)*A[4]+C(5,5)*A[3]*cste+C(4,5)*A[0];
	this->v[30]=C(1,0)*A[5]+C(3,0)*A[4]*cste+C(5,0)*A[2];
	this->v[31]=C(1,1)*A[5]+C(3,1)*A[4]*cste+C(5,1)*A[2];
	this->v[32]=C(1,2)*A[5]+C(3,2)*A[4]*cste+C(5,2)*A[2];
	this->v[33]=C(1,3)*A[5]+C(3,3)*A[4]*cste+C(5,3)*A[2];
	this->v[34]=C(1,4)*A[5]+C(3,4)*A[4]*cste+C(5,4)*A[2];
	this->v[35]=C(1,5)*A[5]+C(3,5)*A[4]*cste+C(5,5)*A[2];
      } // end of Expr
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
      /*!
       * \return the runtime properties of this object
       * In this case, the number of lines and columns
       * are deduced from the template parameter
       */
      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }
    }; // end of struct Expr<ST2toST2ResultType,StensorProductRightDerivativeExpr<1u> >

  } // end of namespace tfel

} // end of namespace math

#endif /* LIB_TFEL_MATH_STENSORPRODUCTRIGHTDERIVATIVEEXPR_H_ */

