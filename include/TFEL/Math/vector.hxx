/*!
 * \file   vector.hxx
 * \brief  this class declares the vector template class which is base
 * on STL'vector storage class. This classe is suitable for describing
 * medium sized vectors.
 * \author Helfer Thomas
 * \date   05 oct 2006
 */

#ifndef _LIB_TFEL_VECTOR_HXX_
#define _LIB_TFEL_VECTOR_HXX_ 

#include<vector>
#include<string>

#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"
#include"TFEL/TypeTraits/IsAssignableTo.hxx"

#include"TFEL/Math/Forward/vector.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"
#include"TFEL/Math/Vector/VectorConceptOperations.hxx"

namespace tfel{

  namespace math{
    
    /*!
     * \brief Partial specialisation for the vector class.
     * \see VectorTraits
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL VectorTraits<vector<T> >{
      /*
       * The underlying numerical type.
       */
      typedef T NumType;
      /*
       * The index type
       */
      typedef typename std::vector<T>::size_type IndexType;
      /*
       * RunTime Properties
       */
      typedef IndexType RunTimeProperties;
    };
    
    
    template<typename T>
    class TFEL_VISIBILITY_LOCAL vector
      : public VectorConcept<vector<T> >,
	protected std::vector<T>
    {
      /*
       * Typedef to the underlying storage class.
       */
      typedef std::vector<T> Container;

    public:

      /*!
       * type of the vector's values.
       * (this i<s a stl requirement).
       */
      using typename Container::value_type;
      /*!
       * type of a pointer to the value contained.
       * (this is a stl requirement).
       */
      using typename Container::pointer;
      /*!
       * type of a const pointer to the value contained.
       * (this is a stl requirement).
       */
      using typename Container::const_pointer;
      /*!
       * type of the vector's iterator.
       * (provided for stl compatibility).
       */
      using typename Container::iterator;
      /*!
       * type of the vector's const iterator.
       * (provided for stl compatibility).
       */
      using typename Container::const_iterator;
      /*!
       * type of the vector's reverse iterator.
       * (provided for stl compatibility).
       */
      using typename Container::const_reverse_iterator;
      /*!
       * type of the vector's const reverse iterator.
       * (provided for stl compatibility).
       */
      using typename Container::reverse_iterator;
      /*!
       * type of a reference to the value contained.
       * (this is a stl requirement).
       */
      using typename Container::reference;
      /*!
       * type of a const reference to the value contained.
       * (this is a stl requirement).
       */
      using typename Container::const_reference;
      /*!
       * type of the size of the container.
       * (this is a stl requirement).
       */
      typedef typename Container::size_type size_type;
      /*!
       * type of the difference between two iterators.
       * (this is a stl requirement).
       */
      using typename Container::difference_type;
      /*!
       * type of the runtime properties of the vector.
       * (this is a vector concept requirement).
       */
      typedef size_type RunTimeProperties;

      /*
       * Default constructor
       */
      vector();

      /*
       * Copy constructor
       */ 
      vector(const vector&);

      /*
       * Constructor
       * \param size_type size, size of vector
       */
      vector(const size_type);

      /*
       * Constructor
       * \param size_type size, size of vector
       * \param T value, initial value 
       */
      vector(const size_type, const T&);

      /*
       * Constructor from a sequence
       * \param InputIterator, type of the iterator used
       * \param InputIterator begin, beginning of the sequence
       * \param InputIterator end, end of the sequence
       */
      template<typename InputIterator>
      vector(const InputIterator,const InputIterator);

      /*
       * get the first element of the vector
       */
      using Container::front;

      /*
       * get the last element of the vector
       */
      using Container::back;

      /*
       * insert element in the vector
       */
      using Container::insert;

      /*
       * clear the vector
       */
      using Container::clear;

      /*
       * Resize the vector
       * \param size_type, new size of the vector 
       * Elements added have their default values
       */
      using Container::resize;

      /*
       * Assignement operator.
       * \param  const vector&, the vector to be copied.
       * \return vector&, a reference to itself.
       */
      vector& 
      operator=(const vector&);

      /*
       * Assignement operator.
       * \param  const vector&, the vector to be copied.
       * \return vector&, a reference to itself.
       */
      vector& 
      operator+=(const vector&);

      /*
       * Assignement operator.
       * \param  const vector&, the vector to be copied.
       * \return vector&, a reference to itself.
       */
      vector& 
      operator-=(const vector&);

      /*
       * Assignement operator
       * \param const VectorExpr<vector<T2>,Expr>&, a vector
       * expression based on vector
       * \return vector&, a reference to itself.
       */
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE2
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	vector<T>&
      >::type
      operator=(const VectorExpr<vector<T2>,Expr>&);

      /*
       * Assignement operator
       * \param const VectorExpr<vector<T2>,Expr>&, a vector
       * expression based on vector
       * \return vector&, a reference to itself.
       */
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE2
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	vector<T>&
      >::type
      operator+=(const VectorExpr<vector<T2>,Expr>&);

      /*
       * Assignement operator
       * \param const VectorExpr<vector<T2>,Expr>&, a vector
       * expression based on vector
       * \return vector&, a reference to itself.
       */
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE2
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	vector<T>&
      >::type
      operator-=(const VectorExpr<vector<T2>,Expr>&);

      /*
       * index operator
       * \param size_type, index
       * \return T&, a reference to the vector ith element.
       */
      TFEL_MATH_INLINE
      T&
      operator()(const size_type);

      /*
       * index operator
       * this is a vector concept requirement
       * \param size_type, index
       * \return const T&, a reference to the vector ith element
       */
      TFEL_MATH_INLINE
      const T&
      operator()(const size_type) const;

      using std::vector<T>::operator[];

      /*
       * Return the runtime property of the vector
       * \return const RunTimeProperties, the runtime properties of the
       * vector (its size here).
       * (this is a vector concept requirement)
       */
      TFEL_MATH_INLINE2
      const RunTimeProperties
      getRunTimeProperties(void) const;

      /*
       * get vector's size
       */
      using Container::size;
      
      /*
       * swap two vectors
       * \param vector&, the other vector
       */
      void swap(vector&);

      /*
       * return an iterator to the first element of the vector
       * (provided for stl compatibility)
       * \return iterator, an iterator to the first element
       */
      using Container::begin;
      
      /*
       * return an iterator after the last element of the vector
       * (provided for stl compatibility)
       * \return iterator, an iterator after the last element
       */
      using Container::end;

      /*
       * return an reverse iterator to the last element of the vector
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator to the last element
       */
    using Container::rbegin;
      
      /*
       * return an  reverse iterator before the first element of the vector
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator before the first element
       */
      using Container::rend;
      
      /*!
       *  \brief  add data to the end of the %vector.
       *  \param  x : data to be added.
       *
       *  This is a typical stack operation.  The function creates an
       *  element at the end of the %vector and assigns the given data
       *  to it.  Due to the nature of a %vector this operation can be
       *  done in constant time if the %vector has preallocated space
       *  available.
       */
      using Container::push_back;
      
      /*!
       *  returns true if the %vector is empty.  (Thus begin() would
       *  equal end().)
       */
      using Container::empty;
      
      /*
       * copy all the elements between two iterators at the beginning
       * of the vector.
       * \param const InputIterator, an iterator to the first element
       * to be copied.
       * \param const InputIterator, an iterator to the first element
       * not to be copied.
       */
      template<typename InputIterator>
      TFEL_MATH_INLINE2
      void 
      copy(const InputIterator,const InputIterator);

    };

    /*!
     * \brief return the euclidian norm of a vector
     * \param const vector<T>&, the vector.
     * \return const typename tfel::typetraits::RealPartType<T>::type, the result
     */
    template<typename T>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond,
      typename tfel::typetraits::RealPartType<T>::type
      >::type
    norm(const vector<T>&);

  } // end of namespace math

} // end of namespace tfel  

#include"TFEL/Math/Vector/vector.ixx"

#endif /* _LIB_TFEL_VECTOR_HXX */

