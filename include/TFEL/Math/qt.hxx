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

#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/Metaprogramming/TypeList.hxx"

#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsReal.hxx"
#include "TFEL/TypeTraits/IsComplex.hxx"
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include "TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/TypeTraits/Promote.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"

#include "TFEL/Math/Forward/Complex.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/Quantity/Unit.hxx"

namespace tfel {

  namespace math {

    /*!
     * An helper class to deal with some limitations of Visual Studio
     */
    template <typename T, typename T2>
    struct IsQtScalarOperationValid {
      static constexpr bool cond =
          tfel::typetraits::IsFundamentalNumericType<T2>::cond &&
          tfel::typetraits::IsScalar<T>::cond &&
          std::is_same<typename tfel::typetraits::Promote<T, T2>::type,
                       T>::value;
    };

    /*!
     * \class qt
     * \brief This class describes numbers with unit.
     * qt must be has efficient and simple to use that standard numerical types.
     * \param unit, the unit of the qt.
     * \param T, the underlying numerical type.
     * \pre T must be a fundamental numerical type.
     * \see unit_samples.hxx, IsFundamentalNumericType
     * \author Thomas Helfer
     * \date   06 Jun 2006
     */
    template <typename unit, typename T = double>
    class qt {
      /*!
       * A simple check, T must be a fundamental numerical type.
       */
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      /*!
       * A simple check, T must be a scalar.
       */
      TFEL_STATIC_ASSERT(tfel::typetraits::IsScalar<T>::cond);
      /*!
       * The value of the qt
       */
      T value;

     public:
      /*!
       * A simple typedef to indicate the numerical type used
       */
      using value_type = T;

      /*!
       * \brief Default constructor
       * value is left uninitialised.
       */
      TFEL_MATH_INLINE constexpr qt() noexcept(
          std::is_nothrow_constructible<T>::value)
          : value(T()) {}

      /*!
       * \brief Copy constructor.
       * \param qt src.
       * value takes the src's value.
       */
      TFEL_MATH_INLINE constexpr qt(const qt<unit, T>& src) noexcept(
          std::is_nothrow_constructible<T>::value)
          : value(src.value) {}

      /*!
       * \brief constructor from a value
       * \param T src, the src.
       */
      TFEL_MATH_INLINE constexpr explicit qt(const T& src) noexcept(
          std::is_nothrow_constructible<T>::value)
          : value(src) {}

      /*!
       * Return the value of the qt.
       */
      TFEL_MATH_INLINE T& getValue() noexcept;

      /*!
       * Return the value of the qt, const version
       */
      TFEL_MATH_INLINE constexpr const T& getValue() const noexcept;
      //! assignement operator
      TFEL_MATH_INLINE qt<unit, T>& operator=(const qt<unit, T>&);
      /*!
       * Assignement operator
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          std::is_same<typename tfel::typetraits::Promote<T, T2>::type,
                       T>::value,
          qt<unit, T>&>::type
      operator=(const qt<unit, T2>& src);
      /*!
       * Operator +=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          std::is_same<typename tfel::typetraits::Promote<T, T2>::type,
                       T>::value,
          qt<unit, T>&>::type
      operator+=(const qt<unit, T2>& src);

      /*!
       * Operator -=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          std::is_same<typename tfel::typetraits::Promote<T, T2>::type,
                       T>::value,
          qt<unit, T>&>::type
      operator-=(const qt<unit, T2>& src);

      /*!
       * Operator *=
       * \param T2 a, a scalar
       */
      template <typename T2>
      TFEL_MATH_INLINE
          typename std::enable_if<IsQtScalarOperationValid<T, T2>::cond,
                                  qt<unit, T>&>::type
          operator*=(const T2 a);
      /*!
       * Operator /=
       * \param T2 a, a scalar
       */
      template <typename T2>
      TFEL_MATH_INLINE
          typename std::enable_if<IsQtScalarOperationValid<T, T2>::cond,
                                  qt<unit, T>&>::type
          operator/=(const T2 a);

      /*!
       * negation operator
       */
      TFEL_MATH_INLINE constexpr qt<unit, T> operator-() const {
        return qt<unit, T>(-(this->value));
      }
    };

    /*!
     * An helper struct to dealt with some limitations of Visual Studio 10
     */
    template <typename T, typename T2>
    struct IsConvertibleToQtNoUnit {
      static constexpr bool cond =
          std::is_same<typename tfel::typetraits::Promote<T, T2>::type,
                       T>::value &&
          tfel::typetraits::IsFundamentalNumericType<T2>::cond &&
          tfel::typetraits::IsScalar<T2>::cond;
    };

    /*!
     * \brief Specialization in case NoUnit.
     * This class shall be implicitly convertible to T and T must be convertible
     * to qt<T,nounit>. \see qt.
     */
    template <typename T>
    class qt<NoUnit, T> {
      /*!
       * A simple check, T must be a fundamental numerical type.
       */
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<T>::cond);
      /*!
       * A simple check, T must be a scalar.
       */
      TFEL_STATIC_ASSERT(tfel::typetraits::IsScalar<T>::cond);
      /*!
       * The value of the qt
       */
      T value;

     public:
      /*!
       * A simple typedef to indicate the numerical type used
       */
      typedef T value_type;

      /*!
       * \brief Default constructor
       * value is left uninitialised.
       */
      TFEL_MATH_INLINE qt() noexcept(T()) {}

