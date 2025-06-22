/*!
 * \file   include/TFEL/Math/power.ixx
 * \brief  This file implements methods of class Power.
 * \author Helfer Thomas
 * \date   04 Jan 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_POWER_I_
#define LIB_TFEL_MATH_POWER_I_

#include <ratio>
#include <cmath>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/StaticAssert.hxx"

namespace tfel {

  namespace math {

    template <int N, unsigned int D>
    struct TFEL_VISIBILITY_LOCAL PowerGenerator {
      template <typename T>
      static TFEL_MATH_INLINE T exe(const T& x) {
        return std::pow(x, static_cast<T>(N) / static_cast<T>(D));
      }
    };

    template <unsigned int N, unsigned int M>
    struct PowerPosImpl;

    template <unsigned int N>
    class TFEL_VISIBILITY_LOCAL PowerPos : public PowerPosImpl<N / 4, N % 4> {};

    template <unsigned int N, unsigned int M>
    struct TFEL_VISIBILITY_LOCAL PowerPosImpl {
      template <typename T>
      static TFEL_MATH_INLINE T exe(const T& x) {
        T tmp = PowerPos<N>::exe(x);
        T tmp2 = PowerPos<M>::exe(x);
        return tmp * tmp * tmp * tmp * tmp2;
      }
    };

    template <unsigned int N>
    struct TFEL_VISIBILITY_LOCAL PowerPosImpl<N, 0u> {
      template <typename T>
      static TFEL_MATH_INLINE T exe(const T& x) {
        const T tmp = PowerPos<N>::exe(x);
        return tmp * tmp * tmp * tmp;
      }
    };

    template <unsigned int M>
    struct TFEL_VISIBILITY_LOCAL PowerPosImpl<0u, M> {
      template <typename T>
      static TFEL_MATH_INLINE T exe(const T& x) {
        return PowerPos<M>::exe(x);
      }
    };

    template <>
    struct TFEL_VISIBILITY_LOCAL PowerPosImpl<0u, 0u> {
      template <typename T>
      static TFEL_MATH_INLINE T exe(const T&) {
        return T(1);
      }
    };

    template <>
    struct TFEL_VISIBILITY_LOCAL PowerPos<0u> {
      template <typename T>
      static TFEL_MATH_INLINE T exe(const T&) {
        return T(1);
      }
    };

    template <>
    struct TFEL_VISIBILITY_LOCAL PowerPos<1u> {
      template <typename T>
      static TFEL_MATH_INLINE T exe(const T& x) {
        return x;
      }
    };

    template <>
    struct TFEL_VISIBILITY_LOCAL PowerPos<2u> {
      template <typename T>
      static TFEL_MATH_INLINE T exe(const T& x) {
        return x * x;
      }
    };

    template <>
    struct TFEL_VISIBILITY_LOCAL PowerPos<3u> {
      template <typename T>
      static TFEL_MATH_INLINE T exe(const T& x) {
        return x * x * x;
      }
    };

    template <int N>
    struct TFEL_VISIBILITY_LOCAL PowerNeg {
      template <typename T>
      static TFEL_MATH_INLINE T exe(const T& x) {
        return PowerPos<-N>::exe(T(1) / x);
      }
    };

    template <int N>
    struct TFEL_VISIBILITY_LOCAL PowerSqrtNeg {
      template <typename T>
      static TFEL_MATH_INLINE T exe(const T& x) {
        return std::sqrt(PowerPos<-N>::exe(T(1) / x));
      }
    };

    template <int N>
    class TFEL_VISIBILITY_LOCAL PowerSqrtPos {
      TFEL_STATIC_ASSERT(N >= 0);

     public:
      template <typename T>
      static TFEL_MATH_INLINE T exe(const T& x) {
        return std::sqrt(PowerPos<N>::exe(x));
      }
    };

    template <int N, unsigned int D>
    class PowerImplSelector;

    template <int N>
    class TFEL_VISIBILITY_LOCAL PowerImplSelector<N, 1u> {
      static constexpr unsigned short AbsN =
          static_cast<unsigned short>((N < 0) ? -N : N);

      typedef
          typename std::conditional<(N < 0), PowerNeg<N>, PowerPos<AbsN>>::type
              PowerImplSpe;

     public:
      typedef typename std::conditional<(AbsN > 100),
                                        PowerGenerator<N, 1u>,
                                        PowerImplSpe>::type type;
    };

    template <int N>
    class TFEL_VISIBILITY_LOCAL PowerImplSelector<N, 2u> {
      static constexpr unsigned short AbsN =
          static_cast<unsigned short>((N < 0) ? -N : N);

      typedef typename std::conditional<(N < 0),
                                        PowerSqrtNeg<N>,
                                        PowerSqrtPos<N>>::type PowerImplSpe;

     public:
      typedef typename std::conditional<(AbsN > 100),
                                        PowerGenerator<N, 2u>,
                                        PowerImplSpe>::type type;
    };

    template <int N, unsigned int D>
    class TFEL_VISIBILITY_LOCAL PowerImplSelector {
      TFEL_STATIC_ASSERT(D != 0);

      static constexpr int N_ = std::ratio<N, D>::num;
      static constexpr int D_ = std::ratio<N, D>::den;

     public:
      // On fait un choix sur les valeurs de D
      typedef typename std::conditional<
          D_ == 1u,
          typename PowerImplSelector<N_, 1u>::type,
          typename std::conditional<D_ == 2u,
                                    typename PowerImplSelector<N_, 2u>::type,
                                    PowerGenerator<N_, D_>>::type>::type type;
    };

    template <int N, typename T>
    typename std::enable_if<tfel::typetraits::IsScalar<T>::cond, T>::type power(
        const T x) {
      typedef
          typename tfel::math::PowerImplSelector<N, 1u>::type Implementation;
      return Implementation::exe(x);
    }

    template <int N, unsigned int D, typename T>
    typename std::enable_if<tfel::typetraits::IsScalar<T>::cond, T>::type power(
        const T x) {
      typedef typename tfel::math::PowerImplSelector<N, D>::type Implementation;
      return Implementation::exe(x);
    }

    namespace stdfunctions {

      // for backward compatibility with TFEL <= 3.0.x
      using tfel::math::power;

    }  // end of namespace stdfunctions

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_POWER_I_ */
