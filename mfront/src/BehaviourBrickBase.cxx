/*!
 * \file  BehaviourBrickBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21 oct. 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include <algorithm>
#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/BehaviourBrickBase.hxx"

namespace mfront {

  BehaviourBrickBase::BehaviourBrickBase(AbstractBehaviourDSL& dsl_,
                                         BehaviourDescription& bd_)
      : dsl(dsl_), bd(bd_) {}  // end of BehaviourBrickBase::BehaviourBrickBase

  std::pair<bool, BehaviourBrickBase::tokens_iterator>
  BehaviourBrickBase::treatKeyword(const std::string&,
                                   tokens_iterator& p,
                                   const tokens_iterator) {
    return {false, p};
  }  // end of BehaviourBrickBase::treatKeyword

  void BehaviourBrickBase::addRequirements(bbrick::RequirementManager&,
                                           const Hypothesis) const {
  }  // end of BehaviourBrickBase::addRequirements

  void BehaviourBrickBase::checkThatParameterHasNoValue(
      const Parameter& p) const {
    tfel::raise_if(!p.second.empty(),
                   "BehaviourBrickBase::checkThatParameterHasNoValue: "
                   "parameter '" +
                       p.first + "' shall not have any value");
  }  // end of BehaviourBrickBase::checkThatParameterHasNoValue

  void BehaviourBrickBase::addMaterialPropertyIfNotDefined(
      const std::string& t,
      const std::string& n,
      const GlossaryEntry& g,
      const unsigned short s) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "BehaviourBrickBase::addMaterialPropertyIfNotDefined: " + m);
    };
    // treating material properties
    const auto b = this->bd.checkVariableExistence(n);
    if (b.first) {
      throw_if(!b.second, "variable '" + n +
                              "' is not declared for all specialisations "
                              "of the behaviour");
      auto r = this->bd.checkVariableExistence(n, "Parameter", false);
      if (r.first) {
        throw_if(!r.second, "parameter '" + n +
                                "' is not declared for all specialisations "
                                "of the behaviour");
      } else {
        r = this->bd.checkVariableExistence(n, "MaterialProperty", false);
        throw_if(!r.first, "variable '" + n +
                               "' is neither declared as a parameter nor "
                               "a material property");
        throw_if(!r.second, "material property '" + n +
                                "' is not declared"
                                "for all specialisations of the behaviour");
      }
      this->bd.checkVariableGlossaryName(n, g);
    } else {
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      this->bd.addMaterialProperty(h, {t, n, s, 0u});
      this->bd.setGlossaryName(h, n, g);
    }
  }  // end of BehaviourBrickBase::addMaterialPropertyIfNotDefined

  void BehaviourBrickBase::addMaterialPropertyIfNotDefined(
      const std::string& t,
      const std::string& n,
      const std::string& e,
      const unsigned short s) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "BehaviourBrickBase::addMaterialPropertyIfNotDefined: " + m);
    };
    // treating material properties
    const auto b = this->bd.checkVariableExistence(n);
    if (b.first) {
      throw_if(!b.second, "variable '" + n +
                              "' is not declared for all specialisations "
                              "of the behaviour");
      auto r = this->bd.checkVariableExistence(n, "Parameter", false);
      if (r.first) {
        throw_if(!r.second, "parameter '" + n +
                                "' is not declared for all specialisations "
                                "of the behaviour");
      } else {
        r = this->bd.checkVariableExistence(n, "MaterialProperty", false);
        throw_if(!r.first, "variable '" + n +
                               "' is neither declared as a parameter nor "
                               "a material property");
        throw_if(!r.second, "material property '" + n +
                                "' is not declared"
                                "for all specialisations of the behaviour");
      }
      this->bd.checkVariableEntryName(n, e);
    } else {
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      this->bd.addMaterialProperty(h, {t, n, s, 0u});
      this->bd.setEntryName(h, n, e);
    }
  }  // end of BehaviourBrickBase::addMaterialPropertyIfNotDefined

  void BehaviourBrickBase::addExternalStateVariable(
      const std::string& t,
      const std::string& n,
      const unsigned short s) const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    VariableDescription v(t, n, s, 0u);
    this->bd.addExternalStateVariable(h, v);
  }  // end of BehaviourBrickBase::addExternalStateVariable

  void BehaviourBrickBase::addExternalStateVariable(
      const std::string& t,
      const std::string& n,
      const GlossaryEntry& g,
      const unsigned short s) const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    VariableDescription v(t, n, s, 0u);
    this->bd.addExternalStateVariable(h, v);
    this->bd.setGlossaryName(h, n, g);
  }  // end of BehaviourBrickBase::addExternalStateVariable

  void BehaviourBrickBase::addExternalStateVariable(
      const std::string& t,
      const std::string& n,
      const std::string& e,
      const unsigned short s) const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    VariableDescription v(t, n, s, 0u);
    this->bd.addExternalStateVariable(h, v);
    this->bd.setEntryName(h, n, e);
  }  // end of BehaviourBrickBase::addExternalStateVariable

  void BehaviourBrickBase::checkOptionsNames(const DataMap& d,
                                             const std::vector<std::string>& k,
                                             const std::string& n) {
    for (const auto& de : d) {
      tfel::raise_if(std::find(k.begin(), k.end(), de.first) == k.end(),
                     "BehaviourBrickBase::checkOptionsNames: "
                     "brick '" +
                         n + "' does not expect option '" + de.first + "'");
    }
  }  // end of BehaviourBrickBase::checkOptionsNames

  void BehaviourBrickBase::addLocalVariable(const std::string& t,
                                            const std::string& n,
                                            const unsigned short s) const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    VariableDescription v(t, n, s, 0u);
    this->bd.addLocalVariable(h, v);
  }  // end of BehaviourBrickBase::addLocalVariable

  void BehaviourBrickBase::addParameter(const std::string& n,
                                        const GlossaryEntry& g,
                                        const double p) const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    VariableDescription v("real", n, 1u, 0u);
    this->bd.addParameter(h, v);
    this->bd.setGlossaryName(h, n, g);
    this->bd.setParameterDefaultValue(h, n, p);
  }  // end of BehaviourBrickBase::addParameter

  void BehaviourBrickBase::addParameter(const std::string& n,
                                        const GlossaryEntry& g,
                                        const unsigned short s,
                                        const double p) const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    VariableDescription v("real", n, s, 0u);
    this->bd.addParameter(h, v);
    this->bd.setGlossaryName(h, n, g);
    for (unsigned short i = 0; i != s; ++i) {
      this->bd.setParameterDefaultValue(h, n, i, p);
    }
  }  // end of BehaviourBrickBase::addParameter

  void BehaviourBrickBase::addParameter(const std::string& n,
                                        const GlossaryEntry& g,
                                        const unsigned short s,
                                        const std::vector<double>& p) const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    VariableDescription v("real", n, 1u, 0u);
    this->bd.addParameter(h, v);
    this->bd.setGlossaryName(h, n, g);
    for (unsigned short i = 0; i != s; ++i) {
      this->bd.setParameterDefaultValue(h, n, i, p[i]);
    }
  }  // end of BehaviourBrickBase::addParameter

  void BehaviourBrickBase::addParameter(const std::string& n,
                                        const std::string& e,
                                        const double p) const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    VariableDescription v("real", n, 1u, 0u);
    this->bd.addParameter(h, v);
    this->bd.setEntryName(h, n, e);
    this->bd.setParameterDefaultValue(h, n, p);
  }  // end of BehaviourBrickBase::addParameter

  void BehaviourBrickBase::addParameter(const std::string& n,
                                        const std::string& e,
                                        const unsigned short s,
                                        const double p) const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    VariableDescription v("real", n, s, 0u);
    this->bd.addParameter(h, v);
    this->bd.setEntryName(h, n, e);
    for (unsigned short i = 0; i != s; ++i) {
      this->bd.setParameterDefaultValue(h, n, i, p);
    }
  }  // end of BehaviourBrickBase::addParameter

  void BehaviourBrickBase::addParameter(const std::string& n,
                                        const std::string& e,
                                        const unsigned short s,
                                        const std::vector<double>& p) const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    tfel::raise_if(p.size() != s,
                   "BehaviourBrickBase::addParameter: "
                   "invalid number of default parameters values");
    VariableDescription v("real", n, s, 0u);
    this->bd.addParameter(h, v);
    this->bd.setEntryName(h, n, e);
    for (unsigned short i = 0; i != s; ++i) {
      this->bd.setParameterDefaultValue(h, n, i, p[i]);
    }
  }  // end of BehaviourBrickBase::addParameter

  BehaviourBrickBase::~BehaviourBrickBase() = default;

}  // end of namespace mfront
