/*!
 * \file   include/TFEL/Math/Quantity/qtOperations.hxx
 * \brief  This file declares usual operations on quantities.
 * \author Thomas Helfer
 * \date   07 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QT_OPERATIONS_HXX
#define LIB_TFEL_MATH_QT_OPERATIONS_HXX

#include "TFEL/Math/Forward/Complex.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/Quantity/Unit.hxx"
#include "TFEL/Math/qt.hxx"

#define TFEL_MATH_QT_SCALAR_OPERATIONS(X)                                      \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr bool operator<(                                                    \
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,                 \
      const X& b) noexcept {                                                   \
    static_assert(std::is_same_v<UnitType, NoUnit>,                            \
                  "invalid operation (unmatched unit)");                       \
    return a.getValue() < b;                                                   \
  }                                                                            \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr bool operator<(                                                    \
      const X& a,                                                              \
      const Quantity<UnitType, ValueType, OwnershipPolicy>& b) noexcept {      \
    static_assert(std::is_same_v<UnitType, NoUnit>,                            \
                  "invalid operation (unmatched unit)");                       \
    return a < b.getValue();                                                   \
  }                                                                            \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr bool operator<=(                                                   \
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,                 \
      const X& b) noexcept {                                                   \
    static_assert(std::is_same_v<UnitType, NoUnit>,                            \
                  "invalid operation (unmatched unit)");                       \
    return a.getValue() <= b;                                                  \
  }                                                                            \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr bool operator<=(                                                   \
      const X& a,                                                              \
      const Quantity<UnitType, ValueType, OwnershipPolicy>& b) noexcept {      \
    static_assert(std::is_same_v<UnitType, NoUnit>,                            \
                  "invalid operation (unmatched unit)");                       \
    return a <= b.getValue();                                                  \
  }                                                                            \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr bool operator>(                                                    \
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,                 \
      const X& b) noexcept {                                                   \
    static_assert(std::is_same_v<UnitType, NoUnit>,                            \
                  "invalid operation (unmatched unit)");                       \
    return a.getValue() > b;                                                   \
  }                                                                            \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr bool operator>(                                                    \
      const X& a,                                                              \
      const Quantity<UnitType, ValueType, OwnershipPolicy>& b) noexcept {      \
    static_assert(std::is_same_v<UnitType, NoUnit>,                            \
                  "invalid operation (unmatched unit)");                       \
    return a > b.getValue();                                                   \
  }                                                                            \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr bool operator>=(                                                   \
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,                 \
      const X& b) noexcept {                                                   \
    static_assert(std::is_same_v<UnitType, NoUnit>,                            \
                  "invalid operation (unmatched unit)");                       \
    return a.getValue() >= b;                                                  \
  }                                                                            \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr bool operator>=(                                                   \
      const X& a,                                                              \
      const Quantity<UnitType, ValueType, OwnershipPolicy>& b) noexcept {      \
    static_assert(std::is_same_v<UnitType, NoUnit>,                            \
                  "invalid operation (unmatched unit)");                       \
    return a >= b.getValue();                                                  \
  }                                                                            \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr bool operator==(                                                   \
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,                 \
      const X& b) noexcept {                                                   \
    static_assert(std::is_same_v<UnitType, NoUnit>,                            \
                  "invalid operation (unmatched unit)");                       \
    return a.getValue() == b;                                                  \
  }                                                                            \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr bool operator==(                                                   \
      const X& a,                                                              \
      const Quantity<UnitType, ValueType, OwnershipPolicy>& b) noexcept {      \
    static_assert(std::is_same_v<UnitType, NoUnit>,                            \
                  "invalid operation (unmatched unit)");                       \
    return a == b.getValue();                                                  \
  }                                                                            \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr bool operator!=(                                                   \
      const X& a,                                                              \
      const Quantity<UnitType, ValueType, OwnershipPolicy>& b) noexcept {      \
    static_assert(std::is_same_v<UnitType, NoUnit>,                            \
                  "invalid operation (unmatched unit)");                       \
    return a != b.getValue();                                                  \
  }                                                                            \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr bool operator!=(                                                   \
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,                 \
      const X& b) noexcept {                                                   \
    static_assert(std::is_same_v<UnitType, NoUnit>,                            \
                  "invalid operation (unmatched unit)");                       \
    return a.getValue() != b;                                                  \
  }                                                                            \
  /*!                                                                          \
   * \brief Partial specialisation for qt with no unit and a scalar            \
   * \see   ResultType                                                         \
   */                                                                          \
  template <typename ValueType, typename OwnershipPolicy>                      \
  struct ResultType<Quantity<NoUnit, ValueType, OwnershipPolicy>, X, OpPlus> { \
    using type =                                                               \
        qt<NoUnit, typename tfel::typetraits::Promote<ValueType, X>::type>;    \
  };                                                                           \
                                                                               \
  /*!                                                                          \
   * \brief Partial specialisation for qt with no unit and a scalar            \
   * \see   ResultType                                                         \
   */                                                                          \
  template <typename ValueType, typename OwnershipPolicy>                      \
  struct ResultType<X, Quantity<NoUnit, ValueType, OwnershipPolicy>, OpPlus> { \
    using type =                                                               \
        qt<NoUnit, typename tfel::typetraits::Promote<ValueType, X>::type>;    \
  };                                                                           \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr qt<NoUnit, typename tfel::typetraits::Promote<ValueType, X>::type> \
  operator+(const Quantity<NoUnit, ValueType, OwnershipPolicy>&,               \
            const X&) noexcept;                                                \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr qt<NoUnit, typename tfel::typetraits::Promote<ValueType, X>::type> \
  operator+(const X&,                                                          \
            const Quantity<NoUnit, ValueType, OwnershipPolicy>&) noexcept;     \
                                                                               \
  /*!                                                                          \
   * \brief Partial specialisation for qt with no unit and a scalar            \
   * \see   ResultType                                                         \
   */                                                                          \
  template <typename ValueType, typename OwnershipPolicy>                      \
  struct ResultType<Quantity<NoUnit, ValueType, OwnershipPolicy>, X,           \
                    OpMinus> {                                                 \
    using type =                                                               \
        qt<NoUnit, typename tfel::typetraits::Promote<ValueType, X>::type>;    \
  };                                                                           \
                                                                               \
  /*!                                                                          \
   * \brief Partial specialisation for qt with no unit and a scalar            \
   * \see   ResultType                                                         \
   */                                                                          \
  template <typename ValueType, typename OwnershipPolicy>                      \
  struct ResultType<X, Quantity<NoUnit, ValueType, OwnershipPolicy>,           \
                    OpMinus> {                                                 \
    using type =                                                               \
        qt<NoUnit, typename tfel::typetraits::Promote<ValueType, X>::type>;    \
  };                                                                           \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr qt<NoUnit, typename tfel::typetraits::Promote<ValueType, X>::type> \
  operator-(const Quantity<NoUnit, ValueType, OwnershipPolicy>&,               \
            const X&) noexcept;                                                \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr qt<NoUnit, typename tfel::typetraits::Promote<ValueType, X>::type> \
  operator-(const X&,                                                          \
            const Quantity<NoUnit, ValueType, OwnershipPolicy>&) noexcept;     \
                                                                               \
  /*!                                                                          \
   * \brief Partial specialisation for qt by scalar product                    \
   * \see   ResultType                                                         \
   */                                                                          \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  struct ResultType<Quantity<UnitType, ValueType, OwnershipPolicy>, X,         \
                    OpMult> {                                                  \
    using type =                                                               \
        qt<UnitType, typename tfel::typetraits::Promote<ValueType, X>::type>;  \
  };                                                                           \
                                                                               \
  /*!                                                                          \
   * \brief Partial specialisation for scalar by qt product                    \
   * \see   ResultType                                                         \
   */                                                                          \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  struct ResultType<X, Quantity<UnitType, ValueType, OwnershipPolicy>,         \
                    OpMult> {                                                  \
    using type =                                                               \
        qt<UnitType, typename tfel::typetraits::Promote<ValueType, X>::type>;  \
  };                                                                           \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr qt<UnitType,                                                       \
               typename tfel::typetraits::Promote<ValueType, X>::type>         \
  operator*(const Quantity<UnitType, ValueType, OwnershipPolicy>&,             \
            const X&) noexcept;                                                \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr qt<UnitType,                                                       \
               typename tfel::typetraits::Promote<ValueType, X>::type>         \
  operator*(const X&,                                                          \
            const Quantity<UnitType, ValueType, OwnershipPolicy>&) noexcept;   \
                                                                               \
  /*!                                                                          \
   * \brief Partial specialisation for qt by scalar division                   \
   * \see   ResultType                                                         \
   */                                                                          \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  struct ResultType<Quantity<UnitType, ValueType, OwnershipPolicy>, X,         \
                    OpDiv> {                                                   \
    using type =                                                               \
        qt<UnitType, typename tfel::typetraits::Promote<ValueType, X>::type>;  \
  };                                                                           \
                                                                               \
  /*!                                                                          \
   * \brief Partial specialisation for scalar by qt division                   \
   * \see   ResultType                                                         \
   */                                                                          \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  struct ResultType<X, Quantity<UnitType, ValueType, OwnershipPolicy>,         \
                    OpDiv> {                                                   \
    using type = qt<                                                           \
        typename tfel::math::internals::SubstractUnit<NoUnit, UnitType>::type, \
        typename tfel::typetraits::Promote<ValueType, X>::type>;               \
  };                                                                           \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr qt<UnitType,                                                       \
               typename tfel::typetraits::Promote<ValueType, X>::type>         \
  operator/(const Quantity<UnitType, ValueType, OwnershipPolicy>&,             \
            const X&) noexcept;                                                \
                                                                               \
  template <typename ValueType, typename UnitType, typename OwnershipPolicy>   \
  constexpr qt<                                                                \
      typename tfel::math::internals::SubstractUnit<NoUnit, UnitType>::type,   \
      typename tfel::typetraits::Promote<ValueType, X>::type>                  \
  operator/(const X&,                                                          \
            const Quantity<UnitType, ValueType, OwnershipPolicy>&) noexcept

