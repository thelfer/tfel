/*!
 * \file   mfront/src/GlobalDomainSpecificLanguageOptionsManager.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17/01/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/GlobalDomainSpecificLanguageOptionsManager.hxx"

namespace mfront {

  static void add_option(tfel::utilities::DataMap& options,
                         const std::string& o,
                         const tfel::utilities::Data& v) {
    if (options.find(o) != options.end()) {
      tfel::raise("add_option: option '" + o + "' already defined");
    }
    options.insert({o, v});
  }  // end of add_option

  static void add_option(tfel::utilities::DataMap& options,
                         const std::string& o,
                         const std::string& v) {
    tfel::utilities::CxxTokenizer t;
    t.parseString(v);
    auto b = t.begin();
    add_option(options, o, tfel::utilities::Data::read(b, t.end()));
  }  // end of add_option

  GlobalDomainSpecificLanguageOptionsManager&
  GlobalDomainSpecificLanguageOptionsManager::get() {
    static GlobalDomainSpecificLanguageOptionsManager m;
    return m;
  }  // end of get

  void GlobalDomainSpecificLanguageOptionsManager::addDSLOption(
      const std::string& n, const std::string& v) {
    this->addMaterialPropertyDSLOption(n, v);
    this->addBehaviourDSLOption(n, v);
    this->addModelDSLOption(n, v);
  }  // end of addDSLOption

  void GlobalDomainSpecificLanguageOptionsManager::addMaterialPropertyDSLOption(
      const std::string& n, const std::string& v) {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "GlobalDomainSpecificLanguageOptionsManager::"
                     << "addMaterialPropertyDSLOption: "
                     << "adding option '" << n << "' with value '" << v
                     << "'\n";
    }
    add_option(this->material_property_dsl_options, n, v);
  }  // end of addMaterialPropertyDSLOption

  void GlobalDomainSpecificLanguageOptionsManager::addBehaviourDSLOption(
      const std::string& n, const std::string& v) {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "GlobalDomainSpecificLanguageOptionsManager::"
                     << "addBehaviourDSLOption: adding option '" << n
                     << "' with value '" << v << "'\n";
    }
    add_option(this->behaviour_dsl_options, n, v);
  }  // end of addBehaviourDSLOption

  void GlobalDomainSpecificLanguageOptionsManager::addModelDSLOption(
      const std::string& n, const std::string& v) {
    if (getVerboseMode() >= VERBOSE_LEVEL2) {
      getLogStream() << "GlobalDomainSpecificLanguageOptionsManager::"
                     << "addModelDSLOption: adding option '" << n
                     << "' with value '" << v << "'\n";
    }
    add_option(this->model_dsl_options, n, v);
  }  // end of addModelDSLOption

  void GlobalDomainSpecificLanguageOptionsManager::addDSLOption(
      const std::string& n, const tfel::utilities::Data& d) {
    this->addMaterialPropertyDSLOption(n, d);
    this->addBehaviourDSLOption(n, d);
    this->addModelDSLOption(n, d);
  }  // end of addDSLOption

  void GlobalDomainSpecificLanguageOptionsManager::addMaterialPropertyDSLOption(
      const std::string& n, const tfel::utilities::Data& d) {
    add_option(this->material_property_dsl_options, n, d);
  }  // end of addMaterialPropertyDSLOption

  void GlobalDomainSpecificLanguageOptionsManager::addBehaviourDSLOption(
      const std::string& n, const tfel::utilities::Data& d) {
    add_option(this->behaviour_dsl_options, n, d);
  }  // end of addBehaviourDSLOption

  void GlobalDomainSpecificLanguageOptionsManager::addModelDSLOption(
      const std::string& n, const tfel::utilities::Data& d) {
    add_option(this->model_dsl_options, n, d);
  }  // end of addModelDSLOption

  tfel::utilities::DataMap
  GlobalDomainSpecificLanguageOptionsManager::getMaterialPropertyDSLOptions()
      const {
    return this->material_property_dsl_options;
  }

  tfel::utilities::DataMap
  GlobalDomainSpecificLanguageOptionsManager::getBehaviourDSLOptions() const {
    return this->behaviour_dsl_options;
  }  // end of getBehaviourDSLOptions

  tfel::utilities::DataMap
  GlobalDomainSpecificLanguageOptionsManager::getModelDSLOptions() const {
    return this->model_dsl_options;
  }  // end of getModelDSLOptions

  GlobalDomainSpecificLanguageOptionsManager::
      GlobalDomainSpecificLanguageOptionsManager() = default;

  GlobalDomainSpecificLanguageOptionsManager::
      ~GlobalDomainSpecificLanguageOptionsManager() = default;

}  // namespace mfront
