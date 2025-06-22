/*!
 * \file  mtest/src/Evolution.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */
#include <iostream>
#include <string>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/Parser/ExternalFunctionManager.hxx"
#include "TFEL/Math/Parser/ConstantExternalFunction.hxx"
#include "MTest/Evolution.hxx"

namespace mtest {

  Evolution::~Evolution() = default;

  ConstantEvolution::ConstantEvolution(const real v) : value(v) {}

  real ConstantEvolution::operator()(const real) const {
    return this->value;
  }  // end of ConstantEvolution::operator()

  bool ConstantEvolution::isConstant() const {
    return true;
  }  // end of ConstantEvolution::isConstant

  void ConstantEvolution::setValue(const real v) { this->value = v; }

  void ConstantEvolution::setValue(const real, const real) {
    tfel::raise(
        "ConstantEvolution::setValue : "
        "this method does not makes sense "
        "for constant evolution");
  }

  ConstantEvolution::~ConstantEvolution() = default;

  LPIEvolution::LPIEvolution(const std::vector<real>& t,
                             const std::vector<real>& v) {
    tfel::raise_if(t.size() != v.size(),
                   "LPIEvolution::LPIEvolution : "
                   "the number of values of the times don't match "
                   "the number of values of the evolution");
    auto pt = t.begin();
    auto pv = v.begin();
    while (pt != t.end()) {
      this->values.insert({*pt, *pv});
      ++pt;
      ++pv;
    }
  }

  void LPIEvolution::setValue(const real) {
    tfel::raise(
        "LPIEvolution::setValue : "
        "this method does not makes sense "
        "for LPI evolution");
  }

  void LPIEvolution::setValue(const real t, const real v) {
    this->values[t] = v;
  }

  real LPIEvolution::operator()(const real t) const {
    return LPIEvolution::interpolate(this->values, t);
  }  // end of operator()

  real LPIEvolution::interpolate(const std::map<real, real>& values,
                                 const real t) {
    tfel::raise_if(values.empty(),
                   "LPILoadingEvolution::interpolate: "
                   "no values specified");
    if (values.size() == 1u) {
      return values.begin()->second;
    }
    auto p = values.lower_bound(t);
    if (p == values.begin()) {
      return p->second;
    } else if (p == values.end()) {
      --p;
      return p->second;
    }
    const auto pp = std::prev(p);
    const auto x0 = pp->first;
    const auto y0 = pp->second;
    const auto x1 = p->first;
    const auto y1 = p->second;
    return (y1 - y0) / (x1 - x0) * (t - x0) + y0;
  }  // end of interpolate

  bool LPIEvolution::isConstant() const {
    return (this->values.size() == 1);
  }  // end of LPIEvolution::isConstant

  LPIEvolution::~LPIEvolution() = default;

  std::shared_ptr<Evolution> make_evolution(const real v) {
    return std::shared_ptr<Evolution>{new ConstantEvolution(v)};
  }

  std::shared_ptr<Evolution> make_evolution(const std::map<real, real>& v) {
    std::vector<real> tv(v.size());
    std::vector<real> ev(v.size());
    std::vector<real>::size_type i = 0u;
    for (const auto& mv : v) {
      tv[i] = mv.first;
      ev[i] = mv.second;
      ++i;
    }
    return std::shared_ptr<Evolution>{new LPIEvolution(tv, ev)};
  }

  std::shared_ptr<tfel::math::parser::ExternalFunctionManager>
  buildExternalFunctionManagerFromConstantEvolutions(
      const EvolutionManager& evm) {
    using namespace tfel::math::parser;
    auto efm = std::make_shared<ExternalFunctionManager>();
    for (const auto& ev : evm) {
      auto pcev = dynamic_cast<const ConstantEvolution*>(ev.second.get());
      if (pcev != nullptr) {
        const auto& cev = *pcev;
        efm->insert(
            {ev.first, std::make_shared<ConstantExternalFunction>(cev(0))});
      }
    }
    return efm;
  }  // end of buildExternalFunctionManagerFromConstantEvolutions

  void checkIfDeclared(const std::vector<std::string>& names,
                       const EvolutionManager& m,
                       const std::string& type) {
    for (const auto& n : names) {
      if (m.find(n) == m.end()) {
        tfel::raise("no " + type +
                    " named "
                    "'" +
                    n + "' declared");
      }
    }
  }

  void checkIfDeclared(const std::vector<std::string>& names,
                       const EvolutionManager& evm1,
                       const EvolutionManager& evm2,
                       const std::string& type) {
    for (const auto& n : names) {
      if (evm1.find(n) == evm1.end()) {
        tfel::raise_if(evm2.find(n) == evm2.end(), "no " + type +
                                                       " named "
                                                       "'" +
                                                       n + "' declared");
      }
    }
  }

}  // end of namespace mtest
