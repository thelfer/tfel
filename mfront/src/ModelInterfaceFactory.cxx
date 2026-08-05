/*!
 * \file   mfront/src/ModelInterfaceFactory.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cassert>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "MFront/ModelInterfaceFactory.hxx"

namespace mfront {

  ModelInterfaceFactory& ModelInterfaceFactory::getModelInterfaceFactory() {
    static ModelInterfaceFactory f;
    return f;
  }  // end of
     // ModelInterfaceFactory::getModelInterfaceFactory

  ModelInterfaceFactory::InterfaceDependencyContainer&
  ModelInterfaceFactory::getDependenciesMap() const {
    static InterfaceDependencyContainer map;
    return map;
  }  // end of ModelInterfaceFactory::getDependenciesMap

  ModelInterfaceFactory::InterfaceCreatorsContainer&
  ModelInterfaceFactory::getInterfaceCreatorsMap() const {
    static InterfaceCreatorsContainer map;
    return map;
  }  // end of ModelInterfaceFactory::getInterfaceCreatorsMap

  ModelInterfaceFactory::AliasContainer& ModelInterfaceFactory::getAliasesMap()
      const {
    static AliasContainer map;
    return map;
  }  // end of ModelInterfaceFactory::getAliasesMap

  ModelInterfaceFactory::ModelInterfaceFactory() = default;

  std::vector<std::string> ModelInterfaceFactory::getRegistredInterfaces()
      const {
    auto res = std::vector<std::string>{};
    for (const auto& a : this->getAliasesMap()) {
      res.push_back(a.first);
    }
    return res;
  }

  void ModelInterfaceFactory::registerInterfaceCreator(
      const std::string& i, const ModelInterfaceFactory::InterfaceCreator f) {
    auto& imap = this->getInterfaceCreatorsMap();
    tfel::raise_if(imap.find(i) != imap.end(),
                   "ModelInterfaceFactory::registerInterfaceCreator: "
                   "interface creator '" +
                       i + "' already declared");
    imap.insert(make_pair(i, f));
  }

  void ModelInterfaceFactory::registerInterfaceAlias(const std::string& i,
                                                     const std::string& a) {
    auto& amap = this->getAliasesMap();
    tfel::raise_if(amap.find(a) != amap.end(),
                   "ModelInterfaceFactory::registerInterfaceCreator: "
                   "interface alias '" +
                       a + "' already declared");
    amap.insert(make_pair(a, i));
  }

  void ModelInterfaceFactory::registerInterfaceDependency(
      const std::string& name, const std::string& dep) {
    this->getDependenciesMap()[name].push_back(dep);
  }  // end of ModelInterfaceFactory::registerInterfaceDependency

  std::string ModelInterfaceFactory::getUniqueNameFromAlias(
      const std::string& n) const {
    auto p = this->getAliasesMap().find(n);
    if (p == this->getAliasesMap().end()) {
      auto msg = std::string("ModelFactory::getUniqueNameFromAlias: ");
      msg += "no interface named '" + n + "'.\n";
      msg += "Available interface are : \n";
      for (p = this->getAliasesMap().begin(); p != this->getAliasesMap().end();
           ++p) {
        msg += p->first + " ";
      }
      tfel::raise(msg);
    }
    return p->second;
  }  // end of getUniqueNameFromAlias

  std::vector<std::string> ModelInterfaceFactory::getInterfaceDependencies(
      const std::string& name) const {
    std::vector<std::string> res;
    const auto id = this->getUniqueNameFromAlias(name);
    const auto& deps = this->getDependenciesMap()[id];
    std::vector<std::string> tmp;
    std::copy(deps.begin(), deps.end(), back_inserter(tmp));
    for (const auto& d : deps) {
      const auto& deps2 = this->getInterfaceDependencies(d);
      std::copy(deps2.begin(), deps2.end(), std::back_inserter(tmp));
    }
    std::unique_copy(tmp.begin(), tmp.end(), std::back_inserter(res));
    return res;
  }  // end of ModelInterfaceFactory::getInterfaceDependencies

  bool ModelInterfaceFactory::exists(const std::string& n) const {
    return this->getAliasesMap().count(n) != 0;
  }  // end of ModelInterfaceFactory::exists

  std::shared_ptr<AbstractModelInterface> ModelInterfaceFactory::getInterface(
      const std::string& interfaceName) const {
    const auto id = this->getUniqueNameFromAlias(interfaceName);
    const auto p = this->getInterfaceCreatorsMap().find(id);
    assert(p != this->getInterfaceCreatorsMap().end());
    auto c = p->second;
    return c();
  }

  ModelInterfaceFactory::~ModelInterfaceFactory() = default;

}  // end of namespace mfront
