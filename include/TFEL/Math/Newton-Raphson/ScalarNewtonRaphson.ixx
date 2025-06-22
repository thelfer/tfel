/*!
 * \file   include/TFEL/Math/Newton-Raphson/ScalarNewtonRaphson.ixx
 * \brief
 * \author Thomas Helfer
 * \date   31/03/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_SCALARNEWTONRAPHSON_IXX
#define LIB_TFEL_MATH_SCALARNEWTONRAPHSON_IXX

#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/RootFinding/BissectionAlgorithmBase.hxx"

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
      auto i = IndexType{};
      auto x = p.x0;
      auto b = BissectionAlgorithmBase<NumericType>{};
      auto dx = NumericType{};
      if (i >= p.im) {
        return std::make_tuple(false, p.x0, i);
      }
#if __cplusplus >= 201703L
      auto [fv, dfv] = f(p.x0);
#else
      auto fdf = f(p.x0);
      auto fv = std::get<0>(fdf);
      auto dfv = std::get<1>(fdf);
#endif
      if (tfel::math::ieee754::isfinite(p.xmin0)) {
        b.updateBounds(p.xmin0, std::get<0>(f(p.xmin0)));
      }
      if (tfel::math::ieee754::isfinite(p.xmax0)) {
        b.updateBounds(p.xmax0, std::get<0>(f(p.xmax0)));
      }
      auto converged = false;
      while ((!converged) && (i != p.im)) {
        b.updateBounds(x, fv);
        auto have_valid_increment_estimate = false;
        if ((tfel::math::ieee754::isfinite(fv)) ||
            (tfel::math::ieee754::fpclassify(dfv) == FP_ZERO)) {
          dx = -fv / dfv;
          have_valid_increment_estimate = true;
        } else {
          // here we can't do a Newton step,
          // hence we see the bissection can provide a new estimate
          // of the root.
          auto xold = x;
          if (b.getNextRootEstimate(x)) {
            dx = x - xold;
            x = xold;
            have_valid_increment_estimate = true;
          } else {
            // the bissection method can't provide a new estimate, so the only
            // thing that we can do here is to divide the previous Newton step
            // by 2
            if (i == 0) {
              return std::make_tuple(false, p.x0, i);
            } else {
              // step back
              x -= dx / 2;
              b.iterate(x);
              std::tie(fv, dfv) = f(x);
            }
          }
        }
        converged = have_valid_increment_estimate &&
                    tfel::math::ieee754::isfinite(x) &&
                    tfel::math::ieee754::isfinite(fv) && c(fv, dx, x, i);
        if (!converged) {
          x += dx;
          b.iterate(x);
          std::tie(fv, dfv) = f(x);
          ++i;
        }
      }
      return std::make_tuple(converged, x, i);
    }  // end of scalarNewtonRaphson

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_SCALARNEWTONRAPHSON_IXX */
