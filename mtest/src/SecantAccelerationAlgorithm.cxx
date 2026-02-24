/*!
 * \file  mtest/src/SecantAccelerationAlgorithm.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 juin 2014
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
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/SecantAccelerationAlgorithm.hxx"

namespace mtest {

  SecantAccelerationAlgorithm::SecantAccelerationAlgorithm()
      : sa_w(1.),
        sat(-1) {
  }  // end of SecantAccelerationAlgorithm::SecantAccelerationAlgorithm

  std::string SecantAccelerationAlgorithm::getName() const { return "secant"; }

  void SecantAccelerationAlgorithm::setParameter(const std::string& p,
                                                 const std::string& v) {
    const std::string m = "SecantAccelerationAlgorithm::setParameter";
    if (p == "AccelerationTrigger") {
      const auto i = AccelerationAlgorithm::convertToUnsignedShort(m, v);
      tfel::raise_if(this->sat != -1, m + ": the acceleration trigger "
                                          "has already been defined");
      tfel::raise_if(i < 3, "m : invalid acceleration trigger value.");
      this->sat = i;
    } else {
      tfel::raise(m + ": invalid parameter '" + p + "'.");
    }
  }  // end of SecantAccelerationAlgorithm::setParameter

  void SecantAccelerationAlgorithm::initialize(const unsigned short psz) {
    this->sa_u0.resize(psz, 0.);
    this->sa_u1.resize(psz, 0.);
    this->sa_r0.resize(psz, 0.);
    this->sa_r1.resize(psz, 0.);
    this->sa_r.resize(psz, 0.);
    this->sa_dr.resize(psz, 0.);
    if (this->sat == -1) {
      this->sat = 3;
    }
  }  // end of SecantAccelerationAlgorithm::initialize

  void SecantAccelerationAlgorithm::preExecuteTasks() {
    this->sa_w = std::max(std::min(this->sa_w, 1.), -1.);
  }  // end of AccelerationAlgorithm::preExecuteTaks

  void SecantAccelerationAlgorithm::execute(tfel::math::vector<real>& u1,
                                            const tfel::math::vector<real>&,
                                            const tfel::math::vector<real>& r,
                                            const real,
                                            const real seps,
                                            const unsigned short iter) {
    const real sa_eps = 100 * seps * std::numeric_limits<real>::epsilon();
    this->sa_u0.swap(this->sa_u1);
    this->sa_r0.swap(this->sa_r1);
    this->sa_r1 = r;
    this->sa_u1 = u1;
    this->sa_dr = this->sa_r1 - this->sa_r0;
    if (iter >= this->sat) {
      if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
        auto& log = mfront::getLogStream();
        log << "Secant acceleration convergence\n";
      }
      const real nr2_dr = this->sa_dr | this->sa_dr;
      if (nr2_dr > sa_eps) {
        const real sa_a = (this->sa_r1 | this->sa_dr) / nr2_dr;
        u1 -= sa_a * (this->sa_u1 - this->sa_u0);
      }
    }
  }  // end of SecantAccelerationAlgorithm::execute

  void SecantAccelerationAlgorithm::postExecuteTasks() {
  }  // end of AccelerationAlgorithm::postExecuteTaks

  SecantAccelerationAlgorithm::~SecantAccelerationAlgorithm() = default;

}  // end of namespace mtest
