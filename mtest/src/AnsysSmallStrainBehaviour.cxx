/*!
 * \file   mfront/mtest/AnsysSmallStrainBehaviour.cxx
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
#include "MFront/Ansys/Ansys.hxx"
#include "MFront/Ansys/AnsysComputeStiffnessTensor.hxx"

#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/AnsysNormaliseTangentOperator.hxx"
#include "MTest/AnsysSmallStrainBehaviour.hxx"

namespace mtest {

  AnsysSmallStrainBehaviour::AnsysSmallStrainBehaviour(const Hypothesis h,
                                                       const std::string& l,
                                                       const std::string& b)
      : AnsysStandardBehaviour(h, l, b) {}

  void AnsysSmallStrainBehaviour::getDrivingVariablesDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    std::fill(v.begin(), v.end(), real(0));
  }

  std::pair<bool, real> AnsysSmallStrainBehaviour::call_behaviour(
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
    constexpr const auto sqrt2 = Cste<real>::sqrt2;
    const auto h = this->getHypothesis();
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c,
                     "AnsysSmallStrainBehaviour::"
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
    stensor<3u, real> ue0(real(0));
    stensor<3u, real> ude(real(0));
    stensor<3u, real> us(real(0));
    copy(s.e0.begin(), s.e0.end(), ue0.begin());
    copy(s.s0.begin(), s.s0.end(), us.begin());
    for (AnsysInt i = 0; i != s.e1.size(); ++i) {
      ude(i) = s.e1(i) - s.e0(i);
    }
    // thermal strain
    for (AnsysInt i = 0; i != static_cast<unsigned short>(ntens); ++i) {
      ue0(i) -= s.e_th0(i);
      ude(i) -= s.e_th1(i) - s.e_th0(i);
    }
    // ansys standard convention
    for (AnsysInt i = 3; i != s.e1.size(); ++i) {
      ue0(i) /= sqrt2;
      ude(i) /= sqrt2;
    }
    for (AnsysInt i = 3; i != static_cast<unsigned short>(ntens); ++i) {
      us(i) /= sqrt2;
    }
    if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      std::swap(ue0[4], ue0[5]);
      std::swap(ude[4], ude[5]);
      std::swap(us[4], us[5]);
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
                &sedEl, &sedPl, &epseq, &ue0(0), &ude(0), epsPl, &(wk.mps(0)),
                coords, &drot(0, 0), nullptr, nullptr, &tsstif, &epsZZ, &var[0],
                &var[1], &var[2], &var[3], &var[4], &var[5], &var[6], &var[7]);
    if (keycut != 0) {
      return {false, 0.5};
    }
    // treating the consistent tangent operator
    if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      // Convertion from Voigt 23 <-> 13
      // changing last columns
      std::swap(wk.D(0, 4), wk.D(0, 5));
      std::swap(wk.D(1, 4), wk.D(1, 5));
      std::swap(wk.D(2, 4), wk.D(2, 5));
      std::swap(wk.D(3, 4), wk.D(3, 5));
      std::swap(wk.D(4, 4), wk.D(4, 5));
      std::swap(wk.D(5, 4), wk.D(5, 5));
      // changing last rows
      std::swap(wk.D(4, 0), wk.D(5, 0));
      std::swap(wk.D(4, 1), wk.D(5, 1));
      std::swap(wk.D(4, 2), wk.D(5, 2));
      std::swap(wk.D(4, 3), wk.D(5, 3));
      std::swap(wk.D(4, 4), wk.D(5, 4));
      std::swap(wk.D(4, 5), wk.D(5, 5));
      // normalizing
      AnsysNormaliseTangentOperator::exe(&Kt(0, 0), wk.D, 3u);
    } else if (h == ModellingHypothesis::AXISYMMETRICAL) {
      AnsysNormaliseTangentOperator::exe(&Kt(0, 0), wk.D, 2u);
    } else if (h == ModellingHypothesis::PLANESTRESS) {
      constexpr const auto zero = AnsysReal{0};
      // D has been as a 3*3 fortran matrix. The terms associated with
      // the 2 indices are omitted.
      // D = D00 D10 D30 D01 D11 D31 D03 D13 D33
      double D2[9u];
      std::copy(wk.D.begin(), wk.D.begin() + 9, D2);
      // Let us add the missing term
      // We want D00 D10 D20 D30 D01 D11 D21 D31 D02 D12 D22 D32 D03 D13 D23 D33
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
      // so now we have D in a conventional fortan form, so we can
      // normalise it (transpose and TFEL storage conventions !)
      AnsysNormaliseTangentOperator::exe(&Kt(0, 0), wk.D, 2u);
    } else {
      throw_if(true, "unsupported modelling hypothesis");
    }
    if (b) {
      // treating internal state variables
      if (!s.iv0.empty()) {
        copy_n(wk.ivs.begin(), s.iv1.size(), s.iv1.begin());
      }
      // turning stresses in TFEL conventions
      for (AnsysInt i = 3; i != static_cast<unsigned short>(ntens); ++i) {
        us[i] *= sqrt2;
      }
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        std::swap(us[4], us[5]);
      }
      copy(us.begin(), us.begin() + s.s1.size(), s.s1.begin());
    }
    return {true, 1};
  }

  AnsysSmallStrainBehaviour::~AnsysSmallStrainBehaviour() = default;

}  // end of namespace mtest
