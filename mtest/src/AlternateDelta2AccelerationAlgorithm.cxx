/*!
 * \file  mtest/src/AlternateDelta2AccelerationAlgorithm.cxx
 * \brief
 * \author Ramiere Isabelle
 * \brief 19 mars 2015
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
#include "MTest/AlternateDelta2AccelerationAlgorithm.hxx"

namespace mtest {

  AlternateDelta2AccelerationAlgorithm::AlternateDelta2AccelerationAlgorithm()
      : asat(-1) {
  }  // end of
     // AlternateDelta2AccelerationAlgorithm::AlternateDelta2AccelerationAlgorithm

  std::string AlternateDelta2AccelerationAlgorithm::getName() const {
    return "crossed secant";
  }

  void AlternateDelta2AccelerationAlgorithm::setParameter(
      const std::string& p, const std::string& v) {
    const std::string m = "AlternateDelta2AccelerationAlgorithm::setParameter";
    if (p == "AccelerationTrigger") {
      const auto i = AccelerationAlgorithm::convertToUnsignedShort(m, v);
      tfel::raise_if(this->asat != -1, m + ": the acceleration trigger "
                                           "has already been defined");
      tfel::raise_if(i < 2, m + ": invalid acceleration trigger value.");
      this->asat = i;
    } else {
      tfel::raise(m + ": invalid parameter '" + p + "'.");
    }
  }  // end of AlternateDelta2AccelerationAlgorithm::setParameter

  void AlternateDelta2AccelerationAlgorithm::initialize(
      const unsigned short psz) {
    this->asa_u0.resize(psz, 0.);
    this->asa_u1.resize(psz, 0.);
    this->asa_du0.resize(psz, 0.);
    this->asa_du1.resize(psz, 0.);
    this->asa_d2u.resize(psz, 0.);
    this->asa_r0.resize(psz, 0.);
    this->asa_r1.resize(psz, 0.);
    this->asa_dr0.resize(psz, 0.);
    this->asa_dr1.resize(psz, 0.);
    this->asa_d2r.resize(psz, 0.);
    if (this->asat == -1) {
      this->asat = 3;
    }
  }  // end of AlternateDelta2AccelerationAlgorithm::initialize

  void AlternateDelta2AccelerationAlgorithm::preExecuteTasks() {
  }  // end of AccelerationAlgorithm::preExecuteTaks

  void AlternateDelta2AccelerationAlgorithm::execute(
      tfel::math::vector<real>& u1,
      const tfel::math::vector<real>& rx,
      const tfel::math::vector<real>&,
      const real eeps,
      const real,
      const unsigned short iter) {
    const real asa_eps = 100 * eeps * std::numeric_limits<real>::epsilon();
    this->asa_u0.swap(this->asa_u1);    // G(X{n-1}) stored
    this->asa_du0.swap(this->asa_du1);  // G(X{n-1}) -G(X{n-2})
    this->asa_dr0.swap(this->asa_dr1);  // Delta (X{n-1}) -Delta (X{n-2})
    this->asa_r0.swap(this->asa_r1);    // Delta(X{n-1})
    this->asa_r1 = -rx;  // rx = Xn - G(Xn) = -Delta Xn or rf = F(Xn)
    this->asa_u1 = u1;   // G(Xn)
    this->asa_du1 = this->asa_u1 - this->asa_u0;  // G(Xn) - G(X{n-1})
    this->asa_d2u =
        this->asa_du1 - this->asa_du0;  // G(Xn) - 2*G(X{n-1}) + G(X{n-2})
    this->asa_dr1 =
        this->asa_r1 - this->asa_r0;  // Delta (X{n}) -Delta (X{n-1})
    this->asa_d2r =
        this->asa_dr1 -
        this->asa_dr0;  // Delta (X{n}) - 2Delta (X{n-1}) + Delta (X{n-2})
    if (iter >= this->asat) {
      if (iter == 2) {
        // alternate secant
        const real nr2_dr = this->asa_dr1 | this->asa_dr1;
        if (nr2_dr > (asa_eps * asa_eps)) {
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            auto& log = mfront::getLogStream();
            log << "AlternateSecant acceleration convergence\n";
          }
          const real asa_a = (this->asa_dr1 | this->asa_r1) / nr2_dr;
          u1 -= asa_a * (this->asa_du1);
        }
      } else {
        const real nr2_d2r = this->asa_d2r | this->asa_d2r;
        if (nr2_d2r > (asa_eps * asa_eps)) {
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            auto& log = mfront::getLogStream();
            log << "AlternateDelta2 acceleration convergence\n";
          }
          const real asa_a = (this->asa_d2r | this->asa_r1) / nr2_d2r;
          u1 -= asa_a * (this->asa_d2u);
        }
      }
    }
  }  // end of AlternateDelta2AccelerationAlgorithm::execute

  void AlternateDelta2AccelerationAlgorithm::postExecuteTasks() {
  }  // end of AccelerationAlgorithm::postExecuteTaks

  AlternateDelta2AccelerationAlgorithm::
      ~AlternateDelta2AccelerationAlgorithm() = default;

}  // end of namespace mtest
