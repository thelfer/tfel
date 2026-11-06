/*!
 * \file  mtest/src/ImposedThermodynamicForce.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>

#include "MTest/Behaviour.hxx"
#include "MTest/ImposedThermodynamicForce.hxx"

namespace mtest {

  ImposedThermodynamicForce::ImposedThermodynamicForce(
      const Behaviour& b,
      const std::string& cname,
      const std::shared_ptr<Evolution> s_)
      : sev(s_) {
    using tfel::material::MechanicalBehaviourBase;
    this->c = b.getThermodynamicForceComponentPosition(cname);
    // for finite strain behaviours, the equation associated with an
    // off-diagonal component of the Cauchy stress is duplicated on the rows of
    // the two associated components of the deformation gradient
    if ((b.getBehaviourType() ==
         MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR) &&
        (this->c >= 3)) {
      const auto i = static_cast<unsigned short>(2 * (this->c - 3) + 3);
      this->rows = {i, static_cast<unsigned short>(i + 1)};
    } else {
      this->rows = {this->c};
    }
  }  // end of ImposedThermodynamicForce::ImposedThermodynamicForce

  ImposedThermodynamicForce::ImposedThermodynamicForce(
      const unsigned short c_, const std::shared_ptr<Evolution> s_)
      : sev(s_),
        c(c_),
        rows({c_}) {
  }  // end of ImposedThermodynamicForce::ImposedThermodynamicForce

  unsigned short ImposedThermodynamicForce::getNumberOfLagrangeMultipliers()
      const {
    return 0u;
  }  // end of ImposedThermodynamicForce::getNumberOfLagrangeMultipliers

  void ImposedThermodynamicForce::setValues(tfel::math::matrix<real>&,
                                            tfel::math::vector<real>& r,
                                            const tfel::math::vector<real>&,
                                            const tfel::math::vector<real>&,
                                            const tfel::math::matrix<real>&,
                                            const tfel::math::vector<real>&,
                                            const unsigned short,
                                            const unsigned short,
                                            const real t,
                                            const real dt,
                                            const real) const {
    if (this->isActive()) {
      const auto v = this->sev->operator()(t + dt);
      for (const auto i : this->rows) {
        r(i) -= v;
      }
    }
  }  // end of ImposedThermodynamicForce::setValues

  bool ImposedThermodynamicForce::checkConvergence(
      const tfel::math::vector<real>&,
      const tfel::math::vector<real>& s,
      const real,
      const real seps,
      const real t,
      const real dt) const {
    const auto& sv = *(this->sev);
    return std::abs(s(this->c) - sv(t + dt)) < seps;
  }

  std::string ImposedThermodynamicForce::getFailedCriteriaDiagnostic(
      const tfel::math::vector<real>&,
      const tfel::math::vector<real>& s,
      const real,
      const real seps,
      const real t,
      const real dt) const {
    const auto& sv = *(this->sev);
    std::ostringstream msg;
    msg << "imposed thermodynmic force not reached for component " << this->c
        << " (imposed value : " << sv(t + dt)
        << ", computed value : " << s(this->c) << ", criteria : " << seps
        << ")";
    return msg.str();
  }

  ImposedThermodynamicForce::~ImposedThermodynamicForce() = default;

}  // end of namespace mtest
