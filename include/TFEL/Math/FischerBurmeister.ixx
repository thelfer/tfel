/*!
 * \file   include/TFEL/MATH/FischerBurmeister.ixx
 * \brief    
 * \author Thomas Helfer
 * \date   29/05/2019
 */

#ifndef LIB_TFEL_MATH_FISCHERBURMEISTER_IXX
#define LIB_TFEL_MATH_FISCHERBURMEISTER_IXX

#include "TFEL/Math/General/IEEE754.hxx"

namespace tfel{

  namespace math {

    template <typename real>
    real regularisedFischerBurmeisterFunction(const real& x, const real& y, const real& e) {
      return x + y - std::sqrt(x * x + y * y + e * e);
    }  // end of FischerBurmeisterFunction

    template <typename real>
    std::tuple<real, real> regularisedFischerBurmeisterFunctionFirstDerivatives(const real& x,
                                                                                const real& y,
                                                                                const real& e) {
      const auto is = 1 / std::sqrt(x * x + y * y + e * e);
      return {1 - x * is, 1 - y * is};
    }  // end of regularisedFischerBurmeisterFunctionFirstDerivatives

    template <typename real>
    real squaredFischerBurmeisterFunction(const real& x, const real& y) {
      const auto f = regularisedFischerBurmeisterFunction(x, y, real(0));
      return f * f;
    }  // end of squaredFischerBurmeisterFunction

    template <typename real>
    std::tuple<real,real> squaredFischerBurmeisterFunctionFirstDerivatives(const real& x, const real& y) {
      const auto df_dx = [&x, &y] {
        constexpr auto zero = real{0};
        if (tfel::math::ieee754::fpclassify(x) == FP_ZERO) {
          return (y > zero) ? 4 * y : zero;
        }
        const auto s = std::sqrt(x * x + y * y);
        return 2 * (1 - x / s) * (x + y - s);
      }();
      const auto df_dy = [&x, &y] {
        constexpr auto zero = real{0};
        if (tfel::math::ieee754::fpclassify(y) == FP_ZERO) {
          return (x > zero) ? 4 * x : zero;
        }
        const auto s = std::sqrt(x * x + y * y);
        return 2 * (1 - y / s) * (x + y - s);
      }();
      return {df_dx, df_dy};
    }  // end of squaredFischerBurmeisterFunctionFirstDerivatives

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_FISCHERBURMEISTER_IXX */
