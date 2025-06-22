/*!
 * \file  mtest/src/ReferenceFileComparisonTest.cxx
 * \brief
 * \author Thomas Helfer
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
#include "TFEL/Raise.hxx"
#include "MTest/TextDataUtilities.hxx"
#include "MTest/ReferenceFileComparisonTest.hxx"

namespace mtest {

  ReferenceFileComparisonTest::ReferenceFileComparisonTest(
      const tfel::utilities::TextData& d,
      const unsigned int c,
      const std::string& v,
      const std::function<real(const CurrentState&)>& g,
      const real eps_)
      : values(d.getColumn(c)),
        name(v),
        get(g),
        eps(eps_) {
  }  // ReferenceFileComparisonTest::ReferenceFileComparisonTest

  ReferenceFileComparisonTest::ReferenceFileComparisonTest(
      const tfel::utilities::TextData& d,
      const EvolutionManager& e,
      const std::string& f,
      const std::string& v,
      const std::function<real(const CurrentState&)>& g,
      const real eps_)
      : values(eval(d, e, f)),
        name(v),
        get(g),
        eps(eps_) {
  }  // ReferenceFileComparisonTest::ReferenceFileComparisonTest

  void ReferenceFileComparisonTest::check(const CurrentState& s,
                                          const real t,
                                          const real dt,
                                          const unsigned int p) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "ReferenceFileComparisonTest::check: " + m);
    };
    const real v = get(s);
    if (p >= this->values.size()) {
      std::ostringstream msg;
      msg << "ReferenceFileComparisonTest::check : comparison for variable '"
          << this->name << "' failed for time '" << t + dt << "' "
          << "(reference value is not available for period  '" << p << "')";
      this->results.append({false, msg.str()});
    } else {
      throw_if(!std::isfinite(v), "invalid result for '" + this->name + "'");
      const real err = std::abs(v - this->values[p]);
      if (err > this->eps) {
        std::ostringstream msg;
        msg << "ReferenceFileComparisonTest::check : comparison for variable '"
            << this->name << "' failed for time '" << t + dt << "' "
            << "(computed value: '" << v << "', "
            << "expected value: '" << this->values[p] << "', "
            << "error: '" << err << "', criterion '" << this->eps << "')";
        this->results.append({false, msg.str()});
      }
    }
  }  // end of ReferenceFileComparisonTest::check

  tfel::tests::TestResult ReferenceFileComparisonTest::getResults() const {
    if (this->results.success()) {
      std::ostringstream msg;
      msg << "ReferenceFileComparisonTest::check : comparison for variable '"
          << this->name << "' was successfull for all times ("
          << "criterion '" << this->eps << "')";
      return {true, msg.str()};
    }
    return this->results;
  }

  ReferenceFileComparisonTest::~ReferenceFileComparisonTest() = default;

}  // end of namespace mtest
