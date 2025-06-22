/*!
 * \file   TestCaseParameters.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include "TFEL/Raise.hxx"
#include "MFMTestGenerator/Evolution.hxx"
#include "MFMTestGenerator/TestCaseParameters.hxx"

namespace mfmtg {

  void check(const TestCaseParameters& m, const std::vector<std::string>& k) {
    for (const auto& p : m) {
      if (std::find(k.begin(), k.end(), p.first) == k.end()) {
        tfel::raise("check: invalid key '" + p.first + "'");
      }
    }
  }  // end of check

  bool contains(const TestCaseParameters& parameters, const std::string& n) {
    return parameters.count(n) != 0;
  }  // end of contains

  bool contains(const TestCaseParameters& parameters, const char* const n) {
    return parameters.count(n) != 0;
  }  // end of contains

  const TestCaseParameter& getParameter(const TestCaseParameters& parameters,
                                        const std::string& n) {
    const auto p = parameters.find(n);
    if (p == parameters.end()) {
      tfel::raise("getParameter: no parameter named '" + n + "'");
    }
    return p->second;
  }  // end of getParameter

  const TestCaseParameter& getParameter(const TestCaseParameters& parameters,
                                        const char* const n) {
    const auto p = parameters.find(n);
    if (p == parameters.end()) {
      tfel::raise("getParameter: no parameter named '" + std::string(n) + "'");
    }
    return p->second;
  }  // end of getParameter

  const TestCaseParameters& getTestCaseParameters(
      const TestCaseParameters& parameters, const std::string& n) {
    const auto& p = getParameter(parameters, n);
    if (!p.is<TestCaseParameters>()) {
      tfel::raise("getTestCaseParameters: parameter '" + n +
                  "' has not the expected type");
    }
    return p.get<TestCaseParameters>();
  }  // end of getTestCaseParameters

  const TestCaseParameters& getTestCaseParameters(
      const TestCaseParameters& parameters, const char* const n) {
    const auto& p = getParameter(parameters, n);
    if (!p.is<TestCaseParameters>()) {
      tfel::raise("getTestCaseParameters: parameter '" + std::string(n) +
                  "' has not the expected type");
    }
    return p.get<TestCaseParameters>();
  }  // end of getTestCaseParameters

  Evolution getEvolution(const TestCaseParameters& p, const std::string& n) {
    return getEvolution(p, n.c_str());
  }  // end of getEvolution

  Evolution getEvolution(const TestCaseParameters& p, const char* const n) {
    const auto& e = getParameter(p, n);
    if (e.is<int>()) {
      return static_cast<double>(e.get<int>());
    } else if (e.is<double>()) {
      return e.get<double>();
    } else if (e.is<std::map<double, double>>()) {
      return e.get<std::map<double, double>>();
    } else if (!e.is<TestCaseParameters>()) {
      tfel::raise("getEvolution: invalid type for evolution '" +
                  std::string(n) + "'");
    } else if (e.is<std::map<double, double>>()) {
      return e.get<std::map<double, double>>();
    }
    const auto& evd = e.get<TestCaseParameters>();
    if (contains(evd, "file")) {
      auto get_column =
          [&evd](const char* const nvalues) -> EvolutionFromFile::Values {
        const auto pvalues = getParameter(evd, nvalues);
        if (pvalues.is<int>()) {
          const auto c = pvalues.get<int>();
          if (c <= 0) {
            tfel::raise(
                "mfmtg::getEvolution: "
                "invalid column value");
          }
          return static_cast<unsigned int>(c);
        }
        return pvalues.get<std::string>();
      };
      auto ev = EvolutionFromFile{};
      ev.file = get(evd, "file");
      ev.times = get_column("times");
      ev.values = get_column("values");
      return ev;
    }
    // the evolution is defined explicitly
    auto ev = std::map<double, double>{};
    check(e, {"times", "values"});
    const auto times = tfel::utilities::convert<std::vector<double>>(
        getParameter(evd, "times"));
    const auto values = tfel::utilities::convert<std::vector<double>>(
        getParameter(evd, "values"));
    if (times.size() != values.size()) {
      tfel::raise("getEvolution: times and values don't have the same sizes");
    }
    for (decltype(times.size()) i = 0; i != times.size(); ++i) {
      ev.insert({times[i], values[i]});
    }
    return ev;
  }  // end of getEvolution

  std::map<std::string, Evolution> getEvolutions(const TestCaseParameters& p,
                                                 const std::string& n,
                                                 const bool b) {
    return getEvolutions(p, n.c_str(), b);
  }  // end of getEvolution

  std::map<std::string, Evolution> getEvolutions(const TestCaseParameters& p,
                                                 const char* const n,
                                                 const bool b) {
    if (b && (!contains(p, n))) {
      return {};
    }
    auto evs = std::map<std::string, Evolution>{};
    const auto m = getTestCaseParameters(p, n);
    for (const auto& ev : m) {
      evs.insert({ev.first, getEvolution(m, ev.first)});
    }
    return evs;
  }  // end of getEvolutions

  void throwInvalidParameterTypeException(const std::string& n) {
    tfel::raise("get_parameter: unexpected type for parameter '" + n + "'");
  }  // end of throwInvalidParameterTypeException

}  // end of namespace mfmtg
