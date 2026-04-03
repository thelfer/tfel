/*!
 * \file   mfront/src/LinearIsotropicHardeningRule.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/LinearIsotropicHardeningRule.hxx"

namespace mfront::bbrick {

  void LinearIsotropicHardeningRule::initialize(BehaviourDescription& bd,
                                                AbstractBehaviourDSL& dsl,
                                                const std::string& fid,
                                                const std::string& id,
                                                const DataMap& d) {
    using namespace tfel::glossary;
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto get_mp = [&dsl, &bd, &fid, &id, &d](const std::string& n) {
      const auto ni = IsotropicHardeningRule::getVariableId(n, fid, id);
      auto mp = getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
      declareParameterOrLocalVariable(bd, mp, "stress", ni);
      return mp;
    };
    mfront::bbrick::check(d, this->getOptions());
    tfel::raise_if((d.count("R0") == 0) && (d.count("H") == 0),
                   "LinearIsotropicHardeningRule::initialize: "
                   "at least 'R0' or 'H' must be defined");
    if (d.count("R0") != 0) {
      this->R0 = get_mp("R0");
    }
    if (d.count("H") != 0) {
      this->H = get_mp("H");
    }
    const auto Rel = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto dR = "d" + R + "_dp" + fid;
    bd.reserveName(uh, Rel);
    bd.reserveName(uh, R);
    bd.reserveName(uh, dR);
  }  // end of initialize

  std::vector<OptionDescription> LinearIsotropicHardeningRule::getOptions()
      const {
    std::vector<OptionDescription> opts;
    opts.emplace_back("R0", "Yield strength",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("H", "Hardening slope",
                      OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of getOptions

  std::string LinearIsotropicHardeningRule::computeElasticPrediction(
      const BehaviourDescription&,
      const std::string& fid,
      const std::string& id) const {
    const auto Rel = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
    const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
    const auto Hn = IsotropicHardeningRule::getVariableId("H", fid, id);
    tfel::raise_if((this->R0.empty()) && (this->H.empty()),
                   "LinearIsotropicHardeningRule::computeElasticPrediction: "
                   "at least 'R0' or 'H' must be defined");
    auto c = "const auto " + Rel + " = ";
    if (!this->R0.empty()) {
      c += "this->" + R0n;
    }
    if (!this->H.empty()) {
      if (!this->R0.empty()) {
        c += " + ";
      }
      c += "(this->" + Hn + ")*(this->p" + fid + ")";
    }
    return c + ";\n";
  }  // end of computeElasticPrediction

  std::string LinearIsotropicHardeningRule::computeElasticLimit(
      const BehaviourDescription&,
      const std::string& fid,
      const std::string& id) const {
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
    const auto Hn = IsotropicHardeningRule::getVariableId("H", fid, id);
    const auto pn = "p" + fid;
    tfel::raise_if((this->R0.empty()) && (this->H.empty()),
                   "LinearIsotropicHardeningRule::computeElastic: "
                   "at least 'R0' or 'H' must be defined");
    auto c = "const auto " + R + " = ";
    if (!this->R0.empty()) {
      c += "this->" + R0n;
    }
    if (!this->H.empty()) {
      if (!this->R0.empty()) {
        c += " + ";
      }
      c += "(this->" + Hn + ")*";
      c += "(this->" + pn + "+(this->theta)*(this->d" + pn + "))";
    }
    return c + ";\n";
  }  // end of computeElasticLimit

  std::string LinearIsotropicHardeningRule::computeElasticLimitAndDerivative(
      const BehaviourDescription&,
      const std::string& fid,
      const std::string& id) const {
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto dR = "d" + R + "_dp" + fid;
    const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
    const auto Hn = IsotropicHardeningRule::getVariableId("H", fid, id);
    const auto pn = "p" + fid;
    tfel::raise_if(
        (this->R0.empty()) && (this->H.empty()),
        "LinearIsotropicHardeningRule::computeElasticLimitAndDerivative: "
        "at least 'R0' or 'H' must be defined");
    auto c = "const auto " + R + " = ";
    if (!this->R0.empty()) {
      c += "this->" + R0n;
    }
    if (!this->H.empty()) {
      if (!this->R0.empty()) {
        c += " + ";
      }
      c += "(this->" + Hn + ")*";
      c += "(this->" + pn + "+(this->theta)*(this->d" + pn + "));\n";
      c += "const auto " + dR + " = this->" + Hn + ";\n";
    } else {
      c += ";\n";
      c += "const auto " + dR + " = stress(0);\n";
    }
    return c;
  }  // end of computeElasticLimitAndDerivative

  void LinearIsotropicHardeningRule::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& id) const {
    auto mts = getMiddleOfTimeStepModifier(bd);
    // computation of the material properties
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    CodeBlock i;
    std::ostringstream mps;
    if ((!this->R0.empty()) &&
        (!this->R0.is<BehaviourDescription::ConstantMaterialProperty>())) {
      const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
      mps << "this->" + R0n + " = ";
      dsl.writeMaterialPropertyEvaluation(mps, this->R0, mts);
      mps << ";\n";
    }
    if ((!this->H.empty()) &&
        (!this->H.is<BehaviourDescription::ConstantMaterialProperty>())) {
      const auto Hn = IsotropicHardeningRule::getVariableId("H", fid, id);
      mps << "this->" + Hn + " = ";
      dsl.writeMaterialPropertyEvaluation(mps, this->H, mts);
      mps << ";\n";
    }
    i.code += mps.str();
    if (!i.code.empty()) {
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  LinearIsotropicHardeningRule::~LinearIsotropicHardeningRule() = default;

}  // end of namespace mfront::bbrick
