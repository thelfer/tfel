/*!
 * \file   mfront/src/MaterialKnowledgeDescriptionAttribute.cxx
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
#include "MFront/MaterialKnowledgeDescriptionAttribute.hxx"

namespace mfront {

  const char* const MaterialKnowledgeDescriptionAttributesHandler::
      parametersAsStaticVariables = "parametersAsStaticVariables";

  void MaterialKnowledgeDescriptionAttributesHandler::throwUndefinedAttribute(
      const std::string& n) {
    tfel::raise(
        "MaterialKnowledgeDescriptionAttributesHandler::getAttribute: "
        "no attribute named '" +
        n + "'");
  }  // end of throwUndefinedAttribute

  void MaterialKnowledgeDescriptionAttributesHandler::setAttribute(
      const std::string& n,
      const MaterialKnowledgeDescriptionAttribute& a,
      const bool b) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(
          c,
          "MaterialKnowledgeDescriptionAttributesHandler::setAttribute: " + m);
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

  void MaterialKnowledgeDescriptionAttributesHandler::updateAttribute(
      const std::string& n, const MaterialKnowledgeDescriptionAttribute& a) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(
          c,
          "MaterialKnowledgeDescriptionAttributesHandler::updateAttribute: " +
              m);
    };
    auto p = this->attributes.find(n);
    throw_if(p == this->attributes.end(), "unknown attribute '" + n + "'");
    throw_if(a.getTypeIndex() != p->second.getTypeIndex(),
             "attribute already exists with a different type");
    p->second = a;
  }  // end of setMaterialKnowledgeDescriptionAttribute

  bool MaterialKnowledgeDescriptionAttributesHandler::hasAttribute(
      const std::string& n) const {
    return this->attributes.count(n) != 0u;
  }  // end of hasAttribute

  const std::map<std::string, MaterialKnowledgeDescriptionAttribute>&
  MaterialKnowledgeDescriptionAttributesHandler::getAttributes() const {
    return this->attributes;
  }  // end of getAttributes

}  // end of namespace mfront
