/*!
 * \file tfel-check/src/Linearization.cxx
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

#include "TFEL/Raise.hxx"
#include "TFEL/Check/Linearization.hxx"

namespace tfel {

  namespace check {

    Linearization::Linearization() = default;
    Linearization::Linearization(Linearization&&) = default;
    Linearization::Linearization(const Linearization&) = default;
    Linearization& Linearization::operator=(Linearization&&) = default;
    Linearization& Linearization::operator=(const Linearization&) = default;

    Linearization::~Linearization() = default;

    Linearization::Linearization(const std::vector<double>& t,
                                 const std::vector<double>& v) {
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b, "Linearization::Linearization: " + m);
      };
      throw_if(t.size() != v.size(),
               "the number of values of the times don't match "
               "the number of values of the evolution");
      throw_if(t.empty(), "wrong number of values for the times");
      throw_if(v.empty(), "wrong number of values for the ordinates");
      auto pX = t.begin();
      auto pY = v.begin();
      while (pX != t.end()) {
        this->values.insert({*pX, *pY});
        ++pX;
        ++pY;
      }
    }  // constructor

    double Linearization::operator()(const double x) const {
      raise_if(this->values.empty(),
               "Linearization::operator(): "
               "no values specified");
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

  }  // end of namespace check

}  // end of namespace tfel
