/*
 * RelativeAndAbsoluteComparison.cxx
 *
 *  Created on: 28 mai 2013
 *      Author: rp238441
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <limits>

#include "TFELCheck/RelativeAndAbsoluteComparison.hxx"

namespace tfel_check {

  RelativeAndAbsoluteComparison::RelativeAndAbsoluteComparison() {
    this->name = "relativeAndAbsolute";
  }

  RelativeAndAbsoluteComparison::~RelativeAndAbsoluteComparison() = default;

  void RelativeAndAbsoluteComparison::compare() {
    using namespace std;
    TFEL_CONSTEXPR const double eps = 100. * numeric_limits<double>::min();
    unsigned int errorAbsLineNumber = 0;
    unsigned int errorRelLineNumber = 0;
    unsigned int errorLinesCount = 0;
    float errorLinesPercent = 0.;
    double absoluteError;
    double relativeError;
    double maxAbsoluteError = 0.;
    double maxRelativeError = 0.;
    unsigned int lineOffset =
        (!this->c2->getData()->getLegends().empty()) ? 2u : 1u;
    this->msgLog += '\n';

    bool s = true;
    for (vector<double>::size_type index = 0;
         index != this->c1->getValues().size(); ++index) {
      absoluteError =
          std::abs(this->c1->getValues()[index] - this->c2->getValues()[index]);
      relativeError = absoluteError / (min(abs(this->c1->getValues()[index]),
                                           abs(this->c2->getValues()[index])) +
                                       eps);

      if (maxRelativeError < relativeError) {
        maxRelativeError = relativeError;
        errorRelLineNumber = index + lineOffset;
      }

      // only store maxAbsoluteError if relative check failed
      if (relativeError > (this->prec)) {
        if (maxAbsoluteError < absoluteError) {
          maxAbsoluteError = absoluteError;
          errorAbsLineNumber = index + lineOffset;
        }
      }
      if ((relativeError > (this->prec)) &&
          (absoluteError > this->precision2)) {
        errorLinesCount++;
        s = false;
      }
    }

    if (!s) this->msgLog += "*** ";
    this->msgLog += "comparison of files " + this->c1->getFilename() + " and " +
                    this->c2->getFilename() + " using column ";

    this->msgLog += "'" + this->c1->getName() + "'";

    this->msgLog += ", with interpolation " + this->interpolationType;
    if (this->interpolationIsConform) {
      this->msgLog += " using column ";
      this->msgLog += this->ci->getName();
    }
    if (!s) {
      errorLinesPercent = (static_cast<double>(errorLinesCount) /
                           static_cast<double>(this->c1->getValues().size())) *
                          100.0;
      this->msgLog.append(" failed (" + this->name +
                          " check).\n    Maximum relative error : ");
      this->msgLog.append(std::to_string(maxRelativeError));
      this->msgLog +=
          " at line " + std::to_string(errorRelLineNumber) +
          "\n    Maximum absolute error : " + std::to_string(maxAbsoluteError) +
          " at line " + std::to_string(errorAbsLineNumber) +
          "\n    Failed comparisons count (for column) : " +
          std::to_string(errorLinesCount) + " / " +
          std::to_string(this->c1->getValues().size()) + " (" +
          std::to_string(errorLinesPercent) +
          " %)\n    Thresholds : rel = " + std::to_string(this->prec) +
          ", abs = " + std::to_string(this->precision2) + "\n";
      this->success = false;
    } else {
      this->msgLog += " succeed (" + this->name +
                      " check).\n    Maximum relative error : " +
                      std::to_string(maxRelativeError) + " at line " +
                      std::to_string(errorRelLineNumber) + "\n";
      if (errorAbsLineNumber) {
        this->msgLog +=
            "    Maximum absolute error : " + std::to_string(maxAbsoluteError) +
            " at line " + std::to_string(errorAbsLineNumber) + "\n";
      } else {
        this->msgLog += "    Absolute comparison not used.\n";
      }
      this->msgLog += "    Thresholds : rel = " + std::to_string(this->prec) +
                      ", abs = " + std::to_string(this->precision2);
    }
  }

} /* namespace tfel_check */
