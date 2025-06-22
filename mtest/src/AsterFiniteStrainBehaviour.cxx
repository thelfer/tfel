/*!
 * \file   mfront/mtest/AsterFiniteStrainBehaviour.cxx
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
#include "MFront/Aster/Aster.hxx"
#include "MFront/Aster/AsterComputeStiffnessTensor.hxx"

#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/AsterFiniteStrainBehaviour.hxx"

namespace mtest {

  static unsigned short getRowIndex(const unsigned short i) {
    switch (i) {
      case 0:
        return 0;
      case 1:
        return 1;
      case 2:
        return 2;
      case 3:
        return 0;
      case 4:
        return 1;
      case 5:
        return 0;
      case 6:
        return 2;
      case 7:
        return 1;
      case 8:
        return 2;
    }
    return 0;
  }

  static unsigned short getColumnIndex(const unsigned short j) {
    switch (j) {
      case 0:
        return 0;
      case 1:
        return 1;
      case 2:
        return 2;
      case 3:
        return 1;
      case 4:
        return 0;
      case 5:
        return 2;
      case 6:
        return 0;
      case 7:
        return 2;
      case 8:
        return 1;
    }
    return 0;
  }

  template <unsigned short N>
  static void convertTangentOperator(
      tfel::math::matrix<real>& Kt,
      const tfel::math::matrix<aster::AsterReal>& D,
      const tfel::math::vector<real>& sv,
      const tfel::math::vector<real>& Fv0,
      const tfel::math::vector<real>& Fv1) {
    using namespace tfel::math;
    t2tost2<N, real> dtau_ddF;
    t2tost2<N, real> dtau;
    t2tost2<N, real> dsig;
    stensor<N, real> sig(&sv[0]);
    tensor<N, real> F0(&Fv0[0]);
    tensor<N, real> F1(&Fv1[0]);
    tensor<N, real> inv_F0 = invert(F0);
    // reverting things
    const aster::AsterReal* v = &D(0, 0);
    for (unsigned short i = 0; i != StensorDimeToSize<N>::value;
         ++i) {  // boucle sur tau
      for (unsigned short j = 0; j != TensorDimeToSize<N>::value;
           ++j) {  // boucle sur F
        const unsigned short mi = getRowIndex(j);
        const unsigned short mj = getColumnIndex(j);
        dtau_ddF(i, j) = v[i + 6 * (mi + 3 * mj)];
      }
    }
    dtau = dtau_ddF * t2tot2<N, real>::tpld(inv_F0);
    computeCauchyStressDerivativeFromKirchhoffStressDerivative(dsig, dtau, sig,
                                                               F1);
    for (unsigned short i = 0; i != StensorDimeToSize<N>::value;
         ++i) {  // boucle sur tau
      for (unsigned short j = 0; j != TensorDimeToSize<N>::value;
           ++j) {  // boucle sur F
        Kt(i, j) = dsig(i, j);
      }
    }
  }

  AsterFiniteStrainBehaviour::AsterFiniteStrainBehaviour(const Hypothesis h,
                                                         const std::string& l,
                                                         const std::string& b)
      : AsterStandardBehaviour(h, l, b) {}

  void AsterFiniteStrainBehaviour::getDrivingVariablesDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    std::fill(v.begin(), v.end(), real(0));
    v[0] = v[1] = v[2] = real(1);
  }

  void AsterFiniteStrainBehaviour::allocate(BehaviourWorkSpace& wk) const {
    AsterStandardBehaviour::allocate(wk);
    wk.D.resize(6u, 9u);
  }

  std::pair<bool, real> AsterFiniteStrainBehaviour::call_behaviour(
      tfel::math::matrix<real>& Kt,
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype,
      const bool b) const {
    using namespace std;
    using namespace tfel::math;
    using namespace aster;
    using tfel::math::vector;
    constexpr const auto sqrt2 = Cste<real>::sqrt2;
    AsterInt ntens;
    AsterInt dimension;
    AsterInt nprops =
        s.mprops1.size() == 0 ? 1 : static_cast<AsterInt>(s.mprops1.size());
    AsterInt nstatv;
    AsterInt nummod;
    const auto h = this->getHypothesis();
    if (h == ModellingHypothesis::AXISYMMETRICAL) {
      ntens = 4;
      dimension = 2u;
      nummod = 4u;
    } else if (h == ModellingHypothesis::PLANESTRESS) {
      ntens = 4;
      dimension = 2u;
      nummod = 5u;
    } else if (h == ModellingHypothesis::PLANESTRAIN) {
      ntens = 4;
      dimension = 2u;
      nummod = 6u;
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      ntens = 6;
      dimension = 3u;
      nummod = 3u;
    } else {
      throw(
          runtime_error("AsterFiniteStrainBehaviour::call_behaviour: "
                        "unsupported hypothesis"));
    }
    fill(wk.D.begin(), wk.D.end(), 0.);
    // choosing the type of stiffness matrix
    UmatBehaviourBase::initializeTangentOperator(wk.D, ktype, b);
    // using a local copy of material properties to handle the
    // case where s.mprops1 is empty
    copy(s.mprops1.begin(), s.mprops1.end(), wk.mps.begin());
    if (s.mprops1.empty()) {
      wk.mps[0] = real(0);
    }
    // using a local copy of internal state variables to handle the
    // case where s.iv0 is empty
    copy(s.iv0.begin(), s.iv0.end(), wk.ivs.begin());
    if (s.iv0.empty()) {
      wk.ivs[0] = real(0);
    }
    nstatv = static_cast<AsterInt>(wk.ivs.size());
    // rotation matrix
    tmatrix<3u, 3u, real> drot = transpose(s.r);
    tmatrix<3u, 3u, real> uu0(real(0));
    tmatrix<3u, 3u, real> uu1(real(0));
    uu0(0, 0) = s.e0(0);
    uu1(0, 0) = s.e1(0);
    uu0(1, 1) = s.e0(1);
    uu1(1, 1) = s.e1(1);
    uu0(2, 2) = s.e0(2);
    uu1(2, 2) = s.e1(2);
    if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
        (h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::PLANESTRAIN) ||
        (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
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
          runtime_error("UmatFiniteStrainBehaviour::integrate: "
                        "unsupported hypothesis"));
    }
    copy(s.s0.begin(), s.s0.end(), s.s1.begin());
    for (unsigned short i = 3; i != static_cast<unsigned short>(ntens); ++i) {
      s.s1(i) /= sqrt2;
    }
    AsterReal ndt(1.);
    (this->fct)(&(s.s1(0)), &(wk.ivs(0)), &(wk.D(0, 0)), &uu0(0, 0), &uu1(0, 0),
                &dt, &(s.esv0(0)), &(s.desv(0)), &(s.esv0(0)) + 1,
                &(s.desv(0)) + 1, &ntens, &nstatv, &wk.mps(0), &nprops,
                &drot(0, 0), &ndt, &nummod);
    if (ndt < 1) {
      return {false, ndt};
    }
    if (b) {
      if (!s.iv0.empty()) {
        copy_n(wk.ivs.begin(), s.iv1.size(), s.iv1.begin());
      }
      // turning things in standard conventions
      for (unsigned short i = 3; i != static_cast<unsigned short>(ntens); ++i) {
        s.s1(i) *= sqrt2;
      }
    }
    if (ktype != StiffnessMatrixType::NOSTIFFNESS) {
      if (dimension == 1u) {
        convertTangentOperator<1u>(Kt, wk.D, s.s1, s.e0, s.e1);
      } else if (dimension == 2u) {
        convertTangentOperator<2u>(Kt, wk.D, s.s1, s.e0, s.e1);
      } else if (dimension == 3u) {
        convertTangentOperator<3u>(Kt, wk.D, s.s1, s.e0, s.e1);
      }
    }
    return {true, ndt};
  }

  AsterFiniteStrainBehaviour::~AsterFiniteStrainBehaviour() = default;

}  // end of namespace mtest
