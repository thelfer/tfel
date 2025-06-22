/*!
 * \file  mfront/mtest/CrossedDelta2AccelerationAlgorithm.cxx
 * \brief
 * \author Ramière Isabelle
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

#include "MFront/MFrontLogStream.hxx"
#include "MTest/CrossedDelta2AccelerationAlgorithm.hxx"

namespace mtest {

  CrossedDelta2AccelerationAlgorithm::CrossedDelta2AccelerationAlgorithm()
      : csat(-1) {
  }  // end of
     // CrossedDelta2AccelerationAlgorithm::CrossedDelta2AccelerationAlgorithm

  std::string CrossedDelta2AccelerationAlgorithm::getName() const {
    return "crossed secant";
  }

  void CrossedDelta2AccelerationAlgorithm::setParameter(const std::string& p,
                                                        const std::string& v) {
    using namespace std;
    const string m = "CrossedDelta2AccelerationAlgorithm::setParameter";
    if (p == "AccelerationTrigger") {
      const unsigned short i =
          AccelerationAlgorithm::convertToUnsignedShort(m, v);
      if (this->csat != -1) {
        string msg(
            "CrossedDelta2AccelerationAlgorithm::setParameter : "
            "the castem acceleration trigger has already "
            "been defined");
        throw(runtime_error(msg));
      }
      if (i < 2) {
        string msg("CrossedDelta2AccelerationAlgorithm::setParameter",
                   "invalid acceleration trigger value.");
        throw(runtime_error(msg));
      }
      this->csat = i;
    } else {
      string msg(
          "CrossedDelta2AccelerationAlgorithm::setParameter : "
          "invalid parameter '" +
          p + "'.");
      throw(runtime_error(msg));
    }
  }  // end of CrossedDelta2AccelerationAlgorithm::setParameter

  void CrossedDelta2AccelerationAlgorithm::initialize(
      const unsigned short psz) {
    this->csa_u0.resize(psz, 0.);
    this->csa_u1.resize(psz, 0.);
    this->csa_du.resize(psz, 0.);
    this->csa_r0.resize(psz, 0.);
    this->csa_r1.resize(psz, 0.);
    this->csa_dr0.resize(psz, 0.);
    this->csa_dr1.resize(psz, 0.);
    this->csa_d2r.resize(psz, 0.);
    if (this->csat == -1) {
      this->csat = 3;
    }
  }  // end of CrossedDelta2AccelerationAlgorithm::initialize

  void CrossedDelta2AccelerationAlgorithm::preExecuteTasks() {
  }  // end of AccelerationAlgorithm::preExecuteTaks

  void CrossedDelta2AccelerationAlgorithm::execute(
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
    this->csa_d2r =
        this->csa_dr1 -
        this->csa_dr0;  // Delta X{n} - 2*Delta X{n-1} + Delta X{n-2}
    if (iter >= this->csat) {
      if (iter == 2) {
        // crossed secant acceleration
        const real nr2_dr = this->csa_dr1 | this->csa_dr1;
        if (nr2_dr > (csa_eps * csa_eps)) {
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            ostream& log = mfront::getLogStream();
            log << "CrossedSecant acceleration convergence" << endl;
          }
          const real csa_a = (this->csa_du | this->csa_dr1) / nr2_dr;
          u1 -= csa_a * (this->csa_r1);
        }
      } else {
        const real nr2_d2r = this->csa_d2r | this->csa_d2r;
        if (nr2_d2r > (csa_eps * csa_eps)) {
          if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
            ostream& log = mfront::getLogStream();
            log << "CrossedDelta2 acceleration convergence" << endl;
          }
          const real csa_a = (this->csa_du | this->csa_d2r) / nr2_d2r;
          u1 -= csa_a * (this->csa_dr1);
        }
      }
    }
  }  // end of CrossedDelta2AccelerationAlgorithm::execute

  void CrossedDelta2AccelerationAlgorithm::postExecuteTasks() {
  }  // end of AccelerationAlgorithm::postExecuteTaks

  CrossedDelta2AccelerationAlgorithm::~CrossedDelta2AccelerationAlgorithm() {
  }  // end of AccelerationAlgorithm::~AccelerationAlgorithm

}  // end of namespace mtest
