/*!
 * \file   tvector.hxx  
 * \brief    
 * \author Helfer Thomas
 */

#ifndef _LIB_TFEL_TINY_VECTOR_
#define _LIB_TFEL_TINY_VECTOR_ 1

#include<string>
#include<ostream>
#include<iterator>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Utilities/ToString.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/TypeTraits/IsAssignableTo.hxx"
#include"TFEL/TypeTraits/RealPartType.hxx"
#include"TFEL/FSAlgorithm/copy.hxx"

#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include"TFEL/Math/Vector/VectorConcept.hxx"
#include"TFEL/Math/Vector/VectorConceptOperations.hxx"
#include"TFEL/Math/Vector/VectorExpr.hxx"

namespace tfel{
  
  namespace math{

    /*!
     * \class tvector.
     * \brief a class representing tiny vectors of fixed size.
     * \param unsigned short, the number of values hold by the
     * tvector.
     * \param typename T, the type hold by the tvector.
     */
    template<unsigned short N, typename T>
    class tvector;

    template<unsigned short N,
	     unsigned short M,
	     typename T>
    class tmatrix;

    //! Partial specialisation for tvectors.
    /*
     * This is a VectorConcept requirement.
     * \see VectorTraits.
     */
    template<unsigned short N, typename T>
    struct VectorTraits<tvector<N,T> >{
      //! the type holded by the tvector.
      typedef T NumType;
      //! the type of the index used by the tvector.
      typedef unsigned short IndexType;
      //! a simple alias to the tvector runtime properties
      typedef EmptyRunTimeProperties RunTimeProperties;
    };
    
