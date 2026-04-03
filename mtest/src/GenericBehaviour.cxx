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
#include <ostream>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/CurrentState.hxx"
#include "MTest/BehaviourWorkSpace.hxx"
#include "MFront/GenericBehaviour/BehaviourData.hxx"
#include "MTest/GenericBehaviour.hxx"

namespace mtest {

  template <unsigned short N>
  void convertToSecondPiolaKirchhoffStress(real* const Sv,
                                           const real* const sv,
                                           const real* const Fv) {
    tfel::math::StensorView<N, real> S(Sv);
    tfel::math::tensor<N, real> F;
    tfel::math::stensor<N, real> s;
    std::copy(Fv, Fv + F.size(), F.begin());
    std::copy(sv, sv + s.size(), s.begin());
    S = tfel::math::convertCauchyStressToSecondPiolaKirchhoffStress(s, F);
  }  // end of convertToSecondPiolaKirchhoffStress

  template <unsigned short N>
  void convertFromSecondPiolaKirchhoffStress(real* const sv,
                                             const real* const Sv,
                                             const real* const Fv) {
    tfel::math::StensorView<N, real> s(sv);
    tfel::math::tensor<N, real> F;
    tfel::math::stensor<N, real> S;
    std::copy(Fv, Fv + F.size(), F.begin());
    std::copy(Sv, Sv + S.size(), S.begin());
    s = tfel::math::convertSecondPiolaKirchhoffStressToCauchyStress(S, F);
  }  // end of convertFromSecondPiolaKirchhoffStress

  template <unsigned short N>
  void convertToFirstPiolaKirchhoffStress(real* const pkv,
                                          const real* const sv,
                                          const real* const Fv) {
    tfel::math::TensorView<N, real> pk(pkv);
    tfel::math::tensor<N, real> F;
    tfel::math::stensor<N, real> s;
    std::copy(Fv, Fv + F.size(), F.begin());
    std::copy(sv, sv + s.size(), s.begin());
    pk = tfel::math::convertCauchyStressToFirstPiolaKirchhoffStress(s, F);
  }  // end of convertToFirstPiolaKirchhoffStress

  template <unsigned short N>
  void convertFromFirstPiolaKirchhoffStress(real* const sv,
                                            const real* const pkv,
                                            const real* const Fv) {
    tfel::math::StensorView<N, real> s(sv);
    tfel::math::tensor<N, real> F;
    tfel::math::tensor<N, real> pk;
    std::copy(Fv, Fv + F.size(), F.begin());
    std::copy(pkv, pkv + pk.size(), pk.begin());
    s = tfel::math::convertFirstPiolaKirchhoffStressToCauchyStress(pk, F);
  }  // end of convertFromFirstPiolaKirchhoffStress

  template <unsigned short N>
  void convertFromDSDEGL(real* const K,
                         const real* const F0,
                         const real* const F1,
                         const real* const s) {
    using FSTOBase = tfel::material::FiniteStrainBehaviourTangentOperatorBase;
    tfel::math::st2tost2<N, real> Cse;
    std::copy(K, K + Cse.size(), Cse.begin());
    const auto Kv =
        tfel::material::convert<FSTOBase::DSIG_DF, FSTOBase::DS_DEGL>(
            Cse, tfel::math::tensor<N, real>(F0),
            tfel::math::tensor<N, real>(F1), tfel::math::stensor<N, real>(s));
    std::copy(Kv.begin(), Kv.end(), K);
  }  // end of convertFromDSDEGL

  template <unsigned short N>
  void convertFromDPK1DF(real* const K,
                         const real* const F0,
                         const real* const F1,
                         const real* const s) {
    using FSTOBase = tfel::material::FiniteStrainBehaviourTangentOperatorBase;
    tfel::math::t2tot2<N, real> dP;
    std::copy(K, K + dP.size(), dP.begin());
    const auto Kv =
        tfel::material::convert<FSTOBase::DSIG_DF, FSTOBase::DPK1_DF>(
            dP, tfel::math::tensor<N, real>(F0),
            tfel::math::tensor<N, real>(F1), tfel::math::stensor<N, real>(s));
    std::copy(Kv.begin(), Kv.end(), K);
  }  // end of convertFromDPK1DF

