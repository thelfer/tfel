/*!
 * \file   mfront/mtest/AnsysFiniteStrainBehaviour.cxx
 * \brief
 * \author Thomas Helfer
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

#include "TFEL/Raise.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "MFront/Ansys/Ansys.hxx"
#include "MFront/Ansys/AnsysComputeStiffnessTensor.hxx"

#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/UmatNormaliseTangentOperator.hxx"
#include "MTest/AnsysNormaliseTangentOperator.hxx"
#include "MTest/AnsysFiniteStrainBehaviour.hxx"

namespace mtest {

  AnsysFiniteStrainBehaviour::AnsysFiniteStrainBehaviour(const Hypothesis h,
                                                         const std::string& l,
                                                         const std::string& b)
      : AnsysStandardBehaviour(h, l, b) {}

  void AnsysFiniteStrainBehaviour::getDrivingVariablesDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    v[0] = v[1] = v[2] = real(1);
    std::fill(v.begin() + 3, v.end(), real(0));
  }

  std::pair<bool, real> AnsysFiniteStrainBehaviour::call_behaviour(
      tfel::math::matrix<real>& Kt,
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype,
      const bool b) const {
    using namespace std;
    using namespace tfel::math;
    using namespace ansys;
    using tfel::math::vector;
    using TangentOperator =
        tfel::material::FiniteStrainBehaviourTangentOperatorBase;
    using tfel::material::convert;
    constexpr const auto sqrt2 = Cste<real>::sqrt2;
    const auto h = this->getHypothesis();
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c,
                     "AnsysFiniteStrainBehaviour::"
                     "call_behaviour: " +
                         m);
    };
    throw_if(ktype != StiffnessMatrixType::CONSISTENTTANGENTOPERATOR,
             "ansys behaviours only provide the "
             "consistent tangent operator");
    const AnsysInt nprops = s.mprops1.size();
    const AnsysInt ndirect = [&h, &throw_if] {
      if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::PLANESTRESS)) {
        return 3;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 3;
      }
      throw_if(true, "unsupported hypothesis");
    }();
    const AnsysInt nshear = [&h, &throw_if] {
      if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::PLANESTRESS)) {
        return 1;
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 3;
      }
      throw_if(true, "unsupported hypothesis");
    }();
    const AnsysInt ntens = [&h, &throw_if] {
      if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::PLANESTRESS)) {
        return 4;
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
    if (this->stype == 1u) {
      const auto o = this->getOrthototropicAxesOffset();
      if ((h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::PLANESTRAIN)) {
        throw_if(wk.mps.size() < o + 2,
                 "invalid number of material properties");
        wk.mps[o] = s.r(0, 0);
        wk.mps[o + 1] = s.r(1, 0);
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        throw_if(wk.mps.size() < o + 6,
                 "invalid number of material properties");
        wk.mps[o] = s.r(0, 0);
        wk.mps[o + 1] = s.r(1, 0);
        wk.mps[o + 2] = s.r(2, 0);
        wk.mps[o + 3] = s.r(0, 1);
        wk.mps[o + 4] = s.r(1, 1);
        wk.mps[o + 5] = s.r(2, 1);
      } else {
        throw_if(true, "unsupported hypothesis (" +
                           ModellingHypothesis::toString(h) + ")");
      }
    }
    // using a local copy of internal state variables to handle the
    // case where s.iv0 is empty
    copy(s.iv0.begin(), s.iv0.end(), wk.ivs.begin());
    const auto nstatv = static_cast<AnsysInt>(wk.ivs.size());
    // infinitesimal rotation matrix, set to identity
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
      tfel::raise(
          "AbaqusFiniteStrainBehaviour::integrate: "
          "unsupported hypothesis");
    }
    copy(s.s0.begin(), s.s0.end(), us.begin());
    // ansys standard convention
    for (AnsysInt i = 3; i != static_cast<unsigned short>(ntens); ++i) {
      us(i) /= sqrt2;
    }
    const AnsysInt matId = 0;
    const AnsysInt elemId = 0;
    const AnsysInt kDomIntPt = 0;
    const AnsysInt kLayer = 0;
    const AnsysInt kSectPt = 0;
    const AnsysInt ldsetp = 0;
    const AnsysInt isubst = 0;
    const AnsysReal TIME = 0;
    const AnsysReal tsstif = 0;
    AnsysReal epsZZ = 0;
    AnsysReal sedEl = 0;
    AnsysReal sedPl = 0;
    AnsysReal epseq = 0;
    AnsysReal epsPl[6] = {0, 0, 0, 0, 0, 0};
    const AnsysReal coords[3] = {0, 0, 0};
    const AnsysReal var[8] = {
        0, 0, 0, 0, 0, 0, 0, 0,
    };
    AnsysInt keycut = 0;
    (this->fct)(&matId, &elemId, &kDomIntPt, &kLayer, &kSectPt, &ldsetp,
                &isubst, &keycut, &ndirect, &nshear, &ntens, &nstatv, &nprops,
                &TIME, &dt, &(s.esv0(0)), &(s.desv(0)), &us(0),
                wk.ivs.size() == 0 ? nullptr : &wk.ivs(0), &(wk.D(0, 0)),
                &sedEl, &sedPl, &epseq, nullptr, nullptr, epsPl, &(wk.mps(0)),
                coords, &drot(0, 0), &uu0(0, 0), &uu1(0, 0), &tsstif, &epsZZ,
                &var[0], &var[1], &var[2], &var[3], &var[4], &var[5], &var[6],
                &var[7]);
    if (keycut != 0) {
      return {false, 0.5};
    }
    if (b) {
      // treating internal state variables
      if (!s.iv0.empty()) {
        copy_n(wk.ivs.begin(), s.iv1.size(), s.iv1.begin());
      }
      // treating stresses
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        std::swap(us[4], us[5]);
      }
      for (AnsysInt i = 3; i != static_cast<unsigned short>(ntens); ++i) {
        us[i] *= sqrt2;
      }
      copy(us.begin(), us.begin() + s.s1.size(), s.s1.begin());
    }
    if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      st2tost2<3u, AnsysReal> K;
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
        constexpr const auto zero = AnsysReal{0};
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
      st2tost2<2u, AnsysReal> K;
      UmatNormaliseTangentOperator::exe(&K(0, 0), wk.D, 2u);
      const auto ds =
          convert<TangentOperator::DSIG_DF, TangentOperator::ABAQUS>(
              K, tensor<2u, double>(&s.e0[0]), tensor<2u, double>(&s.e1[0]),
              stensor<2u, double>(&s.s1[0]));
      std::copy(ds.begin(), ds.end(), Kt.begin());
    } else {
      throw_if(true, "unsupported modelling hypothesis");
    }
    return {true, 1};
  }

  AnsysFiniteStrainBehaviour::~AnsysFiniteStrainBehaviour() = default;

}  // end of namespace mtest
