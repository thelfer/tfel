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

#include <limits>
#include <algorithm>

#include "TFEL/Math/General/Abs.hxx"

namespace tfel {

  namespace math {

    template <typename real, typename value>
    bool CubicSpline<real, value>::PointComparator::operator()(
        const typename CubicSpline<real, value>::Point& p,
        const real& x) const {
      return p.x < x;
    }  // end of CubicSpline<real,value>::Point

    template <typename real, typename value>
    template <typename AIterator, typename OIterator>
    void CubicSpline<real, value>::setCollocationPoints(AIterator px,
                                                        AIterator pxe,
                                                        OIterator py) {
      using namespace std;
      AIterator px2;
      if (px == pxe) {
        throw(CubicSplineInvalidAbscissaVectorSize());
      }
      this->values.clear();
      Point p;
      p.x = *px;
      p.y = *py;
      this->values.push_back(p);
      px2 = px;
      ++px;
      ++py;
      while (px != pxe) {
        px2 = px - 1u;
        if (*px2 >= *px) {
          throw(CubicSplineUnorderedAbscissaVector());
        }
        p.x = *px;
        p.y = *py;
        this->values.push_back(p);
        px2 = px;
        ++px;
        ++py;
      }
      this->buildInterpolation();
    }  // CubicSpline<real,value>::CubicSpline

    template <typename real, typename value>
    template <typename AContainer, typename OContainer>
    void CubicSpline<real, value>::setCollocationPoints(const AContainer& x,
                                                        const OContainer& y) {
      if (x.size() < 1) {
        throw(CubicSplineInvalidAbscissaVectorSize());
      }
      if (y.size() < 1) {
        throw(CubicSplineInvalidOrdinateVectorSize());
      }
      if (x.size() != y.size()) {
        throw(CubicSplineInvalidInputs());
      }
      this->setCollocationPoints(x.begin(), x.end(), y.begin());
    }  // CubicSpline<real,value>::CubicSpline

    template <typename real, typename value>
    void CubicSpline<real, value>::buildInterpolation() {
      using namespace std;
      using std::vector;
      if (this->values.size() == 1) {
        this->values[0].d = value(real(0));
      } else {
        typename vector<real>::size_type i;
        typename vector<real>::size_type s = this->values.size() - 1u;
        vector<real> m(2 * s + 1u);    // matrix (main an upper diagonal)
        real* const md = &m[0];        // main  diagonal
        real* const mu = md + s + 1u;  // upper diagonal
        real ho = real(0);
        value uo = value(real(0));
        for (i = 0; i != s; ++i) {
          const real hn = 1. / (this->values[i + 1].x - this->values[i].x);
          const value un =
              3. * hn * hn * (this->values[i + 1].y - this->values[i].y);
          mu[i] = hn;
          md[i] = 2. * (hn + ho);
          this->values[i].d = un + uo;
          uo = un;
          ho = hn;
        }
        md[s] = 2. * ho;
        this->values[s].d = uo;
        solveTridiagonalLinearSystem(mu, md);
      }
    }

    template <typename real, typename value>
    void CubicSpline<real, value>::solveTridiagonalLinearSystem(
        const real* const c, real* const b) {
      using namespace std;
      using std::vector;
      typename vector<typename CubicSpline<real, value>::Point>::size_type i;
      typename vector<typename CubicSpline<real, value>::Point>::size_type n;
      const real prec = 100 * numeric_limits<real>::min();
      n = this->values.size();
      for (i = 1; i < n; i++) {
        if (abs(b[i - 1]) < prec) {
          throw(CubicSplineNullPivot());
        }
        real m = c[i - 1] / b[i - 1];
        b[i] -= m * c[i - 1];
        this->values[i].d -= m * this->values[i - 1].d;
      }
      if (abs(b[n - 1]) < prec) {
        throw(CubicSplineNullPivot());
      }
      this->values[n - 1].d /= b[n - 1];
      i = n;
      i -= 2u;
      for (; i != 0; i--) {
        if (abs(b[i]) < prec) {
          throw(CubicSplineNullPivot());
        }
        this->values[i].d =
            (this->values[i].d - c[i] * (this->values[i + 1].d)) / b[i];
      }
      if (abs(b[0]) < prec) {
        throw(CubicSplineNullPivot());
      }
      this->values[0].d =
          (this->values[0].d - c[0] * (this->values[1].d)) / b[0];
    }

    template <typename real, typename value>
    void CubicSpline<real, value>::computeLocalCoefficients(
        value& a2,
        value& a3,
        const typename std::vector<
            typename CubicSpline<real, value>::Point>::const_iterator pa) {
      using namespace std;
      const typename vector<Point>::const_iterator pb = pa + 1;
      const real usL = 1 / (pb->x - pa->x);
      const value Dy = (pb->y - pa->y) * usL;
      a2 = (3 * Dy - pb->d - 2 * pa->d) * usL;
      a3 = (-2 * Dy + pb->d + pa->d) * usL * usL;
    }  // end of CubicSpline<real,value>::computeLocalCoefficient

