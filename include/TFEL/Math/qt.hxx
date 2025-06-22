/*!
 * \file   include/TFEL/Math/qt.hxx
 * \brief  This file declares the qt class.
 * \author Thomas Helfer
 * \date   06 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QT_HXX
#define LIB_TFEL_MATH_QT_HXX

#include <cmath>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/TypeList.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsReal.hxx"
#include "TFEL/TypeTraits/IsComplex.hxx"
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/TypeTraits/Promote.hxx"
#include "TFEL/Math/Forward/Complex.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/UnaryResultType.hxx"
#include "TFEL/Math/Quantity/Unit.hxx"
#include "TFEL/Math/power.hxx"

namespace tfel::math::internals {

  template <typename ValueType, bool AllowImplicitConversion>
  struct QuantityValueOwnershipPolicy {
    //
    static_assert(std::is_same_v<ValueType, std::decay_t<ValueType>>,
                  "invalid value type");
    //! \brief default constructor
    constexpr QuantityValueOwnershipPolicy() noexcept = default;
    //! \brief copy constructor.
    constexpr QuantityValueOwnershipPolicy(
        const QuantityValueOwnershipPolicy&) noexcept = default;
    //! \brief move constructor.
    constexpr QuantityValueOwnershipPolicy(
        QuantityValueOwnershipPolicy&&) noexcept = default;
    /*!
     * \brief constructor from a value
     * \param src: the src.
     */
    template <typename T,
              std::enable_if_t<((std::is_constructible_v<ValueType, T>)&&  //
                                (std::is_convertible_v<ValueType, T>)&&    //
                                (!AllowImplicitConversion)),
                               bool> = false>
    constexpr explicit QuantityValueOwnershipPolicy(const T& src) noexcept
        : value(src) {}
    /*!
     * \brief constructor from a value
     * \param src: the src.
     */
    template <typename T,
              std::enable_if_t<((std::is_constructible_v<ValueType, T>)&&  //
                                (std::is_convertible_v<ValueType, T>)&&    //
                                (AllowImplicitConversion)),
                               bool> = false>
    constexpr QuantityValueOwnershipPolicy(const T& src) noexcept
        : value(src) {}
    //! \brief return the value
    constexpr ValueType& getValue() noexcept { return this->value; }
    //! \brief return the value
    constexpr const ValueType& getValue() const noexcept { return this->value; }
    //! \brief assignement operator
    constexpr QuantityValueOwnershipPolicy& operator=(
        const QuantityValueOwnershipPolicy&) noexcept = default;
    //! \brief move assignement operator
    constexpr QuantityValueOwnershipPolicy& operator=(
        QuantityValueOwnershipPolicy&&) noexcept = default;
    /*!
     * \brief constructor from a value
     * \param src: the src.
     */
    template <typename T>
    constexpr std::enable_if_t<((std::is_constructible_v<ValueType, T>)&&  //
                                (std::is_convertible_v<ValueType, T>)&&    //
                                (AllowImplicitConversion)),
                               QuantityValueOwnershipPolicy&>
    operator=(const T& src) noexcept {
      this->value = src;
      return *this;
    }
    //! \brief conversion operator
    template <typename T,
              std::enable_if_t<((std::is_same_v<ValueType, T>)&&  //
                                (AllowImplicitConversion)),
                               bool> = false>
    constexpr operator T&() noexcept {
      return this->value;
    }
    //! \brief conversion operator
    template <typename T,
              std::enable_if_t<((std::is_same_v<ValueType, T>)&&  //
                                (AllowImplicitConversion)),
                               bool> = false>
    constexpr operator T() const noexcept {
      return this->value;
    }
    //! \brief destructor
    ~QuantityValueOwnershipPolicy() noexcept = default;

   private:
    //! \brief the value of the quantity
    ValueType value;
  };

  template <typename ValueType, bool AllowImplicitConversion>
  struct QuantityReferenceOwnershipPolicy {
    //! \brief default constructor
    constexpr QuantityReferenceOwnershipPolicy() noexcept = default;
    //! \brief copy constructor.
    constexpr QuantityReferenceOwnershipPolicy(
        const QuantityReferenceOwnershipPolicy&) noexcept = default;
    //! \brief move constructor.
    constexpr QuantityReferenceOwnershipPolicy(
        QuantityReferenceOwnershipPolicy&&) noexcept = default;
    /*!
     * \brief constructor from a value
     * \param src: the src.
     */
    constexpr explicit QuantityReferenceOwnershipPolicy(ValueType& src) noexcept
        : value(src) {}
    //! \brief return the value
    constexpr ValueType& getValue() noexcept { return this->value; }
    //! \brief return the value
    constexpr const ValueType& getValue() const noexcept { return this->value; }
    //! \brief assignement operator
    constexpr QuantityReferenceOwnershipPolicy& operator=(
        const QuantityReferenceOwnershipPolicy&) noexcept = default;
    //! \brief move assignement operator
    constexpr QuantityReferenceOwnershipPolicy& operator=(
        QuantityReferenceOwnershipPolicy&&) noexcept = default;
    /*!
     * \brief constructor from a value
     * \param src: the src.
     */
    template <typename T>
    constexpr std::enable_if_t<((std::is_constructible_v<ValueType, T>)&&  //
                                (std::is_convertible_v<ValueType, T>)&&    //
                                (AllowImplicitConversion)),
                               QuantityReferenceOwnershipPolicy&>
    operator=(const T& src) noexcept {
      this->value = src;
      return *this;
    }
    //! \brief conversion operator
    template <typename T,
              std::enable_if_t<((std::is_same_v<ValueType, T>)&&  //
                                (AllowImplicitConversion)),
                               bool> = false>
    constexpr operator T&() noexcept {
      return this->value;
    }
    //! \brief conversion operator
    template <typename T,
              std::enable_if_t<((std::is_same_v<ValueType, T>)&&  //
                                (AllowImplicitConversion)),
                               bool> = false>
    constexpr operator const T&() const noexcept {
      return this->value;
    }
    //! \brief destructor
    ~QuantityReferenceOwnershipPolicy() noexcept = default;

   private:
    //! \brief reference to the value of the quantity
    ValueType& value;
  };

}  // namespace tfel::math::internals

