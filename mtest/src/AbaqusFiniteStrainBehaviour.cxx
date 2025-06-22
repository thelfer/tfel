/*!
 * \file   mfront/mtest/AbaqusFiniteStrainBehaviour.cxx
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
#include <limits>
#include <algorithm>

#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "MFront/Abaqus/Abaqus.hxx"
#include "MFront/Abaqus/AbaqusComputeStiffnessTensor.hxx"

#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/UmatNormaliseTangentOperator.hxx"
#include "MTest/AbaqusFiniteStrainBehaviour.hxx"

namespace mtest {

  AbaqusFiniteStrainBehaviour::AbaqusFiniteStrainBehaviour(const Hypothesis h,
                                                           const std::string& l,
                                                           const std::string& b)
      : AbaqusStandardBehaviour(h, l, b) {
    if ((this->stype == 1u) && (this->omp != 2u)) {
      throw(std::runtime_error(
          "AbaqusFiniteStrainBehaviour::AbaqusFiniteStrainBehaviour: "
          "orthotropic finite strain behaviour is only supported "
          "with the 'MFront' orthotropy management policy"));
    }
  }

  void AbaqusFiniteStrainBehaviour::getDrivingVariablesDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    v[0] = real(1);
    v[1] = real(1);
    v[2] = real(1);
    std::fill(v.begin() + 3, v.end(), real(0));
  }

  std::pair<bool, real> AbaqusFiniteStrainBehaviour::call_behaviour(
      tfel::math::matrix<real>& Kt,
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype,
      const bool b) const {
    using namespace std;
    using namespace tfel::math;
    using namespace abaqus;
    using tfel::math::vector;
    using TangentOperator =
        tfel::material::FiniteStrainBehaviourTangentOperatorBase;
    using tfel::material::convert;
    constexpr const auto sqrt2 = Cste<real>::sqrt2;
    const auto h = this->getHypothesis();
    auto throw_if = [](const bool c, const std::string& m) {
      if (c) {
        throw(
            std::runtime_error("AbaqusSmallStrainBehaviour::"
                               "call_behaviour: " +
                               m));
      }
    };
    throw_if(ktype != StiffnessMatrixType::CONSISTENTTANGENTOPERATOR,
             "abaqus behaviours only provide the "
             "consistent tangent operator");
    const AbaqusInt nprops =
        s.mprops1.empty() ? 1 : static_cast<AbaqusInt>(s.mprops1.size());
    const AbaqusInt ntens = [&h, &throw_if]() {
      if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::PLANESTRAIN)) {
        return 4;
      } else if (h == ModellingHypothesis::PLANESTRESS) {
        return 3;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 6;
      }
      throw_if(true, "unsupported hypothesis");
    }();
    fill(wk.D.begin(), wk.D.end(), 0.);
    // using a local copy of material properties to handle the
    // case where s.mprops1 is empty
    copy(s.mprops1.begin(), s.mprops1.end(), wk.mps.begin());
    if (s.mprops1.empty()) {
      wk.mps[0] = real(0);
    }
    // using a local copy of internal state variables to handle the
    // case where s.iv0 is empty
    if ((this->stype == 1u) && (this->omp == 2u)) {
      if ((h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::PLANESTRAIN)) {
        throw_if(s.iv0.size() < 2, "invalid number of state variables");
        s.iv0[0] = s.r(0, 0);
        s.iv0[1] = s.r(1, 0);
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        throw_if(s.iv0.size() < 6, "invalid number of state variables");
        s.iv0[0] = s.r(0, 0);
        s.iv0[1] = s.r(1, 0);
        s.iv0[2] = s.r(2, 0);
        s.iv0[3] = s.r(0, 1);
        s.iv0[4] = s.r(1, 1);
        s.iv0[5] = s.r(2, 1);
      } else {
        throw_if(true, "unsupported hypothesis (" +
                           ModellingHypothesis::toString(h) + ")");
      }
    }
    copy(s.iv0.begin(), s.iv0.end(), wk.ivs.begin());
    const auto nstatv = static_cast<AbaqusInt>(wk.ivs.size());
    // rotation matrix, set to identity
    const tmatrix<3u, 3u, real> drot = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    tmatrix<3u, 3u, real> uu0(real(0));
    tmatrix<3u, 3u, real> uu1(real(0));
    stensor<3u, real> us(real(0));
    uu0(0, 0) = s.e0(0);
    uu1(0, 0) = s.e1(0);
    uu0(1, 1) = s.e0(1);
    uu1(1, 1) = s.e1(1);
    uu0(2, 2) = s.e0(2);
    uu1(2, 2) = s.e1(2);
    if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
        (h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::PLANESTRAIN)) {
      // uu0 and uu1 must be built using Fortran notations
      uu0(1, 0) = s.e0(3);
      uu1(1, 0) = s.e1(3);
      uu0(0, 1) = s.e0(4);
      uu1(0, 1) = s.e1(4);
      uu0(2, 0) = 0.;
      uu1(2, 0) = 0.;
      uu0(0, 2) = 0.;
      uu1(0, 2) = 0.;
      uu0(2, 1) = 0.;
      uu1(2, 1) = 0.;
      uu0(1, 2) = 0.;
      uu1(1, 2) = 0.;
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
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
    } else {
      throw(
          runtime_error("AbaqusFiniteStrainBehaviour::integrate: "
                        "unsupported hypothesis"));
    }
    copy(s.s0.begin(), s.s0.end(), us.begin());
    if (h == ModellingHypothesis::PLANESTRESS) {
      us[2] = us[3] / sqrt2;
      us[3] = 0;
    } else {
      for (AbaqusInt i = 3; i != static_cast<unsigned short>(ntens); ++i) {
        us(i) /= sqrt2;
      }
    }
    AbaqusReal ndt = std::numeric_limits<AbaqusReal>::max();
    const AbaqusInt KSTEP[3u] = {0, 0, 1};
    (this->fct)(&us(0), wk.ivs.size() == 0 ? nullptr : &wk.ivs(0),
                &(wk.D(0, 0)), nullptr, nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr, nullptr, &dt, &(s.esv0(0)),
                &(s.desv(0)), &(s.esv0(0)) + 1, &(s.desv(0)) + 1, nullptr,
                nullptr, nullptr, &ntens, &nstatv, &(wk.mps(0)), &nprops,
                nullptr, &drot(0, 0), &ndt, nullptr, &uu0(0, 0), &uu1(0, 0),
                nullptr, nullptr, nullptr, nullptr, KSTEP, nullptr, 0);
    if (ndt < 1.) {
      return {false, ndt};
    }
    // treating the consistent tangent operator
    if (b) {
      // treating internal state variables
      if (!s.iv0.empty()) {
        copy_n(wk.ivs.begin(), s.iv1.size(), s.iv1.begin());
      }
      // treating stresses
      if (h == ModellingHypothesis::PLANESTRESS) {
        us[3] = us[2] * sqrt2;
        us[2] = real(0);
      } else {
        // turning stresses in TFEL conventions
        for (AbaqusInt i = 3; i != static_cast<unsigned short>(ntens); ++i) {
          us[i] *= sqrt2;
        }
      }
      copy(us.begin(), us.begin() + s.s1.size(), s.s1.begin());
    }
    if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      st2tost2<3u, AbaqusReal> K;
      UmatNormaliseTangentOperator::exe(&K(0, 0), wk.D, 3u);
      const auto ds =
          convert<TangentOperator::DSIG_DF, TangentOperator::ABAQUS>(
              K, tensor<3u, double>(&s.e0[0]), tensor<3u, double>(&s.e1[0]),
              stensor<3u, double>(&s.s1[0]));
      std::copy(ds.begin(), ds.end(), Kt.begin());
    } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
               (h == ModellingHypothesis::PLANESTRAIN) ||
               (h == ModellingHypothesis::PLANESTRESS)) {
      if (h == ModellingHypothesis::PLANESTRESS) {
        constexpr const auto zero = AbaqusReal{0};
        // D has been as a 3*3 fortran matrix. The terms associated with
        // the 2 indices are omitted.
        // D = D00 D10 D30 D01 D11 D31 D03 D13 D33
        double D2[9u];
        std::copy(wk.D.begin(), wk.D.begin() + 9, D2);
        // Let us add the missing term
        // We want D00 D10 D20 D30 D01 D11 D21 D31 D02 D12 D22 D32 D03 D13 D23
        // D33
        auto p = wk.D.begin();
        // D00 D10 D20 D30
        *p = D2[0];
        *(p + 1) = D2[1];
        *(p + 2) = zero;
        *(p + 3) = D2[2];
        // D01 D11 D21 D31
        *(p + 4) = D2[3];
        *(p + 5) = D2[4];
        *(p + 6) = zero;
        *(p + 7) = D2[5];
        // D02 D12 D22 D32
        *(p + 8) = zero;
        *(p + 9) = zero;
        *(p + 10) = zero;
        *(p + 11) = zero;
        // D03 D13 D23 D33
        *(p + 12) = D2[6];
        *(p + 13) = D2[7];
        *(p + 14) = zero;
        *(p + 15) = D2[8];
      }
      st2tost2<2u, AbaqusReal> K;
      UmatNormaliseTangentOperator::exe(&K(0, 0), wk.D, 2u);
      const auto ds =
          convert<TangentOperator::DSIG_DF, TangentOperator::ABAQUS>(
              K, tensor<2u, double>(&s.e0[0]), tensor<2u, double>(&s.e1[0]),
              stensor<2u, double>(&s.s1[0]));
      std::copy(ds.begin(), ds.end(), Kt.begin());
    } else {
      throw_if(true, "unsupported modelling hypothesis");
    }
    return {true, ndt};
  }

  AbaqusFiniteStrainBehaviour::~AbaqusFiniteStrainBehaviour() = default;

}  // end of namespace mtest
