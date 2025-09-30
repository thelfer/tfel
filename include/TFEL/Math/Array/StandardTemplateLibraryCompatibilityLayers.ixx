/*!
 * \file  include/TFEL/Math/Array/StandardTemplateLibraryCompatibilityLayers.ixx
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

#ifndef LIB_TFEL_MATH_ARRAY_STANDARDTEMPLATELIBRARYCOMPATIBILITYLAYERS_IXX
#define LIB_TFEL_MATH_ARRAY_STANDARDTEMPLATELIBRARYCOMPATIBILITYLAYERS_IXX

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace tfel::math {

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_reference
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::front()
      const noexcept {
    static_assert(((ArrayPolicy::IndexingPolicy::areDataContiguous) &&
                   (ArrayPolicy::IndexingPolicy::arity == 1)),
                  "invalid indexing policy");
    return *(this->cbegin());
  }  // end of front

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_reference
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::back()
      const noexcept {
    static_assert(((ArrayPolicy::IndexingPolicy::areDataContiguous) &&
                   (ArrayPolicy::IndexingPolicy::arity == 1)),
                  "invalid indexing policy");
    return *(this->crbegin());
  }  // end of back

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::begin()
      const noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::areDataContiguous,
                  "invalid indexing policy");
    return this->cbegin();
  }  // end of begin

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child,
                                                    ArrayPolicy>::cbegin()
      const noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::areDataContiguous,
                  "invalid indexing policy");
    return static_cast<const Child&>(*this).data();
  }  // end of begin

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::end()
      const noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::areDataContiguous,
                  "invalid indexing policy");
    return this->cend();
  }  // end of end

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::cend()
      const noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::areDataContiguous,
                  "invalid indexing policy");
    return static_cast<const Child&>(*this).data() +
           static_cast<const Child&>(*this).size();
  }  // end of end

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_reverse_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child,
                                                    ArrayPolicy>::rbegin()
      const noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::areDataContiguous,
                  "invalid indexing policy");
    return this->crbegin();
  }  // end of rbegin

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_reverse_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child,
                                                    ArrayPolicy>::crbegin()
      const noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::areDataContiguous,
                  "invalid indexing policy");
    return const_reverse_iterator(this->cend());
  }  // end of rbegin

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_reverse_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::rend()
      const noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::areDataContiguous,
                  "invalid indexing policy");
    return this->crend();
  }  // end of rend

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_reverse_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::crend()
      const noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::areDataContiguous,
                  "invalid indexing policy");
    return const_reverse_iterator(this->cbegin());
  }  // end of rend

  template <typename Child, typename ArrayPolicy>
  constexpr typename MutableArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::reference
  MutableArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::
      front() noexcept {
    static_assert(((ArrayPolicy::IndexingPolicy::areDataContiguous) &&
                   (ArrayPolicy::IndexingPolicy::arity == 1)),
                  "invalid indexing policy");
    return *(this->begin());
  }  // end of front

  template <typename Child, typename ArrayPolicy>
  constexpr typename MutableArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::reference
  MutableArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::
      back() noexcept {
    static_assert(((ArrayPolicy::IndexingPolicy::areDataContiguous) &&
                   (ArrayPolicy::IndexingPolicy::arity == 1)),
                  "invalid indexing policy");
    return *(this->rbegin());
  }  // end of back

  template <typename Child, typename ArrayPolicy>
  constexpr typename MutableArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::iterator
  MutableArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::
      begin() noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::areDataContiguous,
                  "invalid indexing policy");
    return static_cast<Child&>(*this).data();
  }  // end of begin

  template <typename Child, typename ArrayPolicy>
  constexpr typename MutableArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::iterator
  MutableArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::
      end() noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::areDataContiguous,
                  "invalid indexing policy");
    return static_cast<Child&>(*this).data() +
           static_cast<Child&>(*this).size();
  }  // end of end

  template <typename Child, typename ArrayPolicy>
  constexpr typename MutableArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::reverse_iterator
  MutableArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::
      rbegin() noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::areDataContiguous,
                  "invalid indexing policy");
    return reverse_iterator(this->end());
  }  // end of rbegin

  template <typename Child, typename ArrayPolicy>
  constexpr typename MutableArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::reverse_iterator
  MutableArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::
      rend() noexcept {
    static_assert(ArrayPolicy::IndexingPolicy::areDataContiguous,
                  "invalid indexing policy");
    return reverse_iterator(this->begin());
  }  // end of rend

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_STANDARDTEMPLATELIBRARYCOMPATIBILITYLAYERS_IXX \
        */
