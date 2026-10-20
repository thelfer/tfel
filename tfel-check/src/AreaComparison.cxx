/*
 * \file tfel-check/src/AreaComparison.cxx
 *
 * \date 12/06/2013
 * \author Remy Petkantchin
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <vector>
#include <numeric>
#include <iterator>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Check/AreaComparison.hxx"

namespace tfel::check {

  static double trapezoidalIntegration(const std::vector<double>& abscissas,
                                       const std::vector<double>& ordinates) {
    double result = 0;
    for (std::vector<double>::size_type i = 1; i < abscissas.size(); ++i) {
      double absInter = abscissas.at(i) - abscissas.at(i - 1);
      double ordInter = ordinates.at(i) + ordinates.at(i - 1);
      result += ((absInter * ordInter) / 2);
    }
    return result;
  }

  namespace {

    /*!
     * \brief a curve, i.e. a set of points sorted by increasing abscissa,
     * with no duplicated abscissa.
     */
    struct Curve {
      std::vector<double> abscissas;
      std::vector<double> values;
    };

  }  // end of namespace

  /*!
   * \return the curve defined by the given abscissas and values
   * \param[in] x: abscissas
   * \param[in] y: values
   *
   * The points are sorted by increasing abscissa. If an abscissa
   * appears more than once, only the first point is kept, as in the
   * `Linearization` class.
   */
  static Curve buildCurve(const std::vector<double>& x,
                          const std::vector<double>& y) {
    raise_if(x.size() != y.size(),
             "AreaComparison::compare: the abscissa and value "
             "columns do not have the same size");
    raise_if(x.empty(), "AreaComparison::compare: no values");
    auto indexes = std::vector<std::vector<double>::size_type>(x.size());
    std::iota(indexes.begin(), indexes.end(), 0);
    std::stable_sort(indexes.begin(), indexes.end(),
                     [&x](const auto i, const auto j) { return x[i] < x[j]; });
    auto c = Curve{};
    for (const auto i : indexes) {
      // the abscissas are sorted, so this test is an exact equality
      if ((!c.abscissas.empty()) && (!(c.abscissas.back() < x[i]))) {
        continue;
      }
      c.abscissas.push_back(x[i]);
      c.values.push_back(y[i]);
    }
    return c;
  }

  /*!
   * \return the values of a curve at the given abscissas
   * \param[in] i: interpolation
   * \param[in] c: curve
   * \param[in] x: abscissas, sorted by increasing values
   *
   * The values of the curve are used at its own abscissas. Between
   * them, the given interpolation is used. Outside the range of the
   * curve, the first or last value is used.
   */
  static std::vector<double> evaluateCurve(Interpolation& i,
                                           const Curve& c,
                                           const std::vector<double>& x) {
    i.interpolate(c.abscissas, c.values);
    auto r = std::vector<double>{};
    r.reserve(x.size());
    auto p = c.abscissas.begin();
    for (const auto xv : x) {
      p = std::lower_bound(p, c.abscissas.end(), xv);
      if (p == c.abscissas.end()) {
        r.push_back(c.values.back());
      } else if (!(xv < *p)) {
        // exact match, since *p is not lower than xv
        r.push_back(c.values[p - c.abscissas.begin()]);
      } else if (p == c.abscissas.begin()) {
        r.push_back(c.values.front());
      } else {
        r.push_back(i.getValue(xv));
      }
    }
    return r;
  }

  AreaComparison::AreaComparison() { this->name = "area"; }

  AreaComparison::~AreaComparison() = default;

  void AreaComparison::compare() {
    using size_type = std::vector<double>::size_type;
    raise_if(this->colIntegralInterpolated == nullptr,
             "AreaComparison::compare: no abscissa column defined");
    raise_if(this->integralInterpolation == nullptr,
             "AreaComparison::compare: no interpolation defined");
    // reading the abscissas of both files
    auto getAbscissas = [this](const std::shared_ptr<Column>& c) {
      auto x = Column(*(this->colIntegralInterpolated));
      x.setFilename(c->getFilename());
      return x.getValues();
    };
    const auto ca = buildCurve(getAbscissas(this->c1), this->c1->getValues());
    const auto cb = buildCurve(getAbscissas(this->c2), this->c2->getValues());
    raise_if((!this->integralInterpolation->isConform()) &&
                 (ca.abscissas != cb.abscissas),
             "AreaComparison::compare: the abscissas of files '" +
                 this->c1->getFilename() + "' and '" + this->c2->getFilename() +
                 "' differ, an interpolation is required");
    // union of the abscissas of both files
    auto times = std::vector<double>{};
    std::set_union(ca.abscissas.begin(), ca.abscissas.end(),
                   cb.abscissas.begin(), cb.abscissas.end(),
                   std::back_inserter(times));
    const auto valA = evaluateCurve(*(this->integralInterpolation), ca, times);
    const auto valB = evaluateCurve(*(this->integralInterpolation), cb, times);
    std::vector<double> valuesDifference;
    this->msgLog += '\n';
    bool s = true;
    for (size_type i = 0; i != valA.size(); ++i) {
      // make the difference between ref and res values
      valuesDifference.push_back(std::abs(valA[i] - valB[i]));
    }
    // getting the value of the integral
    double areaValue = trapezoidalIntegration(times, valuesDifference);
    double maxValueA = this->c1->getValues().at(0);

    for (size_type i = 0; i < this->c1->getValues().size(); ++i) {
      // fetching the max value of the ref column
      if (maxValueA < this->c1->getValues().at(i)) {
        maxValueA = this->c1->getValues().at(i);
      }
    }
    areaValue /= maxValueA;  // divide by the max value of ref, in order to
                             // normalize the area

    if (areaValue > this->prec) {
      s = false;
    }
    if (!s) {
      this->msgLog += "*** ";
    }
    this->msgLog += "comparison of files " + this->c1->getFilename() + " and " +
                    this->c2->getFilename() + " using column ";
    this->msgLog += "'" + this->c1->getName() + "'";
    this->msgLog +=
        ", with interpolation " + this->integralInterpolation->getType();
    if (this->integralInterpolation->isConform()) {
      this->msgLog += " using column ";
      this->msgLog += this->colIntegralInterpolated->getName();
    }
    if (!s) {
      this->msgLog += " failed (" + this->name + " check).\n    Area error : ";
      this->msgLog += tfel::utilities::convert(areaValue);
      this->msgLog +=
          " \n    Max area = " + tfel::utilities::convert(this->prec) + "\n";
      this->success = false;
    } else {
      this->msgLog +=
          " succeed (" + this->name +
          " check).\n    Area error : " + tfel::utilities::convert(areaValue) +
          "\n    Max area : " + tfel::utilities::convert(this->prec) + "\n";
    }
  }

}  // end of namespace tfel::check
