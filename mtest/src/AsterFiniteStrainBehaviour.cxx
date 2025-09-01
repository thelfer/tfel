/*!
 * \file   mtest/src/AsterFiniteStrainBehaviour.cxx
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
#include <ostream>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/Aster/Aster.hxx"
#include "MFront/Aster/AsterComputeStiffnessTensor.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/UmatNormaliseTangentOperator.hxx"
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
    using size_type = unsigned short;
    t2tost2<N, real> dtau_ddF;
    t2tost2<N, real> dtau;
    t2tost2<N, real> dsig;
    stensor<N, real> sig(&sv[0]);
    tensor<N, real> F0(&Fv0[0]);
    tensor<N, real> F1(&Fv1[0]);
    tensor<N, real> inv_F0 = invert(F0);
    // reverting things
    const aster::AsterReal* v = &D(0, 0);
    for (size_type i = 0; i != StensorDimeToSize<N>::value;
         ++i) {  // boucle sur tau
      for (size_type j = 0; j != TensorDimeToSize<N>::value;
           ++j) {  // boucle sur F
        const size_type mi = getRowIndex(j);
        const size_type mj = getColumnIndex(j);
        dtau_ddF(i, j) = v[i + 6 * (mi + 3 * mj)];
      }
    }
    dtau = dtau_ddF * t2tot2<N, real>::tpld(inv_F0);
    computeCauchyStressDerivativeFromKirchhoffStressDerivative(dsig, dtau, sig,
                                                               F1);
    for (size_type i = 0; i != StensorDimeToSize<N>::value;
         ++i) {  // boucle sur tau
      for (size_type j = 0; j != TensorDimeToSize<N>::value;
           ++j) {  // boucle sur F
        Kt(i, j) = dsig(i, j);
      }
    }
  }

  AsterFiniteStrainBehaviour::AsterFiniteStrainBehaviour(const Hypothesis h,
                                                         const std::string& l,
                                                         const std::string& b)
      : AsterStandardBehaviour(h, l, b) {
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    const auto fs = elm.getAsterFiniteStrainFormulation(l, b);
    if (fs == 1) {
      this->afsf = 1u;
    } else if (fs == 2) {
      this->afsf = 2u;
    } else {
      tfel::raise(
          "AsterFiniteStrainBehaviour::AsterFiniteStrainBehaviour: "
          "invalid finite strain formulation for behaviour '" +
          b +
          "' "
          "in library '" +
          l + "'");
    }
  }  // end of AsterFiniteStrainBehaviour::AsterFiniteStrainBehaviour

  void AsterFiniteStrainBehaviour::getGradientsDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    std::fill(v.begin(), v.end(), real(0));
    v[0] = v[1] = v[2] = real(1);
  }

  void AsterFiniteStrainBehaviour::allocateWorkSpace(
      BehaviourWorkSpace& wk) const {
    AsterStandardBehaviour::allocateWorkSpace(wk);
    if (this->afsf == 1u) {
      wk.D.resize(6u, 9u);
    }
  }

  std::pair<bool, real> AsterFiniteStrainBehaviour::call_behaviour(
      tfel::math::matrix<real>& Kt,
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype,
      const bool b) const {
    using namespace tfel::math;
    using namespace tfel::material;
    using namespace aster;
    using tfel::math::vector;
    using TangentOperator =
        tfel::material::FiniteStrainBehaviourTangentOperatorBase;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "AsterFiniteStrainBehaviour::call_behaviour: " + m);
    };
    constexpr auto sqrt2 = Cste<real>::sqrt2;
    const auto h = this->getHypothesis();
    const auto [ntens, dimension,
                nummod] = [h]() -> std::tuple<AsterInt, AsterInt, AsterInt> {
      if (h == ModellingHypothesis::AXISYMMETRICAL) {
        return {4, 2, 4};
      } else if (h == ModellingHypothesis::PLANESTRESS) {
        return {4, 2, 5};
      } else if (h == ModellingHypothesis::PLANESTRAIN) {
        return {4, 2, 6};
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return {6, 3, 3};
      }
      tfel::raise("unsupported hypothesis");
    }();
    const auto nprops =
        s.mprops1.size() == 0 ? 1 : static_cast<AsterInt>(s.mprops1.size());
    // using a local copy of material properties to handle the
    // case where s.mprops1 is empty
    std::copy(s.mprops1.begin(), s.mprops1.end(), wk.mps.begin());
    if (s.mprops1.empty()) {
      wk.mps[0] = real(0);
    }
    // using a local copy of internal state variables to handle the
    // case where s.iv0 is empty
    std::copy(s.iv0.begin(), s.iv0.end(), wk.ivs.begin());
    if (s.iv0.empty()) {
      wk.ivs[0] = real(0);
    }
    const auto nstatv = static_cast<AsterInt>(wk.ivs.size());
    //
    std::fill(wk.D.begin(), wk.D.end(), 0.);
    // choosing the type of stiffness matrix
    StandardBehaviourBase::initializeTangentOperator(wk.D, ktype, b);
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
      throw_if(true, "unsupported hypothesis");
    }
    std::copy(s.s0.begin(), s.s0.end(), s.s1.begin());
    for (unsigned short i = 3; i != static_cast<unsigned short>(ntens); ++i) {
      s.s1(i) /= sqrt2;
    }
    AsterReal ndt(1.);
    (this->fct)(&(s.s1(0)), &(wk.ivs(0)), &(wk.D(0, 0)), &uu0(0, 0), &uu1(0, 0),
                &dt, &(s.esv0(0)), &(s.desv(0)), &(s.esv0(0)) + 1,
                &(s.desv(0)) + 1, &ntens, &nstatv, &wk.mps(0), &nprops,
                &drot(0, 0), &ndt, &nummod);
    if (ndt < 1) {
      if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
        if (this->emsg != nullptr) {
          mfront::getLogStream() << this->emsg() << std::endl;
        }
      }
      return {false, ndt};
    }
    if (b) {
      if (!s.iv0.empty()) {
        std::copy_n(wk.ivs.begin(), s.iv1.size(), s.iv1.begin());
      }
      // turning things in standard conventions
      for (unsigned short i = 3; i != static_cast<unsigned short>(ntens); ++i) {
        s.s1(i) *= sqrt2;
      }
      if (this->afsf == 2u) {
        // converting pk2-stress to cauchy stress
        if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          const stensor<3u, real> pk2(&s.s1[0]);
          StensorView<3u, real> sig(&s.s1[0]);
          sig = convertSecondPiolaKirchhoffStressToCauchyStress(
              pk2, tensor<3u, real>(&s.e1[0]));
        } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                   (h == ModellingHypothesis::PLANESTRAIN) ||
                   (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
          const stensor<2u, real> pk2(&s.s1[0]);
          StensorView<2u, real> sig(&s.s1[0]);
          sig = convertSecondPiolaKirchhoffStressToCauchyStress(
              pk2, tensor<2u, real>(&s.e1[0]));
        } else if (h == ModellingHypothesis::PLANESTRESS) {
          throw_if(true, "plane stress modelling hypothesis is not supported");
        } else {
          throw_if(true, "unsupported hypothesis");
        }
      }
    }
    if (ktype != StiffnessMatrixType::NOSTIFFNESS) {
      if (this->afsf == 1u) {
        if (dimension == 1u) {
          convertTangentOperator<1u>(Kt, wk.D, s.s1, s.e0, s.e1);
        } else if (dimension == 2u) {
          convertTangentOperator<2u>(Kt, wk.D, s.s1, s.e0, s.e1);
        } else if (dimension == 3u) {
          convertTangentOperator<3u>(Kt, wk.D, s.s1, s.e0, s.e1);
        }
      } else if (this->afsf == 2u) {
        if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          st2tost2<3u, real> K;
          UmatNormaliseTangentOperator::exe(&K(0, 0), wk.D, 3u);
          const auto ds =
              convert<TangentOperator::DSIG_DF, TangentOperator::DS_DEGL>(
                  K, tensor<3u, real>(&s.e0[0]), tensor<3u, real>(&s.e1[0]),
                  stensor<3u, real>(&s.s1[0]));
          std::copy(ds.begin(), ds.end(), Kt.begin());
        } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                   (h == ModellingHypothesis::PLANESTRAIN) ||
                   (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
          st2tost2<2u, real> K;
          UmatNormaliseTangentOperator::exe(&K(0, 0), wk.D, 2u);
          const auto ds =
              convert<TangentOperator::DSIG_DF, TangentOperator::DS_DEGL>(
                  K, tensor<2u, real>(&s.e0[0]), tensor<2u, real>(&s.e1[0]),
                  stensor<2u, real>(&s.s1[0]));
          std::copy(ds.begin(), ds.end(), Kt.begin());
        } else if (h == ModellingHypothesis::PLANESTRESS) {
          throw_if(true, "plane stress modelling hypothesis is not supported");
        } else {
          throw_if(true, "unsupported hypothesis");
        }
      }
    }
    return {true, ndt};
  }

  AsterFiniteStrainBehaviour::~AsterFiniteStrainBehaviour() = default;

}  // end of namespace mtest