      /*!
       * \brief Copy constructor.
       * \param qt src.
       * value takes the src's value.
       */
      TFEL_MATH_INLINE qt(const qt<NoUnit, T>& src) noexcept(
          std::is_nothrow_constructible<T>::value)
          : value(src.value) {}

      /*!
       * \brief constructor from a value
       * Here, it is not declared explicit.
       * \param T src, the src.
       */
      TFEL_MATH_INLINE qt(const T& src) noexcept(
          std::is_nothrow_constructible<T>::value)
          : value(src) {}

      /*!
       * Return the value of the qt
       */
      TFEL_MATH_INLINE T& getValue() noexcept;

      /*!
       * Return the value of the qt, const version
       */
      TFEL_MATH_INLINE constexpr const T& getValue() const noexcept;
      /*!
       * Operator =
       */
      TFEL_MATH_INLINE qt<NoUnit, T>& operator=(const qt<NoUnit, T>& src);
      /*!
       * Operator =
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          std::is_same<typename tfel::typetraits::Promote<T, T2>::type,
                       T>::value,
          qt<NoUnit, T>&>::type
      operator=(const qt<NoUnit, T2>& src);

      template <typename T2>
      TFEL_MATH_INLINE
          typename std::enable_if<IsConvertibleToQtNoUnit<T, T2>::cond,
                                  qt<NoUnit, T>&>::type
          operator=(const T2 src);

      /*!
       * Operator +=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          std::is_same<typename tfel::typetraits::Promote<T, T2>::type,
                       T>::value,
          qt<NoUnit, T>&>::type
      operator+=(const qt<NoUnit, T2>& src);

      /*!
       * Operator -=
       */
      template <typename T2>
      TFEL_MATH_INLINE typename std::enable_if<
          std::is_same<typename tfel::typetraits::Promote<T, T2>::type,
                       T>::value,
          qt<NoUnit, T>&>::type
      operator-=(const qt<NoUnit, T2>& src);

      /*!
       * Operator *=
       * \param T2 a, a scalar
       */
      template <typename T2>
      TFEL_MATH_INLINE
          typename std::enable_if<IsConvertibleToQtNoUnit<T, T2>::cond,
                                  qt<NoUnit, T>&>::type
          operator*=(const T2 a);

      /*!
       * Operator /=
       * \param T2 a, a scalar
       */
      template <typename T2>
      TFEL_MATH_INLINE
          typename std::enable_if<IsConvertibleToQtNoUnit<T, T2>::cond,
                                  qt<NoUnit, T>&>::type
          operator/=(const T2 a);

      /*!
       * Conversion operator
       */
      constexpr operator T() const;

      /*!
       * negation operator
       */
      qt<NoUnit, T> operator-() const { return qt<NoUnit, T>(-(this->value)); }
    };

    template <typename unit, typename T>
    TFEL_MATH_INLINE constexpr bool operator<(const qt<unit, T>,
                                              const qt<unit, T>);

    template <typename unit, typename T>
    TFEL_MATH_INLINE constexpr bool operator<=(const qt<unit, T>,
                                               const qt<unit, T>);

    template <typename unit, typename T>
    TFEL_MATH_INLINE constexpr bool operator>(const qt<unit, T>,
                                              const qt<unit, T>);

    template <typename unit, typename T>
    TFEL_MATH_INLINE constexpr bool operator>=(const qt<unit, T>,
                                               const qt<unit, T>);

    template <typename unit, typename T>
    TFEL_MATH_INLINE constexpr bool operator==(const qt<unit, T>,
                                               const qt<unit, T>);

    template <typename unit, typename T>
    TFEL_MATH_INLINE constexpr bool operator!=(const qt<unit, T>,
                                               const qt<unit, T>);

    template <typename Unit, typename Scal>
    TFEL_MATH_INLINE constexpr Scal abs(const qt<Unit, Scal>&);

    namespace stdfunctions {

      template <int N, typename Unit>
      TFEL_MATH_INLINE qt<typename PowUnit_<N, 1u, Unit>::type, float> power(
          const qt<Unit, float>);

      template <int N, typename Unit>
      TFEL_MATH_INLINE qt<typename PowUnit_<N, 1u, Unit>::type, double> power(
          const qt<Unit, double>);

      template <int N, typename Unit>
      TFEL_MATH_INLINE qt<typename PowUnit_<N, 1u, Unit>::type, long double>
      power(const qt<Unit, long double>);

      template <int N, typename Unit>
      TFEL_MATH_INLINE qt<typename PowUnit_<N, 1u, Unit>::type, Complex<float>>
      power(const qt<Unit, Complex<float>>);

      template <int N, typename Unit>
      TFEL_MATH_INLINE qt<typename PowUnit_<N, 1u, Unit>::type, Complex<double>>
      power(const qt<Unit, Complex<double>>);

      template <int N, typename Unit>
      TFEL_MATH_INLINE
          qt<typename PowUnit_<N, 1u, Unit>::type, Complex<long double>>
          power(const qt<Unit, Complex<long double>>);

    }  // end of namespace stdfunctions

  }  // namespace math

}  // end of namespace tfel

#include "TFEL/Math/Quantity/qtLimits.hxx"
#include "TFEL/Math/Quantity/qtSpecific.hxx"
#include "TFEL/Math/Quantity/qtSamples.hxx"
#include "TFEL/Math/Quantity/qt.ixx"
#include "TFEL/Math/Quantity/qtOperations.hxx"

#endif /* LIB_TFEL_MATH_QT_HXX */
