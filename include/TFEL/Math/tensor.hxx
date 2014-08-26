/*!
 * \file   tensor.hxx  
 * \brief    
 * \author Helfer Thomas
 * \date   04 May 2006
 */

#ifndef _LIB_TFEL_TENSOR_H_
#define _LIB_TFEL_TENSOR_H_ 

#include<string>
#include<cassert>
#include<cstddef>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/TypeTraits/IsAssignableTo.hxx"
#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include"TFEL/Math/fsarray.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include"TFEL/Math/Tensor/TensorConcept.hxx"
#include"TFEL/Math/Tensor/TensorConceptOperations.hxx"
#include"TFEL/Math/Tensor/TensorExpr.hxx"

#include"TFEL/Math/Forward/tvector.hxx"
#include"TFEL/Math/Forward/tmatrix.hxx"
#include"TFEL/Math/Forward/tensor.hxx"

namespace tfel{
  
  namespace math {

    template<unsigned short N, typename T>
    struct TFEL_VISIBILITY_LOCAL TensorTraits<tensor<N,T> >
    {
      typedef T NumType;
      typedef unsigned short IndexType;
      static const unsigned short dime = N;
    };

    /*!
     * \brief a base for tensor or classes acting like tensor.
     * \param Child : child class
     * \param N     : spatial dimension
     * \param T     : numerical type
     */
    template<typename Child>
    struct tensor_base
    {
      /*!
       * Assignement operator
       */
      template<typename TensorType>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<TensorType,TensorConcept>::cond &&
	TensorTraits<Child>::dime==TensorTraits<TensorType>::dime &&
	tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,
					 typename TensorTraits<Child>::NumType>::cond,
	Child&>::type
      operator=(const TensorType&);
      //! Assignement operator
      template<typename TensorType>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<TensorType,TensorConcept>::cond &&
	TensorTraits<Child>::dime==TensorTraits<TensorType>::dime &&
	tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,
					 typename TensorTraits<Child>::NumType>::cond,
	Child&>::type
      operator+=(const TensorType&);
      //! Assignement operator
      template<typename TensorType>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<TensorType,TensorConcept>::cond &&
	TensorTraits<Child>::dime==TensorTraits<TensorType>::dime &&
	tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,
					 typename TensorTraits<Child>::NumType>::cond,
	Child&>::type
      operator-=(const TensorType&);
      /*!
       * operator*=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
	tfel::meta::IsSameType<typename ResultType<typename TensorTraits<Child>::NumType,
						   T2,OpMult>::type,
			       typename TensorTraits<Child>::NumType>::cond,
				 Child&>::type
      operator*=(const T2);
      /*!
       * operator/=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
        tfel::meta::IsSameType<typename ResultType<typename TensorTraits<Child>::NumType,
						   T2,OpDiv>::type,
			       typename TensorTraits<Child>::NumType>::cond,
	Child&>::type
      operator/=(const T2);
    }; // end of struct tensor_base

    template<unsigned short N,typename T>
    struct tensor
      : public TensorConcept<tensor<N,T> >,
	public tensor_base<tensor<N,T> >,
	public fsarray<TensorDimeToSize<N>::value,T>
    {
      
      /*
       * This is a TensorConcept requirement.
       */
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * \brief Build a tensor from a fortran matrix.
       * \param[in] t: tensor to be filled
       * \param[in] v: pointer to an array used to initialise the
       * components of the tensor. This array is left unchanged.
       */
      TFEL_MATH_INLINE2 static void
      buildFromFortranMatrix(tensor<N,T>&,
			     const typename tfel::typetraits::BaseType<T>::type* const);

      /*!
       * \brief Default Constructor 
       * \warning enabled only if storage is static
       */
      TFEL_MATH_INLINE explicit
      tensor()
      {};
      /*!
       * \brief Default Constructor 
       * \param T, value used to initialise the components of the tensor 
       * \warning enabled only if storage is static
       */
      TFEL_MATH_INLINE explicit
      tensor(const T);
      /*!
       * \brief Default Constructor.
       * \param init, pointer to an array used to initialise the
       * components of the tensor. This array is left unchanged.
       */
      TFEL_MATH_INLINE explicit
      tensor(const typename tfel::typetraits::BaseType<T>::type* const init)
      {
	typedef typename tfel::typetraits::BaseType<T>::type base;
	TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
	tfel::fsalgo::copy<TensorDimeToSize<N>::value>::exe(init,reinterpret_cast<base*>(this->v));
      }

