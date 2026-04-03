/*!
 * \file   mfront/src/DSLFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <vector>
#include <string>
#include <utility>
#include <iterator>
#include <stdexcept>
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "MFront/DSLFactory.hxx"
#include "MFront/DSLProxy.hxx"

namespace mfront {

  DSLFactory& DSLFactory::getDSLFactory() {
    static DSLFactory f;
    return f;
  }

  DSLFactory::DSLFactory() = default;

  std::vector<std::string> DSLFactory::getRegistredDSLs(const bool b) const {
    auto res = std::vector<std::string>{};
    for (const auto& p : this->generators) {
      res.push_back(p.first);
    }
    if (b) {
      for (const auto& as : this->aliases) {
        for (const auto& a : as.second) {
          res.push_back(a);
        }
      }
    }
    return res;
  }  // end of getRegistredDSLs()

  void DSLFactory::registerDSLCreator(const std::string& n,
                                      const DSLGenerator f,
                                      const DescriptionGenerator f2) {
    auto raise = [&n] {
      tfel::raise("DSLFactory::registerDSLCreator: a DSL named '" + n +
                  "' has already been registred");
    };
    if (!this->generators.insert({n, f}).second) {
      raise();
    }
    for (const auto& as : this->aliases) {
      if (std::find(as.second.cbegin(), as.second.cend(), n) !=
          as.second.cend()) {
        raise();
      }
    }
    if (!this->descriptions.insert({n, f2}).second) {
      raise();
    }
  }  // end of registerDSLCreator

  void DSLFactory::registerDSLAlias(const std::string& n,
                                    const std::string& a) {
    auto raise = [](const std::string& m) {
      tfel::raise("DSLFactory::registerAlias: " + m);
    };
    if (this->generators.find(n) == this->generators.end()) {
      tfel::raise("no DSL named '" + n + "' registred");
    }
    if (this->generators.find(a) != this->generators.end()) {
      tfel::raise("a DSL named '" + a + "' has already been registred");
    }
    for (const auto& as : this->aliases) {
      if (std::find(as.second.cbegin(), as.second.cend(), a) !=
          as.second.cend()) {
        raise("a DSL named '" + a + "' has already been registred");
      }
    }
    this->aliases[n].push_back(a);
  }  // end of registerDSLAlias

  std::shared_ptr<AbstractDSL> DSLFactory::createNewDSL(
      const std::string& n, const AbstractDSL::DSLOptions& opts) const {
    const auto rn = [this, &n]() -> const std::string& {
      for (const auto& as : this->aliases) {
        if (std::find(as.second.cbegin(), as.second.cend(), n) !=
            as.second.cend()) {
          return as.first;
        }
      }
      return n;
    }();
    const auto p = this->generators.find(rn);
    if (p == this->generators.end()) {
      auto msg =
          "DSLFactory::createNewDSL: "
          "no DSL named '" +
          n + "'.\nAvailable dsls are:";
      for (const auto& dsl : this->getRegistredParsers()) {
        msg += " " + dsl;
      }
      tfel::raise(msg);
    }
    auto c = p->second;
    return c(opts);
  }  // end of createNewDSL

  std::shared_ptr<AbstractDSL> DSLFactory::createNewParser(
      const std::string& n, const AbstractDSL::DSLOptions& opts) const {
    return this->createNewDSL(n, opts);
  }  // end of createNewParser

  std::string DSLFactory::getDSLDescription(const std::string& n) const {
    const auto rn = [this, &n]() -> const std::string& {
      for (const auto& as : this->aliases) {
        if (std::find(as.second.cbegin(), as.second.cend(), n) !=
            as.second.cend()) {
          return as.first;
        }
      }
      return n;
    }();
    auto p = this->descriptions.find(rn);
    if (p == this->descriptions.end()) {
      tfel::raise(
          "DSLFactory::getDSLDescription: "
          "no DSL named '" +
          n + "'");
    }
    auto c = p->second;
    return c();
  }  // end of getDSLDescription

  std::vector<std::string> DSLFactory::getRegistredParsers(const bool b) const {
    return this->getRegistredDSLs(b);
  }  // end of getRegistredParsers()

  void DSLFactory::registerParserCreator(const std::string& n,
                                         const DSLGenerator f,
                                         const DescriptionGenerator f2) {
    this->registerDSLCreator(n, f, f2);
  }  // end of registerParserCreator

  std::string DSLFactory::getParserDescription(const std::string& n) const {
    return this->getDSLDescription(n);
  }  // end of getParserDescription

  DSLFactory::~DSLFactory() = default;

}  // end of namespace mfront
