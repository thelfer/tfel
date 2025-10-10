/*!
 * \file  include/TFEL/Math/Array/StandardTemplateLibraryCompatibilityLayers.hxx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ARRAY_STANDARDTEMPLATELIBRARYCOMPATIBILITYLAYERS_HXX
#define LIB_TFEL_MATH_ARRAY_STANDARDTEMPLATELIBRARYCOMPATIBILITYLAYERS_HXX

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace tfel::math {

  /*!
   * \brief partial specialisation of the `ArraySTLCompatibilityLayer`
   * class when the memoy layout is contiguous.
   * \tparam Child: child class
   * \tparam ValueType: value hold by the array
   */
  template <typename Child, typename ArrayPolicy>
  struct ConstArrayWithContiguousDataSTLCompatibilityLayer {
    /*!
     * \brief const type of the values hold by the array.
     */
    using const_value_type = const typename ArrayPolicy::value_type;
    /*!
     * \brief type of a const reference to the value contained.
     * \note this alias is defined for compatibility with the standard
     * template library (STL).
     */
    using const_reference = const typename ArrayPolicy::const_reference&;
    /*!
     * \brief type of a const pointer to the value contained.
     * \note this alias is defined for compatibility with the standard
     * template library (STL).
     */
    using const_pointer = const typename ArrayPolicy::storage_type*;
    /*!
     * \brief type of the difference between two iterators.
     * \note this alias is defined for compatibility with the standard
     * template library (STL).
     */
    using difference_type = ptrdiff_t;
    /*!
     * \brief type of the const iterator.
     * \note this alias is defined for compatibility with the standard
     * template library (STL).
     */
    using const_iterator = const_pointer;
    /*!
     * \brief type of the reverse iterator.
     * \note this alias is defined for compatibility with the standard
     * template library (STL).
     */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    //!
    TFEL_HOST_DEVICE constexpr const_reference front() const noexcept;
    //!
    TFEL_HOST_DEVICE constexpr const_reference back() const noexcept;
    //! \return an const iterator to the first element of the array.
    TFEL_HOST_DEVICE constexpr const_iterator begin() const noexcept;
    //! \return an const iterator to the first element of the array.
    TFEL_HOST_DEVICE constexpr const_iterator cbegin() const noexcept;
    //! \return an const iterator after the last element of the array.
    TFEL_HOST_DEVICE constexpr const_iterator end() const noexcept;
    //! \return an const iterator after the last element of the array.
    TFEL_HOST_DEVICE constexpr const_iterator cend() const noexcept;
    /*!
     * \return an const reverse iterator past the last element of the
     * array.
     */
    TFEL_HOST_DEVICE constexpr const_reverse_iterator rbegin() const noexcept;
    /*!
     * \return an const reverse iterator past the last element of the
     * array.
     */
    TFEL_HOST_DEVICE constexpr const_reverse_iterator crbegin() const noexcept;
    /*!
     * \return an const reverse iterator before the first element of the
     * array.
     */
    TFEL_HOST_DEVICE constexpr const_reverse_iterator rend() const noexcept;
    /*!
     * \return an const reverse iterator before the first element of the
     * array.
     */
    TFEL_HOST_DEVICE constexpr const_reverse_iterator crend() const noexcept;
  };  // end of struct ConstArrayWithContiguousDataSTLCompatibilityLayer

  template <typename Child, typename ArrayPolicy>
  struct ConstArrayWithoutContiguousDataSTLCompatibilityLayer {};

  /*!
   * \brief base class providing standard methods and aliases for
   * compatibility with the standard template library (STL) based on the
   * curiously recurring template pattern (CRTP).
   * \tparam Child: child class
   * \tparam ValueType: value hold by the array
   */
  template <typename Child, typename ArrayPolicy>
  struct ConstArraySTLCompatibilityLayer
      : std::conditional_t<
            ArrayPolicy::IndexingPolicy::areDataContiguous,
            ConstArrayWithContiguousDataSTLCompatibilityLayer<Child,
                                                              ArrayPolicy>,
            ConstArrayWithoutContiguousDataSTLCompatibilityLayer<Child,
                                                                 ArrayPolicy>> {
  };  // end of ArraySTLCompatibilityLayer;

  /*!
   * \brief partial specialisation of the `ArraySTLCompatibilityLayer`
   * class when the memoy layout is contiguous. \tparam Child: child class
   * \tparam ValueType: value hold by the array
   */
  template <typename Child, typename ArrayPolicy>
  struct MutableArrayWithContiguousDataSTLCompatibilityLayer
      : ConstArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy> {
    /*!
     * \brief type of the values hold by the array.
     * \note this alias is defined for compatibility with the standard
     * template library (STL).
     */
    using value_type = typename ArrayPolicy::value_type;
    /*!
     * \brief type of a reference to the value contained.
     * \note this alias is defined for compatibility with the standard
     * template library (STL).
     */
    using reference = typename ArrayPolicy::reference;
    /*!
     * \brief type of a pointer to the value contained.
     * \note this alias is defined for compatibility with the standard
     * template library (STL).
     */
    using pointer = typename ArrayPolicy::storage_type*;
    /*!
     * \brief type of the iterator.
     * \note this alias is defined for compatibility with the standard
     * template library (STL).
     */
    using iterator = pointer;
    /*!
     * \brief type of the const reverse iterator.
     * \note this alias is defined for compatibility with the standard
     * template library (STL).
     */
    using reverse_iterator = std::reverse_iterator<iterator>;
    //
    using ConstArrayWithContiguousDataSTLCompatibilityLayer<Child,
                                                            ArrayPolicy>::front;
    //
    TFEL_HOST_DEVICE constexpr reference front() noexcept;
    //
    using ConstArrayWithContiguousDataSTLCompatibilityLayer<Child,
                                                            ArrayPolicy>::back;
    //
    TFEL_HOST_DEVICE constexpr reference back() noexcept;
    //
    using ConstArrayWithContiguousDataSTLCompatibilityLayer<Child,
                                                            ArrayPolicy>::begin;
    //
    using ConstArrayWithContiguousDataSTLCompatibilityLayer<Child,
                                                            ArrayPolicy>::end;
    //! \return an iterator to the first element of the array
    TFEL_HOST_DEVICE constexpr iterator begin() noexcept;
    //! \return an iterator after the last element of the array.
    TFEL_HOST_DEVICE constexpr iterator end() noexcept;
    //
    using ConstArrayWithContiguousDataSTLCompatibilityLayer<
        Child,
        ArrayPolicy>::rbegin;
    //
    using ConstArrayWithContiguousDataSTLCompatibilityLayer<Child,
                                                            ArrayPolicy>::rend;
    //! \return an reverse iterator to the last element of the array.
    TFEL_HOST_DEVICE constexpr reverse_iterator rbegin() noexcept;
    /*!
     * \return an  reverse iterator before the first element of the
     * array.
     */
    TFEL_HOST_DEVICE constexpr reverse_iterator rend() noexcept;
  };  // end of struct MutableArrayWithContiguousDataSTLCompatibilityLayer

  template <typename Child, typename ArrayPolicy>
  struct MutableArrayWithoutContiguousDataSTLCompatibilityLayer
      : ConstArrayWithoutContiguousDataSTLCompatibilityLayer<Child,
                                                             ArrayPolicy> {};

  /*!
   * \brief base class providing standard methods and aliases for
   * compatibility with the standard template library (STL) based on the
   * curiously recurring template pattern (CRTP).
   * \tparam Child: child class
   * \tparam ValueType: value hold by the array
   */
  template <typename Child, typename ArrayPolicy>
  struct MutableArraySTLCompatibilityLayer
      : std::conditional_t<
            ArrayPolicy::IndexingPolicy::areDataContiguous,
            MutableArrayWithContiguousDataSTLCompatibilityLayer<Child,
                                                                ArrayPolicy>,
            MutableArrayWithoutContiguousDataSTLCompatibilityLayer<
                Child,
                ArrayPolicy>> {};  // end of ArraySTLCompatibilityLayer;

}  // end of namespace tfel::math

#include "TFEL/Math/Array/StandardTemplateLibraryCompatibilityLayers.ixx"

#endif /* LIB_TFEL_MATH_ARRAY_STANDARDTEMPLATELIBRARYCOMPATIBILITYLAYERS*/
