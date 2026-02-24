/*!
 * \file  src/Math/KrigingUtilities.cxx
 * \brief
 * \author Thomas Helfer
 * \date 14/08/2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include <stdexcept>
#include <sstream>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/Kriging/KrigingUtilities.hxx"

namespace tfel::math {

  static bool compareFloatingPointValues(const double a, const double b) {
    constexpr const auto emin = std::numeric_limits<double>::min();
    constexpr const auto eps = std::numeric_limits<double>::epsilon();
    const auto aa = std::abs(a);
    const auto ab = std::abs(b);
    const auto d = std::abs(a - b);
    if ((aa < 10 * emin) && (ab < 10 * emin)) {
      // a and b are almost 0
      return true;
    }
    return ((d < aa * 10 * eps) || (d < ab * 10 * eps));
  }  // end of compareFloatingPointValues

  std::pair<double, double> KrigingUtilities::normalize(
      const std::vector<double>& v) {
    const auto max = *(max_element(v.begin(), v.end()));
    const auto min = *(min_element(v.begin(), v.end()));
    raise_if(compareFloatingPointValues(max - min, 0.),
             "KrigingUtilities::normalize: "
             "values '" +
                 std::to_string(min) + "' and '" + std::to_string(max) +
                 "' are almost identical");
    return {1 / (max - min), -min / (max - min)};
  }

  std::pair<double, double> KrigingUtilities::normalize(
      const tfel::math::vector<double>& v) {
    const auto max = *(std::max_element(v.begin(), v.end()));
    const auto min = *(std::min_element(v.begin(), v.end()));
    raise_if(compareFloatingPointValues(max - min, 0.),
             "KrigingUtilities::normalize: "
             "values '" +
                 std::to_string(min) + "' and '" + std::to_string(max) +
                 "' are almost identical");
    return {1 / (max - min), -min / (max - min)};
  }  // end of KrigingUtilities::normalize

}  // end of namespace tfel::math
