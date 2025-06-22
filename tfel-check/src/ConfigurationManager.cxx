/*!
 * \file   ConfigurationManager.cxx
 * \brief
 * \author Thomas Helfer
 * \date   13 sept. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Check/ConfigurationManager.hxx"

namespace tfel {

  namespace check {

    std::pair<std::string, std::string> ConfigurationManager::extract(
        const std::string& n) {
      const auto pos = n.find_first_of("::");
      if (pos == std::string::npos) {
        return {n, ""};
      }
      return {n.substr(0, pos), n.substr(pos + 2)};
    }  // end of ConfigurationManager::extract

    ConfigurationManager::ConfigurationManager() = default;

    ConfigurationManager::ConfigurationManager(const Configuration& src)
        : c(src) {}  // end of ConfigurationManager::ConfigurationManager

    ConfigurationManager::ConfigurationManager(ConfigurationManager&&) =
        default;

    ConfigurationManager::ConfigurationManager(const ConfigurationManager&) =
        default;

    void ConfigurationManager::modify(std::function<void(Configuration&)> f) {
      for (auto& s : this->subordinates) {
        s.second.modify(f);
      }
      f(this->c);
    }  // end of ConfigurationManager::modify

    ConfigurationManager& ConfigurationManager::getConfigurationManager(
        const std::string& n) {
      if (n.empty()) {
        return *this;
      }
      auto r = ConfigurationManager::extract(n);
      raise_if(r.first.empty(),
               "ConfigurationManager::"
               "getConfigurationManager: "
               "invalid location '" +
                   n + "'");
      if (this->subordinates.find(r.first) == this->subordinates.end()) {
        this->subordinates.insert({r.first, this->c});
      }
      return this->subordinates[r.first].getConfigurationManager(r.second);
    }  // end of ConfigurationManager::getConfigurationManager

    const ConfigurationManager& ConfigurationManager::getConfigurationManager(
        const std::string& n) const {
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b,
                 "ConfigurationManager::"
                 "getConfigurationManager: " +
                     m);
      };
      if (n.empty()) {
        return *this;
      }
      auto r = ConfigurationManager::extract(n);
      throw_if(r.first.empty(), "invalid location '" + n + "'");
      throw_if(this->subordinates.find(r.first) == this->subordinates.end(),
               "invalid location '" + n +
                   "' "
                   "(no subordinates called '" +
                   r.first + "')");
      return this->subordinates.at(r.first).getConfigurationManager(r.second);
    }  // end of ConfigurationManager::getConfigurationManager

    const Configuration& ConfigurationManager::getConfiguration() const {
      return c;
    }  // end of ConfigurationManager::getConfiguration

    ConfigurationManager& ConfigurationManager::operator=(
        ConfigurationManager&&) = default;

    ConfigurationManager& ConfigurationManager::operator=(
        const ConfigurationManager&) = default;

    void parse(ConfigurationManager& c, const std::string& n) {
      using namespace tfel::utilities;
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b, "tfel::check::parse: " + m);
      };
      CxxTokenizer tokenizer;
      tokenizer.treatCharAsString(true);
      tokenizer.mergeStrings(false);
      tokenizer.openFile(n);
      auto p = tokenizer.begin();
      const auto pe = tokenizer.end();
      while (p != pe) {
        if (p->value == "substitutions") {
          ++p;
          CxxTokenizer::readSpecifiedToken("tfel::check::parse", ":", p, pe);
          CxxTokenizer::checkNotEndOfLine("tfel::check::parse", p, pe);
          const auto variables =
              convert<std::map<std::string, std::string>>(Data::read(p, pe));
          c.modify([&variables, &throw_if](Configuration& conf) {
            for (const auto& v : variables) {
              std::cout << v.first << ": " << v.second << std::endl;
              throw_if(!conf.substitutions.insert(v).second,
                       "variable '" + v.first + "' already declared");
            }
          });
        } else {
          throw_if(true, "unexpected token 'p->value'");
        }
      }
    }  // end of parse

  }  // end of namespace check

}  // end of namespace tfel