  template <unsigned short N>
  void convertFromDTAUDDF(real* const K,
                          const real* const F0,
                          const real* const F1,
                          const real* const s) {
    using FSTOBase = tfel::material::FiniteStrainBehaviourTangentOperatorBase;
    tfel::math::t2tost2<N, real> dtau_ddF;
    std::copy(K, K + dtau_ddF.size(), dtau_ddF.begin());
    const auto dtau_dF =
        tfel::material::convert<FSTOBase::DTAU_DF, FSTOBase::DTAU_DDF>(
            dtau_ddF, tfel::math::tensor<N, real>(F0),
            tfel::math::tensor<N, real>(F1), tfel::math::stensor<N, real>(s));
    const auto Kv =
        tfel::material::convert<FSTOBase::DSIG_DF, FSTOBase::DTAU_DF>(
            dtau_dF, tfel::math::tensor<N, real>(F0),
            tfel::math::tensor<N, real>(F1), tfel::math::stensor<N, real>(s));
    std::copy(Kv.begin(), Kv.end(), K);
  }  // end of convertFromDTAUDDF

  GenericBehaviour::GenericBehaviour(const Hypothesis h,
                                     const std::string& l,
                                     const std::string& b)
      : StandardBehaviourBase(h, l, b) {
    using tfel::system::ExternalLibraryManager;
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    const auto f = b + "_" + ModellingHypothesis::toString(h);
    this->fct = elm.getGenericBehaviourFunction(l, f);
    if (this->stype == 1u) {
      // load the rotation functions
      this->rg_fct = elm.getGenericBehaviourRotateGradientsFunction(
          l, f + "_rotateGradients");
      if (this->btype == 2u) {
        // finite strain behaviour
        this->rtf_fct =
            elm.getGenericBehaviourRotateThermodynamicForcesFunction(
                l, f + "_rotateThermodynamicForces_CauchyStress");
        this->rto_fct =
            elm.getGenericBehaviourRotateTangentOperatorBlocksFunction(
                l, f + "_rotateTangentOperatorBlocks_dsig_dF");
      } else {
        this->rtf_fct =
            elm.getGenericBehaviourRotateThermodynamicForcesFunction(
                l, f + "_rotateThermodynamicForces");
        this->rto_fct =
            elm.getGenericBehaviourRotateTangentOperatorBlocksFunction(
                l, f + "_rotateTangentOperatorBlocks");
      }
    }
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
  }  // end of GenericBehaviour

  GenericBehaviour::GenericBehaviour(const Hypothesis h,
                                     const std::string& l,
                                     const std::string& b,
                                     const ParametersMap& params)
      : GenericBehaviour(h, l, b) {
    if (params.empty()) {
      return;
    }
    tfel::raise_if(this->btype != 2u,
                   "GenericBehaviour::GenericBehaviour: "
                   "no parameter expected");
    for (const auto& p : params) {
      if ((p.first != "stress_measure") && (p.first != "tangent_operator")) {
        tfel::raise(
            "GenericBehaviour::GenericBehaviour: "
            "unexpected parameter '" +
            p.first + "'");
      }
      tfel::raise_if(!p.second.is<std::string>(),
                     "GenericBehaviour::GenericBehaviour: "
                     "unexpected type for parameter '" +
                         p.first + "'");
      if (p.first == "stress_measure") {
        const auto& ss = p.second.get<std::string>();
        if ((ss == "CAUCHY") || (ss == "CauchyStress")) {
        } else if ((ss == "PK2") || (ss == "SecondPiolaKirchoffStress")) {
          this->stress_measure = PK2;
        } else if ((ss == "PK1") || (ss == "FirstPiolaKirchoffStress")) {
          this->stress_measure = PK1;
        } else {
          tfel::raise(
              "GenericBehaviour::GenericBehaviour: "
              "unsupported stress measure'" +
              ss + "'");
        }
      } else {
        const auto& to = p.second.get<std::string>();
        if ((to == "DSIG_DF") || (to == "DCAUCHY_DF")) {
        } else if (to == "DS_DEGL") {
          this->fsto = DS_DEGL;
        } else if (to == "DPK1_DF") {
          this->fsto = DPK1_DF;
        } else if (to == "DTAU_DDF") {
          this->fsto = DTAU_DDF;
        } else {
          tfel::raise(
              "GenericBehaviour::GenericBehaviour: "
              "unsupported tangent operator '" +
              to + "'");
        }
      }
    }
  }  // end of GenericBehaviour

