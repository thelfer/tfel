/*!
 * \file  include/TFEL/Math/matrix.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 avr 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_MATRIX_HXX
#define LIB_TFEL_MATH_MATRIX_HXX

#include <type_traits>
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/Forward/matrix.hxx"
#include "TFEL/Math/Array/GenericRuntimeArray.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"
#include "TFEL/Math/Matrix/MatrixConceptOperations.hxx"

namespace tfel::math {

  /*!
   * \brief runtime row-major matrices
   * \tparam ValueType: type of values hold by the matrix
   */
  template <typename ValueType>
  struct matrix
      : VectorConcept<matrix<ValueType>>,
        GenericRuntimeArray<matrix<ValueType>,
                            RuntimeRowMajorMatrixArrayPolicy<ValueType>> {
    //! \brief a simple alias
    using GenericRuntimeArrayBase =
        GenericRuntimeArray<matrix,
                            RuntimeRowMajorMatrixArrayPolicy<ValueType>>;
    // inheriting constructors
    TFEL_MATH_RUNTIME_ARRAY_DEFAULT_METHODS(matrix, GenericRuntimeArrayBase);
    //!
    matrix(const typename matrix::size_type,
           const typename matrix::size_type,
           const ValueType& = ValueType{});
    // inheriting GenericRuntimeArray' access operators
    using GenericRuntimeArrayBase::operator();
    //! \brief resize the matrix
    void resize(const typename matrix::size_type,
                const typename matrix::size_type,
                const ValueType& = ValueType{});
    //
    typename matrix::size_type getNbRows() const;
    //
    typename matrix::size_type getNbCols() const;
    /*!
     * \brief swap two matrixs
     * \param[in,out] the other matrix
     */
    void swap(matrix&);
    //     /*!
    //      * copy all the elements between two iterators at the beginning
    //      * of the matrix.
    //      * \param const InputIterator, an iterator to the first element
    //      * to be copied.
    //      * \param const InputIterator, an iterator to the first element
    //      * not to be copied.
    //      */
    //     template <typename InputIterator>
    //     void copy(const InputIterator, const InputIterator);
    //! \brief destructor
    ~matrix() noexcept;
  };

}  // end of namespace tfel::math

namespace tfel::typetraits {

  //! \brief partial specialisation for matrices
  template <typename ValueType, typename ValueType2>
  struct IsAssignableTo<tfel::math::matrix<ValueType>,
                        tfel::math::matrix<ValueType2>> {
    //! \brief result
    static constexpr bool cond = isAssignableTo<ValueType, ValueType2>();
  };

}  // end of namespace tfel::typetraits

#include "TFEL/Math/Matrix/matrix.ixx"

#endif /* LIB_TFEL_MATH_MATRIX_HXX */
