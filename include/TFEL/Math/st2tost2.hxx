/*!
 * \file   include/TFEL/Math/st2tost2.hxx  
 * \brief    
 * \author Helfer Thomas
 * \date   04 May 2006
 */

#ifndef _LIB_TFEL_ST2TOST2_H_
#define _LIB_TFEL_ST2TOST2_H_ 

#include<string>
#include<cstddef>
#include<cassert>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/TypeTraits/IsAssignableTo.hxx"
#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"

#include"TFEL/Math/fsarray.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include"TFEL/Math/Forward/st2tost2.hxx"
#include"TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include"TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2Concept.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2ConceptOperations.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2Expr.hxx"

namespace tfel{
  
  namespace math {

    // forward declaration
    template<unsigned short N>
    struct StensorProductLeftDerivativeExpr;

    // forward declaration
    template<unsigned short N>
    struct StensorProductRightDerivativeExpr;

    // forward declaration
    template<unsigned short N>
    struct ConvertT2toST2ToST2toST2Expr;

    /*
     * Partial specialisation for st2tost2
     */
    template<unsigned short N,typename T>
    struct ST2toST2Traits<st2tost2<N,T> >{
      typedef T              NumType;
      typedef unsigned short IndexType;
      static const unsigned short dime = N;
    };

    /*!
     * \brief a base for stensor or classes acting like stensor.
     * \param Child : child class
     * \param N     : spatial dimension
     * \param T     : numerical type
     */
    template<typename Child>
    struct st2tost2_base
    {
      /*!
       * Assignement operator
       */
      template<typename ST2toST2Type>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond &&
	ST2toST2Traits<Child>::dime==ST2toST2Traits<ST2toST2Type>::dime &&
	tfel::typetraits::IsAssignableTo<typename ST2toST2Traits<ST2toST2Type>::NumType,
					 typename ST2toST2Traits<Child>::NumType>::cond,
	Child&>::type
      operator=(const ST2toST2Type&);
      //! Assignement operator
      template<typename ST2toST2Type>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond &&
	ST2toST2Traits<Child>::dime==ST2toST2Traits<ST2toST2Type>::dime &&
	tfel::typetraits::IsAssignableTo<typename ST2toST2Traits<ST2toST2Type>::NumType,
					 typename ST2toST2Traits<Child>::NumType>::cond,
	Child&>::type
      operator+=(const ST2toST2Type&);
      //! Assignement operator
      template<typename ST2toST2Type>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond &&
	ST2toST2Traits<Child>::dime==ST2toST2Traits<ST2toST2Type>::dime &&
	tfel::typetraits::IsAssignableTo<typename ST2toST2Traits<ST2toST2Type>::NumType,
					 typename ST2toST2Traits<Child>::NumType>::cond,
	Child&>::type
      operator-=(const ST2toST2Type&);
      /*!
       * operator*=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
	tfel::meta::IsSameType<typename ResultType<typename ST2toST2Traits<Child>::NumType,
						   T2,OpMult>::type,
			       typename ST2toST2Traits<Child>::NumType>::cond,
	Child&>::type
      operator*=(const T2);
      /*!
       * operator/=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
        tfel::meta::IsSameType<typename ResultType<typename ST2toST2Traits<Child>::NumType,
						   T2,OpDiv>::type,
			       typename ST2toST2Traits<Child>::NumType>::cond,
	Child&>::type
      operator/=(const T2);
    }; // end of struct st2tost2_base

    template<unsigned short N,typename T>
    struct st2tost2
      : public ST2toST2Concept<st2tost2<N,T> >,
	public fsarray<StensorDimeToSize<N>::value*StensorDimeToSize<N>::value,T>,
	public st2tost2_base<st2tost2<N,T> >
    {
      /*!
       * \param[in] B : second tensor of the product
       * \return the left part of the derivative of a tensor product
       */
      template<typename StensorType>
      static TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<StensorType,StensorConcept>::cond &&
	StensorTraits<StensorType>::dime==N&&
	tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,T>::cond,
	ST2toST2Expr<st2tost2<N,T>,StensorProductLeftDerivativeExpr<N> > >::type
      tpld(const StensorType&);
      /*!
       * \param[in] B : second tensor of the product
       * \param[in] C : derivative of the first tensor
       * \return the left part of the derivative of a tensor product
       */
      template<typename StensorType,
	       typename ST2toST2Type>
      static TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<StensorType,StensorConcept>::cond &&
	tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond &&
	StensorTraits<StensorType>::dime==N&&
	ST2toST2Traits<ST2toST2Type>::dime==N&&
	tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename StensorTraits<StensorType>::NumType,
								      typename ST2toST2Traits<ST2toST2Type>::NumType,
								      OpMult>::Result,T>::cond,
	ST2toST2Expr<st2tost2<N,T>,StensorProductLeftDerivativeExpr<N> > >::type
      tpld(const StensorType&,
	   const ST2toST2Type&);
      /*!
       * \param[in] A : first tensor of the product
       * \return the right part of the derivative of a tensor product
       */
      template<typename StensorType>
      static TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<StensorType,StensorConcept>::cond &&
	StensorTraits<StensorType>::dime==N&&
	tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,T>::cond,
	ST2toST2Expr<st2tost2<N,T>,StensorProductRightDerivativeExpr<N> > >::type
      tprd(const StensorType&);
      /*!
       * \param[in] A : first tensor of the product
       * \param[in] C : derivative of the first tensor
       * \return the right part of the derivative of a tensor product
       */
      template<typename StensorType,
	       typename ST2toST2Type>
      static TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<StensorType,StensorConcept>::cond &&
	tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond &&
	StensorTraits<StensorType>::dime==N&&
	ST2toST2Traits<ST2toST2Type>::dime==N&&
	tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename StensorTraits<StensorType>::NumType,
								      typename ST2toST2Traits<ST2toST2Type>::NumType,
								      OpMult>::Result,T>::cond,
	ST2toST2Expr<st2tost2<N,T>,StensorProductLeftDerivativeExpr<N> > >::type
      tprd(const StensorType&,
	   const ST2toST2Type&);
      /*!
       * convert a T2toST2 to a ST2toST2
       * \param[in] src : T2toST2 to be converted
       */
      template<typename T2toST2Type>
      static TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<T2toST2Type,T2toST2Concept>::cond &&
	T2toST2Traits<T2toST2Type>::dime==N&&
	tfel::typetraits::IsAssignableTo<typename T2toST2Traits<T2toST2Type>::NumType,T>::cond,
	ST2toST2Expr<st2tost2<N,T>,ConvertT2toST2ToST2toST2Expr<N> > >::type
      convert(const T2toST2Type&);
      /*!
       * This is a StensorConcept requirement.
       */
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \brief Default Constructor 
       */
      TFEL_MATH_INLINE 
      explicit st2tost2()
      {};
      /*!
       * \brief Default Constructor 
       * \param T, value used to initialise the components of the st2tost2 
       */
      TFEL_MATH_INLINE
      explicit st2tost2(const T);

