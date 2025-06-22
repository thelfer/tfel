/*!
 * \file   include/TFEL/Math/power.ixx
 * \brief  This file implements methods of class Power.
 * \author Thomas Helfer
 * \date   04 Jan 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_POWER_IXX
#define LIB_TFEL_MATH_POWER_IXX

#include <ratio>
#include <cmath>
#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::math::internals {

  template <int N, unsigned int D>
  struct TFEL_VISIBILITY_LOCAL PowerGenerator {
    template <typename T>
    static constexpr T exe(const T& x) {
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
    static constexpr T exe(const T& x) {
      const auto tmp = PowerPos<N>::exe(x);
      const auto tmp2 = PowerPos<M>::exe(x);
      return tmp * tmp * tmp * tmp * tmp2;
    }
  };

  template <unsigned int N>
  struct TFEL_VISIBILITY_LOCAL PowerPosImpl<N, 0u> {
    template <typename T>
    static constexpr T exe(const T& x) {
      const auto tmp = PowerPos<N>::exe(x);
      return tmp * tmp * tmp * tmp;
    }
  };

  template <unsigned int M>
  struct TFEL_VISIBILITY_LOCAL PowerPosImpl<0u, M> {
    template <typename T>
    static constexpr T exe(const T& x) {
      return PowerPos<M>::exe(x);
    }
  };

  template <>
  struct TFEL_VISIBILITY_LOCAL PowerPosImpl<0u, 0u> {
    template <typename T>
    static constexpr T exe(const T&) {
      return {1};
    }
  };

  template <>
  struct TFEL_VISIBILITY_LOCAL PowerPos<0u> {
    template <typename T>
    static constexpr T exe(const T&) {
      return {1};
    }
  };

  template <>
  struct TFEL_VISIBILITY_LOCAL PowerPos<1u> {
    template <typename T>
    static constexpr T exe(const T& x) {
      return x;
    }
  };

  template <>
  struct TFEL_VISIBILITY_LOCAL PowerPos<2u> {
    template <typename T>
    static constexpr T exe(const T& x) {
      return x * x;
    }
  };

  template <>
  struct TFEL_VISIBILITY_LOCAL PowerPos<3u> {
    template <typename T>
    static constexpr T exe(const T& x) {
      return x * x * x;
    }
  };
  /*!
   * \brief an helper structure to compute the negative power of a
   * number.
   * \tparam N: exponent
   */
  template <int N>
  struct TFEL_VISIBILITY_LOCAL PowerNeg {
    template <typename T>
    static constexpr T exe(const T& x) {
      static_assert(N < 0, "invalid exponent");
      constexpr auto opposite = static_cast<unsigned int>(-N);
      constexpr auto one = T{1};
      return PowerPos<opposite>::exe(one / x);
    }
  };

  template <int N>
  struct TFEL_VISIBILITY_LOCAL PowerSqrtNeg {
    template <typename T>
    static constexpr T exe(const T& x) {
      static_assert(N < 0, "invalid exponent");
      constexpr auto opposite = static_cast<unsigned int>(-N);
      constexpr auto one = T{1};
      return std::sqrt(PowerPos<opposite>::exe(one / x));
    }
  };

  template <unsigned int N>
  class TFEL_VISIBILITY_LOCAL PowerSqrtPos {
    static_assert(N >= 0);

   public:
    template <typename T>
    static constexpr T exe(const T& x) {
      return std::sqrt(PowerPos<N>::exe(x));
    }
  };

  template <int N, unsigned int D>
  class PowerImplSelector;

  template <int N>
  class TFEL_VISIBILITY_LOCAL PowerImplSelector<N, 1u> {
    static constexpr unsigned short AbsN =
        static_cast<unsigned short>((N < 0) ? -N : N);

    using PowerImplSpe =
        std::conditional_t<(N < 0), PowerNeg<N>, PowerPos<AbsN>>;

   public:
    using type =
        std::conditional_t<(AbsN > 100), PowerGenerator<N, 1u>, PowerImplSpe>;
  };

  template <int N>
  class TFEL_VISIBILITY_LOCAL PowerImplSelector<N, 2u> {
    static constexpr unsigned short AbsN =
        static_cast<unsigned short>((N < 0) ? -N : N);

    using PowerImplSpe =
        std::conditional_t<(N < 0),
                           PowerSqrtNeg<N>,
                           PowerSqrtPos<static_cast<unsigned int>(N)>>;

   public:
    using type =
        std::conditional_t<(AbsN > 100), PowerGenerator<N, 2u>, PowerImplSpe>;
  };

  template <int N, unsigned int D>
  class TFEL_VISIBILITY_LOCAL PowerImplSelector {
    static_assert(D != 0);

    static constexpr auto N_ = std::ratio<N, D>::num;
    static constexpr auto D_ = static_cast<unsigned int>(std::ratio<N, D>::den);

   public:
    // On fait un choix sur les valeurs de D
    using type = std::conditional_t<
        D_ == 1u,
        typename PowerImplSelector<N_, 1u>::type,
        std::conditional_t<D_ == 2u,
                           typename PowerImplSelector<N_, 2u>::type,
                           PowerGenerator<N_, D_>>>;
  };

}  // namespace tfel::math::internals

namespace tfel::math {

  template <int N, typename T>
  constexpr std::enable_if_t<std::is_floating_point_v<T>, T> power(const T x) {
    return tfel::math::internals::PowerImplSelector<N, 1u>::type::exe(x);
  }

  template <int N, unsigned int D, typename T>
  constexpr std::enable_if_t<std::is_floating_point_v<T>, T> power(const T x) {
    return tfel::math::internals::PowerImplSelector<N, D>::type::exe(x);
  }

  namespace stdfunctions {

    // for backward compatibility with TFEL <= 3.0.x
    using tfel::math::power;

  }  // end of namespace stdfunctions

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_POWER_IXX */
