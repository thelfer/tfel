/*!
 * \file   mfront/src/MaterialKnowledgeDescription.cxx
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
#include "MFront/MaterialKnowledgeDescription.hxx"

namespace mfront {

  const char* const MaterialKnowledgeDescription::parametersAsStaticVariables =
      "parametersAsStaticVariables";
  const char* const MaterialKnowledgeDescription::buildIdentifier =
      "buildIdentifier";

  void MaterialKnowledgeDescription::throwUndefinedAttribute(
      const std::string& n) {
    tfel::raise(
        "MaterialKnowledgeDescription::getAttribute: "
        "no attribute named '" +
        n + "'");
  }  // end of throwUndefinedAttribute

  void MaterialKnowledgeDescription::setAttribute(
      const std::string& n, const MaterialKnowledgeAttribute& a, const bool b) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "MaterialKnowledgeDescription::setAttribute: " + m);
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

  void MaterialKnowledgeDescription::updateAttribute(
      const std::string& n, const MaterialKnowledgeAttribute& a) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "MaterialKnowledgeDescription::updateAttribute: " + m);
    };
    auto p = this->attributes.find(n);
    throw_if(p == this->attributes.end(), "unknown attribute '" + n + "'");
    throw_if(a.getTypeIndex() != p->second.getTypeIndex(),
             "attribute already exists with a different type");
    p->second = a;
  }  // end of setMaterialKnowledgeAttribute

  bool MaterialKnowledgeDescription::hasAttribute(const std::string& n) const {
    return this->attributes.count(n) != 0u;
  }  // end of hasAttribute

  const std::map<std::string, MaterialKnowledgeAttribute>&
  MaterialKnowledgeDescription::getAttributes() const {
    return this->attributes;
  }  // end of getAttributes

  bool areParametersTreatedAsStaticVariables(
      const MaterialKnowledgeDescription& h) {
    return h.getAttribute<bool>(
        MaterialKnowledgeDescription::parametersAsStaticVariables, false);
  }  // end of areParametersTreatedAsStaticVariables

}  // end of namespace mfront
