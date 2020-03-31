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

#include"TFEL/Math/General/IEEE754.hxx"

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
      auto i = IndexType{};
      auto x = x0;
      auto dx = NumericType{};
      if (i >= im) {
        return {false, x0, i};
      }
#if __cplusplus >= 201703L
      auto[f, df] = f(x0);
#else
      auto fdf = f(x0);
      auto fv = std::get<0>(fdf);
      auto dfv = std::get<1>(fdf);
#endif
      auto converged = false;
      while ((!converged) && (i != im)) {
        if ((!tfel::math::ieee754::isfinite(fv)) ||
            (!tfel::math::ieee754::isfinite(dfv)) ||
            (tfel::math::ieee754::fpclassify(dfv) == FP_ZERO)) {
          return {false, x, i};
        }
        dx = -fv / dfv;
        converged = c(fv, dx, x, i);
        if (!converged) {
          x += dx;
          std::tie(fv, dfv) = f(x);
          ++i;
        }
      }
      return {converged, x, i};
    }  // end of scalarNewtonRaphson

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_SCALARNEWTONRAPHSON_IXX */
