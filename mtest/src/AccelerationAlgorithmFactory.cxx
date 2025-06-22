/*!
 * \file  mfront/mtest/AccelerationAlgorithmFactory.cxx
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

#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "MTest/AccelerationAlgorithm.hxx"
#include "MTest/CastemAccelerationAlgorithm.hxx"
#include "MTest/IronsTuckAccelerationAlgorithm.hxx"
#include "MTest/SteffensenAccelerationAlgorithm.hxx"
#include "MTest/SecantAccelerationAlgorithm.hxx"
#include "MTest/AlternateSecantAccelerationAlgorithm.hxx"
#include "MTest/AlternateDelta2AccelerationAlgorithm.hxx"
#include "MTest/Alternate2DeltaAccelerationAlgorithm.hxx"
#include "MTest/CrossedSecantAccelerationAlgorithm.hxx"
#include "MTest/CrossedDelta2AccelerationAlgorithm.hxx"
#include "MTest/Crossed2DeltaAccelerationAlgorithm.hxx"
#include "MTest/Crossed2DeltabisAccelerationAlgorithm.hxx"
#include "MTest/UAndersonAccelerationAlgorithm.hxx"
#include "MTest/FAndersonAccelerationAlgorithm.hxx"
#include "MTest/AccelerationAlgorithmFactory.hxx"

namespace mtest {

  template <typename T>
  static std::shared_ptr<AccelerationAlgorithm> buildAlgorithmConstructor() {
    return std::make_shared<T>();
  }  // end of buildAlgoritmConstructor

  AccelerationAlgorithmFactory&
  AccelerationAlgorithmFactory::getAccelerationAlgorithmFactory() {
    static AccelerationAlgorithmFactory factory;
    return factory;
  }

  std::shared_ptr<AccelerationAlgorithm>
  AccelerationAlgorithmFactory::getAlgorithm(const std::string& a) const {
    const auto p = this->constructors.find(a);
    tfel::raise_if(p == this->constructors.end(),
                   "AccelerationAlgorithmFactory::getAlgorithm : "
                   "no algorithm '" +
                       a + "' registred");
    return (*(p->second))();
  }

  AccelerationAlgorithmFactory::AccelerationAlgorithmFactory() {
    this->registerAlgorithm(
        "Cast3M", buildAlgorithmConstructor<CastemAccelerationAlgorithm>);
    this->registerAlgorithm(
        "Secant", buildAlgorithmConstructor<SecantAccelerationAlgorithm>);
    this->registerAlgorithm(
        "AlternateSecant",
        buildAlgorithmConstructor<AlternateSecantAccelerationAlgorithm>);
    this->registerAlgorithm(
        "AlternateDelta2",
        buildAlgorithmConstructor<AlternateDelta2AccelerationAlgorithm>);
    this->registerAlgorithm(
        "Alternate2Delta",
        buildAlgorithmConstructor<Alternate2DeltaAccelerationAlgorithm>);
    this->registerAlgorithm(
        "CrossedSecant",
        buildAlgorithmConstructor<CrossedSecantAccelerationAlgorithm>);
    this->registerAlgorithm(
        "CrossedDelta2",
        buildAlgorithmConstructor<CrossedDelta2AccelerationAlgorithm>);
    this->registerAlgorithm(
        "Crossed2Delta",
        buildAlgorithmConstructor<Crossed2DeltaAccelerationAlgorithm>);
    this->registerAlgorithm(
        "Crossed2Deltabis",
        buildAlgorithmConstructor<Crossed2DeltabisAccelerationAlgorithm>);
    this->registerAlgorithm(
        "Steffensen",
        buildAlgorithmConstructor<SteffensenAccelerationAlgorithm>);
    this->registerAlgorithm(
        "IronsTuck", buildAlgorithmConstructor<IronsTuckAccelerationAlgorithm>);
    this->registerAlgorithm(
        "UAnderson", buildAlgorithmConstructor<UAndersonAccelerationAlgorithm>);
    this->registerAlgorithm(
        "FAnderson", buildAlgorithmConstructor<FAndersonAccelerationAlgorithm>);
  }  // end of AccelerationAlgorithmFactory::AccelerationAlgorithmFactory

  void AccelerationAlgorithmFactory::registerAlgorithm(const std::string& a,
                                                       const constructor c) {
    tfel::raise_if(!this->constructors.insert({a, c}).second,
                   "AccelerationAlgorithmFactory::registerAlgorithm: "
                   "algorithm '" +
                       a + "' already declared");
  }  // end of AccelerationAlgorithmFactory::registerAlgorithm

}  // end of namespace mtest
