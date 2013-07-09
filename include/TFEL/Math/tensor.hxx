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

    template<unsigned short N,typename T>
    class tensor
      : public TensorConcept<tensor<N,T> >
    {
      
      /*!
       * A simple check
       */
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));

    public:

      /*
       * This is a TensorConcept requirement.
       */
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * type of the tensor's values.
       * (this is a stl requirement).
       */
      typedef T value_type;
      /*!
       * type of a pointer to the value contained.
       * (this is a stl requirement).
       */
      typedef value_type* pointer;
      /*!
       * type of a const pointer to the value contained.
       * (this is a stl requirement).
       */
      typedef const value_type* const_pointer;
      /*!
       * type of the tensor's iterator.
       * (provided for stl compatibility).
       */
      typedef pointer iterator;
      /*!
       * type of the tensor's const iterator.
       * (provided for stl compatibility).
       */
      typedef const_pointer const_iterator;
      /*!
       * type of the tensor's reverse iterator.
       * (provided for stl compatibility).
       */
#ifndef __SUNPRO_CC
      typedef std::reverse_iterator<iterator> reverse_iterator; 
#else
      typedef std::reverse_iterator<iterator,T,
      				    reference,
      				    difference_type> reverse_iterator;
#endif
      /*!
       * type of the tensor's const reverse iterator.
       * (provided for stl compatibility).
       */
#ifndef __SUNPRO_CC
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator; 
#else
      typedef std::reverse_iterator<const_iterator,T,
      				    const_reference,
      				    difference_type> const_reverse_iterator;
#endif
      /*!
       * type of a reference to the value contained.
       * (this is a stl requirement).
       */
      typedef value_type& reference;
      /*!
       * type of a const reference to the value contained.
       * (this is a stl requirement).
       */
      typedef const value_type& const_reference;
      /*!
       * type of the size of the container.
       * (this is a stl requirement).
       */
      typedef unsigned short size_type;
      /*!
       * type of the difference between two iterators.
       * (this is a stl requirement).
       */
      typedef ptrdiff_t difference_type;

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
       * \param const typename tfel::typetraits::BaseType<T>::type*
       * const, pointer to a tabular used to initialise the components
       * of the tensor. This tabular is left unchanged.
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

      /*!
       * Assignement operator
       */
      TFEL_MATH_INLINE tensor<N,T>&
      operator=(const tensor<N,T>&);

      /*!
       * Assignement operator
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tensor<N,T>&>::type
      operator=(const tensor<N,T2>&);

      /*!
       * Assignement operator
       */
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tensor<N,T>&
      >::type
      operator=(const TensorExpr<tensor<N,T2>,Expr>&);

      // Assignement operator
      template<typename T2>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tensor<N,T>&
      >::type
      operator+=(const tensor<N,T2>&);
    
      // Assignement operator
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tensor<N,T>&
      >::type
      operator+=(const TensorExpr<tensor<N,T2>,Expr>&);

      // Assignement operator
      template<typename T2>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tensor<N,T>&
      >::type
      operator-=(const tensor<N,T2>&);
    
      // Assignement operator
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tensor<N,T>&
      >::type
      operator-=(const TensorExpr<tensor<N,T2>,Expr>&);

      /*!
       * operator*=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	tensor<N,T>&
      >::type
      operator*=(const T2);

      /*!
       * operator/=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
        tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	tensor<N,T>&
      >::type
      operator/=(const T2);

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
      operator[](const unsigned short) const;      
      TFEL_MATH_INLINE       T&
      operator[](const unsigned short);

      /*!
       * return the size of a symmetric tensor
       */
      TFEL_MATH_INLINE size_type
      size(void) const;
       /*
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const;
      /*
       * return an iterator to the first element of the tensor
       * (provided for stl compatibility)
       * \return iterator, an iterator to the first element
       */
      TFEL_MATH_INLINE2 
      iterator
      begin(void);
      
      /*
       * return an const iterator to the first element of the tensor
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator to the first element
       */
      TFEL_MATH_INLINE2
      const_iterator 
      begin(void) const;

      /*
       * return an iterator after the last element of the tensor
       * (provided for stl compatibility)
       * \return iterator, an iterator after the last element
       */
      TFEL_MATH_INLINE2
      iterator
      end(void);
      
      /*
       * return an const iterator after the last element of the tensor
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator after the last element
       */
      TFEL_MATH_INLINE2
      const_iterator
      end(void) const;

      /*
       * return an reverse iterator to the last element of the tensor
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator to the last element
       */
      TFEL_MATH_INLINE2
      reverse_iterator
      rbegin(void);
      
      /*
       * return an const reverse iterator to the last element of the tensor
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator to the last element
       */
      TFEL_MATH_INLINE2
      const_reverse_iterator
      rbegin(void) const;
      
      /*
       * return an  reverse iterator before the first element of the tensor
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator before the first element
       */
      TFEL_MATH_INLINE2
      reverse_iterator
      rend(void);
      
      /*
       * return an const reverse iterator before the first element of the tensor
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator before the first element
       */
      TFEL_MATH_INLINE2
      const_reverse_iterator
      rend(void) const;

      
      template<typename InputIterator>
      TFEL_MATH_INLINE2 void 
      copy(const InputIterator src);

    protected:
      
      T v[TensorDimeToSize<N>::value];

    }; // end of class tensor
        
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

