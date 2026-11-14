/*!
 * \file   include/TFEL/MATH/FischerBurmeister.ixx
 * \brief
 * \author Thomas Helfer
 * \date   29/05/2019
 */

#ifndef LIB_TFEL_MATH_FISCHERBURMEISTER_IXX
#define LIB_TFEL_MATH_FISCHERBURMEISTER_IXX

#include "TFEL/Math/General/IEEE754.hxx"

namespace tfel::math {

  template <typename real>
  real regularisedFischerBurmeisterFunction(const real& x,
                                            const real& y,
                                            const real& e) {
    return x + y - std::sqrt(x * x + y * y + e * e);
  }  // end of FischerBurmeisterFunction

  template <typename real>
  std::tuple<real, real> regularisedFischerBurmeisterFunctionFirstDerivatives(
      const real& x, const real& y, const real& e) {
    const auto is = 1 / std::sqrt(x * x + y * y + e * e);
    return {1 - x * is, 1 - y * is};
  }  // end of regularisedFischerBurmeisterFunctionFirstDerivatives

  template <typename real>
  real squaredFischerBurmeisterFunction(const real& x, const real& y) {
    const auto f = regularisedFischerBurmeisterFunction(x, y, real(0));
    return f * f;
  }  // end of squaredFischerBurmeisterFunction

  template <typename real>
  std::tuple<real, real> squaredFischerBurmeisterFunctionFirstDerivatives(
      const real& x, const real& y) {
    constexpr auto zero = real{0};
    const auto s = std::sqrt(x * x + y * y);
    if (tfel::math::ieee754::fpclassify(s) == FP_ZERO) {
      return {zero, zero};
    }
    const auto f = x + y - s;
    return {2 * (1 - x / s) * f, 2 * (1 - y / s) * f};
  }  // end of squaredFischerBurmeisterFunctionFirstDerivatives

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FISCHERBURMEISTER_IXX */
