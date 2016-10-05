/*!
 * SplineInterpolation.cpp
 *
 *  Created on: 7 mai 2013
 *      Author: rp238441
 */

#include "TFELCheck/SplineInterpolation.hxx"

namespace tfel_check {

  SplineInterpolation::SplineInterpolation()  = default;
  SplineInterpolation::SplineInterpolation(SplineInterpolation&&)  = default;
  SplineInterpolation::SplineInterpolation(const SplineInterpolation&)  = default;
  SplineInterpolation& SplineInterpolation::operator=(SplineInterpolation&&)  = default;
  SplineInterpolation& SplineInterpolation::operator=(const SplineInterpolation&)  = default;
  SplineInterpolation::~SplineInterpolation() = default;
  
  void SplineInterpolation::interpolate(const std::vector<double>& times,
					const std::vector<double>& values) {
    this->spline.setCollocationPoints(times, values);
  }

  double SplineInterpolation::getValue(const double x) const {
    return this->spline.getValue(x);
  }

  std::string SplineInterpolation::getType() const {
    return "spline";
  }

  bool SplineInterpolation::isConform() const {
    return true;
  }

  std::shared_ptr<Interpolation> SplineInterpolation::clone() const {
    return std::shared_ptr<Interpolation>(new SplineInterpolation(*this));
  }

} /* namespace tfel_check */
