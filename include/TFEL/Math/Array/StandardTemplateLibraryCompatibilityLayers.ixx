/*!
 * \file  include/TFEL/Math/Array/StandardTemplateLibraryCompatibilityLayers.ixx
 * \brief
 * \author Thomas Helfer
 * \date 01/01/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
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
      ArrayPolicy>::const_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::begin()
      const noexcept {
    return this->cbegin();
  }  // end of begin

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child,
                                                    ArrayPolicy>::cbegin() const
      noexcept {
    return static_cast<const Child&>(*this).data();
  }  // end of begin

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::end()
      const noexcept {
    return this->cend();
  }  // end of end

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::cend()
      const noexcept {
    return static_cast<const Child&>(*this).data() +
           static_cast<const Child&>(*this).size();
  }  // end of end

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_reverse_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child,
                                                    ArrayPolicy>::rbegin() const
      noexcept {
    return this->crbegin();
  }  // end of rbegin

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_reverse_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child,
                                                    ArrayPolicy>::crbegin()
      const noexcept {
    return const_reverse_iterator(this->cend());
  }  // end of rbegin

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_reverse_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::rend()
      const noexcept {
    return this->crend();
  }  // end of rend

  template <typename Child, typename ArrayPolicy>
  constexpr typename ConstArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::const_reverse_iterator
  ConstArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::crend()
      const noexcept {
    return const_reverse_iterator(this->cbegin());
  }  // end of rend

  template <typename Child, typename ArrayPolicy>
  constexpr typename MutableArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::iterator
  MutableArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::
      begin() noexcept {
    return static_cast<Child&>(*this).data();
  }  // end of begin

  template <typename Child, typename ArrayPolicy>
  constexpr typename MutableArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::iterator
  MutableArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::
      end() noexcept {
    return static_cast<Child&>(*this).data() +
           static_cast<Child&>(*this).size();
  }  // end of end

  template <typename Child, typename ArrayPolicy>
  constexpr typename MutableArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::reverse_iterator
  MutableArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::
      rbegin() noexcept {
    return reverse_iterator(this->end());
  }  // end of rbegin

  template <typename Child, typename ArrayPolicy>
  constexpr typename MutableArrayWithContiguousDataSTLCompatibilityLayer<
      Child,
      ArrayPolicy>::reverse_iterator
  MutableArrayWithContiguousDataSTLCompatibilityLayer<Child, ArrayPolicy>::
      rend() noexcept {
    return reverse_iterator(this->begin());
  }  // end of rend

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_STANDARDTEMPLATELIBRARYCOMPATIBILITYLAYERS_IXX \
        */
