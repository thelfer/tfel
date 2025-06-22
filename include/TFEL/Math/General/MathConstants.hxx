/*!
 * \file   MathConstants.hxx
 * \brief
 * \author Thomas Helfer
 * \date   06/10/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_MATHCONSTANTS_HXX
#define LIB_TFEL_MATH_MATHCONSTANTS_HXX

#include <type_traits>
#include "TFEL/Metaprogramming/EmptyClass.hxx"
#include "TFEL/TypeTraits/IsReal.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"

namespace tfel {

  namespace math {

    template <typename T>
    struct CsteBase;

    template <typename real>
    struct CsteBase {
#ifndef _MSC_VER
      static constexpr real sqrt2 = constexpr_fct::sqrt(real{2});
      static constexpr real isqrt2 = 1 / constexpr_fct::sqrt(real{2});
      static constexpr real sqrt3 = constexpr_fct::sqrt(real{3});
      static constexpr real isqrt3 = 1 / constexpr_fct::sqrt(real{3});
#else
      static constexpr const real sqrt2 = 1.41421356237309504880;
      static constexpr const real isqrt2 = 0.70710678118654752440;
      static constexpr const real sqrt3 =
          1.7320508075688772935274463415058723669428052538103806280;
      static constexpr const real isqrt3 = 0.57735026919;
#endif
    };

    template <typename T>
    struct Cste : std::conditional<tfel::typetraits::IsReal<T>::cond,
                                   CsteBase<tfel::typetraits::base_type<T>>,
                                   tfel::meta::EmptyClass>::type {};

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_MATHCONSTANTS_HXX */