namespace tfel::math {

  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  struct Quantity : OwnershipPolicy {
    static_assert(tfel::typetraits::IsFundamentalNumericType<ValueType>::cond);
    static_assert(isScalar<ValueType>());
    /*!
     * An helper class to deal with some limitations of Visual Studio
     */
    template <typename T, typename T2>
    struct IsQtScalarOperationValid {
      static constexpr bool cond =
          tfel::typetraits::IsFundamentalNumericType<T2>::cond &&
          isScalar<T>() &&
          std::is_same<typename tfel::typetraits::Promote<T, T2>::type,
                       T>::value;
    };
    //! \brief a simple alias
    template <typename ValueType1, typename ValueType2>
    using promote =
        typename tfel::typetraits::Promote<ValueType1, ValueType2>::type;
    //! \brief a simple alias
    using value_type = ValueType;
    //
    using OwnershipPolicy::OwnershipPolicy;
    //
    template <typename ValueType2,
              typename OwnershipPolicy2,
              std::enable_if_t<
                  std::is_same_v<promote<ValueType, ValueType2>, ValueType>,
                  bool> = true>
    constexpr Quantity(
        const Quantity<UnitType, ValueType2, OwnershipPolicy2>& src) noexcept
        : OwnershipPolicy(src.getValue()) {}  // end of Quantity
    //
    using OwnershipPolicy::operator=;
    //! \brief assignement operator
    template <typename ValueType2, typename OwnershipPolicy2>
    constexpr std::enable_if_t<
        std::is_same_v<promote<ValueType, ValueType2>, ValueType>,
        Quantity&>
    operator=(
        const Quantity<UnitType, ValueType2, OwnershipPolicy2>& src) noexcept {
      this->getValue() = src.getValue();
      return *this;
    }
    //! \brief operator +=
    template <typename ValueType2, typename OwnershipPolicy2>
    constexpr std::enable_if_t<
        std::is_same_v<promote<ValueType, ValueType2>, ValueType>,
        Quantity&>
    operator+=(
        const Quantity<UnitType, ValueType2, OwnershipPolicy2>& src) noexcept {
      this->getValue() += src.getValue();
      return *this;
    }
    //! \brief operator -=
    template <typename ValueType2, typename OwnershipPolicy2>
    constexpr std::enable_if_t<
        std::is_same<promote<ValueType, ValueType2>, ValueType>::value,
        Quantity&>
    operator-=(
        const Quantity<UnitType, ValueType2, OwnershipPolicy2>& src) noexcept {
      this->getValue() -= src.getValue();
      return *this;
    }
    /*!
     * \brief multiplication by a scalar
     * \tparam ValueType2: another numeric type
     * \param[in] a: a scalar
     */
    template <typename ValueType2>
    constexpr std::enable_if_t<
        IsQtScalarOperationValid<ValueType, ValueType2>::cond,
        Quantity&>
    operator*=(const ValueType2& a) noexcept {
      this->getValue() *= a;
      return *this;
    }
    /*!
     * \brief multiplication by a scalar
     * \tparam ValueType2: another numeric type
     * \param[in] a: a scalar
     */
    template <typename ValueType2, typename OwnershipPolicy2>
    constexpr std::enable_if_t<
        IsQtScalarOperationValid<ValueType, ValueType2>::cond,
        Quantity&>
    operator*=(
        const Quantity<NoUnit, ValueType2, OwnershipPolicy2>& a) noexcept {
      this->getValue() *= a.getValue();
      return *this;
    }
    /*!
     * \brief division by a scalar
     * \tparam ValueType2: another numeric type
     * \param[in] a: a scalar
     */
    template <typename ValueType2>
    constexpr std::enable_if_t<
        IsQtScalarOperationValid<ValueType, ValueType2>::cond,
        Quantity&>
    operator/=(const ValueType2& a) noexcept {
      this->getValue() /= a;
      return *this;
    }
    /*!
     * \brief division by a scalar
     * \tparam ValueType2: another numeric type
     * \param[in] a: a scalar
     */
    template <typename ValueType2, typename OwnershipPolicy2>
    constexpr std::enable_if_t<
        IsQtScalarOperationValid<ValueType, ValueType2>::cond,
        Quantity&>
    operator/=(
        const Quantity<NoUnit, ValueType2, OwnershipPolicy2>& a) noexcept {
      this->getValue() /= a.getValue();
      return *this;
    }
    //! \brief negation operator
    constexpr qt<UnitType, ValueType> operator-() const noexcept {
      return qt<UnitType, ValueType>(-(this->getValue()));
    }
  };  // end of struct Quantity

  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename UnitType2,
            typename ValueType2,
            typename OwnershipPolicy2>
  constexpr bool operator<(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,
      const Quantity<UnitType2, ValueType2, OwnershipPolicy2>& b) noexcept {
    static_assert(std::is_same_v<UnitType, UnitType2>,
                  "invalid operation (unmatched unit)");
    return a.getValue() < b.getValue();
  }

  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename UnitType2,
            typename ValueType2,
            typename OwnershipPolicy2>
  constexpr bool operator<=(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,
      const Quantity<UnitType2, ValueType2, OwnershipPolicy2>& b) noexcept {
    static_assert(std::is_same_v<UnitType, UnitType2>,
                  "invalid operation (unmatched unit)");
    return a.getValue() <= b.getValue();
  }

  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename UnitType2,
            typename ValueType2,
            typename OwnershipPolicy2>
  constexpr bool operator>(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,
      const Quantity<UnitType2, ValueType2, OwnershipPolicy2>& b) noexcept {
    static_assert(std::is_same_v<UnitType, UnitType2>,
                  "invalid operation (unmatched unit)");
    return a.getValue() > b.getValue();
  }

  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename UnitType2,
            typename ValueType2,
            typename OwnershipPolicy2>
  constexpr bool operator>=(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,
      const Quantity<UnitType2, ValueType2, OwnershipPolicy2>& b) noexcept {
    static_assert(std::is_same_v<UnitType, UnitType2>,
                  "invalid operation (unmatched unit)");
    return a.getValue() >= b.getValue();
  }

  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename UnitType2,
            typename ValueType2,
            typename OwnershipPolicy2>
  constexpr bool operator==(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,
      const Quantity<UnitType2, ValueType2, OwnershipPolicy2>& b) noexcept {
    static_assert(std::is_same_v<UnitType, UnitType2>,
                  "invalid operation (unmatched unit)");
    return a.getValue() == b.getValue();
  }

  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename UnitType2,
            typename ValueType2,
            typename OwnershipPolicy2>
  constexpr bool operator!=(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& a,
      const Quantity<UnitType2, ValueType2, OwnershipPolicy2>& b) noexcept {
    static_assert(std::is_same_v<UnitType, UnitType2>,
                  "invalid operation (unmatched unit)");
    return a.getValue() != b.getValue();
  }

  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  constexpr qt<UnitType, ValueType> abs(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& v) noexcept {
    return v.getValue() < 0 ? qt<UnitType, ValueType>{-v.getValue()}
                            : qt<UnitType, ValueType>{v.getValue()};
  }

