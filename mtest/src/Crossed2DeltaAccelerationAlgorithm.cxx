/*!
 * \file  mfront/mtest/Crossed2DeltaAccelerationAlgorithm.cxx
 * \brief
 * \author Ramière Isabelle
 * \brief 09 mars 2015
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

#include "MFront/MFrontLogStream.hxx"
#include "MTest/Crossed2DeltaAccelerationAlgorithm.hxx"

namespace mtest {

  Crossed2DeltaAccelerationAlgorithm::Crossed2DeltaAccelerationAlgorithm()
      : csat(-1) {
  }  // end of
     // Crossed2DeltaAccelerationAlgorithm::Crossed2DeltaAccelerationAlgorithm

  std::string Crossed2DeltaAccelerationAlgorithm::getName() const {
    return "crossed secant";
  }

  void Crossed2DeltaAccelerationAlgorithm::setParameter(const std::string& p,
                                                        const std::string& v) {
    auto throw_if = [](const bool c, const std::string& m) {
      if (c) {
        throw(
            std::runtime_error("Crossed2DeltaAccelerationAlgorithm::"
                               "setParameter: " +
                               m));
      }
    };
    if (p == "AccelerationTrigger") {
      const auto m = "Crossed2DeltaAccelerationAlgorithm::setParameter";
      throw_if(this->csat != -1,
               "the acceleration trigger has already "
               "been defined");
      const auto i = AccelerationAlgorithm::convertToUnsignedShort(m, v);
      throw_if(i < 2, "invalid acceleration trigger value.");
      this->csat = i;
    } else {
      throw_if(true, "invalid parameter '" + p + "'.");
    }
  }  // end of Crossed2DeltaAccelerationAlgorithm::setParameter

  void Crossed2DeltaAccelerationAlgorithm::initialize(
      const unsigned short psz) {
    this->csa_u0.resize(psz, 0.);
    this->csa_u1.resize(psz, 0.);
    this->csa_du.resize(psz, 0.);
    this->csa_r0.resize(psz, 0.);
    this->csa_r1.resize(psz, 0.);
    this->csa_dr0.resize(psz, 0.);
    this->csa_dr1.resize(psz, 0.);
    if (this->csat == -1) {
      this->csat = 2;
    }
  }  // end of Crossed2DeltaAccelerationAlgorithm::initialize

  void Crossed2DeltaAccelerationAlgorithm::preExecuteTasks() {
  }  // end of AccelerationAlgorithm::preExecuteTaks

  void Crossed2DeltaAccelerationAlgorithm::execute(
      tfel::math::vector<real>& u1,
      const tfel::math::vector<real>& rx,
      const tfel::math::vector<real>&,
      const real eeps,
      const real,
      const unsigned short iter) {
    using namespace std;
    const real csa_eps = 100 * (eeps * numeric_limits<real>::epsilon());
    this->csa_u0.swap(this->csa_u1);    // previous G(Xn) stored => G(X{n-1})
    this->csa_r0.swap(this->csa_r1);    // Delta X{n-1}
    this->csa_dr0.swap(this->csa_dr1);  // Delta X{n-1} - Delta X{n-2}
    this->csa_r1 = -rx;  // rx =  Xn - G(Xn) = -Delta Xn or rf = F(Xn)
    this->csa_u1 = u1;   // G(Xn)
    this->csa_du = this->csa_u1 - this->csa_u0;   // G(Xn) - G(X{n-1})
    this->csa_dr1 = this->csa_r1 - this->csa_r0;  // Delta X{n} - Delta X{n-1}
    if (iter >= this->csat) {
      if (iter == 2) {
        // crossed secant acceleration
        const real nr2_dr1 = this->csa_dr1 | this->csa_dr1;
        if (nr2_dr1 > (csa_eps * csa_eps)) {
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            ostream& log = mfront::getLogStream();
            log << "CrossedSecant acceleration convergence" << endl;
          }
          const real csa_a = (this->csa_du | this->csa_dr1) / nr2_dr1;
          u1 -= csa_a * (this->csa_r1);
        }
      } else {
        const real nr2_dr1 = this->csa_dr1 | this->csa_dr1;
        const real nr2_dr0 = this->csa_dr0 | this->csa_dr0;
        const real dr1_dr0 = this->csa_dr1 | this->csa_dr0;
        const real det = nr2_dr1 * nr2_dr0 - dr1_dr0 * dr1_dr0;
        if ((dr1_dr0 * dr1_dr0) / (nr2_dr1 * nr2_dr0) < 0.99) {
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            ostream& log = mfront::getLogStream();
            log << "Crossed2Delta acceleration convergence" << endl;
          }
          const real b1 = this->csa_dr1 | this->csa_du;
          const real b2 = this->csa_dr0 | this->csa_du;
          real l1 = nr2_dr0 * b1 - dr1_dr0 * b2;
          l1 /= det;
          real l2 = nr2_dr1 * b2 - dr1_dr0 * b1;
          l2 /= det;
          u1 -= l1 * (this->csa_r1) + l2 * (this->csa_r0);
        } else if (nr2_dr1 > (csa_eps * csa_eps)) {
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            auto& log = mfront::getLogStream();
            log << "CrossedSecant acceleration convergence" << endl;
          }
          const real csa_a = (this->csa_du | this->csa_dr1) / nr2_dr1;
          u1 -= csa_a * (this->csa_r1);
        }
      }
    }
  }  // end of Crossed2DeltaAccelerationAlgorithm::execute

  void Crossed2DeltaAccelerationAlgorithm::postExecuteTasks() {
  }  // end of AccelerationAlgorithm::postExecuteTaks

  Crossed2DeltaAccelerationAlgorithm::~Crossed2DeltaAccelerationAlgorithm() =
      default;

}  // end of namespace mtest
