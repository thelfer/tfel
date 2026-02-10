/*!
 * \file  mfront/src/MaterialPropertyDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"

namespace mfront {

  bool useQuantities(const MaterialPropertyDescription& mpd) {
    if (mpd.use_qt.has_value()) {
      return *(mpd.use_qt);
    }
    return false;
  }  // end of useQuantities

  void writeBeginningOfMaterialPropertyBody(
      std::ostream& os,
      const MaterialPropertyDescription& mpd,
      const FileDescription& fd,
      const std::string& numeric_type,
      const bool areQuantitiesSupported) {
    const auto use_qt = (areQuantitiesSupported && useQuantities(mpd));
    os << "using namespace std;\n"
       << "using tfel::math::invert_type;\n"
       << "using tfel::math::result_type;\n"
       << "using tfel::math::derivative_type;\n";
    os << "[[maybe_unused]] constexpr auto use_qt = "
       << (use_qt ? "true" : "false") << ";\n";
    writeScalarStandardTypedefs(os, mpd, numeric_type, areQuantitiesSupported);
    writeMaterialLaws(os, mpd.materialLaws);
    writeStaticVariables(os, mpd.staticVars, fd.fileName);
  }  // end of writeBeginningOfMaterialPropertyBody

  void writeScalarStandardTypedefs(std::ostream& os,
                                   const MaterialPropertyDescription& mpd,
                                   const std::string& numeric_type,
                                   const bool areQuantitiesSupported) {
    const auto use_qt =
        (areQuantitiesSupported && useQuantities(mpd)) ? "true" : "false";
    os << "using NumericType [[maybe_unused]] = " << numeric_type << ";\n";
    for (const auto& a : getScalarTypeAliases()) {
      os << "using " << a << " [[maybe_unused]] = "
         << "typename tfel::config::ScalarTypes<" << numeric_type << ", "
         << use_qt << ">::" << a << ";\n";
    }
  }  // end of writeScalarStandardTypedefs

  const VariableDescription&
  MaterialPropertyDescription::getVariableDescription(
      const std::string& n) const {
    if (this->output.name == n) {
      return this->output;
    }
    if (this->inputs.contains(n)) {
      return this->inputs.getVariable(n);
    }
    if (this->parameters.contains(n)) {
      return this->parameters.getVariable(n);
    }
    tfel::raise(
        "MaterialPropertyDescription::getVariableDescription:"
        "no variable named '" +
        n +
        "'.\n"
        "'" +
        n +
        "' is neither:\n"
        "- The output.\n"
        "- An input.\n"
        "- A parameter.");
  }  // end of MaterialPropertyDescription::getVariableDescription

  VariableDescription& MaterialPropertyDescription::getVariableDescription(
      const std::string& n) {
    if (this->output.name == n) {
      return this->output;
    }
    if (this->inputs.contains(n)) {
      return this->inputs.getVariable(n);
    }
    if (this->parameters.contains(n)) {
      return this->parameters.getVariable(n);
    }
    tfel::raise(
        "MaterialPropertyDescription::getVariableDescription:"
        "no variable named '" +
        n +
        "'.\n"
        "'" +
        n +
        "' is neither:\n"
        "- The output.\n"
        "- An input.\n"
        "- A parameter.");
  }  // end of MaterialPropertyDescription::getVariableDescription

  bool MaterialPropertyDescription::isParameterName(
      const std::string& n) const {
    for (const auto& p : this->parameters) {
      if (p.name == n) {
        return true;
      }
    }
    return false;
  }

  bool MaterialPropertyDescription::isInputName(const std::string& n) const {
    for (const auto& p : this->inputs) {
      if (p.name == n) {
        return true;
      }
    }
    return false;
  }

  std::vector<std::string>
  MaterialPropertyDescription::getInputVariablesExternalNames() {
    return this->inputs.getExternalNames();
  }  // end of MaterialPropertyDescription::getInputVariablesExternalNames

  bool MaterialPropertyDescription::isGlossaryNameUsed(
      const std::string& g) const {
    using tfel::glossary::Glossary;
    const auto& glossary = Glossary::getGlossary();
    auto is_used = [](const VariableDescription& v, const std::string& k) {
      if (v.hasGlossaryName()) {
        if (v.getExternalName() == k) {
          return true;
        }
      }
      return false;
    };
    auto is_used2 = [is_used](const VariableDescriptionContainer& c,
                              const std::string& k) {
      for (const auto& v : c) {
        if (is_used(v, k)) {
          return true;
        }
      }
      return false;
    };
    const auto k = glossary.getGlossaryEntry(g).getKey();
    if (is_used(this->output, k)) {
      return true;
    }
    if (is_used2(this->inputs, k)) {
      return true;
    }
    return is_used2(this->parameters, k);
  }  // end of MaterialPropertyDescription::isGlossaryNameUsed

  bool MaterialPropertyDescription::isEntryNameUsed(
      const std::string& e) const {
    auto is_used = [&e](const VariableDescription& v) {
      if (v.getExternalName() == e) {
        return !((v.name == e) && (!v.hasEntryName()));
      }
      return false;
    };
    auto is_used2 = [is_used](const VariableDescriptionContainer& c) {
      for (const auto& v : c) {
        if (is_used(v)) {
          return true;
        }
      }
      return false;
    };
    if (is_used(this->output)) {
      return true;
    }
    if (is_used2(this->inputs)) {
      return true;
    }
    return is_used2(this->parameters);
  }  // end of MaterialPropertyDescription::isEntryNameUsed

  void MaterialPropertyDescription::setGlossaryName(const std::string& n,
                                                    const std::string& g) {
    using tfel::glossary::Glossary;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "MaterialPropertyDescription::setGlossaryName: " + m);
    };
    const auto& glossary = Glossary::getGlossary();
    throw_if(!glossary.contains(g), "'" + g + "' is not a valid glossary name");
    throw_if(this->isGlossaryNameUsed(g),
             "glossary name '" + g + "' already used");
    this->getVariableDescription(n).setGlossaryName(g);
  }  // end of MaterialPropertyDescription::setGlossaryName

  void MaterialPropertyDescription::setEntryName(const std::string& n,
                                                 const std::string& e) {
    using tfel::glossary::Glossary;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "MaterialPropertyDescription::setEntryName: " + m);
    };
    const auto& glossary = Glossary::getGlossary();
    throw_if(glossary.contains(e), "'" + e + "' is a glossary name");
    throw_if(this->isEntryNameUsed(e), "entry name '" + e + "' already used");
    this->getVariableDescription(n).setEntryName(e);
  }  // end of MaterialPropertyDescription::setEntryName

  void MaterialPropertyDescription::appendToIncludes(const std::string& c) {
    this->includes += c;
    if (!this->includes.empty()) {
      if (*(this->includes.rbegin()) != '\n') {
        this->includes += '\n';
      }
    }
  }  // end of MaterialPropertyDescription::appendToIncludes

  void MaterialPropertyDescription::appendToMembers(const std::string& c) {
    this->members += c;
    if (!this->members.empty()) {
      if (*(this->members.rbegin()) != '\n') {
        this->members += '\n';
      }
    }
  }  // end of MaterialPropertyDescription::appendToMembers

  void MaterialPropertyDescription::appendToPrivateCode(const std::string& c) {
    this->privateCode += c;
    if (!this->privateCode.empty()) {
      if (*(this->privateCode.rbegin()) != '\n') {
        this->privateCode += '\n';
      }
    }
  }  // end of MaterialPropertyDescription::appendToPrivateCode

  void MaterialPropertyDescription::appendToSources(const std::string& c) {
    this->sources += c;
    if (!this->sources.empty()) {
      if (*(this->sources.rbegin()) != '\n') {
        this->sources += '\n';
      }
    }
  }  // end of MaterialPropertyDescription::appendToSources

  void MaterialPropertyDescription::addMaterialLaw(const std::string& m) {
    if (std::find(this->materialLaws.begin(), this->materialLaws.end(), m) ==
        this->materialLaws.end()) {
      this->materialLaws.push_back(m);
    }
  }  // end of MaterialPropertyDescription::addMaterialLaw

  void MaterialPropertyDescription::reserveName(const std::string& n) {
    tfel::raise_if(!this->reservedNames.insert(n).second,
                   "MaterialPropertyDescription::reserveName: "
                   "name '" +
                       n + "' already reserved");
  }

  bool MaterialPropertyDescription::isNameReserved(const std::string& n) const {
    return this->reservedNames.count(n) != 0;
  }

  MaterialPropertyDescription::MaterialPropertyDescription() = default;
  MaterialPropertyDescription::MaterialPropertyDescription(
      const MaterialPropertyDescription&) = default;
  MaterialPropertyDescription::MaterialPropertyDescription(
      MaterialPropertyDescription&&) = default;
  MaterialPropertyDescription& MaterialPropertyDescription::operator=(
      const MaterialPropertyDescription&) = default;
  MaterialPropertyDescription& MaterialPropertyDescription::operator=(
      MaterialPropertyDescription&&) = default;
  MaterialPropertyDescription::~MaterialPropertyDescription() = default;

}  // end of namespace mfront