  template <typename Unit,
            typename ValueType,
            typename OwnershipPolicy,
            int N,
            unsigned int D>
  class UnaryResultType<Quantity<Unit, ValueType, OwnershipPolicy>,
                        Power<N, D>> {
    using ResultUnit =
        typename tfel::math::internals::PowerUnit<N, D, Unit>::type;
    using ResultValueType =
        typename UnaryResultType<ValueType, Power<N, D>>::type;

   public:
    //! \brief result of the metafunction
    using type = qt<ResultUnit, ResultValueType>;
  };

  template <int N, typename Unit, typename ValueType, typename OwnershipPolicy>
  constexpr std::enable_if_t<
      std::is_floating_point_v<ValueType>,
      typename UnaryResultType<Quantity<Unit, ValueType, OwnershipPolicy>,
                               Power<N, 1u>>::type>
  power(const Quantity<Unit, ValueType, OwnershipPolicy>& x) {
    using Result =
        typename UnaryResultType<Quantity<Unit, ValueType, OwnershipPolicy>,
                                 Power<N, 1u>>::type;
    return Result{power<N>(x.getValue())};
  }

  template <int N,
            unsigned int D,
            typename Unit,
            typename ValueType,
            typename OwnershipPolicy>
  constexpr std::enable_if_t<
      std::is_floating_point_v<ValueType>,
      typename UnaryResultType<Quantity<Unit, ValueType, OwnershipPolicy>,
                               Power<N, D>>::type>
  power(const Quantity<Unit, ValueType, OwnershipPolicy>& x) {
    using Result =
        typename UnaryResultType<Quantity<Unit, ValueType, OwnershipPolicy>,
                                 Power<N, D>>::type;
    return Result{power<N, D>(x.getValue())};
  }

  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  constexpr auto square_root(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& q) noexcept {
    return power<1, 2>(q.getValue());
  }

}  // namespace tfel::math

