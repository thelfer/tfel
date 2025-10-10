/*!
 * \file  mtest/src/Crossed2DeltabisAccelerationAlgorithm.cxx
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
#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/Crossed2DeltabisAccelerationAlgorithm.hxx"

namespace mtest {

  Crossed2DeltabisAccelerationAlgorithm::Crossed2DeltabisAccelerationAlgorithm()
      : csat(-1) {
  }  // end of
     // Crossed2DeltabisAccelerationAlgorithm::Crossed2DeltabisAccelerationAlgorithm

  std::string Crossed2DeltabisAccelerationAlgorithm::getName() const {
    return "crossed secant";
  }

  void Crossed2DeltabisAccelerationAlgorithm::setParameter(
      const std::string& p, const std::string& v) {
    const std::string m = "Crossed2DeltabisAccelerationAlgorithm::setParameter";
    if (p == "AccelerationTrigger") {
      const auto i = AccelerationAlgorithm::convertToUnsignedShort(m, v);
      tfel::raise_if(this->csat != -1, m + ": the acceleration trigger "
                                           "has already been defined");
      tfel::raise_if(i < 2, m + ": invalid acceleration trigger value.");
      this->csat = i;
    } else {
      tfel::raise(m + ": invalid parameter '" + p + "'.");
    }
  }  // end of Crossed2DeltabisAccelerationAlgorithm::setParameter

  void Crossed2DeltabisAccelerationAlgorithm::initialize(
      const unsigned short psz) {
    this->csa_u0.resize(psz, 0.);
    this->csa_u1.resize(psz, 0.);
    this->csa_du.resize(psz, 0.);
    this->csa_x0.resize(psz, 0.);
    this->csa_x1.resize(psz, 0.);
    this->csa_x2.resize(psz, 0.);
    this->csa_dx0.resize(psz, 0.);
    this->csa_dx1.resize(psz, 0.);
    this->csa_dx2.resize(psz, 0.);
    this->csa_dudx.resize(psz, 0.);
    this->csa_r0.resize(psz, 0.);
    this->csa_r1.resize(psz, 0.);
    this->csa_dr.resize(psz, 0.);
    if (this->csat == -1) {
      this->csat = 2;
    }
  }  // end of Crossed2DeltabisAccelerationAlgorithm::initialize

  void Crossed2DeltabisAccelerationAlgorithm::preExecuteTasks() {
  }  // end of AccelerationAlgorithm::preExecuteTaks

  void Crossed2DeltabisAccelerationAlgorithm::execute(
      tfel::math::vector<real>& u1,
      const tfel::math::vector<real>& rx,
      const tfel::math::vector<real>&,
      const real eeps,
      const real,
      const unsigned short iter) {
    const real csa_eps = 100 * eeps * std::numeric_limits<real>::epsilon();
    this->csa_u0.swap(this->csa_u1);    // previous G(Xn) stored => G(X{n-1})
    this->csa_x0.swap(this->csa_x1);    // X{n-1}
    this->csa_x1.swap(this->csa_x2);    // X{n}
    this->csa_dx0.swap(this->csa_dx1);  // X{n-1} - X{n-2}
    this->csa_dx1.swap(this->csa_dx2);  // X{n} - X{n-1}
    this->csa_r0.swap(this->csa_r1);    // Delta X{n-1}
    this->csa_r1 = -rx;  // rx =  Xn - G(Xn) = -Delta Xn or rf = F(Xn)
    this->csa_u1 = u1;   // G(Xn)
    this->csa_du = this->csa_u1 - this->csa_u0;  // G(Xn) - G(X{n-1})
    this->csa_dr = this->csa_r1 - this->csa_r0;  // Delta X{n} - Delta X{n-1}
    this->csa_dudx = this->csa_du - this->csa_dx0;
    if (iter >= this->csat) {
      if (iter == 2) {
        // crossed secant acceleration
        const real nr2_dr = this->csa_dr | this->csa_dr;
        if (nr2_dr > (csa_eps * csa_eps)) {
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            auto& log = mfront::getLogStream();
            log << "CrossedSecant acceleration convergence\n";
          }
          const real csa_a = (this->csa_du | this->csa_dr) / nr2_dr;
          u1 -= csa_a * (this->csa_r1);
        }
      } else {
        const real ma = this->csa_dr | this->csa_dr;
        const real mc = this->csa_dudx | this->csa_dudx;
        const real mb = this->csa_dr | this->csa_dudx;
        const real det = ma * mc - mb * mb;
        if ((mb * mb) / (ma * mc) < 0.99) {
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            auto& log = mfront::getLogStream();
            log << "Crossed2Deltabis acceleration convergence\n";
          }
          const real b1 = this->csa_dr | this->csa_du;
          const real b2 = this->csa_dudx | this->csa_du;
          real l1 = mc * b1 - mb * b2;
          l1 /= det;
          real l2 = ma * b2 - mb * b1;
          l2 /= det;
          u1 -= l1 * (this->csa_r1) + l2 * (this->csa_u1 - this->csa_x0);
        } else {
          const real nr2_dr = ma;
          if (nr2_dr > (csa_eps * csa_eps)) {
            if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
              auto& log = mfront::getLogStream();
              log << "CrossedSecant acceleration convergence\n";
            }
            const real csa_a = (this->csa_du | this->csa_dr) / nr2_dr;
            u1 -= csa_a * (this->csa_r1);
          }
        }
      }
    }
    this->csa_x2 = u1;                            // X{n+1}
    this->csa_dx2 = this->csa_x2 - this->csa_x1;  // X{n+1} -Xn
  }  // end of Crossed2DeltabisAccelerationAlgorithm::execute

  void Crossed2DeltabisAccelerationAlgorithm::postExecuteTasks() {
  }  // end of AccelerationAlgorithm::postExecuteTaks

  Crossed2DeltabisAccelerationAlgorithm::
      ~Crossed2DeltabisAccelerationAlgorithm() = default;

}  // end of namespace mtest
