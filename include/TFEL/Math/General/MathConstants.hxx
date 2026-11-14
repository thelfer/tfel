/*!
 * \file   include/TFEL/Math/General/MathConstants.hxx
 * \brief
 * \author Thomas Helfer
 * \date 06/10/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_MATHCONSTANTS_HXX
#define LIB_TFEL_MATH_MATHCONSTANTS_HXX

#include <numbers>
#include <type_traits>
#include "TFEL/Metaprogramming/EmptyClass.hxx"
#include "TFEL/TypeTraits/IsReal.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"

namespace tfel::math {

  template <typename T>
  struct CsteBase;

  template <typename real>
  struct CsteBase {
    static constexpr real sqrt2 = std::numbers::sqrt2_v<real>;
    static constexpr real isqrt2 = sqrt2 / 2;
    static constexpr real sqrt3 = std::numbers::sqrt3_v<real>;
    static constexpr real isqrt3 = std::numbers::inv_sqrt3_v<real>;
  };

  template <typename T>
  struct Cste : std::conditional_t<tfel::typetraits::IsReal<T>::cond,
                                   CsteBase<base_type<T>>,
                                   tfel::meta::EmptyClass> {};

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_MATHCONSTANTS_HXX */
