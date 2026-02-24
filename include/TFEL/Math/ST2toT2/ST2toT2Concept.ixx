/*!
 * \file   include/TFEL/Math/ST2toT2/ST2toT2Concept.ixx
 * \brief
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOT2_CONCEPT_IXX
#define LIB_TFEL_MATH_ST2TOT2_CONCEPT_IXX 1

namespace tfel::math {

  TFEL_HOST_DEVICE constexpr auto abs(const ST2toT2Concept auto& t) noexcept {
    using ST2toT2Type = decltype(t);
    using NumType = numeric_type<ST2toT2Type>;
    using AbsNumType = typename tfel::typetraits::AbsType<NumType>::type;
    constexpr auto tsize =
        TensorDimeToSize<getSpaceDimension<ST2toT2Type>()>::value;
    constexpr auto ssize =
        StensorDimeToSize<getSpaceDimension<ST2toT2Type>()>::value;
    auto a = AbsNumType{};
    for (unsigned short i = 0; i < tsize; ++i) {
      for (unsigned short j = 0; j < ssize; ++j) {
        a += abs(t(i, j));
      }
    }
    return a;
  }

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_ST2TOT2_CONCEPT_IXX */
