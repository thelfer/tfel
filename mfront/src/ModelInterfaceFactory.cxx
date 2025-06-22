/*!
 * \file   mfront/src/ModelInterfaceFactory.cxx
 *
 * \brief
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include <cassert>

#include "MFront/ModelInterfaceFactory.hxx"

namespace mfront {

  ModelInterfaceFactory& ModelInterfaceFactory::getModelInterfaceFactory() {
    static ModelInterfaceFactory f;
    return f;
  }  // end of ModelInterfaceFactory::getModelInterfaceFactory

  ModelInterfaceFactory::InterfaceCreatorsContainer&
  ModelInterfaceFactory::getInterfaceCreatorsMap() const {
    static InterfaceCreatorsContainer map;
    return map;
  }  // end of ModelInterfaceFactory::getInterfaceCreatorsMap

  ModelInterfaceFactory::ModelInterfaceFactory() = default;

  std::vector<std::string> ModelInterfaceFactory::getRegistredInterfaces()
      const {
    using namespace std;
    auto res = vector<string>{};
    for (const auto& c : this->getInterfaceCreatorsMap()) {
      res.push_back(c.first);
    }
    return res;
  }

  bool ModelInterfaceFactory::exists(const std::string& n) const {
    return this->getInterfaceCreatorsMap().count(n) != 0;
  }  // end of ModelInterfaceFactory::exists

  void ModelInterfaceFactory::registerInterfaceCreator(
      const std::string& i, const ModelInterfaceFactory::InterfaceCreator f) {
    using namespace std;
    auto& imap = this->getInterfaceCreatorsMap();
    if (imap.find(i) != imap.end()) {
      string msg("ModelInterfaceFactory::registerInterfaceCreator : ");
      msg += "interface creator '" + i + "' already declared";
      throw(runtime_error(msg));
    }
    imap.insert({i, f});
  }

  std::shared_ptr<AbstractModelInterface> ModelInterfaceFactory::getInterface(
      const std::string& n) const {
    using namespace std;
    auto p = this->getInterfaceCreatorsMap().find(n);
    if (p == this->getInterfaceCreatorsMap().end()) {
      string msg =
          "ModelInterfaceFactory::createNewInterface : no interface named '";
      msg += n + "'.\n";
      msg += "Available interface are : \n";
      for (p = this->getInterfaceCreatorsMap().begin();
           p != this->getInterfaceCreatorsMap().end(); ++p) {
        msg += p->first + " ";
      }
      throw(runtime_error(msg));
    }
    InterfaceCreator c = p->second;
    return c();
  }

  ModelInterfaceFactory::~ModelInterfaceFactory() = default;

}  // end of namespace mfront
