/*!
 * \file   include/TFEL/Math/Newton-Raphson/ScalarNewtonRaphson.ixx
 * \brief
 * \author Thomas Helfer
 * \date   31/03/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_SCALARNEWTONRAPHSON_IXX
#define LIB_TFEL_MATH_SCALARNEWTONRAPHSON_IXX

#include "TFEL/Math/General/IEEE754.hxx"

namespace tfel {

  namespace math {

    template <typename Function,
              typename Criterion,
              typename NumericType,
              typename IndexType>
    std::tuple<bool, NumericType, IndexType> scalarNewtonRaphson(
        const Function& f,
        const Criterion& c,
        const NumericType x0,
        const IndexType im) {
      auto p = ScalarNewtonRaphsonParameters<NumericType, IndexType>{};
      p.x0 = x0;
      p.im = im;
      return scalarNewtonRaphson(f, c, p);
    }  // end of scalarNewtonRaphson

    template <typename Function,
              typename Criterion,
              typename NumericType,
              typename IndexType>
    std::tuple<bool, NumericType, IndexType> scalarNewtonRaphson(
        const Function& f,
        const Criterion& c,
        const ScalarNewtonRaphsonParameters<NumericType, IndexType>& p) {
      auto has_opposite_signs = [](const NumericType a, const NumericType b) {
        auto sgn = [](const NumericType value) {
          constexpr const auto zero = NumericType{};
          return (zero < value) - (value < zero);
        };
        return sgn(a) != sgn(b);
      };
      auto i = IndexType{};
      auto x = p.x0;
      auto xmin = p.xmin0;
      auto xmax = p.xmax0;
      if (xmax < xmin) {
        std::swap(xmin, xmax);
      }
      auto dx = NumericType{};
      if (i >= p.im) {
        return std::make_tuple(false, p.x0, i);
      }
#if __cplusplus >= 201703L
      auto[fv, dfv] = f(p.x0);
#else
      auto fdf = f(p.x0);
      auto fv = std::get<0>(fdf);
      auto dfv = std::get<1>(fdf);
#endif
      auto fxmin = std::numeric_limits<NumericType>::quiet_NaN();
      auto fxmax = std::numeric_limits<NumericType>::quiet_NaN();
      if (tfel::math::ieee754::isfinite(xmin)) {
        fxmin = std::get<0>(f(xmin));
        if (!tfel::math::ieee754::isfinite(fxmin)) {
          fxmin = std::numeric_limits<NumericType>::quiet_NaN();
          xmin = std::numeric_limits<NumericType>::quiet_NaN();
        }
      }
      if (tfel::math::ieee754::isfinite(xmax)) {
        fxmax = std::get<0>(f(xmax));
        if (!tfel::math::ieee754::isfinite(fxmax)) {
          fxmax = std::numeric_limits<NumericType>::quiet_NaN();
          xmax = std::numeric_limits<NumericType>::quiet_NaN();
        }
      }
      // check that fxmin and fmax have opposite signs
      if ((tfel::math::ieee754::isfinite(fxmin)) &&
          (tfel::math::ieee754::isfinite(fxmax)) &&
          (!has_opposite_signs(fxmin, fxmax))) {
        if (std::abs(fxmin) < std::abs(fxmax)) {
          fxmax = std::numeric_limits<NumericType>::quiet_NaN();
          xmax = std::numeric_limits<NumericType>::quiet_NaN();
        } else {
          fxmin = std::numeric_limits<NumericType>::quiet_NaN();
          xmin = std::numeric_limits<NumericType>::quiet_NaN();
        }
      }
      auto converged = false;
      while ((!converged) && (i != p.im)) {
        auto bounds_changed = false;
        const auto has_xmin = tfel::math::ieee754::isfinite(xmin);
        const auto has_xmax = tfel::math::ieee754::isfinite(xmax);
        if ((!tfel::math::ieee754::isfinite(fv)) ||
            (!tfel::math::ieee754::isfinite(dfv)) ||
            (tfel::math::ieee754::fpclassify(dfv) == FP_ZERO)) {
          if (has_xmin && has_xmax) {
            x = (xmin + xmax) / 2;
          } else if (has_xmin) {
            x = (xmin + x) / 2;
          } else if (has_xmax) {
            x = (xmax + x) / 2;
          } else {
            return std::make_tuple(false, x, i);
          }
        } else {
          if (has_xmin) {
            if ((!has_opposite_signs(fv, fxmin)) && (x > xmin)) {
              xmin = x;
              fxmin = fv;
              bounds_changed = true;
            }
          } else {
            xmin = x;
            fxmin = fv;
            bounds_changed = true;
          }
          if (has_xmax) {
            if ((!has_opposite_signs(fv, fxmax)) && (x < xmax)) {
              xmax = x;
              fxmax = fv;
              bounds_changed = true;
            }
          } else if (!bounds_changed) {
            if ((has_xmin) && (has_opposite_signs(fv, fxmin))) {
              xmax = x;
              fxmax = fv;
              if (xmin > xmax) {
                std::swap(xmax, xmin);
                std::swap(fxmax, fxmin);
              }
              bounds_changed = true;
            }
          }
        }
        dx = -fv / dfv;
        converged = c(fv, dx, x, i);
        if (!converged) {
          x += dx;
          if (!bounds_changed) {
            if (has_xmin && has_xmax && ((x < xmin) || (x > xmax))) {
              x = (xmin + xmax) / 2;
            }
          }
          std::tie(fv, dfv) = f(x);
          ++i;
        }
      }
      return std::make_tuple(converged, x, i);
    }  // end of scalarNewtonRaphson

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_SCALARNEWTONRAPHSON_IXX */
