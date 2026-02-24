/*!
 * \file  mtest/src/SteffensenAccelerationAlgorithm.cxx
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
#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/SteffensenAccelerationAlgorithm.hxx"

namespace mtest {

  SteffensenAccelerationAlgorithm::SteffensenAccelerationAlgorithm()
      : stat(-1) {
  }  // end of SteffensenAccelerationAlgorithm::SteffensenAccelerationAlgorithm

  std::string SteffensenAccelerationAlgorithm::getName() const {
    return "Steffensen";
  }  // end of SteffensenAccelerationAlgorithm::getName

  void SteffensenAccelerationAlgorithm::setParameter(const std::string& p,
                                                     const std::string& v) {
    const std::string m = "SteffensenAccelerationAlgorithm::setParameter";
    if (p == "AccelerationTrigger") {
      const auto i = AccelerationAlgorithm::convertToUnsignedShort(m, v);
      tfel::raise_if(this->stat != -1, m + ": the acceleration trigger "
                                           "has already been defined");
      tfel::raise_if(i < 3, m + ": invalid acceleration trigger value.");
      this->stat = i;
    } else {
      tfel::raise(m + ": invalid parameter '" + p + "'.");
    }
  }  // end of SteffensenAccelerationAlgorithm::setParameter

  void SteffensenAccelerationAlgorithm::initialize(const unsigned short psz) {
    this->sta_u0.resize(psz, 0.);
    this->sta_u1.resize(psz, 0.);
    this->sta_u2.resize(psz, 0.);
    this->sta_du1.resize(psz, 0.);
    this->sta_du2.resize(psz, 0.);
    if (this->stat == -1) {
      this->stat = 3;
    }
  }  // end of SteffensenAccelerationAlgorithm::initialize

  void SteffensenAccelerationAlgorithm::preExecuteTasks() {
  }  // end of SteffensenAccelerationAlgorithm::preExecuteTasks

  void SteffensenAccelerationAlgorithm::execute(tfel::math::vector<real>& u1,
                                                const tfel::math::vector<real>&,
                                                const tfel::math::vector<real>&,
                                                const real eeps,
                                                const real,
                                                const unsigned short iter) {
    const real it_eps = 100 * eeps * std::numeric_limits<real>::epsilon();
    this->sta_u0.swap(this->sta_u1);
    this->sta_u1.swap(this->sta_u2);
    this->sta_u2 = u1;
    if ((iter >= this->stat) && ((iter - this->stat) % 2 == 0)) {
      if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
        auto& log = mfront::getLogStream();
        log << "Steffensen acceleration convergence\n";
      }
      this->sta_du2 = this->sta_u2 - this->sta_u1;
      this->sta_du1 = this->sta_u1 - this->sta_u0;
      for (std::vector<real>::size_type id = 0; id != this->sta_du1.size();
           ++id) {
        if ((std::abs(this->sta_du2[id]) > it_eps) &&
            (std::abs(this->sta_du1[id]) > it_eps)) {
          const auto i1 = 1 / (this->sta_du2[id]);
          const auto i2 = 1 / (this->sta_du1[id]);
          if (std::abs(i1 - i2) > it_eps) {
            u1[id] = this->sta_u1[id] + 1 / (i1 - i2);
          }
        }
      }
    }
  }  // end of SteffensenAccelerationAlgorithm::execute

  void SteffensenAccelerationAlgorithm::postExecuteTasks() {
  }  // end of SteffensenAccelerationAlgorithm::postExecuteTasks

  SteffensenAccelerationAlgorithm::~SteffensenAccelerationAlgorithm() = default;

}  // end of namespace mtest
