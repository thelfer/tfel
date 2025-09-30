/*!
 * \file  mtest/src/AlternateSecantAccelerationAlgorithm.cxx
 * \brief
 * \author Ramiere Isabelle
 * \brief 16 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <limits>
#include <ostream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/AlternateSecantAccelerationAlgorithm.hxx"

namespace mtest {

  AlternateSecantAccelerationAlgorithm::AlternateSecantAccelerationAlgorithm()
      : asat(-1) {
  }  // end of
     // AlternateSecantAccelerationAlgorithm::AlternateSecantAccelerationAlgorithm

  std::string AlternateSecantAccelerationAlgorithm::getName() const {
    return "crossed secant";
  }

  void AlternateSecantAccelerationAlgorithm::setParameter(
      const std::string& p, const std::string& v) {
    const std::string m = "AlternateSecantAccelerationAlgorithm::setParameter";
    if (p == "AccelerationTrigger") {
      const auto i = AccelerationAlgorithm::convertToUnsignedShort(m, v);
      tfel::raise_if(this->asat != -1, m + ": the castem acceleration trigger "
                                           "has already been defined");
      tfel::raise_if(i < 2, m + ": invalid acceleration trigger value.");
      this->asat = i;
    } else {
      tfel::raise(m + ": invalid parameter '" + p + "'.");
    }
  }  // end of AlternateSecantAccelerationAlgorithm::setParameter

  void AlternateSecantAccelerationAlgorithm::initialize(
      const unsigned short psz) {
    this->asa_u0.resize(psz, 0.);
    this->asa_u1.resize(psz, 0.);
    this->asa_du.resize(psz, 0.);
    this->asa_r0.resize(psz, 0.);
    this->asa_r1.resize(psz, 0.);
    this->asa_dr.resize(psz, 0.);
    if (this->asat == -1) {
      this->asat = 2;
    }
  }  // end of AlternateSecantAccelerationAlgorithm::initialize

  void AlternateSecantAccelerationAlgorithm::preExecuteTasks() {
  }  // end of AccelerationAlgorithm::preExecuteTaks

  void AlternateSecantAccelerationAlgorithm::execute(
      tfel::math::vector<real>& u1,
      const tfel::math::vector<real>& rx,
      const tfel::math::vector<real>&,
      const real eeps,
      const real,
      const unsigned short iter) {
    const real asa_eps = 100 * eeps * std::numeric_limits<real>::epsilon();
    this->asa_u0.swap(this->asa_u1);  // previous G(u0) stored
    this->asa_r0.swap(this->asa_r1);
    this->asa_r1 = -rx;  // rx = u0 - G(u0) or rf = F(u0)
    this->asa_u1 = u1;   // G(u0)
    this->asa_du = this->asa_u1 - this->asa_u0;
    this->asa_dr = this->asa_r1 - this->asa_r0;
    if (iter >= this->asat) {
      const real nr2_dr = this->asa_dr | this->asa_dr;
      if (nr2_dr > (asa_eps * asa_eps)) {
        if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
          auto& log = mfront::getLogStream();
          log << "AlternateSecant acceleration convergence\n";
        }
        const real asa_a = (this->asa_dr | this->asa_r1) / nr2_dr;
        u1 -= asa_a * (this->asa_du);
      }
    }

  }  // end of AlternateSecantAccelerationAlgorithm::execute

  void AlternateSecantAccelerationAlgorithm::postExecuteTasks() {
  }  // end of AccelerationAlgorithm::postExecuteTaks

  AlternateSecantAccelerationAlgorithm::
      ~AlternateSecantAccelerationAlgorithm() = default;

}  // end of namespace mtest
