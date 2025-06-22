/*!
 * \file   include/TFEL/Math/Quantity/qtOperations.ixx
 * \brief  This file implements usual operations on quantities.
 * \author Thomas Helfer
 * \date   07 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QTOPERATIONS_IXX
#define LIB_TFEL_MATH_QTOPERATIONS_IXX

namespace tfel {

  namespace math {

    template <typename unit, typename T1, typename T2>
    TFEL_MATH_INLINE constexpr qt<
        unit,
        typename tfel::typetraits::Promote<T1, T2>::type>
    operator+(const qt<unit, T1> a, const qt<unit, T2> b) {
      typedef
          typename ResultType<qt<unit, T1>, qt<unit, T2>, OpPlus>::type result;
      return static_cast<result>(a.getValue() + b.getValue());
    }

    template <typename unit, typename T1, typename T2>
    TFEL_MATH_INLINE constexpr qt<
        unit,
        typename tfel::typetraits::Promote<T1, T2>::type>
    operator-(const qt<unit, T1> a, const qt<unit, T2> b) {
      typedef
          typename ResultType<qt<unit, T1>, qt<unit, T2>, OpMinus>::type result;
      return result((a.getValue()) - b.getValue());
    }

    template <typename unit, typename T1, typename unit2, typename T2>
    TFEL_MATH_INLINE constexpr qt<
        typename AddUnit_<unit, unit2>::type,
        typename tfel::typetraits::Promote<T1, T2>::type>
    operator*(const qt<unit, T1> a, const qt<unit2, T2> b) {
      typedef
          typename ResultType<qt<unit, T1>, qt<unit2, T2>, OpMult>::type result;
      return result((a.getValue()) * (b.getValue()));
    }

    template <typename unit, typename T1, typename unit2, typename T2>
    TFEL_MATH_INLINE constexpr qt<
        typename SubUnit_<unit, unit2>::type,
        typename tfel::typetraits::Promote<T1, T2>::type>
    operator/(const qt<unit, T1> a, const qt<unit2, T2> b) {
      typedef
          typename ResultType<qt<unit, T1>, qt<unit2, T2>, OpDiv>::type result;
      return result((a.getValue()) / (b.getValue()));
    }

    // Multiplication by a scalar
#define TFEL_MATH_QT_RESULT_TYPE_IMPL(X)                                      \
  template <typename unit, typename T1>                                       \
  TFEL_MATH_INLINE constexpr qt<                                              \
      unit, typename tfel::typetraits::Promote<T1, X>::type>                  \
  operator*(const qt<unit, T1> a, const X b) {                                \
    typedef qt<unit, typename tfel::typetraits::Promote<T1, X>::type> result; \
    return result((a.getValue()) * b);                                        \
  }                                                                           \
                                                                              \
  template <typename unit, typename T1>                                       \
  TFEL_MATH_INLINE constexpr qt<                                              \
      unit, typename tfel::typetraits::Promote<T1, X>::type>                  \
  operator*(const X b, const qt<unit, T1> a) {                                \
    typedef qt<unit, typename tfel::typetraits::Promote<T1, X>::type> result; \
    return result(b * (a.getValue()));                                        \
  }                                                                           \
                                                                              \
  template <typename unit, typename T1>                                       \
  TFEL_MATH_INLINE constexpr qt<                                              \
      unit, typename tfel::typetraits::Promote<T1, X>::type>                  \
  operator/(const qt<unit, T1> a, const X b) {                                \
    typedef qt<unit, typename tfel::typetraits::Promote<T1, X>::type> result; \
    return result((a.getValue()) / b);                                        \
  }                                                                           \
                                                                              \
  template <typename T1, typename unit>                                       \
  TFEL_MATH_INLINE constexpr qt<                                              \
      typename SubUnit_<NoUnit, unit>::type,                                  \
      typename tfel::typetraits::Promote<T1, X>::type>                        \
  operator/(const X b, const qt<unit, T1> a) {                                \
    typedef qt<typename SubUnit_<NoUnit, unit>::type,                         \
               typename tfel::typetraits::Promote<T1, X>::type>               \
        result;                                                               \
    return result(b / (a.getValue()));                                        \
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

  }  // namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_QTOPERATIONS_IXX */
