/*!
 * \file tfel-check/src/SplineLocalInterpolation.cxx
 *
 *  Created on: 3 juin 2013
 *      Author: rp238441
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Check/SplineLocalInterpolation.hxx"

namespace tfel::check {

  SplineLocalInterpolation::SplineLocalInterpolation() = default;
  SplineLocalInterpolation::SplineLocalInterpolation(
      const SplineLocalInterpolation&) = default;
  SplineLocalInterpolation::SplineLocalInterpolation(
      SplineLocalInterpolation&&) noexcept = default;
  SplineLocalInterpolation& SplineLocalInterpolation::operator=(
      const SplineLocalInterpolation&) = default;
  SplineLocalInterpolation& SplineLocalInterpolation::operator=(
      SplineLocalInterpolation&&) noexcept = default;
  SplineLocalInterpolation::~SplineLocalInterpolation() = default;

  void SplineLocalInterpolation::interpolate(
      const std::vector<double>& times, const std::vector<double>& values) {
    raise_if(times.size() != values.size(),
             "SplineLocalInterpolation::interpolate: "
             "the number of times does not match the number of values");
    raise_if(times.empty(), "SplineLocalInterpolation::interpolate: no values");
    // removes the splines of a previous call
    this->splines.clear();
    this->timesBefore.clear();
    // with less than three points, only one spline is built
    if (times.size() < 3) {
      tfel::math::CubicSpline<double> spline;
      spline.setCollocationPoints(times, values);
      this->splines[times.front()] = spline;
      this->timesBefore.push_back(times.front());
      return;
    }
    // starts at second element, and stops before the last to prevent
    // duplicated splines
    for (unsigned i = 1; i < (times.size() - 1); ++i) {
      std::vector<double> timesTmp;
      std::vector<double> valuesTmp;

      timesTmp.push_back(times[i - 1]);
      timesTmp.push_back(times[i]);
      timesTmp.push_back(times[i + 1]);
      valuesTmp.push_back(values[i - 1]);
      valuesTmp.push_back(values[i]);
      valuesTmp.push_back(values[i + 1]);

      tfel::math::CubicSpline<double> spline;
      spline.setCollocationPoints(timesTmp, valuesTmp);
      this->splines[times[i - 1]] = spline;
      this->timesBefore.push_back(times[i - 1]);
    }
  }

  double SplineLocalInterpolation::getValue(const double x) const {
    // if we have only one spline, return it
    if (this->timesBefore.size() == 1) {
      return (this->splines.find(this->timesBefore.at(0)))->second.getValue(x);
    }
    // else, we search the right spline
    for (unsigned i = 0; i < this->timesBefore.size(); ++i) {
      if (this->timesBefore.at(i) > x) {
        // before the first spline, the first one is used
        const auto t = this->timesBefore.at(i == 0 ? 0 : i - 1);
        return (this->splines.find(t))->second.getValue(x);
      }
    }
    return this->splines.find(this->timesBefore.back())->second.getValue(x);
  }

  std::string SplineLocalInterpolation::getType() const {
    return "local spline";
  }

  bool SplineLocalInterpolation::isConform() const { return true; }

  std::shared_ptr<Interpolation> SplineLocalInterpolation::clone() const {
    return std::make_shared<SplineLocalInterpolation>(*this);
  }

}  // end of namespace tfel::check
