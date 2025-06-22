/*
 * \file tfel-check/src/AreaComparison.cxx
 *
 * \date 12 juin 2013
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
#include <algorithm>

#include "TFEL/Check/AreaComparison.hxx"

namespace tfel::check {

  static double trapezoidalIntegration(const std::vector<double>& abscissas,
                                       const std::vector<double>& ordinates) {
    double result = 0;
    for (std::vector<double>::size_type i = 0; i < (abscissas.size() - 1);
         ++i) {
      double absInter = abscissas.at(i + 1) - abscissas.at(i);
      double ordInter = ordinates.at(i + 1) + ordinates.at(i);
      result += ((absInter * ordInter) / 2);
    }
    return result;
  }

  AreaComparison::AreaComparison() { this->name = "area"; }

  AreaComparison::~AreaComparison() = default;

  void AreaComparison::compare() {
    using size_type = std::vector<double>::size_type;
    // interpolation before integration
    auto colInterpolatedTmpA =
        std::make_shared<Column>(*(this->colIntegralInterpolated));
    auto colInterpolatedA(
        colInterpolatedTmpA); /**!< abscissa column for .ref */

    auto colInterpolatedTmpB =
        std::make_shared<Column>(*(this->colIntegralInterpolated));
    std::shared_ptr<Column> colInterpolatedB(
        colInterpolatedTmpB); /**!< abscissa column for .res */

    colInterpolatedA->setFilename(this->c1->getFilename());
    colInterpolatedB->setFilename(this->c2->getFilename());

    std::vector<double> valA(this->c1->getValues());
    std::vector<double> valB(this->c2->getValues());
    std::vector<double> timesA(colInterpolatedA->getValues());
    std::vector<double> timesB(colInterpolatedB->getValues());

    this->integralInterpolation->interpolate(timesB, valB);

    // interpolation of the column B
    for (size_type i = 0; i < timesA.size(); ++i) {
      for (size_type j = 0;
           (j < timesB.size()) && (timesA.at(i) != timesB.at(j)); ++j) {
        if (timesA.at(i) < timesB.at(j)) {  // if time from c1 not found in c2
          timesB.insert(timesB.begin() + j,
                        timesA.at(i));  // insert this time in times of c2
          valB.insert(
              valB.begin() + j,
              this->integralInterpolation->getValue(timesB.at(
                  j)));  // and insert the interpolated value at this time
          break;
        } else if ((j == (timesB.size() - 1)) &&
                   (timesA.at(i) >
                    timesB.at(
                        j))) {  // if value needs to be inserted at the end
          timesB.insert(timesB.end(),
                        timesA.at(i));  // insert this time in times of c2
          valB.insert(
              valB.end(),
              this->integralInterpolation->getValue(timesB.at(
                  j)));  // and insert the interpolated value at this time
          break;
        }
      }
    }

    this->integralInterpolation->interpolate(timesA, valA);

    // interpolation of the column A
    for (size_type i = 0; i < timesB.size(); ++i) {
      for (size_type j = 0;
           (j < timesA.size()) && (timesB.at(i) != timesA.at(j)); ++j) {
        if (timesB.at(i) < timesA.at(j)) {  // if time from c1 not found in c2
          timesA.insert(timesA.begin() + j,
                        timesB.at(i));  // insert this time in times of c2
          valA.insert(
              valA.begin() + j,
              this->integralInterpolation->getValue(timesA.at(
                  j)));  // and insert the interpolated value at this time
          break;
        } else if ((j == (timesA.size() - 1)) &&
                   (timesB.at(i) >
                    timesA.at(
                        j))) {  // if value needs to be inserted at the end
          timesA.insert(timesA.end(),
                        timesB.at(i));  // insert this time in times of c2
          valA.insert(
              valA.end(),
              this->integralInterpolation->getValue(timesA.at(
                  j)));  // and insert the interpolated value at this time
          break;
        }
      }
    }
    std::vector<double> valuesDifference;
    this->msgLog += '\n';
    bool s = true;
    for (size_type i = 0; i != valA.size(); ++i) {
      // make the difference between ref and res values
      valuesDifference.push_back(std::abs(valA.at(i) - valB.at(i)));
    }
    // getting the value of the integral
    double areaValue = trapezoidalIntegration(timesA, valuesDifference);
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
      this->msgLog += std::to_string(areaValue);
      this->msgLog += " \n    Max area = " + std::to_string(this->prec) + "\n";
      this->success = false;
    } else {
      this->msgLog +=
          " succeed (" + this->name +
          " check).\n    Area error : " + std::to_string(areaValue) +
          "\n    Max area : " + std::to_string(this->prec) + "\n";
    }
  }

}  // end of namespace tfel::check
