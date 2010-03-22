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

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/TypeTraits/IsAssignableTo.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"
#include"TFEL/Math/Vector/VectorConceptOperations.hxx"

namespace tfel{

  namespace math{
    
    /*!
     * \class vector
     * \brief  A class representing resizable medium sized vector.
     * \param  ValueType, the underlying numerical type.
     * \author Helfer Thomas
     * \date   04 April 2007
     */
    template<typename ValueType>
    class vector;
    
    /*!
     * \brief Partial specialisation for the vector class.
     * \see VectorTraits
     */
    template<typename ValueType>
    struct TFEL_VISIBILITY_LOCAL VectorTraits<vector<ValueType> >{
      /*
       * The underlying numerical type.
       */
      typedef ValueType NumType;
      /*
       * The index type
       */
      typedef typename std::vector<ValueType>::size_type IndexType;
      /*
       * RunTime Properties
       */
      typedef IndexType RunTimeProperties;
    };
    
    
    template<typename ValueType>
    class vector
      : public VectorConcept<vector<ValueType> >,
	protected std::vector<ValueType>
    {
      /*
       * Typedef to the underlying storage class.
       */
      typedef std::vector<ValueType> Container;

    public:

      /*!
       * type of the vector's values.
       * (this i<s a stl requirement).
       */
      using Container::value_type;
      /*!
       * type of a pointer to the value contained.
       * (this is a stl requirement).
       */
      using Container::pointer;
      /*!
       * type of a const pointer to the value contained.
       * (this is a stl requirement).
       */
      using Container::const_pointer;
      /*!
       * type of the vector's iterator.
       * (provided for stl compatibility).
       */
      using Container::iterator;
      /*!
       * type of the vector's const iterator.
       * (provided for stl compatibility).
       */
      using Container::const_iterator;
      /*!
       * type of the vector's reverse iterator.
       * (provided for stl compatibility).
       */
      using Container::const_reverse_iterator;
      /*!
       * type of the vector's const reverse iterator.
       * (provided for stl compatibility).
       */
      using Container::reverse_iterator;
      /*!
       * type of a reference to the value contained.
       * (this is a stl requirement).
       */
      using Container::reference;
      /*!
       * type of a const reference to the value contained.
       * (this is a stl requirement).
       */
      using Container::const_reference;
      /*!
       * type of the size of the container.
       * (this is a stl requirement).
       */
      typedef typename Container::size_type size_type;
      /*!
       * type of the difference between two iterators.
       * (this is a stl requirement).
       */
      using Container::difference_type;
      /*!
       * type of the runtime properties of the vector.
       * (this is a vector concept requirement).
       */
      typedef size_type RunTimeProperties;

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
	return string("tfel::math::vector<")+Name<ValueType>::getName()+string(">");
      }

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
       * \param ValueType value, initial value 
       */
      vector(const size_type, const ValueType&);

      /*
       * Constructor from a sequence
       * \param InputIterator, type of the iterator used
       * \param InputIterator begin, beginning of the sequence
       * \param InputIterator end, end of the sequence
       */
      template<typename InputIterator>
      vector(const InputIterator,const InputIterator);

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
       * \param const VectorExpr<vector<ValueType2>,Expr>&, a vector
       * expression based on vector
       * \return vector&, a reference to itself.
       */
      template<typename ValueType2,typename Expr>
      TFEL_MATH_INLINE2
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<ValueType2,ValueType>::cond,
	vector<ValueType>&
      >::type
      operator=(const VectorExpr<vector<ValueType2>,Expr>&);

      /*
       * Assignement operator
       * \param const VectorExpr<vector<ValueType2>,Expr>&, a vector
       * expression based on vector
       * \return vector&, a reference to itself.
       */
      template<typename ValueType2,typename Expr>
      TFEL_MATH_INLINE2
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<ValueType2,ValueType>::cond,
	vector<ValueType>&
      >::type
      operator+=(const VectorExpr<vector<ValueType2>,Expr>&);

      /*
       * Assignement operator
       * \param const VectorExpr<vector<ValueType2>,Expr>&, a vector
       * expression based on vector
       * \return vector&, a reference to itself.
       */
      template<typename ValueType2,typename Expr>
      TFEL_MATH_INLINE2
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<ValueType2,ValueType>::cond,
	vector<ValueType>&
      >::type
      operator-=(const VectorExpr<vector<ValueType2>,Expr>&);

      /*
       * index operator
       * \param size_type, index
       * \return ValueType&, a reference to the vector ith element.
       */
      TFEL_MATH_INLINE
      ValueType&
      operator()(const size_type);

      /*
       * index operator
       * this is a vector concept requirement
       * \param size_type, index
       * \return const ValueType&, a reference to the vector ith element
       */
      TFEL_MATH_INLINE
      const ValueType&
      operator()(const size_type) const;

      using std::vector<ValueType>::operator[];

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

    using Container::push_back;
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

