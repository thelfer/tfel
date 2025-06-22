/*!
 * \file  mfront/mtest/CrossedSecantAccelerationAlgorithm.cxx
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
#include "MTest/CrossedSecantAccelerationAlgorithm.hxx"

namespace mtest {

  CrossedSecantAccelerationAlgorithm::CrossedSecantAccelerationAlgorithm()
      : csat(-1) {
  }  // end of
     // CrossedSecantAccelerationAlgorithm::CrossedSecantAccelerationAlgorithm

  std::string CrossedSecantAccelerationAlgorithm::getName() const {
    return "crossed secant";
  }

  void CrossedSecantAccelerationAlgorithm::setParameter(const std::string& p,
                                                        const std::string& v) {
    using namespace std;
    const string m = "CrossedSecantAccelerationAlgorithm::setParameter";
    if (p == "AccelerationTrigger") {
      const unsigned short i =
          AccelerationAlgorithm::convertToUnsignedShort(m, v);
      if (this->csat != -1) {
        string msg(
            "CrossedSecantAccelerationAlgorithm::setParameter : "
            "the castem acceleration trigger has already "
            "been defined");
        throw(runtime_error(msg));
      }
      if (i < 2) {
        string msg("CrossedSecantAccelerationAlgorithm::setParameter",
                   "invalid acceleration trigger value.");
        throw(runtime_error(msg));
      }
      this->csat = i;
    } else {
      string msg(
          "CrossedSecantAccelerationAlgorithm::setParameter : "
          "invalid parameter '" +
          p + "'.");
      throw(runtime_error(msg));
    }
  }  // end of CrossedSecantAccelerationAlgorithm::setParameter

  void CrossedSecantAccelerationAlgorithm::initialize(
      const unsigned short psz) {
    this->csa_u0.resize(psz, 0.);
    this->csa_u1.resize(psz, 0.);
    this->csa_du.resize(psz, 0.);
    this->csa_r0.resize(psz, 0.);
    this->csa_r1.resize(psz, 0.);
    this->csa_dr.resize(psz, 0.);
    if (this->csat == -1) {
      this->csat = 2;
    }
  }  // end of CrossedSecantAccelerationAlgorithm::initialize

  void CrossedSecantAccelerationAlgorithm::preExecuteTasks() {
  }  // end of AccelerationAlgorithm::preExecuteTaks

  void CrossedSecantAccelerationAlgorithm::execute(
      tfel::math::vector<real>& u1,
      const tfel::math::vector<real>& rx,
      const tfel::math::vector<real>&,
      const real eeps,
      const real,
      const unsigned short iter) {
    using namespace std;
    const real csa_eps = 100 * (eeps * numeric_limits<real>::epsilon());
    this->csa_u0.swap(this->csa_u1);  // previous G(u0) stored
    this->csa_r0.swap(this->csa_r1);
    this->csa_r1 = -rx;  // rx = u0 - G(u0) or rf = F(u0)
    this->csa_u1 = u1;   // G(u0)
    this->csa_du = this->csa_u1 - this->csa_u0;
    this->csa_dr = this->csa_r1 - this->csa_r0;
    if (iter >= this->csat) {
      const real nr2_dr = this->csa_dr | this->csa_dr;
      if (nr2_dr > (csa_eps * csa_eps)) {
        if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
          ostream& log = mfront::getLogStream();
          log << "CrossedSecant acceleration convergence" << endl;
        }
        const real csa_a = (this->csa_du | this->csa_dr) / nr2_dr;
        u1 -= csa_a * (this->csa_r1);
      }
    }

  }  // end of CrossedSecantAccelerationAlgorithm::execute

  void CrossedSecantAccelerationAlgorithm::postExecuteTasks() {
  }  // end of AccelerationAlgorithm::postExecuteTaks

  CrossedSecantAccelerationAlgorithm::~CrossedSecantAccelerationAlgorithm() {
  }  // end of AccelerationAlgorithm::~AccelerationAlgorithm

}  // end of namespace mtest
