/*!
 * \file   mtest/src/CalculiXFiniteStrainBehaviour.cxx
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
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

#include "MFront/CalculiX/CalculiX.hxx"
#include "MFront/CalculiX/CalculiXComputeStiffnessTensor.hxx"

#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/UmatNormaliseTangentOperator.hxx"
#include "MTest/CalculiXFiniteStrainBehaviour.hxx"

namespace mtest {

  CalculiXFiniteStrainBehaviour::CalculiXFiniteStrainBehaviour(
      const Hypothesis h, const std::string& l, const std::string& b)
      : CalculiXStandardBehaviour(h, l, b) {}

  void CalculiXFiniteStrainBehaviour::getGradientsDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    v[0] = v[1] = v[2] = real(1);
    std::fill(v.begin() + 3, v.end(), real(0));
  }

  std::pair<bool, real> CalculiXFiniteStrainBehaviour::call_behaviour(
      tfel::math::matrix<real>& Kt,
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype,
      const bool b) const {
    using namespace tfel::math;
    using namespace tfel::material;
    using namespace calculix;
    using tfel::math::vector;
    using FSTOBase = FiniteStrainBehaviourTangentOperatorBase;
    constexpr auto sqrt2 = Cste<real>::sqrt2;
    const auto h = this->getHypothesis();
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "CalculiXFiniteStrainBehaviour::call_behaviour: " + m);
    };
    throw_if(ktype != StiffnessMatrixType::CONSISTENTTANGENTOPERATOR,
             "CalculiX behaviours only provide the "
             "consistent tangent operator");
    throw_if(h != ModellingHypothesis::TRIDIMENSIONAL,
             "unsupported modelling hypothesis");
    const CalculiXInt nprops =
        -100 - static_cast<CalculiXInt>(s.mprops1.size());
    std::fill(wk.D.begin(), wk.D.end(), 0.);
    // using a local copy of internal state variables to handle the
    // case where s.iv0 is empty
    std::copy(s.iv0.begin(), s.iv0.end(), wk.ivs.begin());
    const auto nstatv = static_cast<CalculiXInt>(wk.ivs.size());
    tmatrix<3u, 3u, real> uu0(real(0));
    tmatrix<3u, 3u, real> uu1(real(0));
    stensor<3u, real> us(real(0));
    real orab[7] = {real(0), real(0), real(0), real(0),
                    real(0), real(0), real(0)};
    if (this->stype == 1u) {
      orab[0] = s.r(0, 0);
      orab[1] = s.r(1, 0);
      orab[2] = s.r(2, 0);
      orab[3] = s.r(0, 1);
      orab[4] = s.r(1, 1);
      orab[5] = s.r(2, 1);
      orab[6] = real(1);
    }
    uu0(0, 0) = s.e0(0);
    uu1(0, 0) = s.e1(0);
    uu0(1, 1) = s.e0(1);
    uu1(1, 1) = s.e1(1);
    uu0(2, 2) = s.e0(2);
    uu1(2, 2) = s.e1(2);
    // uu0 and uu1 must be built using Fortran notations
    uu0(1, 0) = s.e0(3);
    uu1(1, 0) = s.e1(3);
    uu0(0, 1) = s.e0(4);
    uu1(0, 1) = s.e1(4);
    uu0(2, 0) = s.e0(5);
    uu1(2, 0) = s.e1(5);
    uu0(0, 2) = s.e0(6);
    uu1(0, 2) = s.e1(6);
    uu0(2, 1) = s.e0(7);
    uu1(2, 1) = s.e1(7);
    uu0(1, 2) = s.e0(8);
    uu1(1, 2) = s.e1(8);
    const auto F0 = tensor<3u, real>(&s.e0(0));
    const auto F1 = tensor<3u, real>(&s.e1(0));
    const auto v0j = det(F0);
    const auto vj = det(F1);
    auto ue0 = computeGreenLagrangeTensor(F0);
    auto ue1 = computeGreenLagrangeTensor(F1);
    // CalculiX standard convention
    std::copy(s.s0.begin(), s.s0.end(), us.begin());
    auto pk2 = convertCauchyStressToSecondPiolaKirchhoffStress(us, F0);
    for (CalculiXInt i = 3; i != 6; ++i) {
      ue0(i) /= sqrt2;
      ue1(i) /= sqrt2;
      pk2(i) /= sqrt2;
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
    (this->fct)(
        nullptr, &iel, &iint, &nprops,
        s.mprops1.empty() ? nullptr : &s.mprops1[0], &ue1(0), &ue0(0), nullptr,
        &uu0(0, 0), &v0j, &uu1(0, 0), &vj, &ithermal, &T, &dt, nullptr, nullptr,
        &icmd, &ielas, &mi, &nstatv, s.iv0.empty() ? nullptr : &s.iv0(0),
        wk.ivs.empty() ? nullptr : &wk.ivs(0), &pk2(0), &(wk.D(0, 0)), &ioren,
        nullptr, orab, &ndt, nullptr, 0 /*hidden fortran parameter */);
    if ((ndt > 0) && (ndt < 1.)) {
      return {false, ndt};
    }
    // turning stresses in TFEL conventions
    for (CalculiXInt i = 3; i != 6; ++i) {
      pk2[i] *= sqrt2;
    }
    // turning pk2 to Cauchy stress
    us = convertSecondPiolaKirchhoffStressToCauchyStress(pk2, F1);
    // converting the tangent operator
    const auto Cse = this->convertTangentOperator(&(wk.D(0, 0)));
    const auto K =
        convert<FSTOBase::DSIG_DF, FSTOBase::DS_DEGL>(Cse, F0, F1, us);
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
      std::copy(us.begin(), us.begin() + s.s1.size(), s.s1.begin());
    }
    return {true, (ndt > 0) ? ndt : 1};
  }

  CalculiXFiniteStrainBehaviour::~CalculiXFiniteStrainBehaviour() = default;

}  // end of namespace mtest
