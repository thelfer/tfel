/*!
 * \file   mfront/src/MaterialPropertyInterfaceFactory.cxx
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
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "MFront/MaterialPropertyInterfaceFactory.hxx"

namespace mfront {

  MaterialPropertyInterfaceFactory&
  MaterialPropertyInterfaceFactory::getMaterialPropertyInterfaceFactory() {
    static MaterialPropertyInterfaceFactory f;
    return f;
  }  // end of
     // MaterialPropertyInterfaceFactory::getMaterialPropertyInterfaceFactory

  MaterialPropertyInterfaceFactory::InterfaceDependencyContainer&
  MaterialPropertyInterfaceFactory::getDependenciesMap() const {
    static InterfaceDependencyContainer map;
    return map;
  }  // end of MaterialPropertyInterfaceFactory::getDependenciesMap

  MaterialPropertyInterfaceFactory::InterfaceCreatorsContainer&
  MaterialPropertyInterfaceFactory::getInterfaceCreatorsMap() const {
    static InterfaceCreatorsContainer map;
    return map;
  }  // end of MaterialPropertyInterfaceFactory::getInterfaceCreatorsMap

  MaterialPropertyInterfaceFactory::AliasContainer&
  MaterialPropertyInterfaceFactory::getAliasesMap() const {
    static AliasContainer map;
    return map;
  }  // end of MaterialPropertyInterfaceFactory::getAliasesMap

  MaterialPropertyInterfaceFactory::MaterialPropertyInterfaceFactory() =
      default;

  std::vector<std::string>
  MaterialPropertyInterfaceFactory::getRegistredInterfaces() const {
    auto res = std::vector<std::string>{};
    for (const auto& a : this->getAliasesMap()) {
      res.push_back(a.first);
    }
    return res;
  }

  void MaterialPropertyInterfaceFactory::registerInterfaceCreator(
      const std::string& i,
      const MaterialPropertyInterfaceFactory::InterfaceCreator f) {
    auto& imap = this->getInterfaceCreatorsMap();
    tfel::raise_if(
        imap.find(i) != imap.end(),
        "MaterialPropertyInterfaceFactory::registerInterfaceCreator: "
        "interface creator '" +
            i + "' already declared");
    imap.insert(make_pair(i, f));
  }

  void MaterialPropertyInterfaceFactory::registerInterfaceAlias(
      const std::string& i, const std::string& a) {
    auto& amap = this->getAliasesMap();
    tfel::raise_if(
        amap.find(a) != amap.end(),
        "MaterialPropertyInterfaceFactory::registerInterfaceCreator: "
        "interface alias '" +
            a + "' already declared");
    amap.insert(make_pair(a, i));
  }

  void MaterialPropertyInterfaceFactory::registerInterfaceDependency(
      const std::string& name, const std::string& dep) {
    this->getDependenciesMap()[name].push_back(dep);
  }  // end of MaterialPropertyInterfaceFactory::registerInterfaceDependency

  std::vector<std::string>
  MaterialPropertyInterfaceFactory::getInterfaceDependencies(
      const std::string& name) const {
    std::vector<std::string> res;
    std::vector<std::string> tmp;
    auto p = this->getAliasesMap().find(name);
    if (p == this->getAliasesMap().end()) {
      auto msg = std::string(
          "MaterialPropertyInterfaceFactory::getInterfaceDependencies: ");
      msg += "no interface named '" + name + "'.\n";
      msg += "Available interface are : \n";
      for (p = this->getAliasesMap().begin(); p != this->getAliasesMap().end();
           ++p) {
        msg += p->first + " ";
      }
      tfel::raise(msg);
    }
    const auto& deps = this->getDependenciesMap()[p->second];
    copy(deps.begin(), deps.end(), back_inserter(tmp));
    for (const auto& d : deps) {
      const auto& deps2 = this->getInterfaceDependencies(d);
      std::copy(deps2.begin(), deps2.end(), std::back_inserter(tmp));
    }
    std::unique_copy(tmp.begin(), tmp.end(), std::back_inserter(res));
    return res;
  }  // end of MaterialPropertyInterfaceFactory::getInterfaceDependencies

  bool MaterialPropertyInterfaceFactory::exists(const std::string& n) const {
    return this->getAliasesMap().count(n) != 0;
  }  // end of MaterialPropertyInterfaceFactory::exists

  std::shared_ptr<AbstractMaterialPropertyInterface>
  MaterialPropertyInterfaceFactory::getInterface(
      const std::string& interfaceName) const {
    auto p2 = this->getAliasesMap().find(interfaceName);
    if (p2 == this->getAliasesMap().end()) {
      auto msg =
          std::string("MaterialPropertyInterfaceFactory::createNewInterface: ");
      msg += "no interface named '" + interfaceName + "'.\n";
      msg += "Available interface are : \n";
      for (p2 = this->getAliasesMap().begin();
           p2 != this->getAliasesMap().end(); ++p2) {
        msg += p2->first + " ";
      }
      tfel::raise(msg);
    }
    auto p = this->getInterfaceCreatorsMap().find(p2->second);
    assert(p != this->getInterfaceCreatorsMap().end());
    auto c = p->second;
    return c();
  }

  MaterialPropertyInterfaceFactory::~MaterialPropertyInterfaceFactory() =
      default;

}  // end of namespace mfront
