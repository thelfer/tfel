/*!
 * \file   include/TFEL/Math/vector.hxx
 * \brief  this class declares the vector template class which is base
 * on STL'vector storage class. This classe is suitable for describing
 * medium sized vectors.
 * \author Thomas Helfer
 * \date   05 oct 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_VECTOR_HXX
#define LIB_TFEL_MATH_VECTOR_HXX

#include <vector>
#include <type_traits>
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/Forward/vector.hxx"
#include "TFEL/Math/Array/GenericRuntimeArray.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/Vector/VectorConceptOperations.hxx"

namespace tfel::math {

  template <typename ValueType>
  struct vector : VectorConcept<vector<ValueType>>,
                  GenericRuntimeArray<vector<ValueType>,
                                      RuntimeVectorArrayPolicy<ValueType>> {
    //! \brief a simple alias
    using GenericRuntimeArrayBase =
        GenericRuntimeArray<vector<ValueType>,
                            RuntimeVectorArrayPolicy<ValueType>>;
    //
    TFEL_MATH_RUNTIME_ARRAY_DEFAULT_METHODS(vector, GenericRuntimeArrayBase);
    // inheriting GenericRuntimeArray' access operators
    using GenericRuntimeArrayBase::operator[];
    using GenericRuntimeArrayBase::operator();
    //     /*!
    //      * Constructor from a sequence
    //      * \param[in] b: beginning of the sequence
    //      * \param[in] e: end of the sequence
    //      */
    //     template <typename InputIterator>
    //     vector(const InputIterator, const InputIterator);

    //     using Container::back;
    //     using Container::begin;
    //     using Container::cbegin;
    //     using Container::cend;
    //     using Container::clear;
    //     using Container::data;
    //     using Container::emplace_back;
    //     using Container::empty;
    //     using Container::end;
    //     using Container::front;
    //     using Container::insert;
    //     using Container::push_back;
    //     using Container::rbegin;
    //     using Container::rend;
    //     using Container::resize;
    //     using Container::size;
    //     /*!
    //      * \brief swap two vectors
    //      * \param vector&, the other vector
    //      */
    //     void swap(vector&);
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
    //! \brief destructor
    ~vector() noexcept;
  };

  /*!
   * \return the euclidian norm of a vector
   * \param v: the vector.
   */
  template <typename T>
  std::enable_if_t<isScalar<T>(),
                   typename tfel::typetraits::RealPartType<T>::type>
  norm(const vector<T>&);

}  // end of namespace tfel::math

#include "TFEL/Math/Vector/vector.ixx"

#endif /* LIB_TFEL_MATH_VECTOR_HXX */