      /*!
       * \brief Default Constructor.
       * \param const typename tfel::typetraits::BaseType<T>::type* const,
       *  pointer to a tabular used to initialise the components of
       *  the st2tost2. This tabular is left unchanged.
       */
      TFEL_MATH_INLINE
      explicit st2tost2(const typename tfel::typetraits::BaseType<T>::type* const);

      /*!
       * \brief Copy Constructor
       */
      TFEL_MATH_INLINE
      st2tost2(const st2tost2<N,T>&);

      // Copy Constructor
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE 
      st2tost2(const ST2toST2Expr<st2tost2<N,T2>,Expr>&);

      /*!
       * Import values
       */
      template<typename T2>
      TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
	tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
	void>::type
      import(const T2* const);

      /*!
       * Assignement operator
       */
      using st2tost2_base<st2tost2>::operator=;

      //! access operator
      TFEL_MATH_INLINE const T& operator()(const unsigned short,const unsigned short) const;      
      //! access operator
      TFEL_MATH_INLINE       T& operator()(const unsigned short,const unsigned short);
      /*!
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const;

      template<typename InputIterator>
      TFEL_MATH_INLINE2 void copy(const InputIterator src);

      static const st2tost2& Id(void);
      static const st2tost2& J(void);
      static const st2tost2& K(void);
      static const st2tost2& M(void);
      static const st2tost2& IxI(void);

    private:
            
      /*!
       * A simple check
       */
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));

      enum ParticularSt2toSt2{
	ST2TOST2_IDENTITY,
	ST2TOST2_J,
	ST2TOST2_K,
	ST2TOST2_IxI
      }; // end of enum ParticularSt2toSt2

      st2tost2(const ParticularSt2toSt2);

    }; // end of struct st2tost2

    /*!
     * \return the invert of a st2tost2
     * \param[in] s : st2tost2 to be inverted
     */
    template<typename ST2toST2Type>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond,
      st2tost2<ST2toST2Traits<ST2toST2Type>::dime,
	       typename ComputeBinaryResult<typename tfel::typetraits::BaseType<typename ST2toST2Traits<ST2toST2Type>::NumType>::type,
					    typename ST2toST2Traits<ST2toST2Type>::NumType,OpDiv>::Result>
    >::type
    invert(const ST2toST2Type&);
        
  } // end of namespace math

  namespace typetraits{
    
    template<unsigned short N,typename T2,typename T>
    struct IsAssignableTo<tfel::math::st2tost2<N,T2>,
			  tfel::math::st2tost2<N,T> >
    {
      /*!
       *  Result
       */
      static const bool cond = IsAssignableTo<T2,T>::cond;
    };
    
  } // end of namespace typetraits

} // end of namespace tfel

#include"TFEL/Math/ST2toST2/st2tost2.ixx"
#include"TFEL/Math/ST2toST2/st2tost2ResultType.hxx"

#endif /* _LIB_TFEL_ST2TOST2_H */

