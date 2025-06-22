/*!
 * \file   include/TFEL/Math/Quantity/qtOperations.hxx
 * \brief  This file declares usual operations on quantities.
 * \author Helfer Thomas
 * \date   07 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_QT_OPERATIONS_H_
#define LIB_TFEL_QT_OPERATIONS_H_

#include "TFEL/Math/Forward/Complex.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/Quantity/Unit.hxx"
#include "TFEL/Math/qt.hxx"

#define QT_RESULT_TYPE(X)                                                   \
  /*!                                                                       \
   * \brief Partial specialisation for qt by scalar product                 \
   * \see   ResultType                                                      \
   */                                                                       \
  template <typename unit, typename T1>                                     \
  struct ResultType<qt<unit, T1>, X, OpMult> {                              \
    typedef qt<unit, typename tfel::typetraits::Promote<T1, X>::type> type; \
  };                                                                        \
                                                                            \
  /*!                                                                       \
   * \brief Partial specialisation for scalar by qt product                 \
   * \see   ResultType                                                      \
   */                                                                       \
  template <typename unit, typename T1>                                     \
  struct ResultType<X, qt<unit, T1>, OpMult> {                              \
    typedef qt<unit, typename tfel::typetraits::Promote<T1, X>::type> type; \
  };                                                                        \
                                                                            \
  template <typename unit, typename T1>                                     \
  TFEL_MATH_INLINE constexpr qt<                                            \
      unit, typename tfel::typetraits::Promote<T1, X>::type>                \
  operator*(const qt<unit, T1>, const X);                                   \
                                                                            \
  template <typename unit, typename T1>                                     \
  TFEL_MATH_INLINE constexpr qt<                                            \
      unit, typename tfel::typetraits::Promote<T1, X>::type>                \
  operator*(const X, const qt<unit, T1>);                                   \
                                                                            \
  /*!                                                                       \
   * \brief Partial specialisation for qt by scalar division                \
   * \see   ResultType                                                      \
   */                                                                       \
  template <typename unit, typename T1>                                     \
  struct ResultType<qt<unit, T1>, X, OpDiv> {                               \
    typedef qt<unit, typename tfel::typetraits::Promote<T1, X>::type> type; \
  };                                                                        \
                                                                            \
  /*!                                                                       \
   * \brief Partial specialisation for scalar by qt division                \
   * \see   ResultType                                                      \
   */                                                                       \
  template <typename unit, typename T1>                                     \
  struct ResultType<X, qt<unit, T1>, OpDiv> {                               \
    typedef qt<typename SubUnit_<NoUnit, unit>::type,                       \
               typename tfel::typetraits::Promote<T1, X>::type>             \
        type;                                                               \
  };                                                                        \
                                                                            \
  template <typename unit, typename T1>                                     \
  TFEL_MATH_INLINE constexpr qt<                                            \
      unit, typename tfel::typetraits::Promote<T1, X>::type>                \
  operator/(const qt<unit, T1>, const X);                                   \
                                                                            \
  template <typename T1, typename unit>                                     \
  TFEL_MATH_INLINE constexpr qt<                                            \
      typename SubUnit_<NoUnit, unit>::type,                                \
      typename tfel::typetraits::Promote<T1, X>::type>                      \
  operator/(const X, const qt<unit, T1>)

namespace tfel {

  namespace math {

    template <typename UnitType, typename ValueType>
    struct UnaryResultType_<ScalarTag,
                            UnaryOperatorTag,
                            qt<UnitType, ValueType>,
                            OpNeg> {
      //! \brief result
      using type =
          qt<UnitType, typename UnaryResultType<ValueType, OpNeg>::type>;
    };  // end of struct UnaryResultType_

    /*!
     * \brief Partial specialisation for addition of two qt having the same unit
     * \see   ResultType
     */
    template <typename unit, typename T1, typename T2>
    struct ResultType<qt<unit, T1>, qt<unit, T2>, OpPlus> {
      typedef qt<unit, typename tfel::typetraits::Promote<T1, T2>::type> type;
    };

    template <typename unit, typename T1, typename T2>
    TFEL_MATH_INLINE constexpr qt<
        unit,
        typename tfel::typetraits::Promote<T1, T2>::type>
    operator+(const qt<unit, T1>, const qt<unit, T2>);

    /*!
     * \brief Partial specialisation for substraction of two qt having the same
     * unit \see   ResultType
     */
    template <typename unit, typename T1, typename T2>
    struct ResultType<qt<unit, T1>, qt<unit, T2>, OpMinus> {
      typedef qt<unit, typename tfel::typetraits::Promote<T1, T2>::type> type;
    };

    template <typename unit, typename T1, typename T2>
    TFEL_MATH_INLINE constexpr qt<
        unit,
        typename tfel::typetraits::Promote<T1, T2>::type>
    operator-(const qt<unit, T1>, const qt<unit, T2>);

    /*!
     * \brief Partial specialisation for multiplication of two qt
     * \see   ResultType
     */
    template <typename unit, typename T1, typename unit2, typename T2>
    struct ResultType<qt<unit, T1>, qt<unit2, T2>, OpMult> {
      typedef qt<typename AddUnit_<unit, unit2>::type,
                 typename tfel::typetraits::Promote<T1, T2>::type>
          type;
    };

    template <typename unit, typename T1, typename unit2, typename T2>
    TFEL_MATH_INLINE constexpr qt<
        typename AddUnit_<unit, unit2>::type,
        typename tfel::typetraits::Promote<T1, T2>::type>
    operator*(const qt<unit, T1>, const qt<unit2, T2>);

    /*!
     * \brief Partial specialisation for division of two qt
     * \see   ResultType
     */
    template <typename unit, typename T1, typename unit2, typename T2>
    struct ResultType<qt<unit, T1>, qt<unit2, T2>, OpDiv> {
      typedef qt<typename SubUnit_<unit, unit2>::type,
                 typename tfel::typetraits::Promote<T1, T2>::type>
          type;
    };

    template <typename unit, typename T1, typename unit2, typename T2>
    TFEL_MATH_INLINE constexpr qt<
        typename SubUnit_<unit, unit2>::type,
        typename tfel::typetraits::Promote<T1, T2>::type>
    operator/(const qt<unit, T1>, const qt<unit2, T2>);

    QT_RESULT_TYPE(unsigned short);
    QT_RESULT_TYPE(unsigned int);
    QT_RESULT_TYPE(long unsigned int);
    QT_RESULT_TYPE(short);
    QT_RESULT_TYPE(int);
    QT_RESULT_TYPE(long int);
    QT_RESULT_TYPE(float);
    QT_RESULT_TYPE(double);
    QT_RESULT_TYPE(long double);
    QT_RESULT_TYPE(Complex<unsigned short>);
    QT_RESULT_TYPE(Complex<unsigned int>);
    QT_RESULT_TYPE(Complex<long unsigned int>);
    QT_RESULT_TYPE(Complex<short>);
    QT_RESULT_TYPE(Complex<int>);
    QT_RESULT_TYPE(Complex<long int>);
    QT_RESULT_TYPE(Complex<float>);
    QT_RESULT_TYPE(Complex<double>);
    QT_RESULT_TYPE(Complex<long double>);

  }  // namespace math

}  // end of namespace tfel

#include "TFEL/Math/Quantity/qtOperations.ixx"

#endif /* LIB_TFEL_QT_OPERATIONS_H_ */
