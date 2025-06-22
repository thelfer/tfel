/*!
 * \file  mfront/mtest/ImposedDrivingVariable.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <sstream>

#include "MTest/Behaviour.hxx"
#include "MTest/ImposedDrivingVariable.hxx"

namespace mtest {

  ImposedDrivingVariable::ImposedDrivingVariable(
      const Behaviour& b,
      const std::string& cname,
      const std::shared_ptr<Evolution> e_)
      : eev(e_) {
    this->c = b.getDrivingVariableComponentPosition(cname);
  }  // end of ImposedDrivingVariable::ImposedDrivingVariable

  ImposedDrivingVariable::ImposedDrivingVariable(
      const unsigned short c_, const std::shared_ptr<Evolution> e_)
      : eev(e_),
        c(c_) {}  // end of ImposedDrivingVariable::ImposedDrivingVariable

  unsigned short ImposedDrivingVariable::getNumberOfLagrangeMultipliers()
      const {
    return 1u;
  }  // end of ImposedDrivingVariable::getNumberOfLagrangeMultipliers

  void ImposedDrivingVariable::setValues(tfel::math::matrix<real>& K,
                                         tfel::math::vector<real>& r,
                                         const tfel::math::vector<real>&,
                                         const tfel::math::vector<real>& u1,
                                         const unsigned short pos,
                                         const unsigned short,
                                         const real t,
                                         const real dt,
                                         const real a) const {
    const auto& e = *(this->eev);
    K(pos, this->c) -= a;
    K(this->c, pos) -= a;
    r(this->c) -= a * u1(pos);
    r(pos) -= a * (u1(this->c) - e(t + dt));
  }  // end of ImposedDrivingVariable::setValues

  bool ImposedDrivingVariable::checkConvergence(
      const tfel::math::vector<real>& u,
      const tfel::math::vector<real>&,
      const real eeps,
      const real,
      const real t,
      const real dt) const {
    const auto& e = *(this->eev);
    return std::abs(u(this->c) - e(t + dt)) < eeps;
  }

  std::string ImposedDrivingVariable::getFailedCriteriaDiagnostic(
      const tfel::math::vector<real>& e,
      const tfel::math::vector<real>&,
      const real eeps,
      const real,
      const real t,
      const real dt) const {
    const auto& ev = *(this->eev);
    std::ostringstream msg;
    msg << "imposed driving variable not reached for component " << this->c
        << " (imposed value : " << ev(t + dt)
        << ", computed value : " << e(this->c) << ", criteria : " << eeps
        << ")";
    return msg.str();
  }

  ImposedDrivingVariable::~ImposedDrivingVariable() {
  }  // end of ImposedDrivingVariable::~ImposedDrivingVariable

}  // end of namespace mtest
