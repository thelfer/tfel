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
#include <iterator>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Check/Curve.hxx"
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

  AreaComparison::AreaComparison() { this->name = "area"; }

  AreaComparison::AreaComparison(const std::string& n, const Norm t) : norm(t) {
    this->name = n;
  }

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
    const auto ca = buildCurve(this->c1->getFilename(), getAbscissas(this->c1),
                               this->c1->getValues());
    const auto cb = buildCurve(this->c2->getFilename(), getAbscissas(this->c2),
                               this->c2->getValues());
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
    raise_if(times.size() < 2,
             "AreaComparison::compare: at least two distinct abscissas "
             "are required");
    const auto valA = evaluateCurve(*(this->integralInterpolation), ca, times);
    const auto valB = evaluateCurve(*(this->integralInterpolation), cb, times);
    std::vector<double> valuesDifference;
    this->msgLog += '\n';
    bool s = true;
    const auto l2 = this->norm == Norm::L2;
    for (size_type i = 0; i != valA.size(); ++i) {
      // make the difference between ref and res values
      const auto d = valA[i] - valB[i];
      valuesDifference.push_back(l2 ? d * d : std::abs(d));
    }
    // getting the value of the integral
    double areaValue = trapezoidalIntegration(times, valuesDifference);
    if (l2) {
      areaValue = std::sqrt(areaValue);
    }
    // the area is normalized by the maximum absolute value of the
    // reference column. A signed maximum would be negative for a negative
    // curve, and the check would always succeed.
    auto maxValueA = double{0};
    for (const auto v : this->c1->getValues()) {
      maxValueA = std::max(maxValueA, std::abs(v));
    }
    // no normalization if the reference column is null
    if (maxValueA > 0) {
      areaValue /= maxValueA;
    }

    // written this way so that a NaN fails
    if (!(areaValue <= this->prec)) {
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
    const auto error = l2 ? "L2 error" : "Area error";
    const auto max = l2 ? "Max L2 error" : "Max area";
    if (!s) {
      this->msgLog += " failed (" + this->name + " check).\n    " +
                      std::string{error} + " : ";
      this->msgLog += tfel::utilities::convert(areaValue);
      this->msgLog += " \n    " + std::string{max} + " = " +
                      tfel::utilities::convert(this->prec) + "\n";
      this->success = false;
    } else {
      this->msgLog += " succeed (" + this->name + " check).\n    " +
                      std::string{error} + " : " +
                      tfel::utilities::convert(areaValue) + "\n    " +
                      std::string{max} + " : " +
                      tfel::utilities::convert(this->prec) + "\n";
    }
  }

}  // end of namespace tfel::check
