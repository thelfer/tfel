/*!
 * \file  mtest/src/AbaqusExplicitBehaviour.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <ostream>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Matrix/tmatrixIO.hxx"
#include "TFEL/Math/Stensor/StensorConceptIO.hxx"
#include "TFEL/Math/Tensor/TensorConceptIO.hxx"
#include "TFEL/Math/Matrix/tmatrixIO.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/Abaqus/Abaqus.hxx"
#include "MFront/Abaqus/AbaqusComputeStiffnessTensor.hxx"

#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MTest/UmatNormaliseTangentOperator.hxx"
#include "MTest/AbaqusExplicitBehaviour.hxx"

namespace mtest {

  std::string AbaqusExplicitBehaviour::getHypothesisSuffix(const Hypothesis h) {
    if (h == ModellingHypothesis::AXISYMMETRICAL) {
      return "_AXIS";
    } else if (h == ModellingHypothesis::PLANESTRAIN) {
      return "_PSTRAIN";
    } else if (h == ModellingHypothesis::PLANESTRESS) {
      return "_PSTRESS";
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      return "_3D";
    }
    tfel::raise(
        "AbaqusExplicitBehaviour::getHypothesisSuffix: "
        "invalid hypothesis.");
  }  // end of AbaqusExplicitBehaviour::getHypothesisSuffix

  std::string AbaqusExplicitBehaviour::extractBehaviourName(
      const std::string& b, const Hypothesis h) {
    auto ends = [&b](const std::string& s) {
      if (b.length() >= s.length()) {
        return b.compare(b.length() - s.length(), s.length(), s) == 0;
      }
      return false;
    };
    const auto s = AbaqusExplicitBehaviour::getHypothesisSuffix(h);
    tfel::raise_if(!ends(s),
                   "AbaqusExplicitBehaviour::extractBehaviourName: "
                   "invalid function name.");
    return {b.begin(), b.begin() + b.length() - s.length()};
  }

  AbaqusExplicitBehaviour::AbaqusExplicitBehaviour(const Hypothesis h,
                                                   const std::string& l,
                                                   const std::string& b)
      : StandardBehaviourBase(
            h, l, AbaqusExplicitBehaviour::extractBehaviourName(b, h)) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c,
                     "AbaqusExplicitBehaviour::AbaqusExplicitBehaviour: " + m);
    };
    auto& elm =
        tfel::system::ExternalLibraryManager::getExternalLibraryManager();
    const auto bn = AbaqusExplicitBehaviour::extractBehaviourName(b, h);
    throw_if(elm.getInterface(l, bn) != "AbaqusExplicit",
             "invalid interface '" + elm.getInterface(l, bn) + "'");
    this->fct = elm.getAbaqusExplicitExternalBehaviourFunction(l, b);
    if (this->stype == 1u) {
      this->omp = elm.getAbaqusOrthotropyManagementPolicy(l, bn);
      if (this->omp == 2u) {
        auto aivs = std::vector<std::string>{};
        if ((h == ModellingHypothesis::PLANESTRESS) ||
            (h == ModellingHypothesis::PLANESTRAIN) ||
            (h == ModellingHypothesis::AXISYMMETRICAL) ||
            (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
          aivs = {"FirstOrthotropicDirection_1", "FirstOrthotropicDirection_2"};
        } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          aivs = {
              "FirstOrthotropicDirection_1",  "FirstOrthotropicDirection_2",
              "FirstOrthotropicDirection_3",  "SecondOrthotropicDirection_1",
              "SecondOrthotropicDirection_2", "SecondOrthotropicDirection_3"};
        } else {
          throw_if(true, "unsupported modelling hypothesis");
        }
        for (const auto& iv : aivs) {
          throw_if(std::find(this->ivnames.begin(), this->ivnames.end(), iv) !=
                       this->ivnames.end(),
                   iv + " is a reserved name");
          this->ivtypes.insert(this->ivtypes.begin(), 0);
        }
        this->ivnames.insert(this->ivnames.begin(), aivs.begin(), aivs.end());
      }
    }
    auto tmp = std::vector<std::string>{};
    tmp.emplace_back("MassDensity");
    if (this->etype == 0u) {
      if (this->requiresStiffnessTensor) {
        tmp.insert(tmp.end(), {"YoungModulus", "PoissonRatio"});
      }
      if (this->requiresThermalExpansionCoefficientTensor) {
        tmp.push_back("ThermalExpansion");
      }
    } else if (this->etype == 1u) {
      if ((h == ModellingHypothesis::PLANESTRESS) ||
          (h == ModellingHypothesis::PLANESTRAIN) ||
          (h == ModellingHypothesis::AXISYMMETRICAL) ||
          (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
        if (this->requiresStiffnessTensor) {
          tmp.insert(tmp.end(),
                     {"YoungModulus1", "YoungModulus2", "YoungModulus3",
                      "PoissonRatio12", "PoissonRatio23", "PoissonRatio13",
                      "ShearModulus12"});
        }
        if (this->requiresThermalExpansionCoefficientTensor) {
          tmp.insert(tmp.end(), {"ThermalExpansion1", "ThermalExpansion2",
                                 "ThermalExpansion3"});
        }
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        if (this->requiresStiffnessTensor) {
          tmp.insert(tmp.end(),
                     {"YoungModulus1", "YoungModulus2", "YoungModulus3",
                      "PoissonRatio12", "PoissonRatio23", "PoissonRatio13",
                      "ShearModulus12", "ShearModulus23", "ShearModulus13"});
        }
        if (this->requiresThermalExpansionCoefficientTensor) {
          tmp.insert(tmp.end(), {"ThermalExpansion1", "ThermalExpansion2",
                                 "ThermalExpansion3"});
        }
      } else {
        tfel::raise(
            "AbaqusExplicitBehaviour::AbaqusExplicitBehaviour : "
            "unsupported modelling hypothesis");
      }
    } else {
      tfel::raise(
          "AbaqusExplicitBehaviour::AbaqusExplicitBehaviour : "
          "unsupported behaviour type "
          "(neither isotropic nor orthotropic)");
    }
    this->mpnames.insert(this->mpnames.begin(), tmp.begin(), tmp.end());
  }

  void AbaqusExplicitBehaviour::getGradientsDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    std::fill(v.begin(), v.end(), real(0));
    v[0] = v[1] = v[2] = real(1);
  }

  tfel::math::tmatrix<3u, 3u, real> AbaqusExplicitBehaviour::getRotationMatrix(
      const tfel::math::vector<real>&,
      const tfel::math::tmatrix<3u, 3u, real>& r) const {
    return r;
  }  // end of AbaqusExplicitBehaviour::getRotationMatrix

  void AbaqusExplicitBehaviour::allocate(BehaviourWorkSpace& wk) const {
    const auto ndv = this->getGradientsSize();
    const auto nth = this->getThermodynamicForcesSize();
    wk.D.resize(nth, nth);
    wk.kt.resize(nth, ndv);
    wk.k.resize(nth, ndv);
    wk.mps.resize(this->mpnames.size() == 0 ? 1u : this->mpnames.size(),
                  real(0));
    wk.evs.resize(this->evnames.size());
    mtest::allocate(wk.cs, this->shared_from_this());
  }  // end of AbaqusExplicitBehaviour::allocate

  StiffnessMatrixType AbaqusExplicitBehaviour::getDefaultStiffnessMatrixType()
      const {
    return StiffnessMatrixType::ELASTIC;
  }

  bool AbaqusExplicitBehaviour::doPackagingStep(CurrentState& s,
                                                BehaviourWorkSpace& wk) const {
    using abaqus::AbaqusInt;
    using tfel::math::matrix;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "AbaqusExplicitBehaviour::doPackagingStep:" + m);
    };
    matrix<real> K;
    const auto h = this->getHypothesis();
    if ((h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::AXISYMMETRICAL) ||
        (h == ModellingHypothesis::PLANESTRAIN)) {
      K.resize(4u, 4u, 0);
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      K.resize(6u, 6u, 0);
    } else {
      throw_if(true, "unsupported hypothesis (" +
                         ModellingHypothesis::toString(h) + ")");
    }
    const AbaqusInt nblock = 1;
    const AbaqusInt ndir = [&h, &throw_if] {
      switch (h) {
        case ModellingHypothesis::PLANESTRESS:
          return 2;
        case ModellingHypothesis::AXISYMMETRICAL:
        case ModellingHypothesis::PLANESTRAIN:
        case ModellingHypothesis::TRIDIMENSIONAL:
          return 3;
      }
      throw_if(true, "unsupported hypothesis (" +
                         ModellingHypothesis::toString(h) + ")");
    }();
    const AbaqusInt nshr = [&h, &throw_if] {
      switch (h) {
        case ModellingHypothesis::PLANESTRESS:
        case ModellingHypothesis::AXISYMMETRICAL:
        case ModellingHypothesis::PLANESTRAIN:
          return 1;
        case ModellingHypothesis::TRIDIMENSIONAL:
          return 3;
      }
      throw_if(true, "unsupported hypothesis (" +
                         ModellingHypothesis::toString(h) + ")");
    }();
    const auto nprops = static_cast<AbaqusInt>(s.mprops1.size()) - 1;
    const auto nstatv = static_cast<AbaqusInt>(s.iv1.size());
    const auto nfieldv = static_cast<AbaqusInt>(s.esv0.size()) - 1;
    const auto density = s.mprops1[0];
    const real stepTime = 0;
    const real totalTime = 0;
    const real dt = 0;
    // using a local copy of material properties to handle the
    // case where s.mprops1 is empty
    copy(s.mprops1.begin() + 1, s.mprops1.end(), wk.mps.begin());
    if (s.mprops1.size() == 1) {
      wk.mps[0] = real(0);
    }
    for (decltype(wk.evs.size()) i = 0; i != wk.evs.size(); ++i) {
      wk.evs[i] = s.esv0(i) + s.desv(i);
    }
    if (this->omp == 2u) {
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
    const real stressOld[6u] = {0, 0, 0, 0, 0, 0};
    real stressNew[6u] = {0, 0, 0, 0, 0, 0};
    const real stretchOld[6u] = {1, 1, 1, 0, 0, 0};
    const real stretchNew[6u] = {1, 1, 1, 0, 0, 0};
    const real defgradOld[9u] = {1, 1, 1, 0, 0, 0, 0, 0, 0};
    const real defgradNew[9u] = {1, 1, 1, 0, 0, 0, 0, 0, 0};
    for (AbaqusInt i = 0; i != ndir + nshr; ++i) {
      real strainInc[6u] = {0, 0, 0, 0, 0, 0};
      strainInc[i] = 1;
      (this->fct)(&nblock, &ndir, &nshr, &nstatv, &nfieldv, &nprops, nullptr,
                  &stepTime, &totalTime, &dt, nullptr, nullptr, nullptr,
                  &(wk.mps[0]), &density, strainInc, nullptr, &(s.esv0[0]),
                  stretchOld, defgradOld,
                  s.esv0.size() == 1 ? nullptr : &(s.esv0[0]) + 1, stressOld,
                  nstatv == 0 ? nullptr : &(s.iv0[0]), &s.se0, &s.de0,
                  &(wk.evs[0]), stretchNew, defgradNew,
                  wk.evs.size() == 1 ? nullptr : &(wk.evs[0]) + 1, stressNew,
                  nstatv == 0 ? nullptr : &(s.iv1[0]), &s.se1, &s.de1, 0);
      if (h == ModellingHypothesis::PLANESTRESS) {
        const auto idx = (i == 2) ? 3 : i;
        K(0, idx) = stressNew[0];
        K(1, idx) = stressNew[1];
        K(2, idx) = 0;
        K(3, idx) = stressNew[3];
      } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::PLANESTRAIN)) {
        K(0, i) = stressNew[0];
        K(1, i) = stressNew[1];
        K(2, i) = stressNew[2];
        K(3, i) = stressNew[3];
      } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        K(0, i) = stressNew[0];
        K(1, i) = stressNew[1];
        K(2, i) = stressNew[2];
        K(3, i) = stressNew[3];
        K(4, i) = stressNew[5];
        K(5, i) = stressNew[4];
      } else {
        throw_if(true, "unsupported hypothesis (" +
                           ModellingHypothesis::toString(h) + ")");
      }
    }
    if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL1) {
      auto& log = mfront::getLogStream();
      log << "AbaqusExplicitBehaviour::doPackagingStep: stiffness matrix\n";
      for (matrix<real>::size_type i = 0; i != K.getNbRows(); ++i) {
        for (matrix<real>::size_type j = 0; j != K.getNbCols(); ++j) {
          log << K(i, j) << " ";
        }
        log << '\n';
      }
      log << "\n\n";
    }
    // convertion to TFEL conventions and storage in packaging
    // information
    const std::string n = "InitialElasticStiffness";
    s.packaging_info.insert({n, matrix<real>()});
    auto& m = s.packaging_info.at(n).get<matrix<real>>();
    const real cste = std::sqrt(real{2});
    if ((h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::AXISYMMETRICAL) ||
        (h == ModellingHypothesis::PLANESTRAIN)) {
      m.resize(4u, 5u);
      for (unsigned short i = 0; i != 4; ++i) {
        K(i, 3) /= 2 * cste;
      }
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      m.resize(6u, 9u);
      for (unsigned short i = 0; i != 6; ++i) {
        K(i, 3) /= 2 * cste;
        K(i, 4) /= 2 * cste;
        K(i, 5) /= 2 * cste;
      }
    } else {
      throw_if(true,
               "computePredictionOperator: "
               "no 'InitialElasticStiffness' found. "
               "Was the packaging step done ?");
    }
    if ((h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::AXISYMMETRICAL) ||
        (h == ModellingHypothesis::PLANESTRAIN)) {
      tfel::math::st2tost2<2, real> D;
      for (unsigned short i = 0; i != 4; ++i) {
        for (unsigned short j = 0; j != 4; ++j) {
          D(i, j) = K(i, j);
        }
      }
      const auto D2 = tfel::math::change_basis(D, tfel::math::transpose(s.r));
      for (unsigned short i = 0; i != 4; ++i) {
        for (unsigned short j = 0; j != 3; ++j) {
          m(i, j) = D2(i, j);
        }
        m(i, 3) = m(i, 4) = D2(i, 3);
      }
    } else {
      tfel::math::st2tost2<3, real> D;
      for (unsigned short i = 0; i != 6; ++i) {
        for (unsigned short j = 0; j != 6; ++j) {
          D(i, j) = K(i, j);
        }
      }
      const auto D2 = tfel::math::change_basis(D, tfel::math::transpose(s.r));
      for (unsigned short i = 0; i != 6; ++i) {
        for (unsigned short j = 0; j != 3; ++j) {
          m(i, j) = D2(i, j);
        }
        m(i, 3) = m(i, 4) = D2(i, 3);
        m(i, 5) = m(i, 6) = D2(i, 4);
        m(i, 7) = m(i, 8) = D2(i, 5);
      }
    }
    return true;
  }

  std::pair<bool, real> AbaqusExplicitBehaviour::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    if (ktype == StiffnessMatrixType::ELASTIC) {
      const auto p = s.packaging_info.find("InitialElasticStiffness");
      tfel::raise_if(p == s.packaging_info.end(),
                     "AbaqusExplicitBehaviour::"
                     "computePredictionOperator: "
                     "no 'InitialElasticStiffness' found. "
                     "Was the packaging step done ?");
      wk.kt = p->second.get<tfel::math::matrix<real>>();
      return {true, real{1}};
    }
    return {false, real(-1)};
  }

  std::pair<bool, real> AbaqusExplicitBehaviour::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    using namespace tfel::math;
    using abaqus::AbaqusInt;
    constexpr const auto sqrt2 = Cste<real>::sqrt2;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "AbaqusExplicitBehaviour::ointegrate:" + m);
    };
    if (ktype == StiffnessMatrixType::ELASTIC) {
      const auto p = s.packaging_info.find("InitialElasticStiffness");
      throw_if(p == s.packaging_info.end(),
               "no 'InitialElasticStiffness' found. "
               "Was the packaging step done ?");
      wk.k = p->second.get<tfel::math::matrix<real>>();
    } else {
      throw_if(true, "unsupported stiffness matrix type");
    }
    const AbaqusInt nblock = 1;
    const auto h = this->getHypothesis();
    const AbaqusInt ndir = 3;
    const AbaqusInt nshr = [&h, &throw_if] {
      switch (h) {
        case ModellingHypothesis::PLANESTRESS:
        case ModellingHypothesis::AXISYMMETRICAL:
        case ModellingHypothesis::PLANESTRAIN:
          return 1;
        case ModellingHypothesis::TRIDIMENSIONAL:
          return 3;
      }
      throw_if(true, "unsupported hypothesis (" +
                         ModellingHypothesis::toString(h) + ")");
    }();
    const auto nprops = static_cast<AbaqusInt>(s.mprops1.size()) - 1;
    const auto nstatv = static_cast<AbaqusInt>(s.iv0.size());
    const auto nfieldv = static_cast<AbaqusInt>(s.esv0.size()) - 1;
    const auto density = s.mprops1[0];
    const real stepTime = 1;
    const real totalTime = 1;
    // using a local copy of material properties to handle the
    // case where s.mprops1 is empty
    copy(s.mprops1.begin() + 1, s.mprops1.end(), wk.mps.begin());
    if (s.mprops1.size() == 1) {
      wk.mps[0] = real(0);
    }
    for (decltype(wk.evs.size()) i = 0; i != wk.evs.size(); ++i) {
      wk.evs[i] = s.esv0(i) + s.desv(i);
    }
    if (this->omp == 2u) {
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
    std::copy(s.iv0.begin(), s.iv0.end(), s.iv1.begin());
    const real strainInc[6u] = {0, 0, 0, 0, 0, 0};
    real stressOld[6u] = {0, 0, 0, 0, 0, 0};
    real stressNew[6u] = {0, 0, 0, 0, 0, 0};
    real stretchOld[6u] = {1, 1, 1, 0, 0, 0};
    real stretchNew[6u] = {1, 1, 1, 0, 0, 0};
    real defgradOld[9u] = {1, 1, 1, 0, 0, 0, 0, 0, 0};
    real defgradNew[9u] = {1, 1, 1, 0, 0, 0, 0, 0, 0};
    // rotation matrix from the polar decomposition
    tmatrix<3u, 3u, real> r1;
    if ((h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::AXISYMMETRICAL) ||
        (h == ModellingHypothesis::PLANESTRAIN)) {
      tensor<2u, real> F0(&s.e0[0]);
      tensor<2u, real> F1(&s.e1[0]);
      tensor<2u, real> R0;
      tensor<2u, real> R1;
      stensor<2u, real> U0;
      stensor<2u, real> U1;
      stensor<2u, real> s0(&(s.s0[0]));
      if (this->omp != 2u) {
        F0.changeBasis(s.r);
        F1.changeBasis(s.r);
        s0.changeBasis(s.r);
      }
      polar_decomposition(R0, U0, F0);
      polar_decomposition(R1, U1, F1);
      tfel::fsalgo::copy<5u>::exe(F0.begin(), defgradOld);
      tfel::fsalgo::copy<5u>::exe(F1.begin(), defgradNew);
      U0.exportTab(stretchOld);
      U1.exportTab(stretchNew);
      r1 = matrix_view(R1);
      s0.changeBasis(r1);
      s0.exportTab(stressOld);
      s0.exportTab(stressNew);
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      auto convert = [](real* aF, const tensor<3u, real>& F) {
        aF[0] = F[0];
        aF[1] = F[1];
        aF[2] = F[2];
        aF[3] = F[3];
        aF[4] = F[7];
        aF[5] = F[6];
        aF[6] = F[4];
        aF[7] = F[8];
        aF[8] = F[5];
      };
      tensor<3u, real> F0(&s.e0[0]);
      tensor<3u, real> F1(&s.e1[0]);
      tensor<3u, real> R0;
      tensor<3u, real> R1;
      stensor<3u, real> U0;
      stensor<3u, real> U1;
      stensor<3u, real> s0(&(s.s0[0]));
      if (this->omp != 2u) {
        F0.changeBasis(s.r);
        F1.changeBasis(s.r);
        s0.changeBasis(s.r);
      }
      polar_decomposition(R0, U0, F0);
      polar_decomposition(R1, U1, F1);
      convert(defgradOld, F0);
      convert(defgradNew, F1);
      U0.exportTab(stretchOld);
      U1.exportTab(stretchNew);
      std::swap(stretchOld[4], stretchOld[5]);
      std::swap(stretchNew[4], stretchNew[5]);
      r1 = matrix_view(R1);
      s0.changeBasis(r1);
      s0.exportTab(stressOld);
      s0.exportTab(stressNew);
      std::swap(stressOld[5], stressOld[4]);
      std::swap(stressNew[5], stressNew[4]);
    } else {
      throw_if(true,
               "unsupported hypothesis "
               "(" +
                   ModellingHypothesis::toString(h) + ")");
    }
    (this->fct)(
        &nblock, &ndir, &nshr, &nstatv, &nfieldv, &nprops, nullptr, &stepTime,
        &totalTime, &dt, nullptr, nullptr, nullptr, &(wk.mps[0]), &density,
        strainInc, nullptr, &(s.esv0[0]), stretchOld, defgradOld,
        s.esv0.size() == 1 ? nullptr : &(s.esv0[0]) + 1, stressOld,
        nstatv == 0 ? nullptr : &(s.iv0[0]), &s.se0, &s.de0, &(wk.evs[0]),
        stretchNew, defgradNew, wk.evs.size() == 1 ? nullptr : &(wk.evs[0]) + 1,
        stressNew, nstatv == 0 ? nullptr : &(s.iv1[0]), &s.se1, &s.de1, 0);
    if ((h == ModellingHypothesis::PLANESTRESS) ||
        (h == ModellingHypothesis::AXISYMMETRICAL) ||
        (h == ModellingHypothesis::PLANESTRAIN)) {
      stensor<2u, real> sig;
      if (h == ModellingHypothesis::PLANESTRESS) {
        sig[0] = stressNew[0];
        sig[1] = stressNew[1];
        sig[2] = 0;
        sig[3] = stressNew[3] * sqrt2;
      } else {
        sig[0] = stressNew[0];
        sig[1] = stressNew[1];
        sig[2] = stressNew[2];
        sig[3] = stressNew[3] * sqrt2;
      }
      sig.changeBasis(transpose(r1));
      if (this->omp != 2u) {
        sig.changeBasis(transpose(s.r));
      }
      tfel::fsalgo::copy<4u>::exe(sig.begin(), s.s1.begin());
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      stensor<3u, real> sig = {stressNew[0],         stressNew[1],
                               stressNew[2],         stressNew[3] * sqrt2,
                               stressNew[5] * sqrt2, stressNew[4] * sqrt2};
      sig.changeBasis(transpose(r1));
      if (this->omp != 2u) {
        sig.changeBasis(transpose(s.r));
      }
      tfel::fsalgo::copy<6u>::exe(sig.begin(), s.s1.begin());
    } else {
      throw_if(true,
               "unsupported hypothesis "
               "(" +
                   ModellingHypothesis::toString(h) + ")");
    }
    return {true, real(1)};
  }  // end of AbaqusExplicitBehaviour::integrate

  AbaqusExplicitBehaviour::~AbaqusExplicitBehaviour() = default;

}  // end of namespace mtest
