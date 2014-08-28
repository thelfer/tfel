/*!
 * \file   include/TFEL/Math/t2tot2.hxx  
 * \brief    
 * \author Helfer Thomas
 * \date   19 November 2013
 */

#ifndef _LIB_TFEL_MATH_T2TOT2_H_
#define _LIB_TFEL_MATH_T2TOT2_H_ 

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

#include"TFEL/Math/Forward/t2tot2.hxx"
#include"TFEL/Math/Tensor/TensorConcept.hxx"
#include"TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include"TFEL/Math/T2toT2/T2toT2Concept.hxx"
#include"TFEL/Math/T2toT2/T2toT2ConceptOperations.hxx"
#include"TFEL/Math/T2toT2/T2toT2Expr.hxx"

namespace tfel{
  
  namespace math {

    // forward declaration
    template<unsigned short N>
    struct TensorProductLeftDerivativeExpr;

    // forward declaration
    template<unsigned short N>
    struct TensorProductRightDerivativeExpr;

    /*
     * Partial specialisation for t2tot2
     */
    template<unsigned short N,typename T>
    struct T2toT2Traits<t2tot2<N,T> >{
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
    struct t2tot2_base
    {
      /*!
       * Assignement operator
       */
      template<typename T2toT2Type>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond &&
	T2toT2Traits<Child>::dime==T2toT2Traits<T2toT2Type>::dime &&
	tfel::typetraits::IsAssignableTo<typename T2toT2Traits<T2toT2Type>::NumType,
					 typename T2toT2Traits<Child>::NumType>::cond,
	Child&>::type
      operator=(const T2toT2Type&);
      //! Assignement operator
      template<typename T2toT2Type>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond &&
	T2toT2Traits<Child>::dime==T2toT2Traits<T2toT2Type>::dime &&
	tfel::typetraits::IsAssignableTo<typename T2toT2Traits<T2toT2Type>::NumType,
					 typename T2toT2Traits<Child>::NumType>::cond,
	Child&>::type
      operator+=(const T2toT2Type&);
      //! Assignement operator
      template<typename T2toT2Type>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond &&
	T2toT2Traits<Child>::dime==T2toT2Traits<T2toT2Type>::dime &&
	tfel::typetraits::IsAssignableTo<typename T2toT2Traits<T2toT2Type>::NumType,
					 typename T2toT2Traits<Child>::NumType>::cond,
	Child&>::type
      operator-=(const T2toT2Type&);
      /*!
       * operator*=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
	tfel::meta::IsSameType<typename ResultType<typename T2toT2Traits<Child>::NumType,
						   T2,OpMult>::type,
			       typename T2toT2Traits<Child>::NumType>::cond,
	Child&>::type
      operator*=(const T2);
      /*!
       * operator/=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
        tfel::meta::IsSameType<typename ResultType<typename T2toT2Traits<Child>::NumType,
						   T2,OpDiv>::type,
			       typename T2toT2Traits<Child>::NumType>::cond,
	Child&>::type
      operator/=(const T2);
    }; // end of struct t2tot2_base

    template<unsigned short N,typename T>
    struct t2tot2
      : public T2toT2Concept<t2tot2<N,T> >,
	public fsarray<TensorDimeToSize<N>::value*TensorDimeToSize<N>::value,T>,
	public t2tot2_base<t2tot2<N,T> >
    {
      /*!
       * \param[in] B : second tensor of the product
       * \return the left part of the derivative of a tensor product
       */
      template<typename TensorType>
      static TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<TensorType,TensorConcept>::cond &&
	TensorTraits<TensorType>::dime==N&&
	tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,T>::cond,
	T2toT2Expr<t2tot2<N,T>,TensorProductLeftDerivativeExpr<N> > >::type
      tpld(const TensorType&);
      /*!
       * \param[in] B : second tensor of the product
       * \param[in] C : derivative of the first tensor
       * \return the left part of the derivative of a tensor product
       */
      template<typename TensorType,
	       typename T2toT2Type>
      static TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<TensorType,TensorConcept>::cond &&
	tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond &&
	TensorTraits<TensorType>::dime==N&&
	T2toT2Traits<T2toT2Type>::dime==N&&
	tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename TensorTraits<TensorType>::NumType,
								      typename T2toT2Traits<T2toT2Type>::NumType,
								      OpMult>::Result,T>::cond,
	T2toT2Expr<t2tot2<N,T>,TensorProductLeftDerivativeExpr<N> > >::type
      tpld(const TensorType&,
	   const T2toT2Type&);
      /*!
       * \param[in] A : first tensor of the product
       * \return the right part of the derivative of a tensor product
       */
      template<typename TensorType>
      static TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<TensorType,TensorConcept>::cond &&
	TensorTraits<TensorType>::dime==N&&
	tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,T>::cond,
	T2toT2Expr<t2tot2<N,T>,TensorProductRightDerivativeExpr<N> > >::type
      tprd(const TensorType&);
      /*!
       * \param[in] A : first tensor of the product
       * \param[in] C : derivative of the first tensor
       * \return the right part of the derivative of a tensor product
       */
      template<typename TensorType,
	       typename T2toT2Type>
      static TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<TensorType,TensorConcept>::cond &&
	tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond &&
	TensorTraits<TensorType>::dime==N&&
	T2toT2Traits<T2toT2Type>::dime==N&&
	tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<typename TensorTraits<TensorType>::NumType,
								      typename T2toT2Traits<T2toT2Type>::NumType,
								      OpMult>::Result,T>::cond,
	T2toT2Expr<t2tot2<N,T>,TensorProductLeftDerivativeExpr<N> > >::type
      tprd(const TensorType&,
	   const T2toT2Type&);
      /*!
       * This is a StensorConcept requirement.
       */
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \brief Default Constructor 
       */
      TFEL_MATH_INLINE 
      explicit t2tot2()
      {};
      /*!
       * \brief Default Constructor 
       * \param T, value used to initialise the components of the t2tot2 
       */
      TFEL_MATH_INLINE
      explicit t2tot2(const T);
      /*!
       * \brief Copy Constructor
       */
      TFEL_MATH_INLINE
      t2tot2(const t2tot2<N,T>&);

      // Copy Constructor
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE
      t2tot2(const T2toT2Expr<t2tot2<N,T2>,Expr>&);

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
      using t2tot2_base<t2tot2>::operator=;

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

    }; // end of struct t2tot2
        
  } // end of namespace math

  namespace typetraits{
    
    template<unsigned short N,typename T2,typename T>
    struct IsAssignableTo<tfel::math::t2tot2<N,T2>,
			  tfel::math::t2tot2<N,T> >
    {
      /*!
       *  Result
       */
      static const bool cond = IsAssignableTo<T2,T>::cond;
    };
    
  } // end of namespace typetraits

} // end of namespace tfel

#include"TFEL/Math/T2toT2/t2tot2.ixx"
#include"TFEL/Math/T2toT2/t2tot2ResultType.hxx"

#endif /* _LIB_TFEL_MATH_T2TOT2_H */

