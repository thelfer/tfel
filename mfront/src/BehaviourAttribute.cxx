/*!
 * \file   mfront/src/BehaviourAttribute.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   10/01/2022
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourAttribute.hxx"

namespace mfront{

  void BehaviourAttributesHandler::throwUndefinedAttribute(const std::string& n) {
    tfel::raise(
        "BehaviourAttributesHandler::getAttribute: "
        "no attribute named '" +
        n + "'");
  }  // end of throwUndefinedAttribute

  void BehaviourAttributesHandler::setAttribute(const std::string& n,
                                   const BehaviourAttribute& a,
                                   const bool b) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourAttributesHandler::setAttribute: " + m);
    };
    auto p = this->attributes.find(n);
    if (p != this->attributes.end()) {
      throw_if(a.getTypeIndex() != p->second.getTypeIndex(),
               "attribute already exists with a different type");
    }
    if (!this->attributes.insert({n, a}).second) {
      throw_if(!b, "attribute '" + n + "' already declared");
    }
  }  // end of setAttribute

  void BehaviourAttributesHandler::updateAttribute(const std::string& n,
                                      const BehaviourAttribute& a) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "BehaviourAttributesHandler::updateAttribute: " + m);
    };
    auto p = this->attributes.find(n);
    throw_if(p == this->attributes.end(), "unknown attribute '" + n + "'");
    throw_if(a.getTypeIndex() != p->second.getTypeIndex(),
             "attribute already exists with a different type");
    p->second = a;
  }  // end of setAttribute

  bool BehaviourAttributesHandler::hasAttribute(const std::string& n) const {
    return this->attributes.count(n) != 0u;
  }  // end of hasAttribute

  const std::map<std::string, BehaviourAttribute>&
  BehaviourAttributesHandler::getAttributes() const {
    return this->attributes;
  }  // end of getAttributes

} // end of namespace mfront