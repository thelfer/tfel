/*!
 * \file   TextDataUtilities.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   11 janv. 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include "TFEL/Math/Evaluator.hxx"
#include "MTest/Evolution.hxx"
#include "MTest/TextDataUtilities.hxx"

namespace mtest {

  std::vector<double> eval(const tfel::utilities::TextData& d,
                           const EvolutionManager& m,
                           const std::string& f) {
    struct Variable {
      std::string n;
      std::vector<double> values;
    };
    auto matches = [](const std::string& vn) {
      if (vn.size() < 2) {
        return false;
      }
      if (vn[0] != '$') {
        return false;
      }
      auto p = std::begin(vn) + 1;
      for (; p != vn.end(); ++p) {
        const char c = *p;
        if (!((c >= '0') && (c <= '9'))) {
          return false;
        }
      }
      return true;
    };
    auto convert = [](const std::string& vn) {
      auto p = std::begin(vn) + 1;
      unsigned short value{0};
      for (; p != vn.end(); ++p) {
        const char c = *p;
        if (!((c >= '0') && (c <= '9'))) {
          throw(
              std::runtime_error("mtest::eval::convert: "
                                 "invalid input '" +
                                 vn + "'"));
        }
        value *= 10;
        value += c - '0';
      }
      return value;
    };
    tfel::math::Evaluator e{f};
    std::vector<Variable> variables;
    for (const auto& v : e.getVariablesNames()) {
      if (matches(v)) {
        variables.push_back(Variable{v, d.getColumn(convert(v))});
      } else {
        auto pev = m.find(v);
        if (pev == m.end()) {
          throw(
              std::runtime_error("mtest::eval: undeclared "
                                 "variable '" +
                                 v + "'"));
        }
        const auto& ev = *(pev->second);
        if (!ev.isConstant()) {
          throw(
              std::runtime_error("mtest::eval: evolution "
                                 "'" +
                                 v + "' is not constant"));
        }
        e.setVariableValue(v, ev(real(0)));
      }
    }
    if (variables.empty()) {
      return d.getColumn(convert(f));
    }
    std::vector<double> r;
    r.resize(variables[0].values.size());
    for (std::vector<double>::size_type i = 0; i != r.size(); ++i) {
      for (const auto& v : variables) {
        e.setVariableValue(v.n, v.values[i]);
      }
      r[i] = e.getValue();
    }
    return r;
  }  // end of eval

}  // namespace mtest
