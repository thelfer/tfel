/*!
 * \file   mfront/src/DSLFactory.cxx
 * \brief
 * \author Helfer Thomas
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

#include "MFront/DSLFactory.hxx"
#include "MFront/DSLProxy.hxx"

namespace mfront {

  DSLFactory& DSLFactory::getDSLFactory() {
    static DSLFactory f;
    return f;
  }

  DSLFactory::DescriptionPtrContainer& DSLFactory::getDescriptionMap() {
    static DescriptionPtrContainer map;
    return map;
  }  // end of DSLFactory::getDescriptionMap

  DSLFactory::ParserCreatorsContainer& DSLFactory::getMap() {
    static ParserCreatorsContainer map;
    return map;
  }  // end of DSLFactory::getMap

  DSLFactory::DSLFactory() = default;

  std::vector<std::string> DSLFactory::getRegistredParsers() {
    auto res = std::vector<std::string>{};
    for (const auto& p : this->getMap()) {
      res.push_back(p.first);
    }
    return res;
  }

  void DSLFactory::registerParserCreator(const std::string& n,
                                         const DSLFactory::ParserCreator f,
                                         const DSLFactory::DescriptionPtr f2) {
    if (!this->getMap().insert({n, f}).second) {
      throw(
          std::runtime_error("DSLFactory::registerParserCreator: "
                             "a dsl named '" +
                             n + "' has already been registred"));
    }
    if (!this->getDescriptionMap().insert({n, f2}).second) {
      throw(
          std::runtime_error("DSLFactory::registerParserCreator: "
                             "a parser named " +
                             n + " has already been registred"));
    }
  }  // end of DSLFactory::registerParserCreator

  std::string DSLFactory::getParserDescription(const std::string& n) {
    auto p = this->getDescriptionMap().find(n);
    if (p == this->getDescriptionMap().end()) {
      throw(
          std::runtime_error("DSLFactory::getParserDescription: "
                             "no parser named '" +
                             n + "'"));
    }
    auto c = p->second;
    return c();
  }  // end of

  std::shared_ptr<AbstractDSL> DSLFactory::createNewParser(
      const std::string& n) {
    return this->createNewDSL(n);
  }

  std::shared_ptr<AbstractDSL> DSLFactory::createNewDSL(const std::string& n) {
    auto p = this->getMap().find(n);
    if (p == this->getMap().end()) {
      throw(
          std::runtime_error("DSLFactory::createNewDSL: "
                             "no parser named '" +
                             n + "'"));
    }
    auto c = p->second;
    return c();
  }

  DSLFactory::~DSLFactory() = default;

}  // end of namespace mfront
