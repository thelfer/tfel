/*!
 * \file   ConfigurationManager.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09/04/2026
 */

#include <optional>
#include "MFront/ConfigurationManager.hxx"
#include "MFront/MaterialPropertyInterfaceFactory.hxx"
#include "MFront/BehaviourInterfaceFactory.hxx"
#include "MFront/ModelInterfaceFactory.hxx"

namespace mfront {

  [[nodiscard]] static std::string getUniqueInterfaceIdentifier(
      const std::string& n) {
    const auto& mpf =
        MaterialPropertyInterfaceFactory::getMaterialPropertyInterfaceFactory();
    const auto& bf = BehaviourInterfaceFactory::getBehaviourInterfaceFactory();
    const auto& mf = ModelInterfaceFactory::getModelInterfaceFactory();
    auto id = std::optional<std::string>{};
    if (mpf.exists(n)) {
      id = mpf.getUniqueNameFromAlias(n);
    }
    if (bf.exists(n)) {
      const auto bid = bf.getUniqueNameFromAlias(n);
      if (id.has_value()) {
        if (*id != bid) {
          tfel::raise("interface '" + n +
                      "' is associated with two unique identifiers ('" + *id +
                      "' and '" + bid + "')");
        }
      } else {
        id = bid;
      }
    }
    if (mf.exists(n)) {
      const auto mid = mf.getUniqueNameFromAlias(n);
      if (id.has_value()) {
        if (*id != mid) {
          tfel::raise("interface '" + n +
                      "' is associated with two unique identifiers ('" + *id +
                      "' and '" + mid + "')");
        }
      } else {
        id = mid;
      }
    }
    if (!id.has_value()) {
      tfel::raise("unknown interface '" + n +
                  "' (neither an interface for material properties, nor "
                  "behaviours nor models)");
    }
    return *id;
  }  // end of getInterfaceIdentifier

  ConfigurationManager& ConfigurationManager::get() noexcept {
    static ConfigurationManager m;
    return m;
  }

  void ConfigurationManager::addInterfaceOptions(const std::string& i,
                                                 const DataMap& opts) {
    const auto id = getUniqueInterfaceIdentifier(i);
    auto p = this->interfaces_options.find(id);
    if (p == this->interfaces_options.end()) {
      this->interfaces_options.insert({id, opts});
    } else {
      p->second = merge(p->second, opts, false);
    }
  }  // end of addInterfaceOptions

  void ConfigurationManager::addMaterialPropertyInterfaceOptions(
      const std::string& i, const DataMap& opts) {
    const auto id = getUniqueInterfaceIdentifier(i);
    auto p = this->material_property_interfaces_options.find(id);
    if (p == this->material_property_interfaces_options.end()) {
      this->material_property_interfaces_options.insert({id, opts});
    } else {
      p->second = merge(p->second, opts, false);
    }
  }  // end of addMaterialPropertyInterfaceOptions

  void ConfigurationManager::addBehaviourInterfaceOptions(const std::string& i,
                                                          const DataMap& opts) {
    const auto id = getUniqueInterfaceIdentifier(i);
    auto p = this->behaviour_interfaces_options.find(id);
    if (p == this->behaviour_interfaces_options.end()) {
      this->behaviour_interfaces_options.insert({id, opts});
    } else {
      p->second = merge(p->second, opts, false);
    }
  }  // end of addBehaviourInterfaceOptions

  void ConfigurationManager::addModelInterfaceOptions(const std::string& i,
                                                      const DataMap& opts) {
    const auto id = getUniqueInterfaceIdentifier(i);
    auto p = this->model_interfaces_options.find(id);
    if (p == this->model_interfaces_options.end()) {
      this->model_interfaces_options.insert({id, opts});
    } else {
      p->second = merge(p->second, opts, false);
    }
  }  // end of addModelInterfaceOptions

  tfel::utilities::DataMap
  ConfigurationManager::getMaterialPropertyInterfaceOptions(
      const std::string& i) const {
    auto opts = DataMap{};
    const auto id = getUniqueInterfaceIdentifier(i);
    auto p = this->interfaces_options.find(id);
    if (p != this->interfaces_options.end()) {
      opts = merge(opts, p->second, false);
    }
    auto p2 = this->material_property_interfaces_options.find(id);
    if (p2 != this->material_property_interfaces_options.end()) {
      opts = merge(opts, p2->second, false);
    }
    return opts;
  }  // end of getMaterialPropertyInterfaceOptions

  tfel::utilities::DataMap ConfigurationManager::getBehaviourInterfaceOptions(
      const std::string& i) const {
    auto opts = DataMap{};
    const auto id = getUniqueInterfaceIdentifier(i);
    auto p = this->interfaces_options.find(id);
    if (p != this->interfaces_options.end()) {
      opts = merge(opts, p->second, false);
    }
    auto p2 = this->behaviour_interfaces_options.find(id);
    if (p2 != this->behaviour_interfaces_options.end()) {
      opts = merge(opts, p2->second, false);
    }
    return opts;
  }  // end of getBehaviourInterfaceOptions

  tfel::utilities::DataMap ConfigurationManager::getModelInterfaceOptions(
      const std::string& i) const {
    auto opts = DataMap{};
    const auto id = getUniqueInterfaceIdentifier(i);
    auto p = this->interfaces_options.find(id);
    if (p != this->interfaces_options.end()) {
      opts = merge(opts, p->second, false);
    }
    auto p2 = this->model_interfaces_options.find(id);
    if (p2 != this->model_interfaces_options.end()) {
      opts = merge(opts, p2->second, false);
    }
    return opts;
  }  // end of getModelInterfaceOptions

  void ConfigurationManager::addCompilationOption(
      const Language l, const LanguageOptionCategory c, const std::string& o) {
    auto& options = this->compilation_options[l][c];
    options.insert(o);
  }  // end of addCompilationOptions

  void ConfigurationManager::addCompilationOptions(
      const Language l,
      const LanguageOptionCategory c,
      const std::vector<std::string>& opts) {
    auto& options = this->compilation_options[l][c];
    options.insert(opts.begin(), opts.end());
  }  // end of addCompilationOptions

  std::set<std::string> ConfigurationManager::getCompilationOptions(
      const Language l, const LanguageOptionCategory c) const {
    const auto p = this->compilation_options.find(l);
    if (p == this->compilation_options.end()) {
      return {};
    }
    const auto p2 = p->second.find(c);
    if (p2 == p->second.end()) {
      return {};
    }
    return p2->second;
  }  // end of getCompilationOptions

}  // end of namespace mfront