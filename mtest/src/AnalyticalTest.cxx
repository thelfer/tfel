/*!
 * \file  mfront/mtest/AnalyticalTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <sstream>

#include "MTest/Evolution.hxx"
#include "MTest/AnalyticalTest.hxx"

namespace mtest {

  AnalyticalTest::AnalyticalTest(
      const std::string& f_,
      const std::string& v,
      const std::function<real(const CurrentState&)>& g,
      const EvolutionManager& evm_,
      const real eps_)
      : f(f_),
        name(v),
        get(g),
        evm(evm_),
        eps(eps_) {}  // AnalyticalTest::AnalyticalTest

  void AnalyticalTest::check(const CurrentState& s,
                             const real t,
                             const real dt,
                             const unsigned int) {
    auto throw_if = [](const bool c, const std::string& m) {
      if (c) {
        throw(std::runtime_error("AnalyticalTest::check:" + m));
      }
    };
    for (const auto& vn : this->f.getVariablesNames()) {
      if (vn == "t") {
        this->f.setVariableValue("t", t + dt);
      } else {
        auto pev = this->evm.find(vn);
        throw_if(pev == this->evm.end(),
                 "no evolution named '" + vn + "' defined");
        const auto& ev = *(pev->second);
        this->f.setVariableValue(vn, ev(t + dt));
      }
    }
    const real v = get(s);
    const auto fv = this->f.getValue();
    throw_if(!std::isfinite(v), "invalid result for '" + this->name + "'");
    throw_if(!std::isfinite(fv), "invalid evolution of reference value");
    const real err = std::abs(v - fv);
    if (err > this->eps) {
      std::ostringstream msg;
      msg << "AnalyticalTest::check : comparison for variable '" << this->name
          << "' failed for time '" << t + dt << "' "
          << "(computed value: '" << v << "', "
          << "expected value: '" << fv << "', "
          << "error: '" << err << "', criterion '" << this->eps << "')";
      this->results.append({false, msg.str()});
    }
  }  // end of AnalyticalTest::check

  tfel::tests::TestResult AnalyticalTest::getResults() const {
    if (this->results.success()) {
      std::ostringstream msg;
      msg << "AnalyticalTest::check : comparison for variable '" << this->name
          << "' was successfull for all times ("
          << "criterion '" << this->eps << "')";
      return {true, msg.str()};
    }
    return this->results;
  }

  AnalyticalTest::~AnalyticalTest() = default;

}  // end of namespace mtest
