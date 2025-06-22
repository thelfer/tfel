/*!
 * \file  src/Math/KrigingUtilities.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 aoû 2010
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

#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/Kriging/KrigingUtilities.hxx"

namespace tfel {

  namespace math {

    static bool compareFloatingPointValues(const double a, const double b) {
      using namespace std;
      if ((abs(a) < 10 * numeric_limits<double>::min()) &&
          (abs(b) < 10 * numeric_limits<double>::min())) {
        // a and b are almost 0
        return true;
      }
      if ((abs(a - b) < abs(a) * 10 * numeric_limits<double>::epsilon()) ||
          (abs(a - b) < abs(b) * 10 * numeric_limits<double>::epsilon())) {
        return true;
      }
      return false;
    }  // end of compareFloatingPointValues

    std::pair<double, double> KrigingUtilities::normalize(
        const std::vector<double>& v) {
      using namespace std;
      pair<double, double> r;
      double max = *(max_element(v.begin(), v.end()));
      double min = *(min_element(v.begin(), v.end()));
      if (compareFloatingPointValues(max - min, 0.)) {
        ostringstream msg;
        msg << "KrigingUtilities::normalize : "
            << "values '" << min << "' and '" << max
            << "' are almost identical";
        throw(runtime_error(msg.str()));
      }
      r.first = 1. / (max - min);
      r.second = -min / r.first;
      return r;
    }

    std::pair<double, double> KrigingUtilities::normalize(
        const tfel::math::vector<double>& v) {
      using namespace std;
      pair<double, double> r;
      double max = *(max_element(v.begin(), v.end()));
      double min = *(min_element(v.begin(), v.end()));
      if (compareFloatingPointValues(max - min, 0.)) {
        ostringstream msg;
        msg << "KrigingUtilities::normalize : "
            << "values '" << min << "' and '" << max
            << "' are almost identical";
        throw(runtime_error(msg.str()));
      }
      r.first = 1. / (max - min);
      r.second = -min / r.first;
      return r;
    }  // end of KrigingUtilities::normalize

  }  // end of namespace math

}  // end of namespace tfel
