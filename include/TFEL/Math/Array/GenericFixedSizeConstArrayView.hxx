/*!
 * \file  include/TFEL/Math/Array/GenericFixedSizeConstArrayView.hxx
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

#ifndef LIB_TFEL_MATH_ARRAY_GENERICFIXEDSIZECONSTARRAYVIEW_HXX
#define LIB_TFEL_MATH_ARRAY_GENERICFIXEDSIZECONSTARRAYVIEW_HXX

#include "TFEL/Math/Array/ConstFixedSizeArrayBase.hxx"

namespace tfel::math {

  template <typename ArrayPolicy>
  struct GenericFixedSizeConstArrayView
      : tfel::math::ConstFixedSizeArrayBase<
            GenericFixedSizeConstArrayView<ArrayPolicy>,
            ArrayPolicy> {
    //! \brief default constructor
    TFEL_HOST_DEVICE constexpr GenericFixedSizeConstArrayView(
        const typename ArrayPolicy::value_type* const p) noexcept
        : data_values(p) {}
    //! \brief copy constructor
    TFEL_HOST_DEVICE constexpr GenericFixedSizeConstArrayView(
        const GenericFixedSizeConstArrayView&) noexcept = default;
    //! \brief move constructor
    TFEL_HOST_DEVICE constexpr GenericFixedSizeConstArrayView(
        GenericFixedSizeConstArrayView&&) noexcept = default;
    //! \return a pointer to the underlying array serving as element storage.
    TFEL_HOST_DEVICE constexpr
        typename GenericFixedSizeConstArrayView::const_pointer
        data() const noexcept {
      return this->data_values;
    }  // end of data
    //! \brief destructor
    ~GenericFixedSizeConstArrayView() noexcept = default;

   protected:
    const typename ArrayPolicy::value_type* const data_values;
  };  // end of struct GenericFixedSizeConstArrayView

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ARRAY_GENERICFIXEDSIZECONSTARRAYVIEW_HXX */