namespace tfel::math {

  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  struct ComputeUnaryOperationResult<
      ScalarTag,
      UnaryOperatorTag,
      Quantity<UnitType, ValueType, OwnershipPolicy>,
      OpNeg> {
    //! \brief result
    using type = qt<UnitType, typename UnaryResultType<ValueType, OpNeg>::type>;
  };  // end of struct ComputeUnaryOperationResult

  /*!
   * \brief Partial specialisation for addition of two qt having the same unit
   * \see   ResultType
   */
  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename ValueType2,
            typename OwnershipPolicy2>
  struct ResultType<Quantity<UnitType, ValueType, OwnershipPolicy>,
                    Quantity<UnitType, ValueType2, OwnershipPolicy2>,
                    OpPlus> {
    using type =
        qt<UnitType,
           typename tfel::typetraits::Promote<ValueType, ValueType2>::type>;
  };

  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename UnitType2,
            typename ValueType2,
            typename OwnershipPolicy2>
  constexpr qt<UnitType,
               typename tfel::typetraits::Promote<ValueType, ValueType2>::type>
  operator+(const Quantity<UnitType, ValueType, OwnershipPolicy>& a,
            const Quantity<UnitType2, ValueType2, OwnershipPolicy2>& b) {
    static_assert(std::is_same_v<UnitType, UnitType2>, "invalid operation");
    return qt<UnitType,
              typename tfel::typetraits::Promote<ValueType, ValueType2>::type>{
        a.getValue() + b.getValue()};
  }

