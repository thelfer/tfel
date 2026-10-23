/*!
 * MixedComparison.cxx
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
#include <cstddef>
#include <optional>
#include <sstream>
#include <iomanip>
#include <vector>
#include <limits>
#include <cstdlib>

#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Check/MixedComparison.hxx"

namespace tfel::check {

  MixedComparison::MixedComparison() { this->name = "mixed"; }

  MixedComparison::~MixedComparison() = default;

  void MixedComparison::compare() {
    using namespace std;
    double mixedError;
    double maxMixedError =
        -std::numeric_limits<double>::max();  // smallest possible value
    unsigned int errorLinesCount = 0;
    float errorLinesPercent = 0.;
    std::optional<std::size_t> errorLineNumber;

    this->msgLog += '\n';

    bool s = true;
    for (vector<double>::size_type index = 0;
         index != this->c1->getValues().size(); ++index) {
      const auto va = this->c1->getValues().at(index);
      const auto vb = this->c2->getValues().at(index);
      // the relative tolerance is scaled by the absolute value of vb:
      // a negative value of vb would make the check fail for identical
      // values
      mixedError =
          std::abs(va - vb) - (this->prec * std::abs(vb)) - this->precision2;
      // a NaN is reported as the maximum error
      if ((!std::isnan(maxMixedError)) &&
          (std::isnan(mixedError) || (maxMixedError < mixedError))) {
        maxMixedError = mixedError;
        errorLineNumber = index;
      }
      // written this way so that a NaN fails
      if (!(mixedError <= 0)) {
        s = false;
        errorLinesCount++;
      }
    }
    if (!s) {
      this->msgLog += "*** ";
    }
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
                          " check).\n    Maximum mixed error : ");
      this->msgLog.append(tfel::utilities::convert(maxMixedError));
      this->msgLog +=
          this->getLocation(errorLineNumber) +
          "\n    Failed comparisons (for column) : " +
          std::to_string(errorLinesCount) + " / " +
          std::to_string(this->c1->getValues().size()) + " (" +
          tfel::utilities::convert(errorLinesPercent) +
          " %)\n    Threshold : rel = " + tfel::utilities::convert(this->prec) +
          ", abs = " + tfel::utilities::convert(this->precision2) + "\n";
      this->success = false;
    } else {
      this->msgLog +=
          " succeed (" + this->name + " check).\n    Maximum mixed error : " +
          tfel::utilities::convert(maxMixedError) +
          "\n    Threshold : rel = " + tfel::utilities::convert(this->prec) +
          ", abs = " + tfel::utilities::convert(this->precision2) + "\n";
    }
  }

}  // end of namespace tfel::check
