/*!
 * \file   mtest/src/GenericBehaviour.cxx
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
#include <limits>
#include <cstring>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/ST2toST2/ST2toST2View.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MFront/GenericBehaviour/BehaviourData.hxx"
#include "MTest/GenericBehaviour.hxx"

namespace mtest {

  static void applyRotation(real* const v,
                            const std::vector<int>& types,
                            const GenericBehaviour::Hypothesis h,
                            const tfel::math::tmatrix<3u, 3u, real>& r) {
    auto o = size_t{};
    const auto n = tfel::material::getSpaceDimension(h);
    for (const auto& type : types) {
      if (type == 0) {
        o += 1;
      } else if (type == 1) {
        if (n == 2u) {
          tfel::math::stensor<2u, real> s(v + o);
          const auto rs = change_basis(s, r);
          tfel::fsalgo::copy<4u>::exe(rs.begin(), v + o);
        } else if (n == 3u) {
          tfel::math::stensor<3u, real> s(v + o);
          const auto rs = change_basis(s, r);
          tfel::fsalgo::copy<6u>::exe(rs.begin(), v + o);
        }
        o += tfel::material::getStensorSize(h);
      } else if (type == 2) {
        tfel::raise("applyRotation: vector are not supported yet");
        o += tfel::material::getSpaceDimension(h);
      } else if (type == 3) {
        if (n == 2u) {
          tfel::math::tensor<2u, real> t(v + o);
          const auto rt = change_basis(t, r);
          tfel::fsalgo::copy<5u>::exe(rt.begin(), v + o);
        } else if (n == 3u) {
          tfel::math::tensor<3u, real> t(v + o);
          const auto rt = change_basis(t, r);
          tfel::fsalgo::copy<9u>::exe(rt.begin(), v + o);
        }
        o += tfel::material::getTensorSize(h);
      }
    }
  }  // end of applyRotation

  static void applyRotation(real* const v,
                            const std::vector<int>& dvtypes,
                            const std::vector<int>& thtypes,
                            const GenericBehaviour::Hypothesis h,
                            const tfel::math::tmatrix<3u, 3u, real>& r) {
    auto o = size_t{};
    const auto n = tfel::material::getSpaceDimension(h);
    tfel::raise_if(dvtypes.size() != thtypes.size(),
                   "applyRotation: the number of driving variables "
                   "does not match the number of thermodynamic fores");
    tfel::raise_if(dvtypes.size() != 1u, "applyRotation: unsupported case");
    for (decltype(dvtypes.size()) i = 0; i != dvtypes.size(); ++i) {
      if (dvtypes[i] == 1) {
        // symmetric tensors
        tfel::raise_if(thtypes[i] != 1,
                       "applyRotation: "
                       "unsupported case");
        if (n == 2u) {
          tfel::math::st2tost2<2u, real> k;
          std::copy(v + o, v + o + k.size(), k.begin());
          const auto rk = change_basis(k, r);
          std::copy(rk.begin(), rk.end(), v + o);
        } else if (n == 3u) {
          tfel::math::st2tost2<3u, real> k;
          std::copy(v + o, v + o + k.size(), k.begin());
          const auto rk = change_basis(k, r);
          std::copy(rk.begin(), rk.end(), v + o);
        }
      } else if (dvtypes[i] == 2) {
        tfel::raise_if(dvtypes[i] != 1,
                       "applyRotation: "
                       "unsupported case");
        if (n == 2u) {
          tfel::math::t2tost2<2u, real> k;
          std::copy(v + o, v + o + k.size(), k.begin());
          const auto rk = change_basis(k, r);
          std::copy(rk.begin(), rk.end(), v + o);
        } else if (n == 3u) {
          tfel::math::t2tost2<3u, real> k;
          std::copy(v + o, v + o + k.size(), k.begin());
          const auto rk = change_basis(k, r);
          std::copy(rk.begin(), rk.end(), v + o);
        }
      } else {
        tfel::raise(
            "applyRotation: "
            "unsupported driving variable type");
      }
    }
  }  // end of applyRotation

  GenericBehaviour::GenericBehaviour(const Hypothesis h,
                                     const std::string& l,
                                     const std::string& b)
      : StandardBehaviourBase(h, l, b) {
    using tfel::system::ExternalLibraryManager;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    const auto f = b + "_" + ModellingHypothesis::toString(h);
    this->fct = elm.getGenericBehaviourFunction(l, f);
    // additional material properties to compute the elastic stiffness
    auto mps = std::vector<std::string>{};
    if (this->requiresStiffnessTensor) {
      if (this->etype == 0u) {
        mps.insert(mps.end(), {"YoungModulus", "PoissonRatio"});
      } else if (this->etype == 1u) {
        if ((h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) ||
            (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
          mps.insert(mps.end(),
                     {"YoungModulus1", "YoungModulus2", "YoungModulus3",
                      "PoissonRatio12", "PoissonRatio23", "PoissonRatio13"});
        } else if ((h == ModellingHypothesis::PLANESTRESS) ||
                   (h == ModellingHypothesis::PLANESTRAIN) ||
                   (h == ModellingHypothesis::AXISYMMETRICAL) ||
                   (h == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
          mps.insert(mps.end(),
                     {"YoungModulus1", "YoungModulus2", "YoungModulus3",
                      "PoissonRatio12", "PoissonRatio23", "PoissonRatio13",
                      "ShearModulus12"});
        } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          mps.insert(mps.end(),
                     {"YoungModulus1", "YoungModulus2", "YoungModulus3",
                      "PoissonRatio12", "PoissonRatio23", "PoissonRatio13",
                      "ShearModulus12", "ShearModulus23", "ShearModulus13"});
        } else {
          tfel::raise(
              "GenericBehaviour::GenericBehaviour : "
              "unsupported modelling hypothesis");
        }
      } else {
        tfel::raise(
            "GenericBehaviour::GenericBehaviour : "
            "unsupported symmetry type "
            "(neither isotropic nor orthotropic)");
      }
    }
    if (this->requiresThermalExpansionCoefficientTensor) {
      if (this->stype == 0u) {
        mps.push_back("ThermalExpansion");
      } else if (this->stype == 1u) {
        mps.insert(mps.end(), {"ThermalExpansion1", "ThermalExpansion2",
                               "ThermalExpansion3"});
      } else {
        tfel::raise(
            "GenericBehaviour::GenericBehaviour : "
            "unsupported symmetry type "
            "(neither isotropic nor orthotropic)");
      }
    }
    this->mpnames.insert(this->mpnames.begin(), mps.begin(), mps.end());
  }  // end of GenericBehaviour::GenericBehaviour

  void GenericBehaviour::allocate(BehaviourWorkSpace& wk) const {
    const auto ndv = this->getGradientsSize();
    const auto nth = this->getThermodynamicForcesSize();
    const auto nstatev = this->getInternalStateVariablesSize();
    const auto nevs = this->getExternalStateVariablesSize();
    wk.D.resize(nth, ndv);
    wk.k.resize(nth, ndv);
    wk.kt.resize(nth, ndv);
    wk.nk.resize(nth, ndv);
    wk.ne.resize(ndv);
    wk.ns.resize(nth);
    wk.mps.resize(this->getMaterialPropertiesSize());
    wk.ivs0.resize(nstatev);
    wk.ivs.resize(nstatev);
    wk.nivs.resize(nstatev);
    wk.evs0.resize(nevs);
    wk.evs.resize(nevs);
    if ((this->stype == 1u) || (this->btype == 1u)) {
      wk.e0.resize(ndv);
      wk.e1.resize(ndv);
      wk.s0.resize(nth);
    }
    mtest::allocate(wk.cs, this->shared_from_this());
  }  // end f GenericBehaviour::allocate

  void GenericBehaviour::getGradientsDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    std::fill(v.begin(), v.end(), real(0));
    if (this->btype == 2u) {
      // finite strain behaviour
      v[0] = v[1] = v[2] = real(1);
    }
  }  // end of GenericBehaviour::setGradientsDefaultInitialValue

  std::pair<bool, real> GenericBehaviour::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    wk.cs = s;
    return this->call_behaviour(wk.kt, wk.cs, wk, real(1), ktype, false);
  }  // end of GenericBehaviour::computePredictionOperator

  std::pair<bool, real> GenericBehaviour::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    return this->call_behaviour(wk.k, s, wk, dt, ktype, true);
  }  // end of GenericBehaviour::integrate

  std::pair<bool, real> GenericBehaviour::call_behaviour(
      tfel::math::matrix<real>& Kt,
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype,
      const bool b) const {
    using namespace std;
    using namespace tfel::math;
    using tfel::math::vector;
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "GenericBehaviour::call_behaviour: " + m);
    };
    auto init_ptr = [](vector<real>& t, const vector<real>& v) -> real* {
      if (v.empty()) {
        return nullptr;
      }
      t = v;
      return &t[0];
    };
    throw_if(wk.mps.size() != s.mprops1.size(),
             "temporary material properties vector was not allocated properly");
    throw_if(
        wk.ivs0.size() != s.iv0.size(),
        "temporary internal state variable vector was not allocated properly");
    throw_if(
        wk.ivs.size() != s.iv1.size(),
        "temporary internal state variable vector was not allocated properly");
    throw_if(
        wk.evs0.size() != s.esv0.size(),
        "temporary external state variable vector was not allocated properly");
    throw_if(
        wk.evs.size() != s.esv0.size(),
        "temporary external state variable vector was not allocated properly");
    throw_if((wk.D.getNbRows() != Kt.getNbRows()) ||
                 (wk.D.getNbCols() != Kt.getNbCols()),
             "the memory has not been allocated correctly");
    std::fill(wk.D.begin(), wk.D.end(), 0.);
    mfront::gb::BehaviourData d;
    if (this->stype == 1u) {
      // orthotropic behaviour
      std::copy(s.e0.begin(), s.e0.end(), wk.e0.begin());
      std::copy(s.e1.begin(), s.e1.end(), wk.e1.begin());
      std::copy(s.s0.begin(), s.s0.end(), wk.s0.begin());
      if (this->btype == 1u) {
        // small strain behaviour
        for (decltype(s.e1.size()) i = 0; i != s.e1.size(); ++i) {
          wk.e0(i) -= s.e_th0(i);
          wk.e1(i) -= s.e_th1(i);
        }
      }
      d.s0.gradients = &(wk.e0[0]);
      d.s1.gradients = &(wk.e1[0]);
      d.s0.thermodynamic_forces = &(wk.s0[0]);
      d.s1.thermodynamic_forces = &s.s1[0];
      applyRotation(d.s0.gradients, this->dvtypes, this->getHypothesis(), s.r);
      applyRotation(d.s1.gradients, this->dvtypes, this->getHypothesis(), s.r);
      applyRotation(d.s0.thermodynamic_forces, this->thtypes,
                    this->getHypothesis(), s.r);
    } else {
      if (this->btype == 1u) {
        // small strain behaviour
        std::copy(s.e0.begin(), s.e0.end(), wk.e0.begin());
        std::copy(s.e1.begin(), s.e1.end(), wk.e1.begin());
        for (decltype(s.e1.size()) i = 0; i != s.e1.size(); ++i) {
          wk.e0(i) -= s.e_th0(i);
          wk.e1(i) -= s.e_th1(i);
        }
        d.s0.gradients = &(wk.e0[0]);
        d.s1.gradients = &(wk.e1[0]);
      } else {
        d.s0.gradients = &(s.e0[0]);
        d.s1.gradients = &(s.e1[0]);
      }
      d.s0.thermodynamic_forces = &s.s0[0];
      d.s1.thermodynamic_forces = &s.s1[0];
    }
    d.s0.material_properties = init_ptr(wk.mps, s.mprops1);
    d.s1.material_properties = d.s0.material_properties;
    d.s0.internal_state_variables = init_ptr(wk.ivs0, s.iv0);
    d.s1.internal_state_variables = init_ptr(wk.ivs, s.iv1);
    d.s0.stored_energy = &s.se0;
    d.s1.stored_energy = &s.se1;
    d.s0.dissipated_energy = &s.de0;
    d.s1.dissipated_energy = &s.de1;
    d.s0.external_state_variables = init_ptr(wk.evs0, s.esv0);
    if (!s.esv0.empty()) {
      for (decltype(s.esv0.size()) i = 0; i != s.esv0.size(); ++i) {
        wk.evs[i] = s.esv0[i] + s.desv[i];
      }
      d.s1.external_state_variables = &wk.evs[0];
    } else {
      d.s1.external_state_variables = nullptr;
    }
    d.dt = dt;
    d.rdt = 1;
    // choosing the type of stiffness matrix
    StandardBehaviourBase::initializeTangentOperator(wk.D, ktype, b);
    d.K = &(wk.D(0, 0));
    // calling the behaviour
    const auto r = (this->fct)(&d);
    if (r != 1) {
      return {false, d.rdt};
    }
    // tangent operator (...)
    if (ktype != StiffnessMatrixType::NOSTIFFNESS) {
      const auto ndv = this->getGradientsSize();
      const auto nth = this->getThermodynamicForcesSize();
      if (this->stype == 1u) {
        if ((this->btype == 1u) || (this->btype == 2u)) {
          applyRotation(&(wk.D(0, 0)), this->dvtypes, this->thtypes,
                        this->getHypothesis(), transpose(s.r));
        } else {
          tfel::raise(
              "GenericBehaviour::call_behaviour: "
              "orthotropic behaviours are only "
              "supported for small or finite strain behaviours");
        }
      }
      for (unsigned short i = 0; i != nth; ++i) {
        for (unsigned short j = 0; j != ndv; ++j) {
          Kt(i, j) = wk.D(i, j);
        }
      }
    }
    if (b) {
      if (stype == 1u) {
        applyRotation(d.s1.thermodynamic_forces, this->thtypes,
                      this->getHypothesis(), transpose(s.r));
      }
      std::copy(wk.ivs.begin(), wk.ivs.end(), s.iv1.begin());
    }
    return {true, d.rdt};
  }  // end of GenericBehaviour::call_behaviour

  tfel::math::tmatrix<3u, 3u, real> GenericBehaviour::getRotationMatrix(
      const tfel::math::vector<real>&,
      const tfel::math::tmatrix<3u, 3u, real>& r) const {
    return r;
  }  // end of GenericBehaviour::getRotationMatrix

  void GenericBehaviour::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager&, const EvolutionManager&) const {
  }  // end of GenericBehaviour::setOptionalMaterialPropertiesDefaultValues

  StiffnessMatrixType GenericBehaviour::getDefaultStiffnessMatrixType() const {
    return StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }  // end of GenericBehaviour::getDefaultStiffnessMatrixType

  GenericBehaviour::~GenericBehaviour() = default;

}  // end of namespace mtest
