/*!
 * \file   mtest/src/AbaqusSmallStrainBehaviour.cxx
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
#include "MFront/Abaqus/Abaqus.hxx"
#include "MFront/Abaqus/AbaqusComputeStiffnessTensor.hxx"

#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/UmatNormaliseTangentOperator.hxx"
#include "MTest/AbaqusSmallStrainBehaviour.hxx"

namespace mtest {

  AbaqusSmallStrainBehaviour::AbaqusSmallStrainBehaviour(const Hypothesis h,
                                                         const std::string& l,
                                                         const std::string& b)
      : AbaqusStandardBehaviour(h, l, b) {}

  void AbaqusSmallStrainBehaviour::getGradientsDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    std::fill(v.begin(), v.end(), real(0));
  }

  std::pair<bool, real> AbaqusSmallStrainBehaviour::call_behaviour(
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
    constexpr const auto sqrt2 = Cste<real>::sqrt2;
    const auto h = this->getHypothesis();
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "AbaqusSmallStrainBehaviour::call_behaviour: " + m);
    };
    throw_if(ktype != StiffnessMatrixType::CONSISTENTTANGENTOPERATOR,
             "abaqus behaviours only provide the "
             "consistent tangent operator");
    const AbaqusInt nprops =
        s.mprops1.empty() ? 1 : static_cast<AbaqusInt>(s.mprops1.size());
    const AbaqusInt ntens = [&h, &throw_if] {
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
    std::fill(wk.D.begin(), wk.D.end(), 0.);
    // using a local copy of material properties to handle the
    // case where s.mprops1 is empty
    std::copy(s.mprops1.begin(), s.mprops1.end(), wk.mps.begin());
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
    std::copy(s.iv0.begin(), s.iv0.end(), wk.ivs.begin());
    const auto nstatv = static_cast<AbaqusInt>(wk.ivs.size());
    // rotation matrix, set to identity
    const tmatrix<3u, 3u, real> drot = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    stensor<3u, real> ue0(real(0));
    stensor<3u, real> ude(real(0));
    stensor<3u, real> us(real(0));
    copy(s.e0.begin(), s.e0.end(), ue0.begin());
    copy(s.s0.begin(), s.s0.end(), us.begin());
    for (AbaqusInt i = 0; i != s.e1.size(); ++i) {
      ude(i) = s.e1(i) - s.e0(i);
    }
    // thermal strain
    for (AbaqusInt i = 0; i != static_cast<unsigned short>(ntens); ++i) {
      ue0(i) -= s.e_th0(i);
      ude(i) -= s.e_th1(i) - s.e_th0(i);
    }
    if ((this->stype == 1u) && (this->omp != 2u)) {
      ue0.changeBasis(s.r);
      ude.changeBasis(s.r);
      us.changeBasis(s.r);
    }
    // abaqus standard convention
    for (AbaqusInt i = 3; i != s.e1.size(); ++i) {
      ue0(i) *= sqrt2;
      ude(i) *= sqrt2;
    }
    if (h == ModellingHypothesis::PLANESTRESS) {
      std::swap(ue0[2], ue0[3]);
      std::swap(ude[2], ude[3]);
      us[2] = us[3] / sqrt2;
      us[3] = 0;
    } else {
      for (AbaqusInt i = 3; i != static_cast<unsigned short>(ntens); ++i) {
        us(i) /= sqrt2;
      }
    }
    AbaqusReal ndt = std::numeric_limits<AbaqusReal>::max();
    const AbaqusInt KSTEP[3u] = {0, 0, 0};
    auto scd = AbaqusReal{};
    s.se1 = s.se0;
    s.de1 = s.de0;
    (this->fct)(&us(0), wk.ivs.size() == 0 ? nullptr : &wk.ivs(0),
                &(wk.D(0, 0)), &s.se1, &s.de1, &scd, nullptr, nullptr, nullptr,
                nullptr, &ue0(0), &ude(0), nullptr, &dt, &(s.esv0(0)),
                &(s.desv(0)), &(s.esv0(0)) + 1, &(s.desv(0)) + 1, nullptr,
                nullptr, nullptr, &ntens, &nstatv, &(wk.mps(0)), &nprops,
                nullptr, &drot(0, 0), &ndt, nullptr, nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr, KSTEP, nullptr, 0);
    if (ndt < 1.) {
      return {false, ndt};
    }
    const auto rb = transpose(s.r);
    // treating the consistent tangent operator
    if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      UmatNormaliseTangentOperator::exe(&Kt(0, 0), wk.D, 3u);
      if ((this->stype == 1u) && (this->omp != 2u)) {
        st2tost2<3u, AbaqusReal> K;
        for (unsigned short i = 0; i != 6u; ++i) {
          for (unsigned short j = 0; j != 6u; ++j) {
            K(i, j) = Kt(i, j);
          }
        }
        const auto nK = change_basis(K, rb);
        for (unsigned short i = 0; i != 6u; ++i) {
          for (unsigned short j = 0; j != 6u; ++j) {
            Kt(i, j) = nK(i, j);
          }
        }
      }
    } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
               (h == ModellingHypothesis::PLANESTRAIN)) {
      UmatNormaliseTangentOperator::exe(&Kt(0, 0), wk.D, 2u);
      if ((this->stype == 1u) && (this->omp != 2u)) {
        st2tost2<2u, AbaqusReal> K;
        for (unsigned short i = 0; i != 4u; ++i) {
          for (unsigned short j = 0; j != 4u; ++j) {
            K(i, j) = Kt(i, j);
          }
        }
        const auto nK = change_basis(K, rb);
        for (unsigned short i = 0; i != 4u; ++i) {
          for (unsigned short j = 0; j != 4u; ++j) {
            Kt(i, j) = nK(i, j);
          }
        }
      }
    } else if (h == ModellingHypothesis::PLANESTRESS) {
      constexpr const auto zero = AbaqusReal{0};
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
      UmatNormaliseTangentOperator::exe(&Kt(0, 0), wk.D, 2u);
      if ((this->stype == 1u) && (this->omp != 2u)) {
        // the last step: rotation in the global frame
        st2tost2<2u, AbaqusReal> K;
        for (unsigned short i = 0; i != 4u; ++i) {
          for (unsigned short j = 0; j != 4u; ++j) {
            K(i, j) = Kt(i, j);
          }
        }
        const auto nK = change_basis(K, rb);
        for (unsigned short i = 0; i != 4u; ++i) {
          for (unsigned short j = 0; j != 4u; ++j) {
            Kt(i, j) = nK(i, j);
          }
        }
      }
    } else {
      throw_if(true, "unsupported modelling hypothesis");
    }
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
      if ((this->stype == 1u) && (this->omp != 2u)) {
        us.changeBasis(rb);
      }
      copy(us.begin(), us.begin() + s.s1.size(), s.s1.begin());
    }
    return {true, ndt};
  }

  AbaqusSmallStrainBehaviour::~AbaqusSmallStrainBehaviour() = default;

}  // end of namespace mtest
