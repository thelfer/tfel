/*!
 * \file   mfront/src/BehaviourInterfaceFactory.cxx
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
#include "MFront/BehaviourInterfaceFactory.hxx"

namespace mfront {

  BehaviourInterfaceFactory&
  BehaviourInterfaceFactory::getBehaviourInterfaceFactory() {
    static BehaviourInterfaceFactory f;
    return f;
  }  // end of BehaviourInterfaceFactory::getBehaviourInterfaceFactory

  BehaviourInterfaceFactory::InterfaceCreatorsContainer&
  BehaviourInterfaceFactory::getInterfaceCreatorsMap() const {
    static InterfaceCreatorsContainer map;
    return map;
  }  // end of BehaviourInterfaceFactory::getInterfaceCreatorsMap

  std::vector<std::string> BehaviourInterfaceFactory::getRegistredInterfaces()
      const {
    auto res = std::vector<std::string>{};
    for (const auto& p : this->getInterfaceCreatorsMap()) {
      res.push_back(p.first);
    }
    return res;
  }

  bool BehaviourInterfaceFactory::exists(const std::string& n) const {
    return this->getAliasesMap().count(n) != 0;
  }  // end of BehaviourInterfaceFactory::exists

  void BehaviourInterfaceFactory::registerInterfaceCreator(
      const std::string& i,
      const BehaviourInterfaceFactory::InterfaceCreator f) {
    auto& imap = this->getInterfaceCreatorsMap();
    tfel::raise_if(imap.find(i) != imap.end(),
                   "BehaviourInterfaceFactory::registerInterfaceCreator: "
                   "interface creator '" +
                       i + "' already declared");
    imap.insert({i, f});
    this->registerInterfaceAlias(i, i);
  }

  void BehaviourInterfaceFactory::registerInterfaceAlias(const std::string& i,
                                                         const std::string& a) {
    auto& amap = this->getAliasesMap();
    tfel::raise_if(amap.find(a) != amap.end(),
                   "BehaviourInterfaceFactory::registerInterfaceAlias: "
                   "interface alias '" +
                       a + "' already declared");
    auto& imap = this->getInterfaceCreatorsMap();
    tfel::raise_if(imap.find(i) == imap.end(),
                   "BehaviourInterfaceFactory::registerInterfaceAlias: "
                   "no interface named '" +
                       i + "' declared");
    amap.insert({a, i});
  }

  std::shared_ptr<AbstractBehaviourInterface>
  BehaviourInterfaceFactory::getInterface(const std::string& n) {
    auto p2 = this->getAliasesMap().find(n);
    if (p2 == this->getAliasesMap().end()) {
      auto msg = std::string(
          "BehaviourInterfaceFactory::createNewInterface:"
          " no interface named '");
      msg += n + "'.\n";
      msg += "Available interfaces are : \n";
      for (p2 = this->getAliasesMap().begin();
           p2 != this->getAliasesMap().end(); ++p2) {
        msg += p2->first + " ";
      }
      tfel::raise(msg);
    }
    const auto p = this->getInterfaceCreatorsMap().find(p2->second);
    assert(p != this->getInterfaceCreatorsMap().end());
    auto c = p->second;
    return c();
  }

  BehaviourInterfaceFactory::~BehaviourInterfaceFactory() = default;

  BehaviourInterfaceFactory::AliasContainer&
  BehaviourInterfaceFactory::getAliasesMap() const {
    static AliasContainer map;
    return map;
  }  // end of BehaviourInterfaceFactory::getAliasesMap

}  // end of namespace mfront
