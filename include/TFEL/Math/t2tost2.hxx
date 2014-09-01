/*!
 * \file   include/TFEL/Math/t2tost2.hxx  
 * \brief    
 * \author Helfer Thomas
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_T2TOST2_H_
#define _LIB_TFEL_MATH_T2TOST2_H_ 

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

#include"TFEL/Math/Forward/t2tost2.hxx"
#include"TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include"TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include"TFEL/Math/Tensor/TensorConcept.hxx"
#include"TFEL/Math/T2toST2/T2toST2Concept.hxx"
#include"TFEL/Math/T2toST2/T2toST2ConceptOperations.hxx"
#include"TFEL/Math/T2toST2/T2toST2Expr.hxx"

namespace tfel{
  
  namespace math {

    //! forward declaration
    template<unsigned short>
    struct RightCauchyGreenTensorDerivativeExpr;

    //! forward declaration
    template<unsigned short>
    struct LeftCauchyGreenTensorDerivativeExpr;

    /*
     * Partial specialisation for t2tost2
     */
    template<unsigned short N,typename T>
    struct T2toST2Traits<t2tost2<N,T> >{
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
    struct t2tost2_base
    {
      /*!
       * Assignement operator
       */
      template<typename T2toST2Type>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<T2toST2Type,T2toST2Concept>::cond &&
	T2toST2Traits<Child>::dime==T2toST2Traits<T2toST2Type>::dime &&
	tfel::typetraits::IsAssignableTo<typename T2toST2Traits<T2toST2Type>::NumType,
					 typename T2toST2Traits<Child>::NumType>::cond,
	Child&>::type
      operator=(const T2toST2Type&);
      //! Assignement operator
      template<typename T2toST2Type>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<T2toST2Type,T2toST2Concept>::cond &&
	T2toST2Traits<Child>::dime==T2toST2Traits<T2toST2Type>::dime &&
	tfel::typetraits::IsAssignableTo<typename T2toST2Traits<T2toST2Type>::NumType,
					 typename T2toST2Traits<Child>::NumType>::cond,
	Child&>::type
      operator+=(const T2toST2Type&);
      //! Assignement operator
      template<typename T2toST2Type>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<T2toST2Type,T2toST2Concept>::cond &&
	T2toST2Traits<Child>::dime==T2toST2Traits<T2toST2Type>::dime &&
	tfel::typetraits::IsAssignableTo<typename T2toST2Traits<T2toST2Type>::NumType,
					 typename T2toST2Traits<Child>::NumType>::cond,
	Child&>::type
      operator-=(const T2toST2Type&);
      /*!
       * operator*=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
	tfel::meta::IsSameType<typename ResultType<typename T2toST2Traits<Child>::NumType,
						   T2,OpMult>::type,
			       typename T2toST2Traits<Child>::NumType>::cond,
	Child&>::type
      operator*=(const T2);
      /*!
       * operator/=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
        tfel::meta::IsSameType<typename ResultType<typename T2toST2Traits<Child>::NumType,
						   T2,OpDiv>::type,
			       typename T2toST2Traits<Child>::NumType>::cond,
	Child&>::type
      operator/=(const T2);
    }; // end of struct t2tost2_base

    template<unsigned short N,typename T>
    struct t2tost2
      : public T2toST2Concept<t2tost2<N,T> >,
	public fsarray<StensorDimeToSize<N>::value*TensorDimeToSize<N>::value,T>,
	public t2tost2_base<t2tost2<N,T> >
    {
      /*!
       * \param[in] F : deformation gradient
       * \return the derivative of the Cauchy right symmetric tensor
       * with respect to the deformation gradient
       */
      template<typename TensorType>
      static TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<TensorType,TensorConcept>::cond &&
	TensorTraits<TensorType>::dime==N&&
	tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,T>::cond,
	T2toST2Expr<t2tost2<N,T>,RightCauchyGreenTensorDerivativeExpr<N> > >::type
      dCdF(const TensorType&);
      /*!
       * \param[in] F : deformation gradient
       * \return the derivative of the Cauchy left symmetric tensor
       * with respect to the deformation gradient
       */
      template<typename TensorType>
      static TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<TensorType,TensorConcept>::cond &&
	TensorTraits<TensorType>::dime==N&&
	tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,T>::cond,
	T2toST2Expr<t2tost2<N,T>,LeftCauchyGreenTensorDerivativeExpr<N> > >::type
      dBdF(const TensorType&);
      /*!
       * This is a StensorConcept requirement.
       */
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \brief Default Constructor 
       */
      TFEL_MATH_INLINE 
      explicit t2tost2()
      {};
      /*!
       * \brief Default Constructor 
       * \param T, value used to initialise the components of the t2tost2 
       */
      TFEL_MATH_INLINE
      explicit t2tost2(const T);
      /*!
       * \brief Copy Constructor
       */
      TFEL_MATH_INLINE 
      t2tost2(const t2tost2<N,T>&);

      // Copy Constructor
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE 
      t2tost2(const T2toST2Expr<t2tost2<N,T2>,Expr>&);

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
      using t2tost2_base<t2tost2>::operator=;

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

    }; // end of struct t2tost2
        
  } // end of namespace math

  namespace typetraits{
    
    template<unsigned short N,typename T2,typename T>
    struct IsAssignableTo<tfel::math::t2tost2<N,T2>,
			  tfel::math::t2tost2<N,T> >
    {
      /*!
       *  Result
       */
      static const bool cond = IsAssignableTo<T2,T>::cond;
    };
    
  } // end of namespace typetraits

} // end of namespace tfel

#include"TFEL/Math/T2toST2/t2tost2.ixx"
#include"TFEL/Math/T2toST2/t2tost2ResultType.hxx"

#endif /* _LIB_TFEL_MATH_T2TOST2_H */

