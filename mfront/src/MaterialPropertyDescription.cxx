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
      const std::string_view numeric_type,
      const bool areQuantitiesSupported) {
    const auto use_qt = (areQuantitiesSupported && useQuantities(mpd));
    os << "using namespace std;\n"
       << "using tfel::math::invert_type;\n"
       << "using tfel::math::result_type;\n"
       << "using tfel::math::derivative_type;\n";
    if (use_qt) {
      os << "using PhysicalConstants [[maybe_unused]] = "
         << "tfel::PhysicalConstants<" << numeric_type << ", true>;\n";
    } else {
      os << "using PhysicalConstants [[maybe_unused]] = "
         << "tfel::PhysicalConstants<" << numeric_type << ", false>;\n";
    }
    os << "[[maybe_unused]] auto min = [](const auto a, const auto b) "
       << "{ return a < b ? a : b; };\n"
       << "[[maybe_unused]] auto max = [](const auto a, const auto b) "
       << "{ return a > b ? a : b; };\n";
    writeScalarStandardTypedefs(os, mpd, numeric_type, areQuantitiesSupported);
    writeMaterialLaws(os, mpd.materialLaws);
    writeStaticVariables(os, mpd.staticVars, fd.fileName);
  }  // end of writeBeginningOfMaterialPropertyBody

  void writeScalarStandardTypedefs(std::ostream& os,
                                   const MaterialPropertyDescription& mpd,
                                   const std::string_view numeric_type,
                                   const bool areQuantitiesSupported) {
    const auto use_qt =
        (areQuantitiesSupported && useQuantities(mpd)) ? "true" : "false";
    for (const auto& a : getScalarTypeAliases()) {
      os << "using " << a << " [[maybe_unused]] = "
         << "typename tfel::config::ScalarTypes<" << numeric_type << ", "
         << use_qt << ">::" << a << ";\n";
    }
  }  // end of writeScalarStandardTypedefs

  MaterialPropertyDescription::MaterialPropertyDescription() = default;
  MaterialPropertyDescription::MaterialPropertyDescription(
      const MaterialPropertyDescription&) = default;
  MaterialPropertyDescription::MaterialPropertyDescription(
      MaterialPropertyDescription&&) = default;
  MaterialPropertyDescription& MaterialPropertyDescription::operator=(
      const MaterialPropertyDescription&) = default;
  MaterialPropertyDescription& MaterialPropertyDescription::operator=(
      MaterialPropertyDescription&&) = default;

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
  }  // end of getVariableDescription

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
  }  // end of getVariableDescription

  const VariableDescription&
  MaterialPropertyDescription::getVariableDescriptionByExternalName(
      const std::string& n) const {
    auto contains = [&n](const VariableDescriptionContainer& vc) {
      for (const auto& v : vc) {
        if (v.getExternalName() == n) {
          return true;
        }
      }
      return false;
    };
    if (this->output.getExternalName() == n) {
      return this->output;
    }
    if (contains(this->inputs)) {
      return this->inputs.getVariableByExternalName(n);
    }
    if (contains(this->parameters)) {
      return this->parameters.getVariableByExternalName(n);
    }
    tfel::raise(
        "MaterialPropertyDescription::getVariableDescriptionByExternalName: "
        "no variable with external name '" +
        n +
        "' found. "
        "Such variable is *not*:\n"
        "- the output\n"
        "- an input\n"
        "- a parameter");
  }  // end of getVariableDescriptionByExternalName

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
  }  // end of getInputVariablesExternalNames

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
  }  // end of isGlossaryNameUsed

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
  }  // end of isEntryNameUsed

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
  }  // end of setGlossaryName

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
  }  // end of setEntryName

  void MaterialPropertyDescription::appendToIncludes(const std::string& c) {
    this->includes += c;
    if (!this->includes.empty()) {
      if (*(this->includes.rbegin()) != '\n') {
        this->includes += '\n';
      }
    }
  }  // end of appendToIncludes

  void MaterialPropertyDescription::appendToMembers(const std::string& c) {
    this->members += c;
    if (!this->members.empty()) {
      if (*(this->members.rbegin()) != '\n') {
        this->members += '\n';
      }
    }
  }  // end of appendToMembers

  void MaterialPropertyDescription::appendToPrivateCode(const std::string& c) {
    this->privateCode += c;
    if (!this->privateCode.empty()) {
      if (*(this->privateCode.rbegin()) != '\n') {
        this->privateCode += '\n';
      }
    }
  }  // end of appendToPrivateCode

  void MaterialPropertyDescription::appendToSources(const std::string& c) {
    this->sources += c;
    if (!this->sources.empty()) {
      if (*(this->sources.rbegin()) != '\n') {
        this->sources += '\n';
      }
    }
  }  // end of appendToSources

  void MaterialPropertyDescription::addMaterialLaw(const std::string& m) {
    if (std::find(this->materialLaws.begin(), this->materialLaws.end(), m) ==
        this->materialLaws.end()) {
      this->materialLaws.push_back(m);
    }
  }  // end of addMaterialLaw

  void MaterialPropertyDescription::reserveName(const std::string& n) {
    tfel::raise_if(!this->reservedNames.insert(n).second,
                   "MaterialPropertyDescription::reserveName: "
                   "name '" +
                       n + "' already reserved");
  }

  bool MaterialPropertyDescription::isNameReserved(const std::string& n) const {
    return this->reservedNames.count(n) != 0;
  }

  void
  MaterialPropertyDescription::checkAndCompletePhysicalBoundsDeclaration() {
    if (this->hasUnitSystem()) {
      const auto& s = this->getUnitSystem();
      mfront::checkAndCompletePhysicalBoundsDeclaration(this->output, s);
      mfront::checkAndCompletePhysicalBoundsDeclaration(this->inputs, s);
      mfront::checkAndCompletePhysicalBoundsDeclaration(this->parameters, s);
    }
  }  // end of checkAndCompletePhysicalBoundsDeclaration

  MaterialPropertyDescription::~MaterialPropertyDescription() = default;

  std::string getParametersFileName(const MaterialPropertyDescription& mpd) {
    if (mpd.material.empty()) {
      return mpd.className + "-parameters.txt";
    }
    return mpd.material + "_" + mpd.className + "-parameters.txt";
  }  // end of getParametersFileName

}  // end of namespace mfront
