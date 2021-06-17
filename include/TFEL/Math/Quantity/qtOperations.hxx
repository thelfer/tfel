/*!
 * \file   include/TFEL/Math/Quantity/qtOperations.hxx
 * \brief  This file declares usual operations on quantities.
 * \author Thomas Helfer
 * \date   07 Jun 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QT_OPERATIONS_HXX
#define LIB_TFEL_MATH_QT_OPERATIONS_HXX

#include "TFEL/Math/Forward/Complex.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/Quantity/Unit.hxx"
#include "TFEL/Math/qt.hxx"

#define QT_RESULT_TYPE(X)                                                      \
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
  operator*(const Quantity<UnitType, ValueType, OwnershipPolicy>&, const X);   \
                                                                               \
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>   \
  constexpr qt<UnitType,                                                       \
               typename tfel::typetraits::Promote<ValueType, X>::type>         \
  operator*(const X, const Quantity<UnitType, ValueType, OwnershipPolicy>&);   \
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
  constexpr Quantity<UnitType,                                                 \
                     typename tfel::typetraits::Promote<ValueType, X>::type,   \
                     OwnershipPolicy>                                          \
  operator/(const Quantity<UnitType, ValueType, OwnershipPolicy>&, const X);   \
                                                                               \
  template <typename ValueType, typename UnitType, typename OwnershipPolicy>   \
  constexpr Quantity<                                                          \
      typename tfel::math::internals::SubstractUnit<NoUnit, UnitType>::type,   \
      typename tfel::typetraits::Promote<ValueType, X>::type, OwnershipPolicy> \
  operator/(const X, const Quantity<UnitType, ValueType, OwnershipPolicy>&)

namespace tfel::math {

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
            typename ValueType2,
            typename OwnershipPolicy2>
  constexpr qt<UnitType,
               typename tfel::typetraits::Promote<ValueType, ValueType2>::type>
  operator+(const Quantity<UnitType, ValueType, OwnershipPolicy>& a,
            const Quantity<UnitType, ValueType2, OwnershipPolicy2>& b) {
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
            typename ValueType2,
            typename OwnershipPolicy2>
  constexpr qt<UnitType,
               typename tfel::typetraits::Promote<ValueType, ValueType2>::type>
  operator-(const Quantity<UnitType, ValueType, OwnershipPolicy>& a,
            const Quantity<UnitType, ValueType, OwnershipPolicy2>& b) {
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

}  // namespace tfel::math

#include "TFEL/Math/Quantity/qtOperations.ixx"

#endif /* LIB_TFEL_MATH_QT_OPERATIONS_HXX */