    template <typename real, typename value>
    value CubicSpline<real, value>::computeLocalIntegral(
        const real xa,
        const real xb,
        const typename std::vector<
            typename CubicSpline<real, value>::Point>::const_iterator pa) {
      using namespace std;
      const value py = pa->y;
      const value d = pa->d;
      value a2;
      value a3;
      CubicSpline<real, value>::computeLocalCoefficients(a2, a3, pa);
      const real x0 = xa - pa->x;
      const real x1 = xb - pa->x;
      return (3 * a3 * (x1 * x1 * x1 * x1 - x0 * x0 * x0 * x0) +
              4 * a2 * (x1 * x1 * x1 - x0 * x0 * x0) +
              6 * d * (x1 * x1 - x0 * x0) + 12 * py * (x1 - x0)) /
             12;
    }  // end of

    template <typename real, typename value>
    value CubicSpline<real, value>::computeMeanValue(const real xa,
                                                     const real xb) const {
      return this->computeIntegral(xa, xb) / (xb - xa);
    }  // end of value CubicSpline<real,value>::computeMeanValue

    template <typename real, typename value>
    value CubicSpline<real, value>::computeIntegral(const real xa,
                                                    const real xb) const {
      using namespace std;
      typename vector<Point>::const_iterator pa;
      typename vector<Point>::const_iterator pb;
      if (this->values.empty()) {
        throw(CubicSplineUninitialised());
      }
      if (this->values.size() == 1) {
        const auto& f = values.back().y;
        return f * (xb - xa);
      }
      if (xb < xa) {
        return -this->computeIntegral(xb, xa);
      }
      pa = lower_bound(this->values.begin(), this->values.end(), xa,
                       PointComparator());
      pb = lower_bound(this->values.begin(), this->values.end(), xb,
                       PointComparator());
      if (pa == pb) {
        if (pb == this->values.begin()) {
          const real xe = this->values.front().x;
          const auto& ye = this->values.front().y;
          const auto& df = this->values.front().d;
          // l'équation de l'extrapoltion est :
          // y = ye-df*xe + df*x
          // l'intégrale est alors:
          // (ye-df*xe)*(xb-xa)+0.5*df*(xb-xa)*(xb-xa)
          return ye * (xb - xa) +
                 0.5 * df * ((xb - xe) * (xb - xe) - (xa - xe) * (xa - xe));
        } else if (pb == this->values.end()) {
          const real xe = this->values.back().x;
          const auto& ye = this->values.back().y;
          const auto& df = this->values.back().d;
          // l'équation de l'extrapoltion est :
          // y = ye-df*xe + df*x
          // l'intégrale est alors:
          // (ye-df*xe)*(xb-xa)+0.5*df*(xb-xa)*(xb-xa)
          return ye * (xb - xa) +
                 0.5 * df * ((xb - xe) * (xb - xe) - (xa - xe) * (xa - xe));
        } else {
          pa = pb - 1;
          // spline=pa->y+(x-pa->x)*(pa->d+(x-pa->x)*(a2+(x-pa->x)*a3));
          //       =pa->y+(x-pa->x)*(pa->d+(x-pa->x)*(a2+(x-pa->x)*a3));
          return CubicSpline<real, value>::computeLocalIntegral(xa, xb, pa);
        }
      }
      value s(real(0));
      if (pa == this->values.begin()) {
        const real xe = this->values.front().x;
        const auto& ye = this->values.front().y;
        const auto& df = this->values.front().d;
        // l'équation de l'extrapoltion est :
        // y = ye-df*xe + df*x
        // l'intégrale est alors:
        // (ye-df*xe)*(pa->a-xa)+0.5*df*(pa->a-xa)*(pa->a-xa)
        s += ye * (xe - xa) - 0.5 * df * ((xa - xe) * (xa - xe));
      } else {
        s += CubicSpline<real, value>::computeLocalIntegral(xa, pa->x, pa - 1);
      }
      if (pb == this->values.end()) {
        const real xe = this->values.back().x;
        const auto& ye = this->values.back().y;
        const auto& df = this->values.back().d;
        // l'équation de l'extrapoltion est :
        // y = ye-df*xe + df*x
        // l'intégrale est alors:
        // (ye-df*xe)*(xb-xa)+0.5*df*(xb-xa)*(xb-xa)
        s += ye * (xb - xe) + 0.5 * df * ((xb - xe) * (xb - xe));
        //	s += (ye-df*xe)*(xb-pb->x)+0.5*df*(xb-pb->x)*(xb-pb->x);
      } else {
        s += CubicSpline<real, value>::computeLocalIntegral((pb - 1)->x, xb,
                                                            pb - 1);
      }
      --pb;
      while (pa != pb) {
        s += CubicSpline<real, value>::computeLocalIntegral(pa->x, (pa + 1)->x,
                                                            pa);
        ++pa;
      }
      return s;
    }  // end of CubicSpline<real,value>::computeIntegral

