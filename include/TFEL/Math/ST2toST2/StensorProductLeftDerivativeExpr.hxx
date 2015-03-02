/*! 
 * \file  include/TFEL/Math/ST2toST2/StensorProductLeftDerivativeExpr.hxx
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

#ifndef LIB_TFEL_MATH_STENSORPRODUCTLEFTDERIVATIVEEXPR_H_
#define LIB_TFEL_MATH_STENSORPRODUCTLEFTDERIVATIVEEXPR_H_ 

#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"

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
    template<typename ST2toST2ResultType>
    struct Expr<ST2toST2ResultType,StensorProductLeftDerivativeExpr<1u> >
      : public ST2toST2Concept<Expr<ST2toST2ResultType,StensorProductLeftDerivativeExpr<1u> > >,
	public fsarray<9u,typename ST2toST2Traits<ST2toST2ResultType>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2ResultType>::dime==1u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
     /*!
       * \param[in] B : second tensor of the product
       */
      template<typename StensorType>
      Expr(const StensorType& B)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2ResultType>::dime==StensorTraits<StensorType>::dime));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
							     typename ST2toST2Traits<ST2toST2ResultType>::NumType>::cond));
	const value_type zero = value_type(0);
	this->v[0] = B[0];
	this->v[4] = B[1];
	this->v[8] = B[2];
	this->v[1] = this->v[2] = zero;
	this->v[3] = this->v[5] = zero;
	this->v[6] = this->v[7] = zero;
      } // end of Expr
      /*!
       * \param[in] B : second tensor of the product
       * \param[in] C : derivative of the first tensor
       */
      template<typename StensorType,
	       typename ST2toST2Type>
      Expr(const StensorType& B,
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
	this->v[0]=C(0,1)*B[0];
	this->v[1]=C(0,1)*B[0];
	this->v[2]=C(0,2)*B[0];
	this->v[3]=C(1,0)*B[1];
	this->v[4]=C(1,1)*B[1];
	this->v[5]=C(1,2)*B[1];
	this->v[6]=C(2,0)*B[2];
	this->v[7]=C(2,1)*B[2];
	this->v[8]=C(2,2)*B[2];
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
    }; // end of struct Expr<ST2toST2ResultType,StensorProductLeftDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 2D tensor
     */
    template<typename ST2toST2ResultType>
    struct Expr<ST2toST2ResultType,StensorProductLeftDerivativeExpr<2u> >
      : public ST2toST2Concept<Expr<ST2toST2ResultType,StensorProductLeftDerivativeExpr<2u> > >,
	public fsarray<16u,typename ST2toST2Traits<ST2toST2ResultType>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2ResultType>::dime==2u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] B : second tensor of the product
       */
      template<typename StensorType>
      Expr(const StensorType& B)
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
      } // end of Expr
      /*!
       * \param[in] B : second tensor of the product
       * \param[in] C : derivative of the first tensor
       */
      template<typename StensorType,
	       typename ST2toST2Type>
      Expr(const StensorType& B,
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
	this->v[0]=C(3,0)*B[3]*one_half+C(0,0)*B[0];
	this->v[1]=C(3,1)*B[3]*one_half+C(0,1)*B[0];
	this->v[2]=C(3,2)*B[3]*one_half+C(0,2)*B[0];
	this->v[3]=C(3,3)*B[3]*one_half+C(0,3)*B[0];
	this->v[4]=C(3,0)*B[3]*one_half+C(1,0)*B[1];
	this->v[5]=C(3,1)*B[3]*one_half+C(1,1)*B[1];
	this->v[6]=C(3,2)*B[3]*one_half+C(1,2)*B[1];
	this->v[7]=C(3,3)*B[3]*one_half+C(1,3)*B[1];
	this->v[8]=C(2,0)*B[2];
	this->v[9]=C(2,1)*B[2];
	this->v[10]=C(2,2)*B[2];
	this->v[11]=C(2,3)*B[2];
	this->v[12]=C(0,0)*B[3]+C(3,0)*B[1];
	this->v[13]=C(0,1)*B[3]+C(3,1)*B[1];
	this->v[14]=C(0,2)*B[3]+C(3,2)*B[1];
	this->v[15]=C(0,3)*B[3]+C(3,3)*B[1];
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
    }; // end of struct Expr<ST2toST2ResultType,StensorProductLeftDerivativeExpr<1u> >

    /*!
     * Partial specialisation for 3D tensor
     */
    template<typename ST2toST2ResultType>
    struct Expr<ST2toST2ResultType,StensorProductLeftDerivativeExpr<3u> >
      : public ST2toST2Concept<Expr<ST2toST2ResultType,StensorProductLeftDerivativeExpr<3u> > >,
	public fsarray<36u,typename ST2toST2Traits<ST2toST2ResultType>::NumType>
    {
      //! a simple check
      TFEL_STATIC_ASSERT(ST2toST2Traits<ST2toST2ResultType>::dime==3u);
      //! a simple alias
      typedef typename ST2toST2Traits<ST2toST2ResultType>::NumType value_type;
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \param[in] B : second tensor of the product
       */
      template<typename StensorType>
      Expr(const StensorType& B)
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
      } // end of Expr
      /*!
       * \param[in] B : second tensor of the product
       * \param[in] C : derivative of the first tensor
       */
      template<typename StensorType,
	       typename ST2toST2Type>
      Expr(const StensorType& B,
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
	const real cste  = real(1)/std::sqrt(real(2));
	this->v[0]=C(4,0)*B[4]*one_half+C(3,0)*B[3]*one_half+C(0,0)*B[0];
	this->v[1]=C(4,1)*B[4]*one_half+C(3,1)*B[3]*one_half+C(0,1)*B[0];
	this->v[2]=C(4,2)*B[4]*one_half+C(3,2)*B[3]*one_half+C(0,2)*B[0];
	this->v[3]=C(4,3)*B[4]*one_half+C(3,3)*B[3]*one_half+C(0,3)*B[0];
	this->v[4]=C(4,4)*B[4]*one_half+C(3,4)*B[3]*one_half+C(0,4)*B[0];
	this->v[5]=C(4,5)*B[4]*one_half+C(3,5)*B[3]*one_half+C(0,5)*B[0];
	this->v[6]=C(5,0)*B[5]*one_half+C(3,0)*B[3]*one_half+C(1,0)*B[1];
	this->v[7]=C(5,1)*B[5]*one_half+C(3,1)*B[3]*one_half+C(1,1)*B[1];
	this->v[8]=C(5,2)*B[5]*one_half+C(3,2)*B[3]*one_half+C(1,2)*B[1];
	this->v[9]=C(5,3)*B[5]*one_half+C(3,3)*B[3]*one_half+C(1,3)*B[1];
	this->v[10]=C(5,4)*B[5]*one_half+C(3,4)*B[3]*one_half+C(1,4)*B[1];
	this->v[11]=C(5,5)*B[5]*one_half+C(3,5)*B[3]*one_half+C(1,5)*B[1];
	this->v[12]=C(5,0)*B[5]*one_half+C(4,0)*B[4]*one_half+C(2,0)*B[2];
	this->v[13]=C(5,1)*B[5]*one_half+C(4,1)*B[4]*one_half+C(2,1)*B[2];
	this->v[14]=C(5,2)*B[5]*one_half+C(4,2)*B[4]*one_half+C(2,2)*B[2];
	this->v[15]=C(5,3)*B[5]*one_half+C(4,3)*B[4]*one_half+C(2,3)*B[2];
	this->v[16]=C(5,4)*B[5]*one_half+C(4,4)*B[4]*one_half+C(2,4)*B[2];
	this->v[17]=C(5,5)*B[5]*one_half+C(4,5)*B[4]*one_half+C(2,5)*B[2];
	this->v[18]=C(4,0)*B[5]*cste+C(0,0)*B[3]+C(3,0)*B[1];
	this->v[19]=C(4,1)*B[5]*cste+C(0,1)*B[3]+C(3,1)*B[1];
	this->v[20]=C(4,2)*B[5]*cste+C(0,2)*B[3]+C(3,2)*B[1];
	this->v[21]=C(4,3)*B[5]*cste+C(0,3)*B[3]+C(3,3)*B[1];
	this->v[22]=C(4,4)*B[5]*cste+C(0,4)*B[3]+C(3,4)*B[1];
	this->v[23]=C(4,5)*B[5]*cste+C(0,5)*B[3]+C(3,5)*B[1];
	this->v[24]=C(3,0)*B[5]*cste+C(0,0)*B[4]+C(4,0)*B[2];
	this->v[25]=C(3,1)*B[5]*cste+C(0,1)*B[4]+C(4,1)*B[2];
	this->v[26]=C(3,2)*B[5]*cste+C(0,2)*B[4]+C(4,2)*B[2];
	this->v[27]=C(3,3)*B[5]*cste+C(0,3)*B[4]+C(4,3)*B[2];
	this->v[28]=C(3,4)*B[5]*cste+C(0,4)*B[4]+C(4,4)*B[2];
	this->v[29]=C(3,5)*B[5]*cste+C(0,5)*B[4]+C(4,5)*B[2];
	this->v[30]=C(2,0)*B[5]+C(4,0)*B[3]*cste+C(5,0)*B[1];
	this->v[31]=C(2,1)*B[5]+C(4,1)*B[3]*cste+C(5,1)*B[1];
	this->v[32]=C(2,2)*B[5]+C(4,2)*B[3]*cste+C(5,2)*B[1];
	this->v[33]=C(2,3)*B[5]+C(4,3)*B[3]*cste+C(5,3)*B[1];
	this->v[34]=C(2,4)*B[5]+C(4,4)*B[3]*cste+C(5,4)*B[1];
	this->v[35]=C(2,5)*B[5]+C(4,5)*B[3]*cste+C(5,5)*B[1];
      }
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
    }; // end of struct Expr<ST2toST2ResultType,StensorProductLeftDerivativeExpr<1u> >

  } // end of namespace tfel

} // end of namespace math

#endif /* LIB_TFEL_MATH_STENSORPRODUCTLEFTDERIVATIVEEXPR_H_ */

