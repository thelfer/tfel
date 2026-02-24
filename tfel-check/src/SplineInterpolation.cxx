/*!
 * \file tfel-check/src/SplineInterpolation.cxx
 *
 *  Created on: 7 mai 2013
 *      Author: rp238441
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Check/SplineInterpolation.hxx"

namespace tfel::check {

  SplineInterpolation::SplineInterpolation() = default;
  SplineInterpolation::SplineInterpolation(SplineInterpolation&&) = default;
  SplineInterpolation::SplineInterpolation(const SplineInterpolation&) =
      default;
  SplineInterpolation& SplineInterpolation::operator=(SplineInterpolation&&) =
      default;
  SplineInterpolation& SplineInterpolation::operator=(
      const SplineInterpolation&) = default;
  SplineInterpolation::~SplineInterpolation() = default;

  void SplineInterpolation::interpolate(const std::vector<double>& times,
                                        const std::vector<double>& values) {
    this->spline.setCollocationPoints(times, values);
  }

  double SplineInterpolation::getValue(const double x) const {
    return this->spline.getValue(x);
  }

  std::string SplineInterpolation::getType() const { return "spline"; }

  bool SplineInterpolation::isConform() const { return true; }

  std::shared_ptr<Interpolation> SplineInterpolation::clone() const {
    return std::make_shared<SplineInterpolation>(*this);
  }

}  // end of namespace tfel::check
