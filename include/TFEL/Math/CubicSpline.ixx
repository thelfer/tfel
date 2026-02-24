/*!
 * \file   include/TFEL/Math/CubicSpline.ixx
 * \author Castelier Etienne
 * \date   07/08/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CUBICSPLINE_IXX
#define LIB_TFEL_MATH_CUBICSPLINE_IXX 1

#include <tuple>
#include <limits>
#include <iterator>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/General/Abs.hxx"

namespace tfel::math {

  namespace internals {

    template <typename ForwardIterator, typename Tp, typename Compare>
    constexpr ForwardIterator lower_bound(ForwardIterator first,
                                          ForwardIterator last,
                                          const Tp& val,
                                          Compare comp) {
      using DistanceType =
          typename std::iterator_traits<ForwardIterator>::difference_type;
      DistanceType len = std::distance(first, last);
      while (len > 0) {
        DistanceType half = len >> 1;
        ForwardIterator middle = first;
        std::advance(middle, half);
        if (comp(*middle, val)) {
          first = middle;
          ++first;
          len = len - half - 1;
        } else
          len = half;
      }
      return first;
    }  // end of lower_bound

    //! \brief internal structure to compare collocation points
    struct CubicSplineCollocationPointComparator {
      /*!
       * \return true if p->x < x
       * \param p : first point
       * \param x : abscissa
       */
      template <typename AbscissaType, typename ValueType>
      constexpr bool operator()(
          const CubicSplineCollocationPoint<AbscissaType, ValueType>& p,
          const AbscissaType& x) const noexcept {
        return p.x < x;
      }
    };  // end of struct PointComparator

    template <typename AbscissaType, typename ValueType>
    constexpr auto computeCubicSplineLocalCoefficients(
        const CubicSplineCollocationPoint<AbscissaType, ValueType>& pa,
        const CubicSplineCollocationPoint<AbscissaType, ValueType>& pb) {
      using ResultType1 =
          derivative_type<ValueType, AbscissaType, AbscissaType>;
      using ResultType2 =
          derivative_type<ValueType, AbscissaType, AbscissaType, AbscissaType>;
      const auto usL = 1 / (pb.x - pa.x);
      const auto Dy = (pb.y - pa.y) * usL;
      return std::make_pair(ResultType1{(3 * Dy - pb.d - 2 * pa.d) * usL},
                            ResultType2{(-2 * Dy + pb.d + pa.d) * usL * usL});
    }  // end of computeCubicSplineLocalCoefficients

    template <typename AbscissaType, typename ValueType>
    constexpr result_type<ValueType, AbscissaType, OpMult>
    computeCubicSplineLocalIntegral(
        const AbscissaType xa,
        const AbscissaType xb,
        const CubicSplineCollocationPoint<AbscissaType, ValueType>& pa,
        const CubicSplineCollocationPoint<AbscissaType, ValueType>& pb) {
      const auto py = pa.y;
      const auto d = pa.d;
      const auto [a2, a3] = computeCubicSplineLocalCoefficients(pa, pb);
      const auto x0 = xa - pa.x;
      const auto x1 = xb - pa.x;
      return (3 * a3 * (x1 * x1 * x1 * x1 - x0 * x0 * x0 * x0) +
              4 * a2 * (x1 * x1 * x1 - x0 * x0 * x0) +
              6 * d * (x1 * x1 - x0 * x0) + 12 * py * (x1 - x0)) /
             12;
    }  // end of computeCubicSplineLocalIntegral

  }  // end of namespace internals

  template <typename AbscissaType, typename ValueType>
  constexpr bool
  CubicSpline<AbscissaType, ValueType>::PointComparator::operator()(
      const Point& p, const AbscissaType& x) const noexcept {
    return p.x < x;
  }  // end of operator()

  template <typename AbscissaType, typename ValueType>
  template <typename AIterator, typename OIterator>
  void CubicSpline<AbscissaType, ValueType>::setCollocationPoints(
      AIterator px, AIterator pxe, OIterator py) {
    if (px == pxe) {
      tfel::raise<CubicSplineInvalidAbscissaVectorSize>();
    }
    this->points.clear();
    Point p;
    p.x = *px;
    p.y = *py;
    this->points.push_back(p);
    auto px2 = px;
    ++px;
    ++py;
    while (px != pxe) {
      px2 = px - 1u;
      if (*px2 >= *px) {
        tfel::raise<CubicSplineUnorderedAbscissaVector>();
      }
      p.x = *px;
      p.y = *py;
      this->points.push_back(p);
      px2 = px;
      ++px;
      ++py;
    }
    this->buildInterpolation();
  }  // end of setCollocationPoints

  template <typename AbscissaType, typename ValueType>
  template <typename AContainer, typename OContainer>
  void CubicSpline<AbscissaType, ValueType>::setCollocationPoints(
      const AContainer& x, const OContainer& y) {
    if (x.size() < 1) {
      tfel::raise<CubicSplineInvalidAbscissaVectorSize>();
    }
    if (y.size() < 1) {
      tfel::raise<CubicSplineInvalidOrdinateVectorSize>();
    }
    if (x.size() != y.size()) {
      tfel::raise<CubicSplineInvalidInputs>();
    }
    this->setCollocationPoints(x.begin(), x.end(), y.begin());
  }  // end of setCollocationPoints

  template <typename AbscissaType, typename ValueType>
  void CubicSpline<AbscissaType, ValueType>::buildInterpolation() {
    if (this->points.empty()) {
      return;
    }
    if (this->points.size() == 1) {
      this->points[0].d = derivative_type<ValueType, AbscissaType>{0};
      return;
    }
    const auto s = this->points.size() - 1u;
    std::vector<AbscissaType> main_diagonal(s + 1u);
    std::vector<AbscissaType> upper_diagonal(s);
    auto* const md = main_diagonal.data();
    auto* const mu = upper_diagonal.data();
    auto ho = AbscissaType(0);
    auto uo = ValueType{0};
    for (decltype(this->points.size()) i = 0; i != s; ++i) {
      const auto hn = 1 / (this->points[i + 1].x - this->points[i].x);
      const auto un = 3 * hn * hn * (this->points[i + 1].y - this->points[i].y);
      mu[i] = hn;
      md[i] = 2 * (hn + ho);
      this->points[i].d = un + uo;
      uo = un;
      ho = hn;
    }
    md[s] = 2 * ho;
    this->points[s].d = uo;
    this->solveTridiagonalLinearSystem(mu, md);
  }  // end of buildInterpolation

  template <typename AbscissaType, typename ValueType>
  void CubicSpline<AbscissaType, ValueType>::solveTridiagonalLinearSystem(
      const AbscissaType* const c, AbscissaType* const b) {
    const auto prec = 100 * std::numeric_limits<AbscissaType>::min();
    const auto n = this->points.size();
    decltype(this->points.size()) i;
    for (i = 1; i < n; i++) {
      if (std::abs(b[i - 1]) < prec) {
        tfel::raise<CubicSplineNullPivot>();
      }
      const auto m = c[i - 1] / b[i - 1];
      b[i] -= m * c[i - 1];
      this->points[i].d -= m * this->points[i - 1].d;
    }
    if (std::abs(b[n - 1]) < prec) {
      tfel::raise<CubicSplineNullPivot>();
    }
    this->points[n - 1].d /= b[n - 1];
    i = n;
    i -= 2u;
    for (; i != 0; i--) {
      if (std::abs(b[i]) < prec) {
        tfel::raise<CubicSplineNullPivot>();
      }
      this->points[i].d =
          (this->points[i].d - c[i] * (this->points[i + 1].d)) / b[i];
    }
    if (std::abs(b[0]) < prec) {
      tfel::raise<CubicSplineNullPivot>();
    }
    this->points[0].d = (this->points[0].d - c[0] * (this->points[1].d)) / b[0];
  }  // end of solveTridiagonalLinearSystem

  template <typename AbscissaType, typename ValueType>
  ValueType CubicSpline<AbscissaType, ValueType>::computeMeanValue(
      const AbscissaType xa, const AbscissaType xb) const {
    return this->computeIntegral(xa, xb) / (xb - xa);
  }  // end of computeMeanValue

  template <typename AbscissaType, typename ValueType>
  result_type<ValueType, AbscissaType, OpMult>
  CubicSpline<AbscissaType, ValueType>::computeIntegral(
      const AbscissaType xa, const AbscissaType xb) const {
    if (this->points.empty()) {
      tfel::raise<CubicSplineUninitialised>();
    }
    if (this->points.size() == 1) {
      const auto& f = points.back().y;
      return f * (xb - xa);
    }
    if (xb < xa) {
      return -this->computeIntegral(xb, xa);
    }
    const auto pa = internals::lower_bound(
        this->points.begin(), this->points.end(), xa, PointComparator());
    const auto pb = internals::lower_bound(
        this->points.begin(), this->points.end(), xb, PointComparator());
    if (pa == pb) {
      if (pb == this->points.begin()) {
        const auto xe = this->points.front().x;
        const auto& ye = this->points.front().y;
        const auto& df = this->points.front().d;
        // l'équation de l'extrapolation est :
        // y = ye-df*xe + df*x
        // l'intégrale est alors:
        // (ye-df*xe)*(xb-xa)+0.5*df*(xb-xa)*(xb-xa)
        return ye * (xb - xa) +
               0.5 * df * ((xb - xe) * (xb - xe) - (xa - xe) * (xa - xe));
      } else if (pb == this->points.end()) {
        const AbscissaType xe = this->points.back().x;
        const auto& ye = this->points.back().y;
        const auto& df = this->points.back().d;
        // l'équation de l'extrapoltion est :
        // y = ye-df*xe + df*x
        // l'intégrale est alors:
        // (ye-df*xe)*(xb-xa)+0.5*df*(xb-xa)*(xb-xa)
        return ye * (xb - xa) +
               0.5 * df * ((xb - xe) * (xb - xe) - (xa - xe) * (xa - xe));
      } else {
        const auto ppb = std::prev(pb);
        // spline=pa->y+(x-pa->x)*(pa->d+(x-pa->x)*(a2+(x-pa->x)*a3));
        //       =pa->y+(x-pa->x)*(pa->d+(x-pa->x)*(a2+(x-pa->x)*a3));
        return tfel::math::internals::computeCubicSplineLocalIntegral(
            xa, xb, *ppb, *pb);
      }
    }
    ValueType s(AbscissaType(0));
    if (pa == this->points.begin()) {
      const auto xe = this->points.front().x;
      const auto& ye = this->points.front().y;
      const auto& df = this->points.front().d;
      // l'équation de l'extrapoltion est :
      // y = ye-df*xe + df*x
      // l'intégrale est alors:
      // (ye-df*xe)*(pa->a-xa)+0.5*df*(pa->a-xa)*(pa->a-xa)
      s += ye * (xe - xa) - 0.5 * df * ((xa - xe) * (xa - xe));
    } else {
      s += tfel::math::internals::computeCubicSplineLocalIntegral(
          xa, pa->x, *(std::prev(pa)), *pa);
    }
    if (pb == this->points.end()) {
      const AbscissaType xe = this->points.back().x;
      const auto& ye = this->points.back().y;
      const auto& df = this->points.back().d;
      // l'équation de l'extrapoltion est :
      // y = ye-df*xe + df*x
      // l'intégrale est alors:
      // (ye-df*xe)*(xb-xa)+0.5*df*(xb-xa)*(xb-xa)
      s += ye * (xb - xe) + 0.5 * df * ((xb - xe) * (xb - xe));
      //	s += (ye-df*xe)*(xb-pb->x)+0.5*df*(xb-pb->x)*(xb-pb->x);
    } else {
      s += tfel::math::internals::computeCubicSplineLocalIntegral(
          (pb - 1)->x, xb, *(std::prev(pb)), *pb);
    }
    auto p = pa;
    const auto pe = std::prev(pb);
    while (p != pe) {
      s += tfel::math::internals::computeCubicSplineLocalIntegral(
          p->x, (p + 1)->x, *p, *(std::next(p)));
      ++p;
    }
    return s;
  }  // end of CubicSpline<AbscissaType,ValueType>::computeIntegral

  template <typename AbscissaType, typename ValueType>
  void CubicSpline<AbscissaType, ValueType>::getValues(
      ValueType& f,
      derivative_type<ValueType, AbscissaType>& df,
      const AbscissaType x) const {
    if (this->points.empty()) {
      tfel::raise<CubicSplineUninitialised>();
    }
    std::tie(f, df) =
        computeCubicSplineInterpolationAndDerivative<true>(this->points, x);
  }  // end of getValues

  template <typename AbscissaType, typename ValueType>
  ValueType CubicSpline<AbscissaType, ValueType>::operator()(
      const AbscissaType x) const {
    return this->getValue(x);
  }  // end of operator()

  template <typename AbscissaType, typename ValueType>
  ValueType CubicSpline<AbscissaType, ValueType>::getValue(
      const AbscissaType x) const {
    if (this->points.empty()) {
      tfel::raise<CubicSplineUninitialised>();
    }
    return computeCubicSplineInterpolation<true>(this->points, x);
  }  // end of getValue

  template <typename AbscissaType, typename ValueType>
  void CubicSpline<AbscissaType, ValueType>::getValues(
      ValueType& f,
      derivative_type<ValueType, AbscissaType>& df,
      derivative_type<ValueType, AbscissaType, AbscissaType>& d2f,
      const AbscissaType x) const {
    if (this->points.empty()) {
      tfel::raise<CubicSplineUninitialised>();
    }
    // Cas d'un seul couple
    if (this->points.size() == 1) {
      f = this->points[0].y;
      df = derivative_type<ValueType, AbscissaType>{0};
      d2f = derivative_type<ValueType, AbscissaType, AbscissaType>{0};
      return;
    }
    auto in = internals::lower_bound(this->points.begin(), this->points.end(),
                                     x, PointComparator());
    // Extrapolation
    if (in == this->points.begin()) {
      df = in->d;
      f = in->y + (x - in->x) * df;
      d2f = derivative_type<ValueType, AbscissaType, AbscissaType>{0};
      return;
    }
    const auto ip = std::prev(in);
    if (in == this->points.end()) {
      df = ip->d;
      f = ip->y + (x - ip->x) * df;
      d2f = derivative_type<ValueType, AbscissaType, AbscissaType>{0};
      return;
    }
    const auto [a2, a3] =
        tfel::math::internals::computeCubicSplineLocalCoefficients(*ip, *in);
    const auto x2 = x - ip->x;
    f = ip->y + x2 * (ip->d + x2 * (a2 + x2 * a3));
    df = ip->d + x2 * (2 * a2 + x2 * 3 * a3);
    d2f = 2 * a2 + x2 * 6 * a3;
  }

  //! \brief return the collocation points
  template <typename AbscissaType, typename ValueType>
  const auto& CubicSpline<AbscissaType, ValueType>::getCollocationPoints()
      const {
    return this->points;
  }

  template <bool extrapolate,
            typename CollocationPointContainer,
            typename AbscissaType>
  constexpr auto computeCubicSplineInterpolation(
      const CollocationPointContainer& points, const AbscissaType x) {
    using collocation_point_type =
        typename CollocationPointContainer::value_type;
    using abscissa_type = typename collocation_point_type::abscissa_type;
    using value_type = typename collocation_point_type::value_type;
    const auto xv = static_cast<abscissa_type>(x);
    if (points.empty()) {
      tfel::reportContractViolation(
          "computeCubicSplineInterpolation: empty collocation points");
    }
    if (points.size() == 1) {
      return points[0].y;
    }
    const auto in = internals::lower_bound(
        points.begin(), points.end(), xv,
        tfel::math::internals::CubicSplineCollocationPointComparator());
    // extrapolation
    if (in == points.begin()) {
      if constexpr (extrapolate) {
        return value_type{in->y + (xv - in->x) * in->d};
      } else {
        return in->y;
      }
    }
    const auto ip = std::prev(in);
    if (in == points.end()) {
      if constexpr (extrapolate) {
        return value_type{ip->y + (xv - ip->x) * ip->d};
      } else {
        return ip->y;
      }
    }
    // interpolation
    const auto [a2, a3] =
        tfel::math::internals::computeCubicSplineLocalCoefficients(*ip, *in);
    const auto x2 = xv - ip->x;
    return value_type{ip->y + x2 * (ip->d + x2 * (a2 + x2 * a3))};
  }  // end of computeCubicSplineInterpolation

  template <bool extrapolate,
            typename CollocationPointContainer,
            typename AbscissaType>
  constexpr auto computeCubicSplineInterpolationAndDerivative(
      const CollocationPointContainer& points, const AbscissaType x) {
    using collocation_point_type =
        typename CollocationPointContainer::value_type;
    using abscissa_type = typename collocation_point_type::abscissa_type;
    using value_type = typename collocation_point_type::value_type;
    const auto xv = static_cast<abscissa_type>(x);
    if (points.empty()) {
      tfel::reportContractViolation(
          "computeCubicSplineInterpolation: empty collocation points");
    }
    if (points.size() == 1) {
      return std::make_pair(points[0].y,
                            derivative_type<value_type, abscissa_type>{});
    }
    const auto in = internals::lower_bound(
        points.begin(), points.end(), xv,
        tfel::math::internals::CubicSplineCollocationPointComparator());
    // extrapolation
    if (in == points.begin()) {
      if constexpr (extrapolate) {
        const auto df = in->d;
        return std::make_pair(value_type{in->y + (xv - in->x) * df}, df);
      } else {
        return std::make_pair(in->y,
                              derivative_type<value_type, abscissa_type>{});
      }
    }
    const auto ip = std::prev(in);
    if (in == points.end()) {
      if constexpr (extrapolate) {
        const auto df = ip->d;
        return std::make_pair(value_type{ip->y + (xv - ip->x) * df}, df);
      } else {
        return std::make_pair(ip->y,
                              derivative_type<value_type, abscissa_type>{});
      }
    }
    const auto [a2, a3] =
        tfel::math::internals::computeCubicSplineLocalCoefficients(*ip, *in);
    const auto x2 = xv - ip->x;
    const auto f = value_type{ip->y + x2 * (ip->d + x2 * (a2 + x2 * a3))};
    const auto df = derivative_type<value_type, abscissa_type>{
        ip->d + x2 * (2 * a2 + x2 * 3 * a3)};
    return std::make_pair(f, df);
  }  // end of computeCubicSplineInterpolationAndDerivative

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_CUBICSPLINE_IXX */
