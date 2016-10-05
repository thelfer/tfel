/*
 * LinearInterpolation.cpp
 *
 *  Created on: 7 mai 2013
 *      Author: rp238441
 */

#include "TFELCheck/LinearInterpolation.hxx"

namespace tfel_check {

  LinearInterpolation::LinearInterpolation()  = default;
  LinearInterpolation::LinearInterpolation(LinearInterpolation&&)  = default;
  LinearInterpolation::LinearInterpolation(const LinearInterpolation&)  = default;
  LinearInterpolation& LinearInterpolation::operator=(LinearInterpolation&&)  = default;
  LinearInterpolation& LinearInterpolation::operator=(const LinearInterpolation&)  = default;
  LinearInterpolation::~LinearInterpolation() = default;
  
  void LinearInterpolation::interpolate(const std::vector<double>& times,
					const std::vector<double>& values) {
    this->linear = Linearization(times, values);
  }

  double LinearInterpolation::getValue(const double x) const {

    return this->linear(x);
  }

  std::string LinearInterpolation::getType() const {
    return "linear";
  }

  bool LinearInterpolation::isConform() const {
    return true;
  }

  std::shared_ptr<Interpolation> LinearInterpolation::clone() const {
    return std::shared_ptr<Interpolation>(new LinearInterpolation(*this));
  }

} /* namespace tfel_check */
