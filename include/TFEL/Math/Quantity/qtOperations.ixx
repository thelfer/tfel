/*!
 * \file   include/TFEL/Math/Quantity/qtOperations.ixx
 * \brief  This file implements usual operations on quantities.
 * \author Thomas Helfer
 * \date   07 Jun 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QTOPERATIONS_IXX
#define LIB_TFEL_MATH_QTOPERATIONS_IXX

namespace tfel::math {

  // Multiplication by a scalar
#define TFEL_MATH_QT_RESULT_TYPE_IMPL(X)                                       \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr qt<UnitType,                                                       \
               typename tfel::typetraits::Promote<ValueType, X>::type>         \
  operator*(const Quantity<UnitType, ValueType, OwnershipPolicy>& a,           \
            const X b) {                                                       \
    typedef qt<UnitType,                                                       \
               typename tfel::typetraits::Promote<ValueType, X>::type>         \
        result;                                                                \
    return result((a.getValue()) * b);                                         \
  }                                                                            \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr qt<UnitType,                                                       \
               typename tfel::typetraits::Promote<ValueType, X>::type>         \
  operator*(const X b,                                                         \
            const Quantity<UnitType, ValueType, OwnershipPolicy>& a) {         \
    typedef qt<UnitType,                                                       \
               typename tfel::typetraits::Promote<ValueType, X>::type>         \
        result;                                                                \
    return result(b * (a.getValue()));                                         \
  }                                                                            \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr qt<UnitType,                                                       \
               typename tfel::typetraits::Promote<ValueType, X>::type>         \
  operator/(const Quantity<UnitType, ValueType, OwnershipPolicy>& a,           \
            const X b) {                                                       \
    typedef qt<UnitType,                                                       \
               typename tfel::typetraits::Promote<ValueType, X>::type>         \
        result;                                                                \
    return result((a.getValue()) / b);                                         \
  }                                                                            \
                                                                               \
  template <typename ValueType, typename UnitType, typename OwnershipPolicy>   \
  constexpr qt<                                                                \
      typename tfel::math::internals::SubstractUnit<NoUnit, UnitType>::type,   \
      typename tfel::typetraits::Promote<ValueType, X>::type>                  \
  operator/(const X b,                                                         \
            const Quantity<UnitType, ValueType, OwnershipPolicy>& a) {         \
    typedef qt<                                                                \
        typename tfel::math::internals::SubstractUnit<NoUnit, UnitType>::type, \
        typename tfel::typetraits::Promote<ValueType, X>::type>                \
        result;                                                                \
    return result(b / (a.getValue()));                                         \
  }

  TFEL_MATH_QT_RESULT_TYPE_IMPL(unsigned short)
  TFEL_MATH_QT_RESULT_TYPE_IMPL(unsigned int)
  TFEL_MATH_QT_RESULT_TYPE_IMPL(long unsigned int)
  TFEL_MATH_QT_RESULT_TYPE_IMPL(short)
  TFEL_MATH_QT_RESULT_TYPE_IMPL(int)
  TFEL_MATH_QT_RESULT_TYPE_IMPL(long int)
  TFEL_MATH_QT_RESULT_TYPE_IMPL(float)
  TFEL_MATH_QT_RESULT_TYPE_IMPL(double)
  TFEL_MATH_QT_RESULT_TYPE_IMPL(long double)

}  // namespace tfel::math

#endif /* LIB_TFEL_MATH_QTOPERATIONS_IXX */
