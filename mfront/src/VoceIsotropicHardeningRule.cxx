/*!
 * \file   mfront/src/VoceIsotropicHardeningRule.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#include <sstream>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/VoceIsotropicHardeningRule.hxx"

namespace mfront::bbrick {

  void VoceIsotropicHardeningRule::initialize(BehaviourDescription& bd,
                                              AbstractBehaviourDSL& dsl,
                                              const std::string& fid,
                                              const std::string& id,
                                              const DataMap& d) {
    using namespace tfel::glossary;
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    auto get_mp = [&dsl, &bd, &fid, &id, &d](const std::string& t,
                                             const std::string& n) {
      const auto ni = IsotropicHardeningRule::getVariableId(n, fid, id);
      if (d.count(n) == 0) {
        tfel::raise(
            "VoceIsotropicHardeningRule::initialize: "
            "material property '" +
            n + "' is not defined");
      }
      auto mp = getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
      declareParameterOrLocalVariable(bd, mp, t, ni);
      return mp;
    };
    mfront::bbrick::check(d, this->getOptions());
    this->R0 = get_mp("stress", "R0");
    this->Rinf = get_mp("stress", "Rinf");
    this->b = get_mp("real", "b");
    //
    const auto Rel = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto dR = "d" + R + "_ddp" + fid;
    bd.reserveName(uh, Rel);
    bd.reserveName(uh, R);
    bd.reserveName(uh, dR);
  }  // end of initialize

  std::vector<OptionDescription> VoceIsotropicHardeningRule::getOptions()
      const {
    std::vector<OptionDescription> opts;
    opts.emplace_back("R0", "Yield strength",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("Rinf", "Ultimate Yield strength",
                      OptionDescription::MATERIALPROPERTY);
    opts.emplace_back("b", "Parameter describing the hardening evolution",
                      OptionDescription::MATERIALPROPERTY);
    return opts;
  }  // end of getOptions

  std::string VoceIsotropicHardeningRule::computeElasticPrediction(
      const std::string& fid, const std::string& id) const {
    const auto Rel = id.empty() ? "Rel" + fid : "Rel" + fid + "_" + id;
    const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
    const auto Rin = IsotropicHardeningRule::getVariableId("Rinf", fid, id);
    const auto bn = IsotropicHardeningRule::getVariableId("b", fid, id);
    return "const auto " + Rel + " = this->" + Rin + "+(this->" + R0n +
           "-this->" + Rin + ")*exp(-(this->" + bn + ")*(this->p" + fid +
           "));\n";
  }  // end of computeElasticPrediction

  std::string VoceIsotropicHardeningRule::computeElasticLimit(
      const std::string& fid, const std::string& id) const {
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto dR = "d" + R + "_ddp" + fid;
    const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
    const auto Rin = IsotropicHardeningRule::getVariableId("Rinf", fid, id);
    const auto bn = IsotropicHardeningRule::getVariableId("b", fid, id);
    return "const auto " + R + " = this->" + Rin + "+(this->" + R0n +
           "-this->" + Rin + ")*exp(-(this->" + bn + ")*(this->p" + fid +
           "+(this->theta)*(this->dp" + fid + ")));\n";
  }  // end of computeElasticLimit

  std::string VoceIsotropicHardeningRule::computeElasticLimitAndDerivative(
      const std::string& fid, const std::string& id) const {
    const auto R = id.empty() ? "R" + fid : "R" + fid + "_" + id;
    const auto dR = "d" + R + "_ddp" + fid;
    const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
    const auto Rin = IsotropicHardeningRule::getVariableId("Rinf", fid, id);
    const auto bn = IsotropicHardeningRule::getVariableId("b", fid, id);
    return "const auto " + R + " = this->" + Rin + "+(this->" + R0n +
           "-this->" + Rin + ")*exp(-(this->" + bn + ")*(this->p" + fid +
           "+(this->theta)*(this->dp" + fid +
           ")));\n"  //
           "const auto " +
           dR +
           " = "
           "-(this->theta)*(this->" +
           bn + ")*(" + R + "-(this->" + Rin + "));\n";
  }  // end of computeElasticLimitAndDerivative

  void VoceIsotropicHardeningRule::endTreatment(BehaviourDescription& bd,
                                                const AbstractBehaviourDSL& dsl,
                                                const std::string& fid,
                                                const std::string& id) const {
    auto mts = getMiddleOfTimeStepModifier(bd);
    // computation of the material properties
    if ((!this->R0.is<BehaviourDescription::ConstantMaterialProperty>()) ||
        (!this->Rinf.is<BehaviourDescription::ConstantMaterialProperty>()) ||
        (!this->b.is<BehaviourDescription::ConstantMaterialProperty>())) {
      constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      CodeBlock i;
      std::ostringstream mps;
      if (!this->R0.is<BehaviourDescription::ConstantMaterialProperty>()) {
        const auto R0n = IsotropicHardeningRule::getVariableId("R0", fid, id);
        mps << "this->" + R0n + " = ";
        dsl.writeMaterialPropertyEvaluation(mps, this->R0, mts);
        mps << ";\n";
      }
      if (!this->Rinf.is<BehaviourDescription::ConstantMaterialProperty>()) {
        const auto Rin = IsotropicHardeningRule::getVariableId("Rinf", fid, id);
        mps << "this->" + Rin + " = ";
        dsl.writeMaterialPropertyEvaluation(mps, this->Rinf, mts);
        mps << ";\n";
      }
      if (!this->b.is<BehaviourDescription::ConstantMaterialProperty>()) {
        const auto bn = IsotropicHardeningRule::getVariableId("b", fid, id);
        mps << "this->" + bn + " = ";
        dsl.writeMaterialPropertyEvaluation(mps, this->b, mts);
        mps << ";\n";
      }
      i.code += mps.str();
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  VoceIsotropicHardeningRule::~VoceIsotropicHardeningRule() = default;

}  // end of namespace mfront::bbrick
