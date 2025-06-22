/*
 * \file tfel-check/src/LinearInterpolation.cxx
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

#include "TFEL/Check/LinearInterpolation.hxx"

namespace tfel {

  namespace check {

    LinearInterpolation::LinearInterpolation() = default;
    LinearInterpolation::LinearInterpolation(LinearInterpolation&&) = default;
    LinearInterpolation::LinearInterpolation(const LinearInterpolation&) =
        default;
    LinearInterpolation& LinearInterpolation::operator=(LinearInterpolation&&) =
        default;
    LinearInterpolation& LinearInterpolation::operator=(
        const LinearInterpolation&) = default;
    LinearInterpolation::~LinearInterpolation() = default;

    void LinearInterpolation::interpolate(const std::vector<double>& times,
                                          const std::vector<double>& values) {
      this->linear = Linearization(times, values);
    }

    double LinearInterpolation::getValue(const double x) const {
      return this->linear(x);
    }

    std::string LinearInterpolation::getType() const { return "linear"; }

    bool LinearInterpolation::isConform() const { return true; }

    std::shared_ptr<Interpolation> LinearInterpolation::clone() const {
      return std::make_shared<LinearInterpolation>(*this);
    }

  }  // end of namespace check

}  // end of namespace tfel
