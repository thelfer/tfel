/*!
 * \file  mfront/mtest/UAndersonAccelerationAlgorithm.cxx
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
#include <iterator>
#include <stdexcept>

#include "MFront/MFrontLogStream.hxx"
#include "MTest/UAndersonAccelerationAlgorithm.hxx"

namespace mtest {

  UAndersonAccelerationAlgorithm::UAndersonAccelerationAlgorithm() = default;

  std::string UAndersonAccelerationAlgorithm::getName() const {
    return "UAnderson";
  }  // end of UAndersonAccelerationAlgorithm::getName

  void UAndersonAccelerationAlgorithm::setParameter(const std::string& p,
                                                    const std::string& v) {
    const std::string m = "UAndersonAccelerationAlgorithm::setParameter";
    auto throw_if = [&m](const bool c, const std::string& msg) {
      if (c) {
        throw(std::runtime_error(m + ": " + msg));
      }
    };
    if (p == "MethodOrder") {
      throw_if(this->Nmax != -1, "the method order has already been defined");
      const auto i = AccelerationAlgorithm::convertToUnsignedShort(m, v);
      throw_if(i == 0, "invalid method order value.");
      this->Nmax = i;
    } else if (p == "AccelerationPeriod") {
      throw_if(this->alMax != -1,
               "the acceleration period has already been defined");
      const auto i = AccelerationAlgorithm::convertToUnsignedShort(m, v);
      throw_if(i == 0, "invalid acceleration period value.");
      this->alMax = i;
    } else {
      throw_if(true, "invalid parameter '" + p + "'.");
    }
  }  // end of UAndersonAccelerationAlgorithm::setParameter

  void UAndersonAccelerationAlgorithm::initialize(const unsigned short psz) {
    this->a = std::unique_ptr<UAnderson>(
        new UAnderson([psz]() { return new tfel::math::vector<real>(psz); }));
    if (this->Nmax == -1) {
      this->Nmax = 4;
    }
    if (this->alMax == -1) {
      this->alMax = 2;
    }
    if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
      mfront::getLogStream()
          << "UAndersonAccelerationAlgorithm::initialize: " << this->Nmax << " "
          << this->alMax << '\n';
    }
    this->a->setAnderson(this->Nmax, this->alMax);
  }  // end of UAndersonAccelerationAlgorithm::initialize

  void UAndersonAccelerationAlgorithm::preExecuteTasks() {
    this->a->restart(this->uO, this->uN);
  }  // end of UAndersonAccelerationAlgorithm::preExecuteTasks

  void UAndersonAccelerationAlgorithm::execute(
      tfel::math::vector<real>& u1,
      const tfel::math::vector<real>& du,
      const tfel::math::vector<real>&,
      const real,
      const real,
      const unsigned short iter) {
    if (iter == 1u) {
      *(this->uO) = u1 - du;
    }
    *(this->uN) = u1;
    this->a->newIter(this->uO, this->uN);
    u1 = *(this->uO);
  }  // end of UAndersonAccelerationAlgorithm::execute

  void UAndersonAccelerationAlgorithm::postExecuteTasks() {
  }  // end of UAndersonAccelerationAlgorithm::postExecuteTasks

  UAndersonAccelerationAlgorithm::~UAndersonAccelerationAlgorithm() = default;

}  // end of namespace mtest
