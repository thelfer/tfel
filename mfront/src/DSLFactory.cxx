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
    for (const auto& p : this->creators) {
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
  }  // end of DSLFactory::getRegistredDSLs()

  void DSLFactory::registerDSLCreator(const std::string& n,
                                      const DSLFactory::DSLCreator f,
                                      const DSLFactory::DescriptionPtr f2) {
    auto raise = [&n] {
      tfel::raise("DSLFactory::registerDSLCreator: a DSL named '" + n +
                  "' has already been registred");
    };
    if (!this->creators.insert({n, f}).second) {
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
  }  // end of DSLFactory::registerDSLCreator

  void DSLFactory::registerDSLAlias(const std::string& n,
                                    const std::string& a) {
    auto raise = [](const std::string& m) {
      tfel::raise("DSLFactory::registerAlias: " + m);
    };
    if (this->creators.find(n) == this->creators.end()) {
      tfel::raise("no DSL named '" + n + "' registred");
    }
    if (this->creators.find(a) != this->creators.end()) {
      tfel::raise("a DSL named '" + a + "' has already been registred");
    }
    for (const auto& as : this->aliases) {
      if (std::find(as.second.cbegin(), as.second.cend(), a) !=
          as.second.cend()) {
        raise("a DSL named '" + a + "' has already been registred");
      }
    }
    this->aliases[n].push_back(a);
  }  // end of DSLFactory::registerDSLAlias

  std::shared_ptr<AbstractDSL> DSLFactory::createNewDSL(
      const std::string& n) const {
    const auto rn = [this, &n]() -> const std::string& {
      for (const auto& as : this->aliases) {
        if (std::find(as.second.cbegin(), as.second.cend(), n) !=
            as.second.cend()) {
          return as.first;
        }
      }
      return n;
    }();
    auto p = this->creators.find(rn);
    if (p == this->creators.end()) {
      tfel::raise(
          "DSLFactory::createNewDSL: "
          "no DSL named '" +
          n + "'");
    }
    auto c = p->second;
    return c();
  }  // end of DSLFactory::createNewDSL

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
  }  // end of DSLFactory::getDSLDescription

  std::vector<std::string> DSLFactory::getRegistredParsers(const bool b) const {
    return this->getRegistredDSLs(b);
  }  // end of DSLFactory::getRegistredParsers()

  void DSLFactory::registerParserCreator(const std::string& n,
                                         const DSLFactory::ParserCreator f,
                                         const DSLFactory::DescriptionPtr f2) {
    this->registerDSLCreator(n, f, f2);
  }  // end of DSLFactory::registerParserCreator

  std::string DSLFactory::getParserDescription(const std::string& n) const {
    return this->getDSLDescription(n);
  }  // end of DSLFactory::getParserDescription

  std::shared_ptr<AbstractDSL> DSLFactory::createNewParser(
      const std::string& n) const {
    return this->createNewDSL(n);
  }

  DSLFactory::~DSLFactory() = default;

}  // end of namespace mfront
