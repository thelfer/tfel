/*!
 * \file   stensor.hxx  
 * \brief    
 * \author Helfer Thomas
 * \date   04 May 2006
 */

#ifndef _LIB_TFEL_STENSOR_H_
#define _LIB_TFEL_STENSOR_H_ 

#include<string>
#include<cassert>

#include"Config/TFELConfig.hxx"

#include"Metaprogramming/EnableIf.hxx"
#include"Metaprogramming/StaticAssert.hxx"

#include"Utilities/Name.hxx"
#include"Utilities/ToString.hxx"

#include"TypeTraits/IsScalar.hxx"
#include"TypeTraits/BaseType.hxx"
#include"TypeTraits/IsAssignableTo.hxx"

#include"Math/General/BasicOperations.hxx"

#include"Math/Stensor/StensorNullStorage.hxx"
#include"Math/Stensor/StensorConcept.hxx"
#include"Math/Stensor/StensorConceptOperations.hxx"
#include"Math/Stensor/StensorStaticStorage.hxx"
#include"Math/Stensor/StensorExpr.hxx"

#include"Math/tvector.hxx"
#include"Math/tmatrix.hxx"

namespace tfel{
  
  namespace math {

    /*!
     * \class stensor
     * \brief finite size symmetric tensor.
     * \param N, the spatial dimension, see StensorDimeToSize for details. 
     * \param T, numerical type used, by default, double
     * \param Storage, type of storage used
     * \pre   This class is only defined for N=1u,2u and 3u.
     * \see   StensorDimeToSize and StensorSizeToDime. 
     * \see   StensorStatic, StensorExternalTab,StensorExternalVoigt.
     */
    template<unsigned short N,typename T=double,
	     template<unsigned short,typename> class Storage=StensorStatic>
    class stensor;

    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage>
    struct StensorTraits<stensor<N,T,Storage> >
    {
      typedef T NumType;
      static const unsigned short dime = N;
    };

    template<unsigned short N,typename T,
	     template<unsigned short,typename> class Storage>
    class stensor
      : public StensorConcept<stensor<N,T,Storage> >,
	public Storage<StensorDimeToSize<N>::value,T>
    {
      
      /*!
       * A simple check
       */
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));