      /*!
       * \brief Copy Constructor
       */
      TFEL_MATH_INLINE tensor(const tensor<N,T>& src)
      {
	tfel::fsalgo::copy<TensorDimeToSize<N>::value>::exe(src.v,this->v);
      }

      // Copy Constructor
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE tensor(const TensorExpr<tensor<N,T2>,Expr>& src)
      {
	TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T2,T>::cond));
	vectorToTab<TensorDimeToSize<N>::value>::exe(src,this->v);
      }

      /*!
       * Write to Tab
       */
      TFEL_MATH_INLINE2 void
      write(typename tfel::typetraits::BaseType<T>::type* const) const;

      /*!
       * Import values
       */
      template<typename T2>
      TFEL_MATH_INLINE2
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
	void>::type
      import(const T2* const);
      
      //! assignement operator
      using tensor_base<tensor<N,T> >::operator=;

      /*!
       * change basis
       */
      TFEL_MATH_INLINE2 void
      changeBasis(const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>&);

      /*!
       * Return Identity
       */
      TFEL_MATH_INLINE static const tensor<N,T>& Id(void);

      TFEL_MATH_INLINE const T&
      operator()(const unsigned short) const;      
      TFEL_MATH_INLINE       T&
      operator()(const unsigned short);

      TFEL_MATH_INLINE const T&
      operator()(const unsigned short,
		 const unsigned short) const;      
      
      TFEL_MATH_INLINE       T&
      operator()(const unsigned short,
		 const unsigned short);

      TFEL_MATH_INLINE const T&
      operator[](const unsigned short) const;      
      TFEL_MATH_INLINE       T&
      operator[](const unsigned short);

      /*!
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const;

      template<typename InputIterator>
      TFEL_MATH_INLINE2 void 
      copy(const InputIterator src);

    private:
      //! a simple check
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
    }; // end of class tensor

    template<unsigned short N, typename T,
	     typename OutputIterator>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond,
      void>::type
    exportToBaseTypeArray(const tensor<N,T>&,
			  OutputIterator);
    /*!
     * \return the invert of a 1D tensor
     * \param[in] t : tensor to be inverted
     */
    template<typename TensorType>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 1u,
      tensor<1u,typename ComputeBinaryResult<typename tfel::typetraits::BaseType<typename TensorTraits<TensorType>::NumType>::type,
					     typename TensorTraits<TensorType>::NumType,OpDiv>::Result>
    >::type
    invert(const TensorType&);
    /*!
     * \return the invert of a 2D tensor
     * \param[in] t : tensor to be inverted
     */
    template<typename TensorType>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 2u,
      tensor<2u,typename ComputeBinaryResult<typename tfel::typetraits::BaseType<typename TensorTraits<TensorType>::NumType>::type,
					     typename TensorTraits<TensorType>::NumType,OpDiv>::Result>
    >::type
    invert(const TensorType&);
    /*!
     * \return the invert of a 3D tensor
     * \param[in] t : tensor to be inverted
     */
    template<typename TensorType>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 3u,
      tensor<3u,typename ComputeBinaryResult<typename tfel::typetraits::BaseType<typename TensorTraits<TensorType>::NumType>::type,
					     typename TensorTraits<TensorType>::NumType,OpDiv>::Result>
    >::type
    invert(const TensorType&);

  } // end of namespace math

  namespace typetraits{
    
    template<unsigned short N,typename T2,typename T>
    struct IsAssignableTo<tfel::math::tensor<N,T2>,
			  tfel::math::tensor<N,T> >
    {
      /*!
       *  Result
       */
      static const bool cond = IsAssignableTo<T2,T>::cond;
    };
    
  } // end of namespace typetraits

} // end of namespace tfel

#include"TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include"TFEL/Math/Tensor/tensor.ixx"
#include"TFEL/Math/Tensor/tensorResultType.hxx"

#endif /* _LIB_TFEL_TENSOR_H */

