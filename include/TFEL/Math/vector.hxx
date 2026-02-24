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

#ifndef LIB_TFEL_MATH_VECTOR_HXX
#define LIB_TFEL_MATH_VECTOR_HXX

#include <type_traits>
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/Forward/vector.hxx"
#include "TFEL/Math/Array/GenericRuntimeArray.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/Vector/VectorConceptOperations.hxx"

namespace tfel::math {

  template <typename ValueType>
  struct vector : VectorConceptBase<vector<ValueType>>,
                  GenericRuntimeArray<vector<ValueType>,
                                      RuntimeVectorArrayPolicy<ValueType>> {
    //! \brief a simple alias
    using GenericRuntimeArrayBase =
        GenericRuntimeArray<vector, RuntimeVectorArrayPolicy<ValueType>>;
    // inheriting constructors
    TFEL_MATH_RUNTIME_ARRAY_DEFAULT_METHODS(vector, GenericRuntimeArrayBase);
    //!
    vector(const typename vector::size_type, const ValueType& = ValueType{});
    //!
    template <typename InputIterator>
    vector(const InputIterator, const InputIterator);
    // inheriting GenericRuntimeArray' access operators
    using GenericRuntimeArrayBase::operator[];
    using GenericRuntimeArrayBase::operator();
    //!
    void push_back(const ValueType&);
    //
    using GenericRuntimeArrayBase::resize;
    //! \brief resize the array
    void resize(const typename vector::size_type,
                const ValueType& = ValueType{});
    //!
    template <typename InputIterator>
    void insert(const typename vector::const_iterator,
                const InputIterator,
                const InputIterator);
    /*!
     * \brief swap two vectors
     * \param[in,out] the other vector
     */
    void swap(vector&);
    /*!
     * copy all the elements between two iterators at the beginning
     * of the vector.
     * \param const InputIterator, an iterator to the first element
     * to be copied.
     * \param const InputIterator, an iterator to the first element
     * not to be copied.
     */
    template <typename InputIterator>
    void copy(const InputIterator, const InputIterator);
    //! \brief destructor
    ~vector() noexcept;
  };

  /*!
   * \return the euclidian norm of a vector
   * \param v: the vector.
   */
  template <ScalarConcept T>
  TFEL_HOST_DEVICE auto norm(const vector<T>&) noexcept;

}  // end of namespace tfel::math

namespace tfel::typetraits {

  //! \brief partial specialisation for vectors
  template <typename ValueType, typename ValueType2>
  struct IsAssignableTo<tfel::math::vector<ValueType>,
                        tfel::math::vector<ValueType2>> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<ValueType, ValueType2>();
  };

}  // end of namespace tfel::typetraits

#include "TFEL/Math/Vector/vector.ixx"

#endif /* LIB_TFEL_MATH_VECTOR_HXX */
