/*!
 * \file   CurrentState.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   23 oct. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Math/stensor.hxx"

#include "MTest/Evolution.hxx"
#include "MTest/Behaviour.hxx"
#include "MTest/CurrentState.hxx"

namespace mtest {

  CurrentState::CurrentState() = default;

  CurrentState::CurrentState(const CurrentState&) = default;

  CurrentState::CurrentState(CurrentState&&) = default;

  CurrentState& CurrentState::operator=(const CurrentState&) = default;

  CurrentState& CurrentState::operator=(CurrentState&&) = default;

  CurrentState::~CurrentState() noexcept = default;

  void allocate(CurrentState& s, const std::shared_ptr<const Behaviour>& b) {
    if (s.behaviour.get() != nullptr) {
      throw(std::runtime_error("mtest::allocate: state already allocated"));
    }
    s.behaviour = b;
    const auto mpnames = s.behaviour->getMaterialPropertiesNames();
    const auto esvnames = s.behaviour->getExternalStateVariablesNames();
    // clear
    s.s_1.clear();
    s.s0.clear();
    s.s1.clear();
    s.e0.clear();
    s.e1.clear();
    s.e_th0.clear();
    s.e_th1.clear();
    s.mprops1.clear();
    s.iv_1.clear();
    s.iv0.clear();
    s.iv1.clear();
    s.esv0.clear();
    s.desv.clear();
    // resizing
    const auto ndv = b->getDrivingVariablesSize();
    const auto nth = b->getThermodynamicForcesSize();
    s.s_1.resize(nth, 0.);
    s.s0.resize(nth, 0.);
    s.s1.resize(nth, 0.);
    s.e0.resize(ndv, 0.);
    s.e1.resize(ndv, 0.);
    s.e_th0.resize(ndv, 0.);
    s.e_th1.resize(ndv, 0.);
    s.mprops1.resize(mpnames.size());
    s.iv_1.resize(b->getInternalStateVariablesSize(), 0.);
    s.iv0.resize(s.iv_1.size(), 0.);
    s.iv1.resize(s.iv0.size(), 0.);
    s.esv0.resize(esvnames.size(), 0.);
    s.desv.resize(esvnames.size(), 0.);
  }

  void computeMaterialProperties(CurrentState& s,
                                 const EvolutionManager& evm,
                                 const EvolutionManager& dvm,
                                 const std::vector<std::string>& mpnames,
                                 const real t,
                                 const real dt) {
    if (s.behaviour.get() == nullptr) {
      throw(
          std::runtime_error("mtest::computeMaterialProperties: "
                             "uninitialised state"));
    }
    if (s.mprops1.size() != mpnames.size()) {
      throw(
          std::runtime_error("computeMaterialProperties:"
                             "CurrentState variable was not "
                             "initialized appropriately"));
    }
    std::vector<real>::size_type i = 0;
    for (const auto& mpn : mpnames) {
      auto pev = evm.find(mpn);
      if (pev != evm.end()) {
        const auto& ev = *(pev->second);
        s.mprops1[i] = ev(t + dt);
      } else {
        pev = dvm.find(mpn);
        if (pev != dvm.end()) {
          const auto& ev = *(pev->second);
          s.mprops1[i] = ev(t + dt);
        } else {
          throw(
              std::runtime_error("computeMaterialProperties: "
                                 "no evolution named '" +
                                 mpn + "'"));
        }
      }
      ++i;
    }
  }  // end of computeMaterialProperties

  void computeExternalStateVariables(CurrentState& s,
                                     const EvolutionManager& evm,
                                     const std::vector<std::string>& esvnames,
                                     const real t,
                                     const real dt) {
    if (s.behaviour.get() == nullptr) {
      throw(
          std::runtime_error("mtest::computeThermalExpanstion: "
                             "uninitialised state"));
    }
    if ((s.esv0.size() != esvnames.size()) ||
        (s.desv.size() != esvnames.size())) {
      throw(
          std::runtime_error("computeExternalStateVariables:"
                             "CurrentState variable was not "
                             "initialized appropriately"));
    }
    std::vector<real>::size_type i = 0;
    for (const auto& evn : esvnames) {
      auto pev = evm.find(evn);
      if (pev == evm.end()) {
        throw(
            std::runtime_error("computeExternalStateVariables:"
                               "no evolution named '" +
                               evn + "'"));
      }
      const auto& ev = *(pev->second);
      s.esv0[i] = ev(t);
      s.desv[i] = ev(t + dt) - s.esv0[i];
      ++i;
    }
  }  // end of computeExternalStateVariables

  void computeThermalExpansion(CurrentState& s,
                               const EvolutionManager& evm,
                               const real t,
                               const real dt) {
    static const std::string T{"Temperature"};
    static const std::string a{"ThermalExpansion"};
    if (s.behaviour.get() == nullptr) {
      throw(
          std::runtime_error("mtest::computeThermalExpanstion: "
                             "uninitialised state"));
    }
    auto pev = evm.find(T);
    auto pev2 = evm.find(a);
    if ((pev == evm.end()) || (pev2 == evm.end())) {
      return;
    }
    const auto& T_ev = *(pev->second);
    const auto& a_ev = *(pev2->second);
    const auto eth0 = a_ev(t) * (T_ev(t) - s.Tref);
    const auto eth1 = a_ev(t + dt) * (T_ev(t + dt) - s.Tref);
    for (unsigned short i = 0; i != 3; ++i) {
      s.e_th0[i] = eth0;
      s.e_th1[i] = eth1;
    }
  }

  void computeThermalExpansion(CurrentState& s,
                               const EvolutionManager& evm,
                               const real t,
                               const real dt,
                               const unsigned short d) {
    static const std::string T{"Temperature"};
    static const std::string a1{"ThermalExpansion1"};
    static const std::string a2{"ThermalExpansion2"};
    static const std::string a3{"ThermalExpansion3"};
    if (s.behaviour.get() == nullptr) {
      throw(
          std::runtime_error("mtest::computeThermalExpanstion: "
                             "uninitialised state"));
    }
    const auto pev = evm.find(T);
    const auto pev2 = evm.find(a1);
    const auto pev3 = evm.find(a2);
    const auto pev4 = evm.find(a3);
    if ((pev == evm.end()) ||
        ((pev2 == evm.end()) || (pev3 == evm.end()) || (pev4 == evm.end()))) {
      return;
    }
    if ((pev2 == evm.end()) || (pev3 == evm.end()) || (pev4 == evm.end())) {
      throw(
          std::runtime_error("computeThermalExpansion: at least one "
                             "of the three thermal expansion coefficient is "
                             "defined and at least one is not"));
    }
    const auto& T_ev = *(pev->second);
    const auto& a1_ev = *(pev2->second);
    const auto& a2_ev = *(pev3->second);
    const auto& a3_ev = *(pev4->second);
    if (d == 1u) {
      s.e_th0[0u] = a1_ev(t) * (T_ev(t) - s.Tref);
      s.e_th1[0u] = a1_ev(t + dt) * (T_ev(t + dt) - s.Tref);
      s.e_th0[1u] = a2_ev(t) * (T_ev(t) - s.Tref);
      s.e_th1[1u] = a2_ev(t + dt) * (T_ev(t + dt) - s.Tref);
      s.e_th0[2u] = a3_ev(t) * (T_ev(t) - s.Tref);
      s.e_th1[2u] = a3_ev(t + dt) * (T_ev(t + dt) - s.Tref);
    } else if ((d == 2u) || (d == 3u)) {
      // thermal expansion tensors in the material frame
      tfel::math::stensor<3u, real> se_th0(real(0));
      tfel::math::stensor<3u, real> se_th1(real(0));
      se_th0[0u] = a1_ev(t) * (T_ev(t) - s.Tref);
      se_th1[0u] = a1_ev(t + dt) * (T_ev(t + dt) - s.Tref);
      se_th0[1u] = a2_ev(t) * (T_ev(t) - s.Tref);
      se_th1[1u] = a2_ev(t + dt) * (T_ev(t + dt) - s.Tref);
      se_th0[2u] = a3_ev(t) * (T_ev(t) - s.Tref);
      se_th1[2u] = a3_ev(t + dt) * (T_ev(t + dt) - s.Tref);
      // backward rotation matrix
      const auto brm =
          transpose(s.behaviour->getRotationMatrix(s.mprops1, s.r));
      se_th0.changeBasis(brm);
      se_th1.changeBasis(brm);
      const auto ss = tfel::math::getStensorSize(d);
      std::copy(se_th0.begin(), se_th0.begin() + ss, s.e_th0.begin());
      std::copy(se_th1.begin(), se_th1.begin() + ss, s.e_th1.begin());
    } else {
      throw(std::runtime_error("MTest::prepare: invalid dimension"));
    }
  }

  void update(CurrentState& s) {
    s.iv_1 = s.iv0;
    s.s0 = s.s1;
    s.iv0 = s.iv1;
  }

  void revert(CurrentState& s) {
    s.e1 = s.e0;
    s.s1 = s.s0;
    s.iv1 = s.iv0;
  }

  void setInternalStateVariableValue(CurrentState& s,
                                     const std::string& n,
                                     const real v) {
    setInternalStateVariableValue(s, n, v, -1);
    setInternalStateVariableValue(s, n, v, 0);
    setInternalStateVariableValue(s, n, v, 1);
  }

  void setInternalStateVariableValue(CurrentState& s,
                                     const std::string& n,
                                     const real v,
                                     const int d) {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("mtest::setInternalStateVariableValue: " + m));
      }
    };
    throw_if(s.behaviour.get() == nullptr, "no behaviour defined");
    const auto& ivsnames = s.behaviour->getInternalStateVariablesNames();
    throw_if(std::find(ivsnames.begin(), ivsnames.end(), n) == ivsnames.end(),
             "the behaviour don't declare an internal state "
             "variable named '" +
                 n + "'");
    const auto type = s.behaviour->getInternalStateVariableType(n);
    throw_if(type != 0,
             "invalid state variable type (not scalar) for "
             "internal state variable '" +
                 n + "'");
    const auto pos = s.behaviour->getInternalStateVariablePosition(n);
    throw_if((s.iv_1.size() <= pos) || (s.iv0.size() <= pos) ||
                 (s.iv1.size() <= pos),
             "invalid size for state variables (bad initialization)");
    auto& iv = [&s, throw_if, d]() -> tfel::math::vector<mtest::real>& {
      throw_if((d != 1) && (d != 0) && (d != -1), "invalid depth");
      if (d == -1) {
        return s.iv_1;
      } else if (d == 0) {
        return s.iv0;
      }
      return s.iv1;
    }();
    iv[pos] = v;
  }  // end of setInternalStateVariableValue

  void setInternalStateVariableValue(CurrentState& s,
                                     const std::string& n,
                                     const std::vector<real>& v) {
    setInternalStateVariableValue(s, n, v, -1);
    setInternalStateVariableValue(s, n, v, 0);
    setInternalStateVariableValue(s, n, v, 1);
  }

  void setInternalStateVariableValue(CurrentState& s,
                                     const std::string& n,
                                     const std::vector<real>& v,
                                     const int d) {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("mtest::setInternalStateVariableValue: " + m));
      }
    };
    throw_if(s.behaviour.get() == nullptr, "no behaviour defined");
    const auto& ivsnames = s.behaviour->getInternalStateVariablesNames();
    throw_if(std::find(ivsnames.begin(), ivsnames.end(), n) == ivsnames.end(),
             "the behaviour don't declare an internal state "
             "variable named '" +
                 n + "'");
    const auto type = s.behaviour->getInternalStateVariableType(n);
    const auto size = [&s, throw_if, type]() -> std::vector<real>::size_type {
      if (type == 0) {
        return 1;
      } else if (type == 1) {
        return tfel::material::getStensorSize(s.behaviour->getHypothesis());
      } else if (type == 2) {
        return tfel::material::getSpaceDimension(s.behaviour->getHypothesis());
      } else if (type == 3) {
        return tfel::material::getTensorSize(s.behaviour->getHypothesis());
      }
      throw_if(true, "unsupported variable type");
      return 0;
    }();
    throw_if(v.size() != size, "bad number of values");
    const auto pos = s.behaviour->getInternalStateVariablePosition(n);
    throw_if((s.iv_1.size() < pos + size) || (s.iv0.size() < pos + size) ||
                 (s.iv1.size() < pos + size),
             "invalid size for state variables (bad initialization)");
    auto& iv = [&s, throw_if, d]() -> tfel::math::vector<mtest::real>& {
      throw_if((d != 1) && (d != 0) && (d != -1), "invalid depth");
      if (d == -1) {
        return s.iv_1;
      } else if (d == 0) {
        return s.iv0;
      }
      return s.iv1;
    }();
    std::copy(v.begin(), v.end(), iv.begin() + pos);
  }  // end of setInternalStateVariableValue

}  // end of namespace mtest
