/*!
 * \file   mfront/mtest/EuroplexusFiniteStrainBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <algorithm>

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/T2toST2/T2toST2ConceptIO.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/Europlexus/Europlexus.hxx"
#include "MFront/Europlexus/EuroplexusComputeStiffnessTensor.hxx"

#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/EuroplexusFiniteStrainBehaviour.hxx"

namespace mtest {

  EuroplexusFiniteStrainBehaviour::EuroplexusFiniteStrainBehaviour(
      const Hypothesis h, const std::string& l, const std::string& b)
      : EuroplexusStandardBehaviour(h, l, b) {}

  void EuroplexusFiniteStrainBehaviour::getDrivingVariablesDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    std::fill(v.begin(), v.end(), real(0));
    v[0] = v[1] = v[2] = real(1);
  }

  void EuroplexusFiniteStrainBehaviour::allocate(BehaviourWorkSpace& wk) const {
    EuroplexusStandardBehaviour::allocate(wk);
    wk.D.resize(6u, 9u);
  }

  std::pair<bool, real> EuroplexusFiniteStrainBehaviour::call_behaviour(
      tfel::math::matrix<real>& Kt,
      CurrentState& s,
      BehaviourWorkSpace&,
      const real dt,
      const StiffnessMatrixType ktype,
      const bool b) const {
    const auto nprops = static_cast<epx::EuroplexusInt>(s.mprops1.size());
    const auto nstatv = static_cast<epx::EuroplexusInt>(s.iv0.size());
    const auto npredef = static_cast<epx::EuroplexusInt>(s.esv0.size()) - 1;
    const auto h = this->getHypothesis();
    const epx::EuroplexusInt hv = [&h]() {
      if (h == ModellingHypothesis::AXISYMMETRICAL) {
        return 3;
      } else if (h == ModellingHypothesis::PLANESTRESS) {
        return 2;
      } else if (h == ModellingHypothesis::PLANESTRAIN) {
        return 1;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 0;
      } else {
        throw(std::runtime_error(
            "EuroplexusFiniteStrainBehaviour::call_behaviour: "
            "unsupported hypothesis"));
      }
    }();
    UmatBehaviourBase::initializeTangentOperator(Kt, ktype, b);
    tfel::math::tensor<3u, real> uu0(real(0));
    tfel::math::tensor<3u, real> uu1(real(0));
    tfel::math::stensor<3u, real> s1(real(0));
    std::copy(s.e0.begin(), s.e0.end(), uu0.begin());
    std::copy(s.e1.begin(), s.e1.end(), uu1.begin());
    std::copy(s.s0.begin(), s.s0.end(), s1.begin());
    std::copy(s.iv0.begin(), s.iv0.end(), s.iv1.begin());
    epx::EuroplexusReal ndt(1.);
    epx::EuroplexusInt status(0);
    epx::EuroplexusInt broken = 0;
    char msg[256];
    (this->fct)(&status, &(s1(0)), nstatv == 0 ? nullptr : &(s.iv1(0)),
                &Kt(0, 0), &ndt, &broken, msg, &nstatv, &hv, &dt, &uu0(0),
                &uu1(0), &(s.r(0, 0)), nprops == 0 ? nullptr : &(s.mprops1(0)),
                &nprops, &(s.esv0(0)), &(s.desv(0)),
                npredef == 0 ? nullptr : &(s.esv0(1)),
                npredef == 0 ? nullptr : &(s.desv(1)), &npredef);
    if (status != 0) {
      return {false, ndt};
    }
    std::copy_n(s1.begin(), s.s1.size(), s.s1.begin());
    return {true, ndt};
  }

  EuroplexusFiniteStrainBehaviour::~EuroplexusFiniteStrainBehaviour() {}

}  // end of namespace mtest
