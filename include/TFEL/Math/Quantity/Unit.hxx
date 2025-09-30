/*!
 * \file   include/TFEL/Math/Quantity/Unit.hxx
 * \brief  This file declares the classes Unit, GenerateUnit, AddUnit_,
 * SubUnit_, PowUnit_ and RootUnit_. \author Thomas Helfer \date   07 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_UNIT_HXX
#define LIB_TFEL_MATH_UNIT_HXX

#include <ratio>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Forward/qt.hxx"

namespace tfel::math {

  template <typename N1,
            typename N2,
            typename N3,
            typename N4,
            typename N5,
            typename N6,
            typename N7,
            typename D1,
            typename D2,
            typename D3,
            typename D4,
            typename D5,
            typename D6,
            typename D7>
  struct Unit;

  template <int N1,
            int N2,
            int N3,
            int N4,
            int N5,
            int N6,
            int N7,
            unsigned int D1,
            unsigned int D2,
            unsigned int D3,
            unsigned int D4,
            unsigned int D5,
            unsigned int D6,
            unsigned int D7>
  struct Unit<std::integral_constant<int, N1>,
              std::integral_constant<int, N2>,
              std::integral_constant<int, N3>,
              std::integral_constant<int, N4>,
              std::integral_constant<int, N5>,
              std::integral_constant<int, N6>,
              std::integral_constant<int, N7>,
              std::integral_constant<unsigned int, D1>,
              std::integral_constant<unsigned int, D2>,
              std::integral_constant<unsigned int, D3>,
              std::integral_constant<unsigned int, D4>,
              std::integral_constant<unsigned int, D5>,
              std::integral_constant<unsigned int, D6>,
              std::integral_constant<unsigned int, D7>> {};

}  // end of namespace tfel::math

#include "TFEL/Math/Quantity/Unit.ixx"

#endif /* LIB_TFEL_MATH_UNIT_HXX */
