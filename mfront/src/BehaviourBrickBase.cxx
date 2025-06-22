/*!
 * \file  BehaviourBrickBase.cxx
 * \brief
 * \author Helfer Thomas
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

  void BehaviourBrickBase::addRequirements(bbrick::RequirementManager&,
                                           const Hypothesis) const {
  }  // end of BehaviourBrickBase::addRequirements

  void BehaviourBrickBase::checkThatParameterHasNoValue(
      const Parameter& p) const {
    if (!p.second.empty()) {
      throw(std::runtime_error(
          "BehaviourBrickBase::checkThatParameterHasNoValue: "
          "parameter '" +
          p.first + "' shall not have any value"));
    }
  }  // end of BehaviourBrickBase::checkThatParameterHasNoValue

  void BehaviourBrickBase::addMaterialPropertyIfNotDefined(
      const std::string& t,
      const std::string& n,
      const GlossaryEntry& g) const {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error(
            "BehaviourBrickBase::addMaterialPropertyIfNotDefined: " + m));
      }
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
      this->bd.addMaterialProperty(h, {t, n, 1u, 0u});
      this->bd.setGlossaryName(h, n, g);
    }
  }  // end of BehaviourBrickBase::addMaterialPropertyIfNotDefined

  void BehaviourBrickBase::addMaterialPropertyIfNotDefined(
      const std::string& t, const std::string& n, const std::string& e) const {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error(
            "BehaviourBrickBase::addMaterialPropertyIfNotDefined: " + m));
      }
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
      this->bd.addMaterialProperty(h, {t, n, 1u, 0u});
      this->bd.setEntryName(h, n, e);
    }
  }  // end of BehaviourBrickBase::addMaterialPropertyIfNotDefined

  void BehaviourBrickBase::checkOptionsNames(const DataMap& d,
                                             const std::vector<std::string>& k,
                                             const std::string& n) {
    for (const auto& de : d) {
      if (std::find(k.begin(), k.end(), de.first) == k.end()) {
        throw(std::runtime_error(
            "BehaviourBrickBase::checkOptionsNames: "
            "brick '" +
            n + "' does not expect option '" + de.first + "'"));
      }
    }
  }  // end of BehaviourBrickBase::checkOptionsNames

  void BehaviourBrickBase::addLocalVariable(const std::string& t,
                                            const std::string& n) const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    VariableDescription v(t, n, 1u, 0u);
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
                                        const std::string& e,
                                        const double p) const {
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    VariableDescription v("real", n, 1u, 0u);
    this->bd.addParameter(h, v);
    this->bd.setEntryName(h, n, e);
    this->bd.setParameterDefaultValue(h, n, p);
  }  // end of BehaviourBrickBase::addParameter

  BehaviourBrickBase::~BehaviourBrickBase() = default;

}  // end of namespace mfront