  /*!
   * \brief Partial specialisation for substraction of two qt having the same
   * unit \see   ResultType
   */
  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename ValueType2,
            typename OwnershipPolicy2>
  struct ResultType<Quantity<UnitType, ValueType, OwnershipPolicy>,
                    Quantity<UnitType, ValueType2, OwnershipPolicy2>,
                    OpMinus> {
    using type =
        qt<UnitType,
           typename tfel::typetraits::Promote<ValueType, ValueType2>::type>;
  };

  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename UnitType2,
            typename ValueType2,
            typename OwnershipPolicy2>
  constexpr qt<UnitType,
               typename tfel::typetraits::Promote<ValueType, ValueType2>::type>
  operator-(const Quantity<UnitType, ValueType, OwnershipPolicy>& a,
            const Quantity<UnitType2, ValueType2, OwnershipPolicy2>& b) {
    static_assert(std::is_same_v<UnitType, UnitType2>, "invalid operation");
    return qt<UnitType,
              typename tfel::typetraits::Promote<ValueType, ValueType2>::type>{
        a.getValue() - b.getValue()};
  }

  /*!
   * \brief Partial specialisation for multiplication of two qt
   * \see   ResultType
   */
  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename UnitType2,
            typename ValueType2,
            typename OwnershipPolicy2>
  struct ResultType<Quantity<UnitType, ValueType, OwnershipPolicy>,
                    Quantity<UnitType2, ValueType2, OwnershipPolicy2>,
                    OpMult> {
    using type =
        qt<typename tfel::math::internals::AddUnit<UnitType, UnitType2>::type,
           typename tfel::typetraits::Promote<ValueType, ValueType2>::type>;
  };

  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename UnitType2,
            typename ValueType2,
            typename OwnershipPolicy2>
  constexpr auto operator*(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,
      const Quantity<UnitType2, ValueType2, OwnershipPolicy2>& b) {
    return qt<
        typename tfel::math::internals::AddUnit<UnitType, UnitType2>::type,
        typename tfel::typetraits::Promote<ValueType, ValueType2>::type>{
        a.getValue() * b.getValue()};
  }

