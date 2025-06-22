/*!
 * \file   mfront/src/VariableDescription.cxx
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

#include <sstream>
#include <stdexcept>

#include "MFront/MFrontLogStream.hxx"
#include "MFront/VariableDescription.hxx"

namespace mfront {

  const std::string VariableDescription::depth("depth");
  const std::string VariableDescription::bound("bound");
  const std::string VariableDescription::physicalBound("physicalBound");
  const std::string VariableDescription::initialValue("initialValue");
  const std::string VariableDescription::defaultValue("defaultValue");

  VariableDescription::VariableDescription() = default;

  VariableDescription::VariableDescription(const std::string& t,
                                           const std::string& n,
                                           const unsigned short s,
                                           const size_t l)
      : type(t), name(n), arraySize(s), lineNumber(l) {
    if (this->arraySize == 0) {
      throw(
          std::runtime_error("VariableDescription::VariableDescription: "
                             "invalid array size"));
    }
  }  // end of VariableDescription::VariableDescription

  VariableDescription::VariableDescription(const VariableDescription&) =
      default;
  VariableDescription::VariableDescription(VariableDescription&&) = default;
  VariableDescription& VariableDescription::operator=(VariableDescription&&) =
      default;
  VariableDescription& VariableDescription::operator=(
      const VariableDescription&) = default;

  const std::string& VariableDescription::getExternalName(
      const std::map<std::string, std::string>& glossaryNames,
      const std::map<std::string, std::string>& entryNames) const {
    auto p2 = glossaryNames.find(this->name);
    auto p3 = entryNames.find(this->name);
    if (p2 != glossaryNames.end()) {
      return p2->second;
    } else if (p3 != entryNames.end()) {
      return p3->second;
    }
    return this->name;
  }  // end of VariableDescription::getExternalName

  void VariableDescription::throwUndefinedAttribute(const std::string& n) {
    throw(
        std::runtime_error("VariableDescription::getAttribute : "
                           "no attribute named '" +
                           n + "'"));
  }  // end of VariableDescription::throwUndefinedAttribute

  void VariableDescription::setAttribute(const std::string& n,
                                         const VariableAttribute& a,
                                         const bool b) {
    auto throw_if = [](const bool c, const std::string& m) {
      if (c) {
        throw(std::runtime_error("VariableDescription::setAttribute: " + m));
      }
    };
    if (b) {
      auto p = this->attributes.find(n);
      if (p != this->attributes.end()) {
        throw_if(a.getTypeIndex() != p->second.getTypeIndex(),
                 "attribute already exists with a different type");
        return;
      }
    }
    throw_if(!this->attributes.insert({n, a}).second,
             "attribute '" + n + "' already declared");
  }  // end of VariableDescription::setAttribute

  bool VariableDescription::hasAttribute(const std::string& n) const {
    return this->attributes.count(n) != 0u;
  }  // end of VariableDescription::hasAttribute

  const std::map<std::string, VariableAttribute>&
  VariableDescription::getAttributes() const {
    return this->attributes;
  }  // end of VariableDescription::getAttributes

  VariableDescription::~VariableDescription() = default;

  VariableDescriptionContainer::VariableDescriptionContainer() = default;

  VariableDescriptionContainer::VariableDescriptionContainer(
      const std::initializer_list<VariableDescription>& l)
      : std::vector<VariableDescription>(l) {}

  VariableDescriptionContainer::VariableDescriptionContainer(
      VariableDescriptionContainer&&) = default;
  VariableDescriptionContainer::VariableDescriptionContainer(
      const VariableDescriptionContainer&) = default;
  VariableDescriptionContainer& VariableDescriptionContainer::operator=(
      VariableDescriptionContainer&&) = default;
  VariableDescriptionContainer& VariableDescriptionContainer::operator=(
      const VariableDescriptionContainer&) = default;

  bool VariableDescriptionContainer::contains(const std::string& n) const {
    for (const auto& v : *this) {
      if (v.name == n) {
        return true;
      }
    }
    return false;
  }  // end of VariableDescriptionContainer::contains

  std::vector<std::string> VariableDescriptionContainer::getExternalNames(
      const std::map<std::string, std::string>& glossaryNames,
      const std::map<std::string, std::string>& entryNames) const {
    std::vector<std::string> n;
    this->appendExternalNames(n, glossaryNames, entryNames);
    return n;
  }

  void VariableDescriptionContainer::getExternalNames(
      std::vector<std::string>& n,
      const std::map<std::string, std::string>& glossaryNames,
      const std::map<std::string, std::string>& entryNames) const {
    n.clear();
    this->appendExternalNames(n, glossaryNames, entryNames);
  }

  void VariableDescriptionContainer::appendExternalNames(
      std::vector<std::string>& n,
      const std::map<std::string, std::string>& glossaryNames,
      const std::map<std::string, std::string>& entryNames) const {
    for (const auto& v : *this) {
      const auto name = v.getExternalName(glossaryNames, entryNames);
      if (v.arraySize == 1u) {
        n.push_back(name);
      } else {
        for (unsigned short i = 0; i != v.arraySize; ++i) {
          std::ostringstream nb;
          nb << '[' << i << ']';
          n.push_back(name + nb.str());
        }
      }
    }
  }  // end of VariableDescriptionContainer::appendExternalNames

  VariableDescription& VariableDescriptionContainer::getVariable(
      const std::string& n) {
    for (auto& v : *this) {
      if (v.name == n) {
        return v;
      }
    }
    throw(
        std::runtime_error("VariableDescriptionContainer::getVariable : "
                           "no variable named '" +
                           n + "'"));
  }

  const VariableDescription&
  VariableDescriptionContainer::getVariableByExternalName(
      const std::string& n,
      const std::map<std::string, std::string>& gn,
      const std::map<std::string, std::string>& en) const {
    for (auto& v : *this) {
      if (v.getExternalName(gn, en) == n) {
        return v;
      }
    }
    throw(std::runtime_error(
        "VariableDescriptionContainer::getVariableByExternalName : "
        "no variable with external name '" +
        n + "'"));
  }  // end of VariableDescriptionContainer::getVariableByExternalName

  const VariableDescription& VariableDescriptionContainer::getVariable(
      const std::string& n) const {
    for (const auto& v : *this) {
      if (v.name == n) {
        return v;
      }
    }
    throw(
        std::runtime_error("VariableDescriptionContainer::getVariable : "
                           "no variable named '" +
                           n + "'"));
  }

  VariableDescriptionContainer::~VariableDescriptionContainer() = default;

}  // end of namespace mfront
