/*!
 * \file   include/TFEL/Math/vector.hxx
 * \brief  this class declares the vector template class which is base
 * on STL'vector storage class. This classe is suitable for describing
 * medium sized vectors.
 * \author Thomas Helfer
 * \date   05 oct 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_VECTOR_HXX
#define LIB_TFEL_VECTOR_HXX

#include <vector>
#include <type_traits>

#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/Forward/vector.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/Vector/VectorConceptOperations.hxx"

namespace tfel {

  namespace math {

    /*!
     * \brief Partial specialisation for the vector class.
     * \see VectorTraits
     */
    template <typename T>
    struct TFEL_VISIBILITY_LOCAL VectorTraits<vector<T>> {
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

    template <typename T>
    class TFEL_VISIBILITY_LOCAL vector : public VectorConcept<vector<T>>,
                                         protected std::vector<T> {
      //! typedef to the underlying container class.
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

      //! default constructor
      vector() = default;
      //! copy constructor
      vector(const vector&) = default;
      //! move constructor
      vector(vector&&) = default;
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
      //! constructor from initializer list
      vector(const std::initializer_list<T>&);
      /*!
       * Constructor from a sequence
       * \param[in] b: beginning of the sequence
       * \param[in] e: end of the sequence
       */
      template <typename InputIterator>
      vector(const InputIterator, const InputIterator);

      using Container::back;
      using Container::begin;
      using Container::cbegin;
      using Container::cend;
      using Container::clear;
      using Container::emplace_back;
      using Container::empty;
      using Container::end;
      using Container::front;
      using Container::insert;
      using Container::push_back;
      using Container::rbegin;
      using Container::rend;
      using Container::resize;
      using Container::size;
      using Container::operator[];
      //! assignement operator
      vector& operator=(const vector&);
      /*!
       * Assignement operator.
       * \param  const vector&, the vector to be copied.
       * \return vector&, a reference to itself.
       */
      vector& operator+=(const vector&);
      /*!
       * Assignement operator.
       * \param  const vector&, the vector to be copied.
       * \return vector&, a reference to itself.
       */
      vector& operator-=(const vector&);
      /*
       * Assignement operator
       * \param const Expr<vector<T2>,Expr>&, a vector
       * expression based on vector
       * \return vector&, a reference to itself.
       */
      template <typename T2, typename Operation>
      TFEL_MATH_INLINE2
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  vector<T>&>::type
          operator=(const Expr<vector<T2>, Operation>&);
      /*
       * Assignement operator
       * \param const Expr<vector<T2>,Operation>&, a vector
       * expression based on vector
       * \return vector&, a reference to itself.
       */
      template <typename T2, typename Operation>
      TFEL_MATH_INLINE2
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  vector<T>&>::type
          operator+=(const Expr<vector<T2>, Operation>&);
      /*
       * Assignement operator
       * \param const Expr<vector<T2>,Operation>&, a vector
       * expression based on vector
       * \return vector&, a reference to itself.
       */
      template <typename T2, typename Operation>
      TFEL_MATH_INLINE2
          typename std::enable_if<tfel::typetraits::IsAssignableTo<T2, T>::cond,
                                  vector<T>&>::type
          operator-=(const Expr<vector<T2>, Operation>&);
      /*
       * index operator
       * \param size_type, index
       * \return T&, a reference to the vector ith element.
       */
      TFEL_MATH_INLINE T& operator()(const size_type) noexcept;
      /*
       * index operator
       * this is a vector concept requirement
       * \param size_type, index
       * \return const T&, a reference to the vector ith element
       */
      TFEL_MATH_INLINE constexpr const T& operator()(
          const size_type) const noexcept;
      /*
       * Return the runtime property of the vector
       * \return const RunTimeProperties, the runtime properties of the
       * vector (its size here).
       * (this is a vector concept requirement)
       */
      TFEL_MATH_INLINE2
      const RunTimeProperties getRunTimeProperties() const;
      /*
       * swap two vectors
       * \param vector&, the other vector
       */
      void swap(vector&);
      /*
       * copy all the elements between two iterators at the beginning
       * of the vector.
       * \param const InputIterator, an iterator to the first element
       * to be copied.
       * \param const InputIterator, an iterator to the first element
       * not to be copied.
       */
      template <typename InputIterator>
      TFEL_MATH_INLINE2 void copy(const InputIterator, const InputIterator);
      //! destructor
      ~vector() noexcept;
    };

    /*!
     * \brief return the euclidian norm of a vector
     * \param const vector<T>&, the vector.
     * \return const typename tfel::typetraits::RealPartType<T>::type, the
     * result
     */
    template <typename T>
    typename std::enable_if<
        tfel::typetraits::IsScalar<T>::cond,
        typename tfel::typetraits::RealPartType<T>::type>::type
    norm(const vector<T>&);

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Vector/vector.ixx"

#endif /* LIB_TFEL_VECTOR_HXX */
