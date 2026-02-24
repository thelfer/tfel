/*
 * \file tfel-check/src/AbsoluteComparison.cxx
 *
 *  Created on: 27 mai 2013
 *      Author: rp238441
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <vector>
#include <cstdlib>
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Check/AbsoluteComparison.hxx"

namespace tfel::check {

  AbsoluteComparison::AbsoluteComparison() { this->name = "absolute"; }

  AbsoluteComparison::~AbsoluteComparison() = default;

  void AbsoluteComparison::compare() {
    using namespace std;
    unsigned int errorLineNumber = 0;
    unsigned int errorLinesCount = 0;
    float errorLinesPercent = 0.;
    double absoluteError;
    double maxAbsoluteError = 0.;
    unsigned int lineOffset =
        (!this->c2->getData()->getLegends().empty()) ? 2u : 1u;
    this->msgLog += '\n';

    bool s = true;
    for (vector<double>::size_type idx = 0; idx != this->c1->getValues().size();
         ++idx) {
      absoluteError =
          std::abs(this->c1->getValues()[idx] - this->c2->getValues()[idx]);
      if (maxAbsoluteError < absoluteError) {
        maxAbsoluteError = absoluteError;
        errorLineNumber = idx + lineOffset;
      }
      if (absoluteError > this->prec) {
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
      this->msgLog +=
          " failed (" + this->name + " check).\n    Maximum absolute error : ";
      this->msgLog += tfel::utilities::convert(maxAbsoluteError);
      this->msgLog +=
          " at line " + std::to_string(errorLineNumber) +
          "\n    Failed comparisons (for column) : " +
          std::to_string(errorLinesCount) + " / " +
          std::to_string(this->c1->getValues().size()) + " (" +
          tfel::utilities::convert(errorLinesPercent) +
          " %)\n    Threshold : abs = " + tfel::utilities::convert(this->prec) +
          "\n";
      this->success = false;
    } else {
      this->msgLog +=
          " succeed (" + this->name +
          " check).\n    Maximum absolute error : " +
          tfel::utilities::convert(maxAbsoluteError) +
          "\n    Threshold : abs = " + tfel::utilities::convert(this->prec) +
          "\n";
    }
  }

}  // end of namespace tfel::check
