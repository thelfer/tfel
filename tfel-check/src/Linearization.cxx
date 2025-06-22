/*
 * Linearization.cpp
 *
 *  Created on: 13 mai 2013
 *      Author: rp238441
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <string>
#include <stdexcept>

#include "TFELCheck/Linearization.hxx"

namespace tfel_check {

  Linearization::Linearization() = default;
  Linearization::~Linearization() = default;

  Linearization::Linearization(const std::vector<double>& t,
                               const std::vector<double>& v) {
    if (t.size() != v.size()) {
      throw(
          std::runtime_error("Linearization::Linearization : "
                             "the number of values of the times don't match "
                             "the number of values of the evolution"));
    } else if (t.size() < 1) {
      throw(
          std::runtime_error("Linearization::Linearization : "
                             "wrong number of values for the times"));
    } else if (v.size() < 1) {
      throw(
          std::runtime_error("Linearization::Linearization : "
                             "wrong number of values for the ordinates"));
    }
    auto pX = t.begin();
    auto pY = v.begin();
    while (pX != t.end()) {
      this->values.insert({*pX, *pY});
      ++pX;
      ++pY;
    }

  }  // constructor

  double Linearization::operator()(const double x) const {
    if (this->values.empty()) {
      throw(
          std::runtime_error("Linearization::operator(): "
                             "no values specified"));
    }
    auto p = this->values.lower_bound(x);
    double x0;
    double x1;
    double y0;
    double y1;
    if (this->values.size() == 1u) {
      return this->values.begin()->second;
    }
    if (p == this->values.begin()) {
      return p->second;
    } else if (p == this->values.end()) {
      --p;
      return p->second;
    } else {
      x1 = p->first;
      y1 = p->second;
      --p;
      x0 = p->first;
      y0 = p->second;
    }
    return (y1 - y0) / (x1 - x0) * (x - x0) + y0;
  }  // operator()(const double x)

  bool Linearization::isConstant() const {
    return (this->values.size() == 1);
  }  // isConstant

} /* namespace tfel_check */
