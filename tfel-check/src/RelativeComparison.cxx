/*!
 * \file tfel-check/src/RelativeComparison.cxx
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
#include <vector>
#include <cstdlib>
#include <cmath>
#include <limits>
#include "TFEL/Check/RelativeComparison.hxx"

namespace tfel {

  namespace check {

    RelativeComparison::RelativeComparison() { this->name = "relative"; }

    RelativeComparison::~RelativeComparison() = default;

    void RelativeComparison::compare() {
      using namespace std;
      TFEL_CONSTEXPR const double eps = 100. * numeric_limits<double>::min();
      unsigned int errorLineNumber = 0;
      unsigned int errorLinesCount = 0;
      float errorLinesPercent = 0.;
      double relativeError;
      double maxRelativeError = 0.;
      unsigned int lineOffset =
          (!this->c2->getData()->getLegends().empty()) ? 2u : 1u;
      this->msgLog += '\n';

      bool s = true;
      for (vector<double>::size_type index = 0;
           index != this->c1->getValues().size(); ++index) {
        relativeError = std::abs(this->c1->getValues()[index] -
                                 this->c2->getValues()[index]) /
                        (min(abs(this->c1->getValues()[index]),
                             abs(this->c2->getValues()[index])) +
                         eps);
        if (maxRelativeError < relativeError) {
          maxRelativeError = relativeError;
          errorLineNumber = index + lineOffset;
        }
        if (relativeError > (this->prec)) {
          errorLinesCount++;
          s = false;
        }
      }

      if (!s) this->msgLog += "*** ";
      this->msgLog += "comparison of files " + this->c1->getFilename() +
                      " and " + this->c2->getFilename() + " using column ";

      this->msgLog += "'" + this->c1->getName() + "'";

      this->msgLog += ", with interpolation " + this->interpolationType;
      if (this->interpolationIsConform) {
        this->msgLog += " using column ";
        this->msgLog += this->ci->getName();
      }
      if (!s) {
        errorLinesPercent =
            (static_cast<double>(errorLinesCount) /
             static_cast<double>(this->c1->getValues().size())) *
            100.0;
        this->msgLog.append(" failed (" + this->name +
                            " check).\n    Maximum relative error : ");
        this->msgLog.append(std::to_string(maxRelativeError));
        this->msgLog +=
            " at line " + std::to_string(errorLineNumber) +
            "\n    Failed comparisons (for column) : " +
            std::to_string(errorLinesCount) + " / " +
            std::to_string(this->c1->getValues().size()) + " (" +
            std::to_string(errorLinesPercent) +
            " %)\n    Threshold : rel = " + std::to_string(this->prec) + "\n";
        this->success = false;
      } else {
        this->msgLog +=
            " succeed (" + this->name +
            " check).\n    Maximum relative error : " +
            std::to_string(maxRelativeError) +
            "\n    Threshold : rel = " + std::to_string(this->prec) + "\n";
      }
    }

  }  // end of namespace check

}  // end of namespace tfel
