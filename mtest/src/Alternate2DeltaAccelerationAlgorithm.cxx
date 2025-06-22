/*!
 * \file  mfront/mtest/Alternate2DeltaAccelerationAlgorithm.cxx
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
#include "MTest/Alternate2DeltaAccelerationAlgorithm.hxx"

namespace mtest {

  Alternate2DeltaAccelerationAlgorithm::Alternate2DeltaAccelerationAlgorithm()
      : asat(-1) {
  }  // end of
     // Alternate2DeltaAccelerationAlgorithm::Alternate2DeltaAccelerationAlgorithm

  std::string Alternate2DeltaAccelerationAlgorithm::getName() const {
    return "Alternate2Delta";
  }

  void Alternate2DeltaAccelerationAlgorithm::setParameter(
      const std::string& p, const std::string& v) {
    const std::string m = "Alternate2DeltaAccelerationAlgorithm::setParameter";
    auto throw_if = [&m](const bool c, const std::string& msg) {
      tfel::raise_if(c, m + ": " + msg);
    };
    if (p == "AccelerationTrigger") {
      const auto i = AccelerationAlgorithm::convertToUnsignedShort(m, v);
      throw_if(this->asat != -1,
               "the acceleration trigger "
               "has already been defined");
      throw_if(i < 2, "invalid acceleration trigger value");
      this->asat = i;
    } else {
      throw_if(true, "invalid parameter '" + p + "'");
    }
  }  // end of Alternate2DeltaAccelerationAlgorithm::setParameter

  void Alternate2DeltaAccelerationAlgorithm::initialize(
      const unsigned short psz) {
    this->asa_u0.resize(psz, 0.);
    this->asa_u1.resize(psz, 0.);
    this->asa_du0.resize(psz, 0.);
    this->asa_du1.resize(psz, 0.);
    this->asa_r0.resize(psz, 0.);
    this->asa_r1.resize(psz, 0.);
    this->asa_dr0.resize(psz, 0.);
    this->asa_dr1.resize(psz, 0.);
    if (this->asat == -1) {
      this->asat = 2;
    }
  }  // end of Alternate2DeltaAccelerationAlgorithm::initialize

  void Alternate2DeltaAccelerationAlgorithm::preExecuteTasks() {
  }  // end of AccelerationAlgorithm::preExecuteTaks

  void Alternate2DeltaAccelerationAlgorithm::execute(
      tfel::math::vector<real>& u1,
      const tfel::math::vector<real>& rx,
      const tfel::math::vector<real>&,
      const real eeps,
      const real,
      const unsigned short iter) {
    using namespace std;
    const real asa_eps = 100 * (eeps * numeric_limits<real>::epsilon());
    this->asa_u0.swap(this->asa_u1);    // G(X{n-1}) stored
    this->asa_r0.swap(this->asa_r1);    // Delta(X{n-1})
    this->asa_du0.swap(this->asa_du1);  // G(X{n-1}) -G(X{n-2})
    this->asa_dr0.swap(this->asa_dr1);  // Delta (X{n-1}) -Delta (X{n-2})
    this->asa_r1 = -rx;  // rx = Xn - G(Xn) = -Delta Xn or rf = F(Xn)
    this->asa_u1 = u1;   // G(Xn)
    this->asa_du1 = this->asa_u1 - this->asa_u0;  // G(Xn) - G(X{n-1})
    this->asa_dr1 =
        this->asa_r1 - this->asa_r0;  // Delta (X{n}) -Delta (X{n-1})
    if (iter >= this->asat) {
      if (iter == 2) {
        // alternate secant
        const real nr2_dr1 = this->asa_dr1 | this->asa_dr1;
        if (nr2_dr1 > (asa_eps * asa_eps)) {
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            ostream& log = mfront::getLogStream();
            log << "Alternate2Delta acceleration convergence" << endl;
          }
          const real asa_a = (this->asa_dr1 | this->asa_r1) / nr2_dr1;
          u1 -= asa_a * (this->asa_du1);
        }
      } else {
        const real nr2_dr1 = this->asa_dr1 | this->asa_dr1;
        const real nr2_dr0 = this->asa_dr0 | this->asa_dr0;
        const real dr1_dr0 = this->asa_dr1 | this->asa_dr0;
        const real det = nr2_dr1 * nr2_dr0 - dr1_dr0 * dr1_dr0;
        if ((dr1_dr0 * dr1_dr0) / (nr2_dr1 * nr2_dr0) < 0.99) {
          // if(abs(det/(nr2_dr0*b1))>(1e-3)){
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            auto& log = mfront::getLogStream();
            log << "Alternate2Delta acceleration convergence" << endl;
          }
          const real b1 = this->asa_dr1 | this->asa_r1;
          const real b2 = this->asa_dr0 | this->asa_r1;
          real l1 = nr2_dr0 * b1 - dr1_dr0 * b2;
          l1 /= det;
          real l2 = nr2_dr1 * b2 - dr1_dr0 * b1;
          l2 /= det;
          u1 -= l1 * (this->asa_du1) + l2 * (this->asa_du0);
        } else {
          if (nr2_dr1 > (asa_eps * asa_eps)) {
            if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
              auto& log = mfront::getLogStream();
              log << "Alternate2Delta acceleration convergence" << endl;
            }
            const real asa_a = (this->asa_dr1 | this->asa_r1) / nr2_dr1;
            u1 -= asa_a * (this->asa_du1);
          }
        }
      }
    }
  }  // end of Alternate2DeltaAccelerationAlgorithm::execute

  void Alternate2DeltaAccelerationAlgorithm::postExecuteTasks() {
  }  // end of AccelerationAlgorithm::postExecuteTaks

  Alternate2DeltaAccelerationAlgorithm::
      ~Alternate2DeltaAccelerationAlgorithm() = default;

}  // end of namespace mtest