    template<unsigned short N, typename T = double>
    class tvector
      : public VectorConcept<tvector<N,T> >
    {

      //! a simple assertion stating that the dimension is valid.
      TFEL_STATIC_ASSERT(N!=0);
      
    protected:
      // values hold by the tvector.
      T v[N];
      
    public:

      //! a simple typedef to the tvector runtime properties
      /*
       * This is a VectorConcept requirement.
       */
      typedef EmptyRunTimeProperties RunTimeProperties;

      /*!
       * type of the tvector's values.
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
       * type of the tvector's iterator.
       * (provided for stl compatibility).
       */
      typedef pointer iterator;
      /*!
       * type of the tvector's const iterator.
       * (provided for stl compatibility).
       */
      typedef const_pointer const_iterator;
      /*!
       * type of the tvector's reverse iterator.
       * (provided for stl compatibility).
       */
#ifdef __GNUG__
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator; 
#else
      typedef std::reverse_iterator<const_iterator,T,
      				    const_reference,
      				    difference_type> const_reverse_iterator;
#endif
      /*!
       * type of the tvector's const reverse iterator.
       * (provided for stl compatibility).
       */
#ifdef __GNUG__
      typedef std::reverse_iterator<iterator> reverse_iterator;
#else
      typedef std::reverse_iterator<iterator,T,
      				    reference,
      				    difference_type> reverse_iterator;
#endif

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static 
      const std::string 
      getName(void)
      {
	using namespace std;
	using namespace tfel::utilities;
	return string("tvector<")+ToString(N)+string(",")+Name<T>::getName()+string(">");
      }

      /*
       * Default constructor.
       */
      tvector();
      
      /*
       * Constructor from a scalar.
       * \param const T : initial value.
       */
      explicit
      tvector(const T);

      /*
       * Constructor from a pointer.
       * \param const T* : initial values.
       */
      explicit
      tvector(const T*const);

      //! Assignement operator
      /*
       * \param const VectorExpr<tvector<N,T2>,Expr>&, a vector expression.
       * \return tvector<N,T>& a reference to this.
       * \rec T2 must be assignable to a T.
       */
      template<typename T2,typename Expr>
      tvector(const VectorExpr<tvector<N,T2>,Expr>&);
      
      /*!
       * \brief index operator.
       * This is a vector concept requirement.
       * \param const unsigned short, index.
       * \return const T&, a reference to the tvector ith element.
       */
      TFEL_MATH_INLINE
      const T& 
      operator()(const unsigned short) const;
      
      /*!
       * \brief index operator.
       * \param const unsigned short, index.
       * \return T&, a reference to the tvector ith element.
       */
      TFEL_MATH_INLINE
      T& operator()(const unsigned short);

      /*!
       * \brief index operator.
       * This is a vector concept requirement.
       * \param const unsigned short, index.
       * \return const T&, a reference to the tvector ith element.
       */
      TFEL_MATH_INLINE
      const T& 
      operator[](const unsigned short) const;
      
      /*!
       * \brief index operator.
       * \param const unsigned short, index.
       * \return T&, a reference to the tvector ith element.
       */
      TFEL_MATH_INLINE
      T& operator[](const unsigned short);
      
      //! Assignement operator
      /*
       * \param const VectorExpr<tvector<N,T2>,Expr>&, a vector expression.
       * \return tvector<N,T>& a reference to this.
       * \rec T2 must be assignable to a T.
       */
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tvector<N,T>&
	>::type
      operator=(const VectorExpr<tvector<N,T2>,Expr>&);

      // Assignement operator
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tvector<N,T>&
	>::type
      operator+=(const VectorExpr<tvector<N,T2>,Expr>&);

      // Assignement operator
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tvector<N,T>&
	>::type
      operator-=(const VectorExpr<tvector<N,T2>,Expr>&);
      
      // Assignement operator
      template<typename T2>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tvector<N,T>&
	>::type
      operator=(const tvector<N,T2>&);
    
      // Assignement operator
      template<typename T2>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tvector<N,T>&
	>::type
      operator+=(const tvector<N,T2>&);
    
      // Assignement operator
      template<typename T2>
      TFEL_MATH_INLINE
      typename 
      tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	tvector<N,T>&
	>::type
      operator-=(const tvector<N,T2>&);
    
      /*!
       * operator*=
       */
      template<typename T2>
      TFEL_MATH_INLINE 
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	tvector<N,T>&
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
	tvector<N,T>&
      >::type
      operator/=(const T2);

      /*
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      const RunTimeProperties
      getRunTimeProperties(void) const;

      /*
       * return an iterator to the first element of the vector
       * (provided for stl compatibility)
       * \return iterator, an iterator to the first element
       */
      TFEL_MATH_INLINE
      iterator
      begin(void);
      
      /*
       * return an const iterator to the first element of the vector
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator to the first element
       */
      TFEL_MATH_INLINE
      const_iterator 
      begin(void) const;

      /*
       * return an iterator after the last element of the vector
       * (provided for stl compatibility)
       * \return iterator, an iterator after the last element
       */
      TFEL_MATH_INLINE
      iterator
      end(void);
      
      /*
       * return an const iterator after the last element of the vector
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator after the last element
       */
      TFEL_MATH_INLINE
      const_iterator
      end(void) const;

      /*
       * return an reverse iterator to the last element of the vector
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator to the last element
       */
      TFEL_MATH_INLINE
      reverse_iterator
      rbegin(void);
      
      /*
       * return an const reverse iterator to the last element of the vector
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator to the last element
       */
      TFEL_MATH_INLINE
      const_reverse_iterator
      rbegin(void) const;
      
      /*
       * return an  reverse iterator before the first element of the vector
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator before the first element
       */
      TFEL_MATH_INLINE
      reverse_iterator
      rend(void);
      
      /*
       * return an const reverse iterator before the first element of the vector
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator before the first element
       */
      TFEL_MATH_INLINE
      const_reverse_iterator
      rend(void) const;

      /*
       * copy the Nth elements following this argument.
       * \param const InputIterator, an iterator to the first element
       * to be copied.
       */
      template<typename InputIterator>
      void 
      copy(const InputIterator src);

      unsigned short
      size(void) const;

    };

    //! Serialisation operator
    /*
     * \param  std::ostream&, stream to which the vector is serialized.
     * \param  const tvector<N,T>&, the tvector serialized.
     * \return std::ostream&, stream to which the vector has been serialized.
     */
    template<unsigned short N,typename T>
    std::ostream&
    operator << (std::ostream &, const tvector<N,T>&);

    /*!
     * \brief return the euclidian norm of a tvector
     * \param const tvector<N,T>&, the vector.
     * \return const typename tfel::typetraits::RealPartType<T>::type, the result
     */
    template<unsigned short N,typename T>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond,
      typename tfel::typetraits::RealPartType<T>::type
      >::type
    norm(const tvector<N,T>&);

  } // end of namespace math

  namespace typetraits{
    
    /*
     * Partial specialisation for tvectors
     */
    template<unsigned short N,typename T2,typename T>
    struct IsAssignableTo<tfel::math::tvector<N,T2>,tfel::math::tvector<N,T> >
    {
      /*!
       *  Result
       */
      static const bool cond = IsAssignableTo<T2,T>::cond;
    };

  } // end of namespace typetraits

} // end of namespace tfel

#include"TFEL/Math/Vector/tvector.ixx"
#include"TFEL/Math/Vector/tvectorResultType.hxx"

#endif /* _LIB_TFEL_TINY_VECTOR_ */