  void GenericBehaviour::allocateWorkSpace(BehaviourWorkSpace& wk) const {
    const auto ndv = this->getGradientsSize();
    const auto nth = this->getThermodynamicForcesSize();
    const auto nstatev = this->getInternalStateVariablesSize();
    const auto nevs = this->getExternalStateVariablesSize();
    if (this->btype == 2u) {
      // allocating an array large enough to store
      // the tangent operator and its conversion to
      // DSIG_DF
      if (this->fsto == DSIG_DF) {
        wk.D.resize(nth, ndv);
        wk.k.resize(nth, ndv);
        wk.kt.resize(nth, ndv);
      } else if (this->fsto == DS_DEGL) {
        wk.D.resize(nth, ndv);
        wk.k.resize(nth, ndv);
        wk.kt.resize(nth, ndv);
      } else if (this->fsto == DPK1_DF) {
        wk.D.resize(ndv, ndv);
        wk.k.resize(ndv, ndv);
        wk.kt.resize(ndv, ndv);
      } else if (this->fsto == DTAU_DDF) {
        wk.D.resize(nth, ndv);
        wk.k.resize(nth, ndv);
        wk.kt.resize(nth, ndv);
      } else {
        tfel::raise(
            "GenericBehaviour::allocate: "
            "unsupported tangent operator type");
      }
    } else if ((this->btype == 1u) || (btype == 3u)) {
      wk.D.resize(nth, ndv);
      wk.k.resize(nth, ndv);
      wk.kt.resize(nth, ndv);
    } else {
      using size_type = tfel::math::matrix<real>::size_type;
      constexpr auto one = size_type{1u};
      const auto endv = ndv == 0u ? one : ndv;
      const auto enth = nth == 0u ? one : nth;
      const auto tobs_size = [this] {
        const auto h = this->getHypothesis();
        auto s = size_type{};
        for (const auto& to : this->getTangentOperatorBlocks()) {
          const auto ptf =
              std::find(this->thnames.begin(), this->thnames.end(), to.first);
          const auto piv =
              std::find(this->ivnames.begin(), this->ivnames.end(), to.first);
          const auto pg =
              std::find(this->gnames.begin(), this->gnames.end(), to.second);
          const auto pev =
              std::find(this->evnames.begin(), this->evnames.end(), to.second);
          const auto to_ro = [&, this]() -> size_type {
            if (pev != this->evnames.end()) {
              return mtest::getVariableSize(
                  this->evtypes[pev - this->evnames.begin()], h);
            }
            if (pg == this->gnames.end()) {
              tfel::raise(
                  "GenericBehaviour::call_behaviour(1): "
                  "invalid tangent operator block ('" +
                  to.first + "'" + " vs '" + to.second + "')");
            }
            return mtest::getVariableSize(
                this->gtypes[pg - this->gnames.begin()], h);
          }();
          const auto to_co = [&, this]() -> size_type {
            if (piv != this->ivnames.end()) {
              return mtest::getVariableSize(
                  this->ivtypes[piv - this->ivnames.begin()], h);
            }
            if (ptf == this->thnames.end()) {
              tfel::raise(
                  "GenericBehaviour::call_behaviour(2): "
                  "invalid tangent operator block ('" +
                  to.first + "'" + " vs '" + to.second + "')");
            }
            return mtest::getVariableSize(
                this->thtypes[ptf - this->thnames.begin()], h);
          }();
          s += to_ro * to_co;
        }
        return s;
      }();
      wk.D.resize(std::max(tobs_size, one), 1u);
      wk.kt.resize(enth, endv);
      wk.k.resize(enth, endv);
    }
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
    this->allocateCurrentState(wk.cs);
    if (this->btype == 2u) {
      if (this->stress_measure == PK1) {
        wk.pk0.resize(ndv);
        wk.pk1.resize(ndv);
      } else if (this->stress_measure == PK2) {
        wk.S0.resize(ndv);
        wk.S1.resize(ndv);
      }
    }
  }  // end of allocateWorkSpace

  void GenericBehaviour::getGradientsDefaultInitialValues(
      tfel::math::vector<real>& v) const {
    std::fill(v.begin(), v.end(), real(0));
    if (this->btype == 2u) {
      // finite strain behaviour
      v[0] = v[1] = v[2] = real(1);
    }
  }  // end of setGradientsDefaultInitialValue

