/*!
 * \file   include/TFEL/Math/Quantity/qt.ixx
 * \brief  This file implements the methods defined in the qt class.
 * \author Thomas Helfer
 * \date   09 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_QT_I_
#define LIB_TFEL_QT_I_

#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/UnaryResultType.hxx"

namespace tfel {

  namespace math {

    template <typename unit, typename T>
    T& qt<unit, T>::getValue() noexcept {
      return this->value;
    }

    template <typename unit, typename T>
    constexpr const T& qt<unit, T>::getValue() const noexcept {
      return value;
    }

    template <typename unit, typename T>
    qt<unit, T>& qt<unit, T>::operator=(const qt<unit, T>& src) {
      this->value = src.getValue();
      return *this;
    }

    template <typename unit, typename T>
    template <typename T2>
    typename std::enable_if<
        std::is_same<typename tfel::typetraits::Promote<T, T2>::type, T>::value,
        qt<unit, T>&>::type
    qt<unit, T>::operator=(const qt<unit, T2>& src) {
      this->value = src.getValue();
      return *this;
    }

    template <typename unit, typename T>
    template <typename T2>
    typename std::enable_if<
        std::is_same<typename tfel::typetraits::Promote<T, T2>::type, T>::value,
        qt<unit, T>&>::type
    qt<unit, T>::operator+=(const qt<unit, T2>& src) {
      this->value += src.getValue();
      return *this;
    }

    template <typename unit, typename T>
    template <typename T2>
    typename std::enable_if<
        std::is_same<typename tfel::typetraits::Promote<T, T2>::type, T>::value,
        qt<unit, T>&>::type
    qt<unit, T>::operator-=(const qt<unit, T2>& src) {
      this->value -= src.getValue();
      return *this;
    }

    template <typename unit, typename T>
    template <typename T2>
    typename std::enable_if<IsQtScalarOperationValid<T, T2>::cond,
                            qt<unit, T>&>::type
    qt<unit, T>::operator*=(const T2 a) {
      this->value *= a;
      return *this;
    }

    template <typename unit, typename T>
    template <typename T2>
    typename std::enable_if<IsQtScalarOperationValid<T, T2>::cond,
                            qt<unit, T>&>::type
    qt<unit, T>::operator/=(const T2 a) {
      this->value /= a;
      return *this;
    }

    template <typename T>
    T& qt<NoUnit, T>::getValue() noexcept {
      return value;
    }

    template <typename T>
    constexpr const T& qt<NoUnit, T>::getValue() const noexcept {
      return value;
    }

    template <typename T>
    qt<NoUnit, T>& qt<NoUnit, T>::operator=(const qt<NoUnit, T>& src) {
      this->value = src.getValue();
      return *this;
    }

    template <typename T>
    template <typename T2>
    typename std::enable_if<
        std::is_same<typename tfel::typetraits::Promote<T, T2>::type, T>::value,
        qt<NoUnit, T>&>::type
    qt<NoUnit, T>::operator=(const qt<NoUnit, T2>& src) {
      this->value = src.getValue();
      return *this;
    }

    template <typename T>
    template <typename T2>
    typename std::enable_if<IsConvertibleToQtNoUnit<T, T2>::cond,
                            qt<NoUnit, T>&>::type
    qt<NoUnit, T>::operator=(const T2 src) {
      this->value = src;
      return *this;
    }

    template <typename T>
    template <typename T2>
    typename std::enable_if<
        std::is_same<typename tfel::typetraits::Promote<T, T2>::type, T>::value,
        qt<NoUnit, T>&>::type
    qt<NoUnit, T>::operator+=(const qt<NoUnit, T2>& src) {
      this->value += src.getValue();
      return *this;
    }

    template <typename T>
    template <typename T2>
    typename std::enable_if<
        std::is_same<typename tfel::typetraits::Promote<T, T2>::type, T>::value,
        qt<NoUnit, T>&>::type
    qt<NoUnit, T>::operator-=(const qt<NoUnit, T2>& src) {
      this->value -= src.getValue();
      return *this;
    }

    template <typename T>
    template <typename T2>
    typename std::enable_if<IsConvertibleToQtNoUnit<T, T2>::cond,
                            qt<NoUnit, T>&>::type
    qt<NoUnit, T>::operator*=(const T2 a) {
      this->value *= a;
      return *this;
    }

    template <typename T>
    template <typename T2>
    typename std::enable_if<IsConvertibleToQtNoUnit<T, T2>::cond,
                            qt<NoUnit, T>&>::type
    qt<NoUnit, T>::operator/=(const T2 a) {
      this->value /= a;
      return *this;
    }

    template <typename T>
    constexpr qt<NoUnit, T>::operator T() const {
      return this->value;
    }

    template <typename unit, typename T>
    TFEL_MATH_INLINE constexpr bool operator<(const qt<unit, T> a,
                                              const qt<unit, T> b) {
      return a.getValue() < b.getValue();
    }

    template <typename unit, typename T>
    TFEL_MATH_INLINE constexpr bool operator<=(const qt<unit, T> a,
                                               const qt<unit, T> b) {
      return a.getValue() <= b.getValue();
    }

    template <typename unit, typename T>
    TFEL_MATH_INLINE constexpr bool operator>(const qt<unit, T> a,
                                              const qt<unit, T> b) {
      return a.getValue() > b.getValue();
    }

    template <typename unit, typename T>
    TFEL_MATH_INLINE constexpr bool operator>=(const qt<unit, T> a,
                                               const qt<unit, T> b) {
      return a.getValue() >= b.getValue();
    }

    template <typename unit, typename T>
    TFEL_MATH_INLINE constexpr bool operator==(const qt<unit, T> a,
                                               const qt<unit, T> b) {
      return a.getValue() == b.getValue();
    }

    template <typename unit, typename T>
    TFEL_MATH_INLINE constexpr bool operator!=(const qt<unit, T> a,
                                               const qt<unit, T> b) {
      return a.getValue() != b.getValue();
    }

    template <typename Unit, typename Scal>
    TFEL_MATH_INLINE constexpr Scal abs(const qt<Unit, Scal>& s) {
      return std::abs(s.getValue());
    }

    namespace stdfunctions {

      template <int N, typename Unit>
      TFEL_MATH_INLINE
          tfel::math::qt<typename PowUnit_<N, 1u, Unit>::type, float>
          power(const tfel::math::qt<Unit, float> x) {
        typedef
            typename tfel::math::PowerImplSelector<N, 1u>::type Implementation;
        typedef tfel::math::qt<typename PowUnit_<N, 1u, Unit>::type, float> Res;
        return Res(Implementation::exe(x.getValue()));
      }

      template <int N, typename Unit>
      TFEL_MATH_INLINE
          tfel::math::qt<typename PowUnit_<N, 1u, Unit>::type, double>
          power(const tfel::math::qt<Unit, double> x) {
        typedef
            typename tfel::math::PowerImplSelector<N, 1u>::type Implementation;
        typedef tfel::math::qt<typename PowUnit_<N, 1u, Unit>::type, double>
            Res;
        return Res(Implementation::exe(x.getValue()));
      }

      template <int N, typename Unit>
      TFEL_MATH_INLINE
          tfel::math::qt<typename PowUnit_<N, 1u, Unit>::type, long double>
          power(const tfel::math::qt<Unit, long double> x) {
        typedef
            typename tfel::math::PowerImplSelector<N, 1u>::type Implementation;
        typedef tfel::math::qt<typename PowUnit_<N, 1u, Unit>::type,
                               long double>
            Res;
        return Res(Implementation::exe(x.getValue()));
      }

      template <int N, typename Unit>
      TFEL_MATH_INLINE
          tfel::math::qt<typename PowUnit_<N, 1u, Unit>::type, Complex<float>>
          power(const tfel::math::qt<Unit, Complex<float>> x) {
        typedef
            typename tfel::math::PowerImplSelector<N, 1u>::type Implementation;
        typedef tfel::math::qt<typename PowUnit_<N, 1u, Unit>::type,
                               Complex<float>>
            Res;
        return Res(Implementation::exe(x.getValue()));
      }

      template <int N, typename Unit>
      TFEL_MATH_INLINE
          tfel::math::qt<typename PowUnit_<N, 1u, Unit>::type, Complex<double>>
          power(const tfel::math::qt<Unit, Complex<double>> x) {
        typedef
            typename tfel::math::PowerImplSelector<N, 1u>::type Implementation;
        typedef tfel::math::qt<typename PowUnit_<N, 1u, Unit>::type,
                               Complex<double>>
            Res;
        return Res(Implementation::exe(x.getValue()));
      }

      template <int N, typename Unit>
      TFEL_MATH_INLINE tfel::math::qt<typename PowUnit_<N, 1u, Unit>::type,
                                      Complex<long double>>
      power(const tfel::math::qt<Unit, Complex<long double>> x) {
        typedef
            typename tfel::math::PowerImplSelector<N, 1u>::type Implementation;
        typedef tfel::math::qt<typename PowUnit_<N, 1u, Unit>::type,
                               Complex<long double>>
            Res;
        return Res(Implementation::exe(x.getValue()));
      }

      template <int N, unsigned int D, typename Unit>
      TFEL_MATH_INLINE
          tfel::math::qt<typename PowUnit_<N, D, Unit>::type, float>
          power(const tfel::math::qt<Unit, float> x) {
        typedef
            typename tfel::math::PowerImplSelector<N, D>::type Implementation;
        typedef tfel::math::qt<typename PowUnit_<N, D, Unit>::type, float> Res;
        return Res(Implementation::exe(x.getValue()));
      }

      template <int N, unsigned int D, typename Unit>
      TFEL_MATH_INLINE
          tfel::math::qt<typename PowUnit_<N, D, Unit>::type, double>
          power(const tfel::math::qt<Unit, double> x) {
        typedef
            typename tfel::math::PowerImplSelector<N, D>::type Implementation;
        typedef tfel::math::qt<typename PowUnit_<N, D, Unit>::type, double> Res;
        return Res(Implementation::exe(x.getValue()));
      }

      template <int N, unsigned int D, typename Unit>
      TFEL_MATH_INLINE
          tfel::math::qt<typename PowUnit_<N, D, Unit>::type, long double>
          power(const tfel::math::qt<Unit, long double> x) {
        typedef
            typename tfel::math::PowerImplSelector<N, D>::type Implementation;
        typedef tfel::math::qt<typename PowUnit_<N, D, Unit>::type, long double>
            Res;
        return Res(Implementation::exe(x.getValue()));
      }

      template <int N, unsigned int D, typename Unit>
      TFEL_MATH_INLINE tfel::math::qt<typename PowUnit_<N, D, Unit>::type,
                                      tfel::math::Complex<float>>
      power(const tfel::math::qt<Unit, tfel::math::Complex<float>> x) {
        typedef
            typename tfel::math::PowerImplSelector<N, D>::type Implementation;
        typedef tfel::math::qt<typename PowUnit_<N, D, Unit>::type,
                               tfel::math::Complex<float>>
            Res;
        return Res(Implementation::exe(x.getValue()));
      }

      template <int N, unsigned int D, typename Unit>
      TFEL_MATH_INLINE tfel::math::qt<typename PowUnit_<N, D, Unit>::type,
                                      tfel::math::Complex<double>>
      power(const tfel::math::qt<Unit, tfel::math::Complex<double>> x) {
        typedef
            typename tfel::math::PowerImplSelector<N, D>::type Implementation;
        typedef tfel::math::qt<typename PowUnit_<N, D, Unit>::type,
                               tfel::math::Complex<double>>
            Res;
        return Res(Implementation::exe(x.getValue()));
      }

      template <int N, unsigned int D, typename Unit>
      TFEL_MATH_INLINE tfel::math::qt<typename PowUnit_<N, D, Unit>::type,
                                      tfel::math::Complex<long double>>
      power(const tfel::math::qt<Unit, tfel::math::Complex<long double>> x) {
        typedef
            typename tfel::math::PowerImplSelector<N, D>::type Implementation;
        typedef tfel::math::qt<typename PowUnit_<N, D, Unit>::type,
                               tfel::math::Complex<long double>>
            Res;
        return Res(Implementation::exe(x.getValue()));
      }

    }  // end of namespace stdfunctions

    template <int N, unsigned int D>
    struct Power;

    template <typename Unit, int N, unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<qt<Unit, float>, Power<N, D>> {
     public:
      typedef qt<typename PowUnit_<N, D, Unit>::type, float> type;
    };

    template <typename Unit, int N, unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<qt<Unit, double>, Power<N, D>> {
     public:
      typedef qt<typename PowUnit_<N, D, Unit>::type, double> type;
    };

    template <typename Unit, int N, unsigned int D>
    class TFEL_VISIBILITY_LOCAL
        UnaryResultType<qt<Unit, long double>, Power<N, D>> {
     public:
      typedef qt<typename PowUnit_<N, D, Unit>::type, long double> type;
    };

    template <typename Unit, int N, unsigned int D>
    class TFEL_VISIBILITY_LOCAL
        UnaryResultType<qt<Unit, Complex<float>>, Power<N, D>> {
     public:
      typedef qt<typename PowUnit_<N, D, Unit>::type, Complex<float>> type;
    };

    template <typename Unit, int N, unsigned int D>
    class TFEL_VISIBILITY_LOCAL
        UnaryResultType<qt<Unit, Complex<double>>, Power<N, D>> {
     public:
      typedef qt<typename PowUnit_<N, D, Unit>::type, Complex<double>> type;
    };

    template <typename Unit, int N, unsigned int D>
    class TFEL_VISIBILITY_LOCAL
        UnaryResultType<qt<Unit, Complex<long double>>, Power<N, D>> {
     public:
      typedef qt<typename PowUnit_<N, D, Unit>::type, Complex<long double>>
          type;
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_QT_I_ */