namespace tfel::math::ieee754 {

  /*!
   * \return the class of the floating point value according to the
   * IEEE754 standard.
   * Possible return values are:
   * - FP_NORMAL: standard floating point value
   * - FP_SUBNORMAL: subnormal floating point value
   * - FP_NAN: NaN value
   * - FP_INFINITE: +Inf or -Inf value
   * \param[in] q: value to be tested
   */
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  constexpr int fpclassify(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& q) noexcept {
    static_assert(std::is_floating_point_v<ValueType>,
                  "ValueType must a floating point value");
    return fpclassify(q.getValue());
  }  // end of fpclassify

  /*!
   * \return true if the given quantity is a not-a-number (NaN) value.
   * \param[in] q: value to be tested
   */
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  constexpr bool isnan(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& q) noexcept {
    static_assert(std::is_floating_point_v<ValueType>,
                  "ValueType must a floating point value");
    return isnan(q.getValue());
  }

  /*!
   * \return true if the given quantity is finite.
   * \param[in] q: value to be tested
   */
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  constexpr bool isfinite(
      const Quantity<UnitType, ValueType, OwnershipPolicy>& q) noexcept {
    static_assert(std::is_floating_point_v<ValueType>,
                  "ValueType must a floating point value");
    return isfinite(q.getValue());
  }

}  // end of namespace tfel::math::ieee754

#include "TFEL/Math/Quantity/qtLimits.hxx"
#include "TFEL/Math/Quantity/qtSpecific.hxx"
#include "TFEL/Math/Quantity/qtOperations.hxx"

#endif /* LIB_TFEL_MATH_QT_HXX */