  std::pair<bool, real> GenericBehaviour::computePredictionOperator(
      BehaviourWorkSpace& wk,
      const CurrentState& s,
      const StiffnessMatrixType ktype) const {
    wk.cs = s;
    return this->call_behaviour(wk.kt, wk.cs, wk, real(1), ktype, false);
  }  // end of computePredictionOperator

  std::pair<bool, real> GenericBehaviour::integrate(
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype) const {
    return this->call_behaviour(wk.k, s, wk, dt, ktype, true);
  }  // end of integrate

  std::pair<bool, real> GenericBehaviour::call_behaviour(
      tfel::math::matrix<real>& Kt,
      CurrentState& s,
      BehaviourWorkSpace& wk,
      const real dt,
      const StiffnessMatrixType ktype,
      const bool b) const {
    using namespace tfel::math;
    using tfel::math::vector;
    using size_type = tfel::math::matrix<real>::size_type;
    char error_message[512];
    std::fill(error_message, error_message + 512, '\0');
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
             "temporary material properties vector was not allocated "
             "properly");
    throw_if(wk.ivs0.size() != s.iv0.size(),
             "temporary internal state variable vector was not allocated "
             "properly");
    throw_if(wk.ivs.size() != s.iv1.size(),
             "temporary internal state variable vector was not allocated "
             "properly");
    throw_if(wk.evs0.size() != s.esv0.size(),
             "temporary external state variable vector was not allocated "
             "properly");
    throw_if(wk.evs.size() != s.esv0.size(),
             "temporary external state variable vector was not allocated "
             "properly");
    auto rdt = real{1};
    const auto ir = tfel::math::transpose(s.r);
    if (this->btype == 2u) {
      if (this->fsto == DSIG_DF) {
        throw_if((wk.D.getNbRows() != Kt.getNbRows()) ||
                     (wk.D.getNbCols() != Kt.getNbCols()),
                 "the memory has not been allocated correctly");
      } else if (this->fsto == DS_DEGL) {
        const auto ndv = this->getGradientsSize();
        const auto nth = this->getThermodynamicForcesSize();
        throw_if((wk.D.getNbRows() != nth) || (wk.D.getNbCols() != ndv),
                 "the memory has not been allocated correctly");
      } else if (this->fsto == DPK1_DF) {
        const auto ndv = this->getGradientsSize();
        throw_if((wk.D.getNbRows() != ndv) || (wk.D.getNbCols() != ndv),
                 "the memory has not been allocated correctly");
      } else if (this->fsto == DTAU_DDF) {
        throw_if((wk.D.getNbRows() != Kt.getNbRows()) ||
                     (wk.D.getNbCols() != Kt.getNbCols()),
                 "the memory has not been allocated correctly");
      } else {
        throw_if(true, "unsupported tangent operator");
      }
    } else if ((this->btype == 1u) || (this->btype == 3u)) {
      throw_if(((wk.D.getNbRows() != Kt.getNbRows()) ||
                (wk.D.getNbCols() != Kt.getNbCols())),
               "the memory has not been allocated correctly");
    } else {
      throw_if(wk.D.getNbCols() != 1u,
               "the memory has not been allocated correctly");
    }
    std::fill(wk.D.begin(), wk.D.end(), 0.);
    mfront::gb::BehaviourData d;
    d.error_message = error_message;
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
      this->rg_fct(&(wk.e0[0]), &(wk.e0[0]), s.r.begin());
      this->rg_fct(&(wk.e1[0]), &(wk.e1[0]), s.r.begin());
      this->rtf_fct(&(wk.s0[0]), &(wk.s0[0]), ir.begin());
      d.s0.gradients = &(wk.e0[0]);
      d.s1.gradients = &(wk.e1[0]);
      d.s0.thermodynamic_forces = &(wk.s0[0]);
      d.s1.thermodynamic_forces = &s.s1[0];
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
    d.rdt = &rdt;
    // type of integration to be performed
    StandardBehaviourBase::initializeTangentOperator(wk.D, ktype, b);
    d.K = &(wk.D(0, 0));
    // saving the point the thermodynamic forces in the material frame
    auto* const s0 = d.s0.thermodynamic_forces;
    auto* const s1 = d.s1.thermodynamic_forces;
    if (this->btype == 2u) {
      // selecting the stress measure
      this->executeFiniteStrainBehaviourStressPreProcessing(wk, d);
      // choosing the type of stiffness matrix
      this->executeFiniteStrainBehaviourTangentOperatorPreProcessing(d, ktype);
    }
    // calling the behaviour
    const auto r = (this->fct)(&d);
    if (r != 1) {
      mfront::getLogStream() << error_message << '\n';
      return {false, rdt};
    }
    if (mfront::getVerboseMode() >= mfront::VERBOSE_DEBUG) {
      auto& log = mfront::getLogStream();
      log << "Consistent tangent operator returned by the behaviour:\n";
      const auto ndv = this->getGradientsSize();
      const auto nth = this->getThermodynamicForcesSize();
      for (size_type i = 0; i != ndv; ++i) {
        for (size_type j = 0; j != nth;) {
          log << *(&(wk.D(0, 0)) + i * nth + j);
          if (++j != nth) {
            log << " ";
          }
        }
        log << '\n';
      }
    }
    // turn back the gradients in the global frame
    if (stype == 1u) {
      // here we use the fact that d.s1.gradients is a pointer to
      // &(wk.e0[0])
      this->rg_fct(&(wk.e0[0]), &(wk.e0[0]), ir.begin());
      this->rg_fct(&(wk.e1[0]), &(wk.e1[0]), ir.begin());
    }
    if (b) {
      std::copy(wk.ivs.begin(), wk.ivs.end(), s.iv1.begin());
      if (this->btype == 2u) {
        d.s0.thermodynamic_forces = s0;
        d.s1.thermodynamic_forces = s1;
        this->executeFiniteStrainBehaviourStressPostProcessing(wk, d);
      }
    }
    // tangent operator (...)
    if (ktype != StiffnessMatrixType::NOSTIFFNESS) {
      const auto ndv = this->getGradientsSize();
      const auto nth = this->getThermodynamicForcesSize();
      if (this->btype == 2u) {
        this->executeFiniteStrainBehaviourTangentOperatorPostProcessing(d);
      }
      if (this->stype == 1u) {
        this->rto_fct(&(wk.D(0, 0)), &(wk.D(0, 0)), s.r.begin());
      }
      if (this->btype != 0u) {
        for (unsigned short i = 0; i != nth; ++i) {
          for (unsigned short j = 0; j != ndv; ++j) {
            Kt(i, j) = wk.D(i, j);
          }
        }
      } else {
        const auto h = this->getHypothesis();
        auto to_offset = size_type{};
        for (const auto& to : this->getTangentOperatorBlocks()) {
          auto getVariableOffset = [&h](const size_type pos,
                                        const std::vector<int>& types) {
            auto o = size_type{};
            for (size_type p = 0; p != pos; ++p) {
              o += mtest::getVariableSize(types[p], h);
            }
            return o;
          };
          const auto ptf =
              std::find(this->thnames.begin(), this->thnames.end(), to.first);
          const auto piv =
              std::find(this->ivnames.begin(), this->ivnames.end(), to.first);
          const auto pg =
              std::find(this->gnames.begin(), this->gnames.end(), to.second);
          const auto pev =
              std::find(this->evnames.begin(), this->evnames.end(), to.second);
          const auto to_ro = [&, this]() -> size_type {
            if (pev != this->evnames.end()) {
              return mtest::getVariableSize(
                  this->evtypes[pev - this->evnames.begin()], h);
            }
            if (pg == this->gnames.end()) {
              tfel::raise(
                  "GenericBehaviour::call_behaviour(1): "
                  "invalid tangent operator block ('" +
                  to.first + "'" + " vs '" + to.second + "')");
            }
            return mtest::getVariableSize(
                this->gtypes[pg - this->gnames.begin()], h);
          }();
          const auto to_co = [&, this]() -> size_type {
            if (piv != this->ivnames.end()) {
              return mtest::getVariableSize(
                  this->ivtypes[piv - this->ivnames.begin()], h);
            }
            if (ptf == this->thnames.end()) {
              tfel::raise(
                  "GenericBehaviour::call_behaviour(2): "
                  "invalid tangent operator block ('" +
                  to.first + "'" + " vs '" + to.second + "')");
            }
            return mtest::getVariableSize(
                this->thtypes[ptf - this->thnames.begin()], h);
          }();
          if ((ptf == this->thnames.end()) || (pg == this->gnames.end())) {
            to_offset += to_ro * to_co;
            continue;
          }
          const auto tfpos = ptf - this->thnames.begin();
          const auto gpos = pg - this->gnames.begin();
          const auto og = getVariableOffset(gpos, this->gtypes);
          const auto otf = getVariableOffset(tfpos, this->thtypes);
          const auto g_size = mtest::getVariableSize(this->gtypes[gpos], h);
          const auto th_size = mtest::getVariableSize(this->thtypes[tfpos], h);
          const auto p = wk.D.begin();
          for (size_type i = 0; i != th_size; ++i) {
            for (size_type j = 0; j != g_size; ++j) {
              Kt(otf + i, og + j) = p[to_offset + i * g_size + j];
            }
          }
          to_offset += to_ro * to_co;
        }
      }
      if (b && (this->stype == 1u)) {
        this->rtf_fct(&s.s1[0], &s.s1[0], s.r.begin());
      }
    }
    return {true, rdt};
  }  // end of call_behaviour

  void GenericBehaviour::executeFiniteStrainBehaviourStressPreProcessing(
      BehaviourWorkSpace& wk, mfront::gb::BehaviourData& d) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c,
                     "GenericBehaviour::"
                     "executeFiniteStrainBehaviourStressPreProcessing: " +
                         m);
    };
    if (this->stress_measure == CAUCHY) {
      d.K[1] = real(0);
    } else if (this->stress_measure == PK2) {
      const auto n = tfel::material::getSpaceDimension(this->getHypothesis());
      d.K[1] = real(1);
      if (n == 1u) {
        if (this->getHypothesis() ==
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) {
          tfel::raise(
              "GenericBehaviour::call_behaviour: "
              "using DS_DEGL in plane strain is not supported yet");
        }
        convertToSecondPiolaKirchhoffStress<1u>(
            &wk.S0[0], d.s0.thermodynamic_forces, d.s0.gradients);
      } else if (n == 2u) {
        if (this->getHypothesis() == ModellingHypothesis::PLANESTRESS) {
          throw_if(true, "using DS_DEGL in plane strain is not supported yet");
        }
        convertToSecondPiolaKirchhoffStress<2u>(
            &wk.S0[0], d.s0.thermodynamic_forces, d.s0.gradients);
      } else if (n == 3u) {
        convertToSecondPiolaKirchhoffStress<3u>(
            &wk.S0[0], d.s0.thermodynamic_forces, d.s0.gradients);
      } else {
        throw_if(true, "internal error, unsupported space dimension");
      }
      d.s0.thermodynamic_forces = &wk.S0[0];
      d.s1.thermodynamic_forces = &wk.S1[0];
    } else if (this->stress_measure == PK1) {
      const auto n = tfel::material::getSpaceDimension(this->getHypothesis());
      d.K[1] = real(2);
      if (n == 1u) {
        if (this->getHypothesis() ==
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) {
          tfel::raise(
              "GenericBehaviour::call_behaviour: "
              "using DS_DEGL in plane strain is not supported yet");
        }
        convertToFirstPiolaKirchhoffStress<1u>(
            &wk.pk0[0], d.s0.thermodynamic_forces, d.s0.gradients);
      } else if (n == 2u) {
        if (this->getHypothesis() == ModellingHypothesis::PLANESTRESS) {
          throw_if(true, "using DS_DEGL in plane strain is not supported yet");
        }
        convertToFirstPiolaKirchhoffStress<2u>(
            &wk.pk0[0], d.s0.thermodynamic_forces, d.s0.gradients);
      } else if (n == 3u) {
        convertToFirstPiolaKirchhoffStress<3u>(
            &wk.pk0[0], d.s0.thermodynamic_forces, d.s0.gradients);
      } else {
        throw_if(true, "internal error, unsupported space dimension");
      }
      d.s0.thermodynamic_forces = &wk.pk0[0];
      d.s1.thermodynamic_forces = &wk.pk1[0];
    } else {
      throw_if(true, "internal error, unexpected stress measure");
    }
  }  // end of executeFiniteStrainBehaviourStressPreProcessing

  void
  GenericBehaviour::executeFiniteStrainBehaviourTangentOperatorPreProcessing(
      mfront::gb::BehaviourData& d, const StiffnessMatrixType ktype) const {
    d.K[2] = real(0);
    if (ktype != StiffnessMatrixType::NOSTIFFNESS) {
      if (this->fsto == DSIG_DF) {
        d.K[2] = real(0);
      } else if (this->fsto == DS_DEGL) {
        d.K[2] = real(1);
      } else if (this->fsto == DPK1_DF) {
        d.K[2] = real(2);
      } else if (this->fsto == DTAU_DDF) {
        d.K[2] = real(3);
      } else {
        tfel::raise(
            "GenericBehaviour::"
            "executeFiniteStrainBehaviourTangentOperatorPreProcessing: "
            "internal error, unexpected tangent operator type");
      }
    }
  }  // end of executeFiniteStrainBehaviourTangentOperatorPreProcessing

  void GenericBehaviour::executeFiniteStrainBehaviourStressPostProcessing(
      BehaviourWorkSpace& wk, mfront::gb::BehaviourData& d) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c,
                     "GenericBehaviour::"
                     "executeFiniteStrainBehaviourStressPostProcessing: " +
                         m);
    };
    if (this->stress_measure == CAUCHY) {
    } else if (this->stress_measure == PK2) {
      const auto n = tfel::material::getSpaceDimension(this->getHypothesis());
      if (n == 1u) {
        if (this->getHypothesis() ==
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) {
          tfel::raise(
              "GenericBehaviour::call_behaviour: "
              "using DS_DEGL in plane strain is not supported yet");
        }
        convertFromSecondPiolaKirchhoffStress<1u>(d.s1.thermodynamic_forces,
                                                  &wk.S1[0], d.s1.gradients);
      } else if (n == 2u) {
        if (this->getHypothesis() == ModellingHypothesis::PLANESTRESS) {
          throw_if(true, "using DS_DEGL in plane strain is not supported yet");
        }
        convertFromSecondPiolaKirchhoffStress<2u>(d.s1.thermodynamic_forces,
                                                  &wk.S1[0], d.s1.gradients);
      } else if (n == 3u) {
        convertFromSecondPiolaKirchhoffStress<3u>(d.s1.thermodynamic_forces,
                                                  &wk.S1[0], d.s1.gradients);
      } else {
        throw_if(true, "internal error, unsupported space dimension");
      }
    } else if (this->stress_measure == PK1) {
      const auto n = tfel::material::getSpaceDimension(this->getHypothesis());
      if (n == 1u) {
        if (this->getHypothesis() ==
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) {
          tfel::raise(
              "GenericBehaviour::call_behaviour: "
              "using DS_DEGL in plane strain is not supported yet");
        }
        convertFromFirstPiolaKirchhoffStress<1u>(d.s1.thermodynamic_forces,
                                                 &wk.pk1[0], d.s1.gradients);
      } else if (n == 2u) {
        if (this->getHypothesis() == ModellingHypothesis::PLANESTRESS) {
          throw_if(true, "using DS_DEGL in plane strain is not supported yet");
        }
        convertFromFirstPiolaKirchhoffStress<2u>(d.s1.thermodynamic_forces,
                                                 &wk.pk1[0], d.s1.gradients);
      } else if (n == 3u) {
        convertFromFirstPiolaKirchhoffStress<3u>(d.s1.thermodynamic_forces,
                                                 &wk.pk1[0], d.s1.gradients);
      } else {
        throw_if(true, "internal error, unsupported space dimension");
      }
    } else {
      throw_if(true, "internal error, unexpected stress measure");
    }
  }  // end of
     // GenericBehaviour::executeFiniteStrainBehaviourStressPostProcessing

  void
  GenericBehaviour::executeFiniteStrainBehaviourTangentOperatorPostProcessing(
      mfront::gb::BehaviourData& d) const {
    if (this->fsto == DSIG_DF) {
      // nothing to be done
    } else if (this->fsto == DS_DEGL) {
      const auto n = tfel::material::getSpaceDimension(this->getHypothesis());
      if (n == 1u) {
        if (this->getHypothesis() ==
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) {
          tfel::raise(
              "GenericBehaviour::call_behaviour: "
              "using DS_DEGL in plane stress is not supported yet");
        } else {
          convertFromDSDEGL<1u>(d.K, d.s0.gradients, d.s1.gradients,
                                d.s1.thermodynamic_forces);
        }
      } else if (n == 2u) {
        if (this->getHypothesis() == ModellingHypothesis::PLANESTRESS) {
          tfel::raise(
              "GenericBehaviour::call_behaviour: "
              "using DS_DEGL in plane stress is not supported yet");
        } else {
          convertFromDSDEGL<2u>(d.K, d.s0.gradients, d.s1.gradients,
                                d.s1.thermodynamic_forces);
        }
      } else if (n == 3u) {
        convertFromDSDEGL<3u>(d.K, d.s0.gradients, d.s1.gradients,
                              d.s1.thermodynamic_forces);
      } else {
        tfel::raise(
            "GenericBehaviour::call_behaviour: "
            "invalid space dimensions");
      }
    } else if (this->fsto == DPK1_DF) {
      const auto n = tfel::material::getSpaceDimension(this->getHypothesis());
      if (n == 1u) {
        if (this->getHypothesis() ==
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) {
          tfel::raise(
              "GenericBehaviour::call_behaviour: "
              "using DPK1_DG in plane stress is not supported yet");
        } else {
          convertFromDPK1DF<1u>(d.K, d.s0.gradients, d.s1.gradients,
                                d.s1.thermodynamic_forces);
        }
      } else if (n == 2u) {
        if (this->getHypothesis() == ModellingHypothesis::PLANESTRESS) {
          tfel::raise(
              "GenericBehaviour::call_behaviour: "
              "using DS_DEGL in plane stress is not supported yet");
        } else {
          convertFromDPK1DF<2u>(d.K, d.s0.gradients, d.s1.gradients,
                                d.s1.thermodynamic_forces);
        }
      } else if (n == 3u) {
        convertFromDPK1DF<3u>(d.K, d.s0.gradients, d.s1.gradients,
                              d.s1.thermodynamic_forces);
      } else {
        tfel::raise(
            "GenericBehaviour::call_behaviour: "
            "invalid space dimensions");
      }
    } else if (this->fsto == DTAU_DDF) {
      const auto n = tfel::material::getSpaceDimension(this->getHypothesis());
      if (n == 1u) {
        if (this->getHypothesis() ==
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) {
          tfel::raise(
              "GenericBehaviour::call_behaviour: "
              "using DPK1_DG in plane stress is not supported yet");
        } else {
          convertFromDTAUDDF<1u>(d.K, d.s0.gradients, d.s1.gradients,
                                 d.s1.thermodynamic_forces);
        }
      } else if (n == 2u) {
        if (this->getHypothesis() == ModellingHypothesis::PLANESTRESS) {
          tfel::raise(
              "GenericBehaviour::call_behaviour: "
              "using DS_DEGL in plane stress is not supported yet");
        } else {
          convertFromDTAUDDF<2u>(d.K, d.s0.gradients, d.s1.gradients,
                                 d.s1.thermodynamic_forces);
        }
      } else if (n == 3u) {
        convertFromDTAUDDF<3u>(d.K, d.s0.gradients, d.s1.gradients,
                               d.s1.thermodynamic_forces);
      } else {
        tfel::raise(
            "GenericBehaviour::call_behaviour: "
            "invalid space dimensions");
      }
    } else {
      tfel::raise(
          "GenericBehaviour::call_behaviour: "
          "internal error, unexpected tangent operator type");
    }
  }  // end of executeFiniteStrainBehaviourTangentOperatorPostProcessing

  tfel::math::tmatrix<3u, 3u, real> GenericBehaviour::getRotationMatrix(
      const tfel::math::vector<real>&,
      const tfel::math::tmatrix<3u, 3u, real>& r) const {
    return r;
  }  // end of getRotationMatrix

  std::vector<std::string> GenericBehaviour::getOptionalMaterialProperties()
      const {
    return {};
  }  // end of getOptionalMaterialProperties

  void GenericBehaviour::setOptionalMaterialPropertiesDefaultValues(
      EvolutionManager&, const EvolutionManager&) const {
  }  // end of setOptionalMaterialPropertiesDefaultValues

  StiffnessMatrixType GenericBehaviour::getDefaultStiffnessMatrixType() const {
    return StiffnessMatrixType::CONSISTENTTANGENTOPERATOR;
  }  // end of getDefaultStiffnessMatrixType

  GenericBehaviour::~GenericBehaviour() = default;

}  // end of namespace mtest
