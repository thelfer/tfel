/*!
 * \file   TFEL/Math/NumericalIntegration/GaussKronrodQuadrature.ixx
 * \brief
 * \author Thomas Helfer
 * \date   08/10/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_NUMERICALINTEGRATION_GAUSSKRONRODQUADRATURE_IXX
#define LIB_TFEL_MATH_NUMERICALINTEGRATION_GAUSSKRONRODQUADRATURE_IXX

#include "TFEL/Math/ExpressionTemplates/Expr.hxx"

namespace tfel::math {

  constexpr auto GaussKronrodQuadrature::eval_if_required(const auto &r) {
    if constexpr (ExpressionConcept<std::decay_t<decltype(r)>>) {
      return eval(r);
    } else {
      return r;
    }
  }

  template <ScalarConcept real, typename FunctionType>
  constexpr auto GaussKronrodQuadrature::operator()(
      const FunctionType &f,                                     //
      const real a,                                              //
      const real b,                                              //
      const NumericalParameters<real> params) const              //
      noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
      -> std::optional<
          tfel::math::result_type<real,
                                  std::invoke_result_t<FunctionType, real>,
                                  OpMult>>  //
  requires(std::is_invocable_v<FunctionType, real>) {
    using result_type = std::optional<tfel::math::result_type<
        real, std::invoke_result_t<FunctionType, real>, OpMult>>;
    constexpr auto zero = real{};
    auto change_sign = [](const result_type ores) noexcept -> result_type {
      if (!ores.has_value()) {
        return {};
      }
      return -(*ores);
    };
    auto is_nan = [](const real x) {
      if constexpr (isQuantity<real>()) {
        return ieee754::isnan(x.getValue());
      } else {
        return ieee754::isnan(x);
      }
    };
    if (is_nan(a) || is_nan(b)) {
      return {};
    }
    auto is_infinite = [](const real x) {
      if constexpr (isQuantity<real>()) {
        return (ieee754::fpclassify(x.getValue()) == FP_INFINITE) ||
               (x >= std::numeric_limits<base_type<real>>::max()) ||
               (x <= -std::numeric_limits<base_type<real>>::max());
      } else {
        return (ieee754::fpclassify(x) == FP_INFINITE) ||
               (x >= std::numeric_limits<real>::max()) ||
               (x <= -std::numeric_limits<real>::max());
      }
    };
    //
    if (is_infinite(a)) {
      if (is_infinite(b)) {
        if (((a > zero) && (b > zero)) || ((a < zero) && (b < zero))) {
          return {};
        }
        if (b < zero) {
          //- int f(b,a);
          return change_sign(
              this->template computeUnboundedIntegral<real>(f, params));
        } else {
          return this->template computeUnboundedIntegral<real>(f, params);
        }
      } else {
        if (a > zero) {
          return change_sign(this->computeRightUnboundedIntegral(f, b, params));
        } else {
          return this->computeLeftUnboundedIntegral(f, b, params);
        }
      }
    }
    if (is_infinite(b)) {
      if (b < zero) {
        return change_sign(this->computeLeftUnboundedIntegral(f, a, params));
      } else {
        return this->computeRightUnboundedIntegral(f, a, params);
      }
    }
    // both bounds are finite
    if (a > b) {
      const auto ores = this->integrate(f, b, a, params);
      if (ores.has_value()) {
        return -(*ores);
      }
      return {};
    }
    return this->integrate(f, a, b, params);
  }  // end of operator()

  template <ScalarConcept real, typename FunctionType>
  constexpr auto GaussKronrodQuadrature::operator()(const FunctionType &f,
                                                    const real a,
                                                    const real b) const  //
      noexcept(std::is_nothrow_invocable_v<FunctionType, real>)          //
      -> std::optional<std::tuple<
          tfel::math::result_type<real,
                                  std::invoke_result_t<FunctionType, real>,
                                  OpMult>,
          real>>  //
  requires(std::is_invocable_v<FunctionType, real>) {
    using result_type = std::optional<
        std::tuple<tfel::math::result_type<
                       real, std::invoke_result_t<FunctionType, real>, OpMult>,
                   real>>;
    constexpr auto zero = real{};
    auto change_sign = [](const result_type ores) noexcept -> result_type {
      if (!ores.has_value()) {
        return {};
      }
      return std::make_tuple(-std::get<0>(*ores), std::get<1>(*ores));
    };
    auto is_nan = [](const real x) {
      if constexpr (isQuantity<real>()) {
        return ieee754::isnan(x.getValue());
      } else {
        return ieee754::isnan(x);
      }
    };
    if (is_nan(a) || is_nan(b)) {
      return {};
    }
    auto is_infinite = [](const real x) {
      if constexpr (isQuantity<real>()) {
        return ieee754::fpclassify(x.getValue()) == FP_INFINITE;
      } else {
        return ieee754::fpclassify(x) == FP_INFINITE;
      }
    };
    //
    if (is_infinite(a)) {
      if (is_infinite(b)) {
        if (((a > zero) && (b > zero)) || ((a < zero) && (b < zero))) {
          return {};
        }
        if (b < zero) {
          //- int f(b,a);
          return change_sign(this->template computeUnboundedIntegral<real>(f));
        } else {
          return this->template computeUnboundedIntegral<real>(f);
        }
      } else {
        if (a > zero) {
          return change_sign(this->computeRightUnboundedIntegral(f, b));
        } else {
          return this->computeLeftUnboundedIntegral(f, b);
        }
      }
    }
    if (is_infinite(b)) {
      if (b < zero) {
        return change_sign(this->computeLeftUnboundedIntegral(f, a));
      } else {
        return this->computeRightUnboundedIntegral(f, a);
      }
    }
    // both bounds are finite
    if (a > b) {
      const auto [i, e] = this->integrate(f, b, a);
      return std::make_tuple(-i, e);
    }
    return this->integrate(f, a, b);
  }  // end of operator()

  template <ScalarConcept real, typename FunctionType>
  constexpr auto GaussKronrodQuadrature::integrate(
      const FunctionType &f,
      const real a,
      const real b,
      const NumericalParameters<real> &params) const             //
      noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
      -> std::optional<
          tfel::math::result_type<real,
                                  std::invoke_result_t<FunctionType, real>,
                                  OpMult>>  //
  requires(std::is_invocable_v<FunctionType, real>) {
    if (params.maximum_number_of_refinements == 0) {
      return {};
    }
    const auto [i, e] = this->integrate(f, a, b);
    if (e > params.absolute_tolerance) {
      const auto n = params.maximum_number_of_refinements - 1;
      const auto c = std::midpoint(a, b);
      const auto oi1 =
          this->integrate(f, a, c,
                          {.absolute_tolerance = params.absolute_tolerance / 2,
                           .maximum_number_of_refinements = n});
      const auto oi2 =
          this->integrate(f, c, b,
                          {.absolute_tolerance = params.absolute_tolerance / 2,
                           .maximum_number_of_refinements = n});
      if ((!oi1.has_value()) || (!oi2.has_value())) {
        return {};
      }
      return *oi1 + *oi2;
    }
    return i;
  }  // end of integrate

  template <ScalarConcept real, typename FunctionType>
  constexpr auto GaussKronrodQuadrature::integrate(const FunctionType &f,
                                                   const real a,
                                                   const real b) const  //
      noexcept(std::is_nothrow_invocable_v<FunctionType, real>)         //
      requires(std::is_invocable_v<FunctionType, real>) {
    using result_type = std::decay_t<std::invoke_result_t<FunctionType, real>>;
    // positions of Kronrod's quadrature points in [-1:1]
    // Those points are sorted so that pt2, pt4, pt6 and pt8 also corresponds to
    // Gauss' quadrature points.
    constexpr auto pt1 = static_cast<real>(0.991455371120813);
    constexpr auto pt2 = static_cast<real>(0.949107912342759);
    constexpr auto pt3 = static_cast<real>(0.864864423359769);
    constexpr auto pt4 = static_cast<real>(0.741531185599394);
    constexpr auto pt5 = static_cast<real>(0.586087235467691);
    constexpr auto pt6 = static_cast<real>(0.405845151377397);
    constexpr auto pt7 = static_cast<real>(0.207784955007898);
    constexpr auto pt8 = real{};
    // values of the function evaluated at Kronrod's quadature points.
    // those values will be reused to evaluate the integral using the Gauss'
    // quadrature rule.
    const auto values = [&f, a, b ]<std::size_t... Is>(
        std::integer_sequence<std::size_t, Is...>) {
      constexpr auto kronrod_pts =
          std::array<real, 15>{-pt1, -pt2, -pt3, -pt4, -pt5, -pt6, -pt7, pt8,
                               pt7,  pt6,  pt5,  pt4,  pt3,  pt2,  pt1};
      // function to shift the position of the quadrature points to [a, b]
      auto shift = [a, b](const real x) { return ((x + 1) / 2) * (b - a) + a; };
      // evaluation of the function
      return std::array<result_type, 15>{f(shift(kronrod_pts[Is]))...};
    }
    (std::make_integer_sequence<std::size_t, 15>{});
    // value of the integral using Kronrod's quadrature rule
    const auto k15 = [&values, a, b ]<std::size_t... Is>(
        std::integer_sequence<std::size_t, Is...>) {
      // Kronrod's weights in [-1: 1]
      constexpr auto kronrod_w1 = static_cast<real>(0.022935322010529);
      constexpr auto kronrod_w2 = static_cast<real>(0.063092092629979);
      constexpr auto kronrod_w3 = static_cast<real>(0.104790010322250);
      constexpr auto kronrod_w4 = static_cast<real>(0.140653259715525);
      constexpr auto kronrod_w5 = static_cast<real>(0.169004726639267);
      constexpr auto kronrod_w6 = static_cast<real>(0.190350578064785);
      constexpr auto kronrod_w7 = static_cast<real>(0.204432940075298);
      constexpr auto kronrod_w8 = static_cast<real>(0.209482141084728);
      constexpr auto kronrod_weights = std::array<real, 15>{
          kronrod_w1, kronrod_w2, kronrod_w3, kronrod_w4, kronrod_w5,
          kronrod_w6, kronrod_w7, kronrod_w8, kronrod_w7, kronrod_w6,
          kronrod_w5, kronrod_w4, kronrod_w3, kronrod_w2, kronrod_w1};
      // scale the weight of the to segment [a, b]
      auto scale = [a, b](const real w) { return (w / 2) * (b - a); };
      // approximation of the integral
      const auto r = (... + (scale(kronrod_weights[Is]) * values[Is]));
      return eval_if_required(r);
    }
    (std::make_integer_sequence<std::size_t, 15>{});
    // value of the integral using Gauss's quadrature rule
    const auto g7 = [&values, a, b ]<std::size_t... Is>(
        std::integer_sequence<std::size_t, Is...>) {
      constexpr auto gauss_w1 = static_cast<real>(0.129484966168870);
      constexpr auto gauss_w2 = static_cast<real>(0.279705391489277);
      constexpr auto gauss_w3 = static_cast<real>(0.381830050505119);
      constexpr auto gauss_w4 = static_cast<real>(0.417959183673469);
      constexpr auto gauss_weights = std::array<real, 7>{
          gauss_w1, gauss_w2, gauss_w3, gauss_w4, gauss_w3, gauss_w2, gauss_w1};
      // small functions giving the mapping between Kronrod's quadature points
      // and Gauss's quadrature points
      auto from_kronrod_to_gauss = [](const std::size_t i) {
        return 2 * i + 1;
      };
      // scale the weight of the to segment [a, b]
      auto scale = [a, b](const real w) { return (w / 2) * (b - a); };
      // approximation of the integral
      const auto r = (... + (scale(gauss_weights[Is]) *
                             values[from_kronrod_to_gauss(Is)]));
      return eval_if_required(r);
    }
    (std::make_integer_sequence<std::size_t, 7>{});
    //
    const auto e = [&k15, &g7] {
      const auto r = k15 - g7;
      return eval_if_required(r);
    }();
    return std::make_tuple(k15, tfel::math::abs(e));
  }

  template <ScalarConcept real, typename FunctionType>
  constexpr auto GaussKronrodQuadrature::computeUnboundedIntegral(
      const FunctionType &f) const                               //
      noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
      requires(std::is_invocable_v<FunctionType, real>) {
    auto u = [f](const real t) noexcept(
                 std::is_nothrow_invocable_v<FunctionType, real>) {
      const auto t_sq = t * t;
      const auto inv = 1 / (1 - t_sq);
      const auto w = (1 + t_sq) * inv * inv;
      const auto arg = t * inv;
      return f(arg) * w;
    };
    return this->integrate(u, real{-1}, real{1});
  }  // end of computeUnboundedIntegral

  template <ScalarConcept real, typename FunctionType>
  constexpr auto GaussKronrodQuadrature::computeLeftUnboundedIntegral(
      const FunctionType &f,
      const real b) const                                        //
      noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
      requires(std::is_invocable_v<FunctionType, real>) {
    auto u = [f, b](const real t) noexcept(
                 std::is_nothrow_invocable_v<FunctionType, real>) {
      const auto z = 1 / (t + 1);
      const auto arg = 2 * z - 1;
      return f(b - arg) * z * z;
    };
    const auto [i, e] = this->integrate(u, real{-1}, real{1});
    return std::make_tuple(2 * i, e);
  }  // end of computeLeftUnboundedIntegral

  template <ScalarConcept real, typename FunctionType>
  constexpr auto GaussKronrodQuadrature::computeRightUnboundedIntegral(
      const FunctionType &f,
      const real a) const                                        //
      noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
      requires(std::is_invocable_v<FunctionType, real>) {
    auto u = [&](const real t) noexcept(
                 std::is_nothrow_invocable_v<FunctionType, real>) {
      const auto z = 1 / (t + 1);
      const auto arg = 2 * z + a - 1;
      return f(arg) * z * z;
    };
    const auto [i, e] = this->integrate(u, real{-1}, real{1});
    return std::make_tuple(2 * i, e);
  }  // end of computeRightUnboundedIntegral

  template <ScalarConcept real, typename FunctionType>
  constexpr auto GaussKronrodQuadrature::computeUnboundedIntegral(
      const FunctionType &f,
      const NumericalParameters<real> &params) const             //
      noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
      requires(std::is_invocable_v<FunctionType, real>) {
    auto u = [f](const real t) noexcept(
                 std::is_nothrow_invocable_v<FunctionType, real>) {
      const real t_sq = t * t;
      const real inv = 1 / (1 - t_sq);
      const real w = (1 + t_sq) * inv * inv;
      const real arg = t * inv;
      return eval_if_required(f(arg) * w);
    };
    return this->integrate(u, real{-1}, real{1}, params);
  }  // end of computeUnboundedIntegral

  template <ScalarConcept real, typename FunctionType>
  constexpr auto GaussKronrodQuadrature::computeLeftUnboundedIntegral(
      const FunctionType &f,
      const real b,
      const NumericalParameters<real> &params) const             //
      noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
      -> std::optional<
          tfel::math::result_type<real,
                                  std::invoke_result_t<FunctionType, real>,
                                  OpMult>>
  requires(std::is_invocable_v<FunctionType, real>) {
    auto u = [f, b](const real t) noexcept(
                 std::is_nothrow_invocable_v<FunctionType, real>) {
      const auto z = 1 / (t + 1);
      const auto arg = 2 * z - 1;
      return eval_if_required(f(b - arg) * z * z);
    };
    const auto ores = this->integrate(u, real{-1}, real{1}, params);
    if (ores.has_value()) {
      return 2 * (*ores);
    }
    return {};
  }  // end of computeLeftUnboundedIntegral

  template <ScalarConcept real, typename FunctionType>
  constexpr auto GaussKronrodQuadrature::computeRightUnboundedIntegral(
      const FunctionType &f,
      const real a,
      const NumericalParameters<real> &params) const             //
      noexcept(std::is_nothrow_invocable_v<FunctionType, real>)  //
      -> std::optional<
          tfel::math::result_type<real,
                                  std::invoke_result_t<FunctionType, real>,
                                  OpMult>>
  requires(std::is_invocable_v<FunctionType, real>) {
    auto u = [&](const real t) noexcept(
                 std::is_nothrow_invocable_v<FunctionType, real>) {
      const auto z = 1 / (t + 1);
      const auto arg = 2 * z + a - 1;
      return eval_if_required(f(arg) * z * z);
    };
    const auto ores = this->integrate(u, real{-1}, real{1}, params);
    if (ores.has_value()) {
      return 2 * (*ores);
    }
    return {};
  }  // end of computeRightUnboundedIntegral

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_NUMERICALINTEGRATION_GAUSSKRONRODQUADRATURE_HXX */
