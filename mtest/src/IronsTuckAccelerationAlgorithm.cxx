/*!
 * \file  mfront/mtest/IronsTuckAccelerationAlgorithm.cxx
 * \brief
 * \author Helfer Thomas
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

#include "MFront/MFrontLogStream.hxx"
#include "MTest/IronsTuckAccelerationAlgorithm.hxx"

namespace mtest {

  IronsTuckAccelerationAlgorithm::IronsTuckAccelerationAlgorithm()
      : itat(-1) {
  }  // end of IronsTuckAccelerationAlgorithm::IronsTuckAccelerationAlgorithm

  std::string IronsTuckAccelerationAlgorithm::getName() const {
    return "Irons-Tuck";
  }

  void IronsTuckAccelerationAlgorithm::setParameter(const std::string& p,
                                                    const std::string& v) {
    using namespace std;
    const string m = "IronsTuckAccelerationAlgorithm::setParameter";
    if (p == "AccelerationTrigger") {
      const unsigned short i =
          AccelerationAlgorithm::convertToUnsignedShort(m, v);
      if (this->itat != -1) {
        string msg(
            "IronsTuckAccelerationAlgorithm::setParameter : "
            "the castem acceleration trigger has already "
            "been defined");
        throw(runtime_error(msg));
      }
      if (i < 2) {
        string msg("IronsTuckAccelerationAlgorithm::setParameter",
                   "invalid acceleration trigger value.");
        throw(runtime_error(msg));
      }
      this->itat = i;
    } else {
      string msg(
          "IronsTuckAccelerationAlgorithm::setParameter : "
          "invalid parameter '" +
          p + "'.");
      throw(runtime_error(msg));
    }
  }  // end of IronsTuckAccelerationAlgorithm::setParameter

  void IronsTuckAccelerationAlgorithm::initialize(const unsigned short psz) {
    this->ita_r0.resize(psz, 0.);
    this->ita_r1.resize(psz, 0.);
    this->ita_dr.resize(psz, 0.);
    if (this->itat == -1) {
      this->itat = 2;
    }
  }  // end of IronsTuckAccelerationAlgorithm::initialize

  void IronsTuckAccelerationAlgorithm::preExecuteTasks() {
  }  // end of IronsTuckAccelerationAlgorithm::preExecuteTasks

  void IronsTuckAccelerationAlgorithm::execute(
      tfel::math::vector<real>& u1,
      const tfel::math::vector<real>& rx,
      const tfel::math::vector<real>&,
      const real eeps,
      const real,
      const unsigned short iter) {
    using namespace std;
    const real it_eps = 100 * (eeps * numeric_limits<real>::epsilon());
    this->ita_r0.swap(this->ita_r1);
    this->ita_r1 = -rx;  // rx = Xn - G(Xn) = -Delta Xn or rf = F(Xn)
    if ((iter >= this->itat) && ((iter - this->itat) % 2 == 0)) {
      if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
        auto& log = mfront::getLogStream();
        log << "Irons Tuck acceleration convergence" << endl;
      }
      this->ita_dr = this->ita_r1 - this->ita_r0;
      const real nr2_dr = this->ita_dr | this->ita_dr;
      if (nr2_dr > (it_eps * it_eps)) {
        const real ita_a = (this->ita_r1 | this->ita_dr) / nr2_dr;
        u1 -= ita_a * this->ita_r1;
      }
    }
  }  // end of IronsTuckAccelerationAlgorithm::execute

  void IronsTuckAccelerationAlgorithm::postExecuteTasks() {
  }  // end of IronsTuckAccelerationAlgorithm::postExecuteTasks

  IronsTuckAccelerationAlgorithm::~IronsTuckAccelerationAlgorithm() {
  }  // end of AccelerationAlgorithm::~AccelerationAlgorithm

}  // end of namespace mtest