  /*!
   * \brief Partial specialisation for division of two qt
   * \see   ResultType
   */
  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename UnitType2,
            typename ValueType2,
            typename OwnershipPolicy2>
  struct ResultType<Quantity<UnitType, ValueType, OwnershipPolicy>,
                    Quantity<UnitType2, ValueType2, OwnershipPolicy2>,
                    OpDiv> {
    using type =
        qt<typename tfel::math::internals::SubstractUnit<UnitType,
                                                         UnitType2>::type,
           typename tfel::typetraits::Promote<ValueType, ValueType2>::type>;
  };

  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename UnitType2,
            typename ValueType2,
            typename OwnershipPolicy2>
  constexpr auto operator/(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,
      const Quantity<UnitType2, ValueType2, OwnershipPolicy2>& b) {
    return qt<typename tfel::math::internals::SubstractUnit<UnitType,
                                                            UnitType2>::type,
              typename tfel::typetraits::Promote<ValueType, ValueType2>::type>{
        a.getValue() / b.getValue()};
  }

  TFEL_MATH_QT_SCALAR_OPERATIONS(unsigned short);
  TFEL_MATH_QT_SCALAR_OPERATIONS(unsigned int);
  TFEL_MATH_QT_SCALAR_OPERATIONS(long unsigned int);
  TFEL_MATH_QT_SCALAR_OPERATIONS(short);
  TFEL_MATH_QT_SCALAR_OPERATIONS(int);
  TFEL_MATH_QT_SCALAR_OPERATIONS(long int);
  TFEL_MATH_QT_SCALAR_OPERATIONS(float);
  TFEL_MATH_QT_SCALAR_OPERATIONS(double);
  TFEL_MATH_QT_SCALAR_OPERATIONS(long double);
  TFEL_MATH_QT_SCALAR_OPERATIONS(Complex<unsigned short>);
  TFEL_MATH_QT_SCALAR_OPERATIONS(Complex<unsigned int>);
  TFEL_MATH_QT_SCALAR_OPERATIONS(Complex<long unsigned int>);
  TFEL_MATH_QT_SCALAR_OPERATIONS(Complex<short>);
  TFEL_MATH_QT_SCALAR_OPERATIONS(Complex<int>);
  TFEL_MATH_QT_SCALAR_OPERATIONS(Complex<long int>);
  TFEL_MATH_QT_SCALAR_OPERATIONS(Complex<float>);
  TFEL_MATH_QT_SCALAR_OPERATIONS(Complex<double>);
  TFEL_MATH_QT_SCALAR_OPERATIONS(Complex<long double>);

}  // namespace tfel::math

#include "TFEL/Math/Quantity/qtOperations.ixx"

#endif /* LIB_TFEL_MATH_QT_OPERATIONS_HXX */
