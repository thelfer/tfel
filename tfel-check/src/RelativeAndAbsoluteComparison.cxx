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

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <optional>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <limits>
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Check/RelativeAndAbsoluteComparison.hxx"

namespace tfel::check {

  RelativeAndAbsoluteComparison::RelativeAndAbsoluteComparison() {
    this->name = "relativeAndAbsolute";
  }

  RelativeAndAbsoluteComparison::~RelativeAndAbsoluteComparison() = default;

  void RelativeAndAbsoluteComparison::compare() {
    using namespace std;
    constexpr const double eps = 100. * numeric_limits<double>::min();
    std::optional<std::size_t> errorAbsLineNumber;
    std::optional<std::size_t> errorRelLineNumber;
    unsigned int errorLinesCount = 0;
    float errorLinesPercent = 0.;
    double absoluteError;
    double relativeError;
    double maxAbsoluteError = 0.;
    double maxRelativeError = 0.;
    this->msgLog += '\n';

    bool s = true;
    for (vector<double>::size_type index = 0;
         index != this->c1->getValues().size(); ++index) {
      absoluteError =
          std::abs(this->c1->getValues()[index] - this->c2->getValues()[index]);
      relativeError = absoluteError / (min(abs(this->c1->getValues()[index]),
                                           abs(this->c2->getValues()[index])) +
                                       eps);

      // a NaN is reported as the maximum error
      if ((!std::isnan(maxRelativeError)) &&
          (std::isnan(relativeError) || (maxRelativeError < relativeError))) {
        maxRelativeError = relativeError;
        errorRelLineNumber = index;
      }

      // only store maxAbsoluteError if relative check failed
      if (!(relativeError <= this->prec)) {
        // a NaN is reported as the maximum error
        if ((!std::isnan(maxAbsoluteError)) &&
            (std::isnan(absoluteError) || (maxAbsoluteError < absoluteError))) {
          maxAbsoluteError = absoluteError;
          errorAbsLineNumber = index;
        }
      }
      // written this way so that a NaN fails
      if (!((relativeError <= this->prec) ||
            (absoluteError <= this->precision2))) {
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
      this->msgLog.append(tfel::utilities::convert(maxRelativeError));
      this->msgLog += this->getLocation(errorRelLineNumber) +
                      "\n    Maximum absolute error : " +
                      tfel::utilities::convert(maxAbsoluteError) +
                      this->getLocation(errorAbsLineNumber) +
                      "\n    Failed comparisons count (for column) : " +
                      std::to_string(errorLinesCount) + " / " +
                      std::to_string(this->c1->getValues().size()) + " (" +
                      tfel::utilities::convert(errorLinesPercent) +
                      " %)\n    Thresholds : rel = " +
                      tfel::utilities::convert(this->prec) +
                      ", abs = " + tfel::utilities::convert(this->precision2) +
                      "\n";
      this->success = false;
    } else {
      this->msgLog += " succeed (" + this->name +
                      " check).\n    Maximum relative error : " +
                      tfel::utilities::convert(maxRelativeError) +
                      this->getLocation(errorRelLineNumber) + "\n";
      if (errorAbsLineNumber.has_value()) {
        this->msgLog += "    Maximum absolute error : " +
                        tfel::utilities::convert(maxAbsoluteError) +
                        this->getLocation(errorAbsLineNumber) + "\n";
      } else {
        this->msgLog += "    Absolute comparison not used.\n";
      }
      this->msgLog +=
          "    Thresholds : rel = " + tfel::utilities::convert(this->prec) +
          ", abs = " + tfel::utilities::convert(this->precision2);
    }
  }

}  // end of namespace tfel::check