    public:

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static
      std::string
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("stensor<")
	  +ToString(N)+string(",")
	  +Name<T>::getName()+string(",")
	  +Name<Storage<N,T> >::getName()
	  +string(">");
      }

      /*!
       * type of the stensor's values.
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
       * type of the stensor's iterator.
       * (provided for stl compatibility).
       */
      typedef pointer iterator;
      /*!
       * type of the stensor's const iterator.
       * (provided for stl compatibility).
       */
      typedef const_pointer const_iterator;
      /*!
       * type of the stensor's reverse iterator.
       * (provided for stl compatibility).
       */
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      /*!
       * type of the stensor's const reverse iterator.
       * (provided for stl compatibility).
       */
      typedef std::reverse_iterator<iterator> reverse_iterator;
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
      typedef size_t size_type;
      /*!
       * type of the difference between two iterators.
       * (this is a stl requirement).
       */
      typedef ptrdiff_t difference_type;

      /*!
       * \brief Default Constructor 
       * \warning enabled only if storage is static
       */
      explicit stensor()
      {};

      /*!
       * \brief Default Constructor 
       * \param T, value used to initialise the components of the stensor 
       * \warning enabled only if storage is static
       */
      explicit stensor(const T);

      /*!
       * \brief Default Constructor.
       * \param const typename tfel::typetraits::BaseType<T>::type*
       * const, pointer to a tabular used to initialise the components
       * of the stensor. This tabular is left unchanged.
       */
      explicit stensor(const typename tfel::typetraits::BaseType<T>::type* const init)
	: Storage<StensorDimeToSize<N>::value,T>(init)
      {}

      /*!
       * \brief Default Constructor.
       * \param typename tfel::typetraits::BaseType<T>::type* const,
       * pointer to a tabular used to initialise the components of the stensor. 
       */
      explicit stensor(typename tfel::typetraits::BaseType<T>::type* const init)
	: Storage<StensorDimeToSize<N>::value,T>(init)
      {}

      /*!
       * \brief Copy Constructor
       */
      stensor(const stensor<N,T,Storage>& src)
	: Storage<StensorDimeToSize<N>::value,T>(src) 
      {}

      // Copy Constructor
      template<typename T2,template<unsigned short,typename> class Storage2,typename Expr>
      stensor(const StensorExpr<stensor<N,T2,Storage2>,Expr>& src)
      {
	TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T2,T>::cond));
	vectorToTab<StensorDimeToSize<N>::value>::exe(src,this->v);
      }

      /*!
       * \brief Import from Voigt
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
	void
	>::type
      importVoigt(const T2* const);

      /*!
       * Import from Tab (Voigt notations for stresses)
       */
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    importTab(const T2* const);

      /*!
       * Export to Tab (Voigt notations for stresses)
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
	void>::type
      exportTab(T2* const) const;

      /*!
       * Write to Tab
       */
      void write(typename tfel::typetraits::BaseType<T>::type* const) const;

      /*!
       * Import values
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
	void>::type
      import(const T2* const);

      /*!
       * Assignement operator
       */
      template<template<unsigned short,typename> class Storage2>
      stensor<N,T,Storage>&
      operator=(const stensor<N,T,Storage2>&);

      /*!
       * Assignement operator
       */
      template<typename T2,template<unsigned short,typename> class Storage2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	stensor<N,T,Storage>&>::type
      operator=(const stensor<N,T2,Storage2>&);

      /*!
       * Assignement operator
       */
      template<typename T2,template<unsigned short,typename> class Storage2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	stensor<N,T,Storage>&
      >::type
      operator=(const StensorExpr<stensor<N,T2,Storage2>,Expr>&);

      // Assignement operator
      template<typename T2,template<unsigned short,typename> class Storage2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	stensor<N,T,Storage>&
      >::type
      operator+=(const stensor<N,T2,Storage2>&);
    
      // Assignement operator
      template<typename T2,template<unsigned short,typename> class Storage2,typename Expr>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	stensor<N,T,Storage>&
      >::type
      operator+=(const StensorExpr<stensor<N,T2,Storage2>,Expr>&);

      // Assignement operator
      template<typename T2,template<unsigned short,typename> class Storage2>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	stensor<N,T,Storage>&
      >::type
      operator-=(const stensor<N,T2,Storage2>&);
    
      // Assignement operator
      template<typename T2,template<unsigned short,typename> class Storage2,typename Expr>
      TFEL_MATH_INLINE typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	stensor<N,T,Storage>&
      >::type
      operator-=(const StensorExpr<stensor<N,T2,Storage2>,Expr>&);

      /*!
       * operator*=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	stensor<N,T,Storage>&
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
	stensor<N,T,Storage>&
      >::type
      operator/=(const T2);

      /*!
       * compute eigenvalues
       */
      void computeEigenValues(T& vp1,T& vp2,T& vp3) const;

      /*!
       * compute eigenvalues
       */
      void computeEigenValues(tvector<3u,T>&) const;

      /*!
       * compute eigenvectors and eigenvalues 
       */
      bool computeEigenVectors(tvector<3u,T>&,tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>&) const;
    
      /*!
       * change basis
       */
      void changeBasis(const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>&);

      /*!
       * Return Identity
       */
      TFEL_MATH_INLINE static const stensor<N,T,Storage>& Id(void);

      TFEL_MATH_INLINE const T& operator()(const unsigned short) const;      
      TFEL_MATH_INLINE       T& operator()(const unsigned short);

      TFEL_MATH_INLINE const T& operator[](const unsigned short) const;      
      TFEL_MATH_INLINE       T& operator[](const unsigned short);

 
      /*
       * return an iterator to the first element of the vector
       * (provided for stl compatibility)
       * \return iterator, an iterator to the first element
       */
      TFEL_MATH_INLINE2 
      iterator
      begin(void);
      
      /*
       * return an const iterator to the first element of the vector
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator to the first element
       */
      TFEL_MATH_INLINE2
      const_iterator 
      begin(void) const;

      /*
       * return an iterator after the last element of the vector
       * (provided for stl compatibility)
       * \return iterator, an iterator after the last element
       */
      TFEL_MATH_INLINE2
      iterator
      end(void);
      
      /*
       * return an const iterator after the last element of the vector
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator after the last element
       */
      TFEL_MATH_INLINE2
      const_iterator
      end(void) const;

      /*
       * return an reverse iterator to the last element of the vector
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator to the last element
       */
      TFEL_MATH_INLINE2
      reverse_iterator
      rbegin(void);
      
      /*
       * return an const reverse iterator to the last element of the vector
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator to the last element
       */
      TFEL_MATH_INLINE2
      const_reverse_iterator
      rbegin(void) const;
      
      /*
       * return an  reverse iterator before the first element of the vector
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator before the first element
       */
      TFEL_MATH_INLINE2
      reverse_iterator
      rend(void);
      
      /*
       * return an const reverse iterator before the first element of the vector
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator before the first element
       */
      TFEL_MATH_INLINE2
      const_reverse_iterator
      rend(void) const;

      
      template<typename InputIterator>
      TFEL_MATH_INLINE2 
      void 
      copy(const InputIterator src);

    };
        
  } // end of namespace math

  namespace typetraits{
    
    template<unsigned short N,typename T2,
	     template<unsigned short,typename> class Storage2,
	     typename T,template<unsigned short,typename> class Storage >
    struct IsAssignableTo<tfel::math::stensor<N,T2,Storage2>,
			  tfel::math::stensor<N,T,Storage> >
    {
      /*!
       *  Result
       */
      static const bool cond = IsAssignableTo<T2,T>::cond;
    };

  } // end of namespace typetraits

} // end of namespace tfel

#include"Math/Stensor/StensorSizeToDime.hxx"
#include"Math/Stensor/stensor.ixx"
#include"Math/Stensor/stensorResultType.hxx"

#endif /* _LIB_TFEL_STENSOR_H */

