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
#include <sstream>
#include <iomanip>
#include <vector>
#include <limits>
#include <cstdlib>

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
    unsigned int errorLineNumber = 0;
    unsigned int lineOffset =
        (!this->c2->getData()->getLegends().empty()) ? 2u : 1u;

    this->msgLog += '\n';

    bool s = true;
    for (vector<double>::size_type index = 0;
         index != this->c1->getValues().size(); ++index) {
      const auto va = this->c1->getValues().at(index);
      const auto vb = this->c2->getValues().at(index);
      mixedError = std::abs(va - vb) - (this->prec * vb) - this->precision2;
      if (maxMixedError < mixedError) {
        maxMixedError = mixedError;
        errorLineNumber = index + lineOffset;
      }
      if (mixedError > 0) {
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
      this->msgLog.append(std::to_string(maxMixedError));
      this->msgLog +=
          " at line " + std::to_string(errorLineNumber) +
          "\n    Failed comparisons (for column) : " +
          std::to_string(errorLinesCount) + " / " +
          std::to_string(this->c1->getValues().size()) + " (" +
          std::to_string(errorLinesPercent) +
          " %)\n    Threshold : rel = " + std::to_string(this->prec) +
          ", abs = " + std::to_string(this->precision2) + "\n";
      this->success = false;
    } else {
      this->msgLog += " succeed (" + this->name +
                      " check).\n    Maximum mixed error : " +
                      std::to_string(maxMixedError) +
                      "\n    Threshold : rel = " + std::to_string(this->prec) +
                      ", abs = " + std::to_string(this->precision2) + "\n";
    }
  }

}  // end of namespace tfel::check
