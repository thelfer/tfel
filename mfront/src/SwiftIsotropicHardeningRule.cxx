/*!
 * \file   mfront/src/SwiftIsotropicHardeningRule.cxx
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
#include "MFront/BehaviourBrick/SwiftIsotropicHardeningRule.hxx"

namespace mfront::bbrick {

  void SwiftIsotropicHardeningRule::initialize(BehaviourDescription& bd,
                                               AbstractBehaviourDSL& dsl,
                                               const std::string& fid,
                                               const std::string& id,
                                               const DataMap& d) {
    using namespace tfel::glossary;
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto get_mp = [&dsl, &bd, &fid, &id, &d](const std::string& mpn,
                                             const std::string& t,
                                             const std::string& vn) {
      if (d.count(mpn) == 0) {
        tfel::raise(
            "SwiftIsotropicHardeningRule::initialize: "
            "material property '" +
            mpn + "' is not defined");
      }
      const auto ni = IsotropicHardeningRule::getVariableId(vn, fid, id);
      auto mp = getBehaviourDescriptionMaterialProperty(dsl, mpn, d.at(mpn));
      declareParameterOrLocalVariable(bd, mp, t, ni);
      return mp;
    };
    mfront::bbrick::check(d, this->getOptions());
    this->R0 = get_mp("R0", "stress", "R0");
    this->p0 = get_mp("p0", "strain", "p0");
    this->n = get_mp("n", "real", "E");
    const auto Rel = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto dR = "d" + R + "_dp" + fid;
    bd.reserveName(uh, Rel);
    bd.reserveName(uh, R);
    bd.reserveName(uh, dR);
  }  // end of initialize

  std::vector<OptionDescription> SwiftIsotropicHardeningRule::getOptions()
      const {
    std::vector<OptionDescription> opts;
    opts.emplace_back("R0", "Yield strength",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("p0", "Small numerical parameter",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("n", "Swift exponent",
                      OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of getOptions

  std::string SwiftIsotropicHardeningRule::computeElasticPrediction(
      const BehaviourDescription&,
      const std::string& fid,
      const std::string& id) const {
    const auto Rel = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
    const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
    const auto p0n = IsotropicHardeningRule::getVariableId("p0", fid, id);
    const auto nn = IsotropicHardeningRule::getVariableId("E", fid, id);
    const auto pn = "p" + fid;
    auto c = "const auto " + Rel + " = ";
    c += "(this->" + R0n + ")*";
    c += "pow((this->" + pn + "+this->" + p0n + ")/(this->" + p0n + "),this->" +
         nn + ");\n";
    return c;
  }  // end of computeElasticPrediction

  std::string SwiftIsotropicHardeningRule::computeElasticLimit(
      const BehaviourDescription&,
      const std::string& fid,
      const std::string& id) const {
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
    const auto p0n = IsotropicHardeningRule::getVariableId("p0", fid, id);
    const auto nn = IsotropicHardeningRule::getVariableId("E", fid, id);
    const auto pn = "p" + fid;
    auto c = "const auto " + R + " = (this->" + pn + "+(this->theta)*(this->d" +
             pn + ")> strain(0)) ? (this->" + R0n + ")*";
    c += "pow((this->" + pn + "+(this->theta)*(this->d" + pn + ")+this->" +
         p0n + ")/(this->" + p0n + "),this->" + nn + ") : this->" + R0n + ";\n";
    return c;
  }  // end of computeElasticLimit

  std::string SwiftIsotropicHardeningRule::computeElasticLimitAndDerivative(
      const BehaviourDescription&,
      const std::string& fid,
      const std::string& id) const {
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto dR = "d" + R + "_dp" + fid;
    const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
    const auto p0n = IsotropicHardeningRule::getVariableId("p0", fid, id);
    const auto nn = IsotropicHardeningRule::getVariableId("E", fid, id);
    const auto pn = "p" + fid;
    auto c = "const auto " + R + " = (this->" + pn + "+(this->theta)*(this->d" +
             pn + ") > strain(0)) ? (this->" + R0n + ")*";
    c += "pow((this->" + pn + "+(this->theta)*(this->d" + pn + ")+this->" +
         p0n + ")/(this->" + p0n + "),this->" + nn + ") : this->" + R0n + ";\n";
    c += "const auto " + dR + " = ";
    c += "(this->" + nn + ") * " + R + "/(std::max(this->" + pn +
         "+(this->theta)*(this->d" + pn + ")+this->" + p0n + ",this->" + p0n +
         "));\n";
    return c;
  }  // end of computeElasticLimitAndDerivative

  void SwiftIsotropicHardeningRule::endTreatment(
      BehaviourDescription& bd,
      const AbstractBehaviourDSL& dsl,
      const std::string& fid,
      const std::string& id) const {
    auto mts = getMiddleOfTimeStepModifier(bd);
    // computation of the material properties
    std::ostringstream mps;
    if (!this->R0.is<BehaviourDescription::ConstantMaterialProperty>()) {
      const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
      mps << "this->" + R0n + " = ";
      dsl.writeMaterialPropertyEvaluation(mps, this->R0, mts);
      mps << ";\n";
    }
    if (!this->p0.is<BehaviourDescription::ConstantMaterialProperty>()) {
      const auto p0n = IsotropicHardeningRule::getVariableId("p0", fid, id);
      mps << "this->" + p0n + " = ";
      dsl.writeMaterialPropertyEvaluation(mps, this->p0, mts);
      mps << ";\n";
    }
    if (!this->n.is<BehaviourDescription::ConstantMaterialProperty>()) {
      const auto nn = IsotropicHardeningRule::getVariableId("E", fid, id);
      mps << "this->" + nn + " = ";
      dsl.writeMaterialPropertyEvaluation(mps, this->n, mts);
      mps << ";\n";
    }
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    CodeBlock i;
    i.code += mps.str();
    if (!i.code.empty()) {
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  SwiftIsotropicHardeningRule::~SwiftIsotropicHardeningRule() = default;

}  // end of namespace mfront::bbrick
