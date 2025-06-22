/*!
 * \file  include/TFEL/Math/Forward/fsarray.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 19 sept. 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_FSARRAY_HXX
#define LIB_TFEL_MATH_FORWARD_FSARRAY_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"

namespace tfel::math {

  //! \brief a class representing array of dynamic size.
  template <typename ValueType>
  struct runtime_array;

  /*!
   * \brief Partial specialisation for `runtime_array`.
   * \see MathObjectTraits.
   */
  template <typename ValueType>
  struct MathObjectTraits<runtime_array<ValueType>>
      : MathObjectTraitsFromArrayPolicy<RuntimeVectorArrayPolicy<ValueType>> {
    //! brief a `runtime_array` is not related to the space dimension
    static constexpr unsigned short dime = 0;
  };  // end of struct MathObjectTraits<runtime_array<ValueType>>

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FORWARD_FSARRAY_HXX */