    // Valeur de la spline et de sa dérivée
    // Interpolation cubique
    // x : Abscisse où calculer la valeur de la spline
    // f : Valeur de la fonction
    // df : Valeur de la dérivée
    template <typename real, typename value>
    void CubicSpline<real, value>::getValues(value& f,
                                             value& df,
                                             real x) const {
      using namespace std;
      typename vector<Point>::const_iterator in;
      if (this->values.empty()) {
        throw(CubicSplineUninitialised());
      }
      // Cas d'un seul couple
      if (this->values.size() == 1) {
        f = this->values[0].y;
        df = value(real(0));
        return;
      }
      in = lower_bound(this->values.begin(), this->values.end(), x,
                       PointComparator());
      // Extrapolation
      if (in == this->values.begin()) {
        x -= in->x;
        df = in->d;
        f = in->y + x * df;
        return;
      }
      if (in == this->values.end()) {
        --in;
        x -= in->x;
        df = in->d;
        f = in->y + x * df;
        return;
      }
      typename vector<Point>::const_iterator ip = in - 1;
      value a2;
      value a3;
      CubicSpline<real, value>::computeLocalCoefficients(a2, a3, ip);
      x -= ip->x;
      f = ip->y + x * (ip->d + x * (a2 + x * a3));
      df = ip->d + x * (2 * a2 + x * 3 * a3);
    }

    template <typename real, typename value>
    value CubicSpline<real, value>::operator()(const real x) const {
      return this->getValue(x);
    }

    // Valeur de la spline
    // Interpolation cubique
    // x : Abscisse où calculer la valeur de la spline
    template <typename real, typename value>
    value CubicSpline<real, value>::getValue(real x) const {
      using namespace std;
      typename vector<Point>::const_iterator in;
      if (this->values.empty()) {
        throw(CubicSplineUninitialised());
      }
      // Cas d'un seul couple
      if (this->values.size() == 1) {
        return this->values[0].y;
      }
      in = lower_bound(this->values.begin(), this->values.end(), x,
                       PointComparator());
      // Extrapolation
      if (in == this->values.begin()) {
        x -= in->x;
        return in->y + x * in->d;
      }
      if (in == this->values.end()) {
        --in;
        x -= in->x;
        return in->y + x * in->d;
      }
      typename vector<Point>::const_iterator ip = in - 1;
      value a2;
      value a3;
      CubicSpline<real, value>::computeLocalCoefficients(a2, a3, ip);
      x -= ip->x;
      return ip->y + x * (ip->d + x * (a2 + x * a3));
    }

    // Valeur de la spline et de sa dérivée
    // Interpolation cubique
    // x : Abscisse où calculer la valeur de la spline
    // f : Valeur de la fonction
    // df : Valeur de la dérivée
    // d2f : Valeur de la dérivée seconde
    template <typename real, typename value>
    void CubicSpline<real, value>::getValues(value& f,
                                             value& df,
                                             value& d2f,
                                             real x) const {
      using namespace std;
      if (this->values.empty()) {
        throw(CubicSplineUninitialised());
      }
      typename vector<Point>::const_iterator in;
      in = this->values.begin();
      // Cas d'un seul couple
      if (this->values.size() == 1) {
        f = this->values[0].y;
        df = value(real(0));
        d2f = value(real(0));
        return;
      }
      in = lower_bound(this->values.begin(), this->values.end(), x,
                       PointComparator());
      // Extrapolation
      if (in == this->values.begin()) {
        x -= in->x;
        df = in->d;
        f = in->y + x * df;
        d2f = value(real(0));
        return;
      }
      if (in == this->values.end()) {
        --in;
        x -= in->x;
        df = in->d;
        f = in->y + x * df;
        d2f = value(real(0));
        return;
      }
      typename vector<Point>::const_iterator ip = in - 1;
      value a2;
      value a3;
      CubicSpline<real, value>::computeLocalCoefficients(a2, a3, ip);
      x -= ip->x;
      f = ip->y + x * (ip->d + x * (a2 + x * a3));
      df = ip->d + x * (2 * a2 + x * 3 * a3);
      d2f = 2 * a2 + x * 6 * a3;
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_CUBICSPLINE_IXX */
