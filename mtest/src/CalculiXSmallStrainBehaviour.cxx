/*!
 * \file   mtest/src/CalculiXSmallStrainBehaviour.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  03/08/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <limits>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "MFront/CalculiX/CalculiX.hxx"
#include "MFront/CalculiX/CalculiXComputeStiffnessTensor.hxx"

#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/UmatNormaliseTangentOperator.hxx"
#include "MTest/CalculiXSmallStrainBehaviour.hxx"

namespace mtest {

  CalculiXSmallStrainBehaviour::CalculiXSmallStrainBehaviour(
      const Hypothesis h, const std::string& l, const std::string& b)
      : CalculiXStandardBehaviour(h, l, b) {}

  void CalculiXSmallStrainBehaviour::getGradientsDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    std::fill(v.begin(), v.end(), real(0));
  }

  std::pair<bool, real> CalculiXSmallStrainBehaviour::call_behaviour(
      tfel::math::matrix<real>& Kt,
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype,
      const bool b) const {
    using namespace tfel::math;
    using namespace calculix;
    using tfel::math::vector;
    constexpr auto sqrt2 = Cste<real>::sqrt2;
    const auto h = this->getHypothesis();
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c,
                     "CalculiXSmallStrainBehaviour::"
                     "call_behaviour: " +
                         m);
    };
    throw_if(h != ModellingHypothesis::TRIDIMENSIONAL,
             "unsupported modelling hypothesis");
    throw_if(ktype != StiffnessMatrixType::CONSISTENTTANGENTOPERATOR,
             "CalculiX behaviours only provide the "
             "consistent tangent operator");
    real orab[7] = {real(0), real(0), real(0), real(0),
                    real(0), real(0), real(0)};
    const CalculiXInt nprops =
        -100 - static_cast<CalculiXInt>(s.mprops1.size());
    std::fill(wk.D.begin(), wk.D.end(), 0.);
    // using a local copy of internal state variables to handle the
    // case where s.iv0 is empty
    std::copy(s.iv0.begin(), s.iv0.end(), wk.ivs.begin());
    const auto nstatv = static_cast<CalculiXInt>(wk.ivs.size());
    stensor<3u, real> ue0(real(0));
    stensor<3u, real> ue1(real(0));
    stensor<3u, real> us(real(0));
    std::copy(s.e0.begin(), s.e0.end(), ue0.begin());
    std::copy(s.e1.begin(), s.e1.end(), ue1.begin());
    std::copy(s.s0.begin(), s.s0.end(), us.begin());
    // thermal strain
    for (CalculiXInt i = 0; i != 6; ++i) {
      ue0(i) -= s.e_th0(i);
      ue1(i) -= s.e_th1(i);
    }
    if (this->stype == 1u) {
      orab[0] = s.r(0, 0);
      orab[1] = s.r(1, 0);
      orab[2] = s.r(2, 0);
      orab[3] = s.r(0, 1);
      orab[4] = s.r(1, 1);
      orab[5] = s.r(2, 1);
      orab[6] = real(1);
    }
    // CalculiX standard convention
    for (CalculiXInt i = 3; i != 6; ++i) {
      ue0(i) /= sqrt2;
      ue1(i) /= sqrt2;
      us(i) /= sqrt2;
    }
    auto ndt = std::numeric_limits<CalculiXReal>::max();
    const auto iel = CalculiXInt(1);
    const auto iint = CalculiXInt(1);
    const auto ithermal = CalculiXInt(1);
    const auto icmd = CalculiXInt(0);
    const auto ielas = CalculiXInt(1);
    const auto mi = CalculiXInt(1);
    const auto ioren = (this->stype == 1u) ? CalculiXInt(1) : CalculiXInt(0);
    const auto T = s.esv0(0) + s.desv(0);
    (this->fct)(nullptr, &iel, &iint, &nprops,
                s.mprops1.empty() ? nullptr : &s.mprops1[0], &ue1(0), &ue0(0),
                nullptr, nullptr, nullptr, nullptr, nullptr, &ithermal, &T, &dt,
                nullptr, nullptr, &icmd, &ielas, &mi, &nstatv,
                s.iv0.empty() ? nullptr : &s.iv0(0),
                wk.ivs.empty() ? nullptr : &wk.ivs(0), &us(0), &(wk.D(0, 0)),
                &ioren, nullptr, orab, &ndt, nullptr,
                0 /*hidden fortran parameter */);
    if ((ndt > 0) && (ndt < 1.)) {
      return {false, ndt};
    }
    // treating the consistent tangent operator
    const auto K = this->convertTangentOperator(&(wk.D(0, 0)));
    for (unsigned short i = 0; i != 6u; ++i) {
      for (unsigned short j = 0; j != 6u; ++j) {
        Kt(i, j) = K(i, j);
      }
    }
    if (b) {
      // treating internal state variables
      if (!s.iv0.empty()) {
        std::copy_n(wk.ivs.begin(), s.iv1.size(), s.iv1.begin());
      }
      // turning stresses in TFEL conventions
      for (CalculiXInt i = 3; i != 6; ++i) {
        us[i] *= sqrt2;
      }
      std::copy(us.begin(), us.begin() + s.s1.size(), s.s1.begin());
    }
    return {true, (ndt > 0) ? ndt : 1};
  }

  CalculiXSmallStrainBehaviour::~CalculiXSmallStrainBehaviour() = default;

}  // end of namespace mtest
