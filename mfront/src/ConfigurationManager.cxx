/*!
 * \file   ConfigurationManager.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include <optional>
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/SearchPathsHandler.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MaterialPropertyInterfaceFactory.hxx"
#include "MFront/BehaviourInterfaceFactory.hxx"
#include "MFront/ModelInterfaceFactory.hxx"
#include "MFront/ConfigurationManager.hxx"

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

  ConfigurationManager::Language ConfigurationManager::getLanguage(
      std::string_view s) {
    if ((s == "c++") || (s == "cxx") || (s == "CXX") || (s == "C++")) {
      return ConfigurationManager::CXX;
    }
    if ((s == "c") || (s == "C")) {
      return ConfigurationManager::C;
    }
    if ((s == "fortran") || (s == "Fortran") || (s == "FORTRAN")) {
      return ConfigurationManager::FORTRAN;
    }
    if ((s == "cuda") || (s == "CUDA")) {
      return ConfigurationManager::CUDA;
    }
    if ((s == "hip") || (s == "HIP")) {
      return ConfigurationManager::HIP;
    }
    if (!((s == "sycl") || (s == "SYCL"))) {
      tfel::raise("unsupported language '" + std::string{s} + "'");
    }
    return ConfigurationManager::SYCL;
  }

  ConfigurationManager::LanguageOptionCategory
  ConfigurationManager::getLanguageOptionsCategory(std::string_view s) {
    if (s == "preprocessor_flags") {
      return ConfigurationManager::PREPROCESSOR_FLAGS;
    }
    if (s != "compilation_flags") {
      tfel::raise("unsupported language option category '" + std::string{s} +
                  "'");
    }
    return ConfigurationManager::COMPILATION_FLAGS;
  }  // end of getLanguageOptionsCategory

  ConfigurationManager::LinkerOptionCategory
  ConfigurationManager::getLinkerOptionsCategory(std::string_view s) {
    if (s == "linker_flags") {
      return ConfigurationManager::LINKER_FLAGS;
    }
    if (s == "link_paths") {
      return ConfigurationManager::LINK_PATHS;
    }
    if (s != "link_libraries") {
      tfel::raise("unsupported linker option category '" + std::string{s} +
                  "'");
    }
    return ConfigurationManager::LINK_LIBRARIES;
  }  // end of getLinkerOptionsCategory

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

  void ConfigurationManager::setCompiler(const Language l,
                                         const std::string& c) {
    const auto p = this->compilers.find(l);
    if (p != this->compilers.end()) {
      if (p->second != c) {
        tfel::raise("inconsistent compilers ('" + p->second +
                    "' is distinct from '" + c + "')");
      }
      return;
    }
    this->compilers.insert({l, c});
  }  // end of setCompiler

  std::optional<std::string> ConfigurationManager::getCompiler(
      const Language l) const {
    const auto p = this->compilers.find(l);
    if (p == this->compilers.end()) {
      return {};
    }
    return p->second;
  }  // end of getCompiler

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

  std::optional<std::set<std::string>>
  ConfigurationManager::getCompilationOptions(
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

  void ConfigurationManager::addLinkerOption(const LinkerOptionCategory c,
                                             const std::string& o) {
    auto& options = this->linker_options[c];
    options.insert(o);
  }  // end of addLinkerOptions

  void ConfigurationManager::addLinkerOptions(
      const LinkerOptionCategory c, const std::vector<std::string>& opts) {
    auto& options = this->linker_options[c];
    options.insert(opts.begin(), opts.end());
  }  // end of addLinkerOptions

  std::optional<std::set<std::string>> ConfigurationManager::getLinkerOptions(
      const LinkerOptionCategory c) const {
    const auto p = this->linker_options.find(c);
    if (p == this->linker_options.end()) {
      return {};
    }
    return p->second;
  }  // end of getLinkerOptions

  void ConfigurationManager::addIncludePath(const std::string& p) {
    this->include_paths.insert(p);
  }  // end of addIncludePaths

  void ConfigurationManager::addIncludePaths(
      const std::vector<std::string>& paths) {
    for (const auto& p : paths) {
      this->addIncludePath(p);
    }
  }  // end of addIncludePaths

  std::set<std::string> ConfigurationManager::getIncludePaths() const {
    return this->include_paths;
  }  // end of getIncludePaths

  tfel::utilities::CxxTokenizerOptions
  getConfigurationParsingOptions() noexcept {
    auto opts = tfel::utilities::CxxTokenizerOptions{};
    opts.dotAsSeparator = false;
    opts.plusAsSeparator = false;
    opts.minusAsSeparator = false;
    return opts;
  }  // end of getConfigurationParsingOptions

  tfel::utilities::DataMap readConfigurationFile(const std::string& f) {
    std::ifstream ifs(f);
    if (!ifs) {
      tfel::raise("readConfigurationFile: can't open file '" + f + "'");
    }
    auto t = tfel::utilities::CxxTokenizer{getConfigurationParsingOptions()};
    t.parseString('{' + std::string{std::istreambuf_iterator<char>{ifs}, {}} +
                  '}');
    auto b = t.begin();
    return read<tfel::utilities::DataMap>(b, t.end());
  }  // end ofreadConfigurationFile

  void parseConfigurationFile(const std::string& f) {
    using tfel::utilities::DataMap;
    const auto file = SearchPathsHandler::search(f);
    const auto data = readConfigurationFile(file);
    auto validator = tfel::utilities::DataMapValidator{};
    validator.addDataTypeValidator<DataMap>("dsl_options")
        .addDataTypeValidator<DataMap>("material_property_dsl_options")
        .addDataTypeValidator<DataMap>("behaviour_dsl_options")
        .addDataTypeValidator<DataMap>("model_dsl_options")
        .addDataTypeValidator<DataMap>("interfaces_options")
        .addDataTypeValidator<DataMap>("compilation_options")
        .addDataTypeValidator<DataMap>("linking_options");
    validator.validate(data);
    auto& m = ConfigurationManager::get();
    try {
      if (data.contains("dsl_options")) {
        auto& g = static_cast<GlobalDomainSpecificLanguageOptionsManager&>(m);
        for (const auto& [k, d] : data.at("dsl_options").get<DataMap>()) {
          g.addDSLOption(k, d);
        }
      }
      if (data.contains("material_property_dsl_options")) {
        auto& g = static_cast<GlobalDomainSpecificLanguageOptionsManager&>(m);
        for (const auto& [k, d] :
             data.at("material_property_dsl_options").get<DataMap>()) {
          g.addMaterialPropertyDSLOption(k, d);
        }
      }
      if (data.contains("behaviour_dsl_options")) {
        auto& g = static_cast<GlobalDomainSpecificLanguageOptionsManager&>(m);
        for (const auto& [k, d] :
             data.at("behaviour_dsl_options").get<DataMap>()) {
          g.addBehaviourDSLOption(k, d);
        }
      }
      if (data.contains("model_dsl_options")) {
        auto& g = static_cast<GlobalDomainSpecificLanguageOptionsManager&>(m);
        for (const auto& [k, d] : data.at("model_dsl_options").get<DataMap>()) {
          g.addModelDSLOption(k, d);
        }
      }
      if (data.contains("interfaces_options")) {
        for (const auto& [i, opts] :
             data.at("interfaces_options").get<DataMap>()) {
          if (!opts.is<tfel::utilities::DataMap>()) {
            tfel::raise("invalid options types for interface '" + i + "'");
          }
          if (getVerboseMode() >= VERBOSE_DEBUG) {
            getLogStream() << "treating configuration options for material "
                           << "property interface '" << i << " from file '"
                           << file << "''\n";
          }
          m.addInterfaceOptions(i, opts);
        }
      }
      if (data.contains("material_property_interfaces_options")) {
        for (const auto& [i, opts] :
             data.at("material_property_interfaces_options").get<DataMap>()) {
          if (!opts.is<tfel::utilities::DataMap>()) {
            tfel::raise("invalid options types for interface '" + i + "'");
          }
          if (getVerboseMode() >= VERBOSE_DEBUG) {
            getLogStream() << "treating configuration options for material "
                           << "property interface '" << i << " from file '"
                           << file << "''\n";
          }
          m.addMaterialPropertyInterfaceOptions(i, opts);
        }
      }
      if (data.contains("behaviour_interfaces_options")) {
        for (const auto& [i, opts] :
             data.at("behaviour_interfaces_options").get<DataMap>()) {
          if (!opts.is<tfel::utilities::DataMap>()) {
            tfel::raise("invalid options types for interface '" + i + "'");
          }
          if (getVerboseMode() >= VERBOSE_DEBUG) {
            getLogStream()
                << "treating configuration options for behaviour interface '"
                << i << " from file '" << file << "''\n";
          }
          m.addBehaviourInterfaceOptions(i, opts);
        }
      }
      if (data.contains("model_interfaces_options")) {
        for (const auto& [i, opts] :
             data.at("model_interfaces_options").get<DataMap>()) {
          if (!opts.is<tfel::utilities::DataMap>()) {
            tfel::raise("invalid options types for interface '" + i + "'");
          }
          if (getVerboseMode() >= VERBOSE_DEBUG) {
            getLogStream()
                << "treating configuration options for model interface '" << i
                << " from file '" << file << "''\n";
          }
          m.addModelInterfaceOptions(i, opts);
        }
      }
      if (data.contains("compilation_options")) {
        for (const auto& [l, opts] :
             data.at("compilation_options").get<DataMap>()) {
          if (!opts.is<tfel::utilities::DataMap>()) {
            tfel::raise("invalid options types for language'" + l + "'");
          }
          const auto language = ConfigurationManager::getLanguage(l);
          for (const auto& [c, o] : opts.get<tfel::utilities::DataMap>()) {
            if (c == "compiler") {
              if (!o.is<std::string>()) {
                tfel::raise(
                    "unsupported option type for the 'compiler' option for "
                    "language '" +
                    l + "': expected a string");
              }
              m.setCompiler(language, o.get<std::string>());
            } else {
              const auto category =
                  ConfigurationManager::getLanguageOptionsCategory(c);
              if (o.is<std::string>()) {
                m.addCompilationOption(language, category,
                                       o.get<std::string>());
              } else if (is_convertible<std::vector<std::string>>(o)) {
                m.addCompilationOptions(language, category,
                                        convert<std::vector<std::string>>(o));
              } else {
                tfel::raise("unsupported option type for category '" + c +
                            "' for language '" + l +
                            "': expected a string or an array of strings");
              }
            }
          }
          if (getVerboseMode() >= VERBOSE_DEBUG) {
            getLogStream() << "treating configuration options for language '"
                           << l << "' from file '" << file << "'\n";
          }
        }
      }
      if (data.contains("linking_options")) {
        auto linking_options_validator = tfel::utilities::DataMapValidator{};
        linking_options_validator.addDataValidator(
            "linker_flags", [](const tfel::utilities::Data& d) {
              return d.is<std::string>() ||
                     is_convertible<std::vector<std::string>>(d);
            });
        for (const auto& [n, o] : data.at("linking_options").get<DataMap>()) {
          if (n == "linker_flags") {
            if (o.is<std::string>()) {
              m.addLinkerOption(ConfigurationManager::LINKER_FLAGS,
                                o.get<std::string>());
            } else if (is_convertible<std::vector<std::string>>(o)) {
              m.addLinkerOptions(ConfigurationManager::LINKER_FLAGS,
                                 convert<std::vector<std::string>>(o));
            } else {
              tfel::raise(
                  "unsupported type for linker_flags: "
                  "expected a string or an array of strings");
            }
          } else {
            tfel::raise("unsupported linking options '" + n + '\'');
          }
        }
      }
    } catch (std::exception& e) {
      tfel::raise("Error while parsing configuration file '" + f +
                  "': " + e.what());
    } catch (...) {
      tfel::raise("Error while parsing configuration file '" + f +
                  "': unknown exception");
    }
  }  // end of parseConfigurationFile

}  // end of namespace mfront