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

#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/System/System.hxx"
#include "TFEL/Check/ConfigurationManager.hxx"

namespace tfel {

  namespace check {

    std::pair<std::string, std::string> ConfigurationManager::extract(
        const std::string& n) {
      if ((n.empty()) || (n == ".")) {
        return {"", ""};
      }
      const auto b = [&n]() -> std::string::size_type {
        if ((n.size() >= 2) && (n[0] == '.') &&
            (n[1] == tfel::system::dirSeparator())) {
          return 2;
        }
        return 0;
      }();
      const auto pos = n.find_first_of(tfel::system::dirSeparator());
      if (pos == std::string::npos) {
        return {n.substr(b), ""};
      }
      return {n.substr(b, pos), n.substr(pos + 1)};
    }  // end of ConfigurationManager::extract

    ConfigurationManager::ConfigurationManager() = default;

    ConfigurationManager::ConfigurationManager(const Configuration& src)
        : configuration(src) {
    }  // end of ConfigurationManager::ConfigurationManager

    ConfigurationManager::ConfigurationManager(ConfigurationManager&&) =
        default;

    ConfigurationManager::ConfigurationManager(const ConfigurationManager&) =
        default;

    void ConfigurationManager::modify(std::function<void(Configuration&)> f) {
      for (auto& s : this->subordinates) {
        s.second.modify(f);
      }
      f(this->configuration);
    }  // end of ConfigurationManager::modify

    ConfigurationManager& ConfigurationManager::getConfigurationManager(
        const std::string& n) {
      auto r = ConfigurationManager::extract(n);
      if (r.first.empty()) {
        return *this;
      }
      auto p = this->subordinates.find(r.first);
      if (p == this->subordinates.end()) {
        p = this->subordinates.insert({r.first, this->configuration}).first;
      }
      return p->second.getConfigurationManager(r.second);
    }  // end of ConfigurationManager::getConfigurationManager

    const Configuration& ConfigurationManager::getConfiguration(
        const std::string& n) const {
      const auto r = ConfigurationManager::extract(n);
      if (r.first.empty()) {
        return this->configuration;
      }
      const auto p = this->subordinates.find(r.first);
      if (p == this->subordinates.end()) {
        return this->configuration;
      }
      return p->second.getConfiguration(r.second);
    }  // end of ConfigurationManager::getConfigurationManager

    void ConfigurationManager::addSubstitution(const std::string& s1,
                                               const std::string& s2) {
      this->modify([&s1, &s2](Configuration& c) {
        if (!c.substitutions.insert({s1, s2}).second) {
          tfel::raise(
              "ConfigurationManager::addSubstitution: "
              "a substitution for '" +
              s1 + "' has already been defined");
        }
      });
    }  // end of ConfigurationManager::addSubstitution

    void ConfigurationManager::addComponent(const std::string& c) {
      this->modify([&c](Configuration& cf) {
        auto& components = cf.available_components;
        if (std::find(components.begin(), components.end(), c) !=
            components.end()) {
          tfel::raise("ConfigurationManager::addComponent: component '" + c +
                      "' has already been defined");
        }
        components.push_back(c);
      });
    }  // end of ConfigurationManager::addComponent

    const Configuration& ConfigurationManager::getConfiguration() const {
      return this->configuration;
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
        if (p->value == "components") {
          ++p;
          CxxTokenizer::readSpecifiedToken("tfel::check::parse", ":", p, pe);
          CxxTokenizer::checkNotEndOfLine("tfel::check::parse", p, pe);
          const auto components =
              convert<std::vector<std::string>>(Data::read(p, pe));
          for (const auto& component : components) {
            c.addComponent(component);
          }
          CxxTokenizer::readSpecifiedToken("tfel::check::parse", ";", p, pe);
        } else if (p->value == "substitutions") {
          ++p;
          CxxTokenizer::readSpecifiedToken("tfel::check::parse", ":", p, pe);
          CxxTokenizer::checkNotEndOfLine("tfel::check::parse", p, pe);
          const auto variables =
              convert<std::map<std::string, std::string>>(Data::read(p, pe));
          for (const auto& v : variables) {
            c.addSubstitution("@" + v.first + "@", v.second);
          }
          CxxTokenizer::readSpecifiedToken("tfel::check::parse", ";", p, pe);
        } else {
          throw_if(true, "unexpected token '" + p->value + "'");
        }
      }
    }  // end of parse

  }  // end of namespace check

}  // end of namespace tfel
