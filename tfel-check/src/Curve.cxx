/*!
 * \file   tfel-check/src/Curve.cxx
 * \brief  functions to build and evaluate the curves read in files
 * \date   20/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <numeric>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Check/Interpolation.hxx"
#include "TFEL/Check/Curve.hxx"

namespace tfel::check {

  Curve buildCurve(const std::string& f,
                   const std::vector<double>& x,
                   const std::vector<double>& y) {
    raise_if(x.size() != y.size(),
             "buildCurve: the abscissa and value columns of file '" + f +
                 "' do not have the same size");
    raise_if(x.empty(), "buildCurve: no values in file '" + f + "'");
    raise_if(std::any_of(x.begin(), x.end(),
                         [](const double v) { return std::isnan(v); }),
             "buildCurve: invalid abscissa (NaN) in file '" + f + "'");
    auto indexes = std::vector<std::vector<double>::size_type>(x.size());
    std::iota(indexes.begin(), indexes.end(), 0);
    std::stable_sort(indexes.begin(), indexes.end(),
                     [&x](const auto i, const auto j) { return x[i] < x[j]; });
    auto c = Curve{};
    for (const auto i : indexes) {
      // the abscissas are sorted, so this test is an exact equality
      if ((!c.abscissas.empty()) && (!(c.abscissas.back() < x[i]))) {
        // a NaN in a duplicated point is kept, so that the comparison
        // fails
        if (std::isnan(y[i])) {
          c.values.back() = y[i];
        }
        continue;
      }
      c.abscissas.push_back(x[i]);
      c.values.push_back(y[i]);
    }
    return c;
  }  // end of buildCurve

  bool covers(const Curve& c, const std::vector<double>& x) {
    if (c.abscissas.empty()) {
      return x.empty();
    }
    const auto xmin = c.abscissas.front();
    const auto xmax = c.abscissas.back();
    const auto eps =
        1.e-10 * std::max({xmax - xmin, std::abs(xmin), std::abs(xmax)});
    return std::all_of(x.begin(), x.end(), [xmin, xmax, eps](const double v) {
      return (v >= xmin - eps) && (v <= xmax + eps);
    });
  }  // end of covers

  std::vector<double> evaluateCurve(Interpolation& i,
                                    const Curve& c,
                                    const std::vector<double>& x) {
    raise_if(c.abscissas.empty(), "evaluateCurve: empty curve");
    auto r = std::vector<double>{};
    r.reserve(x.size());
    auto interpolated = false;
    const auto b = c.abscissas.begin();
    const auto e = c.abscissas.end();
    for (const auto xv : x) {
      const auto p = std::lower_bound(b, e, xv);
      if (p == e) {
        r.push_back(c.values.back());
      } else if (!(xv < *p)) {
        // exact match, since *p is not lower than xv
        r.push_back(c.values[p - b]);
      } else if (p == b) {
        r.push_back(c.values.front());
      } else {
        if (!interpolated) {
          i.interpolate(c.abscissas, c.values);
          interpolated = true;
        }
        r.push_back(i.getValue(xv));
      }
    }
    return r;
  }  // end of evaluateCurve

}  // end of namespace tfel::check
